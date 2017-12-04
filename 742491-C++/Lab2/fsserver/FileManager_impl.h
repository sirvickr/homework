#pragma once

#include "orb_server.h"

#include "FileSystem_impl.h"

class FileManager_impl
	: public FileSystem_impl
	, public virtual POA_FS::FileManager
{
public:

	typedef std::map<std::string, ::FS::FileSystem_var> mount_map_t;

	FileManager_impl() {};
	~FileManager_impl();

	void mount(const char* mountPoint, ::FS::FileSystem_ptr file_System)
		throw(
			::CORBA::SystemException,
			::FS::InvalidFileName,
			::FS::FileManager::InvalidFileSystem,
			::FS::FileManager::MountPointAlreadyExists)
		;

	void unmount(const char* mountPoint)
		throw(
			::CORBA::SystemException,
			::FS::FileManager::NonExistentMount)
		;

	::FS::FileManager::MountSequence* getMounts()
		throw(
			::CORBA::SystemException)
		;

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

	::FS::FileSystem_var getRelativeFilesystem(const string& sName, string& sFname);

	mount_map_t _mount_map;

	std::string currMountPoint;
};
