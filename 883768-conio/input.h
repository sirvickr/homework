#ifndef __DICTIONARY_INPUT_DLG_H__
#define __DICTIONARY_INPUT_DLG_H__

#include <windows.h>

#define MAX_TITLE 50

#define TITLE   0
#define BUFFER  1
#define COLUMNS 2

typedef struct InputBox {
	HANDLE handle;
	SMALL_RECT rect;
	CHAR_INFO* bak;
	CHAR_INFO* wnd;
	COORD size; // width, height
	WORD edit_attr;
	int max_width;
	// буфер ввода
	//char* buffer;
	// 2-мерный массив строк: 1я колонка - надписи
	// 2я колонка - редактируемые поля (можно передавать начальные значения)
	char*** contents;
	int row_count; // количество полей (пар надпись/значение)
	int row; // текущее редактируемое поле
} InputBox;

int box_init(InputBox* box, HANDLE handle, SMALL_RECT rect, char*** contents, int row_count);
void box_clear(InputBox* box);
int box_save(InputBox* box);
int box_draw(InputBox* box);
int box_restore(InputBox* box);

#endif