/*
4. Создайте программу, позволяющую выяснить: если файл открыт для чтения и записи с флагом O_APPEND, 
можно ли читать данные из произвольного места в файле с помощью функций lseek() и read() ? 
Можно ли записывать данные в произвольное место в файле с помощью функций lseek() и write() ? (упр. 3.6 в учебнике)
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

static char buf[256];

int main(int argc, char* argv[])
{
	if(argc != 3) {
		printf("usage: %s file_name word_to_append\n", argv[0]);
		return 0;
	}

	const char* filename = argv[1];
	const char* message = argv[2];

	// открываем файл
	int fd = open(filename, O_RDWR | O_APPEND);
	if(-1 == fd) {
		perror("open");
		return 1;
	}

	// получаем размер файла
	struct stat stat_buf;
	fstat(fd, &stat_buf);
	printf("file size: %ld\n", stat_buf.st_size);
	// смещаемся на середину файла
	off_t pos = stat_buf.st_size / 2;
	printf("moving to the middle of the file: ");
	pos = lseek(fd, pos, SEEK_SET);
	printf("%ld\n", pos);

	ssize_t count = 0;

	// пишем строку в файла
	size_t size = strlen(message);
	count = write(fd, message, size);
	printf("Wrote %ld bytes to file\n", count);
	// строка в итоге всё равно пишется в конец файла, из-за установленного флага O_APPEND

	// смещаемся в начало
	printf("moving to the beginning of the file: ");
	pos = lseek(fd, 0, SEEK_SET);
	printf("%ld\n", pos);
	// читаем до sizeof(buf) символов с начала файла
	count = read(fd, buf, sizeof(buf));
	if(-1 == count) {
		perror("read");
		return 1;
	}
	buf[count] = 0;
	// выводим прочитанные символы
	printf("Read %ld bytes from file:\n%s\n", count, buf);
	// смещение указателя на чтение работает правильно - здесь мы увидим содержимое всего файла

	// смещаемся на середину файла
	pos = stat_buf.st_size / 2;
	printf("moving to the middle of the file: ");
	pos = lseek(fd, pos, SEEK_SET);
	printf("%ld\n", pos);
	// и снова читаем до sizeof(buf) символов, теперь уже с середины дополненного файла, чтобы
	// окончательно убедиться, что O_APPEND не влияет на позиционирование в файле при чтения
	count = read(fd, buf, sizeof(buf));
	if(-1 == count) {
		perror("read");
		return 1;
	}
	buf[count] = 0;
	// выводим прочитанные символы
	printf("Read %ld bytes from file:\n%s\n", count, buf);
	// смещение указателя на чтение работает правильно - здесь мы увидим вторую половину файла

	// закрываем дескриптор файла
	close(fd);

	return 0;
}
