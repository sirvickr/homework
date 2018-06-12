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
}
//#define TEST_MAIN_TABLE
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
static const int top_item_count = 6; // количество пунктов меню
// положение (x,y), заголовок, указатель на функцию
ITEM_DEF top_menu_items[top_item_count] = {
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

static CONSOLE_SCREEN_BUFFER_INFO csbInfo;

static MENU top_menu;
static MENU *ptable = NULL;

int Run();
// Отображение текста из файла на всплывающем окне
int HelpFromFile(HANDLE hStdOut, const char* file_name, const char* title, SMALL_RECT rect);
//
int ShowMenu(HANDLE hStdOut, ITEM_DEF* menu_items, int item_count, const char* title, SMALL_RECT rect, int flags);
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

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);// = INVALID_HANDLE_VALUE; // дескриптор консольного окна
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

	#ifdef TEST_MAIN_TABLE
	// главное меню (таблица)
	const int main_item_count = 4; // количество пунктов меню
	// положение (x,y), заголовок, указатель на функцию
	ITEM_DEF main_menu_items[main_item_count] = {
		{ { "Hello", "Noun", "Privet", "5", 0 }, Edit },
		{ { "World", "Noun", "Mir", "5", 0 }, Edit },
		{ { "Go", "Verb", "Idti", "2", 0 }, Edit },
		{ { "Run", "Noun", "Beg", "10", 0 }, Edit },
	};
	#else
	// Загрузка словаря из файла
	if(-1 == dict_load(dict_file_name))
		return -1;

	ITEM_DEF* main_menu_items = (ITEM_DEF*)malloc(dict.count * sizeof(ITEM_DEF));
	memset(main_menu_items, 0x00, dict.count * sizeof(ITEM_DEF));
	for(i = 0; i < dict.count; i++) {
		for(int fld = 0; fld < DICT_FLD_CNT; fld++)
			main_menu_items[i].str[fld] = (char*)malloc(MAX_STRING * sizeof(char));
		main_menu_items[i].str[DICT_FLD_CNT] = (char*)malloc(32 * sizeof(char));
	}
	#endif

	rect.Top++;
	rect.Bottom = csbInfo.srWindow.Bottom - 1;

	while (redraw_main) {
		// перекидываем словарь в таблицу
		list1_for_each(&dict, dict_entry_display, main_menu_items);

		redraw_main = 0; // любой callback может запланировать перерисовку
		if(ptable) {
			menu_clear(ptable);
			free(ptable);
		}
		ptable = (MENU*)malloc(sizeof(MENU));
		menu_init(ptable, NULL, hstdout, main_menu_items, dict.count, main_column_count,
			MENU_ORIENT_VERT, &rect, 1, main_headers);
		ptable->user_tag = MENU_TAG_MAIN;
		menu_add_hotkey(ptable, KEY_F1, F1);
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

	#ifndef TEST_MAIN_TABLE
	for(i = 0; i < dict.count; i++)
		for(int fld = 0; fld < DICT_FLD_CNT + 1; fld++)
			if(main_menu_items[i].str[fld])
				free(main_menu_items[i].str[fld]);
	free(main_menu_items);
	#endif


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
		result = -1;
	}
	return result;
}
//---------------------------------------------------------------------------
// Функции меню
//---------------------------------------------------------------------------
// Функция меню <Изменить>
int Edit(MENU* menu, ITEM*) {
	SMALL_RECT wndRect = { 5, 5, 19, 14 }; // [5][10]
	SHORT width = wndRect.Right - wndRect.Left + 1;
	SHORT height = wndRect.Bottom - wndRect.Top + 1;

	SMALL_RECT srctReadRect;
	SMALL_RECT srctWriteRect;
	COORD coordBufSize;
	COORD coordBufCoord;
	BOOL fSuccess;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	//CHAR_INFO chiBuffer[160]; 		// [2][80];
	CHAR_INFO* chiBuffer = NULL;
	if (hStdout == INVALID_HANDLE_VALUE)
		return -1;
	// Установим прямоугольник источника
	srctReadRect.Top = wndRect.Top;       ///0;
	srctReadRect.Left = wndRect.Left;     ///0;
	srctReadRect.Bottom = wndRect.Bottom; ///1;
	srctReadRect.Right = wndRect.Right;   ///79;
	// Размер временного буфера равен 2 x 80
	coordBufSize.Y = height;  ///2;
	coordBufSize.X = width;  ///80;
	// Верхняя левая ячейка назначения
	coordBufCoord.X = 0; ///0;               wndRect.Left
	coordBufCoord.Y = 0; ///0;               wndRect.Top
	chiBuffer = (CHAR_INFO*)malloc(height * width * sizeof(CHAR_INFO));
	if(!chiBuffer) {
		return -1;
	}
	// Скопируем блок из экранного буфера во временный буфер
	fSuccess = ReadConsoleOutput(
	   hStdout,        	// экранный буфер, из которого читаем
	   chiBuffer,      	// буфер, в который копируем
	   coordBufSize,   	// размеры chiBuffer: колонки/строки
	   coordBufCoord,  	// верхняя левая ячейка назначения в chiBuffer
	   &srctReadRect); 	// источник - прямоугольник экранного буфера
	if (! fSuccess) {
		//MyErrorExit("ReadConsoleOutput");
		free(chiBuffer);
		return -1;
	}

	{
		int wndCellCount = height * width;
		CHAR_INFO* wndBuffer = (CHAR_INFO*)malloc(wndCellCount * sizeof(CHAR_INFO));
		for(int i = 0; i < wndCellCount; ++i) {
			wndBuffer[i].Char.AsciiChar = 'x';
			wndBuffer[i].Attributes = BACKGROUND_INTENSITY | BACKGROUND_GREEN |
				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
				;
		}

		fSuccess = WriteConsoleOutput(
			hStdout, 	        // экранный буфер, в который пишем
			wndBuffer,        	// буфер, из которого копируем
			coordBufSize,     	// размеры chiBuffer: колонки/строки
			coordBufCoord,    	// исходная верхняя левая ячейка в chiBuffer
			&srctReadRect);  	// приёмник - прямоугольник экранного буфера
		Sleep(1000);

		/*gotoxy(menu, wndRect.Left, wndRect.Top);
		printf("EXIT");
		Sleep(1000);*/

		free(wndBuffer);
	}

	// Устанавливаем прямоугольник назначения
	srctWriteRect.Top = wndRect.Top; ///10;
	srctWriteRect.Left = wndRect.Left; ///0;
	srctWriteRect.Bottom = wndRect.Bottom; ///11;
	srctWriteRect.Right = wndRect.Right; ///79;
	//
	fSuccess = WriteConsoleOutput(
		hStdout, 	        // экранный буфер, в который пишем
		chiBuffer,        	// буфер, из которого копируем
		coordBufSize,     	// размеры chiBuffer: колонки/строки
		coordBufCoord,    	// исходная верхняя левая ячейка в chiBuffer
		&srctWriteRect);  	// приёмник - прямоугольник экранного буфера
	if (! fSuccess) {
		//MyErrorExit("WriteConsoleOutput");
		free(chiBuffer);
		return -1;
	}
	//Sleep(1000);
	/*int resp;
	//cout << "Вы уверены, что хотите выйти з программы (y/n)?";
	printf("Quit? (y/n)?");
	resp = getchar();
	if (resp == 'y' || resp == 'Y') {
		return -1;
	} */
	free(chiBuffer);
	return 0;
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
// Функция меню <Поиск>
int Search(MENU* menu, ITEM*) {
	//printf("Search\n");
	return 0;
}
// Функция подменю <Сортировка>
int SortItem(MENU* pm, ITEM* item) {
	list1_sort(&dict, dict_entry_compare, &item->index);
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
	#if 0
	// количество ячеек меню (столбцов)
	const int column_count = 1;
	char* headers[column_count];
	headers[0] = (char*)malloc(MAX_MENU_HDR * sizeof(char));
	strcpy(headers[0], title);
	CharToOemA(headers[0], headers[0]);
	for(i = 0; i < main_column_count; i++)
		main_headers[i] = (char*)malloc(MAX_MENU_HDR * sizeof(char));
	#endif
	ITEM_DEF* menu_items = (ITEM_DEF*)malloc(item_count * sizeof(ITEM_DEF));
	memset(menu_items, 0x00, item_count * sizeof(ITEM_DEF));
	for(i = 0; i < item_count; i++) {
		menu_items[i].str[0] = (char*)malloc(max_len * sizeof(char));
		strncpy(menu_items[i].str[0], main_headers[i], max_len);
		menu_items[i].str[0][max_len] = '\0';
		menu_items[i].cb = SortItem;
	}
	ShowMenu(pm->hStdOut, menu_items, item_count, NULL, rect, MENU_FULL_FLAGS);
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
	rect.Left = csbInfo.srWindow.Left + 2;
	rect.Right = csbInfo.srWindow.Right - 2;
	rect.Top = csbInfo.srWindow.Top + 3;
	rect.Bottom = csbInfo.srWindow.Bottom - 2;
	switch(pm->user_tag) {
	case MENU_TAG_TOP:
	case MENU_TAG_MAIN:
		result = HelpFromFile(pm->hStdOut, "help.txt", "Справка о программе", rect);
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

int ShowMenu(HANDLE hStdOut, ITEM_DEF* menu_items, int item_count, const char* title, SMALL_RECT rect, int flags) {
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
	menu.user_tag = MENU_TAG_HELP_GLOBAL;
	menu_active_color(&menu, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_inactive_color(&menu, BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_add_hotkey(&menu, KEY_ESC, DefaultESC);
	menu_add_hotkey(&menu, KEY_F1, DefaultESC);
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
	return ShowMenu(hStdOut, menu_items, item_count, title, rect, MENU_FLAG_WND | MENU_FLAG_ITEMS | MENU_HOTKEYS);
}





