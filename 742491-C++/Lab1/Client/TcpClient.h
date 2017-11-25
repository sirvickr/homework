#pragma once

#include "../AgentReply.h"

class CTcpClient
{
public:
	CTcpClient(void);
	~CTcpClient(void);

	static const size_t MAXTHREADS = 64;

	struct THREADINFO {
		HANDLE hThread[MAXTHREADS];
		SOCKET sd[MAXTHREADS];
	};
	
	AgentReply Request(LPCTSTR lpszServer, LPCTSTR lpszPort, const AgentRequest &request, LPCTSTR objPath = NULL);
	/* TODO
	virtual OSVERSIONINFOEX GetOsVer(const char* lpszServer, const char* lpszPort);
	virtual SYSTEMTIME GetSysTime(const char* lpszServer, const char* lpszPort);
	...
	*/

private:
	THREADINFO g_ThreadInfo;
	BOOL g_bEndClient;
	WSAEVENT g_hCleanupEvent[1];
	//SOCKET sock;

	BOOL CreateConnectedSocket(int nThreadNum);
	BOOL SendBuffer(int nThreadNum, char *outbuf);
	BOOL RecvBuffer(int nThreadNum, char *inbuf);
	static DWORD WINAPI EchoThread(LPVOID lpParameter);
};

