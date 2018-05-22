/*
1.2 Порядок байт, strtol(), htonl()
Добавить к предыдущей программе преобразование полученного числа в сетевой порядок байт. 
Вывести результат (также в порядке возрастания адресов). Сравнить с выводом предыдущей программы.
*/

#include <iostream>

#include "../tasklog.h"

using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 2) {
		cout << "usage: task number" << endl;
		cout << "ex.:" << endl;
		cout << "task 0x01020304" << endl;
		cout << "task 65535" << endl;
		cout << "etc.." << endl;
		return 0;
	}
	uint32_t n = strtol(argv[1], NULL, 0);
	showBytes32(n, "host");
	n = htonl(n);
	showBytes32(n, "network");
	return 0;
}
