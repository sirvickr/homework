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
	} vf;
};