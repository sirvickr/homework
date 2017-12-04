#include "stdafx.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef _WIN32
#include <unistd.h>
#endif

#include "FileSystem_impl.h"
#include "os.h"
#if !defined (LASY_ACTIVATION_POLICY)
#include "File_impl.h"
#endif

using namespace std;

map<string, FileSystem_impl::open_file_info_t> FileSystem_impl::_file_info;

//-------------------------------------------------------------------
FileSystem_impl::FileSystem_impl(bool& retval, PortableServer::POA_ptr _poa, const char* szBasePath)
	: _file_poa(PortableServer::POA::_duplicate(_poa))
	, _sBaseName(szBasePath)
#ifdef ORB_ORBIX2000
	, _nFileNumber(0)
#endif
{
	cout << "FileSystem_impl: ctor" << endl;
	retval = false;
	if (
#ifdef _WIN32
		_sBaseName != "" &&
#endif
		*(_sBaseName.end() - 1) != '/') {
		_sBaseName.append("/");
	}
	cout << "Base path: \"" << _sBaseName << "\"" << endl;

	//#ifdef _WIN32
	//  _sCurPathName = GetCommandLine();
	//  std::string::size_type pos = _sCurPathName.rfind('\\');
	//  if(pos == std::string::npos)
	//    _sCurPathName = "";
	//  _sCurPathName = _sCurPathName.substr(0, pos);
	//  if(_sCurPathName[0] == '\"')
	//    _sCurPathName = _sCurPathName.substr(1);
	//  pos = 0;
	//  while((pos = _sCurPathName.find('\\', pos)) != std::string::npos)
	//    _sCurPathName[pos] = '/';
	//  _sCurPathName[0] = toupper(_sCurPathName[0]); // just for beauty=))
	//#else // #ifndef _WIN32
	const size_t bufSize = 256;
	char buf[bufSize];
#if defined (_MSC_VER)
	_getcwd(buf, bufSize);
#elif defined (HAS_GETCWD)
	getcwd(buf, bufSize);
#endif
	if (buf)
		_sCurPathName = buf;
	else
		_sCurPathName = "";
	//#endif
	cout << "Current directory path: \"" << _sCurPathName << "\"" << endl;

	retval = true;
}

FileSystem_impl::~FileSystem_impl()
{
}

void FileSystem_impl::checkFileName(const char *fileName)
{
	if (*fileName != '/' && (!isalpha(fileName[0]) || fileName[1] != ':')) {
		FS::InvalidFileName e;
		e.errorNumber = FS::NotSet;
		e.msg = CORBA::string_dup("file names must begin with a leading '/' character or '<letter>:'.");
		throw e;
	}
}

//-------------------------------------------------------------------
void FileSystem_impl::remove(const char* fileName)
throw(
	::CORBA::SystemException,
	::FS::FileException,
	::FS::InvalidFileName)
{
	checkFileName(fileName);

	if (::remove(string(_sBaseName + fileName).c_str()) != 0) {
		FS::FileException e;
		e.errorNumber = FS::EAccess;
		e.msg = CORBA::string_dup(strerror(errno));
		throw e;
	}
}

//-------------------------------------------------------------------
void FileSystem_impl::copy(const char* sourceFileName, const char* destinationFileName)
throw(
	::CORBA::SystemException,
	::FS::InvalidFileName,
	::FS::FileException)
{
	//cerr << "FileSystem_impl::copy(" << sourceFileName << " to " << destinationFileName << ")" << endl;
	checkFileName(sourceFileName);
	checkFileName(destinationFileName);

	//cerr << "open (" << sourceFileName << ")" << endl;
	FS::File_var fIn(FileSystem_impl::open(sourceFileName, true));
	//cerr << "1." << endl;
	FS::OctetSequence_var v(new FS::OctetSequence);
	//cerr << "2." << endl;
	v->length(fIn->sizeOf());
	//cerr << "before read" << endl;
	fIn->read(v, fIn->sizeOf());
	//cerr << "after read" << endl;
	fIn->close();

	FS::File_var fOut(FileSystem_impl::create(destinationFileName));
	fOut->write(v.in());
	fOut->close();
}

