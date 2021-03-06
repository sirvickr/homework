#include "stdafx.h"
#include "WinApiHelper.h"
#include "TcpSrv.h"
#include "CryptoAPI.h"
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

	nTotalRecv = 0;

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

				// ������� ������� ������
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

				// ��������� ���������� ���������� � ����������� ����� (���� �� ������� �������)
				sdAccept = WSAAccept(g_sdListen, NULL, NULL, NULL, 0);
				if (sdAccept == SOCKET_ERROR) {

					// ���� ������������ ����� hits Ctrl+C, Ctrl+Brk ��� ������ �������, 
					// ���������� ���� �������������� ����� g_sdListen. ����� WSAAccept 
					// ������ ������ � �� ������� �� �����
					cerr << "WSAAccept() failed: " << WSAGetLastError() << endl;
					__leave;
				}

				// ��������� ����� ������ ��� ��������� ����������, ������ �� ������ 
				// ��������������� �������, � IOCP. ����� ��������� ��� ������ ������
				// "�����������" ��������
				lpPerSocketContext = UpdateCompletionPort(sdAccept, ClientIoRead, TRUE);
				if (lpPerSocketContext == NULL)
					__leave;

				// ���� ������������ ����� hits Ctrl+C ����� ������ WSAAccept, ����������
				// ��������� ���� ���� � �� ������� �� ����� ������, ��� ������� ��������� �����
				if (g_bEndServer)
					break;

				// �������� ��������� ����� ������ � ����� ������
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

	nRet = ::bind(g_sdListen, addrlocal->ai_addr, (int)addrlocal->ai_addrlen);
	if (nRet == SOCKET_ERROR) {
		cerr << "bind() failed: " << WSAGetLastError() << endl;
		return FALSE;
	}

	nRet = listen(g_sdListen, 5);
	if (nRet == SOCKET_ERROR) {
		cerr << "listen() failed: " << WSAGetLastError() << endl;
		return FALSE;
	}

	// ��������� ����������� ��� �������� ������ ����� �����
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

	// � ������ �� ����������� ������ ��������� ���������� (bAddToList == FALSE) ������ 
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
		// �������� ������ ������� � ������� ������
		lpPerSocketContext->pCtxtBack = NULL;
		lpPerSocketContext->pCtxtForward = NULL;
		g_pCtxtList = lpPerSocketContext;
	}
	else {
		// �������� ������� � ������ ������
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
			// ������� ������������ �������
			g_pCtxtList = NULL;
		}
		else if ((pBack == NULL) && (pForward != NULL)) {
			// ������� ��������� �������
			pForward->pCtxtBack = NULL;
			g_pCtxtList = pForward;
		}
		else if ((pBack != NULL) && (pForward == NULL)) {
			// ������� �������� �������
			pBack->pCtxtForward = NULL;
		}
		else if (pBack && pForward) {
			// ������� ���������� �������
			pBack->pCtxtForward = pForward;
			pForward->pCtxtBack = pBack;
		}

		// ����������� ��� ������ ���� ����������� �������� ������
		PER_IO_CONTEXT *pTempIO = (PER_IO_CONTEXT*)(lpPerSocketContext->pIOContext);
		PER_IO_CONTEXT *pNextIO = NULL;
		do {
			pNextIO = (PER_IO_CONTEXT*)(pTempIO->pIOContextForward);
			if (pTempIO) {
 				// ��������� ������� ��������� ����� ������ �����
				// ���������� ����������� �������� �����/������
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

	// ����������� ���� ��� ��������� ������� ����������
	while (TRUE) {

		bSuccess = GetQueuedCompletionStatus(hIOCP, &dwIoSize,
			(PDWORD_PTR)&lpPerSocketContext,
			(LPOVERLAPPED *)&lpOverlapped,
			INFINITE);
		if (!bSuccess)
			cerr << "GetQueuedCompletionStatus() failed: " << GetLastError() << endl;

		if (lpPerSocketContext == NULL) {
			// ���������� ������� CTRL-C � ������� PostQueuedCompletionStatus 
			// �������� ����� � CompletionKey = NULL, ������� �������
			break;
		}

		if (self->g_bEndServer) {
			// ��� ���� ������� ����������
			break;
		}

		if (!bSuccess || (bSuccess && (dwIoSize == 0))) {
			// ���������� ���������� ��������, ���������� ������������ 
			// ���������� � ����� ����������� ����������
			self->CloseClient(lpPerSocketContext, FALSE);
			continue;
		}

		// ��������� ��� ������ ���������� � ������� �������� 
		// ��������� PER_IO_CONTEXT, ��������������� � ���� �������
		lpIOContext = (PER_IO_CONTEXT*)lpOverlapped;

		switch (lpIOContext->IOOperation) {
		case ClientIoRead: {
			int resultCode = ERROR_SUCCESS;
			LPTSTR pObjectOwner = NULL;
			// � ������ ��������� 4 ����� ��� ����� ���������
			LPBYTE pbPayload = (LPBYTE)lpIOContext->wsabuf.buf + sizeof(DWORD);

			LPBYTE pbDecoded = NULL;
			DWORD cbDecoded = 0;
			DWORD cbEncodedBlob = *((LPDWORD)lpIOContext->wsabuf.buf);
			cout << "Received " << dwIoSize << " cbEncodedBlob " << cbEncodedBlob;
			resultCode = Decrypt(&pbDecoded, &cbDecoded, pbPayload, cbEncodedBlob);
			cout << " cbDecoded " << cbDecoded << endl;
			if (resultCode != ERROR_SUCCESS) {
				break;
			}

			AgentRequest* pRequest = (AgentRequest*)pbDecoded;
			cout << "Request: reqType " << pRequest->reqType << " headSize " << pRequest->headSize
				<<  " pathSize " << pRequest->pathSize << " userSize " << pRequest->userSize << endl;

			AgentReply* pReply = (AgentReply*)pbPayload;
			DWORD cbReplyHead = sizeof(pReply->errorCode) + sizeof(pReply->reqType);
			DWORD cbReplyBody = 0;
			DWORD cbUserName = 0;

			// ��������� �������� ������, ���������� ����� ��������� �������,
			// ��������� ��� �� ��������
			cout << "got request: " << pRequest->reqType << endl;
			switch (pRequest->reqType) {
			case reqOsVer:
				pReply->vf.osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
				if (GetVersionEx((LPOSVERSIONINFO)&pReply->vf.osVer)) {
					pReply->vf.osVer.szCSDVersion[127] = TEXT('\0'); // �� ������ ������
					cbReplyBody = sizeof(pReply->vf.osVer);
					cout << "OS version " << pReply->vf.osVer.dwMajorVersion
						<< "." << pReply->vf.osVer.dwMinorVersion
						<< "." << pReply->vf.osVer.dwBuildNumber
						<< endl;
					cout << " dwPlatformId " << pReply->vf.osVer.dwPlatformId << endl;
					wcout << TEXT(" sp \"") << pReply->vf.osVer.szCSDVersion << TEXT("\"") << endl;
					cout << " ServicePack " << pReply->vf.osVer.wServicePackMajor
						<< "." << pReply->vf.osVer.wServicePackMinor << endl;
					cout << " wSuiteMask " << hex << uppercase << setfill('0')
						<< setw(8) << pReply->vf.osVer.wSuiteMask << dec << endl;
					cout << " wProductType " << DWORD(pReply->vf.osVer.wProductType) << endl;
				}
				else {
					resultCode = (int)GetLastError();
					cerr << "GetVersionEx failed: " << resultCode << endl;
				}
				break;
			case reqSysTime:
				GetLocalTime(&pReply->vf.sysTime);
				cbReplyBody = sizeof(pReply->vf.sysTime);
				cout << "GetSystemTime: " << pReply->vf.sysTime.wYear
					<< "-" << pReply->vf.sysTime.wMonth
					<< "-" << pReply->vf.sysTime.wDay
					<< " (" << pReply->vf.sysTime.wDayOfWeek << ")"
					<< " " << pReply->vf.sysTime.wHour
					<< ":" << pReply->vf.sysTime.wMinute
					<< ":" << pReply->vf.sysTime.wSecond
					<< "." << pReply->vf.sysTime.wMilliseconds
					<< endl;
				break;
			case reqTickCount:
				pReply->vf.tickCount = GetTickCount();
				cbReplyBody = sizeof(pReply->vf.tickCount);
				cout << "GetTickCount: " << pReply->vf.tickCount << endl;
				break;
			case reqMemStatus:
				pReply->vf.memStatus.dwLength = sizeof(pReply->vf.memStatus);
				if (GlobalMemoryStatusEx(&pReply->vf.memStatus)) {
					cbReplyBody = sizeof(pReply->vf.memStatus);
					cout << "percent of memory in use " << pReply->vf.memStatus.dwMemoryLoad << endl;
					cout << "total KB of physical memory " << pReply->vf.memStatus.ullTotalPhys << endl;
					cout << "free  KB of physical memory " << pReply->vf.memStatus.ullAvailPhys << endl;
					cout << "total KB of paging file " << pReply->vf.memStatus.ullTotalPageFile << endl;
					cout << "free  KB of paging file " << pReply->vf.memStatus.ullAvailPageFile << endl;
					cout << "total KB of virtual memory " << pReply->vf.memStatus.ullTotalVirtual << endl;
					cout << "free  KB of virtual memory " << pReply->vf.memStatus.ullAvailVirtual << endl;
					cout << "free  KB of extended memory " << pReply->vf.memStatus.ullAvailExtendedVirtual << endl;
				}
				else {
					resultCode = (int)GetLastError();
					cerr << "GlobalMemoryStatusEx failed: " << resultCode << endl;
				}
				break;
			case reqDriveType:
				if (pRequest->pathSize) {
					LPCTSTR objectPath = (LPCTSTR)(pbDecoded + pRequest->headSize);

					tcout << TEXT("object path: \"") << objectPath << TEXT("\"") << endl;
					pReply->vf.driveType = GetDriveType(objectPath);
					cbReplyBody = sizeof(pReply->vf.driveType);
					cout << "GetDriveType: " << pReply->vf.driveType << endl;
				}
				else {
					resultCode = -2;
					cerr << "Unspecified path" << resultCode << endl;
				}
				break;
			case reqFreeSpace:
				if (pRequest->pathSize) {
					LPCTSTR objectPath = (LPCTSTR)(pbDecoded + pRequest->headSize);
					tcout << TEXT("object path: \"") << objectPath << TEXT("\"") << endl;
					if (GetDiskFreeSpaceEx(objectPath,
						&pReply->vf.freeSpace.freeBytesAvailable,
						&pReply->vf.freeSpace.totalNumberOfBytes,
						&pReply->vf.freeSpace.totalNumberOfFreeBytes))
					{
						cbReplyBody = sizeof(pReply->vf.freeSpace);
						cout << "total " << pReply->vf.freeSpace.totalNumberOfBytes.QuadPart
							<< " free " << pReply->vf.freeSpace.totalNumberOfFreeBytes.QuadPart
							<< " (" << pReply->vf.freeSpace.freeBytesAvailable.QuadPart << ")" << endl;
					}
					else {
						resultCode = (int)GetLastError();
						cerr << "GlobalMemoryStatusEx failed: " << resultCode << endl;
					}
				}
				else {
					resultCode = -2;
					cerr << "Unspecified path" << resultCode << endl;
				}
				break;
			case reqAccessRights:
				if (pRequest->pathSize && pRequest->userSize) {
					LPCTSTR objectPath = (LPCTSTR)(pbDecoded + pRequest->headSize);
					LPCTSTR userName = (LPCTSTR)(pbDecoded + pRequest->headSize + pRequest->pathSize);
					tcout << TEXT("objectPath: \"") << objectPath << TEXT("\"") << TEXT(" userName: \"") << userName << TEXT("\"") << endl;
					AUTHZ_ACCESS_REPLY accessReply;
					memset(&accessReply, 0x00, sizeof(accessReply));
					pReply->vf.accessMask = 0;
					DWORD dwRetCode = ERROR_SUCCESS;

					LPTSTR pAccountName = TEXT("vickr");
					PACL                 pacl;
					PSECURITY_DESCRIPTOR psd;
					PSID                 psid = NULL;

					dwRetCode = GetNamedSecurityInfo(
						objectPath,
						SE_FILE_OBJECT,
						DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION,
						NULL,
						NULL,
						&pacl,
						NULL,
						&psd);

					if (dwRetCode == ERROR_SUCCESS) {
						dwRetCode = UseAuthzSolution(psd, pAccountName, &accessReply);
						if (dwRetCode == ERROR_SUCCESS) {
							pReply->vf.accessMask = *accessReply.GrantedAccessMask;
							cbReplyBody = sizeof(pReply->vf.accessMask);
							DisplayAccessReply(accessReply);
						}
						else {
							cerr << "UseAuthzSolution failed: " << dwRetCode << endl;
						}
					}
					else {
						cerr << "GetNamedSecurityInfo failed: " << dwRetCode << endl;
					}

					if (psid != NULL) {
						LocalFree(psid);
						psid = NULL;
					};

					LocalFree(psd);

					resultCode = dwRetCode;

				}
				else {
					resultCode = -2;
					cerr << "Unspecified path" << resultCode << endl;
				}
				break;
			case reqObjectOwn:
				if (pRequest->pathSize) {
					LPCTSTR objectPath = (LPCTSTR)(pbDecoded + pRequest->headSize);
					tcout << TEXT("object path: \"") << objectPath << TEXT("\"") << endl;
					pObjectOwner = GetObjectOwner(objectPath, SE_FILE_OBJECT, &cbReplyBody);
					if (pObjectOwner && cbReplyBody) {
						cbReplyBody += 1;
						tcout << TEXT("GetObjectOwner (size ") << cbReplyBody << TEXT("): \"") << pObjectOwner << TEXT("\"") << endl;
						_tcscpy_s(pReply->vf.objectOwner, cbReplyBody, pObjectOwner);
						cbReplyBody *= sizeof(TCHAR); // ������� � ����� (����� ��� �������)
					}
					else {
						resultCode = (int)GetLastError();
						cerr << "GlobalMemoryStatusEx failed: " << resultCode << endl;
					}
				}
				else {
					resultCode = -2;
					cerr << "Unspecified path" << resultCode << endl;
				}
				break;
			default:
				cerr << "Unknown request: " << pReply->reqType << endl;
			}

			pReply->errorCode = resultCode;
			pReply->reqType = pRequest->reqType;
			dwIoSize = cbReplyHead + cbReplyBody;
			cout << ">>> Ready " << dwIoSize << " bytes (" << cbReplyHead << " + " << cbReplyBody << ") retCode " << pReply->errorCode << endl;
			{ // ���������� ����� ��������� (�� ���������, ����� ������ 4 ���� (DWORD, ������))
				DWORD cbEncodedBlob;
				LPBYTE pbEncodedBlob;

				Encrypt((LPBYTE)pReply, dwIoSize, &pbEncodedBlob, &cbEncodedBlob);
				cout << ">>> Encoded: " << cbEncodedBlob << " bytes";

				memcpy(pbPayload, pbEncodedBlob, cbEncodedBlob);
				*((DWORD*)lpIOContext->wsabuf.buf) = cbEncodedBlob;
				dwIoSize = sizeof(DWORD) + cbEncodedBlob;
				cout << " (send " << dwIoSize << " bytes)" << endl;

				if (pbEncodedBlob)
					free(pbEncodedBlob);
			}
			lpIOContext->IOOperation = ClientIoWrite;
			lpIOContext->nTotalBytes = dwIoSize;
			lpIOContext->nSentBytes = 0;
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
			if (pbDecoded)
				free(pbDecoded);
			break;
		}

		case ClientIoWrite:
			// ��������� �������� ������, ����������, ��� ���������� ��� ������
			lpIOContext->IOOperation = ClientIoWrite;
			lpIOContext->nSentBytes += dwIoSize;
			dwFlags = 0;
			if (lpIOContext->nSentBytes < lpIOContext->nTotalBytes) {

				// ���������� �������� ������ ������ ��������� �� ��� ������
				// ���������� ��� ���� ������ - �� ������� ���������� ������
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
				// ���������� �������� ������ ��������� ���������, ���������� ������ �� �����
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
