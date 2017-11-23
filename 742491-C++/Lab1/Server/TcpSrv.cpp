#include "stdafx.h"
#include "TcpSrv.h"
#include "../AgentReply.h"

using namespace std;

#define xmalloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s))
#define xfree(p)   HeapFree(GetProcessHeap(),0,(p))

TcpSrv::TcpSrv(const char *port, BOOL verbose)
	: g_Port(port), g_bVerbose(verbose)
{
	SYSTEM_INFO systemInfo;
	WSADATA wsaData;

	for (int i = 0; i < MAX_WORKER_THREAD; i++) {
		g_ThreadHandles[i] = INVALID_HANDLE_VALUE;
	}

	GetSystemInfo(&systemInfo);
	g_dwThreadCount = 1;// systemInfo.dwNumberOfProcessors * 2;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	InitializeCriticalSection(&g_CriticalSection);
}


TcpSrv::~TcpSrv()
{
	DeleteCriticalSection(&g_CriticalSection);
	WSACleanup();
}

void TcpSrv::Run()
{
	PER_SOCKET_CONTEXT* lpPerSocketContext = NULL;
	DWORD dwRecvNumBytes = 0;
	DWORD dwFlags = 0;
	int nRet = 0;

	while (g_bRestart) {
		g_bRestart = FALSE;
		g_bEndServer = FALSE;

		__try {
			g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
			if (g_hIOCP == NULL) {
				cerr << "CreateIoCompletionPort() failed to create I/O completion port: " << GetLastError() << endl;
				__leave;
			}

			for (DWORD dwCPU = 0; dwCPU < g_dwThreadCount; dwCPU++) {

				// создаем рабочие потоки
				HANDLE hThread = INVALID_HANDLE_VALUE;
				DWORD dwThreadId = 0;

				hThread = CreateThread(NULL, 0, WorkerThread, (LPVOID)this, 0, &dwThreadId);
				if (hThread == NULL) {
					cerr << "CreateThread() failed: " << GetLastError() << endl;
					__leave;
				}
				g_ThreadHandles[dwCPU] = hThread;
				hThread = INVALID_HANDLE_VALUE;
			}

			if (!CreateListenSocket())
				__leave;

			while (TRUE) {

				// принимаем клиентские соединения в бесконечном цикле (пока не закрыта консоль)
				sdAccept = WSAAccept(g_sdListen, NULL, NULL, NULL, 0);
				if (sdAccept == SOCKET_ERROR) {

					// Если пользователь нажал hits Ctrl+C, Ctrl+Brk или закрыл консоль, 
					// обработчик этих событийзакроет сокет g_sdListen. Вызов WSAAccept 
					// вернул ошибку и мы выходим из цикла
					cerr << "WSAAccept() failed: " << WSAGetLastError() << endl;
					__leave;
				}

				// Добавляем сокет только что принятого соединения, вместе со своими 
				// дополнительными данными, в IOCP. Также добавляем эти данные список
				// "контекстных" структур
				lpPerSocketContext = UpdateCompletionPort(sdAccept, ClientIoRead, TRUE);
				if (lpPerSocketContext == NULL)
					__leave;

				// Если пользователь нажал hits Ctrl+C после вызова WSAAccept, обработчик
				// установил этот флаг и мы выходим из цикла раньше, чем добавим следующий поток
				if (g_bEndServer)
					break;

				// заказать начальный прием данных с этого сокета
				nRet = WSARecv(sdAccept, &(lpPerSocketContext->pIOContext->wsabuf),
					1, &dwRecvNumBytes, &dwFlags,
					&(lpPerSocketContext->pIOContext->Overlapped), NULL);
				if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) {
					cerr << "WSARecv() Failed: " << WSAGetLastError() << endl;
					CloseClient(lpPerSocketContext, FALSE);
				}
			} //while
		}

		__finally {

			g_bEndServer = TRUE;

			//
			// Cause worker threads to exit
			//
			if (g_hIOCP) {
				for (DWORD i = 0; i < g_dwThreadCount; i++)
					PostQueuedCompletionStatus(g_hIOCP, 0, 0, NULL);
			}

			//
			//Make sure worker threads exits.
			//
			if (WAIT_OBJECT_0 != WaitForMultipleObjects(g_dwThreadCount, g_ThreadHandles, TRUE, 1000))
				cerr << "WaitForMultipleObjects() failed: " << GetLastError() << endl;
			else {
				for (DWORD i = 0; i < g_dwThreadCount; i++) {
					if (g_ThreadHandles[i] != INVALID_HANDLE_VALUE)
						CloseHandle(g_ThreadHandles[i]);
					g_ThreadHandles[i] = INVALID_HANDLE_VALUE;
				}
			}

			CtxtListFree();

			if (g_hIOCP) {
				CloseHandle(g_hIOCP);
				g_hIOCP = NULL;
			}

			if (g_sdListen != INVALID_SOCKET) {
				closesocket(g_sdListen);
				g_sdListen = INVALID_SOCKET;
			}

			if (sdAccept != INVALID_SOCKET) {
				closesocket(sdAccept);
				sdAccept = INVALID_SOCKET;
			}

		} //finally

		if (g_bRestart) {
			cout << "\niocpserver is restarting..." << endl;
		}
		else {
			cout << "\niocpserver is exiting..." << endl;
		}

	} //while (g_bRestart)
}

