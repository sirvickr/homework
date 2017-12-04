#include "stdafx.h"
#include "My_Naming_Service.h"

My_Naming_Service::My_Naming_Service(CORBA::ORB_ptr orb, CORBA::ORBid orbid)
	: orb_(CORBA::ORB::_duplicate(orb)), orbid_(orbid), time_(0)
{
}

// Constructor taking command-line arguments.
My_Naming_Service::My_Naming_Service(int argc, ACE_TCHAR* argv[], CORBA::ORB_ptr orb, CORBA::ORBid orbid)
	: orb_(CORBA::ORB::_duplicate(orb)), orbid_(orbid), time_(0)
{
	this->init(argc, argv);
}

CosNaming::NamingContext_ptr My_Naming_Service::operator->() const
{
	return naming_server_.operator->();
}

// Initialize the state of the My_Naming_Service object
int My_Naming_Service::init(int argc, ACE_TCHAR* argv[])
{
	if (CORBA::is_nil(orb_.in())) {
		int result;
		//ACE_DECLARE_NEW_CORBA_ENV;
		try
		{
			// Copy command line parameter.
			ACE_Argv_Type_Converter command_line(argc, argv);
			// Initialize the ORB
			this->orb_ = CORBA::ORB_init(
				command_line.get_argc(),
				command_line.get_ASCII_argv(),
				orbid_ // 0
				//ACE_ENV_ARG_PARAMETER
			);
			// Parse the args for '-t' option. If '-t' option is passed, do
			// the needful and then remove the option from the list of
			// arguments.
			this->parse_args(command_line.get_argc(), command_line.get_TCHAR_argv());
			// This function call initializes the naming service and returns
			// '-1' in case of an exception.
			result = this->naming_server_.init_with_orb(
				command_line.get_argc(),
				command_line.get_TCHAR_argv(),
				this->orb_.in());
			if (result == -1) {
				return result;
			}
		}
		catch(...)
		{
			//ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "My_Naming_Service::init");
			return -1;
		}
		//ACE_ENDTRY;
		//ACE_CHECK_RETURN(-1);
	}
	return 0;
}

int My_Naming_Service::parse_args(int &argc, ACE_TCHAR* argv[])
{
	ACE_Get_Opt get_opts(argc, argv, "-t:");
	int c;
	while ((c = get_opts()) != -1) {
		switch (c) {
		case 't': {
			int time = ACE_OS::atoi(get_opts.opt_arg());
			if (time >= 0)
				this->time_ = time;
			// Remove the option '-t' from argv []
			// to avoid any confusion that might result.
			for (int i = get_opts.opt_ind(); i != argc; ++i)
				argv[i - 2] = argv[i];
			// Decrement the value of argc to reflect the removal
			// of '-t' option.
			argc = argc - 2;
			break;
		}
		case '?':
		default:
			// Don't do anything. The TAO_Naming_Server::parse_args ()
			// takes care of indicating an error in case of error.
			break;
		}
	}
	return 0;
}

// Run the ORB event loop.
int My_Naming_Service::run()
{
	if (time_ == 0)
	{
		this->orb_->run();
	}
	else
	{
		ACE_Time_Value tv(time_);
		this->orb_->run(tv);
	}

	return 0;
}

void My_Naming_Service::shutdown(CORBA::Boolean wait_for_completion)
{
	orb_->shutdown(wait_for_completion);
}

// Destructor.
My_Naming_Service::~My_Naming_Service(void)
{
}
