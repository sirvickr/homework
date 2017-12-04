#pragma once

enum RequestType {
	reqOsVer,
	reqSysTime,
	reqTickCount,
	reqMemStatus,
	reqDriveType,
	reqFreeSpace,
	reqAccessRights,
	reqObjectOwn,
};

struct AgentRequest {
	DWORD reqType;
	DWORD headSize;
	DWORD pathSize;
	DWORD userSize;
};

struct FreeSpaceReply {
	ULARGE_INTEGER freeBytesAvailable;
	ULARGE_INTEGER totalNumberOfBytes;
	ULARGE_INTEGER totalNumberOfFreeBytes;
};

struct AgentReply {
	int errorCode; // < 0 - сетевая ошибка клиента, 0 - успех, > 0 - код ошибки сервера
	DWORD reqType;
	union  {
		OSVERSIONINFOEX osVer;
		SYSTEMTIME sysTime;
		DWORD tickCount;
		MEMORYSTATUSEX memStatus;
		UINT driveType;
		FreeSpaceReply freeSpace;
		ACCESS_MASK accessMask;
		TCHAR objectOwner[1];
	} vf;
};
