//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "menu.h"
#include "dict.h"
#include "codes.h"
#include "list.h"
#include "input.h"
#pragma argsused

// �������� ������
typedef struct SearchCriteria {
	// ����� ���� ��� ��������� (�����, ����� ����, �������, ���������� ����)
	int index;
	// ������ - ������������ �������� ����� ������ �����
	int value;
	// ������ ������ (��� ������ ��� �����)
	const char* str;
} SearchCriteria;

// ��������������� ���������� �������

// �������� ������� ������� ���������
int Run();

// ������� ��������� ������

// �������, ������� ������������ � ������� ����
int Add(MENU* pm, ITEM*);
int Edit(MENU* menu, ITEM*);
int Delete(MENU* menu);
int Search(MENU* menu, ITEM*);
int Sort(MENU* menu, ITEM*);
int Save(MENU* menu, ITEM*);
int Help(MENU* menu, ITEM*);
int Exit(MENU* menu, ITEM*);

// �������, ������� ������������ � ������� ��������
// ����� - ��������� ������
int ExitYes(MENU* menu, ITEM* item);
// ����� - ��������� ������
int ExitNo(MENU* menu, ITEM* item);
// ����� - ������
int ExitCancel(MENU* menu, ITEM* item);
// ���������� ������� ESC �� ��������� ������� �� ����� ��������� �������� ����
int DefaultESC(MENU* menu);
// ���������� ������� ESC
int ESC(MENU* menu);
// ���������� ������� F1 (�������)
int F1(MENU* menu);
// ���������� ������� F2 (���������)
int F2(MENU* menu);
// ���������� ������� F3 (�����)
int F3(MENU* menu);
// ���������� ������� F4 (��������������)
int F4(MENU* menu);
// ���������� ������� F9 (������� �� ������� ����)
int F9(MENU* menu);
// ���������� ��������� ���� �� ���� ������� ����� ��� �����
void curr_menu_changed(MENU* menu, int direction, int wrap);
// ������� ���������� �������� ������� � ������� �������� ������ ����
int dict_entry_display(void* data, int index, void* param);
// �������� �������� ������� �� ������������ �������� ������
int dict_entry_find(void* data, void* param);
// ��������� ���� ��������� �������
int dict_entry_compare(void* data1, void* data2, void* arg);

// ��������������� �������

// ����������� ������ ������� �� ����� �� ����������� ����
int HelpFromFile(HANDLE hStdOut, const char* file_name, const char* title, SMALL_RECT rect);
// ����������� ����������� ������������� ���� � ���� �������
int ShowMenu(HANDLE hStdOut, ITEM_DEF* menu_items, int item_count, const char* title, SMALL_RECT rect, int flags, int user_tag, ExecuteHotketCB f1CB);
// �������� ������� ���� �� �����
ITEM_DEF* MenuItemsFromFile(const char* file_name, int max_count, int max_len, int* pcount);
// �������� �������� ������ � �������
void LoadInitialData();

// ����������� ���������� ����� ������
#define MAX_STRING 256
// ���������������� �����, ����� ��������� ���� � �������� ��������� ������
#define MENU_TAG_DEFAULT     0
#define MENU_TAG_TOP         1
#define MENU_TAG_MAIN        2
#define MENU_TAG_EXIT        3
#define MENU_TAG_HELP_GLOBAL 4
#define MENU_TAG_SORT        5
// ���������� ������� �������� ����
#define top_item_count 7
// ���������� ����� (��������) ��������� ���������� ����
#define main_column_count 4
// ���������� ����� (��������) ���� ������������� ������
#define exit_column_count 1
// ���������� ������� ���� ������������� ������
#define exit_item_count 3

