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

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define KEY_ARROW_UP 72
#define KEY_ARROW_LEFT 75
#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_DOWN 80
#define KEY_ENTER 13

#define CHAR_BORDER_HORZ 205
#define CHAR_BORDER_VERT 186
#define CHAR_BORDER_LEFT_TOP 201
#define CHAR_BORDER_LEFT_BOTTOM 200
#define CHAR_BORDER_RIGHT_TOP 187
#define CHAR_BORDER_RIGHT_BOTTOM 188
#define CHAR_BORDER_LEFT_JOINT 204
#define CHAR_BORDER_RIGHT_JOINT 185
#define CHAR_BORDER_TOP_JOINT 203
#define CHAR_BORDER_BOTTOM_JOINT 202
#define CHAR_BORDER_CROSS_JOINT 206
#define MENU_WHITESPACE ' '

void menu_active_color(MENU* menu, WORD attr) {
	menu->activeItemAttributes = attr;
}

void menu_inactive_color(MENU* menu, WORD attr) {
	menu->inactiveItemAttributes = attr;
}

int menu_init(MENU* menu, HANDLE hstdout, ITEM_DEF* item_defs, int item_count, int cell_count,
	int orient, const SMALL_RECT* prect, int border, const char* headers[])
{
	int i, j, len, next_cell;
	int border_bottom_index = 0; // для окна
	if(NULL == menu || NULL == item_defs || INVALID_HANDLE_VALUE == hstdout)
		return -1;
	memset(menu, 0x00, sizeof(MENU));
	// Получаем дескриптор консольного вывода
	menu->hStdOut = hstdout;
	//
	menu->border = border;
	menu->left_pad = 0;
	menu->top_pad = 0;
	if(MENU_ORIENT_VERT == orient) { //TODO MENU_ORIENT_HORZ
		if(headers) {
			menu->top_pad += 2;
			border_bottom_index += 2;
		}
		if(border) {
			menu->left_pad += 1;
			menu->top_pad += 1;
			border_bottom_index += 1;
			border_bottom_index += item_count;
		}
	}
	// Сохраняем полученные параметры
	if(prect != NULL ) {
		menu->wnd.rect.Left = prect->Left;
		menu->wnd.rect.Top = prect->Top;
		menu->wnd.rect.Right = prect->Right;
		menu->wnd.rect.Bottom = prect->Bottom;
	} else {
		// Получаем размеры консоли
		CONSOLE_SCREEN_BUFFER_INFO csbInfo;
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
		memset(menu->wnd.m[i], MENU_WHITESPACE, menu->wnd.N * sizeof(char));
		menu->wnd.m[i][menu->wnd.N] = '\0';
		if(border) {
			if(i <= border_bottom_index) {
				menu->wnd.m[i][0] = CHAR_BORDER_VERT;
				menu->wnd.m[i][menu->wnd.N - 1] = CHAR_BORDER_VERT;
			}
			if(headers != NULL && 2 == i) {
				memset(menu->wnd.m[i], CHAR_BORDER_HORZ, menu->wnd.N * sizeof(char));
				menu->wnd.m[i][0] = CHAR_BORDER_LEFT_JOINT;
				menu->wnd.m[i][menu->wnd.N - 1] = CHAR_BORDER_RIGHT_JOINT;
			}
			if(0 == i || border_bottom_index == i) {
				memset(menu->wnd.m[i], CHAR_BORDER_HORZ, menu->wnd.N * sizeof(char));
				if(0 == i) { // первая строка
					menu->wnd.m[i][0] = CHAR_BORDER_LEFT_TOP;
					menu->wnd.m[i][menu->wnd.N - 1] = CHAR_BORDER_RIGHT_TOP;
				}
				if(border_bottom_index == i) { // последняя строка
					menu->wnd.m[i][0] = CHAR_BORDER_LEFT_BOTTOM;
					menu->wnd.m[i][menu->wnd.N - 1] = CHAR_BORDER_RIGHT_BOTTOM;
				}
			}
		}
	}
	// координаты углов консоли
	menu->curspos.X = menu->wnd.rect.Left;///0;
	menu->curspos.Y = menu->wnd.rect.Top;///1;

	menu->workWindowAttributes = //0x9E;
		BACKGROUND_RED | BACKGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	menu->inactiveItemAttributes = 0x1F;
	menu->activeItemAttributes = 0xA0;
	// Инициализация элементов меню
	menu->items = (ITEM*)malloc(item_count * sizeof(ITEM));
	menu->orient = orient;
	menu->current = 0;
	int next = 0, width;
	switch(orient) {
	case MENU_ORIENT_HORZ:
		menu->item_width = (menu->wnd.rect.Right - menu->wnd.rect.Left + 1) / item_count;
		menu->cell_count = 1; // cell_count;
		next = menu->curspos.X;
		for(i = 0; i < item_count; ++i) {
			memset(&menu->items[i], 0x00, sizeof(ITEM));
			menu->items[i].cb = item_defs[i].cb;
			menu->items[i].x = next + menu->left_pad;
			menu->items[i].y = menu->curspos.Y + menu->top_pad;
			int next_cell = 0; // смещение очередной ячейки вниз
			if(item_defs[i].str[0]) {
				len = strlen(item_defs[i].str[0]);
				width = MAX(len, menu->item_width);
				menu->items[i].str = (char*)malloc((width + 1) * sizeof(char));

				memset(menu->items[i].str, MENU_WHITESPACE, width);
				if(len < menu->item_width) {
					int indent = (menu->item_width - len) / 2;
					memcpy(menu->items[i].str + indent, item_defs[i].str[0], len);
				} else {
					memcpy(menu->items[i].str, item_defs[i].str[0], menu->item_width - 1);
					menu->items[i].str[menu->item_width - 1] = '_';
				}
				menu->items[i].str[width] = '\0';

				next += menu->item_width;
			}
		} // for(item)
		break;
	case MENU_ORIENT_VERT:
		menu->item_width = (menu->wnd.rect.Right - menu->wnd.rect.Left + 1)
			- (border ? 2 : 0);
		menu->cell_count = cell_count;
		int cell_width = menu->item_width / cell_count;

		if(headers) {
			next_cell = 0; // смещение очередной ячейки вправо
			menu->hdr = (char*)malloc((menu->item_width + 1) * sizeof(char));
			memset(menu->hdr, MENU_WHITESPACE, menu->item_width);
			menu->hdr[menu->item_width] = '\0';
			for (j = 0; j < cell_count; j++) {
				if(headers[j]) {
					len = strlen(headers[j]);
					width = MAX(len, cell_width);
					if(len < cell_width) {
						memcpy(menu->hdr + next_cell, headers[j], len);
					} else {
						memcpy(menu->hdr + next_cell, headers[j], cell_width - 1);
						menu->hdr[next_cell + cell_width - 1] = '_';
					}
					if(border && j < cell_count - 1) {
						menu->wnd.m[0][next_cell + cell_width - 0] = CHAR_BORDER_TOP_JOINT;
						menu->hdr[next_cell + cell_width - 1] = CHAR_BORDER_VERT;
						menu->wnd.m[2][next_cell + cell_width - 0] = CHAR_BORDER_BOTTOM_JOINT;
					}
					next_cell += cell_width;
				}
			} // for(cell)
		} // if(headers)

		next = menu->curspos.Y;
		for(i = 0; i < item_count; ++i) {
			memset(&menu->items[i], 0x00, sizeof(ITEM));
			menu->items[i].cb = item_defs[i].cb;
			menu->items[i].x = menu->curspos.X + menu->left_pad;
			menu->items[i].y = next + menu->top_pad;
			next_cell = 0; // смещение очередной ячейки вправо
			menu->items[i].str = (char*)malloc((menu->item_width + 1) * sizeof(char));
			memset(menu->items[i].str, MENU_WHITESPACE, menu->item_width);
			menu->items[i].str[menu->item_width] = '\0';
			for (j = 0; j < cell_count; j++) {
				if(item_defs[i].str[j]) {
					len = strlen(item_defs[i].str[j]);
					width = MAX(len, cell_width);
					//menu->items[i].str[j] = (char*)malloc((width + 1) * sizeof(char));

					//memset(menu->items[i].str[j], MENU_WHITESPACE, width);
					if(len < cell_width) {
						memcpy(menu->items[i].str + next_cell, item_defs[i].str[j], len);
					} else {
						memcpy(menu->items[i].str + next_cell, item_defs[i].str[j], cell_width - 1);
						menu->items[i].str[next_cell + cell_width - 1] = '_';
					}
					if(border && j < cell_count - 1) {
						int top_shift;
						if(headers) {
							top_shift = 2;
							menu->wnd.m[top_shift][next_cell + cell_width - 0] = CHAR_BORDER_CROSS_JOINT;
						} else {
							top_shift = 0;
							menu->wnd.m[top_shift][next_cell + cell_width - 0] = CHAR_BORDER_TOP_JOINT;
						}
						menu->items[i].str[next_cell + cell_width - 1] = CHAR_BORDER_VERT;
						menu->wnd.m[i + top_shift + 2][next_cell + cell_width - 0] = CHAR_BORDER_BOTTOM_JOINT;
					}
					next_cell += cell_width;
				}
			} // for(cell)
			next++;
		} // for(item)
		break;
	}
	menu->item_count = item_count;

	return 0;
}

