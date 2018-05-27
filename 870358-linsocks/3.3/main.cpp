/*
3.3 socket(), connect()
Добавить к программе из п.3.2 установку исходящего TCP-соединения 
на адрес из первого найденного функцией getaddrinfo() значения
*/

#include <iostream>
#include <unistd.h> // close()
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../tasklog.h"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 3) {
		cout << "usage: " << argv[0] << " domain_name service_name" << endl;
		cout << "e.g. " << argv[0] << " mail.ru http" << endl;
		cout << "or   " << argv[0] << " example.com 80" << endl;
		cout << "etc.." << endl;
		return 0;
	}
	int status;
	addrinfo hints;
	addrinfo* servinfo;
	memset(&hints, 0x00, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP

	status = getaddrinfo(argv[1], argv[2], &hints, &servinfo);
	if(status) {
		cout << "getaddrinfo failed: " << gai_strerror(status) << endl;
		return 1;
	}
	
	int sock = -1;
	for(addrinfo* p = servinfo; p != NULL; p = p->ai_next) {
		printAddrInfo(p);
		sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(sock == -1)
			continue;
		cout << "socket created: " << sock << endl;
		if(connect(sock, p->ai_addr, p->ai_addrlen) != -1) {
			cout << "socket connected" << endl;
			close(sock);
			break; // успешное подключение
		}
	}

	freeaddrinfo(servinfo);
	return 0;
}
