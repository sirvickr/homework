/*
3.3. Создайте программу, позволяющую выяснить поведение функции open() при открытии с флагами O_CREAT | O_EXCL для записи файла, 
который существует и для которого нет прав на запись. Какую ошибку вернет функция open() - "нет прав" или "файл существует" ?
Запуск:
make
echo qwerty123 > file
chmod -w file
ls -l file
./task file
rm -f task file
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
	if(argc != 2) {
		printf("usage: %s file_name\n", argv[0]);
		return 0;
	}

	const char* filename = argv[1];

	// открываем файл
	int fd = open(filename, O_CREAT | O_EXCL | O_WRONLY);
	if(-1 == fd) {
		// "man 2 open" говорит, при таком сочетании флагов и наличии файла filename,
		// код ошибки будет равен 17 (EEXIST), проверяем это
		fprintf(stderr, "errno %d: %s\n", errno, errno == EEXIST ? "EEXIST" : "?");
		return 1;
	}
	
	printf("file created\n");

	// закрываем дескриптор файла
	close(fd);

	return 0;
}
