#include "stdafx.h"
#include "FileManager_impl.h"
#include "os.h"

using namespace std;

FileManager_impl::~FileManager_impl()
{
}

void FileManager_impl::mount( const char* mountPoint, ::FS::FileSystem_ptr file_System )
    throw(
        ::CORBA::SystemException,
        ::FS::InvalidFileName,
        ::FS::FileManager::InvalidFileSystem,
        ::FS::FileManager::MountPointAlreadyExists)
{
	cout << "FileManager::mount \"" << mountPoint << "\"" << endl;
	FS::FileSystem::FileInformationType tInfo;
	OSWrapper::GetFileInfo(mountPoint, tInfo);
	if (tInfo.kind != FS::FileSystem::FDIRECTORY) {
		string sMsg("cannot mount on " + string(mountPoint) + ": Not a directory");
		FS::InvalidFileName e;
		e.errorNumber = FS::NotDir;
		e.msg = CORBA::string_dup(sMsg.c_str());
		throw e;
	}

	if (CORBA::is_nil(file_System)) {
		FS::FileManager::InvalidFileSystem e;
		throw e;
	}

	mount_map_t::iterator i(_mount_map.find(mountPoint));
	if (i != _mount_map.end()) {
		FS::FileManager::MountPointAlreadyExists e;
		throw e;
	}

	_mount_map[mountPoint] = ::FS::FileSystem::_duplicate(file_System);

	if (currMountPoint.empty()) {
		currMountPoint = mountPoint;
	}
}

void FileManager_impl::unmount( const char* mountPoint )
    throw(
        ::CORBA::SystemException,
        ::FS::FileManager::NonExistentMount)
{
	cout << "FileManager::unmount \"" << mountPoint << "\"" << endl;
	mount_map_t::iterator i(_mount_map.find(mountPoint));
	if (i == _mount_map.end()) {
		FS::FileManager::NonExistentMount e;
		throw e;
	}

	_mount_map.erase(mountPoint);
}

::FS::FileManager::MountSequence* FileManager_impl::getMounts()
    throw(
        ::CORBA::SystemException)
{
	cout << "FileManager::getMounts" << endl;
	FS::FileManager::MountSequence_var mount_sequence(new FS::FileManager::MountSequence(0));
	if (mount_sequence == 0) {
		return mount_sequence._retn();
	}

	mount_sequence->length((CORBA::ULong)_mount_map.size());
	CORBA::ULong nCount = 0;
	for (mount_map_t::iterator i = _mount_map.begin(); i != _mount_map.end(); i++, nCount++) {
		(*mount_sequence)[nCount].mountPoint = CORBA::string_dup(i->first.c_str());
		(*mount_sequence)[nCount].fs = ::FS::FileSystem::_duplicate(i->second.in());
	}

	return mount_sequence._retn();
}

void FileManager_impl::remove( const char* fileName )
    throw(
        ::CORBA::SystemException,
        ::FS::FileException,
        ::FS::InvalidFileName)
{
	cout << "FileManager::remove \"" << fileName << "\"" << endl;
	string sRelativeName("");
	FS::FileSystem_var fs(getRelativeFilesystem(fileName, sRelativeName));
	if (CORBA::is_nil(fs.in())) {
		FS::InvalidFileName e;
		e.errorNumber = FS::NotSet;
		e.msg = CORBA::string_dup("The specified directory name doesn't map onto a mounted file system.");
		throw e;
	}

	fs->remove(sRelativeName.c_str());
}

