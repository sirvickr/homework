/*
5.1. Создайте аналог программы 3.1 (копирование файла) с использованием 
потоковых фунций fopen(), fread(), fwrite(), fclose().
варианты запуска и проверки:
echo 12345 > file1
./task file1 file2
ls -l file*
cat file2
ИЛИ
dd if=/dev/zero of=./bigfile1 bs=1M count=4
./task bigfile1 bigfile2
ls -l bigfile**
rm bigfile**
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

static const size_t BUF_SIZE = 1024 * 1024;
static char buf[BUF_SIZE] = {0};

// запись всего переданного буфера в файл
size_t write_all(const void* ptr, size_t size, size_t nmemb, FILE* file) {
	size_t left_to_write = nmemb;
	while(left_to_write > 0) {
		size_t written = fwrite(ptr, size, nmemb, file);
		//printf(" ..wrote %lu blocks of %lu bytes\n", written, size);
		if(written < nmemb) {
			fprintf(stderr, "failed to write %lu blocks of %lu bytes: %lu\n", nmemb, size, written);
			break;
		}
		left_to_write -= written;
	}
	return nmemb - left_to_write;
}

void copy(const char* source, const char* destin) {
	struct stat stat_buf;
	// получаем размер исходного файла
	if(-1 == stat(source, &stat_buf)) {
		perror("stat");
		exit(EXIT_FAILURE);
	}
	// открываем исходный файл на чтение
	FILE* read_file = fopen(source, "rb");
	if(!read_file) {
		fprintf(stderr, "failed to open source file: \"%s\"\n", source);
		exit(EXIT_FAILURE);
	}
	// открываем конечный файл на запись
	FILE* write_file = fopen(destin, "wb");
	if(!write_file) {
		fprintf(stderr, "failed to open destination file: \"%s\"\n", destin);
		fclose(read_file);
		exit(EXIT_FAILURE);
	}
	// размер блока и количество блоков
	size_t size = 1;
	// размер блока и количество блоков
	size_t nmemb = BUF_SIZE / size;
	// копируем всё содержимое исходного файла по блокам
	size_t left_to_read = stat_buf.st_size;
	while(left_to_read > 0) {
		size_t read = fread(buf, size, nmemb, read_file);
		printf("read %lu blocks of %lu bytes\n", read, size);
		if(read == 0) {
			fprintf(stderr, "failed to read %lu blocks of %lu bytes\n", nmemb, size);
			break;
		}
		// записываем всё, что прочитали
		size_t written = write_all(buf, 1, read, write_file);
		printf("wrote %lu blocks of %lu bytes\n", written, size);

		left_to_read -= read;
	}
	//return nmemb - left_to_read;

	// закрываем оба файла
	fclose(read_file);
	fclose(write_file);
}

int main(int argc, char* argv[])
{
	if(argc != 3) {
		printf("usage: %s source_file dest_file\n", argv[0]);
		return 0;
	}

	copy(argv[1], argv[2]);

	return 0;
}
