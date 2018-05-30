/*
1. Создайте программу копирования файлов. Программа должна получать два аргумента командной 
строки, первый аргумент - файл, откуда копировать, второй аргумент - файл, куда копировать.
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

int main(int argc, char* argv[])
{
	if(argc != 3) {
		printf("usage: %s source_file dest_file\n", argv[0]);
		return 0;
	}

	const char* source = argv[1];
	const char* destin = argv[2];

	// открываем исходный файл
	int read_fd = open(source, O_RDONLY);
	if(-1 == read_fd) {
		perror(source);
		return 1;
	}
	// получаем размер исходного файла
	struct stat stat_buf;
	fstat(read_fd, &stat_buf);
	// открываем конечный файл на запись, с теми же привилегиями, что и исходный
	int write_fd = open(destin, O_WRONLY | O_CREAT, stat_buf.st_mode);
	if(-1 == write_fd) {
		perror(destin);
		return 2;
	}
	// копируем исходный файл в конечный 
	sendfile(write_fd, read_fd, NULL, stat_buf.st_size);
	// закрываем оба дескриптора
	close(read_fd);
	close(write_fd);

	return 0;
}
