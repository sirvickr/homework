/*
1.11. Реализовать инструкции типа #define и #undef, то есть выполнить указанные подстановки в области их задания.
Пробельные промежутки следует оставить без изменения.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	struct stat stat_buf;
	FILE *file = NULL, *output = NULL;
	char *buffer = NULL;

	if(argc != 2) {
		printf("usage: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	const char* source = argv[1];
	const char* destin = "sample.c";

	do {
		
		// получаем размер исходного файла
		if(-1 == stat(source, &stat_buf)) {
			perror("stat");
			break;
		}
		// открываем исходный файл на чтение
		file = fopen(source, "r");
		if(!file) {
			fprintf(stderr, "failed to open source file: \"%s\"\n", source);
			break;
		}
		// открываем целевой файл на запись
		output = fopen(source, "w");
		if(!output) {
			fprintf(stderr, "failed to open source file: \"%s\"\n", source);
			break;
		}
		// выделяем блок памяти для содержимого файла
		buffer = (char*)malloc(stat_buf.st_size + 1);
		if(!buffer) {
			fprintf(stderr, "failed to alloc memory\n");
			break;
		}
		size_t block_size = 1;
		size_t nmemb = stat_buf.st_size;
		size_t read = fread(buffer, block_size, nmemb, file);
		//printf("read %lu blocks of %lu bytes (%lu)\n", read, block_size, left_to_read);
		if(read == 0) {
			fprintf(stderr, "failed to read %lu blocks of %lu bytes\n", nmemb, block_size);
			//success = false;
			break;
		}

		typedef enum { 
			stPlainText, 
			stMacroHead, 
			stMacroBody, 
		} ParserStates;
		ParserStates state = stPlainText;
		const char* def_str = "#define";
		const size_t def_len = strlen(def_str);
		size_t def_pos = 0;

		for(size_t i = 0; i < stat_buf.st_size; i++) {
			char ch = buffer[i];
			switch(ch) {
			case '#':
				break;
			}
			fprintf(stderr, "%c\n", ch);
			switch(state) {
			case stPlainText:
				if(def_str[def_pos] == ch) {
					def_pos++;
					if(def_pos == def_len) {
						;
					}
				}
				break;
			case stMacroHead:
				break;
			}
		}

	} while(0);

	if(file)
		fclose(file);
	
	return 0;
}
