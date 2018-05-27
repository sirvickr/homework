/*
1.4 getaddrinfo(), IPv4
Написать программу, которая получает один обязательный аргумент командной строки - 
доменное имя, и выдает все IPv4-адреса для этого имени.
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
		cout << "usage: " << argv[0] << " domain_name" << endl;
		cout << "e.g.:" << endl;
		cout << " " << argv[0] << " example.com" << endl;
		cout << " " << argv[0] << " localhost" << endl;
		cout << " " << argv[0] << " ya.ru" << endl;
		cout << " " << argv[0] << " mail.ru" << endl;
		cout << "etc.." << endl;
		return 0;
	}
	int status;
	addrinfo hints;
	addrinfo* servinfo;
	memset(&hints, 0x00, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
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
