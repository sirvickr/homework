/*
3.5 socket(), connect(), write() или send()
Добавить к программе из п.3.4 копирование ответа удаленной стороны на стандартный вывод
*/

#include <iostream>
#include <unistd.h> // close()
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../tasklog.h"

using namespace std;

const size_t ReqBufSize = 1024;
char req_buf[ReqBufSize];
const size_t DataBufSize = 8192;
char data_buf[DataBufSize];

int main(int argc, char* argv[])
{
	if(argc != 3) {
		cout << "usage: " << argv[0] << " domain_name service_name" << endl;
		cout << "e.g. " << argv[0] << " ya.ru http" << endl;
		cout << "or   " << argv[0] << " example.com 80" << endl;
		cout << "etc.." << endl;
		return 0;
	}
	const char* hostName = argv[1];
	const char* portNumber = argv[2];

	int status;
	addrinfo hints;
	addrinfo* servinfo;
	memset(&hints, 0x00, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP

	status = getaddrinfo(hostName, portNumber, &hints, &servinfo);
	if(status) {
		cout << "getaddrinfo failed" << endl;
		return 1;
	}
	
	int sock = -1;
	for(addrinfo* p = servinfo; p != NULL; p = p->ai_next) {
		printAddrInfo(p);
		sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(sock == -1)
			continue;
		cout << "socket created: " << sock << endl;
		if(connect(sock, p->ai_addr, p->ai_addrlen) != -1)
			break; // успешное подключение
		close(sock);
	}

	freeaddrinfo(servinfo); // больше не понадобится

	do { // только для того, чтобы удобно было при ошибке выйти по break сразу на освобождение ресурсов

	    sprintf(req_buf, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n", hostName);

		cout << "sending request:\n" << req_buf << endl;
	    // отправляем запрос
	    if(send(sock, req_buf, strlen(req_buf) , 0) < 0) {
			cerr << "send failed" << endl;
			break;
		}

		cout << "request sent" << endl;

	    // получаем ответ
	    if(recv(sock, data_buf, DataBufSize, 0) < 0) {
			cerr << "recv failed" << endl;
			break;
	    }

		cout << "reply received:\n";
		cout << data_buf;

	} while(0);

	close(sock);

	return 0;
}
