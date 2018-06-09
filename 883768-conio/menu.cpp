//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "menu.h"
#include "dict.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define KEY_ARROW_UP 72
#define KEY_ARROW_LEFT 75
#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_DOWN 80
#define KEY_ENTER 13

int InitMenu(MENU* menu, ITEM* items, int item_count, int orient, const SMALL_RECT* prect) {
	int i, len;
	if(NULL == menu || NULL == items)
		return -1;
	// �������� ���������� ����������� ������
	menu->hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	// ��������� ���������� ���������
	if(prect != NULL ) {
		menu->wnd.rect.Left = prect->Left;//menu->csbInfo.srWindow.Left + hpad;
		menu->wnd.rect.Top = prect->Top;//menu->csbInfo.srWindow.Top + vpad;
		menu->wnd.rect.Right = prect->Right;//menu->csbInfo.srWindow.Right - hpad;
		menu->wnd.rect.Bottom = prect->Bottom;//menu->csbInfo.srWindow.Bottom - vpad;
	} else {
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;// ���������� � ���������� ���� � ��������� csbInfo
		// �������� ������� �������
		GetConsoleScreenBufferInfo(menu->hStdOut, &csbInfo);
		menu->wnd.rect.Left = csbInfo.srWindow.Left;
		menu->wnd.rect.Top = csbInfo.srWindow.Top;
		menu->wnd.rect.Right = csbInfo.srWindow.Right;
		menu->wnd.rect.Bottom = csbInfo.srWindow.Bottom;
	}
	menu->wnd.M = menu->wnd.rect.Bottom - menu->wnd.rect.Top + 1;
	menu->wnd.N = menu->wnd.rect.Right - menu->wnd.rect.Left + 1;
	menu->wnd.m = (char**)malloc((menu->wnd.M) * sizeof(char*));
	for(int i = 0; i < menu->wnd.M; i++) {
		menu->wnd.m[i] = (char*)malloc((menu->wnd.N + 1) * sizeof(char));
		memset(menu->wnd.m[i], ' ', menu->wnd.N * sizeof(char));
		menu->wnd.m[i][menu->wnd.N] = '\0';
	}
	// ���������� ����� �������
//	menu->consolRect = csbInfo.srWindow;
	menu->curspos.X = menu->wnd.rect.Left;///0;
	menu->curspos.Y = menu->wnd.rect.Top;///1;
	menu->workWindowAttributes = 158;
	menu->inactiveItemAttributes = 31;
	menu->activeItemAttributes = 160;
	// ������������� �������� ��������� ������
	SetConsoleTextAttribute(menu->hStdOut, menu->workWindowAttributes);
	#define MAX(a, b) (((a) > (b)) ? (a) : (b))
	menu->items = (ITEM*)malloc(item_count * sizeof(ITEM));
	menu->orient = orient;
	menu->current = 0;
	int next = 0, width;
	switch(orient) {
	case MENU_ORIENT_HORZ:
		menu->item_width = (menu->wnd.rect.Right - menu->wnd.rect.Left + 1) / item_count;
		next = menu->curspos.X;
		for(i = 0; i < item_count; ++i) {
			if(items[i].str) {
				len = strlen(items[i].str);
				memset(&menu->items[i], 0x00, sizeof(ITEM));
				menu->items[i].x = next;
				menu->items[i].y = menu->curspos.Y;
				width = MAX(len, menu->item_width);
				menu->items[i].str = (char*)malloc((width + 1) * sizeof(char));

				memset(menu->items[i].str, ' ', width);
				if(len < menu->item_width) {
					int indent = (menu->item_width - len) / 2;
					memcpy(menu->items[i].str + indent, items[i].str, len);
				} else {
					memcpy(menu->items[i].str, items[i].str, menu->item_width - 1);
					menu->items[i].str[menu->item_width - 1] = '...';
				}
				menu->items[i].str[width] = '\0';

				menu->items[i].cb = items[i].cb;
				next += menu->item_width;
			}
		}
		break;
	case MENU_ORIENT_VERT:
		menu->item_width = (menu->wnd.rect.Right - menu->wnd.rect.Left + 1);
		next = menu->curspos.Y;
		for(i = 0; i < item_count; ++i) {
			if(items[i].str) {
				len = strlen(items[i].str);
				memset(&menu->items[i], 0x00, sizeof(ITEM));
				menu->items[i].x = menu->curspos.X;
				menu->items[i].y = next;
				width = MAX(len, menu->item_width);
				menu->items[i].str = (char*)malloc((width + 1) * sizeof(char));

				memset(menu->items[i].str, ' ', width);
				if(len < menu->item_width) {
					int indent = 0;//(menu->item_width - len) / 2;
					memcpy(menu->items[i].str + indent, items[i].str, len);
				} else {
					memcpy(menu->items[i].str, items[i].str, menu->item_width - 1);
					menu->items[i].str[menu->item_width - 1] = '...';
				}
				menu->items[i].str[width] = '\0';

				menu->items[i].cb = items[i].cb;
				next++;
			}
		}
		break;
	}
	menu->item_count = item_count;

#if 0
	system("CLS"); // ��������� ��������� ����� ������� �������
#else
	menu_cls(menu);
#endif

	return 0;
}

void ClearMenu(MENU* menu) {
	int i;
	for (i = 0; i < menu->item_count; i++)
		if(menu->items[i].str)
			free(menu->items[i].str);
	free(menu->items);
	for(i = 0; i < menu->wnd.M; i++)
		if(menu->wnd.m[i])
			free(menu->wnd.m[i]);
	free(menu->wnd.m);
//?	CloseHandle(menu->hStdOut);
}

