//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "menu.h"
#include "dict.h"
#include "codes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define MENU_WHITESPACE ' '

int menu_calc_item_pos(void* data, int index, void* ptr);
// Отрисовка пункта меню (callback для элемента списка)
int menu_draw_item(void* data, int index, void* ptr);
// Проверить код, возвращённый обработчиком
int check_cb_retcode(MENU* menu);
// Очистка одного элемента меню
void menu_item_release(void* ptr);

// Обработка горячей клавиши (если была нажата)
int menu_proc_hotkey(void* data, int index, void* ptr);

void menu_active_color(MENU* menu, WORD attr) {
	menu->activeItemAttributes = attr;
}

void menu_inactive_color(MENU* menu, WORD attr) {
	menu->inactiveItemAttributes = attr;
	menu->workWindowAttributes = attr;
}

void menu_fill_wnd(MENU* menu, int items_count) {
	int i, border_bottom_index;
	if(items_count == 0)
		items_count = menu->items.count;
	border_bottom_index = menu->border_bottom_index + items_count;
	// проход по строкам окна
	for(i = 0; i < menu->wnd.M; i++) {
		//menu->wnd.m[i] = (char*)malloc((menu->wnd.N + 1) * sizeof(char));
		memset(menu->wnd.m[i], MENU_WHITESPACE, menu->wnd.N * sizeof(char));
		menu->wnd.m[i][menu->wnd.N] = '\0';
		if(menu->has_border) {
			// отрисовка вертикальной границы слева и справа
			if(i < border_bottom_index) {
				menu->wnd.m[i][0] = CHAR_BORDER_VERT;
				menu->wnd.m[i][menu->wnd.N - 1] = CHAR_BORDER_VERT;
			}
			// отрисовка горизонтальной линии отделения заголовка и её стыковочных линий слева/справа
			if(menu->hdr != NULL && 2 == i) {
				memset(menu->wnd.m[i], CHAR_BORDER_HORZ, menu->wnd.N * sizeof(char));
				menu->wnd.m[i][0] = CHAR_BORDER_LEFT_JOINT;
				menu->wnd.m[i][menu->wnd.N - 1] = CHAR_BORDER_RIGHT_JOINT;
			}
			if(0 == i || border_bottom_index == i) {
				// отрисовка верхней и нижней горизонтальных линий, а также 4 внешних углов
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
	if(menu->has_border) {
		for (i = 0; i < menu->cell_count; i++) {
			if(menu->has_headers) {
				if(i < menu->cell_count - 1) {
					// отрисовка вертикальных линий в заголовке
					menu->wnd.m[0][menu->cell_start[i] + menu->cell_width - 0] = CHAR_BORDER_TOP_JOINT;
					menu->wnd.m[2][menu->cell_start[i] + menu->cell_width - 0] = CHAR_BORDER_BOTTOM_JOINT;
				}
			}
			if(i < menu->cell_count - 1) {
				int top_shift = menu->top_pad - 1;
				if(menu->has_headers) {
					menu->wnd.m[top_shift][menu->cell_start[i] + menu->cell_width - 0] = CHAR_BORDER_CROSS_JOINT;
				} else {
					menu->wnd.m[top_shift][menu->cell_start[i] + menu->cell_width - 0] = CHAR_BORDER_TOP_JOINT;
				}
				menu->wnd.m[border_bottom_index][menu->cell_start[i] + menu->cell_width - 0] = CHAR_BORDER_BOTTOM_JOINT;
			}
		} // if(menu->has_headers)
	} // if(menu->has_border)
}

int menu_init(MENU* menu, MENU* parent, HANDLE hstdout, ITEM_DEF* item_defs, int item_count, int cell_count,
	int orient, const SMALL_RECT* prect, int border, char* headers[])
{
	int left_indent = 1;
	int i, j, len;
	if(NULL == menu || NULL == item_defs || INVALID_HANDLE_VALUE == hstdout)
		return -1;
	memset(menu, 0x00, sizeof(MENU));
	menu->parent = parent;
	menu->orient = orient;

	list1_init(&menu->hk_list);

	// Сохраняем дескриптор консольного вывода
	menu->hStdOut = hstdout;
	// Сохраняем полученные размеры окна
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
	for(i = 0; i < menu->wnd.M; i++) {
		menu->wnd.m[i] = (char*)malloc((menu->wnd.N + 1) * sizeof(char));
		memset(menu->wnd.m[i], MENU_WHITESPACE, menu->wnd.N * sizeof(char));
		menu->wnd.m[i][menu->wnd.N] = '\0';
	}
	//
	menu->has_border = border;
	menu->has_headers = (int)headers;
	menu->cell_count = cell_count;

	menu->left_pad = 0;
	menu->top_pad = 0;
	if(MENU_ORIENT_VERT == orient) { //TODO MENU_ORIENT_HORZ
		menu->item_width = (menu->wnd.rect.Right - menu->wnd.rect.Left + 1) - (menu->has_border ? 2 : 0);
		menu->cell_width = menu->item_width / cell_count;
		for (j = 0; j < cell_count; j++) {
			menu->cell_start[j] = j * menu->cell_width;
		}
		if(menu->has_headers) {
			menu->top_pad += 2;
			menu->border_bottom_index += 2;
		}
		if(menu->has_border) {
			menu->left_pad += 1;
			menu->top_pad += 1;
			menu->border_bottom_index += 1;
		}
		// не можем отобразить не помещающиеся в окно элементы
		if(item_count > menu->wnd.M - menu->border_bottom_index - 1) {
			item_count = menu->wnd.M - menu->border_bottom_index - 1;
		}
	}

	menu_fill_wnd(menu, item_count);

	// координаты углов консоли
//	menu->curspos.X = menu->wnd.rect.Left;///0;
//	menu->curspos.Y = menu->wnd.rect.Top;///1;

	menu->workWindowAttributes = //0x9E;
		BACKGROUND_RED | BACKGROUND_GREEN | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	menu->inactiveItemAttributes = 0x1F;
	menu->activeItemAttributes = 0xA0;

	// Инициализация элементов меню
	list1_init(&menu->items);
	menu->items.item_free = menu_item_release;

	//int next = 0
	switch(orient) {
	case MENU_ORIENT_HORZ:
		menu->item_width = (menu->wnd.rect.Right - menu->wnd.rect.Left + 1) / item_count;
		menu->cell_count = 1; // cell_count;
		//next = menu->wnd.rect.Left;
		for(i = 0; i < item_count; ++i) {
			int width;
			ITEM* item = (ITEM*)malloc(sizeof(ITEM));
			memset(item, 0x00, sizeof(ITEM));
			item->cb = item_defs[i].cb;
			//item->x = next + menu->left_pad; // menu->wnd.rect.Left + index * menu->item_width + menu->left_pad;
			//item->y = menu->wnd.rect.Top + menu->top_pad;
			if(item_defs[i].str[0]) {
				len = strlen(item_defs[i].str[0]);
				width = MAX(len, menu->item_width);
				item->str = (char*)malloc((width + 1) * sizeof(char));
				memset(item->str, MENU_WHITESPACE, width);
				CharToOemA(item_defs[i].str[0], item_defs[i].str[0]);
				if(len < menu->item_width) {
					int indent = (menu->item_width - len) / 2; // по центру
					memcpy(item->str + indent, item_defs[i].str[0], len);
				} else {
					memcpy(item->str, item_defs[i].str[0], menu->item_width - 1);
					item->str[menu->item_width - 1] = '_';
				}
				item->str[width] = '\0';

				//next += menu->item_width;
			}
			item->index = i;
			list1_push_back(&menu->items, item);
		} // for(item)
		break;
	case MENU_ORIENT_VERT:
		if(menu->has_headers) {
			if(menu->has_border) {
				// отрисовка горизонтальной линии отделения заголовка и её стыковочных линий слева/справа
				memset(menu->wnd.m[2], CHAR_BORDER_HORZ, menu->wnd.N * sizeof(char));
				menu->wnd.m[2][0] = CHAR_BORDER_LEFT_JOINT;
				menu->wnd.m[2][menu->wnd.N - 1] = CHAR_BORDER_RIGHT_JOINT;
			}
			menu->hdr = (char*)malloc((menu->item_width + 1) * sizeof(char));
			memset(menu->hdr, MENU_WHITESPACE, menu->item_width);
			menu->hdr[menu->item_width] = '\0';
			for (j = 0; j < cell_count; j++) {
				if(headers[j]) {

					len = strlen(headers[j]);
					//width = MAX(len, menu->cell_width);
					//CharToOemA(headers[j], headers[j]);
					if(len + left_indent < menu->cell_width) {
						memcpy(menu->hdr + menu->cell_start[j] + left_indent, headers[j], len);
					} else {
						memcpy(menu->hdr + menu->cell_start[j] + left_indent, headers[j], menu->cell_width - 1 - left_indent);
						menu->hdr[menu->cell_start[j] + menu->cell_width - 1 - left_indent] = '_';
					}
					if(menu->has_border && j < cell_count - 1) {
						// отрисовка вертикальных линий в заголовке
						menu->wnd.m[0][menu->cell_start[j] + menu->cell_width - 0] = CHAR_BORDER_TOP_JOINT;
						menu->hdr[menu->cell_start[j] + menu->cell_width - 1] = CHAR_BORDER_VERT;
						menu->wnd.m[2][menu->cell_start[j] + menu->cell_width - 0] = CHAR_BORDER_BOTTOM_JOINT;
					}
				}
			} // for(cell)
		} // if(has_headers)

		//next = menu->wnd.rect.Top;
		for(i = 0; i < item_count; ++i) {
			ITEM* item = (ITEM*)malloc(sizeof(ITEM));
			memset(item, 0x00, sizeof(ITEM));
			item->cb = item_defs[i].cb;
			//item->x = menu->wnd.rect.Left + menu->left_pad;
			//item->y = next + menu->top_pad; // menu->wnd.rect.Top + menu->top_pad + index
			item->str = (char*)malloc((menu->item_width + 1) * sizeof(char));
			memset(item->str, MENU_WHITESPACE, menu->item_width);
			item->str[menu->item_width] = '\0';
			for (j = 0; j < cell_count; j++) {
				if(item_defs[i].str[j]) {
					len = strlen(item_defs[i].str[j]);
					//width = MAX(len, menu->cell_width);
					// нехорошо менять каждый раз оригиналы CharToOemA(item_defs[i].str[j], item_defs[i].str[j]);
					if(len + left_indent < menu->cell_width) {
						memcpy(item->str + menu->cell_start[j] + left_indent, item_defs[i].str[j], len);
					} else {
						memcpy(item->str + menu->cell_start[j] + left_indent, item_defs[i].str[j], menu->cell_width - 1 - left_indent);
						item->str[menu->cell_start[j] + menu->cell_width - 1 - left_indent] = '_';
					}
					if(menu->has_border && j < cell_count - 1) {
						int top_shift;
						if(menu->has_headers) {
							top_shift = 2;
							menu->wnd.m[top_shift][menu->cell_start[j] + menu->cell_width - 0] = CHAR_BORDER_CROSS_JOINT;
						} else {
							top_shift = 0;
							menu->wnd.m[top_shift][menu->cell_start[j] + menu->cell_width - 0] = CHAR_BORDER_TOP_JOINT;
						}
						item->str[menu->cell_start[j] + menu->cell_width - 1] = CHAR_BORDER_VERT;
						menu->wnd.m[i + top_shift + 2][menu->cell_start[j] + menu->cell_width - 0] = CHAR_BORDER_BOTTOM_JOINT;
					}
				}
			} // for(cell)
			//портит псевдографику CharToOemA(item->str, item->str);
			item->index = i;
			list1_push_back(&menu->items, item);
			//next++;
		} // for(item)
		break;
	}

	list1_for_each(&menu->items, menu_calc_item_pos, menu);

	return 0;
}

void menu_item_release(void* ptr) {
	ITEM* item = (ITEM*)ptr;
	if(item->str)
		free(item->str);
	free(item);
}

void menu_del_curr(MENU* menu) {
	list1_erase_current(&menu->items);
}

void menu_clear(MENU* menu) {
	int i;

	list1_clear(&menu->items);

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

	list1_clear(&menu->hk_list);

	menu->hStdOut = INVALID_HANDLE_VALUE;
}

void menu_prev(MENU* menu) {
	int wrap = 1;
	itemMenu(menu, 0); // сделать неактивным пункт меню
	list1_curr_rev(&menu->items, wrap);
	itemMenu(menu, 1); // выделить активный пункт меню
	showCursor(menu, 0);
	if(menu->changed_cb) {
		menu->changed_cb(menu, MENU_CURR_REV, wrap);
	}
}

void menu_next(MENU* menu) {
	int wrap = 1;
	itemMenu(menu, 0); // сделать неактивным пункт меню
	list1_curr_fwd(&menu->items, wrap);
	itemMenu(menu, 1); // выделить активный пункт меню
	showCursor(menu, 0);
	if(menu->changed_cb) {
		menu->changed_cb(menu, MENU_CURR_FWD, wrap);
	}
}

int menu_calc_item_pos(void* data, int index, void* ptr) {
	ITEM* item = (ITEM*)data;
	MENU* menu = (MENU*)ptr;
	item->x = menu->wnd.rect.Left + menu->left_pad;
	item->y = menu->wnd.rect.Top + menu->top_pad;
	switch(menu->orient) {
	case MENU_ORIENT_HORZ:
		item->x += index * menu->item_width;
		break;
	case MENU_ORIENT_VERT:
		item->y += index;
		break;
	}
	return 1; // продолжить итерации по остальным элементам
}

int menu_draw_item(void* data, int index, void* ptr) {
	ITEM* item = (ITEM*)data;
	MENU* menu = (MENU*)ptr;
	menu_calc_item_pos(data, index, ptr);
	menu_gotoxy(menu, item->x, item->y);
	printf(item->str);
	return 1; // продолжить итерации по остальным элементам
}

int menu_draw(MENU* menu, int flags) {
	int result = 0, i, run = 1;
	if(flags & MENU_FLAG_WND) {
		// Устанавливаем цветовые параметры текста
		SetConsoleTextAttribute(menu->hStdOut, menu->workWindowAttributes);
		//system("CLS"); // установка атрибутов цвета рабочей области
		menu_cls(menu);
	}
	if(flags & MENU_FLAG_ITEMS) {
		// Номер текущего пункта меню
		SetConsoleTextAttribute(menu->hStdOut, menu->inactiveItemAttributes);
		// рисуем заголовок
		if(menu->hdr) {
			int top_pad = menu->has_border ? 1 : 0;
			menu_gotoxy(menu, menu->wnd.rect.Left + menu->left_pad, menu->wnd.rect.Top + top_pad);
			printf(menu->hdr);
		}
		// рисуем меню
		list1_for_each(&menu->items, menu_draw_item, menu);
		fflush(stdout);
	}
	if(flags & MENU_DRAW_SEL) {
		itemMenu(menu, 1); // выделить пункт меню
	}
	//fflush(stdin); // очистить буфер клавиатуры
	if(!(flags & (MENU_NAVIGATOR | MENU_HOTKEYS)))
		return 0; // не запускаем обработку сообщений
	while (run) {
		if (kbhit()) {
			menu->cb_retcode = 0;
			menu->last_key = _getch();
			if(flags & MENU_NAVIGATOR) {
				switch (menu->last_key) {
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
				case KEY_ENTER:
					// Возвращаем курсор из строки меню в прежнюю позицию
					///menu_gotoxy(menu, menu->curspos.X, menu->curspos.Y);
					// Установить цвет рабочих сообщений
					///SetConsoleTextAttribute(menu->hStdOut, menu->workWindowAttributes);
					///showCursor(menu, 1);
					// Вызываем обработчик пункта меню
					{
						ITEM* item = (ITEM*)list1_curr(&menu->items);
						menu->cb_retcode = item->cb(menu, item);
					}
					run = check_cb_retcode(menu);
					break;
				} // switch(iKey)
			}
			if(flags & MENU_HOTKEYS) {
				// обработка горячих клавиш
				list1_for_each(&menu->hk_list, menu_proc_hotkey, menu);
				run = check_cb_retcode(menu);
			}
		} // if(kbhit())
	} // while(run)
	if(menu->parent) {
		menu_draw(menu->parent, MENU_FLAG_WND | MENU_FLAG_ITEMS | MENU_DRAW_SEL);
	}
	return result;
}

void itemMenu(MENU* menu, int activate)
{
	ITEM* item = (ITEM*)list1_curr(&menu->items);
	WORD itemAttributes;
	if(!item)
		return;
	if (activate)
		itemAttributes = menu->activeItemAttributes;
	else
		itemAttributes = menu->inactiveItemAttributes;
	menu_gotoxy(menu, item->x, item->y);
	SetConsoleTextAttribute(menu->hStdOut, itemAttributes);
	printf(item->str);
}

void menu_cls(MENU* menu)
{
	int i, y;
	SetConsoleTextAttribute(menu->hStdOut, menu->workWindowAttributes);
	for (i = 0, y = menu->wnd.rect.Top; i < menu->wnd.M; i++, y++) {
		menu_gotoxy(menu, menu->wnd.rect.Left, y);
		printf(menu->wnd.m[i]);
	}
}

int menu_add_hotkey(MENU* menu, int code, ExecuteHotketCB cb) {
	HOT_KEY* hk = (HOT_KEY*)malloc(sizeof(HOT_KEY));
	memset(hk, 0x00, sizeof(HOT_KEY));
	hk->code = code;
	hk->cb = cb;
	list1_push_front(&menu->hk_list, hk);
	return menu->hk_list.count;
}

int menu_proc_hotkey(void* data, int index, void* ptr) {
	HOT_KEY* hk = (HOT_KEY*)data;
	MENU* menu = (MENU*)ptr;
	if(menu->last_key == hk->code) {
		menu->cb_retcode = hk->cb(menu);
		return 0; // не проверять остальные обработчики
	}
	return 1; // продолжить итерации по остальным элементам
}

void menu_gotoxy(MENU* menu, int x, int y)
{
	COORD cursorPos;
	cursorPos.X = x;
	cursorPos.Y = y;
	SetConsoleCursorPosition(menu->hStdOut, cursorPos);
	///SetConsoleCursorPosition(hStdOut, {x,y});
}

void saveCursorPosition(MENU* menu)
{
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;// информация о консольном окне в структуре csbInfo
	GetConsoleScreenBufferInfo(menu->hStdOut, &csbInfo);
///	menu->curspos = csbInfo.dwCursorPosition;
}

void showCursor(MENU* menu, int visible)
{
	CONSOLE_CURSOR_INFO ccInfo;
	ccInfo.bVisible = visible;
	ccInfo.dwSize = 20;
	SetConsoleCursorInfo(menu->hStdOut, &ccInfo);
}

int check_cb_retcode(MENU* menu) {
	if(-1 == menu->cb_retcode) {
		///menu_gotoxy(menu, 0, 0);
		///menu_cls(menu, WholeWindow);
		itemMenu(menu, 0); // сделать неактивным пункт меню
		return 0;
	} else if(0 == menu->cb_retcode) {
		saveCursorPosition(menu);
	}
	// очистить буфер клавиатуры
	fflush(stdin);
	// курсор в текущий пункт меню
	///menu_gotoxy(menu, menu->items[menu->current].x, menu->items[menu->current].y);
	// спрятать курсор
	showCursor(menu, 0);
	return 1;
}