void TcpSrv::Restart() 
{
	g_bRestart = TRUE;
}

void TcpSrv::Shutdown()
{
	cout << "closing listening socket" << endl;
	SOCKET sockTemp = g_sdListen;
	g_sdListen = INVALID_SOCKET;
	g_bEndServer = TRUE;
	closesocket(sockTemp);
	sockTemp = INVALID_SOCKET;
}

BOOL TcpSrv::CreateListenSocket(void)
{
	int nRet = 0;
	struct addrinfo hints = { 0 };
	struct addrinfo *addrlocal = NULL;

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_IP;

	if (getaddrinfo(NULL, g_Port, &hints, &addrlocal) != 0) {
		cerr << "getaddrinfo() failed with error " << WSAGetLastError() << endl;
		return FALSE;
	}

	if (addrlocal == NULL) {
		cerr << "getaddrinfo() failed to resolve/convert the interface" << endl;
		return FALSE;
	}

	g_sdListen = WSASocket(addrlocal->ai_family, addrlocal->ai_socktype, 
		addrlocal->ai_protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (g_sdListen == INVALID_SOCKET) {
		cerr << "WSASocket(g_sdListen) failed: " << WSAGetLastError() << endl;
		return FALSE;
	}

	nRet = bind(g_sdListen, addrlocal->ai_addr, (int)addrlocal->ai_addrlen);
	if (nRet == SOCKET_ERROR) {
		cerr << "bind() failed: " << WSAGetLastError() << endl;
		return FALSE;
	}

	nRet = listen(g_sdListen, 5);
	if (nRet == SOCKET_ERROR) {
		cerr << "listen() failed: " << WSAGetLastError() << endl;
		return FALSE;
	}

	// Отключить буферизацию при отправке данных через сокет
	int nZero = 0;
	nRet = setsockopt(g_sdListen, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));
	if (nRet == SOCKET_ERROR) {
		cerr << "setsockopt(SNDBUF) failed: " << WSAGetLastError() << endl;
		return FALSE;
	}

	freeaddrinfo(addrlocal);

	return TRUE;
}

PER_SOCKET_CONTEXT* TcpSrv::UpdateCompletionPort(SOCKET s, IO_OPERATION ClientIo, BOOL bAddToList)
{
	PER_SOCKET_CONTEXT *lpPerSocketContext = CtxtAllocate(s, ClientIo);
	if (lpPerSocketContext == NULL)
		return NULL;

	g_hIOCP = CreateIoCompletionPort((HANDLE)s, g_hIOCP, (DWORD_PTR)lpPerSocketContext, 0);
	if (g_hIOCP == NULL) {
		cerr << "CreateIoCompletionPort() failed: " << GetLastError() << endl;
		if (lpPerSocketContext->pIOContext)
			xfree(lpPerSocketContext->pIOContext);
		xfree(lpPerSocketContext);
		return NULL;
	}

	// В список не добавляются только контексты слушающего (bAddToList == FALSE) сокета 
	if (bAddToList) 
		CtxtListAddTo(lpPerSocketContext);

	if (g_bVerbose)
		cerr << "UpdateCompletionPort: Socket(" << lpPerSocketContext->Socket << ") added to IOCP" << endl;

	return lpPerSocketContext;
}

