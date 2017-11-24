#pragma once

#include <mswsock.h> // эксперимент с AcceptEx

#define DEFAULT_PORT        "5001"
#define MAX_BUFF_SIZE       8192
#define MAX_WORKER_THREAD   16

enum IO_OPERATION {
	ClientIoAccept,
	ClientIoRead,
	ClientIoWrite
};

// данные, которые будут привязаны к каждой операции ввода-вывода сокета
struct PER_IO_CONTEXT {
	WSAOVERLAPPED  Overlapped;
	char           Buffer[MAX_BUFF_SIZE];
	WSABUF         wsabuf;
	int            nTotalBytes;
	int            nSentBytes;
	IO_OPERATION   IOOperation;
	SOCKET         SocketAccept;
	PER_IO_CONTEXT *pIOContextForward;
};

// данные, которые будут привязаны к каждому сокету, связанному с портом завершения
struct PER_SOCKET_CONTEXT {
	SOCKET              Socket;
//	LPFN_ACCEPTEX       fnAcceptEx;
	// связный список для всех операций ввода вывода на сокете
	PER_IO_CONTEXT      *pIOContext;
	PER_SOCKET_CONTEXT  *pCtxtBack;
	PER_SOCKET_CONTEXT  *pCtxtForward;
};

class TcpSrv
{
public:
	TcpSrv(const char *port = DEFAULT_PORT, BOOL verbose = FALSE);
	~TcpSrv();
	void Run();
	void Restart();
	void Shutdown();

private:
	BOOL CreateListenSocket(void);
	PER_SOCKET_CONTEXT* UpdateCompletionPort(
		SOCKET s,
		IO_OPERATION ClientIo,
		BOOL bAddToList // FALSE для серверного, TRUE для клиентского сокета
	);
	void CloseClient(PER_SOCKET_CONTEXT* lpPerSocketContext, BOOL bGraceful);
	PER_SOCKET_CONTEXT* CtxtAllocate(SOCKET s, IO_OPERATION ClientIO);
	void CtxtListAddTo(PER_SOCKET_CONTEXT* lpPerSocketContext);
	void CtxtListDeleteFrom(PER_SOCKET_CONTEXT* lpPerSocketContext);
	void CtxtListFree();
	static DWORD WINAPI WorkerThread(LPVOID lpParam);

private:
	const char *g_Port;
	BOOL g_bVerbose;
	BOOL g_bEndServer = FALSE;			// set to TRUE on CTRL-C
	BOOL g_bRestart = TRUE;				// set to TRUE to CTRL-BRK
	DWORD g_dwThreadCount = 0;
	HANDLE g_hIOCP = INVALID_HANDLE_VALUE;
	SOCKET g_sdListen = INVALID_SOCKET;
	SOCKET sdAccept = INVALID_SOCKET;
	HANDLE g_ThreadHandles[MAX_WORKER_THREAD];
	PER_SOCKET_CONTEXT *g_pCtxtList = NULL; // список контекстных структур
	CRITICAL_SECTION g_CriticalSection;		// защита доступа к этому списку
	// TODO пока только для одного потока
	char InBuf[MAX_BUFF_SIZE];
	int  nTotalRecv;
};
