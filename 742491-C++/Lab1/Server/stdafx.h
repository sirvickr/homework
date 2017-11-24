// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <Aclapi.h>
#include <authz.h>
#include <sstream>
#include <iostream>
#include <iomanip>

typedef std::basic_string<TCHAR> tstring;
typedef std::basic_ostream<TCHAR> tostream;
typedef std::basic_istream<TCHAR> tistream;
typedef std::basic_ostringstream<TCHAR> tostringstream;
typedef std::basic_istringstream<TCHAR> tistringstream;

#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#define tcerr std::wcerr
#else
#define tcout std::cout
#define tcerr std::cerr
#endif
