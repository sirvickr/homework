/*
1.3 Порядок байт, strtol(), htonl(), inet_ntop()
Добавить к предыдущей программе преобразование полученного числа с сетевым порядком байт 
в презентационный формат (строка) для AF_INET (IPv4) и вывод получившейся строки
*/

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../tasklog.h"

using namespace std;

int main(int argc, char* argv[])
{
	char buffer[100];
	if(argc != 2) {
		cout << "usage: task number" << endl;
		cout << "ex.:" << endl;
		cout << "task 0x01020304" << endl;
		cout << "task 0x7F000001" << endl;
		cout << "task 0xC0A80001" << endl;
		cout << "etc.." << endl;
		return 0;
	}
	uint32_t n = strtol(argv[1], NULL, 0);
	showBytes32(n, "host");
	n = htonl(n);
	showBytes32(n, "network");
	const char* szAddr = inet_ntop(AF_INET, &n, buffer, sizeof(buffer));
	if(!szAddr) {
		cerr << "inetntop failed" << endl;
		return 1;
	}
	cout << "s = " << szAddr << endl;
	return 0;
}
