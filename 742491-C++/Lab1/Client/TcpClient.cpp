#include "stdafx.h"
#include "TcpClient.h"

CTcpClient::CTcpClient(void)
: g_bEndClient(FALSE)
{
	WSADATA wsaData;
	WSAStartup(WINSOCK_VERSION, &wsaData);

	//sock = INVALID_SOCKET;
	//sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

CTcpClient::~CTcpClient(void)
{
	/*if(sock != INVALID_SOCKET) {
		closesocket(sock);
	}*/
	WSACleanup();
}

AgentReply CTcpClient::Request(LPCTSTR lpszServer, LPCTSTR lpszPort, const AgentRequest &request, LPCTSTR objPath)
{
	const size_t DEFAULT_BUFLEN = 1024;

	AgentReply reply;
	memset(&reply, 0x00, sizeof(reply));

	SOCKET sock = INVALID_SOCKET;

    ADDRINFOW *result = NULL;
    ADDRINFOW *ptr = NULL;
    ADDRINFOW hints;

	char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Получаем адрес и порт сервера
    iResult = GetAddrInfo(lpszServer, lpszPort, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        return reply;
    }

    // Попытка подключиться к одному из адресов
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Создаем сокет
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            return reply;
        }

        // Подключаемся к серверу
        iResult = connect( sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    FreeAddrInfo(result);

    if (sock == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        return reply;
    }

	DWORD headSize = sizeof(request.reqSize) + sizeof(request.reqType);
#if 1
	DWORD pathSize = objPath ? wcslen(objPath) * sizeof(TCHAR) : 0;
	DWORD sendSize = headSize + pathSize;
	char *sendBuf = new char[sendSize];
	if(sendBuf) {
		memcpy(sendBuf, &request, headSize);
		if(objPath) {
			memcpy(sendBuf + headSize, objPath, pathSize);
		}

		iResult = send( sock, sendBuf, sendSize, 0 );
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			return reply;
		}

		delete[] sendBuf;
		sendBuf = NULL;
	}
#else
	// Отправляем заголовок
    iResult = send( sock, (char*)&request, headSize, 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        return reply;
    }
	if(objPath) {
		DWORD pathSize = wcslen(objPath) * sizeof(TCHAR);
		tcout << TEXT("objPath = ") << objPath << std::endl;
		// Отправляем имя объекта
		iResult = send( sock, (char*)objPath, pathSize, 0 );
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(sock);
			return reply;
		}
	}
#endif

    printf("Bytes Sent: %ld\n", iResult);

    // отключить передачу данных в сокете
	// (после этого recv() почему-то возвращает -1, хотя прием не отключен)
    /*iResult = shutdown(sock, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        return reply;
    }*/

	DWORD errCode = 0;
	int iReceived = 0;
    // Принимаем, пока не получим все запрошенные данные или сервер не оборвет соединение
    do {
        iResult = recv(sock, recvbuf, recvbuflen, 0);
        if ( iResult > 0 ) {
			iReceived += iResult;
            printf("Bytes received: %d/%d\n", iResult, iReceived);
		} else if ( iResult == 0 ) {
            printf("Connection closed\n");
			break;
		} else {
			errCode =  WSAGetLastError();
            printf("recv failed with error: %d\n",iResult);
			break;
		}
    } while( iReceived < headSize ); // TODO  ориентироваться на size в заголовке

    // Очистка
    shutdown(sock, SD_BOTH);
    closesocket(sock);

	memcpy(&reply, recvbuf, iReceived);

	return reply;
}

BOOL CTcpClient::CreateConnectedSocket(int nThreadNum)
{
	return FALSE;
}

BOOL CTcpClient::SendBuffer(int nThreadNum, char *outbuf)
{
	return FALSE;
}

BOOL CTcpClient::RecvBuffer(int nThreadNum, char *inbuf)
{
	return FALSE;
}

DWORD WINAPI CTcpClient::EchoThread(LPVOID lpParameter)
{
	return 0;
}
