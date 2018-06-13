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
	// ����� �����
	//char* buffer;
	// 2-������ ������ �����: 1� ������� - �������
	// 2� ������� - ������������� ���� (����� ���������� ��������� ��������)
	char*** contents;
	int row_count; // ���������� ����� (��� �������/��������)
	int row; // ������� ������������� ����
} InputBox;

int box_init(InputBox* box, HANDLE handle, SMALL_RECT rect, char*** contents, int row_count);
void box_clear(InputBox* box);
int box_save(InputBox* box);
int box_draw(InputBox* box);
int box_restore(InputBox* box);

#endif