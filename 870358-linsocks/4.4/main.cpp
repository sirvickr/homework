/*
4.4 socket(), connect(), send(), recv()
Модифицировать программу из п.4.2: сохранить первый адрес, найденный функцией getaddrinfo(), 
с помощью connect(), затем отправлять данные с помощью функции send().
*/

#include <iostream>
#include <unistd.h> // close()
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../tasklog.h"

using namespace std;

const size_t DataBufSize = 8192;
char data_buf[DataBufSize];

int main(int argc, char* argv[])
{
	if(argc < 3) {
		cout << "usage: " << argv[0] << " host_name port_number" << endl;
		cout << "e.g. " << argv[0] << " kappa.cs.petrsu.ru 7 qwerty" << endl;
		return 0;
	}
	const char* hostName = argv[1];
	const char* portNumber = argv[2];
	const char* message = "qwerty";
	if(argc > 3)
		message = argv[3];

	int status;
	addrinfo hints;
	addrinfo* servinfo;
	memset(&hints, 0x00, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_DGRAM; // UDP

	status = getaddrinfo(hostName, portNumber, &hints, &servinfo);
	if(status) {
		cerr << "getaddrinfo failed" << endl;
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
	
	if(sock == -1) {
		return -1;
	}

	do { // только для того, чтобы удобно было при ошибке выйти по break сразу на освобождение ресурсов

	    // отправляем запрос
	    if(send(sock, message, strlen(message) + 1, 0) < 0) {
			cerr << "send failed" << endl;
			break;
		}
		cout << "data sent: " << message << endl;

	    // получаем ответ
	    if(recv(sock, data_buf, DataBufSize, 0) < 0) {
			cerr << "recv failed" << endl;
			break;
	    }

		cout << "reply received: ";
		cout << data_buf << endl;

	} while(0);

	close(sock);

	return 0;
}
