/*
4.3 socket(), sendto(), recvfrom()
Добавить к программе из п.4.2 отправку произвольной строки на первый адрес, найденный getaddrinfo(), 
и получение ответа. Для тестирования используйте echo-сервер на kappa.cs.prv
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

		if(sendto(sock, message, strlen(message) + 1, 0, p->ai_addr, p->ai_addrlen) < 0) {
			perror("sendto");
			break;
		}
		cout << "data sent: " << message << endl;

        memset(data_buf, '\0', DataBufSize);
        if (recvfrom(sock, data_buf, DataBufSize, 0, (struct sockaddr *) NULL, NULL) < 0) {
			cerr << "recvfrom failed" << endl;
			break;
        }

		cout << "received: " << data_buf << endl;

		break;
	}

	freeaddrinfo(servinfo); // больше не понадобится

	if(sock != -1) {
		close(sock);
	}

	return 0;
}
