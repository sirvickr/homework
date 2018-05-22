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
		cout << "usage: task number" << endl;
		cout << "ex.:" << endl;
		cout << "task 0x01020304" << endl;
		cout << "task 65535" << endl;
		cout << "etc.." << endl;
		return 0;
	}
	uint32_t n = strtol(argv[1], NULL, 0);
	showBytes32(n, "host");
	return 0;
}
