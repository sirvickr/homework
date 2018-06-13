//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "main.h"
#include "dict.h"
#include "codes.h"
extern "C" {
#include "list.h"
#include "input.h"
}
//#define TEST_LIST1

#define MAX_STRING 256

// пользовательские метки, чтобы различать меню в функциях обратного вызова
#define MENU_TAG_DEFAULT     0
#define MENU_TAG_TOP         1
#define MENU_TAG_MAIN        2
#define MENU_TAG_EXIT        3
#define MENU_TAG_CONFIRM     4
#define MENU_TAG_HELP_GLOBAL 5
#define MENU_TAG_SEARCH      6
#define MENU_TAG_SORT        7
// верхнее меню
static const int top_item_count = 7; // количество пунктов меню
// положение (x,y), заголовок, указатель на функцию
ITEM_DEF top_menu_items[top_item_count] = {
	{ { "Добавить", 0 }, Add },
	{ { "Изменить", 0 }, Edit },
	{ { "Поиск", 0 }, Search },
	{ { "Сортировка", 0 }, Sort },
	{ { "Сохранить", 0 }, Save },
	{ { "Помощь", 0 }, Help },
	{ { "Выйти", 0 }, Exit },
};

// количество ячеек меню (столбцов)
static const int main_column_count = 4;
const int exit_column_count = 1;
#if 1
static char* main_headers[main_column_count];
static char* exit_headers[exit_column_count];
#else
static char* main_headers[main_column_count] = { "Слово", "Часть речи", "Перевод", "Количество букв" };
static char* exit_headers[exit_column_count] = { "Сохранить данные?" };
#endif

// глобальный код завершения для функций обратного вызова
int exit_code = 0; // не нулевое значение считается обязательным
int redraw_main = 1;
int data_modified = 0;
int save_data = 0;
int exit_canceled = 0;
int initial_table_index = 0; // индекс для инициализации обновлённого меню

static CONSOLE_SCREEN_BUFFER_INFO csbInfo;

static MENU top_menu;
static MENU *ptable = NULL;

int Run();
// Отображение текста из файла на всплывающем окне
int HelpFromFile(HANDLE hStdOut, const char* file_name, const char* title, SMALL_RECT rect);
//
int ShowMenu(HANDLE hStdOut, ITEM_DEF* menu_items, int item_count, const char* title, SMALL_RECT rect, int flags, int user_tag, ExecuteHotketCB f1CB);
//
ITEM_DEF* MenuItemsFromFile(const char* file_name, int max_count, int max_len, int* pcount);

#pragma argsused
int main(int argc, char* argv[])
{
	return Run();
}

int ExitYes(MENU* menu, ITEM*) {
	save_data = 1;
	return -1;
}

int ExitNo(MENU* menu, ITEM*) {
	save_data = 0;
	return -1;
}

int ExitCancel(MENU* menu, ITEM*) {
	exit_canceled = 1;
	return -1;
}
// количество пунктов меню
static const int exit_item_count = 3;
static ITEM_DEF exit_menu_items[exit_item_count] = {
	{ { "Да", 0 }, ExitYes },
	{ { "Нет", 0 }, ExitNo },
	{ { "Отмена", 0 }, ExitCancel },
};

// Обработчик клавиши ESC по умолчанию выходит из цикла сообщений текущего меню
int DefaultESC(MENU* menu) {
	return -1;
}

int ESC(MENU* menu) {
	return Exit(menu, NULL);
}

int F1(MENU* menu) {
	Help(menu, NULL);
	return 0;
}

int F3(MENU* menu) {
	Search(menu, NULL);
	if(exit_code)
		return exit_code;
	return 0;
}

int F9(MENU* menu) {
	menu_draw(&top_menu, MENU_FULL_FLAGS);
	if(exit_code)
		return exit_code;
	return 0;
}

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

int dict_entry_display(void* data, int index, void* param) {
	DICT_ENTRY* entry = (DICT_ENTRY*)data;
	ITEM_DEF* main_menu_items = (ITEM_DEF*)param;
	for(int fld = 0; fld < DICT_FLD_CNT; fld++)
		CharToOemA(entry->field[fld], main_menu_items[index].str[fld]);
	itoa(strlen(entry->field[0]), main_menu_items[index].str[DICT_FLD_CNT], 10);
	main_menu_items[index].cb = Edit;
	return 1; // продолжить итерации по остальным элементам
}
//
int dict_entry_find(void* data, void* param) {
	DICT_ENTRY* entry = (DICT_ENTRY*)data;
	const char* str = (const char*)param;
	return strcmp(str, entry->field[0]) ? 0 : 1;
}

