#pragma once

#include "../AgentReply.h"

class CTcpClient
{
public:
	CTcpClient(void);
	~CTcpClient(void);

	static const size_t MAXTHREADS = 64;
	static const size_t DEFAULT_BUFLEN = 1024;

	struct THREADINFO {
		HANDLE hThread[MAXTHREADS];
		SOCKET sd[MAXTHREADS];
	};
	
	int GetOsVer(OSVERSIONINFOEX &result, LPCTSTR lpszServer, LPCTSTR lpszPort);
	int GetSysTime(SYSTEMTIME &result, LPCTSTR lpszServer, LPCTSTR lpszPort);
	int GetTickCount(DWORD &result, LPCTSTR lpszServer, LPCTSTR lpszPort);
	int GetMemStatus(MEMORYSTATUSEX &result, LPCTSTR lpszServer, LPCTSTR lpszPort);
	int GetDriveType(UINT &result, LPCTSTR lpszServer, LPCTSTR lpszPort, LPCTSTR objPath);
	int GetDiskFreeSpace(FreeSpaceReply &result, LPCTSTR lpszServer, LPCTSTR lpszPort, LPCTSTR objPath);
	int GetAccessRights(ACCESS_MASK &result, LPCTSTR lpszServer, LPCTSTR lpszPort, LPCTSTR objPath, LPCTSTR userName);
	int GetObjectOwner(LPTSTR lpszResult, DWORD maxResultSize, LPCTSTR lpszServer, LPCTSTR lpszPort, LPCTSTR objPath);

protected:
	AgentRequest PrepareRequest(DWORD reqType, LPCTSTR objPath = NULL, LPCTSTR userName = NULL);
	SOCKET Connect(LPCTSTR lpszServer, LPCTSTR lpszPort);
	int SendRequest(SOCKET sock, const AgentRequest &requestt, LPCTSTR objPath = NULL, LPCTSTR userName = NULL);
	AgentReply* RecvReply(SOCKET sock);
	int MakeRequest(AgentReply &result, LPCTSTR lpszServer, LPCTSTR lpszPort, DWORD reqType, LPCTSTR objPath = NULL, LPCTSTR userName = NULL);

private:
	THREADINFO g_ThreadInfo;
	BOOL g_bEndClient;
	WSAEVENT g_hCleanupEvent[1];
	char recvbuf[DEFAULT_BUFLEN];
	LPBYTE m_pbDecoded = NULL;
	DWORD m_cbDecoded = 0;
	PCHAR m_pbSendBuf = NULL;
	DWORD m_cbContent = 0;
	//SOCKET sock;

};
