#include "stdafx.h"
#include "TcpClient.h"
#include "CryptoAPI.h"

using namespace std;

CTcpClient::CTcpClient(void)
: g_bEndClient(FALSE)
{
	WSADATA wsaData;
	WSAStartup(WINSOCK_VERSION, &wsaData);

	m_cbContent = DEFAULT_BUFLEN - sizeof(DWORD);
	m_pbSendBuf = new char[sizeof(DWORD) + m_cbContent];
}

CTcpClient::~CTcpClient(void)
{
	/*if(sock != INVALID_SOCKET) {
		closesocket(sock);
	}*/
	if (m_pbDecoded) {
		free(m_pbDecoded);
		m_pbDecoded = NULL;
		m_cbDecoded = 0;
	}
	WSACleanup();
}

SOCKET CTcpClient::Connect(LPCTSTR lpszServer, LPCTSTR lpszPort)
{
	SOCKET sock = INVALID_SOCKET;
	ADDRINFOA *result = NULL;
	ADDRINFOA *ptr = NULL;
	ADDRINFOA hints;
	int iResult;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Получаем адрес и порт сервера
	iResult = GetAddrInfo(lpszServer, lpszPort, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		return INVALID_SOCKET;
	}

	// Попытка подключиться к одному из адресов
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Создаем сокет
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			break;
		}

		// Подключаемся к серверу
		iResult = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	} // for()

	FreeAddrInfo(result);

	return sock;
}

int CTcpClient::SendRequest(SOCKET sock, const AgentRequest &request, LPCTSTR objPath, LPCTSTR userName)
{
	int iResult = ERROR_SUCCESS;
#if 1
	DWORD dwIoSize = request.headSize + request.pathSize + request.userSize;

	if (m_cbContent < dwIoSize) {
		m_cbContent = dwIoSize;
		delete[] m_pbSendBuf;
		m_pbSendBuf = new char[sizeof(DWORD) + m_cbContent];
		if (!m_pbSendBuf)
			return -1;
	}
	// оставляем место для размера отправляемых данных
	PCHAR pszContent = m_pbSendBuf + sizeof(DWORD);

	memcpy(pszContent, &request, request.headSize);
	if (objPath) {
		memcpy(pszContent + request.headSize, objPath, request.pathSize);
	}
	if (userName) {
		memcpy(pszContent + request.headSize + request.pathSize, userName, request.userSize);
	}
	// шифрование перед отправкой (всё сообщение, кроме первых 4 байт (DWORD, размер))
	{
		DWORD cbEncodedBlob;
		LPBYTE pbEncodedBlob;
		iResult = (int)Encrypt((LPBYTE)pszContent, dwIoSize, &pbEncodedBlob, &cbEncodedBlob);
		cout << ">>> Encoded: " << cbEncodedBlob << " bytes";

		if (ERROR_SUCCESS == iResult) {
			if (m_cbContent < cbEncodedBlob) {
				m_cbContent = cbEncodedBlob;
				delete[] m_pbSendBuf;
				m_pbSendBuf = new char[sizeof(DWORD) + m_cbContent];
				// оставляем место для размера отправляемых данных
				pszContent = m_pbSendBuf + sizeof(DWORD);
			}
			*((DWORD*)m_pbSendBuf) = cbEncodedBlob;
			memcpy(pszContent, pbEncodedBlob, cbEncodedBlob);
			free(pbEncodedBlob);
			iResult = send(sock, m_pbSendBuf, sizeof(DWORD) + cbEncodedBlob, 0);
			cout << " (sent " << iResult << " bytes)";
		} else {
			cout << " (failure)";
		}
		cout << endl;

	}
#else
	DWORD reqSize = request.headSize + request.pathSize + request.userSize;

	char *sendBuf = new char[reqSize];
	if (sendBuf) {
		memcpy(sendBuf, &request, request.headSize);
		if (objPath) {
			memcpy(sendBuf + request.headSize, objPath, request.pathSize);
		}
		if (userName) {
			memcpy(sendBuf + request.headSize + request.pathSize, userName, request.userSize);
		}

		iResult = send(sock, sendBuf, reqSize, 0);

		delete[] sendBuf;
		sendBuf = NULL;
	}
#endif

	tcout << _T("Bytes Sent: ") << iResult << endl;

	return iResult;
}