//-------------------------------------------------------------------
CORBA::Boolean FileSystem_impl::exists(const char* fileName)
throw(
	::CORBA::SystemException,
	::FS::InvalidFileName)
{
	//cerr << "FileSystem_impl::exists(" << fileName << ")" << endl;
	checkFileName(fileName);

	return OSWrapper::GetFileExists(string(_sBaseName + fileName).c_str());
}

//-------------------------------------------------------------------
::FS::FileSystem::FileInformationSequence* FileSystem_impl::list(const char* pattern)
throw(
	::CORBA::SystemException,
	::FS::FileException,
	::FS::InvalidFileName)
{
	FS::FileSystem::FileInformationSequence_var file_info(new FS::FileSystem::FileInformationSequence(0));
	if (file_info == 0) {
		return file_info._retn();
	}

#ifdef _WIN32 // Get root directories
	if (_sBaseName == "" && (strcmp(pattern, "*") == 0 || strcmp(pattern, "/*") == 0))
	{
		TCHAR *buf = new TCHAR[1];
		DWORD bufLen = GetLogicalDriveStrings(0, buf);

		delete buf;
		buf = 0;

		if (bufLen)
		{
			buf = new TCHAR[bufLen + 1];
			DWORD lenRequired = GetLogicalDriveStrings(bufLen, buf);
			if (lenRequired && lenRequired <= bufLen)
			{
				TCHAR currDrive[4] = TEXT(" :/");
				TCHAR* p = buf;
				do {
					*currDrive = *p;

					if (toupper(*currDrive) != 'A' && toupper(*currDrive) != 'B' &&
						GetDriveType(currDrive) != DRIVE_UNKNOWN &&
						GetDriveType(currDrive) != DRIVE_CDROM &&
						GetDriveType(currDrive) != DRIVE_NO_ROOT_DIR/* &&
																	GetDriveType(currDrive) != DRIVE_RAMDISK*/)
					{
						std::cout << ">" << currDrive << "<\n";
						FS::FileSystem::FileInformationType_var info(new FS::FileSystem::FileInformationType);
						info->name = CORBA::string_dup(currDrive);
						info->kind = FS::FileSystem::FDIRECTORY;

						//CORBA::ULongLong freeBytesAvailable = 0,
						//                 totalNumberOfBytes = 0,
						//                 totalNumberOfFreeBytes = 0;

						CORBA::ULongLong sectorsPerCluster = 0,
							bytesPerSector = 0,
							numberOfFreeClusters = 0,
							totalNumberOfClusters = 0;

						//if(GetDiskFreeSpaceEx(currDrive, (PULARGE_INTEGER)&freeBytesAvailable,
						//                                 (PULARGE_INTEGER)&totalNumberOfBytes,
						//                                 (PULARGE_INTEGER)&totalNumberOfFreeBytes)) {
						if (GetDiskFreeSpace(currDrive, (LPDWORD)&sectorsPerCluster,
							(LPDWORD)&bytesPerSector,
							(LPDWORD)&numberOfFreeClusters,
							(LPDWORD)&totalNumberOfClusters)) {
							//info->size = totalNumberOfBytes;
							info->size = totalNumberOfClusters * sectorsPerCluster * bytesPerSector;
						}
						else {
							TCHAR szBuf[128];
							LPVOID lpMsgBuf;

							FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
								NULL,
								GetLastError(),
								MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
								(LPTSTR)&lpMsgBuf,
								0,
								NULL);

							wsprintf(szBuf, ": %s", lpMsgBuf);
							std::cerr << szBuf << endl;
						}

						file_info->length(file_info->length() + 1);
						(*file_info)[file_info->length() - 1] = info;
					}
					while (*p++); // Go to the next NULL character.
				} while (*p);
				delete buf;
				buf = 0;
			}
		}
		else {
			TCHAR szBuf[128];
			LPVOID lpMsgBuf;

			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0,
				NULL);

			wsprintf(szBuf, ": %s", lpMsgBuf);
			std::cerr << szBuf << endl;

			FS::FileException e;
			e.errorNumber = FS::EInval;
			e.msg = CORBA::string_dup(szBuf);
			throw e;
		}
		return file_info._retn();
	}