void TcpSrv::CloseClient(PER_SOCKET_CONTEXT* lpPerSocketContext, BOOL bGraceful)
{
	EnterCriticalSection(&g_CriticalSection);

	if (lpPerSocketContext) {
		if (g_bVerbose)
			cout << "CloseClient: Socket(" << lpPerSocketContext->Socket << ") "
				<< "connection closing (graceful=" << bGraceful << ")" << endl;
		if (!bGraceful) {
			// force the subsequent closesocket to be abortative.
			LINGER  lingerStruct;
			lingerStruct.l_onoff = 1;
			lingerStruct.l_linger = 0;
			setsockopt(lpPerSocketContext->Socket, SOL_SOCKET, SO_LINGER,
				(char *)&lingerStruct, sizeof(lingerStruct));
		}
		closesocket(lpPerSocketContext->Socket);
		lpPerSocketContext->Socket = INVALID_SOCKET;
		CtxtListDeleteFrom(lpPerSocketContext);
		lpPerSocketContext = NULL;
	}
	else {
		cerr << "CloseClient: lpPerSocketContext is NULL" << endl;
	}

	LeaveCriticalSection(&g_CriticalSection);
}

PER_SOCKET_CONTEXT* TcpSrv::CtxtAllocate(SOCKET s, IO_OPERATION ClientIO)
{
	PER_SOCKET_CONTEXT *lpPerSocketContext;

	EnterCriticalSection(&g_CriticalSection);

	lpPerSocketContext = (PER_SOCKET_CONTEXT*)xmalloc(sizeof(PER_SOCKET_CONTEXT));
	if (lpPerSocketContext) {
		lpPerSocketContext->pIOContext = (PER_IO_CONTEXT*)xmalloc(sizeof(PER_IO_CONTEXT));
		if (lpPerSocketContext->pIOContext) {
			lpPerSocketContext->Socket = s;
			lpPerSocketContext->pCtxtBack = NULL;
			lpPerSocketContext->pCtxtForward = NULL;

			lpPerSocketContext->pIOContext->Overlapped.Internal = 0;
			lpPerSocketContext->pIOContext->Overlapped.InternalHigh = 0;
			lpPerSocketContext->pIOContext->Overlapped.Offset = 0;
			lpPerSocketContext->pIOContext->Overlapped.OffsetHigh = 0;
			lpPerSocketContext->pIOContext->Overlapped.hEvent = NULL;
			lpPerSocketContext->pIOContext->IOOperation = ClientIO;
			lpPerSocketContext->pIOContext->pIOContextForward = NULL;
			lpPerSocketContext->pIOContext->nTotalBytes = 0;
			lpPerSocketContext->pIOContext->nSentBytes = 0;
			lpPerSocketContext->pIOContext->wsabuf.buf = lpPerSocketContext->pIOContext->Buffer;
			lpPerSocketContext->pIOContext->wsabuf.len = sizeof(lpPerSocketContext->pIOContext->Buffer);

			ZeroMemory(lpPerSocketContext->pIOContext->wsabuf.buf, lpPerSocketContext->pIOContext->wsabuf.len);
		}
		else {
			xfree(lpPerSocketContext);
			cerr << "HeapAlloc() PER_IO_CONTEXT failed: " << GetLastError() << endl;
		}

	}
	else {
		cerr << "HeapAlloc() PER_SOCKET_CONTEXT failed: " << GetLastError() << endl;
	}

	LeaveCriticalSection(&g_CriticalSection);

	return lpPerSocketContext;
}

void TcpSrv::CtxtListAddTo(PER_SOCKET_CONTEXT* lpPerSocketContext)
{
	EnterCriticalSection(&g_CriticalSection);

	if (g_pCtxtList == NULL) {
		// добавить первый элемент в связный список
		lpPerSocketContext->pCtxtBack = NULL;
		lpPerSocketContext->pCtxtForward = NULL;
		g_pCtxtList = lpPerSocketContext;
	}
	else {
		// добавить элемент в начало списка
		PER_SOCKET_CONTEXT *pTemp = g_pCtxtList;

		g_pCtxtList = lpPerSocketContext;
		lpPerSocketContext->pCtxtBack = pTemp;
		lpPerSocketContext->pCtxtForward = NULL;

		pTemp->pCtxtForward = lpPerSocketContext;
	}

	LeaveCriticalSection(&g_CriticalSection);
}

