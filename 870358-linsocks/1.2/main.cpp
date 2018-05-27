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
		cout << "usage: " << argv[0] << " number" << endl;
		cout << "e.g. " << argv[0] << " 0x01020304" << endl;
		cout << "or   " << argv[0] << " 65535" << endl;
		return 0;
	}
	uint32_t n = strtol(argv[1], NULL, 0);
	showBytes32(n, "host");
	n = htonl(n);
	showBytes32(n, "network");
	return 0;
}