AgentReply* CTcpClient::RecvReply(SOCKET sock)
{
	int iResult;

	int recvbuflen = DEFAULT_BUFLEN;
	DWORD errCode = 0;
	int iReceived = 0;
	// Принимаем, пока не получим все запрошенные данные или сервер не оборвет соединение
	char *pInput = recvbuf;
	do {
		iResult = recv(sock, pInput, recvbuflen, 0);
		if (iResult > 0) {
			iReceived += iResult;
			DWORD dwIoSize = *((DWORD*)recvbuf);
			printf("Bytes received: %d/%d (message size %u)\n", iResult, iReceived, dwIoSize);

			if (m_pbDecoded) {
				free(m_pbDecoded);
				m_pbDecoded = NULL;
				m_cbDecoded = 0;
			}
			iResult = Decrypt(&m_pbDecoded, &m_cbDecoded, (LPBYTE)(recvbuf + sizeof(DWORD)), dwIoSize);
			if (iResult != ERROR_SUCCESS) {
				tcerr << _T("Ошибка декодирования: ") << iResult << endl;
				break;
			}
		}
		else if (iResult == 0) {
			printf("Connection closed\n");
			break;
		}
		else {
			errCode = WSAGetLastError();
			printf("recv failed with error: %d\n", iResult);
			break;
		}
	} while (iReceived < 12);

	return (AgentReply*)(m_pbDecoded);
}

int CTcpClient::MakeRequest(AgentReply &result, LPCTSTR lpszServer, LPCTSTR lpszPort, DWORD reqType, LPCTSTR objPath, LPCTSTR userName)
{
	int iResult = -1;

	SOCKET sock = Connect(lpszServer, lpszPort);
	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return iResult;
	}

	iResult = SendRequest(sock, PrepareRequest(reqType, objPath, userName), objPath, userName);
	if (iResult == SOCKET_ERROR) {
		tcerr << _T("send failed with error: ") << WSAGetLastError() << endl;
		closesocket(sock);
		return iResult;
	}

	result = *RecvReply(sock);

	// Очистка
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	return iResult;
}

int CTcpClient::GetOsVer(OSVERSIONINFOEX& result, LPCTSTR lpszServer, LPCTSTR lpszPort)
{
	int iResult = -1;

	SOCKET sock = Connect(lpszServer, lpszPort);
	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return iResult;
	}

	iResult = SendRequest(sock, PrepareRequest(reqOsVer));
	if (iResult == SOCKET_ERROR) {
		tcerr << _T("send failed with error: ") << WSAGetLastError() << endl;
		closesocket(sock);
		return iResult;
	}

	AgentReply* reply = RecvReply(sock);

	result = reply->vf.osVer;
	iResult = reply->errorCode;

	// Очистка
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	return iResult;
}

int CTcpClient::GetSysTime(SYSTEMTIME &result, LPCTSTR lpszServer, LPCTSTR lpszPort) {
	int iResult = -1;

	SOCKET sock = Connect(lpszServer, lpszPort);
	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return iResult;
	}

	iResult = SendRequest(sock, PrepareRequest(reqSysTime));
	if (iResult == SOCKET_ERROR) {
		tcerr << _T("send failed with error: ") << WSAGetLastError() << endl;
		closesocket(sock);
		return iResult;
	}

	AgentReply* reply = RecvReply(sock);

	result = reply->vf.sysTime;
	iResult = reply->errorCode;

	// Очистка
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	return iResult;
}

int CTcpClient::GetTickCount(DWORD &result, LPCTSTR lpszServer, LPCTSTR lpszPort)
{
	int iResult = -1;

	SOCKET sock = Connect(lpszServer, lpszPort);
	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return iResult;
	}

	iResult = SendRequest(sock, PrepareRequest(reqTickCount));
	if (iResult == SOCKET_ERROR) {
		tcerr << _T("send failed with error: ") << WSAGetLastError() << endl;
		closesocket(sock);
		return iResult;
	}

	AgentReply* reply = RecvReply(sock);

	result = reply->vf.tickCount;
	iResult = reply->errorCode;

	// Очистка
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	return iResult;
}