void menu_clear(MENU* menu) {
	int i;
	for (i = 0; i < menu->item_count; i++)
		if(menu->items[i].str) {
			free(menu->items[i].str);
			menu->items[i].str = NULL;
		}
	free(menu->items);
	menu->items = NULL;

	if(menu->hdr) {
		free(menu->hdr);
		menu->hdr = NULL;
	}

	for(i = 0; i < menu->wnd.M; i++)
		if(menu->wnd.m[i]) {
			free(menu->wnd.m[i]);
			menu->wnd.m[i] = NULL;
		}
	free(menu->wnd.m);
	menu->wnd.m = NULL;

	menu->hStdOut = INVALID_HANDLE_VALUE;
}

void menu_prev(MENU* menu) {
	itemMenu(menu, false); // сделать неактивным пункт меню
	if (menu->current > 0) {
		--menu->current;
	} else {
		menu->current = menu->item_count - 1; // прокрутка вправо
	}
	itemMenu(menu, true); // выделить активный пункт меню
	showCursor(menu, false);
}

void menu_next(MENU* menu) {
	itemMenu(menu, false); // сделать неактивным пункт меню
	if (menu->current < menu->item_count - 1) {
		++menu->current;
	} else {
		menu->current = 0; // прокрутка влево
	}
	itemMenu(menu, true); // выделить активный пункт меню
	showCursor(menu, false);
}

