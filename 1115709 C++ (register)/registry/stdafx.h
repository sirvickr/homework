// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// нечасто изменяются
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows
#include <windows.h>
#include <commctrl.h> // InitCommonControls
#include <shellapi.h> // CommandLineToArgvW

// Файлы заголовков среды выполнения C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <functional>
#include <algorithm>

using tstring = std::basic_string<TCHAR>;
using tistream = std::basic_istream<TCHAR>;
using tostream = std::basic_ostream<TCHAR>;
using tostringstream = std::basic_ostringstream<TCHAR>;
using tistringstream = std::basic_istringstream<TCHAR>;
#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#define tcerr std::wcerr
#define tofstream std::wofstream
#define tifstream std::wifstream
#define to_tstring std::to_wstring
#else
#define tcout std::cout
#define tcerr std::cerr
#define tofstream std::ofstream
#define tifstream std::ifstream
#define to_tstring std::to_string
#endif
