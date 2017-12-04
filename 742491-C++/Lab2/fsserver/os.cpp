#include "stdafx.h"

#if defined (HAVE_CONFIG_H)
#include <config.h>
#elif defined (HAVE_CONFIG_WIN32_H)
#include <config_win32.h>
#endif

#include "orb_server.h"
#include <iostream>

#if defined (HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined (HAVE_STAT)
#include <sys/stat.h>
#include <sys/types.h>
#endif

#if defined (HAVE_STATVFS)
#include <sys/statvfs.h>
#elif defined (HAVE_STATFS)
#include <sys/vfs.h>
#endif

#if defined (HAVE_GLOB)
#include <glob.h>
#elif defined (HAVE_OPENDIR)
#include <sys/types.h>
#include <direct.h> // #include <dirent.h>
#endif

#include <list>
#include <string>
#include <cerrno>
#include <cstring>

namespace OSWrapper
{

//---------------------------------------------------------
CORBA::ULongLong GetAvailableSpaceOnFileSystem(const char* szFileName)
{
  CORBA::ULongLong size(0);
#if defined (HAVE_STATVFS)
  struct statvfs buf;
  if (statvfs(szFileName, &buf) != 0) {
    return 0;
  }

  size = (buf.f_bsize / 1024) * buf.f_bavail;
#elif defined (HAVE_STATFS)
  struct statvfs buf;
  if (statvfs(szFileName, &buf) != 0) {
    return 0;
  }

  size = (buf.f_bsize / 1024) * buf.f_bavail;
#endif
  return size;
}

//---------------------------------------------------------
CORBA::ULongLong GetTotalSpaceOnFileSystem(const char* szFileName)
{
  CORBA::ULongLong size(0);
#if defined (HAVE_STATVFS)
  struct statvfs buf;
  if (statvfs(szFileName, &buf) != 0) {
    return 0;
  }

  size = (buf.f_bsize / 1024) * buf.f_blocks;
#elif defined (HAVE_STATFS)
  struct statfs buf;
  if (statfs(szFileName, &buf) != 0) {
    return 0;
  }

  size = (buf.f_bsize / 1024) * buf.f_blocks;
#endif
  return size;
}

CORBA::ULong GetFileSize(const char* szFileName)
{
  CORBA::ULong nSize(0);
#if defined (_MSC_VER)
  struct _stat stat_buf;
  if (_stat(szFileName, &stat_buf) != 0) {
    return -1;
  }

  nSize = stat_buf.st_size;
#elif defined (HAVE_STAT)
  struct stat stat_buf;
  if (stat(szFileName, &stat_buf) != 0) {
    return 0;
  }

  nSize = stat_buf.st_size;
#endif

  return nSize;
}

//---------------------------------------------------------
FS::Properties GetFileProperties(const char* szFileName)
    throw(
        ::CORBA::SystemException,
        ::FS::FileException)
{
#if defined (_MSC_VER)
  struct _stat stat_buf;
  if (_stat(szFileName, &stat_buf) != 0) {
#else
  struct stat stat_buf;
  if (stat(szFileName, &stat_buf) != 0) {
#endif
	  FS::FileException e;
	//e.errorNumber = FS::SCA_EBADF;
    e.msg = CORBA::string_dup(strerror(errno));
    throw e;
  }

  FS::Properties _properties;
  /**
   * ACE+TAO wasn't too happy using the default time_t type in assinging
   * the values for the created, last accessed, and modified times for a file
   * to the ANY part of the properties.
   *
   * It seems that the 'ANY' class doesn't have an adequate <<= operator for
   * whatever a 'time_t' type happens to be typedef'd from.  It is interesting
   * to note that neither MICO nor ORBIX2000 have this problem.
   *
   * Anyway, since using a static_cast here won't hurt MICO or ORBIX, and since
   * that static_cast makes ACE+TAO happier, I'll use the static_cast for now.
   */
  _properties.length(3);
  _properties[0].id = CORBA::string_dup("CREATED_TIME");
  _properties[0].value <<= static_cast<long long unsigned int>(stat_buf.st_ctime);
  _properties[1].id = CORBA::string_dup("LAST_ACCESS_TIME");
  _properties[1].value <<= static_cast<long long unsigned int>(stat_buf.st_atime);
  _properties[2].id = CORBA::string_dup("MODIFIED_TIME");
  _properties[2].value <<= static_cast<long long unsigned int>(stat_buf.st_mtime);
  return _properties;
}

//---------------------------------------------------------
CORBA::Boolean GetFileExists( const char* szFileName )
    throw(
        ::CORBA::SystemException)
{
#if defined (_MSC_VER)
  return (_access(szFileName, 00) == 0);
#elif defined (HAVE_ACCESS)
  return (access(szFileName, F_OK) == 0);
#else
  return 0;
#endif
}

//---------------------------------------------------------
int MakeDirectory(const char* szDirectoryName)
{
#if defined (_MSC_VER)
  return _mkdir(szDirectoryName);
#elif defined (HAVE_MKDIR)
  return mkdir(szDirectoryName, 0774);
#else
  return -1;
#endif
}

//---------------------------------------------------------
int RemoveDirectory(const char* szDirectoryName)
{
#if defined (_MSC_VER)
  return _rmdir(szDirectoryName);
#elif defined (HAVE_MKDIR)
  return rmdir(szDirectoryName);
#else
  return -1;
#endif
}

//---------------------------------------------------------
int GetFileList(const char* szPattern, std::list<std::string>& lstMatches)
{
  lstMatches.clear();
#if defined (_MSC_VER)

  std::string base(szPattern);
  std::string::size_type pos = base.rfind('/');
  if(pos == std::string::npos)
    return -1;
  base.erase(pos);
  
  struct _finddata_t data;
  intptr_t lHandle = _findfirst(szPattern, &data);
  for (intptr_t nextHandle = lHandle; nextHandle != -1; nextHandle = _findnext(lHandle, &data)) {
    if(strcmp(data.name, ".") == 0 || strcmp(data.name, "..") == 0)
        continue;
    std::cout << ">" << data.name << "<\n";
    lstMatches.push_back(base + "/" + data.name);
  }

  _findclose(lHandle);
//
// I need to sort the list in ascending order before returning....
//
#elif defined (HAVE_GLOB)
  glob_t  globbuf;
  if (glob(szPattern, GLOB_ERR,0, &globbuf) != 0) {
    globfree(&globbuf);
    return -1;
  }

  for (unsigned int i(0); i < globbuf.gl_pathc; ++i) {
    std::string sFileName(globbuf.gl_pathv[i]);
    lstMatches.push_back(sFileName);
  }

  globfree(&globbuf);
#elif defined (HAVE_OPENDIR)
  DIR* dirstream(opendir(_sBaseName.c_str()));
  if (dirstream == 0) {
    return -1;
  }

  struct dirent* cur_dir_ent(0);
  struct stat fstat_buf;
  while ((cur_dir_ent = readdir(dirstream)) != 0) {
    string sFileName(cur->dir_ent->d_name);
    lstMatches.push_back(_sBaseName);
  }

  if (closedir(dirstream) != 0) {
    return -1;
  }
//
// I need to sort the list in ascending order before returning....
//
//
#endif
  return 0;
}

//-------------------------------------------------------------------
int GetFileInfo(const char* szFileName, FS::FileSystem::FileInformationType& pInfo)
{
#if defined (HAVE_STAT)
  struct stat fstat_buf;
  if (stat(szFileName, &fstat_buf) != 0) {
    return -1;
  }

  pInfo.size = fstat_buf.st_size;
#if defined (_WIN32)
  if (_S_IFREG & fstat_buf.st_mode) {
#else
  if (S_ISREG(fstat_buf.st_mode)) {
#endif
    pInfo.kind = FS::FileSystem::PLAIN;
#if defined (_WIN32)
  } else if (_S_IFDIR & fstat_buf.st_mode) {
#else
  } else if (S_ISDIR(fstat_buf.st_mode)) {
#endif
    pInfo.kind = FS::FileSystem::FDIRECTORY;
  }
#else
  return -1;
#endif
  pInfo.name = CORBA::string_dup(szFileName);
  pInfo.fileProperties = OSWrapper::GetFileProperties(szFileName);
  return 0;
}

};
