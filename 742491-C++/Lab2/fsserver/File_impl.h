#pragma once

#if defined (HAVE_CONFIG_H)
#include "config.h"
#endif

#include "orb_server.h"

class File_impl
	: public virtual POA_FS::File
	, public virtual PortableServer::RefCountServantBase
{
public:
#if defined (LASY_ACTIVATION_POLICY)
	File_impl(PortableServer::Current_ptr current);
#else
	File_impl(const char * name);
#endif
	~File_impl();

	char* fileName()
		throw(::CORBA::SystemException);

	CORBA::ULong filePointer()
		throw(::CORBA::SystemException);

	void read(::FS::OctetSequence_out data, CORBA::ULong length)
		throw(::CORBA::SystemException, ::FS::File::IOException);

	void write(const ::FS::OctetSequence& data)
		throw(::CORBA::SystemException, ::FS::File::IOException);

	CORBA::ULong sizeOf()
		throw(::CORBA::SystemException, ::FS::FileException);

	void close()
		throw(::CORBA::SystemException, ::FS::FileException);

	void setFilePointer(CORBA::ULong filePointer)
		throw(::CORBA::SystemException, ::FS::File::InvalidFilePointer, ::FS::FileException);

protected:
#if defined (LASY_ACTIVATION_POLICY)
	PortableServer::Current_var _current;
#else
	char * name_;
#endif
	ACE_Recursive_Thread_Mutex fileLock;
};
