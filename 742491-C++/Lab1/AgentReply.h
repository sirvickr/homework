#pragma once

enum RequestType {
	reqOsVer,
	reqSysTime,
	reqTickCount,
	reqMemStatus,
	reqDriveType,
	reqFreeSpace,
	reqObjectAcl,
	reqObjectOwn,
};

struct AgentRequest {
	DWORD reqSize;
	DWORD reqType;
	TCHAR objectPath[1];
};

struct AgentReply {
	int errorCode; // > 0 - сетевая ошибка клиента, 0 - успех, > 0 - код ошибки сервера
	DWORD reqType;
	union  {
		OSVERSIONINFOEX osVer;
		SYSTEMTIME sysTime;
		DWORD tickCount;
		MEMORYSTATUSEX memStatus;
		UINT driveType;
		struct {
			ULARGE_INTEGER freeBytesAvailable;
			ULARGE_INTEGER totalNumberOfBytes;
			ULARGE_INTEGER totalNumberOfFreeBytes;
		} freeSpace;
		ACL acl;
		struct {
			DWORD size;
			TCHAR name[1];
		} objectOwn;
	} vf;
};
