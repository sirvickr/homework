/*
8.3. Создайте программу - пример использования функции system()
Запуск:
make
./task
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	// при помощи интерпретатора Bourne shell запускаем команду ls спараметром -l
	return system("ls -l");
}
