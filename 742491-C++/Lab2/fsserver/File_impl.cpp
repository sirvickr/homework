#include "stdafx.h"

#if defined (HAVE_CONFIG_H)
#include <config.h>
#endif

#include <cstdio>
#include <cerrno>
#include <cstring>

#include <sys/types.h> // for lstat and struct stat
#include <sys/stat.h>  // for lstat and struct stat

#include "File_impl.h"
#include "FileSystem_impl.h"

using namespace std;

#if defined (LASY_ACTIVATION_POLICY)
File_impl::File_impl(PortableServer::Current_ptr current)
	: _current(PortableServer::Current::_duplicate(current))
#else
File_impl::File_impl(const char * name)
	: name_(CORBA::string_dup(name))
#endif
{
}

File_impl::~File_impl()
{
#if !defined (LASY_ACTIVATION_POLICY)
	if (name_) CORBA::string_free(name_);
#endif
}

char* File_impl::fileName()
throw(
	::CORBA::SystemException)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> lock(fileLock);
#if defined (LASY_ACTIVATION_POLICY)
	PortableServer::ObjectId_var oid(_current->get_object_id());
	CORBA::String_var name(PortableServer::ObjectId_to_string(oid));
#else
	CORBA::String_var name(CORBA::string_dup(name_));
#endif

	return CORBA::string_dup(FileSystem_impl::get_file_name(name.in()));
}

CORBA::ULong File_impl::filePointer()
throw(
	::CORBA::SystemException)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> lock(fileLock);
#if defined (LASY_ACTIVATION_POLICY)
	PortableServer::ObjectId_var oid(_current->get_object_id());
	CORBA::String_var name(PortableServer::ObjectId_to_string(oid));
#else
	CORBA::String_var name(CORBA::string_dup(name_));
#endif

	return static_cast<CORBA::ULong>(ftell(FileSystem_impl::get_file_pointer(name)));
}

void File_impl::read(::FS::OctetSequence_out data, CORBA::ULong length)
throw(
	::CORBA::SystemException,
	::FS::File::IOException)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> lock(fileLock);
	//fprintf(stderr, "File_impl::read\n");
#if defined (LASY_ACTIVATION_POLICY)
	PortableServer::ObjectId_var oid(_current->get_object_id());
	CORBA::String_var name(PortableServer::ObjectId_to_string(oid));
#else
	CORBA::String_var name(CORBA::string_dup(name_));
#endif

	FS::OctetSequence_var buf = new FS::OctetSequence(length);
	buf->length(length);

	FILE* fp = FileSystem_impl::get_file_pointer(name);
	CORBA::ULong nActual = 0;
	if ((int)(nActual = fread((void*)buf.in().get_buffer(), sizeof(CORBA::Octet), length, fp)) < 0) {
		FS::File::IOException e;
		e.errorNumber = FS::EIo;
		e.msg = CORBA::string_dup(strerror(errno));
		throw e;
	}

	buf->length(nActual);

	data = new FS::OctetSequence(buf.in());

}


void File_impl::write(const ::FS::OctetSequence& data)
throw(
	::CORBA::SystemException,
	::FS::File::IOException)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> lock(fileLock);
#if defined (LASY_ACTIVATION_POLICY)
	PortableServer::ObjectId_var oid(_current->get_object_id());
	CORBA::String_var name(PortableServer::ObjectId_to_string(oid));
#else
	CORBA::String_var name(CORBA::string_dup(name_));
#endif

	FILE* fp = FileSystem_impl::get_file_pointer(name);
	if (fwrite(data.get_buffer(), sizeof(CORBA::Octet), data.length(), fp) != static_cast<size_t>(data.length())) {
		FS::File::IOException e;
		e.errorNumber = FS::EIo;
		e.msg = CORBA::string_dup(strerror(errno));
		throw e;
	}
}

CORBA::ULong File_impl::sizeOf()
throw(
	::CORBA::SystemException,
	::FS::FileException)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> lock(fileLock);
#if defined (LASY_ACTIVATION_POLICY)
	PortableServer::ObjectId_var oid(_current->get_object_id());
	CORBA::String_var name(PortableServer::ObjectId_to_string(oid));
#else
	CORBA::String_var name(CORBA::string_dup(name_));
#endif

	FILE* fp = FileSystem_impl::get_file_pointer(name);
#if defined (_MSC_VER)
	int nFd(_fileno(fp));
#elif defined (HAS_FILENO)
	int nFd(fileno(fp));
#endif

#if defined (_WINDOWS)
	struct _stat st;
	int rc(_fstat(nFd, &st));
#elif defined (HAVE_FSTAT)
	struct stat st;
	int rc(fstat(nFd, &st));
#else
	return CORBA::ULong(0);
#endif

	if (rc != 0) {
		FS::FileException e;
		e.errorNumber = FS::Ebadf;
		e.msg = CORBA::string_dup(strerror(errno));
		throw e;
	}

	return st.st_size;
}

void File_impl::close()
throw(
	::CORBA::SystemException,
	::FS::FileException)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> lock(fileLock);
#if defined (LASY_ACTIVATION_POLICY)
	PortableServer::ObjectId_var oid(_current->get_object_id());
	CORBA::String_var name(PortableServer::ObjectId_to_string(oid));
#else
	CORBA::String_var name(CORBA::string_dup(name_));
#endif

	FileSystem_impl::close_file(name);
}

void File_impl::setFilePointer(CORBA::ULong filePointer)
throw(
	::CORBA::SystemException,
	::FS::File::InvalidFilePointer,
	::FS::FileException)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> lock(fileLock);
#if defined (LASY_ACTIVATION_POLICY)
	PortableServer::ObjectId_var oid(_current->get_object_id());
	CORBA::String_var name(PortableServer::ObjectId_to_string(oid));
#else
	CORBA::String_var name(CORBA::string_dup(name_));
#endif

	if (filePointer > sizeOf()) {
		FS::File::InvalidFilePointer e;
		throw e;
	}

	FILE* fp = FileSystem_impl::get_file_pointer(name);
	if (fseek(fp, filePointer, SEEK_SET) < 0) {
		FS::FileException e;
		e.errorNumber = FS::ERange;
		e.msg = CORBA::string_dup(strerror(errno));
		throw e;
	}
}
