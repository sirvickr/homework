#ifndef dictH
#define dictH
//---------------------------------------------------------------------------
#define MAX_DICT_STR_LEN 32
#define DICT_FLD_CNT 3
//---------------------------------------------------------------------------
// структура записи словаря
typedef struct DICT_ENTRY {
	struct DICT_ENTRY* next;
	#if 1
	char* field[DICT_FLD_CNT];
	#else
	char* word_eng;
	char* word_part;
	char* word_rus;
	#endif
} DICT_ENTRY;
//---------------------------------------------------------------------------
struct MENU;
// Описание функций, которые подключаются к пунктам меню
int Edit(MENU* menu);
int Search(MENU* menu);
int Sort(MENU* menu);
int Save(MENU* menu);
int Help(MENU* menu);
int Exit(MENU* menu);
//
int dict_new(const char* word_eng, const char* word_part, const char* word_rus);
//
int dict_add(DICT_ENTRY* new_entry);
//
int dict_load(const char* file_name);
//
int dict_save(const char* file_name);
//
void dict_clear();
//
void dict_entry_clear(DICT_ENTRY* entry); 
//---------------------------------------------------------------------------
extern DICT_ENTRY* dict_head;
extern DICT_ENTRY* dict_tail;
extern const char* dict_file_name;
extern int dict_size;
//---------------------------------------------------------------------------
#endif