void menu_prev(MENU* menu) {
	itemMenu(menu, false); // ������� ���������� ����� ����
	if (menu->current > 0) {
		--menu->current;
	} else {
		menu->current = menu->item_count - 1; // ��������� ������
	}
	itemMenu(menu, true); // �������� �������� ����� ����
	showCursor(menu, false);
}

void menu_next(MENU* menu) {
	itemMenu(menu, false); // ������� ���������� ����� ����
	if (menu->current < menu->item_count - 1) {
		++menu->current;
	} else {
		menu->current = 0; // ��������� �����
	}
	itemMenu(menu, true); // �������� �������� ����� ����
	showCursor(menu, false);
}

void DrawMenu(MENU* menu, int loop) {
	// ����� �������� ������ ����
	SetConsoleTextAttribute(menu->hStdOut, menu->inactiveItemAttributes);
///	printf(menu->line);
///	fflush(stdout);

	for (int i = 0; i < menu->item_count; i++) { // ���������� ��������� ������� ����
		gotoxy(menu, menu->items[i].x, menu->items[i].y);
		printf(menu->items[i].str);
	}
	fflush(stdout);

	if(0 == loop)
		return;

	itemMenu(menu, true); // �������� ����� ����

	fflush(stdin); // �������� ����� ����������

	int cb_retcode = 0;
	int iKey = 67;
	int run = 1;
	while (run) {
		if (kbhit()) {
			iKey = _getch();
			switch (iKey) {
			case KEY_ARROW_UP:
				if(MENU_ORIENT_VERT == menu->orient)
					menu_prev(menu);
			case KEY_ARROW_LEFT:
				if(MENU_ORIENT_HORZ == menu->orient)
					menu_prev(menu);
				break;
			case KEY_ARROW_RIGHT:
				if(MENU_ORIENT_HORZ == menu->orient)
					menu_next(menu);
				break;
			case KEY_ARROW_DOWN:
				if(MENU_ORIENT_VERT == menu->orient)
					menu_next(menu);
				break;
				break;
			case KEY_ENTER:
				// ���������� ������ �� ������ ���� � ������� �������
				///gotoxy(menu, menu->curspos.X, menu->curspos.Y);
				// ���������� ���� ������� ���������
				///SetConsoleTextAttribute(menu->hStdOut, menu->workWindowAttributes);
				///showCursor(menu, true);
				// �������� ���������� ������ ����
				cb_retcode = menu->items[menu->current].cb(menu);
				if(-1 == cb_retcode) {
					///gotoxy(menu, 0, 0);
					///menu_cls(menu, WholeWindow);
					run = 0;
					break;///exit(0);
				} else if(0 == cb_retcode) {
					saveCursorPosition(menu);
				}
				// �������� ����� ����������
				fflush(stdin);
				// ������ � ������� ����� ����
				///gotoxy(menu, menu->items[menu->current].x, menu->items[menu->current].y);
				// �������� ������
				showCursor(menu, false);
				break;
			case 120: // ����� �� ������� x
			case 88: // ����� �� ������� X
			case 27: // ����� �� ������� ESC
				gotoxy(menu, 0, 0);
				///menu_cls(menu, WholeWindow);
				run = 0;///exit(0);
			} // switch(iKey)
		} // if(kbhit())
	} // while(1)
}

void gotoxy(MENU* menu, int x, int y)
{
	COORD cursorPos = { x, y };
	SetConsoleCursorPosition(menu->hStdOut, cursorPos);
	///SetConsoleCursorPosition(hStdOut, {x,y});
}

void saveCursorPosition(MENU* menu)
{
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;// ���������� � ���������� ���� � ��������� csbInfo
	GetConsoleScreenBufferInfo(menu->hStdOut, &csbInfo);
	menu->curspos = csbInfo.dwCursorPosition;
}

void itemMenu(MENU* menu, bool activate)
{
	WORD itemAttributes;
	if (activate)
		itemAttributes = menu->activeItemAttributes;
	else
		itemAttributes = menu->inactiveItemAttributes;
	gotoxy(menu, menu->items[menu->current].x, menu->items[menu->current].y);
	SetConsoleTextAttribute(menu->hStdOut, itemAttributes);
	printf(menu->items[menu->current].str);
}

void showCursor(MENU* menu, bool visible)
{
	CONSOLE_CURSOR_INFO ccInfo;
	ccInfo.bVisible = visible;
	ccInfo.dwSize = 20;
	SetConsoleCursorInfo(menu->hStdOut, &ccInfo);
}

void menu_cls(MENU* menu)
{
	int i, y;
	SetConsoleTextAttribute(menu->hStdOut, menu->workWindowAttributes);
	//gotoxy(menu, rect.Left, rect.Top);
	// ������� ����
	switch(menu->orient) {
	case MENU_ORIENT_HORZ:
		for (i = 0, y = menu->wnd.rect.Top; i < menu->wnd.M; i++, y++) {
			gotoxy(menu, menu->wnd.rect.Left, y);
			printf(menu->wnd.m[i]); // ������ ��� ������ ����
		}
		break;
	case MENU_ORIENT_VERT:
		for (i = 0, y = menu->wnd.rect.Top; i < menu->wnd.M; i++, y++) {
			gotoxy(menu, menu->wnd.rect.Left, y);
			printf(menu->wnd.m[i]); // ������ ��� ������ ����
		}
		break;
	}
	//gotoxy(menu, 0, 0);
}