void FileManager_impl::copy( const char* sourceFileName, const char* destinationFileName )
    throw(
        ::CORBA::SystemException,
        ::FS::InvalidFileName,
        ::FS::FileException)
{
	cout << "FileManager::copy \"" << sourceFileName << "\"" << " -> \"" << destinationFileName << "\"" << endl;
	string sSourceRelativeName("");
	FS::FileSystem_var src_fs(getRelativeFilesystem(sourceFileName, sSourceRelativeName));
	if (CORBA::is_nil(src_fs.in())) {
		FS::InvalidFileName e;
		e.errorNumber = FS::NotSet;
		e.msg = CORBA::string_dup("The specified directory name doesn't map onto a mounted file system.");
		throw e;
	}

	string sDestinationRelativeName("");
	FS::FileSystem_var dest_fs(getRelativeFilesystem(sourceFileName, sDestinationRelativeName));
	if (CORBA::is_nil(dest_fs.in())) {
		FS::InvalidFileName e;
		e.errorNumber = FS::NotSet;
		e.msg = CORBA::string_dup("The specified directory name doesn't map onto a mounted file system.");
		throw e;
	}

	FS::File_var fIn(src_fs->open(sourceFileName, true));
	FS::OctetSequence_var v(new FS::OctetSequence);
	v->length(fIn->sizeOf());
	fIn->read(v, fIn->sizeOf());
	fIn->close();

	FS::File_var fOut(dest_fs->create(destinationFileName));
	fOut->write(v.in());
	fOut->close();
}

CORBA::Boolean FileManager_impl::exists( const char* fileName )
    throw(
        ::CORBA::SystemException,
        ::FS::InvalidFileName)
{
	cout << "FileManager::exists \"" << fileName << "\"" << endl;
	string sRelativeName("");
	FS::FileSystem_var fs(getRelativeFilesystem(fileName, sRelativeName));
	if (CORBA::is_nil(fs.in())) {
		FS::InvalidFileName e;
		e.errorNumber = FS::NotSet;
		e.msg = CORBA::string_dup("The specified directory name doesn't map onto a mounted file system.");
		throw e;
	}

	return fs->exists(sRelativeName.c_str());
}

::FS::FileSystem::FileInformationSequence* FileManager_impl::list( const char* pattern )
    throw(
        ::CORBA::SystemException,
        ::FS::FileException,
        ::FS::InvalidFileName)
{
	cout << "FileManager::list \"" << pattern << "\"" << endl;
	std::string sRelativeName("");
	FS::FileSystem_var fs(getRelativeFilesystem(pattern, sRelativeName));
	if (CORBA::is_nil(fs.in())) {
		FS::InvalidFileName e;
		e.errorNumber = FS::NotSet;
		e.msg = CORBA::string_dup("The specified directory name doesn't map onto a mounted file system.");
		throw e;
	}
	std::string sMountDir(pattern);
	sMountDir.erase(sMountDir.find_last_of(sRelativeName)-1);
	cout << "sMountDir \"" << sMountDir << "\"" << " sRelativeName \"" << sRelativeName << "\"" << endl;

	FS::FileSystem::FileInformationSequence_var file_info(fs->list(sRelativeName.c_str()));
	for (unsigned int i(0); i < file_info->length(); ++i) {
		string sTest = (*file_info)[i].name.in();
		//cout << "sTest \"" << sTest << "\" -> ";
		//sTest.erase(0, sTest.find_last_of("/"));
		//cout << " \"" << sTest << "\"" << endl;

		//
		// TODO:
		//
		// добавить проверку того, что file_info представляет директорию
		// и если да, проверить, содержит ли она подмонтированную ФС
		//
		(*file_info)[i].name = CORBA::string_dup(string(sMountDir + sTest).c_str());
	}

	cout << "success" << endl;
	return file_info._retn();
}

::FS::File_ptr FileManager_impl::create( const char* fileName )
    throw(
        ::CORBA::SystemException,
        ::FS::InvalidFileName,
        ::FS::FileException)
{
	cout << "FileManager::create \"" << fileName << "\"" << endl;
	string sRelativeName("");
	FS::FileSystem_var fs(getRelativeFilesystem(fileName, sRelativeName));
	if (CORBA::is_nil(fs.in())) {
		FS::InvalidFileName e;
		e.errorNumber = FS::NotSet;
		e.msg = CORBA::string_dup("The specified directory name doesn't map onto a mounted file system.");
		throw e;
	}

	return fs->create(sRelativeName.c_str());
}

