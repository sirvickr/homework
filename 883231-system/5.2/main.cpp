/*
5.2. Измерьте время работы предыдущей программы при чтении 
блоками по 1 байт, 1 Кб, 1 Мб, st_blksize из результата stat().
варианты запуска и проверки:
dd if=/dev/zero of=./file1 bs=1M count=100
./task file1 file2 1b
./task file1 file2 32b
./task file1 file2 1k
./task file1 file2 1m
./task file1 file2 1s
./task file1 file2 10s
rm file**
и т.д.
ИЛИ
dd if=/dev/zero of=./bigfile1 bs=1M count=$[6*1024]
./task bigfile1 bigfile2 1k
./task bigfile1 bigfile2 1m
./task bigfile1 bigfile2 50m
rm bigfile**
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>

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
		left_to_write -= written * size;
	}
	return nmemb - left_to_write;
}

bool copy(const char* source, const char* destin, size_t buf_size, size_t block_size = 1) {
	bool success = false;
	struct stat stat_buf;
	FILE *read_file = NULL, *write_file = NULL;
	char* buf = NULL;
	do {
		// получаем размер исходного файла
		if(-1 == stat(source, &stat_buf)) {
			perror("stat");
			break;
		}
		// открываем исходный файл на чтение
		read_file = fopen(source, "rb");
		if(!read_file) {
			fprintf(stderr, "failed to open source file: \"%s\"\n", source);
			break;
		}
		// открываем конечный файл на запись
		write_file = fopen(destin, "wb");
		if(!write_file) {
			fprintf(stderr, "failed to open destination file: \"%s\"\n", destin);
			break;
		}
		// выделяем блок памяти для копирования данных
		buf = (char*)malloc(buf_size);
		if(!buf) {
			fprintf(stderr, "failed to alloc memory\n");
			break;
		}
		success = true;
		// размер блока и количество блоков
		size_t nmemb = buf_size / block_size;
		// копируем всё содержимое исходного файла по блокам
		size_t left_to_read = stat_buf.st_size;
		while(left_to_read > 0) {
			size_t read = fread(buf, block_size, nmemb, read_file);
			//printf("read %lu blocks of %lu bytes (%lu)\n", read, block_size, left_to_read);
			if(read == 0) {
				fprintf(stderr, "failed to read %lu blocks of %lu bytes\n", nmemb, block_size);
				success = false;
				break;
			}
			// записываем всё, что прочитали
			size_t written = write_all(buf, 1, read, write_file);
			//printf("wrote %lu blocks of %lu bytes\n", written, block_size);

			left_to_read -= read * block_size;
		}
	} while(0);
	// освобождаем ресурсы
	if(read_file)
		fclose(read_file);
	if(write_file)
		fclose(write_file);
	if(buf)
		free(buf);
	return success;
}

int main(int argc, char* argv[])
{
	if(argc < 3) {
		printf("usage: %s source_file dest_file buf_size\n", argv[0]);
		printf("  buf_size values: nb, nk, nm, ns, where n is a number of units:\n");
		printf("  b - Byte, k - KByte, m - MByte, s - st_blksize\n");
		return 0;
	}

	size_t n = 1, units = 1; // 1 byte
	if(argc == 4) {
		char* s = NULL;
		n = strtol(argv[3], &s, 0);
		struct stat sb;
		if(s) {
			switch(s[0]) {
			case 'b':
				units = 1;
				break;
			case 'k':
				units = 1024;
				break;
			case 'm':
				units = 1024 * 1024;
				break;
			case 's':
				if(-1 == stat(argv[1], &sb)) {
					perror("stat");
					exit(EXIT_FAILURE);
				}
				units = sb.st_blksize;
				break;
			}
		}
	}
	size_t buf_size = n * units;
	printf("n %lu units %lu buf_size: %lu bytes\n", n, units, buf_size);

	timeval tv;
	gettimeofday(&tv, NULL);
	suseconds_t start = tv.tv_sec * 1000000 + tv.tv_usec;

	copy(argv[1], argv[2], buf_size);

	gettimeofday(&tv, NULL);
	suseconds_t finish = tv.tv_sec * 1000000 + tv.tv_usec;
	printf("copying time: %lu mks (%lu ms)\n", finish - start, size_t((finish - start + 500.0) / 1000.0));

	return 0;
}
