#ifndef dictH
#define dictH
//--------------------------------------------
#define MAX_DICT_STR_LEN 32
#define DICT_FLD_CNT 3
//--------------------------------------------
#include "list.h"
//--------------------------------------------
// ��������� ������ �������
typedef struct DICT_ENTRY {
	char* field[DICT_FLD_CNT];
} DICT_ENTRY;
//--------------------------------------------
struct MENU;
//
DICT_ENTRY* dict_entry_new(const char* word_eng, const char* word_part, const char* word_rus);
//
int dict_load(const char* file_name);
//
int dict_save(const char* file_name);
//
void dict_clear();
//
void dict_entry_clear(DICT_ENTRY* entry); 
//--------------------------------------------
extern const char* dict_file_name;
extern LIST1 dict;
//--------------------------------------------
#endif