::FS::File_ptr FileManager_impl::open( const char* fileName, CORBA::Boolean read_Only )
    throw(
        ::CORBA::SystemException,
        ::FS::InvalidFileName,
        ::FS::FileException)
{
	cout << "FileManager::open \"" << fileName << "\", " << read_Only << endl;
	string sRelativeName("");
	FS::FileSystem_var fs(getRelativeFilesystem(fileName, sRelativeName));
	if (CORBA::is_nil(fs.in())) {
		FS::InvalidFileName e;
		e.errorNumber = FS::NotSet;
		e.msg = CORBA::string_dup("The specified directory name doesn't map onto a mounted file system.");
		throw e;
	}

	return fs->open(sRelativeName.c_str(), read_Only);
}

void FileManager_impl::mkdir( const char* directoryName )
    throw(
        ::CORBA::SystemException,
        ::FS::InvalidFileName,
        ::FS::FileException)
{
	cout << "FileManager::mkdir \"" << directoryName << "\"" << endl;
	string sRelativeName("");
	FS::FileSystem_var fs(getRelativeFilesystem(directoryName, sRelativeName));
	if (CORBA::is_nil(fs.in())) {
		FS::InvalidFileName e;
		e.errorNumber = FS::NotSet;
		e.msg = CORBA::string_dup("The specified directory name doesn't map onto a mounted file system.");
		throw e;
	}

	fs->mkdir(sRelativeName.c_str());
}

void FileManager_impl::rmdir( const char* directoryName )
    throw(
        ::CORBA::SystemException,
        ::FS::InvalidFileName,
        ::FS::FileException)
{
	cout << "FileManager::rmdir \"" << directoryName << "\"" << endl;
	string sRelativeName("");
	FS::FileSystem_var fs(getRelativeFilesystem(directoryName, sRelativeName));
	if (CORBA::is_nil(fs.in())) {
		FS::InvalidFileName e;
		e.errorNumber = FS::NotSet;
		e.msg = CORBA::string_dup("The specified directory name doesn't map onto a mounted file system.");
		throw e;
	}

	fs->rmdir(sRelativeName.c_str());
}

void FileManager_impl::query( ::FS::Properties& fileSystemProperties )
    throw(
        ::CORBA::SystemException,
        ::FS::FileSystem::UnknownFileSystemProperties)
{
	cout << "FileManager::query" << endl;
	//
	// Пройти по всем ФС и собрать релузьтаты в один
	//
	string sRelativeName("");
	FS::FileSystem_var fs(getRelativeFilesystem("/", sRelativeName));

	fs->query(fileSystemProperties);
}

char * FileManager_impl::currDir(void)
{
	cout << ">> get currDir: currMountPoint " << currMountPoint << endl;
	mount_map_t::const_iterator i(_mount_map.find(currMountPoint));
	if (i != _mount_map.end()) {
		return i->second->currDir();
	}
	cerr << "   FS not found for " << currMountPoint << endl;
	return NULL;
}

void FileManager_impl::currDir(const char * value)
{
	_sCurPathName = value;
}

::FS::FileSystem_var FileManager_impl::getRelativeFilesystem(const string& sName, string& sFname)
{
	string _name(sName);
	string::size_type _nIndex(_name.find_last_of("/"));
	cout << ">> name " << _name << " nIndex " << _nIndex << endl;
	cout << "   mounts:";
	for (const auto& item : _mount_map)
		cout << "[" << item.first << "]";
	cout << endl;

	string _sFname("");
	while (! _name.empty() && _nIndex != string::npos) {
		string _base(_name.substr(0, _nIndex));
		string _rest(_name.substr(_nIndex));
		_sFname = _rest + _sFname;
		_name.erase(_nIndex);
		_nIndex = _name.find_last_of("/");
		if (_base.empty()) 
			_base = "/";
		cout << "   _base [" << _base << "]" << endl;
		mount_map_t::const_iterator i(_mount_map.find(_base));
		if (i != _mount_map.end()) {
#ifdef _WIN32
			sFname = _sFname.substr(1);
#else
			sFname = _sFname;
#endif
			cout << ">> got [" << i->first << "]: " << sFname << endl;
			return i->second;
		}
	}

	return FS::FileSystem::_nil();
}
