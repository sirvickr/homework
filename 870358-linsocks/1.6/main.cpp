/*
1.6 getaddrinfo(), IPv4, IPv6, номер порта
Модифицировать программу из п. 1. так, чтобы она получала два обязательных аргумента 
командной строки - доменное имя и имя сервиса и выдавала вместе с IPv4 или IPv6 адресами 
номер порта для подключения.
Можно использовать task1.c как образец для задач 1.4-1.6
*/

#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "../tasklog.h"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 3) {
		cout << "usage: task domain_name service_name" << endl;
		cout << "task mail.ru http" << endl;
		cout << "task example.com 80" << endl;
		cout << "etc.." << endl;
		return 0;
	}
	int status;
	addrinfo hints;
	addrinfo* servinfo;
	memset(&hints, 0x00, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // IPv4 или IPv6
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(argv[1], argv[2], &hints, &servinfo);
	if(status) {
		cout << "getaddrinfo failed" << endl;
		return 1;
	}
	
	for(addrinfo* p = servinfo; p != NULL; p = p->ai_next) {
		printAddrInfo(p);
	}

	freeaddrinfo(servinfo);
	return 0;
}
