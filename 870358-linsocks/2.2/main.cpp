/*
2.2 inet_pton(), getnameinfo()
Добавить к программе из п.2.1 получение доменного имени для получившегося 
адреса с помощью функции getnameinfo(), вывести доменное имя (если есть).
*/

#include <iostream>

#include "../tasklog.h"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 2) {
		cout << "usage: task ip_string" << endl;
		cout << "ex.: task 127.0.0.1" << endl;
		return 0;
	}
	
	unsigned char buffer[sizeof(struct in6_addr)];
	char str[INET6_ADDRSTRLEN];
	
	int status = inet_pton(AF_INET, argv[1], buffer);
	
	if (status <= 0) {
		if (status == 0)
			fprintf(stderr, "Not in presentation format");
		else
			perror("inet_pton");
		exit(EXIT_FAILURE);
	}

	if(1 != status) {
		cerr << "inet_pton failed" << endl;
		return 1;
	}
	
	showBytes32(*(uint32_t*)buffer, "network");
	/*
	 * if (inet_ntop(AF_INET, buffer, str, INET6_ADDRSTRLEN) == NULL) {
		perror("inet_ntop");
		exit(EXIT_FAILURE);
	}
	cout << "s = " << str << endl;
	*/

#if 0
	struct sockaddr *sa; /* ввод */
	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

	if (getnameinfo(sa, sa->sa_len, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
		printf("host=%s, serv=%s\n", hbuf, sbuf);
#else
	struct sockaddr *sa;    /* входные */
	socklen_t len;         /* входные */
	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	if (getnameinfo(sa, len, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
		printf("host=%s, serv=%s\n", hbuf, sbuf);
#endif

	return 0;
}
