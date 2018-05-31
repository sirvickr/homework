/*
1. Создайте программу, которая будет получать один аргумент командной строки - путь к объекту 
файловой системы, и выводить для указанного объекта всю информацию, которую можно получить с 
помощью функции stat(). Должны корректно обрабатываться все варианты типа объекта (файл, каталог, 
символьное устройство, блочное устройство, конвейер, символическая ссылка или сокет).
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	if(argc != 2) {
		printf("usage: %s file\n", argv[0]);
		return 0;
	}
	struct stat stat_buf;
	// получаем информацию о файле
	bzero(&stat_buf, sizeof(stat_buf));
	if (lstat(argv[1], &stat_buf) == -1) {
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	char* linkname = (char*)malloc(stat_buf.st_size + 1);
	if (linkname == NULL) {
		fprintf(stderr, "not enough memory\n");
		exit(EXIT_FAILURE);
	}
	ssize_t r = readlink(argv[1], linkname, stat_buf.st_size + 1);
	if (r == -1) {
		perror("readlink");
		exit(EXIT_FAILURE);
	}
	linkname[r] = '\0';
	puts(linkname);
	free(linkname);

	return 0;
}