// ������ � ���������� ��������� �������� ����
ITEM_DEF top_menu_items[top_item_count] = {
	{ { "��������", 0 }, Add },
	{ { "��������", 0 }, Edit },
	{ { "�����", 0 }, Search },
	{ { "����������", 0 }, Sort },
	{ { "���������", 0 }, Save },
	{ { "������", 0 }, Help },
	{ { "�����", 0 }, Exit },
};
// ������ � ���������� ��������� ���� ������������� ������
static ITEM_DEF exit_menu_items[exit_item_count] = {
	{ { "��", 0 }, ExitYes },
	{ { "���", 0 }, ExitNo },
	{ { "������", 0 }, ExitCancel },
};
// ������ ���������� �������� �������
char* main_headers[main_column_count];
// ������ ���������� (1 ���������) ���� ������������� ������
char* exit_headers[exit_column_count];
// ��� ���������� ��� ������� ��������� ������
int exit_code = 0; // �� ������� �������� ��������� ������������
// ������� ����������� �������� �������
int redraw_main = 1;
// ������� ������������ ������
int data_modified = 0;
// ������� ������������� ���������� ������
int save_data = 0;
// ������� ������ ������
int exit_canceled = 0;
// ������ ��� ������������� ����������� ����
int initial_table_index = 0;
// ����� ��� �������� �������� �������
CONSOLE_SCREEN_BUFFER_INFO csbInfo;
// ��������� �������� ����
MENU top_menu;
// ��������� �� ��������� �������� �������
MENU *ptable = NULL;

// ����� ����� ���������
int main(int argc, char* argv[])
{
	return Run();
}

// ����� - ��������� ������
int ExitYes(MENU* menu, ITEM* item) {
	save_data = 1;
	return -1;
}
// ����� - �� ��������� ������
int ExitNo(MENU* menu, ITEM* item) {
	save_data = 0;
	return -1;
}
// ����� - ������
int ExitCancel(MENU* menu, ITEM* item) {
	exit_canceled = 1;
	return -1;
}
// ���������� ������� ESC �� ��������� ������� �� ����� ��������� �������� ����
int DefaultESC(MENU* menu) {
	return -1;
}
// ���������� ������� ESC
int ESC(MENU* menu) {
	return Exit(menu, NULL);
}
// ���������� ������� F1 (�������)
int F1(MENU* menu) {
	Help(menu, NULL);
	return 0;
}
// ���������� ������� F2 (���������)
int F2(MENU* menu) {
	Save(menu, NULL);
	if(exit_code)
		return exit_code;
	return 0;
}
// ���������� ������� F3 (�����)
int F3(MENU* menu) {
	Search(menu, NULL);
	if(exit_code)
		return exit_code;
	return 0;
}
// ���������� ������� F4 (��������������)
int F4(MENU* menu) {
	Edit(menu, NULL);
	if(exit_code)
		return exit_code;
	return 0;
}
// ���������� ������� F9 (������� �� ������� ����)
int F9(MENU* menu) {
	menu_draw(&top_menu, MENU_FULL_FLAGS);
	if(exit_code)
		return exit_code;
	return 0;
}
// ���������� ��������� ���� �� ���� ������� ����� ��� �����
void curr_menu_changed(MENU* menu, int direction, int wrap) {
	switch(direction) {
	case MENU_CURR_REV:
		list1_curr_rev(&dict, wrap);
		break;
	case MENU_CURR_FWD:
		list1_curr_fwd(&dict, wrap);
		break;
	}
}
// ������� ���������� �������� ������� � ������� �������� ������ ����
int dict_entry_display(void* data, int index, void* param) {
	int fld;
	DICT_ENTRY* entry = (DICT_ENTRY*)data;
	ITEM_DEF* main_menu_items = (ITEM_DEF*)param;
	for(fld = 0; fld < DICT_FLD_CNT; fld++)
		CharToOemA(entry->field[fld], main_menu_items[index].str[fld]);
	itoa(strlen(entry->field[0]), main_menu_items[index].str[DICT_FLD_CNT], 10);
	main_menu_items[index].cb = Edit;
	return 1; // ���������� �������� �� ��������� ���������
}
// �������� - �������� �� ������� ������� �������� ������
int dict_entry_find(void* data, void* param) {
	DICT_ENTRY* entry = (DICT_ENTRY*)data;
	SearchCriteria* search = (SearchCriteria*)param;
	if(search->index < DICT_FLD_CNT)
		return strlen(entry->field[0]) <= search->value &&
		       strcmp(search->str, entry->field[search->index]) == 0 ? 1 : 0;
	// ����� �� "���������� ����" (��������������� ��� �������������)
	return 0;//atoi(search->str) == strlen(entry->field[0]) ? 1 : 0;
}