int CTcpClient::GetMemStatus(MEMORYSTATUSEX &result, LPCTSTR lpszServer, LPCTSTR lpszPort)
{
	int iResult = -1;

	SOCKET sock = Connect(lpszServer, lpszPort);
	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return iResult;
	}

	iResult = SendRequest(sock, PrepareRequest(reqMemStatus));
	if (iResult == SOCKET_ERROR) {
		tcerr << _T("send failed with error: ") << WSAGetLastError() << endl;
		closesocket(sock);
		return iResult;
	}

	AgentReply* reply = RecvReply(sock);

	result = reply->vf.memStatus;
	iResult = reply->errorCode;

	// Очистка
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	return iResult;
}

int CTcpClient::GetDriveType(UINT &result, LPCTSTR lpszServer, LPCTSTR lpszPort, LPCTSTR objPath)
{
	int iResult = -1;

	SOCKET sock = Connect(lpszServer, lpszPort);
	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return iResult;
	}

	iResult = SendRequest(sock, PrepareRequest(reqDriveType, objPath), objPath);
	if (iResult == SOCKET_ERROR) {
		tcerr << _T("send failed with error: ") << WSAGetLastError() << endl;
		closesocket(sock);
		return iResult;
	}

	AgentReply* reply = RecvReply(sock);

	result = reply->vf.driveType;
	iResult = reply->errorCode;

	// Очистка
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	return iResult;
}

int CTcpClient::GetDiskFreeSpace(FreeSpaceReply &result, LPCTSTR lpszServer, LPCTSTR lpszPort, LPCTSTR objPath)
{
	int iResult = -1;

	SOCKET sock = Connect(lpszServer, lpszPort);
	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return iResult;
	}

	iResult = SendRequest(sock, PrepareRequest(reqFreeSpace, objPath), objPath);
	if (iResult == SOCKET_ERROR) {
		tcerr << _T("send failed with error: ") << WSAGetLastError() << endl;
		closesocket(sock);
		return iResult;
	}

	AgentReply* reply = RecvReply(sock);

	result = reply->vf.freeSpace;
	iResult = reply->errorCode;

	// Очистка
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	return iResult;
}

int CTcpClient::GetAccessRights(ACCESS_MASK &result, LPCTSTR lpszServer, LPCTSTR lpszPort, LPCTSTR objPath, LPCTSTR userName)
{
	int iResult = -1;

	SOCKET sock = Connect(lpszServer, lpszPort);
	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return iResult;
	}

	iResult = SendRequest(sock, PrepareRequest(reqAccessRights, objPath, userName), objPath, userName);
	if (iResult == SOCKET_ERROR) {
		tcerr << _T("send failed with error: ") << WSAGetLastError() << endl;
		closesocket(sock);
		return iResult;
	}

	AgentReply* reply = RecvReply(sock);

	result = reply->vf.accessMask;
	iResult = reply->errorCode;

	// Очистка
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	return iResult;
}

int CTcpClient::GetObjectOwner(LPTSTR lpszResult, DWORD maxResultSize, LPCTSTR lpszServer, LPCTSTR lpszPort, LPCTSTR objPath)
{
	if (!lpszResult) {
		return -1;
	}

	int iResult = -1;

	SOCKET sock = Connect(lpszServer, lpszPort);
	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return iResult;
	}

	iResult = SendRequest(sock, PrepareRequest(reqObjectOwn, objPath), objPath);
	if (iResult == SOCKET_ERROR) {
		tcerr << _T("send failed with error: ") << WSAGetLastError() << endl;
		closesocket(sock);
		return iResult;
	}

	AgentReply* reply = RecvReply(sock);

	_tcscpy_s(lpszResult, maxResultSize, reply->vf.objectOwner);
	iResult = reply->errorCode;

	// Очистка
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	return iResult;
}

AgentRequest CTcpClient::PrepareRequest(DWORD reqType, LPCTSTR objPath, LPCTSTR userName)
{
	AgentRequest request;
	memset(&request, 0x00, sizeof(request));
	request.reqType = reqType;
	request.headSize = sizeof(request.reqType) + sizeof(request.headSize) + sizeof(request.pathSize) + sizeof(request.userSize);
	if (objPath)
		request.pathSize = (_tcslen(objPath) + 1) * sizeof(TCHAR);
	if (userName)
		request.userSize = (_tcslen(userName) + 1) * sizeof(TCHAR);
	return request;
}
