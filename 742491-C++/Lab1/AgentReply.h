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
		DWORD ownerNameSize;
	} vf;
};