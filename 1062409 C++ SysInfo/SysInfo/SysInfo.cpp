#include "pch.h"
#include <windows.h>
#include <tchar.h>

const int BufferSize = 4096;

TCHAR infoBuf[BufferSize];

int main()
{
	SYSTEM_INFO sysInfo;
	DWORD bufCharCount = BufferSize;

	// Отобразить общую аппаратную информацию
	GetSystemInfo(&sysInfo);
	_tprintf(TEXT("Hardware information: \n"));
	_tprintf(TEXT("  OEM ID: %u\n"), sysInfo.dwOemId);
	_tprintf(TEXT("  Number of processors: %u\n"), sysInfo.dwNumberOfProcessors);
	_tprintf(TEXT("  Page size: %u\n"), sysInfo.dwPageSize);
	_tprintf(TEXT("  Processor type: %u\n"), sysInfo.dwProcessorType);
	_tprintf(TEXT("  Minimum application address: %lx\n"), sysInfo.lpMinimumApplicationAddress);
	_tprintf(TEXT("  Maximum application address: %lx\n"), sysInfo.lpMaximumApplicationAddress);
	_tprintf(TEXT("  Active processor mask: %u"), sysInfo.dwActiveProcessorMask);

	// Отобразить имя компьютера
	if (!GetComputerName(infoBuf, &bufCharCount)) {
		_tprintf(TEXT("\nGetComputerName failed"));
		return -1;
	}
	_tprintf(TEXT("\nComputer name:\t%s"), infoBuf);

	// Отобразить имя пользователя
	bufCharCount = BufferSize;
	if (!GetUserName(infoBuf, &bufCharCount)) {
		_tprintf(TEXT("\GetUserName failed"));
		return -1;
	}
	_tprintf(TEXT("\nUser name:\t%s"), infoBuf);

	// Отобразить системный каталог
	if (!GetSystemDirectory(infoBuf, BufferSize)) {
		_tprintf(TEXT("GetSystemDirectory"));
		return -1;
	}
	_tprintf(TEXT("\nSystem Directory:\t%s"), infoBuf);

	// Отобразить каталог операционной системы
	if (!GetWindowsDirectory(infoBuf, BufferSize)) {
		_tprintf(TEXT("GetWindowsDirectory"));
		return -1;
	}
	_tprintf(TEXT("\nWindows Directory:\t%s"), infoBuf);

	_tprintf(TEXT("\n\nPress ENTER to quit "));

	_fgettc(stdin);
}
