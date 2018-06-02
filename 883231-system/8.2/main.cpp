/*
8.2. Создайте программу, позволяющую проверить, что дочерний и 
родительский процессы совместно используют текущую позицию файла
Запуск:
make
echo 123456789qwerty > file
./task prog file
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

static char buf[256];

int main(int argc, char* argv[])
{
	const size_t size = sizeof(buf);
#ifdef ANOTHER_DEMO_PROGRAM
	if(argc != 2) {
		printf("usage: %s file\n", argv[0]);
		return 1;
	}
	const char* file = argv[1];
#else
	if(argc != 3) {
		printf("usage: %s prog file\n", argv[0]);
		return 1;
	}
	const char* file = argv[2];
#endif
	
	// открываем файл
	int fd = open(file, O_RDONLY);
	if(-1 == fd) {
		perror("open");
		exit(EXIT_FAILURE);
	}

#ifdef ANOTHER_DEMO_PROGRAM

	printf("Another program: %d (argc %d:", getpid(), argc);
	for(int i = 0; i < argc; i++)
		printf(" %s", argv[i]);
	printf(")\n");
	
	printf("reading from file \"%s\"\n", file); fflush(stdout);
	// получаем размер файла
	struct stat stat_buf;
	fstat(fd, &stat_buf);
	printf("file size: %ld\n", stat_buf.st_size);
	// смещаемся на середину файла
	off_t pos = stat_buf.st_size / 2;
	printf("moving to the middle of the file: ");
	pos = lseek(fd, pos, SEEK_SET);
	printf("%ld\n", pos);
	// читаем до конца файла
	size_t left = stat_buf.st_size / 2;
	ssize_t count = read(fd, buf, size < left ? size : left);
	if(-1 == count) {
		perror("read");
		return 1;
	}
	buf[count] = 0;
	// выводим прочитанные символы
	printf("Read %ld bytes from file:\n%s\n", count, buf);

#else

	const char* prog = argv[1];

	printf("the main program process id: %d\n", getpid());
	pid_t child_pid = fork();
	if(child_pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(child_pid > 0) {
		printf("this is the main process with id %d (child pid %d)\n", getpid(), child_pid);
	} else {
		printf("this is the child process with id %d\n", getpid());
		execl(prog, prog, file, NULL);
		// возврат из функции произойдет только в случае ошибки
		printf("execl failed\n");
	}
	int status = 0;
	printf("waiting for child process..\n");
	waitpid(child_pid, &status, 0);
	printf("child process finished with code %d\n", status);

	// читаем до sizeof(buf) символов с начала файла
	ssize_t count = read(fd, buf, size);
	if(-1 == count) {
		perror("read");
		return 1;
	}
	buf[count] = 0;
	// выводим прочитанные символы
	printf("Read %ld bytes from file:\n%s\n", count, buf);
	// закрываем дескриптор файла
	close(fd);
	printf("main program finished (child process returned %d):\n", status);

#endif
	return 0;	
}