#endif

	std::list<string> _file_list;
	if (OSWrapper::GetFileList(string(_sBaseName + pattern).c_str(), _file_list) != 0) {
		FS::FileException e;
		e.errorNumber = FS::EInval;
		e.msg = CORBA::string_dup(strerror(errno));
		throw e;
	}

	for (std::list<string>::const_iterator i(_file_list.begin()); i != _file_list.end(); ++i) {
		FS::FileSystem::FileInformationType_var info(new FS::FileSystem::FileInformationType);

		if (OSWrapper::GetFileInfo(i->c_str(), info.inout()) != 0) {
			FS::FileException e;
			e.errorNumber = FS::EAccess;
			e.msg = CORBA::string_dup(strerror(errno));
			throw e;
		}

		// Reset the file name so that it is relative to the receiver file system object.
		info->name = CORBA::string_dup(i->c_str());

		file_info->length(file_info->length() + 1);
		(*file_info)[file_info->length() - 1] = info.in();
	}

	return file_info._retn();
}

//-------------------------------------------------------------------
::FS::File_ptr FileSystem_impl::create(const char* fileName)
	throw(::CORBA::SystemException, ::FS::InvalidFileName, ::FS::FileException)
{
	if (exists(fileName)) {
		FS::FileException e;
		e.errorNumber = FS::EIo;
		e.msg = CORBA::string_dup(strerror(17));
		throw e;
	}

	open_file_info_t _info;
	_info._sFileName = fileName;

	if ((_info._fp = ::fopen((_sBaseName + fileName).c_str(), "wb+")) == 0) {
		FS::FileException e;
		e.errorNumber = FS::EAccess;
		e.msg = CORBA::string_dup(::strerror(errno));
		throw e;
	}

#if defined (ORB_ORBIX2000)
	char sFileId[100];
#if defined (HAVE_SNPRINTF)
	snprintf(sFileId, 99, "%d", _nFileNumber++);
#elif defined (HAVE_SPRINTF)
	sprintf(sFileId, "%d", _nFileNumber++);
#endif

	PortableServer::ObjectId_var id(PortableServer::string_to_ObjectId(sFileId));
	CORBA::Object_var obj = _file_poa->create_reference_with_id(id, "IDL:FS/File:1.0");
#else
	// patch begin
#if defined (LASY_ACTIVATION_POLICY)
	CORBA::Object_var obj = _file_poa->create_reference("IDL:FS/File:1.0");

	FS::File_var ref = FS::File::_narrow(obj);
	assert(!CORBA::is_nil(ref));
	PortableServer::ObjectId_var oid(_file_poa->reference_to_id(ref));
	PortableServer::ObjectId_to_string(oid);
	CORBA::String_var file_id = PortableServer::ObjectId_to_string(oid);
#else
	char id[16];
#if defined (_MSC_VER)
	sprintf(id, "%d", _fileno(_info._fp));
#elif defined (HAS_FILENO)
	sprintf(id, "%d", fileno(_info._fp));
#endif
	PortableServer::ObjectId_var _oid = _file_poa->activate_object(new File_impl(id));
	CORBA::Object_var obj = _file_poa->id_to_reference(_oid.in());

	FS::File_var ref = FS::File::_narrow(obj.in());
	assert(!CORBA::is_nil(ref.in()));
	CORBA::String_var file_id = CORBA::string_dup(id);
#endif
	// patch end 
#endif

	FileSystem_impl::_file_info[file_id.in()] = _info;
	return ref._retn();
}

//-------------------------------------------------------------------
::FS::File_ptr FileSystem_impl::open(const char* fileName, CORBA::Boolean read_Only)
throw(
	::CORBA::SystemException,
	::FS::InvalidFileName,
	::FS::FileException)
{
	cerr << "FileSystem_impl::open(" << fileName << ", " << (int)read_Only << ")" << endl;
	open_file_info_t _info;
	_info._sFileName = fileName;

	string sMode((read_Only ? "rb" : "rb+"));

	cerr << " _sBaseName " << _sBaseName << endl;
	if ((_info._fp = ::fopen(string(_sBaseName + fileName).c_str(), sMode.c_str())) == 0) {
		cerr << " fopen " << (_sBaseName + fileName) << " failed" << endl;
		FS::FileException e;
		e.errorNumber = FS::EAccess;
		e.msg = CORBA::string_dup(::strerror(errno));
		throw e;
	}
	//fprintf(stderr, "opened ptr: %p\n", _info._fp);

#if defined (LASY_ACTIVATION_POLICY)
	CORBA::Object_var obj = _file_poa->create_reference("IDL:FS/File:1.0");
	FS::File_var ref = FS::File::_narrow(obj);
	assert(!CORBA::is_nil(ref));

	PortableServer::ObjectId_var oid(_file_poa->reference_to_id(ref));
	CORBA::String_var file_id(PortableServer::ObjectId_to_string(oid));
#else
	char id[16];
#if defined (_MSC_VER)
	sprintf(id, "%d", _fileno(_info._fp));
#elif defined (HAS_FILENO)
	sprintf(id, "%d", fileno(_info._fp));
#endif
	PortableServer::ObjectId_var _oid = _file_poa->activate_object(new File_impl(id));
	CORBA::Object_var obj = _file_poa->id_to_reference(_oid.in());

	FS::File_var ref = FS::File::_narrow(obj.in());
	assert(!CORBA::is_nil(ref.in()));
	CORBA::String_var file_id = CORBA::string_dup(id);
#endif

	FileSystem_impl::_file_info[file_id.in()] = _info;

	return ref._retn();
}

