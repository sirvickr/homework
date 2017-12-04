/***************************************************************************
                          os.h  -  description
                             -------------------
    begin                : Thu Mar 27 2003
    copyright            : (C) 2003 by Greg Lanthier
    email                : glanthie@chat.carleton.ca
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place, Suite 330,                                           *
 *   Boston, MA                                                            *
 *   02111-1307  USA                                                       *
 *                                                                         *
 ***************************************************************************/
#ifndef HAVE_CONFIG_H
#define HAVE_CONFIG_H
#endif // HAVE_CONFIG_H
#if defined (HAVE_CONFIG_H)
#include "config.h"
#endif

#if defined (ORB_MICO)
#include "fs.h"
#elif defined (ORB_ACETAO)
#include "fsS.h"
#elif defined (ORB_ORBIX2000)
#include "fsS.hh"
#elif defined (ORB_VISIBROKER)
#include "fs_s.hh"
#endif

namespace OSWrapper
{

/**
 * Answers how much space is left on the host file system.
 * <p>
 * @param szFileName path to a file or directory on the host filesystem.
 * @returns number of available bytes remaining on the host file system.
 */
CORBA::ULongLong GetAvailableSpaceOnFileSystem(const char* szFileName);

/**
 * Answers the total size of the host filesystem.
 * <p>
 * @param szFileName path to a file or directory on the host filesystem.
 * @returns total size of the filesystem in bytes.
 */
CORBA::ULongLong GetTotalSpaceOnFileSystem(const char* szFileName);

/**
 * Answers the length of the specified file in bytes.
 * @param szFileName
 * @returns size of file on success, zero on error.
 */
CORBA::ULong GetFileSize(const char* szFileName);

FS::Properties GetFileProperties(const char* szFileName)
    throw(
        ::CORBA::SystemException,
        ::FS::FileException);

/**
 * Answers true or false if the specified file exists on the host file system.
 * <p>
 * @param szFileName path to file according to the host operating system
 * @returns true if the file exists, false otherwise.
 * @throws ::CORBA::SystemException
 */
CORBA::Boolean GetFileExists( const char* szFileName )
    throw(
        ::CORBA::SystemException);

/**
 * Creates a directory on the host filesystem.
 * <p>
 * If a "C" API function for creating directories cannot be found this function
 * will become a no-op which returns non-zero.
 * @param szDirectoryName full path name for the new directory according to the host operating system.
 * @returns 0 on success, non-zero otherwise
 */
int MakeDirectory(const char* szDirectoryName);

/**
 * Removes a directory from the host filesystem.
 * <p>
 * If a "C" API function for removing directories cannot be found this function
 * will become a no-op which returns non-zero.
 * @param szDirectoryName full path name for the new directory according to the host operating system.
 * @returns 0 on success, non-zero otherwise.
 */
int RemoveDirectory(const char* szDirectoryName);

/**
 * Answers a list of files on the host filesystem.
 * @param szPattern pattern specifier.
 * @param lstMatches reference to a container that will hold on to the file names.
 * @returns 0 on success, non-zero otherwise
 */
int GetFileList(const char* szPattern, std::list<std::string>& lstMatches);

/**
 * Retrieves information about the specified file.
 * @param szFileName full path to the file according to the host operating system
 * @param pInfo reference to info type.
 * @returns 0 on success, non-zero otherwise
 */
int GetFileInfo(const char* szFileName, FS::FileSystem::FileInformationType& pInfo);

};
