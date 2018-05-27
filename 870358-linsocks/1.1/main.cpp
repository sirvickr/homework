/*
1.1 Порядок байт, strtol()
Написать программу, которая получает один обязательный аргумент командной строки - 
беззнаковое целое 32-битное число, записывает его в переменную, и затем выводит 
содержимое ячеек памяти, в которых хранится эта переменная (в порядке возрастания адресов).
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
	return 0;
}
