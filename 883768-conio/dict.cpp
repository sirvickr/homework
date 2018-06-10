//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include <sys/types.h>
#include <sys/stat.h>
#include "dict.h"
#include "menu.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
DICT_ENTRY* dict_head = NULL;
DICT_ENTRY* dict_tail = NULL;
const char* dict_file_name = "dictionary.db";
int dict_size = 0;
//---------------------------------------------------------------------------
// Функции меню
//---------------------------------------------------------------------------
// Функция меню <Изменить>
int Edit(MENU* menu) {
	//printf("Edit\n");
	return 0;
}
// Функция меню <Поиск>
int Search(MENU* menu) {
	//printf("Search\n");
	return 0;
}
// Функция меню <Сортировка>
int Sort(MENU* menu) {
	//printf("Sort\n");
	return 0;
}
// Функция меню <Сохранить>
int Save(MENU* menu) {
	//printf("Save\n");
	return 0;
}
// Функция меню <Помощь>. Заполняется кодом пользователя
int Help(MENU* menu) {
	//printf("Help\n");
	return 0;
}
// Функция меню <Выход>. Заполняется кодом пользователя
int Exit(MENU* menu) {
	/*int resp;
	//cout << "Вы уверены, что хотите выйти з программы (y/n)?";
	printf("Quit? (y/n)?");
	resp = getchar();
	if (resp == 'y' || resp == 'Y') {
		return -1;
	} */
	return 0;
}
// Функция меню <Очистить>. Заполняется кодом пользователя
/*int Clear(MENU* menu)
{
	cls(menu, 0);
	menu->curspos.X = 0;
	menu->curspos.Y = 1;
	return 1;
}*/
//---------------------------------------------------------------------------
// Функции словаря
//---------------------------------------------------------------------------
// 
int dict_load(const char* file_name) {
	int nread;
	struct _stat stat_buf;
	FILE* file;
	char* buf = NULL;

	if(0 != _stat(file_name, &stat_buf))
		return -1;

	buf = (char*)malloc(stat_buf.st_size * sizeof(char));
	if(!buf)
		return -1;

	file = fopen(file_name, "rb");
	if(!file) {
		free(buf);
		return -1;
	}

	nread = fread(buf, stat_buf.st_size, 1, file);
	if(nread > 0) {
		int i, j, field_start = 0, nfield = 0;
		DICT_ENTRY temp_entry;
		memset(&temp_entry, 0x00, sizeof(DICT_ENTRY));

		for(i = 0;  i < stat_buf.st_size; i++) {
			char ch = buf[i];
			if('\0' == ch) {
				temp_entry.field[nfield] = (char*)malloc((i - field_start + 1) * sizeof(char));
				CharToOemA(&buf[field_start], temp_entry.field[nfield]);
				//strcpy(temp_entry.field[nfield], &buf[field_start]);
				field_start = i + 1;
				nfield++;
				if(DICT_FLD_CNT == nfield) {
					dict_new(temp_entry.field[0], temp_entry.field[1], temp_entry.field[2]);
					for(int fld = 0; fld < DICT_FLD_CNT; fld++)
						if(temp_entry.field[fld])
							free(temp_entry.field[fld]);
					nfield = 0;
				}
			}
		} // for(i)

	} // if(nread > 0)

	fclose(file);
	free(buf);
	return 0;
}
//
int dict_save(const char* file_name) {
	const char* empty = "";
	int nwrote;
	FILE* file = fopen(file_name, "wb");
	if(!file)
		return -1;

	DICT_ENTRY* curr = dict_head;
	while(curr) {
		int i, nwrote;
		for(i = 0; i < DICT_FLD_CNT; i++) {
			if(curr->field[i]) {
				nwrote = fwrite(curr->field[i], strlen(curr->field[i]) + 1, 1, file);
				if(2 == i)  {
					printf("%s:", curr->field[i]);
					char* pc = curr->field[i];
					while(*pc != 0) {
						printf(" %d", (int)*pc);
						pc++;
					}
					printf(" \n");
					//puts(curr->field[i]);
				}
			} else {
				nwrote = fwrite(empty, 2, 1, file);
			}
			if(nwrote != 1) {
				return -1;
			}
		}
		curr = curr->next;
	}

	fclose(file);
	return 0;
}
//
int dict_new(const char* word_eng, const char* word_part, const char* word_rus) {
	if(NULL == word_eng | NULL == word_part | NULL == word_rus)
		return -1;
	DICT_ENTRY* new_entry = (DICT_ENTRY*)malloc(sizeof(DICT_ENTRY));
	memset(new_entry, 0x00, sizeof(DICT_ENTRY));

	new_entry->field[0] = (char*)malloc((strlen(word_eng) + 1) * sizeof(char));
	strcpy(new_entry->field[0], word_eng);
	new_entry->field[1] = (char*)malloc((strlen(word_part) + 1) * sizeof(char));
	strcpy(new_entry->field[1], word_part);
	new_entry->field[2] = (char*)malloc((strlen(word_rus) + 1) * sizeof(char));
	strcpy(new_entry->field[2], word_rus);

	dict_add(new_entry);

	return 0;
}
//
int dict_add(DICT_ENTRY* new_entry) {
	if(NULL == new_entry)
		return -1;
	if(dict_head) {
		dict_tail->next = new_entry;
	} else {
		dict_head = new_entry;
	}
	dict_tail = new_entry;
	dict_size++;
	return 0;
}
//
void dict_clear() {
	DICT_ENTRY* curr = dict_head;
	while(curr) {
		int i;
		DICT_ENTRY* next = curr->next;
		dict_entry_clear(curr);
		curr = next;
	}
	dict_head = NULL;
	dict_tail = NULL;
}
//
void dict_entry_clear(DICT_ENTRY* entry) {
	int i;
	if(entry) {
		for(i = 0; i < DICT_FLD_CNT; i++)
			if(entry->field[i])
				free(entry->field[i]);
		free(entry);
	}
}

