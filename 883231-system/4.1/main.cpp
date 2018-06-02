/*
4.1. Создайте программу, которая будет получать один аргумент командной строки - путь к объекту 
файловой системы, и выводить для указанного объекта всю информацию, которую можно получить с 
помощью функции stat(). Должны корректно обрабатываться все варианты типа объекта (файл, каталог, 
символьное устройство, блочное устройство, конвейер, символическая ссылка или сокет).

варианты запуска:
./task task
./task /dev/sda1
./task ../
./task /dev/console
и.д.
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

int main(int argc, char* argv[])
{
	if(argc != 2) {
		printf("usage: %s file\n", argv[0]);
		return 0;
	}

	// получаем информацию об объекте
	struct stat stat_buf;
	// используем lstat вместо stat, чтобы отличать ссылки от файлов
	if(-1 == lstat(argv[1], &stat_buf)) {
		perror("stat");
		return 1;
	}
	// печатаем полученную информацию
	printf("S_IFMT = %08X S_IFSOCK %08X S_IFLNK %08X S_IFREG %08X S_IFBLK %08X S_IFDIR %08X S_IFCHR %08X S_IFIFO %08X\n", S_IFMT, S_IFSOCK, S_IFLNK, S_IFREG, S_IFBLK, S_IFDIR, S_IFCHR, S_IFIFO);
	printf("device       %08lX: minor %u major %u\n", stat_buf.st_dev, major(stat_buf.st_dev),minor(stat_buf.st_dev));
	printf("inode        %08lX\n", stat_buf.st_ino);
	printf("mode         %08X: ", stat_buf.st_mode);
	int ifmt = stat_buf.st_mode & S_IFMT;
	switch(ifmt) {
	case S_IFSOCK:
		printf("sock");
		break;
	case S_IFLNK:
		printf("link");
		break;
	case S_IFREG:
		printf("file");
		break;
	case S_IFBLK:
		printf("block device");
		break;
	case S_IFDIR:
		printf("dir");
		break;
	case S_IFCHR:
		printf("character device");
		break;
	case S_IFIFO:
		printf("FIFO");
		break;
	default:
		printf("unexpected value");
		break;
	}
	printf("\n");
	printf("links count  %lu\n", stat_buf.st_nlink);
	printf("user id      %d\n", stat_buf.st_uid);
	printf("group id     %d\n", stat_buf.st_gid);
	printf("st_rdev      %lu\n", stat_buf.st_rdev);
	printf("size         %ld\n", stat_buf.st_size);
	printf("blksize      %ld\n", stat_buf.st_blksize);
	printf("blocks count %ld\n", stat_buf.st_blocks);

	return 0;
}