void TcpSrv::CtxtListDeleteFrom(PER_SOCKET_CONTEXT* lpPerSocketContext)
{
	EnterCriticalSection(&g_CriticalSection);

	if (lpPerSocketContext) {
		PER_SOCKET_CONTEXT *pBack = lpPerSocketContext->pCtxtBack;
		PER_SOCKET_CONTEXT *pForward = lpPerSocketContext->pCtxtForward;

		if ((pBack == NULL) && (pForward == NULL)) {
			// удаляем единственный элемент
			g_pCtxtList = NULL;
		}
		else if ((pBack == NULL) && (pForward != NULL)) {
			// удаляем начальный элемент
			pForward->pCtxtBack = NULL;
			g_pCtxtList = pForward;
		}
		else if ((pBack != NULL) && (pForward == NULL)) {
			// удаляем конечный элемент
			pBack->pCtxtForward = NULL;
		}
		else if (pBack && pForward) {
			// удаляем внутренний элемент
			pBack->pCtxtForward = pForward;
			pForward->pCtxtBack = pBack;
		}

		// освобождаем все память всех контекстных структур сокета
		PER_IO_CONTEXT *pTempIO = (PER_IO_CONTEXT*)(lpPerSocketContext->pIOContext);
		PER_IO_CONTEXT *pNextIO = NULL;
		do {
			pNextIO = (PER_IO_CONTEXT*)(pTempIO->pIOContextForward);
			if (pTempIO) {
 				// безопасно удалить структуру можно только после
				// завершения асинхронной операции ввода/вывода
				if (g_bEndServer)
					while (!HasOverlappedIoCompleted((LPOVERLAPPED)pTempIO)) Sleep(0);
				xfree(pTempIO);
				pTempIO = NULL;
			}
			pTempIO = pNextIO;
		} while (pNextIO);

		xfree(lpPerSocketContext);
		lpPerSocketContext = NULL;

	}
	else {
		cerr << "CtxtListDeleteFrom: lpPerSocketContext is NULL" << endl;
	}

	LeaveCriticalSection(&g_CriticalSection);
}

void TcpSrv::CtxtListFree()
{
	EnterCriticalSection(&g_CriticalSection);

	PER_SOCKET_CONTEXT *pTemp1 = g_pCtxtList;
	while (pTemp1) {
		PER_SOCKET_CONTEXT *pTemp2 = pTemp1->pCtxtBack;
		CloseClient(pTemp1, FALSE);
		pTemp1 = pTemp2;
	}

	LeaveCriticalSection(&g_CriticalSection);
}