// ��������� ���� ��������� �������
int dict_entry_compare(void* data1, void* data2, void* arg) {
	DICT_ENTRY *a, *b;
	int index;
	if(!arg)
		return 0;
	a = (DICT_ENTRY*)data1;
	b = (DICT_ENTRY*)data2;
	index = *(int*)arg;
	if(index < 3)
		return strcmp(a->field[index], b->field[index]) < 0 ? 1 : 0;
	return strlen(a->field[0]) < strlen(b->field[0]) ? 1 : 0;
}

// �������� ������� ������� ���������
int Run() {
	char* title = "�����-������� �������";
	int i, fld;
	HANDLE hstdout;
	SMALL_RECT rect;
	ITEM_DEF* main_menu_items = NULL;
	int main_menu_items_count = 0;
	WORD background = BACKGROUND_INTENSITY | BACKGROUND_BLUE;
	#if 0
	LoadInitialData();
	return 0;
	#endif
	// ������������� ���������� ���������� ����
	for(i = 0; i < main_column_count; i++)
		main_headers[i] = (char*)malloc(MAX_MENU_HDR * sizeof(char));
	CharToOemA("�����", main_headers[0]);
	CharToOemA("����� ����", main_headers[1]);
	CharToOemA("�������", main_headers[2]);
	CharToOemA("���������� ����", main_headers[3]);

	for(i = 0; i < exit_column_count; i++)
		exit_headers[i] = (char*)malloc(MAX_MENU_HDR * sizeof(char));
	CharToOemA("��������� ���������?", exit_headers[0]);

	for(i = 0; i < exit_item_count; i++)
		CharToOemA(exit_menu_items[i].str[0], exit_menu_items[i].str[0]);

	CharToOemA(title, title);
	SetConsoleTitle(title);

	hstdout = GetStdHandle(STD_OUTPUT_HANDLE); // ���������� ����������� ����
	if(INVALID_HANDLE_VALUE == hstdout) {
		return -1;
	}
	// �������� ������� �������
	GetConsoleScreenBufferInfo(hstdout, &csbInfo);
	rect.Left = csbInfo.srWindow.Left;
	rect.Right = csbInfo.srWindow.Right;
	rect.Top = csbInfo.srWindow.Top;
	rect.Bottom = rect.Top;

	SetConsoleTextAttribute(hstdout, background);
	system("cls");

	menu_init(&top_menu, NULL, hstdout, top_menu_items, top_item_count, 1,
		MENU_ORIENT_HORZ, &rect, 0, NULL);
	top_menu.user_tag = MENU_TAG_TOP;
	menu_active_color(&top_menu, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_inactive_color(&top_menu, BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_add_hotkey(&top_menu, KEY_ESC, DefaultESC);
	menu_draw(&top_menu, MENU_FLAG_WND | MENU_FLAG_ITEMS);

	// �������� ������� �� �����
	if(-1 == dict_load(dict_file_name)) {
		return -1;
	}

	rect.Top++;
	rect.Bottom = csbInfo.srWindow.Bottom - 1;

	main_menu_items_count = dict.count;

	while (redraw_main) {
		redraw_main = 0; // ����� callback ����� ������������� �����������
		//
		if(main_menu_items) {
			for(i = 0; i < main_menu_items_count; i++)
				for(fld = 0; fld < DICT_FLD_CNT + 1; fld++)
					if(main_menu_items[i].str[fld])
						free(main_menu_items[i].str[fld]);
			free(main_menu_items);
			main_menu_items = NULL;
		}
		main_menu_items_count = dict.count; // ������� ��� ���� ������
		main_menu_items = (ITEM_DEF*)malloc(main_menu_items_count * sizeof(ITEM_DEF));
		memset(main_menu_items, 0x00, main_menu_items_count * sizeof(ITEM_DEF));
		for(i = 0; i < main_menu_items_count; i++) {
			for(fld = 0; fld < DICT_FLD_CNT; fld++)
				main_menu_items[i].str[fld] = (char*)malloc(MAX_STRING * sizeof(char));
			main_menu_items[i].str[DICT_FLD_CNT] = (char*)malloc(32 * sizeof(char));
		}
		//
		if(ptable) {
			menu_clear(ptable);
			free(ptable);
		}
		ptable = (MENU*)malloc(sizeof(MENU));
		// ������������ ������� � �������
		list1_for_each(&dict, dict_entry_display, main_menu_items);
		// ������ �������� ����
		menu_init(ptable, NULL, hstdout, main_menu_items, main_menu_items_count, main_column_count,
			MENU_ORIENT_VERT, &rect, 1, main_headers);
		// ������������� ������� ������
		list1_set_current_index(&ptable->items, initial_table_index);
		initial_table_index = 0;
		// ���������� ��������� ����
		ptable->user_tag = MENU_TAG_MAIN;
		menu_add_hotkey(ptable, KEY_F1, F1);
		menu_add_hotkey(ptable, KEY_F2, F2);
		menu_add_hotkey(ptable, KEY_F3, F3);
		menu_add_hotkey(ptable, KEY_F4, F4);
		menu_add_hotkey(ptable, KEY_F9, F9);
		menu_add_hotkey(ptable, KEY_DEL, Delete);
		menu_add_hotkey(ptable, KEY_ESC, ESC);
		ptable->changed_cb = curr_menu_changed;
		menu_active_color(ptable,
			BACKGROUND_BLUE | BACKGROUND_GREEN
			| FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		);
		menu_inactive_color(ptable,
			background | FOREGROUND_INTENSITY | FOREGROUND_RED |
			FOREGROUND_GREEN | FOREGROUND_BLUE);
		// ����������� �����: �������� ���� ��������� ��������� ������� �������
		menu_draw(ptable, MENU_FULL_FLAGS);
		// ��������, ����� �� ����������� ���� ����������, ���������� ���
		exit_code = 0;

	}
	menu_clear(ptable);
	free(ptable);

	for(i = 0; i < main_menu_items_count; i++)
		for(fld = 0; fld < DICT_FLD_CNT + 1; fld++)
			if(main_menu_items[i].str[fld])
				free(main_menu_items[i].str[fld]);
	free(main_menu_items);

	menu_clear(&top_menu);

	for(i = 0; i < main_column_count; i++)
		if(main_headers[i])
			free(main_headers[i]);
	return 0;
}
//---------------------------------------------------------------------------
// ������� ����
//---------------------------------------------------------------------------
// ������� ���� <�����>
int Exit(MENU* menu, ITEM* item) {
	int result;
	if(data_modified) {
		int i;
		MENU exit_menu;
		SMALL_RECT rect;
		const int width = 24;
		const int height = 7;
		rect.Left = (csbInfo.srWindow.Right - csbInfo.srWindow.Left - width) / 2;
		rect.Right = rect.Left + width - 1;
		rect.Top = (csbInfo.srWindow.Bottom - csbInfo.srWindow.Top - height) / 2;
		rect.Bottom = rect.Top + height - 1;
		menu_init(&exit_menu, ptable, menu->hStdOut, exit_menu_items, exit_item_count,
			exit_column_count, MENU_ORIENT_VERT, &rect, 1, exit_headers);
		exit_menu.user_tag = MENU_TAG_EXIT;
		menu_active_color(&exit_menu, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN);
		menu_inactive_color(&exit_menu, BACKGROUND_BLUE | BACKGROUND_GREEN);
		menu_add_hotkey(&exit_menu, KEY_ESC, DefaultESC);
		menu_draw(&exit_menu, MENU_FULL_FLAGS);
		menu_clear(&exit_menu);
		if(save_data) {
			Save(menu, item);
		}
	}
	if(exit_canceled) {
		exit_canceled = 0;
		result = 0;
	} else {
		exit_code = -1;
		result = -1;
	}
	return result;
}
// ������� ���� <��������>
int Add(MENU* pm, ITEM* item) {
	int i;
	SMALL_RECT wndRect = { 10, 11, 49, 15 };
	int max_width = (wndRect.Right - wndRect.Left - 1) / 2;
	DICT_ENTRY* entry;
	InputBox box;

	char*** contents;
	int row_count = DICT_FLD_CNT;
	contents = (char***)malloc(row_count * sizeof(char**));
	memset(contents, 0x00, row_count * sizeof(char**));
	for(i = 0; i < row_count; ++i) {
		contents[i] = (char**)malloc(COLUMNS * sizeof(char*));
		// �������
		contents[i][TITLE] = (char*)malloc((MAX_TITLE + 1) * sizeof(char));
		strcpy(contents[i][TITLE], main_headers[i]);
		//CharToOemA(main_headers[i], contents[i][TITLE]);
		// ����� �����
		contents[i][BUFFER] = (char*)malloc((max_width + 1) * sizeof(char));
		memset(contents[i][BUFFER], ' ', max_width * sizeof(char));
		//strcpy(contents[i][BUFFER], entry->field[i]);
		//CharToOemA(entry->field[i], contents[i][BUFFER]);
		contents[i][BUFFER][0] = '\0';
		contents[i][BUFFER][max_width] = '\0';
	}

	if(-1 == box_init(&box, pm->hStdOut, wndRect, contents, row_count)) {
		return -1;
	}
	if(-1 == box_save(&box)) {
		box_clear(&box);
		return -1;
	}
	if(1 == box_draw(&box)) {
		/*for(i = 0; i < row_count; ++i) {
			//strcpy(entry->field[i], contents[i][BUFFER]);
			OemToCharA(contents[i][BUFFER], entry->field[i]);
		}*/
		list1_push_back(&dict,
			dict_entry_new(contents[0][BUFFER], contents[1][BUFFER], contents[2][BUFFER])
		);
		data_modified = 1;
		redraw_main = 1;
		exit_code = -1;
	}

	box_clear(&box);

	for(i = 0; i < row_count; ++i) {
		if(contents[i]) {
			if(contents[i][TITLE])
				free(contents[i][TITLE]);
			if(contents[i][BUFFER]);
				free(contents[i][BUFFER]);
			free(contents[i]);
		}
	}
	free(contents);
	contents = NULL;

	return exit_code;
}
// ������� ���� <��������>
int Edit(MENU* pm, ITEM* item) {
	int i, box_result;
	SMALL_RECT wndRect = { 10, 11, 49, 15 };
	int max_width = (wndRect.Right - wndRect.Left - 1) / 2;
	DICT_ENTRY* entry = (DICT_ENTRY*)list1_curr(&dict);
	InputBox box;

	char*** contents;
	int row_count = DICT_FLD_CNT;
	contents = (char***)malloc(row_count * sizeof(char**));
	memset(contents, 0x00, row_count * sizeof(char**));
	for(i = 0; i < row_count; ++i) {
		contents[i] = (char**)malloc(COLUMNS * sizeof(char*));
		// �������
		contents[i][TITLE] = (char*)malloc((MAX_TITLE + 1) * sizeof(char));
		strcpy(contents[i][TITLE], main_headers[i]);
		//CharToOemA(main_headers[i], contents[i][TITLE]);
		// ����� �����
		contents[i][BUFFER] = (char*)malloc((max_width + 1) * sizeof(char));
		memset(contents[i][BUFFER], ' ', max_width * sizeof(char));
		//strcpy(contents[i][BUFFER], entry->field[i]);
		CharToOemA(entry->field[i], contents[i][BUFFER]);
		contents[i][BUFFER][max_width] = '\0';
	}

	initial_table_index = list1_get_current_index(&dict);
	if(-1 == box_init(&box, pm->hStdOut, wndRect, contents, row_count)) {
		return -1;
	}
	if(-1 == box_save(&box)) {
		box_clear(&box);
		return -1;
	}
	box_result = box_draw(&box);
	if(1 == box_result) {
		for(i = 0; i < row_count; ++i) {
			//strcpy(entry->field[i], contents[i][BUFFER]);
			OemToCharA(contents[i][BUFFER], entry->field[i]);
		}
		data_modified = 1;
		redraw_main = 1;
		exit_code = -1;
	}
	box_clear(&box);

	for(i = 0; i < row_count; ++i) {
		if(contents[i]) {
			if(contents[i][TITLE])
				free(contents[i][TITLE]);
			if(contents[i][BUFFER]);
				free(contents[i][BUFFER]);
			free(contents[i]);
		}
	}
	free(contents);
	contents = NULL;

	return exit_code;
}
// ������� ���� <�������>
int Delete(MENU* menu) {
	list1_erase_current(&dict);
	menu_del_curr(menu);
	menu_fill_wnd(menu, 0);
	menu_cls(menu);
	menu_draw(menu, MENU_FLAG_WND | MENU_FLAG_ITEMS | MENU_DRAW_SEL);
	data_modified = 1;
	return 0;
}
// ������� ������� <�����>
int SearchItem(MENU* pm, ITEM* item) {
	int i;
	int index;
	SMALL_RECT wndRect = { 3, 11,  50, 14 };
	SearchCriteria search;
	int max_width = (wndRect.Right - wndRect.Left - 1) / 2;
	InputBox box;

	char*** contents;
	int row_count = 2;
	char* titles[] = { "������ ������:", "����� ���� � �����" };
	contents = (char***)malloc(row_count * sizeof(char**));
	memset(contents, 0x00, row_count * sizeof(char**));
	for(i = 0; i < row_count; ++i) {
		contents[i] = (char**)malloc(COLUMNS * sizeof(char*));
		// �������
		contents[i][TITLE] = (char*)malloc((MAX_TITLE + 1) * sizeof(char));
		if (0 == i) {
			strcpy(contents[i][TITLE], item->str + 1); // �������� ��� ���������� �������
		} else {
			CharToOemA(titles[i], contents[i][TITLE]);
		}
		// ����� �����
		contents[i][BUFFER] = (char*)malloc((max_width + 1) * sizeof(char));
		memset(contents[i][BUFFER], ' ', max_width * sizeof(char));
		contents[i][BUFFER][0] = '\0';
		contents[i][BUFFER][max_width] = '\0';
	}
	strcpy(contents[1][BUFFER], "20");

	if(-1 == box_init(&box, pm->hStdOut, wndRect, contents, row_count)) {
		return -1;
	}
	if(-1 == box_save(&box)) {
		box_clear(&box);
		return -1;
	}
	if(-1 == box_draw(&box)) {
		box_clear(&box);
		return -1;
	}
	search.str = contents[0][BUFFER];
	OemToCharA(search.str, search.str);
	search.index = item->index;
	search.value = atoi(contents[1][BUFFER]);
	index = list1_search(&dict, dict_entry_find, &search);
	list1_set_current_index(&ptable->items, index);
	menu_fill_wnd(ptable, 0);
	menu_cls(ptable);
	menu_draw(ptable, MENU_FLAG_WND | MENU_FLAG_ITEMS | MENU_DRAW_SEL);

	box_clear(&box);

	for(i = 0; i < row_count; ++i) {
		if(contents[i]) {
			if(contents[i][TITLE])
				free(contents[i][TITLE]);
			if(contents[i][BUFFER]);
				free(contents[i][BUFFER]);
			free(contents[i]);
		}
	}
	free(contents);
	contents = NULL;
	return -1;
}
// ������� ���� <�����>
int Search(MENU* pm, ITEM* item) {
	int i;
	int max_len;
	// ��� �������������� ������ �� "���������� ����"
	int item_count = main_column_count - 1;
	int slen;
	int menu_width = 20;
	int menu_height = item_count + 2;
	SMALL_RECT rect;
	ITEM_DEF* menu_items;
	if(item) {
		rect.Left = item->x;
		rect.Right = rect.Left + menu_width - 1;
		rect.Top = csbInfo.srWindow.Top + 2;
		rect.Bottom = rect.Top + menu_height - 1;
	} else {
		int left = csbInfo.srWindow.Left, right = csbInfo.srWindow.Right;
		int top = csbInfo.srWindow.Top, bottom = csbInfo.srWindow.Bottom;
		rect.Left = left + (right - left + 1 - menu_width) / 2;
		rect.Right = rect.Left + menu_width - 1;
		rect.Top = top + (bottom - top + 1 - menu_height) / 2;
		rect.Bottom = rect.Top + menu_height - 1;
	}
	max_len = menu_width - 2; // 2 ����. ����� �� �����
	menu_items = (ITEM_DEF*)malloc(item_count * sizeof(ITEM_DEF));
	memset(menu_items, 0x00, item_count * sizeof(ITEM_DEF));
	for(i = 0; i < item_count; i++) {
		menu_items[i].str[0] = (char*)malloc(max_len * sizeof(char));
		strncpy(menu_items[i].str[0], main_headers[i], max_len);
		menu_items[i].str[0][max_len] = '\0';
		menu_items[i].cb = SearchItem;
	}
	ShowMenu(pm->hStdOut, menu_items, item_count, NULL, rect, MENU_FULL_FLAGS, MENU_TAG_SORT, F1);
	return -1;
}
// ������� ������� <����������>
int SortItem(MENU* pm, ITEM* item) {
	list1_sort(&dict, dict_entry_compare, &item->index);
	initial_table_index = list1_get_current_index(&dict);
	redraw_main = 1;
	exit_code = -1;
	return exit_code;
}
// ������� ���� <����������>
int Sort(MENU* pm, ITEM* item) {
	int i;
	int max_len;
	int item_count = main_column_count;
	int slen;
	int menu_width = 20;
	int menu_height = item_count + 2;
	SMALL_RECT rect;
	ITEM_DEF* menu_items;
	
	rect.Left = item->x;
	rect.Right = item->x + menu_width - 1;
	rect.Top = csbInfo.srWindow.Top + 2;
	rect.Bottom = rect.Top + menu_height - 1;
	max_len = menu_width - 2; // 2 ����. ����� �� �����
	menu_items = (ITEM_DEF*)malloc(item_count * sizeof(ITEM_DEF));
	memset(menu_items, 0x00, item_count * sizeof(ITEM_DEF));
	for(i = 0; i < item_count; i++) {
		menu_items[i].str[0] = (char*)malloc(max_len * sizeof(char));
		strncpy(menu_items[i].str[0], main_headers[i], max_len);
		menu_items[i].str[0][max_len] = '\0';
		menu_items[i].cb = SortItem;
	}
	ShowMenu(pm->hStdOut, menu_items, item_count, NULL, rect, MENU_FULL_FLAGS, MENU_TAG_SORT, F1);
	return -1;
}
// ������� ���� <���������>
int Save(MENU* menu, ITEM* item) {
	save_data = 0;
	data_modified = 0;
	dict_save(dict_file_name);
	return -1;
}
// ������� ���� <������>. ����������� ����� ������������
int Help(MENU* pm, ITEM* item) {
	int result = 0;
	SMALL_RECT rect;
	int width = 0, height = 0;
	switch(pm->user_tag) {
	case MENU_TAG_TOP:
	case MENU_TAG_MAIN:
		rect.Left = csbInfo.srWindow.Left + 2;
		rect.Right = csbInfo.srWindow.Right - 2;
		rect.Top = csbInfo.srWindow.Top + 3;
		rect.Bottom = csbInfo.srWindow.Bottom - 2;
		result = HelpFromFile(pm->hStdOut, "help.txt", "������� � ���������", rect);
		break;
	case MENU_TAG_SORT:
		width = 55;
		height = 6;
		rect.Left = (csbInfo.srWindow.Right - csbInfo.srWindow.Left - width) / 2;
		rect.Right = rect.Left + width - 1;
		rect.Top = (csbInfo.srWindow.Bottom - csbInfo.srWindow.Top - height) / 2;
		rect.Bottom = rect.Top + height - 1;
		result = HelpFromFile(pm->hStdOut, "sort.txt", "�������: ������� ����������", rect);
		break;
	}
	return result;
}

// ��������������� �������

// �������� ������� ���� �� �����
ITEM_DEF* MenuItemsFromFile(const char* file_name, int max_count, int max_len, int* pcount) {
	FILE* file;
	int i;
	int item_count = 0;
	int slen;

	ITEM_DEF* menu_items = (ITEM_DEF*)malloc(max_count * sizeof(ITEM_DEF));
	if(!menu_items)
		return NULL;
	memset(menu_items, 0x00, max_count * sizeof(ITEM_DEF));

	file = fopen(file_name, "r");
	if(!file) {
		free(menu_items);
		return NULL;
	}
	i = 0;
	while(!feof(file)) {
		menu_items[i].str[0] = (char*)malloc(max_len * sizeof(char));
		if(!menu_items[i].str[0])
			break;
		menu_items[i].str[0][0] = '\0';
		if(fgets(menu_items[i].str[0], max_len, file)) {
			slen = strlen(menu_items[i].str[0]);
			// ������� ������������� ������ - '\n'
			menu_items[i].str[0][slen > 1 ? slen - 1 : 0] = '\0';
		} else {
			menu_items[i].str[0][0] = '\0';
		}
		if(++i == max_count) {
			break; // ��������� ����� �����
		}
	}
	fclose(file);
	item_count = i;
	if(pcount)
		*pcount = item_count;
	return menu_items;
}
// ����������� ����������� ������������� ���� � ���� �������
int ShowMenu(HANDLE hStdOut, ITEM_DEF* menu_items, int item_count, const char* title, SMALL_RECT rect, int flags, int user_tag, ExecuteHotketCB f1CB) {
	int i;
	MENU menu;
	// ���������� ����� ���� (��������)
	const int column_count = 1;
	char** headers = NULL;
	int border = 1;

	if(title) {
		headers = (char**)malloc(column_count * sizeof(char*));
		memset(headers, 0x00, column_count * sizeof(char*));
		headers[0] = (char*)malloc(MAX_MENU_HDR * sizeof(char));
		if(!headers[0]) {
			free(menu_items);
			return -1;
		}
		strcpy(headers[0], title);
		CharToOemA(headers[0], headers[0]);
	}

	menu_init(&menu, ptable, hStdOut, menu_items, item_count, column_count,
		MENU_ORIENT_VERT, &rect, 1, headers);
	menu.user_tag = user_tag;
	menu_active_color(&menu, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_inactive_color(&menu, BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_add_hotkey(&menu, KEY_ESC, DefaultESC);
	menu_add_hotkey(&menu, KEY_F1, f1CB);
	menu_draw(&menu, flags);
	menu_clear(&menu);

	for(i = 0; i < item_count; ++i)
		if(menu_items[i].str[0])
			free(menu_items[i].str[0]);
	free(menu_items);

	if(headers) {
		for(i = 0; i < column_count; ++i)
			if(headers[i])
				free(headers[i]);
		free(headers);
	}

	return 0;
}
// ����������� ������ ������� �� ����� �� ����������� ����
int HelpFromFile(HANDLE hStdOut, const char* file_name, const char* title, SMALL_RECT rect) {
	int i;
	//int menu_width = rect.Right - rect.Left + 1;
	int menu_height = rect.Bottom - rect.Top + 1;
	int item_count = 0;
	// 4 = ��������� + 3 �����. ������
	ITEM_DEF* menu_items = MenuItemsFromFile(file_name, menu_height - 4, 256, &item_count);
	if(!menu_items) {
		return -1;
	}
	return ShowMenu(hStdOut, menu_items, item_count, title, rect, MENU_FLAG_WND | MENU_FLAG_ITEMS | MENU_HOTKEYS, MENU_TAG_HELP_GLOBAL, DefaultESC);
}
// �������� �������� ������ � �������
void LoadInitialData() {
	if(-1 == list1_push_back(&dict, dict_entry_new("hello", "noun", "������")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("world", "noun", "���")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("go", "verb", "����")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("here", "adverb", "�����")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("run", "noun", "������")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("smart", "adjective", "�����")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("those", "pronoun", "��")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("between", "preposition", "�����")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("under", "preposition", "���")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("star", "noun", "������")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("label", "noun", "�����")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("recall", "verb", "���������")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("nice", "adjective", "��������")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("cat", "noun", "���")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("below", "adverb", "�����")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("snake", "noun", "����")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("sit", "verb", "������")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("language", "noun", "����")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("he", "pronoun", "��")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("she", "pronoun", "���")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("into", "preposition", "�")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("yours", "pronoun", "����")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("today", "adverb", "�������")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("strong", "adjective", "�������")))
		return -1;
	if(-1 == list1_push_back(&dict, dict_entry_new("human", "noun", "�������")))
		return -1;
	if(-1 == dict_save(dict_file_name))
		return -1;
	list1_clear(&dict);
	getchar();
}

