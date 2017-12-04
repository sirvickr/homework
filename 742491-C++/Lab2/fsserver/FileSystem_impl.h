#pragma once

#if defined (HAVE_CONFIG_H)
#include "config.h"
#endif

#include "orb_server.h"

using namespace std;

class File_impl;

class FileSystem_impl
	: public virtual POA_FS::FileSystem
	, public virtual PortableServer::RefCountServantBase
{
	friend class File_impl;

public:
	FileSystem_impl(bool& retval, PortableServer::POA_ptr _poa, const char* szBasePath);
	~FileSystem_impl();

	void remove(const char* fileName)
		throw(
			::CORBA::SystemException,
			::FS::FileException,
			::FS::InvalidFileName)
		;

	void copy(const char* sourceFileName, const char* destinationFileName)
		throw(
			::CORBA::SystemException,
			::FS::InvalidFileName,
			::FS::FileException)
		;

	CORBA::Boolean exists(const char* fileName)
		throw(
			::CORBA::SystemException,
			::FS::InvalidFileName)
		;

	::FS::FileSystem::FileInformationSequence* list(const char* pattern)
		throw(
			::CORBA::SystemException,
			::FS::FileException,
			::FS::InvalidFileName)
		;

	::FS::File_ptr create(const char* fileName)
		throw(
			::CORBA::SystemException,
			::FS::InvalidFileName,
			::FS::FileException)
		;

	::FS::File_ptr open(const char* fileName, CORBA::Boolean read_Only)
		throw(
			::CORBA::SystemException,
			::FS::InvalidFileName,
			::FS::FileException)
		;

	void mkdir(const char* directoryName)
		throw(
			::CORBA::SystemException,
			::FS::InvalidFileName,
			::FS::FileException)
		;

	void rmdir(const char* directoryName)
		throw(
			::CORBA::SystemException,
			::FS::InvalidFileName,
			::FS::FileException)
		;

	void query(::FS::Properties& fileSystemProperties)
		throw(
			::CORBA::SystemException,
			::FS::FileSystem::UnknownFileSystemProperties)
		;

	char * currDir(void) override;

	void currDir(const char * value) override;

protected:

	static char* get_file_name(const char* szOidString);

	static FILE* get_file_pointer(const char* szOidString);

	static void  close_file(const char* szOidString);

	typedef struct {
		std::string _sFileName;
		int         _fileDescriptor;
		FILE*       _fp;
	} open_file_info_t;

	FileSystem_impl() : _file_poa(PortableServer::POA::_nil()), _sBaseName("") {};

	PortableServer::POA_var _file_poa;

	std::string _sBaseName;

	std::string _sCurPathName;

	static std::map<std::string, open_file_info_t> _file_info;

	void checkFileName(const char *fileName);

};
