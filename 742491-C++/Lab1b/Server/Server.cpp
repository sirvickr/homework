#include "stdafx.h"
#include "TcpSrv.h"
#include "CryptoAPI.h"
#include "My_Naming_Service.h"
#include "agentI.h"

using namespace std;

BOOL ValidOptions(int argc, char *argv[]);
BOOL WINAPI CtrlHandler(DWORD dwEvent);

void RunORB(int argc, char *argv[]);
My_Naming_Service* StartNameService();
ACE_THR_FUNC_RETURN svc_fn(void * arg);

char *g_Port = DEFAULT_PORT;
BOOL g_bVerbose = FALSE;
TcpSrv* server = NULL;

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");
#if 1
		RunORB(argc, argv);
#else
	if (ValidOptions(argc, argv)) {
		if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
			int err = GetLastError();
			cerr << "SetConsoleCtrlHandler() failed to install console handler: " << err << endl;
			return err;
		}
		server = new TcpSrv(g_Port, g_bVerbose);
		if (server) {
			server->Run();
		}
	}
	SetConsoleCtrlHandler(CtrlHandler, FALSE);
#endif
	return 0;
}

BOOL ValidOptions(int argc, char *argv[])
{
	BOOL bRet = TRUE;

	for (int i = 1; i < argc; i++) {
		if ((argv[i][0] == '-') || (argv[i][0] == '/')) {
			switch (tolower(argv[i][1])) {
			case 'e':
				if (strlen(argv[i]) > 3)
					g_Port = &argv[i][3];
				break;

			case 'v':
				g_bVerbose = TRUE;
				break;

			case '?':
				cout << "Usage:\n  iocpserver [-p:port] [-v] [-?]\n";
				cout << "  -e:port\tSpecify echoing port number\n";
				cout << "  -v\t\tVerbose\n";
				cout << "  -?\t\tDisplay this help" << endl;
				bRet = FALSE;
				break;

			}
		}
	}
	cout << "accepting on " << g_Port << endl;

	return bRet;
}

BOOL WINAPI CtrlHandler(DWORD dwEvent)
{
	SOCKET sockTemp = INVALID_SOCKET;

	switch (dwEvent) {
	case CTRL_BREAK_EVENT:
		if (server) {
			server->Restart();
		}
	case CTRL_C_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_CLOSE_EVENT:
		if (server) {
			server->Shutdown();
		}
		break;

	default:
		// unknown type--better pass it on.
		return(FALSE);
	}
	return TRUE;
}

void RunORB(int argc, char *argv[])
{
	CORBA::ORBid orbid = CORBA::string_dup("MAINORB");
	CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, orbid);
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
			return;
		nc = CosNaming::NamingContext::_duplicate(ns->operator->());
	}

	// регистрация агента
	try {
		CosNaming::Name name(1);
		name.length(1);
		name[0].id = CORBA::string_dup("SysInfoAgent");

		cout << "Binding Agent to the Naming Service ... " << endl;
		Agent_InfoProvider_i * pServant = new Agent_InfoProvider_i;
		cout << "nc " << nc << endl;
		CORBA::Object_var obj = pServant->_this();
		cout << "obj " << obj << endl;
		nc->rebind(name, obj.in());
		cout << "done.\n";
	}
	catch (CORBA::Exception& e) {
		cerr << "не удалось зарегистрировать агента: " << e._info() << endl;
		return;
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
