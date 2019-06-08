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
	_tprintf(TEXT(" OEM ID: %u (obsolete)\n"), sysInfo.dwOemId);
	_tprintf(TEXT(" Processor architecture: %04X ("), sysInfo.wProcessorArchitecture);
	switch (sysInfo.wProcessorArchitecture) {
	case PROCESSOR_ARCHITECTURE_AMD64:
		_tprintf(TEXT("x64 (AMD or Intel)"));
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		_tprintf(TEXT("ARM"));
		break;
	case PROCESSOR_ARCHITECTURE_ARM64:
		_tprintf(TEXT("ARM64"));
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		_tprintf(TEXT("Intel Itanium"));
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		_tprintf(TEXT("x86"));
		break;
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
		_tprintf(TEXT("Unknown"));
		break;
	default:
		_tprintf(TEXT("Unsupported value"));
	}
	_tprintf(TEXT(")\n"));
	_tprintf(TEXT(" Number of processors: %u\n"), sysInfo.dwNumberOfProcessors);
	_tprintf(TEXT(" Page size: %u\n"), sysInfo.dwPageSize);
	_tprintf(TEXT(" Processor type: %u (obsolete)\n"), sysInfo.dwProcessorType);
	_tprintf(TEXT(" Processor revision: %u (obsolete)\n"), sysInfo.wProcessorRevision);
	_tprintf(TEXT(" Processor level: %u\n"), sysInfo.wProcessorLevel);
	_tprintf(TEXT(" Low application address: %08X\n"), sysInfo.lpMinimumApplicationAddress);
	_tprintf(TEXT(" High application address: %08X\n"), sysInfo.lpMaximumApplicationAddress);
	_tprintf(TEXT(" Active processor mask: %08X\n"), sysInfo.dwActiveProcessorMask);
	_tprintf(TEXT(" Allocation granularity: %u\n"), sysInfo.dwAllocationGranularity);

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
