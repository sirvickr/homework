// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <ace/Recursive_Thread_Mutex.h>
#include <ace/Argv_Type_Converter.h>
#include <ace/Get_Opt.h>
#include <ace/Thread_Manager.h>

#include <tao/PortableServer/PortableServer.h>
#include <tao/PortableServer/Servant_Base.h>
#include <orbsvcs/Naming/Naming_Server.h>
#include <ace/Functor_T.h>
#include <ace/SString.h>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <map>

//#include <winsock2.h>
//#include <Ws2tcpip.h>
#include <Aclapi.h>
#include <authz.h>
#include <Wincrypt.h>

#include "../unidefs.h"
