/*
2.3 inet_pton(), getnameinfo()
Модифицировать программу из п.2.3 так, чтобы она получала два обязательных 
аргумента командной строки - IPv4 адрес в строковом формате и номер порта, 
с помощью функции getnameinfo() получить доменное имя и имя сервиса, вывести (если есть).
*/

#include <iostream>

#include "../tasklog.h"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 3) {
		cout << "usage: " << argv[0] << " ip_string port_number" << endl;
		cout << "e.g.: " << argv[0] << " 8.8.8.8 80" << endl;
		cout << "or    " << argv[0] << " 127.0.0.1 23" << endl;
		return 0;
	}
	
	char str[INET_ADDRSTRLEN];
	struct sockaddr_in addr_in;
	memset(&addr_in, 0x00, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(strtol(argv[2], NULL, 0));
	
	int status = inet_pton(AF_INET, argv[1], &addr_in.sin_addr);
	if (status <= 0) {
		if (status == 0)
			fprintf(stderr, "Not in presentation format");
		else
			perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	
	showBytes32(addr_in.sin_addr.s_addr, "network");
	// обратное преобразование, для наглядности
	if (inet_ntop(AF_INET, &addr_in.sin_addr, str, INET_ADDRSTRLEN) == NULL) {
		perror("inet_ntop");
		exit(EXIT_FAILURE);
	}
	// должны увидеть строку адреса, переданную в параметре коммандной строки
	cout << "s = " << str << endl;

	// выводим имя удалённого хоста (getnameinfo() вынесена в отдельную функцию)
	cout << getHostName(&addr_in, sizeof(addr_in)) << endl;

	return 0;
}
