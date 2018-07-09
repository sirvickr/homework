//--------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma hdrstop
#include <sys/types.h>
#include <sys/stat.h>
#define __LIST1_H__
#define __DICT_H__
#include "common.h"
//--------------------------------------------
#pragma package(smart_init)
//--------------------------------------------
const char* dict_file_name = "dictionary.dat";
// глобальный экзепляр словаря
LIST1 dict;
// Вспомогательная функция загрузки из файла
char* file_load(const char* file_name, off_t* psize);
//--------------------------------------------
// Функции словаря
//--------------------------------------------
// 
int dict_load(const char* file_name) {
	off_t i, fld, size = 0;
	char* buf = file_load(file_name, &size);
	if(buf) {
		int field_start = 0, nfield = 0;

		DICT_ENTRY temp_entry;
		memset(&temp_entry, 0x00, sizeof(DICT_ENTRY));

		for(i = 0;  i < size; i++) {
			char ch = buf[i];
			if('\0' == ch) {
				temp_entry.field[nfield] = (char*)malloc((i - field_start + 1) * sizeof(char));
				strcpy(temp_entry.field[nfield], &buf[field_start]);
				field_start = i + 1;
				nfield++;
				if(DICT_FLD_CNT == nfield) {
					list1_push_back(&dict,
						dict_entry_new(temp_entry.field[0], temp_entry.field[1], temp_entry.field[2])
					);
					for(fld = 0; fld < DICT_FLD_CNT; fld++)
						if(temp_entry.field[fld])
							free(temp_entry.field[fld]);
					nfield = 0;
				}
			}
		} // for(i)
		free(buf);
	} // if(buf)

	return 0;
}

int dict_entry_save(void* data, int index, void* param) {
	const char* empty = "";
	int i;
	DICT_ENTRY* entry = (DICT_ENTRY*)data;
	FILE* file = (FILE*)param;
	for(i = 0; i < DICT_FLD_CNT; i++) {
		if(entry->field[i]) {
			fwrite(entry->field[i], strlen(entry->field[i]) + 1, 1, file);
		} else {
			fwrite(empty, 2, 1, file);
		}
	}
	return 1; // продолжить итерации по остальным элементам
}

int dict_save(const char* file_name) {
	int nwrote;
	FILE* file = fopen(file_name, "wb");
	if(!file)
		return -1;
	#if 1
	list1_for_each(&dict, dict_entry_save, file);
	#endif
	fclose(file);
	return 0;
}
//
DICT_ENTRY* dict_entry_new(const char* word_eng, const char* word_part, const char* word_rus) {
	DICT_ENTRY* new_entry;
	if(NULL == word_eng | NULL == word_part | NULL == word_rus)
		return NULL;
	new_entry = (DICT_ENTRY*)malloc(sizeof(DICT_ENTRY));
	memset(new_entry, 0x00, sizeof(DICT_ENTRY));

	new_entry->field[0] = (char*)malloc((strlen(word_eng) + 1) * sizeof(char));
	strcpy(new_entry->field[0], word_eng);
	new_entry->field[1] = (char*)malloc((strlen(word_part) + 1) * sizeof(char));
	strcpy(new_entry->field[1], word_part);
	new_entry->field[2] = (char*)malloc((strlen(word_rus) + 1) * sizeof(char));
	strcpy(new_entry->field[2], word_rus);

	return new_entry;
}

char* file_load(const char* file_name, off_t* psize) {
	int nread;
	struct _stat stat_buf;
	FILE* file;
	char* buf;
	int i, j, field_start = 0, nfield = 0;

	*psize = 0;

	if(0 != _stat(file_name, &stat_buf))
		return NULL;
	if(0 == stat_buf.st_size)
		return NULL;

	buf = (char*)malloc(stat_buf.st_size * sizeof(char));
	if(!buf)
		return NULL;

	file = fopen(file_name, "rb");
	if(!file) {
		free(buf);
		return NULL;
	}

	nread = fread(buf, stat_buf.st_size, 1, file);
	fclose(file);
	if(0 == nread) {
		free(buf);
		return NULL;
	} // if(nread > 0)

	*psize = stat_buf.st_size;
	return buf;
}

