/*
2. Создайте собственный упрощенный аналог утилиты du.
Для упрощенного аналога достаточно выдавать использование дискового пространства 
для всех файлов, указанных в командной строке. Работать с каталогами не требуется.
*/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	if(argc < 2) {
		printf("usage: %s file1 file2 ...\n", argv[0]);
		return 0;
	}

	struct stat stat_buf;
	for(int i = 1; i < argc; ++i) {
		// получаем информацию об очередном файле
		bzero(&stat_buf, sizeof(stat_buf));
		if(-1 == stat(argv[i], &stat_buf)) {
			perror(argv[i]);
			continue;
		}
		// печатаем полученную информацию
		printf("%ld\t%s\n", stat_buf.st_size, argv[i]);
	}

	return 0;
}