#ifdef TEST_LIST1
void dict_entry_release(void* data) {
	int i;
	if(data) {
		DICT_ENTRY* entry = (DICT_ENTRY*)data;
		for(i = 0; i < DICT_FLD_CNT; i++)
			if(entry->field[i])
				free(entry->field[i]);
		free(entry);
	}
}

void dict_entry_print(void* data, int index, void*) {
	DICT_ENTRY* entry = (DICT_ENTRY*)data;
	printf("%d\t%s %s %s\n", index, entry->field[0], entry->field[1], entry->field[2]);
}
#endif

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
	return strlen(a->field[0]) < strlen(b->field[0]) < 0 ? 1 : 0;
}

int Run() {
#ifdef TEST_LIST1
	DICT_ENTRY* entry;
	LIST1 list;
	list1_init(&list);
	list.item_free = dict_entry_release;

	if(entry = dict_new("Hello", "Noun", "1"))
		list1_push_front(&list, entry);
	if(entry = dict_new("World", "Noun", "2"))
		list1_push_front(&list, entry);
	if(entry = dict_new("Go", "Verb", "3"))
		list1_push_front(&list, entry);
	if(entry = dict_new("Run", "Noun", "4"))
		list1_push_back(&list, entry);
	if(entry = dict_new("Star", "Noun", "5"))
		list1_push_back(&list, entry);

	list1_for_each(&list, dict_entry_print, NULL);

	list1_clear(&list);
	getchar();
	return 0;
#endif
	int i;
	ITEM_DEF* main_menu_items = NULL;
	int main_menu_items_count = 0;
	// Инициализация заголовков глобальных меню
	for(i = 0; i < main_column_count; i++)
		main_headers[i] = (char*)malloc(MAX_MENU_HDR * sizeof(char));
	CharToOemA("Слово", main_headers[0]);
	CharToOemA("Часть речи", main_headers[1]);
	CharToOemA("Перевод", main_headers[2]);
	CharToOemA("Количество букв", main_headers[3]);

	for(i = 0; i < exit_column_count; i++)
		exit_headers[i] = (char*)malloc(MAX_MENU_HDR * sizeof(char));
	CharToOemA("Сохранить изменения?", exit_headers[0]);

	for(i = 0; i < exit_item_count; i++)
		CharToOemA(exit_menu_items[i].str[0], exit_menu_items[i].str[0]);
	/*
	//CharToOemA(text, text);
	system("chcp");
	//SetConsoleOutputCP(1251);
	//SetConsoleCP(1251);
	//setlocale(LC_CTYPE, "rus");
	//setlocale(LC_ALL,"russian");
	//setlocale(LC_ALL, "");
	//system("chcp 1251");
	system("chcp");
	//system("chcp 65001"); // utf8
	//system("chcp");
	*/
	/*#if 1
	if(-1 == dict_add(dict_new("Hello", "Noun", "Привет")))
		return -1;
	if(-1 == dict_add(dict_new("World", "Noun", "Мир")))
		return -1;
	if(-1 == dict_add(dict_new("Go", "Verb", "Идти")))
		return -1;
	if(-1 == dict_add(dict_new("Run", "Noun", "Бежать")))
		return -1;
	if(-1 == dict_add(dict_new("Star", "Noun", "Звезда")))
		return -1;
	if(-1 == dict_save(dict_file_name))
		return -1;
	list1_clear(&dict);
	getchar();
	#else
	if(-1 == dict_load(dict_file_name))
		return -1;
	{
		DICT_ENTRY* curr = dict_head;
		while(curr) {
			printf("%s:", curr->field[2]);
			char* pc = curr->field[2];
			while(*pc != 0) {
				printf(" %d", (int)*pc);
				pc++;
			}
			printf(" \n");
			curr = curr->next;
		}
		getchar();
	}
	#endif
	return 0;*/

	SMALL_RECT rect;
	char* title = "Англо-русский словарь";
	CharToOemA(title, title);
	SetConsoleTitle(title);

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE); // дескриптор консольного окна
	if(INVALID_HANDLE_VALUE == hstdout) {
		return -1;
	}
	// Получаем размеры консоли
	GetConsoleScreenBufferInfo(hstdout, &csbInfo);
	rect.Left = csbInfo.srWindow.Left + 1;
	rect.Right = csbInfo.srWindow.Right - 1;
	rect.Top = csbInfo.srWindow.Top + 1;
	rect.Bottom = rect.Top;

	menu_init(&top_menu, NULL, hstdout, top_menu_items, top_item_count, 1,
		MENU_ORIENT_HORZ, &rect, 0, NULL);
	top_menu.user_tag = MENU_TAG_TOP;
	menu_active_color(&top_menu, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_inactive_color(&top_menu, BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_add_hotkey(&top_menu, KEY_ESC, DefaultESC);
	menu_draw(&top_menu, MENU_FLAG_WND | MENU_FLAG_ITEMS);

#if 1

	// Загрузка словаря из файла
	if(-1 == dict_load(dict_file_name)) {
		return -1;
	}

	rect.Top++;
	rect.Bottom = csbInfo.srWindow.Bottom - 1;

	main_menu_items_count = dict.count;

	while (redraw_main) {
		redraw_main = 0; // любой callback может запланировать перерисовку
		//
		if(main_menu_items) {
			for(i = 0; i < main_menu_items_count; i++)
				for(int fld = 0; fld < DICT_FLD_CNT + 1; fld++)
					if(main_menu_items[i].str[fld])
						free(main_menu_items[i].str[fld]);
			free(main_menu_items);
			main_menu_items = NULL;
		}
		main_menu_items_count = dict.count; // словарь мог быть изменён
		main_menu_items = (ITEM_DEF*)malloc(main_menu_items_count * sizeof(ITEM_DEF));
		memset(main_menu_items, 0x00, main_menu_items_count * sizeof(ITEM_DEF));
		for(i = 0; i < main_menu_items_count; i++) {
			for(int fld = 0; fld < DICT_FLD_CNT; fld++)
				main_menu_items[i].str[fld] = (char*)malloc(MAX_STRING * sizeof(char));
			main_menu_items[i].str[DICT_FLD_CNT] = (char*)malloc(32 * sizeof(char));
		}
		//
		if(ptable) {
			menu_clear(ptable);
			free(ptable);
		}
		ptable = (MENU*)malloc(sizeof(MENU));
		// перекидываем словарь в таблицу
		list1_for_each(&dict, dict_entry_display, main_menu_items);
		// создаём элементы меню
		menu_init(ptable, NULL, hstdout, main_menu_items, main_menu_items_count, main_column_count,
			MENU_ORIENT_VERT, &rect, 1, main_headers);
		// устанавливаем текущий индекс
		list1_set_current_index(&ptable->items, initial_table_index);
		initial_table_index = 0;
		// продолжаем настройку меню
		ptable->user_tag = MENU_TAG_MAIN;
		menu_add_hotkey(ptable, KEY_F1, F1);
		menu_add_hotkey(ptable, KEY_F3, F3);
		menu_add_hotkey(ptable, KEY_F9, F9);
		menu_add_hotkey(ptable, KEY_DEL, Delete);
		menu_add_hotkey(ptable, KEY_ESC, ESC);
		ptable->changed_cb = curr_menu_changed;
		menu_active_color(ptable,
			BACKGROUND_BLUE | BACKGROUND_GREEN
			| FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		);
		menu_inactive_color(ptable,
			BACKGROUND_INTENSITY | BACKGROUND_BLUE
			| FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		);
		// блокирующий вызов: основной цикл обработки сообщений главной таблицы
		menu_draw(ptable, MENU_FULL_FLAGS);
		// возвожно, вышли по глобальному коду завершения, сбрасываем его
		exit_code = 0;

	}
	menu_clear(ptable);
	free(ptable);

	for(i = 0; i < main_menu_items_count; i++)
		for(int fld = 0; fld < DICT_FLD_CNT + 1; fld++)
			if(main_menu_items[i].str[fld])
				free(main_menu_items[i].str[fld]);
	free(main_menu_items);

#endif

	menu_clear(&top_menu);
/*#if 1
	//получение дескриптора стандартного устройства вывода
	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle("Demo Console Cls"); // Заголовок консоли
	printf("Hello\n");
	system("pause"); // Наблюдаем текст в окне
	SetConsoleTextAttribute(wHnd,
		BACKGROUND_RED |
		BACKGROUND_GREEN |
		BACKGROUND_BLUE |
		FOREGROUND_RED);
	mycls(wHnd); // очищаем окно
	int iKey = 67;
	// Цикл до тех пор пока не нажата клавиша ESC
	while (iKey != 27) {
		if (kbhit())
			iKey = _getch();
	}

#else
	clrscr();
	//printf("Hit any character key when ready\n");
	//while (!kbhit()) {
	//	//sleep(1);
	//}
	//printf("\nThe key pressed was (%c)\n", _getch());
	//window(20,10,42,42);
	window(8, 5, 25, 15);
	textcolor(3);
	textbackground(6);
	cprintf("window at (20,10,42,42)\n");
	//printf("Hit any character key when ready\n");
#endif*/
	//getc(stdin);
	//getchar();
	for(i = 0; i < main_column_count; i++)
		if(main_headers[i])
			free(main_headers[i]);
	return 0;
}
// Функция меню <Выход>. Заполняется кодом пользователя
int Exit(MENU* menu, ITEM* item) {
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
	int result;
	if(exit_canceled) {
		exit_canceled = 0;
		result = 0;
	} else {
		exit_code = -1;
		result = -1;
	}
	return result;
}
//---------------------------------------------------------------------------
// Функции меню
//---------------------------------------------------------------------------
// Функция меню <Добавить>
int Add(MENU* pm, ITEM*) {
	int i;
	SMALL_RECT wndRect = { 10, 11, 49, 15 };
	int max_width = (wndRect.Right - wndRect.Left - 1) / 2;
	DICT_ENTRY* entry;

	char*** contents;
	int row_count = DICT_FLD_CNT;
	contents = (char***)malloc(row_count * sizeof(char**));
	memset(contents, 0x00, row_count * sizeof(char**));
	for(i = 0; i < row_count; ++i) {
		contents[i] = (char**)malloc(COLUMNS * sizeof(char*));
		// надпись
		contents[i][TITLE] = (char*)malloc((MAX_TITLE + 1) * sizeof(char));
		strcpy(contents[i][TITLE], main_headers[i]);
		//CharToOemA(main_headers[i], contents[i][TITLE]);
		// буфер ввода
		contents[i][BUFFER] = (char*)malloc((max_width + 1) * sizeof(char));
		memset(contents[i][BUFFER], ' ', max_width * sizeof(char));
		//strcpy(contents[i][BUFFER], entry->field[i]);
		//CharToOemA(entry->field[i], contents[i][BUFFER]);
		contents[i][BUFFER][0] = '\0';
		contents[i][BUFFER][max_width] = '\0';
	}

	InputBox box;
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

	/*for(i = 0; i < row_count; ++i) {
		//strcpy(entry->field[i], contents[i][BUFFER]);
		OemToCharA(contents[i][BUFFER], entry->field[i]);
	}*/
	list1_push_back(&dict,
		dict_entry_new(contents[0][BUFFER], contents[1][BUFFER], contents[2][BUFFER])
	);

	/*box_clear(&box);

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
	contents = NULL; */

	data_modified = 1;
	redraw_main = 1;
	exit_code = -1;
	return exit_code;
}

// Функция меню <Изменить>
int Edit(MENU* pm, ITEM*) {
	int i;
	SMALL_RECT wndRect = { 10, 11, 49, 15 };
	int max_width = (wndRect.Right - wndRect.Left - 1) / 2;
	DICT_ENTRY* entry = (DICT_ENTRY*)list1_curr(&dict);

	char*** contents;
	int row_count = DICT_FLD_CNT;
	contents = (char***)malloc(row_count * sizeof(char**));
	memset(contents, 0x00, row_count * sizeof(char**));
	for(i = 0; i < row_count; ++i) {
		contents[i] = (char**)malloc(COLUMNS * sizeof(char*));
		// надпись
		contents[i][TITLE] = (char*)malloc((MAX_TITLE + 1) * sizeof(char));
		strcpy(contents[i][TITLE], main_headers[i]);
		//CharToOemA(main_headers[i], contents[i][TITLE]);
		// буфер ввода
		contents[i][BUFFER] = (char*)malloc((max_width + 1) * sizeof(char));
		memset(contents[i][BUFFER], ' ', max_width * sizeof(char));
		//strcpy(contents[i][BUFFER], entry->field[i]);
		CharToOemA(entry->field[i], contents[i][BUFFER]);
		contents[i][BUFFER][max_width] = '\0';
	}

	InputBox box;
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

	for(i = 0; i < row_count; ++i) {
		//strcpy(entry->field[i], contents[i][BUFFER]);
		OemToCharA(contents[i][BUFFER], entry->field[i]);
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

	data_modified = 1;
	redraw_main = 1;
	exit_code = -1;
	return exit_code;
}

// Функция меню <Удалить>
int Delete(MENU* menu) {
	list1_erase_current(&dict);
	menu_del_curr(menu);
	menu_fill_wnd(menu, 0);
	menu_cls(menu);
	menu_draw(menu, MENU_FLAG_WND | MENU_FLAG_ITEMS | MENU_DRAW_SEL);
	data_modified = 1;
	return 0;
}

int accept_input(char ch) {
	if(isalnum(ch))
		return 1;
	if(ispunct(ch) || ' ' == ch)
		return 1;
	if((-96 <= ch && ch <= -81) || (-32 <= ch && ch <= -17) || -15 == ch)
		return 1;
	return 0;
}
// Функция меню <Поиск>
int Search(MENU* pm, ITEM*) {
	int i;
	SMALL_RECT wndRect = { 3, 11,  29, 13 };
	int max_width = (wndRect.Right - wndRect.Left - 1) / 2;

	char*** contents;
	int row_count = 1;
	contents = (char***)malloc(row_count * sizeof(char**));
	memset(contents, 0x00, row_count * sizeof(char**));
	for(i = 0; i < row_count; ++i) {
		contents[i] = (char**)malloc(COLUMNS * sizeof(char*));
		// надпись
		contents[i][TITLE] = (char*)malloc((MAX_TITLE + 1) * sizeof(char));
		//strcpy(contents[i][TITLE], "Search:");
		CharToOemA("Слово:  ", contents[i][TITLE]);
		// буфер ввода
		contents[i][BUFFER] = (char*)malloc((max_width + 1) * sizeof(char));
		memset(contents[i][BUFFER], ' ', max_width * sizeof(char));
		contents[i][BUFFER][0] = '\0';
		contents[i][BUFFER][max_width] = '\0';
	}

	InputBox box;
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

	int row = 0;

	int index = list1_search(&dict, dict_entry_find, contents[row][BUFFER]);
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
// Функция подменю <Сортировка>
int SortItem(MENU* pm, ITEM* item) {
	list1_sort(&dict, dict_entry_compare, &item->index);
	initial_table_index = list1_get_current_index(&dict);
	redraw_main = 1;
	exit_code = -1;
	return exit_code;
}
// Функция меню <Сортировка>
int Sort(MENU* pm, ITEM* item) {
	int i;
	SMALL_RECT rect;
	int item_count = main_column_count;
	int slen;
	int menu_width = 20;
	int menu_height = item_count + 2;
	rect.Left = item->x;
	rect.Right = item->x + menu_width - 1;
	rect.Top = csbInfo.srWindow.Top + 2;
	rect.Bottom = rect.Top + menu_height - 1;
	int max_len = menu_width - 2; // 2 верт. линии по краям
	ITEM_DEF* menu_items = (ITEM_DEF*)malloc(item_count * sizeof(ITEM_DEF));
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
// Функция меню <Сохранить>
int Save(MENU* menu, ITEM*) {
	save_data = 0;
	data_modified = 0;
	dict_save(dict_file_name);
	return -1;
}
// Функция меню <Помощь>. Заполняется кодом пользователя
int Help(MENU* pm, ITEM*) {
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
		result = HelpFromFile(pm->hStdOut, "help.txt", "Справка о программе", rect);
		break;
	case MENU_TAG_SORT:
		width = 55;
		height = 6;
		rect.Left = (csbInfo.srWindow.Right - csbInfo.srWindow.Left - width) / 2;
		rect.Right = rect.Left + width - 1;
		rect.Top = (csbInfo.srWindow.Bottom - csbInfo.srWindow.Top - height) / 2;
		rect.Bottom = rect.Top + height - 1;
		result = HelpFromFile(pm->hStdOut, "sort.txt", "Справка: Функция сортировки", rect);
		break;
	}
	return result;
}
// Вспомогательные функции

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
			// удаляем предпоследний символ - '\n'
			menu_items[i].str[0][slen > 1 ? slen - 1 : 0] = '\0';
		} else {
			menu_items[i].str[0][0] = '\0';
		}
		if(++i == max_count) {
			break; // достигнут лимит строк
		}
	}
	fclose(file);
	item_count = i;
	if(pcount)
		*pcount = item_count;
	return menu_items;
}

int ShowMenu(HANDLE hStdOut, ITEM_DEF* menu_items, int item_count, const char* title, SMALL_RECT rect, int flags, int user_tag, ExecuteHotketCB f1CB) {
	int i;
	MENU menu;
	// количество ячеек меню (столбцов)
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

int HelpFromFile(HANDLE hStdOut, const char* file_name, const char* title, SMALL_RECT rect) {
	int i;
	//int menu_width = rect.Right - rect.Left + 1;
	int menu_height = rect.Bottom - rect.Top + 1;
	int item_count = 0;
	// 4 = заголовок + 3 гориз. линиии
	ITEM_DEF* menu_items = MenuItemsFromFile(file_name, menu_height - 4, 256, &item_count);
	if(!menu_items) {
		return -1;
	}
	return ShowMenu(hStdOut, menu_items, item_count, title, rect, MENU_FLAG_WND | MENU_FLAG_ITEMS | MENU_HOTKEYS, MENU_TAG_HELP_GLOBAL, DefaultESC);
}





