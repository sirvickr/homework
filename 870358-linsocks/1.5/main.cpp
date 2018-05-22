/*
1.5 getaddrinfo(), IPv4, IPv6
Модифицировать программу из п. 1.4 так, чтобы она выдавала еще и IPv6-адреса.
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
	if(argc != 2) {
		cout << "usage: task domain_name" << endl;
		cout << "ex.:" << endl;
		cout << "task localhost" << endl;
		cout << "task ya.ru" << endl;
		cout << "task mail.ru" << endl;
		cout << "task example.com" << endl;
		cout << "etc.." << endl;
		return 0;
	}
	int status;
	addrinfo hints;
	addrinfo* servinfo;
	memset(&hints, 0x00, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // IPv4 или IPv6
	hints.ai_socktype = SOCK_STREAM;
	status = getaddrinfo(argv[1], NULL, &hints, &servinfo);
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