// static
DWORD WINAPI TcpSrv::WorkerThread(LPVOID lpParam)
{
	TcpSrv *self = (TcpSrv*)lpParam;
	HANDLE hIOCP = self->g_hIOCP;
	BOOL bSuccess = FALSE;
	int nRet = 0;
	LPWSAOVERLAPPED lpOverlapped = NULL;
	PER_SOCKET_CONTEXT *lpPerSocketContext = NULL;
	PER_IO_CONTEXT *lpIOContext = NULL;
	WSABUF buffRecv;
	WSABUF buffSend;
	DWORD dwRecvNumBytes = 0;
	DWORD dwSendNumBytes = 0;
	DWORD dwFlags = 0;
	DWORD dwIoSize = 0;

	// бесконечный цикл для обработки пакетов завершения
	while (TRUE) {

		bSuccess = GetQueuedCompletionStatus(hIOCP, &dwIoSize,
			(PDWORD_PTR)&lpPerSocketContext,
			(LPOVERLAPPED *)&lpOverlapped,
			INFINITE);
		if (!bSuccess)
			cerr << "GetQueuedCompletionStatus() failed: " << GetLastError() << endl;

		if (lpPerSocketContext == NULL) {
			// Обработчик нажатия CTRL-C с помощью PostQueuedCompletionStatus 
			// отправил пакет с CompletionKey = NULL, поэтому выходим
			break;
		}

		if (self->g_bEndServer) {
			// еще один признак завершения
			break;
		}

		if (!bSuccess || (bSuccess && (dwIoSize == 0))) {
			// клиентское соединение сброшено, продолжаем обрабатывать 
			// оставшиеся и вновь поступающие соединения
			self->CloseClient(lpPerSocketContext, FALSE);
			continue;
		}

		DWORD dwReqType = 0;
		AgentReply reply;
		// определим тип пакета завершения с помощью проверки 
		// структуры PER_IO_CONTEXT, ассоциированной с этим сокетом
		lpIOContext = (PER_IO_CONTEXT*)lpOverlapped;
		switch (lpIOContext->IOOperation) {
		case ClientIoRead:
			// завершена операция чтения, отправляем назад результат запроса,
			// используя тот же контекст
			dwIoSize = sizeof(reply);
			memcpy(&dwReqType, lpIOContext->wsabuf.buf, sizeof(dwReqType));
			cout << "got request: " << dwReqType << endl;
			lpIOContext->IOOperation = ClientIoWrite;
			lpIOContext->nTotalBytes = dwIoSize;
			lpIOContext->nSentBytes = 0;
			reply.reqType = dwReqType + 1;
			memcpy(lpIOContext->wsabuf.buf, &reply, dwIoSize);
			lpIOContext->wsabuf.len = dwIoSize;
			dwFlags = 0;
			nRet = WSASend(lpPerSocketContext->Socket, &lpIOContext->wsabuf, 1,
				&dwSendNumBytes, dwFlags, &(lpIOContext->Overlapped), NULL);
			if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) {
				cerr << "WSASend() failed: " << WSAGetLastError() << endl;
				self->CloseClient(lpPerSocketContext, FALSE);
			}
			else if (self->g_bVerbose) {
				cout << "WorkerThread " << GetCurrentThreadId() << ": "
					<< "Socket(" << lpPerSocketContext->Socket << ") "
					<< "Recv completed (" << dwIoSize << " bytes)" << endl;
			}
			break;

		case ClientIoWrite:
			// завершена операция записи, убеждаемся, что отправлены все данные
			lpIOContext->IOOperation = ClientIoWrite;
			lpIOContext->nSentBytes += dwIoSize;
			dwFlags = 0;
			if (lpIOContext->nSentBytes < lpIOContext->nTotalBytes) {

				// предыдущая операция записи смогла отправить не все данные
				// отправляем еще один запрос - на отпрвку оставшихся данных
				buffSend.buf = lpIOContext->Buffer + lpIOContext->nSentBytes;
				buffSend.len = lpIOContext->nTotalBytes - lpIOContext->nSentBytes;
				nRet = WSASend(lpPerSocketContext->Socket, &buffSend, 1,
					&dwSendNumBytes, dwFlags, &(lpIOContext->Overlapped), NULL);
				if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) {
					cerr << "WSASend() failed: " << WSAGetLastError() << endl;
					self->CloseClient(lpPerSocketContext, FALSE);
				}
				else if (self->g_bVerbose) {
					cout << "WorkerThread " << GetCurrentThreadId() << ": "
						<< "Socket(" << lpPerSocketContext->Socket << ") "
						<< "Send partially completed (" << dwIoSize << " bytes)" << endl;
				}
			}
			else {
				// предыдущая операция записи полностью завершена, отправляем запрос на прием
				lpIOContext->IOOperation = ClientIoRead;
				dwRecvNumBytes = 0;
				dwFlags = 0;
				buffRecv.buf = lpIOContext->Buffer;
				buffRecv.len = MAX_BUFF_SIZE;
				nRet = WSARecv(lpPerSocketContext->Socket, &buffRecv, 1,
					&dwRecvNumBytes, &dwFlags, &lpIOContext->Overlapped, NULL);
				if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError())) {
					cerr << "WSARecv() failed: " << WSAGetLastError() << endl;
					self->CloseClient(lpPerSocketContext, FALSE);
				}
				else if (self->g_bVerbose) {
					cout << "WorkerThread " << GetCurrentThreadId() << ": "
						<< "Socket(" << lpPerSocketContext->Socket << ") "
						<< "Send completed (" << dwIoSize << " bytes)" << endl;
				}
			}
			break;

		} //switch
	} //while

	return 0;
}