//-------------------------------------------------------------------
void FileSystem_impl::mkdir(const char* directoryName)
throw(
	::CORBA::SystemException,
	::FS::InvalidFileName,
	::FS::FileException)
{
	checkFileName(directoryName);

	if (OSWrapper::MakeDirectory(string(_sBaseName + directoryName).c_str()) < 0) {
		FS::FileException e;
		e.errorNumber = FS::EAccess;
		e.msg = CORBA::string_dup(strerror(errno));
		throw e;
	}
}

//-------------------------------------------------------------------
void FileSystem_impl::rmdir(const char* directoryName)
throw(
	::CORBA::SystemException,
	::FS::InvalidFileName,
	::FS::FileException)
{
	checkFileName(directoryName);

	if (OSWrapper::RemoveDirectory(string(_sBaseName + directoryName).c_str()) < 0) {
		FS::FileException e;
		e.errorNumber = FS::EAccess;
		e.msg = CORBA::string_dup(strerror(errno));
		throw e;
	}
}

void FileSystem_impl::query(::FS::Properties& fileSystemProperties)
	throw(::CORBA::SystemException, ::FS::FileSystem::UnknownFileSystemProperties)
{
	CORBA::ULong nCount(fileSystemProperties.length());

	FS::FileSystem::UnknownFileSystemProperties ufspException;
	bool shouldThrow(false);
	for (unsigned int i(0); i < nCount; ++i) {
		if (strcmp(fileSystemProperties[i].id.in(), "SIZE") == 0) {
			fileSystemProperties[i].value <<= OSWrapper::GetTotalSpaceOnFileSystem(_sBaseName.c_str());//getTotalSpace();
		}
		else if (strcmp(fileSystemProperties[i].id.in(), "AVAILABLE_SPACE") == 0) {
			fileSystemProperties[i].value <<= OSWrapper::GetAvailableSpaceOnFileSystem(_sBaseName.c_str());
		}
		else if (strcmp(fileSystemProperties[i].id.in(), "BASE_PATH") == 0) {
			fileSystemProperties[i].value <<= CORBA::string_dup(_sBaseName.c_str());
		}
		else if (strcmp(fileSystemProperties[i].id.in(), "CURRENT_PATH") == 0) {
			fileSystemProperties[i].value <<= CORBA::string_dup(_sCurPathName.c_str());
		}
		else {
			shouldThrow = true;
			ufspException.invalidProperties.length(ufspException.invalidProperties.length() + 1);
			ufspException.invalidProperties[ufspException.invalidProperties.length() - 1].id = CORBA::string_dup(fileSystemProperties[i].id.in());
		}
	}

	if (shouldThrow) {
		throw ufspException;
	}
}

char * FileSystem_impl::currDir(void)
{
	return CORBA::string_dup(_sCurPathName.c_str());
}

void FileSystem_impl::currDir(const char * value)
{
	_sCurPathName = value;
}

char* FileSystem_impl::get_file_name(const char* szOidString)
{
	return CORBA::string_dup(FileSystem_impl::_file_info[szOidString]._sFileName.c_str());
}

FILE* FileSystem_impl::get_file_pointer(const char* szOidString)
{
	return FileSystem_impl::_file_info[szOidString]._fp;
}

void FileSystem_impl::close_file(const char* szOidString)
{
	fclose(get_file_pointer(szOidString));
	FileSystem_impl::_file_info.erase(szOidString);
}
