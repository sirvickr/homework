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
	DWORD reqType;
	DWORD pathSize;
	TCHAR objectPath[1];
};

struct AgentReply {
	BOOL valid;
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
