/*
4.1 struct addrinfo *hints
Модифицировать программу из п.3.1: искать только IPv4 и UDP
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
	hints.ai_socktype = SOCK_DGRAM; // UDP

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
