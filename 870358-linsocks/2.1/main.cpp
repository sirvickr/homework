/*
2.1 inet_pton()
Написать программу, которая получает один обязательный аргумент командной строки - IPv4 
адрес в строковом формате (aaa.bbb.ccc.ddd) и с помощью функции inet_pton() преобразует 
адрес во внутреннее представление. Вывести результат (можно использовать код из п.1.2)
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
	return 0;
}
