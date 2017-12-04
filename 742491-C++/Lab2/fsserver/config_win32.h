/***************************************************************************
                          config_win32.h  -  description
                             -------------------
    begin                : Fri Apr 18 2003
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

#if ! defined (CONFIG_WIN32_H)
#define CONFIG_WIN32_H

#define HAVE_SPRINTF   1
#define HAVE_TIME_H    1
#define HAVE_WINDOWS_H 1

const char *VERSION = "1.0";
const char *PACKAGE = "CISCA";


#endif /* CONFIG_WIN32_H */