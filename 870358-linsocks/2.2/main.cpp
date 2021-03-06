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
		cout << "usage: " << argv[0] << " ip_string" << endl;
		cout << "e.g.: " << argv[0] << " 8.8.8.8" << endl;
		cout << "or    " << argv[0] << " 127.0.0.1" << endl;
		return 0;
	}
	
	char str[INET_ADDRSTRLEN];
	struct sockaddr_in addr_in;
	memset(&addr_in, 0x00, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	
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
