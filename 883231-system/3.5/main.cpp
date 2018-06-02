/*
3.5. Создайте программу, с помощью которой можно заметить особенность работы функции 
read() при чтении из терминала (возврат из read() происходит при нажатии enter).
Запуск:
make
./task
<ввести строку и нажать Enter, или просто нажать Enter>
rm task
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

static char buf[256];

int main(int argc, char* argv[])
{
	int fd = fileno(stdin);
	printf("Reading from terminal (%d): ", fd);
	fflush(stdout);
	// читаем из терминала (0 - это дескриптор stdin, он же STDIN_FILENO)
	ssize_t count = read(fd, buf, sizeof(buf));
	if(-1 == count) {
		perror("read");
		return 1;
	}
	buf[count] = 0;
	// выводим прочитанные символы
	printf("Read %ld bytes from terminal: %sLast char is 0x%02X\n", count, buf, buf[count - 1]);
	// если просто нажать enter, ничего не вводя, фугкция read() всё равно вернёт 
	// управление и в буфере будет лежать один символ - перевода строки ('\n' == 0x0A)
	return 0;
}
