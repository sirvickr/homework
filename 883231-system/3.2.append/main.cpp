/*
2. Создайте программу добавления строки в конец произвольного файла. Программа должна получать два аргумента 
командной строки, первый аргумент - файл, в который добавить строку, второй аргумент - строка для добавления.
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	if(argc != 3) {
		printf("usage: %s file_name word_to_append\n", argv[0]);
		return 0;
	}

	const char* filename = argv[1];
	const char* message = argv[2];

	// открываем файл
	int fd = open(filename, O_WRONLY | O_APPEND);
	if(-1 == fd) {
		perror("open");
		return 1;
	}

	// дописываем строку в конец файла
	size_t size = strlen(message);
	write(fd, message, size);

	// закрываем дескриптор файла
	close(fd);

	return 0;
}
