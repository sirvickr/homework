//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "menu.h"
#include "dict.h"

// верхнее меню
SMALL_RECT top_menu_rect = { 0, 0, 99, 3 };
const int top_item_count = 6; // количество пунктов меню
// положение (x,y), заголовок, указатель на функцию
ITEM_DEF top_menu_items[top_item_count] = {
#if 1
	{ { "Edit", "Cell[0][1]", 0 }, Edit },
	{ { "Search", "Cell[1][1]", 0 }, Search },
	{ { "Sort", "Cell[2][1]", 0 }, Sort },
	{ { "Save", "Cell[3][1]", 0 }, Save },
	{ { "Help", "Cell[4][1]", 0 }, Help },
	{ { "Exit", "Cell[5][1]", 0 }, Exit },
#else
	{ 1,  0, "Файл", File },
	{ 11, 0, "Действие", Do },
	{ 21, 0, "Очистить", Clear },
	{ 31, 0, "Справка", Help },
	{ 41, 0, "Выход", Exit },
#endif
};

// главное меню (таблица)
SMALL_RECT main_menu_rect = { 0, 5, 49, 19 };
const int main_item_count = 4; // количество пунктов меню
// положение (x,y), заголовок, указатель на функцию
ITEM_DEF main_menu_items[main_item_count] = {
	{ { "Hello", "Noun", "Privet", "5", 0 }, Edit },
	{ { "World", "Noun", "Mir", "5", 0 }, Edit },
	{ { "Go", "Verb", "Idti", "2", 0 }, Edit },
	{ { "Run", "Noun", "Beg", "10", 0 }, Edit },
};
const int main_column_count = 4;
const char* main_headers[main_column_count] = {
	"Word", "Part", "Trans", "Size"
};

int Run();

#pragma argsused
int main(int argc, char* argv[])
{
	return Run();
}

int Run() {
	MENU top_menu, main_menu;
	SMALL_RECT rect;
	//setlocale(LC_CTYPE, "rus"); // вызов функции настройки национальных параметров

	//SetConsoleTitle("Англо-русский словарь");
	SetConsoleTitle("Dictionary");

	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);// = INVALID_HANDLE_VALUE; // дескриптор консольного окна
	if(INVALID_HANDLE_VALUE == hstdout) {
		return -1;
	}
	// Получаем размеры консоли
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
	GetConsoleScreenBufferInfo(hstdout, &csbInfo);
	rect.Left = csbInfo.srWindow.Left + 1;
	rect.Right = csbInfo.srWindow.Right - 1;
	rect.Top = csbInfo.srWindow.Top + 1;
	rect.Bottom = rect.Top;

	menu_init(&top_menu, hstdout, top_menu_items, top_item_count, 2,
		MENU_ORIENT_HORZ, &rect, 0, NULL);
	menu_active_color(&top_menu, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_inactive_color(&top_menu, BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_draw(&top_menu, 0);

	#if 1
	rect.Top++;
	rect.Bottom = csbInfo.srWindow.Bottom - 1;
	menu_init(&main_menu, hstdout, main_menu_items, main_item_count, main_column_count,
		MENU_ORIENT_VERT, &rect, 1, main_headers);
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
