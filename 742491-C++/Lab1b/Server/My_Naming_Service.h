#pragma once

class My_Naming_Service
{
	// = TITLE
	//   Defines a class that encapsulates the implementation of the
	//   COS Naming Service.
	//
	// = DESCRIPTION
	//   This class makes use of the <TAO_Naming_Server> and
	//   <TAO_ORB_Manager> class to implement the COS Naming Service.
public:
	My_Naming_Service(CORBA::ORB_ptr orb = CORBA::ORB::_nil(), CORBA::ORBid orbid = 0);
	// Constructor taking the command-line arguments.
	My_Naming_Service(int argc, ACE_TCHAR* argv[], CORBA::ORB_ptr orb = CORBA::ORB::_nil(), CORBA::ORBid orbid = 0);
	// Initialize the Naming Service with the arguments.
	virtual int init(int argc, ACE_TCHAR* argv[]);
	// Run the My_Naming_Service.
	int run();
	// Shutdown the My_Naming_Service.
	void shutdown(CORBA::Boolean wait_for_completion = 0);
	CosNaming::NamingContext_ptr operator->() const;
	// Destructor.
	virtual ~My_Naming_Service(void);
protected:
	// The ORB.
	CORBA::ORB_var orb_;
	CORBA::ORBid orbid_;
	// Parse the command line arguments to find
	// the timeout period.
	int parse_args(int &argc, ACE_TCHAR* argv[]);
	// The Root POA.
	//  PortableServer::POA_var root_poa_;
	// Naming Server instance.
	TAO_Naming_Server naming_server_;
	// After how long the server should stop listening to requests (in seconds).
	long time_;
};