void menu_draw(MENU* menu, int loop) {
	// Устанавливаем цветовые параметры текста
	SetConsoleTextAttribute(menu->hStdOut, menu->workWindowAttributes);
	//system("CLS"); // установка атрибутов цвета рабочей области
	menu_cls(menu);
	// Номер текущего пункта меню
	SetConsoleTextAttribute(menu->hStdOut, menu->inactiveItemAttributes);
///	printf(menu->line);
///	fflush(stdout);

	// рисуем заголовок
	if(menu->hdr) {
		int top_pad = menu->border ? 1 : 0;
		gotoxy(menu, menu->wnd.rect.Left + menu->left_pad, menu->wnd.rect.Top + top_pad);
		printf(menu->hdr);
	}
	// рисуем меню
	for (int i = 0; i < menu->item_count; i++) { // Напечатать заголовки пунктов меню
		gotoxy(menu, menu->items[i].x, menu->items[i].y);
		printf(menu->items[i].str);
	}
	fflush(stdout);

	if(0 == loop)
		return;

	itemMenu(menu, true); // выделить пункт меню

	fflush(stdin); // очистить буфер клавиатуры

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
				// Возвращаем курсор из строки меню в прежнюю позицию
				///gotoxy(menu, menu->curspos.X, menu->curspos.Y);
				// Установить цвет рабочих сообщений
				///SetConsoleTextAttribute(menu->hStdOut, menu->workWindowAttributes);
				///showCursor(menu, true);
				// Вызываем обработчик пункта меню
				cb_retcode = menu->items[menu->current].cb(menu);
				if(-1 == cb_retcode) {
					///gotoxy(menu, 0, 0);
					///menu_cls(menu, WholeWindow);
					run = 0;
					break;///exit(0);
				} else if(0 == cb_retcode) {
					saveCursorPosition(menu);
				}
				// очистить буфер клавиатуры
				fflush(stdin);
				// курсор в текущий пункт меню
				///gotoxy(menu, menu->items[menu->current].x, menu->items[menu->current].y);
				// спрятать курсор
				showCursor(menu, false);
				break;
			case 120: // выход по клавише x
			case 88: // выход по клавише X
			case 27: // выход по клавише ESC
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
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;// информация о консольном окне в структуре csbInfo
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
	// очистка окна
	switch(menu->orient) {
	case MENU_ORIENT_HORZ:
		for (i = 0, y = menu->wnd.rect.Top; i < menu->wnd.M; i++, y++) {
			gotoxy(menu, menu->wnd.rect.Left, y);
			printf(menu->wnd.m[i]); // залить фон строки меню
		}
		break;
	case MENU_ORIENT_VERT:
		for (i = 0, y = menu->wnd.rect.Top; i < menu->wnd.M; i++, y++) {
			gotoxy(menu, menu->wnd.rect.Left, y);
			printf(menu->wnd.m[i]); // залить фон строки меню
		}
		break;
	}
	//gotoxy(menu, 0, 0);
}

