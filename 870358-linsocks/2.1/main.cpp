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
		cout << "usage: " << argv[0] << " ip_string" << endl;
		cout << "e.g.: " << argv[0] << " 127.0.0.1" << endl;
		return 0;
	}
	
	char str[INET_ADDRSTRLEN];
	struct in_addr addr;
	
	int status = inet_pton(AF_INET, argv[1], &addr);
	if (status <= 0) {
		if (status == 0)
			fprintf(stderr, "Not in presentation format");
		else
			perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	
	showBytes32(addr.s_addr, "network");
	// обратное преобразование, для наглядности
	if (inet_ntop(AF_INET, &addr, str, INET_ADDRSTRLEN) == NULL) {
		perror("inet_ntop");
		exit(EXIT_FAILURE);
	}
	// должны увидеть строку адреса, переданную в параметре коммандной строки
	cout << "s = " << str << endl;
	
	return 0;
}
