#include "stdafx.h"
#include "My_Naming_Service.h"
#include "FileManager_impl.h"

using namespace std;

My_Naming_Service* StartNameService();
ACE_THR_FUNC_RETURN svc_fn(void * arg);

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");
#if 0
	int _argc = 6;
	char * _argv[6] = {
		"-ORBEndPoint",
		"iiop://:32001",
		"-ORBInitRef",
		"NameService=corbaloc:iiop:box:9999/NameService", // 0,
		"-ORBDottedDecimalAddresses",
		"1"
	};
	//_argv[3] = new char[256];
	//ACE_OS::sprintf(_argv[3], "NameService=corbaloc:iiop:%s:9999/NameService", box_address.c_str());
	//               -ORBInitRef NameService=corbaloc:iiop:box:9999/NameService -ORBDottedDecimalAddresses 1
#endif
#if 1
	CORBA::ORBid orbid = CORBA::string_dup("MAINORB");
	CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, orbid);
#else
	CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
#endif

	CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
	PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj.in());
	PortableServer::POAManager_var poaManager = rootPOA->the_POAManager();
	poaManager->activate();

	cout << "resolving naming service" << endl;
	CosNaming::NamingContext_var nc = CosNaming::NamingContext::_nil();
	try {
		ACE_Time_Value timeout(0, 500000);
		CORBA::Object_var onc = orb->resolve_initial_references("NameService", &timeout);
		if (CORBA::is_nil(onc.in())) {
			cerr << "could not resolve existing NameService (nil), starting new one" << endl;
			My_Naming_Service * ns = StartNameService();
			nc = CosNaming::NamingContext::_duplicate(ns->operator->());
		}
		else {
			nc = CosNaming::NamingContext::_narrow(onc.in());
			cerr << "resolved existing NameService" << endl;
		}
	}
	catch (CORBA::Exception &) {
		cerr << "could not resolve existing NameService (exeption), starting new one" << endl;
		My_Naming_Service * ns = StartNameService();
		if (0 == ns)
			return -1;
		nc = CosNaming::NamingContext::_duplicate(ns->operator->());
	}

	// Registering FileManager
	try {
#if 1
		CosNaming::Name name(1);
		name.length(1);
		name[0].id = CORBA::string_dup("FileManager");
#else // вылет
		CosNaming::Name name(2);
		name.length(2);
		name[0].id = CORBA::string_dup("FS");
		name[1].id = CORBA::string_dup("FileManager");
#endif
		cout << "Binding FileManager to the Naming Service ... " << endl;
		FileManager_impl * file_manager = new FileManager_impl;
		cout << "nc " << nc << endl;
		CORBA::Object_var obj = file_manager->_this();
		cout << "obj " << obj << endl;
		nc->rebind(name, obj.in());
		cout << "done.\n";

		bool retval(false);
		FileSystem_impl* fs = new FileSystem_impl(retval, rootPOA.in(), "");

		if (!fs || !retval)
			cerr << "Unable to allocate file system on \"\"" << endl;

		FS::Properties fileSystemProperties;
		fileSystemProperties.length(1);
		fileSystemProperties[0].id = "BASE_PATH";
		fs->query(fileSystemProperties);
		const char* fsBase;
		fileSystemProperties[0].value >>= fsBase;
		cout << "FileSystem \"" << fsBase << "\" created" << endl;

		file_manager->mount("/", fs->_this());
		cout << "FileSystem \"" << fsBase << "\" mounted under \"" << "/" << "\"" << endl;
	}
	catch (const FS::InvalidFileName & e) {
		cerr << "FS exception: " << e.msg << endl;
		return 1;
	}

	cout << "run the ORB event loop" << endl;
#if 1
	try {
		orb->run();
		orb->destroy();
	}
	catch (...) {
		cerr << "ORB::run() raised exception" << endl;
	}
#else
	if (-1 == ThreadManager.spawn(ORBThreadFunc)) {
		return E_FAIL;
	}
#endif
	return 0;
}

My_Naming_Service* StartNameService()
{
	cerr << "Start Naming Service: " << endl;
	const char *_argv[] = {
		"-m", "0", "-ORBEndPoint", "iiop://:9999", "-ORBDottedDecimalAddresses", "1"
	};

	int _argc = sizeof(_argv) / sizeof(_argv[0]);

	CORBA::ORBid orbid = CORBA::string_dup("NSORB");
	My_Naming_Service * ns = new My_Naming_Service(0, orbid); // (orb.in());
	if (ns->init(_argc, (char **)_argv) == -1) {
		cerr << "Failed to start Naming Service locally." << endl;
		return 0;
	}
	ACE_Sched_Priority prio = ACE_DEFAULT_THREAD_PRIORITY;
	ACE_Thread_Manager::instance()->spawn(svc_fn, ns, THR_NEW_LWP | THR_JOINABLE, 0, 0, prio);
	ACE_Time_Value delay(0, 10000);
	ACE_OS::sleep(delay);
	cerr << "ns done" << endl;
	return ns;
}

ACE_THR_FUNC_RETURN svc_fn(void * arg)
{
	My_Naming_Service* ns = (My_Naming_Service *)arg;
	try {
		ns->run();
	}
	catch (...) {
		; // ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "NamingService");
		return (ACE_THR_FUNC_RETURN)1;
	}
	cerr << "ERROR-----------------------" << endl;
	return NULL;
}
