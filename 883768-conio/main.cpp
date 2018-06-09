//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "menu.h"
#include "dict.h"

// верхнее меню
//SMALL_RECT top_menu_rect = { 0, 0, 99, 2 };
SMALL_RECT top_menu_rect = { 0, 0, 99, 0 };
const int top_item_count = 6; // количество пунктов меню
// положение (x,y), заголовок, указатель на функцию
ITEM_DEF top_menu_items[top_item_count] = {
#if 1
	{ { "Edit", 0 }, 1, Edit },
	{ { "Search", 0 }, 1, Search },
	{ { "Sort", 0 }, 1, Sort },
	{ { "Save", 0 }, 1, Save },
	{ { "Help", 0 }, 1, Help },
	{ { "Exit", 0 }, 1, Exit },
#else
	{ 1,  0, "Файл", File },
	{ 11, 0, "Действие", Do },
	{ 21, 0, "Очистить", Clear },
	{ 31, 0, "Справка", Help },
	{ 41, 0, "Выход", Exit },
#endif
};

// главное меню (таблица)
//SMALL_RECT main_menu_rect = { 0, 3, 99, 19 };
SMALL_RECT main_menu_rect = { 0, 1, 99, 19 };
const int main_item_count = 4; // количество пунктов меню
// положение (x,y), заголовок, указатель на функцию
ITEM_DEF main_menu_items[main_item_count] = {
	{ { "Hello", "Noun", "Privet", "5", 0 }, 4, Edit },
	{ { "World", "Noun", "Mir", "5", 0 }, 4, Edit },
	{ { "Go", "Verb", "Idti", "2", 0 }, 4, Edit },
	{ { "Dictionary", "Noun", "Slovar", "10", 0 }, 4, Edit },
};

int Run();

#pragma argsused
int main(int argc, char* argv[])
{
	return Run();
}

int Run() {
	MENU top_menu, main_menu;
	//setlocale(LC_CTYPE, "rus"); // вызов функции настройки национальных параметров

	//SetConsoleTitle("Англо-русский словарь");
	SetConsoleTitle("Dictionary");

	memset(&top_menu, 0x00, sizeof(top_menu));
	menu_init(&top_menu, top_menu_items, top_item_count, MENU_ORIENT_HORZ, &top_menu_rect);
	menu_active_color(&top_menu, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_inactive_color(&top_menu, BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_draw(&top_menu, 0);

	#if 1
	memset(&main_menu, 0x00, sizeof(main_menu));
	menu_init(&main_menu, main_menu_items, main_item_count, MENU_ORIENT_VERT, &main_menu_rect);
	menu_active_color(&main_menu,
		BACKGROUND_BLUE | BACKGROUND_GREEN
		| FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
	);
	menu_inactive_color(&main_menu,
		BACKGROUND_INTENSITY | BACKGROUND_BLUE
		| FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
	);
	menu_draw(&main_menu, 1);

	menu_clear(&main_menu);
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
	return 0;
}
