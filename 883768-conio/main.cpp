//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "menu.h"
#include "dict.h"

//#define TEST_MAIN_TABLE

// верхнее меню
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

int Run();

#pragma argsused
int main(int argc, char* argv[])
{
	return Run();
}

int Run() {
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
	if(-1 == dict_new("Hello", "Noun", "Привет"))
		return -1;
	if(-1 == dict_new("World", "Noun", "Мир"))
		return -1;
	if(-1 == dict_new("Go", "Verb", "Идти"))
		return -1;
	if(-1 == dict_new("Run", "Noun", "Бежать"))
		return -1;
	if(-1 == dict_new("Star", "Noun", "Звезда"))
		return -1;
	if(-1 == dict_save(dict_file_name))
		return -1;
	dict_clear();
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

	MENU top_menu, main_menu;
	SMALL_RECT rect;

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

	const int main_column_count = 4;
	const char* main_headers[main_column_count] = {
		"Word", "Part", "Trans", "Size"
	};
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
	if(-1 == dict_load(dict_file_name))
		return -1;

	int main_item_count = dict_size; // количество пунктов меню
	ITEM_DEF* main_menu_items = (ITEM_DEF*)malloc(main_item_count * sizeof(ITEM_DEF));
	memset(main_menu_items, 0x00, main_item_count * sizeof(ITEM_DEF));
	DICT_ENTRY* curr = dict_head;
	char** buffers = (char**)malloc(main_item_count * sizeof(char*));
	for(int i = 0; i < main_item_count; i++) {
		for(int fld = 0; fld < DICT_FLD_CNT; fld++)
			main_menu_items[i].str[fld] = curr->field[fld];
		buffers[i] = (char*)malloc(32 * sizeof(char));
		main_menu_items[i].str[DICT_FLD_CNT] = itoa(strlen(curr->field[0]), buffers[i], 10);
		main_menu_items[i].cb = Edit;
		curr = curr->next;
	}
	#endif

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

	#ifndef TEST_MAIN_TABLE
	free(main_menu_items);
	for(int i = 0; i < main_item_count; i++)
		free(buffers[i]);
	free(buffers);
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
	return 0;
}
// Функция меню <Выход>. Заполняется кодом пользователя
int Exit(MENU* menu) {
	int i;
	MENU exit_menu;
	SMALL_RECT rect;
	#if 1
	const int width = 10;
	const int height = 6;
	rect.Left = (csbInfo.srWindow.Right - csbInfo.srWindow.Left - width) / 2;
	rect.Right = rect.Left + width - 1;
	rect.Top = (csbInfo.srWindow.Bottom - csbInfo.srWindow.Top - height) / 2;
	rect.Bottom = rect.Top + height - 1;
	#else
	rect.Left = menu->wnd.rect.Left + 5;
	rect.Right = menu->wnd.rect.Right - 5;
	rect.Top = menu->wnd.rect.Top + 5;
	rect.Bottom = menu->wnd.rect.Bottom - 5;
	#endif
	menu_init(&exit_menu, &main_menu, menu->hStdOut, exit_menu_items, exit_item_count,
		exit_column_count, MENU_ORIENT_VERT, &rect, 1, exit_headers);
	menu_active_color(&exit_menu, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_inactive_color(&exit_menu, BACKGROUND_BLUE | BACKGROUND_GREEN);
	menu_draw(&exit_menu, MENU_MSG_LOOP);
	menu_clear(&exit_menu);
	return -1;

}
//---------------------------------------------------------------------------
// Функции меню
//---------------------------------------------------------------------------
// Функция меню <Изменить>
int Edit(MENU* menu) {
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
	//printf("Search\n");
	return 0;
}
// Функция меню <Поиск>
int Search(MENU* menu) {
	//printf("Search\n");
	return 0;
}
// Функция меню <Сортировка>
int Sort(MENU* menu) {
	//printf("Sort\n");
	return 0;
}
// Функция меню <Сохранить>
int Save(MENU* menu) {
	//printf("Save\n");
	return 0;
}
// Функция меню <Помощь>. Заполняется кодом пользователя
int Help(MENU* menu) {
	//printf("Help\n");
	return 0;
}
// Функция меню <Очистить>. Заполняется кодом пользователя
/*int Clear(MENU* menu)
{
	cls(menu, 0);
	menu->curspos.X = 0;
	menu->curspos.Y = 1;
	return 1;
}*/

