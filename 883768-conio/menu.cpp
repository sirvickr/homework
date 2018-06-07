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

#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_LEFT 75
#define KEY_ENTER 13

// Указатели на функции void f(void) - они будут выполнять пункты меню
typedef int(*FUN) (void);
// Структура для элемента меню
typedef struct {
	int x, y; // Столбец и строка консоли
	char *str; // Наименование пункта меню
	FUN cb; // Функция, привязанная к пункту меню
} ITEM;
// Глобальные переменные, используемые в функциях меню
HANDLE hStdOut; // дескриптор консольного окна
CONSOLE_SCREEN_BUFFER_INFO csbInfo;// информация о консольном окне в структуре csbInfo
SMALL_RECT consolRect; // координаты углов консольного окна
COORD curspos={0,1}; // координаты текстового курсора
WORD workWindowAttributes = 158;// атрибуты рабочей области
WORD inactiveItemAttributes = 31; // атрибуты цвета неактивного пункта меню
WORD activeItemAttributes = 160; // атрибуты цвета активного пункта меню
// Изменяемые элементы меню
enum menuitems { MNUFILE, MNUDO, MNUCLEAR, MNUEXIT };
extern const int numMenu = 4; // количество пунктов меню
ITEM menu[numMenu] = { // положение (x,y), заголовок, указатель на функцию
#if 1
	{ 1,  0, " File     ", File },
	{ 11, 0, " Action   ", Do },
	{ 21, 0, " Clear    ", Clear },
	{ 31, 0, " Exit     ", Exit }
#else
	{ 1,  0, " Файл     ", File },
	{ 11, 0, " Действие ", Do },
	{ 21, 0, " Очистить ", Clear },
	{ 31, 0, " Выход    ", Exit }
#endif
};
// Длина строк заголовков " Файл ", " Действие ", " Очистить ", " Выход "
// должна быть подобрана в соответствии с их X - координатами в массиве menu[]

#define LINE_LEN 80
char line[LINE_LEN] = { ' ' };
// Управление меню
void DrawMenu() {
	memset(line, ' ', LINE_LEN);
	line[LINE_LEN - 1] = '\0';
	// Номер текущего пункта меню
	menuitems sel = MNUFILE;
	SetConsoleTextAttribute(hStdOut, inactiveItemAttributes);
	//string s(80, ' ');
	//cout << s; // залить фон строки меню
	printf(line); fflush(stdout);
	for (int i = 0; i < numMenu; i++) { // Напечатать заголовки пунктов меню
		gotoxy(menu[i].x, menu[i].y);
		printf(menu[i].str);
	}
	fflush(stdout);
	itemMenu(sel, true); // выделить пункт меню
	fflush(stdin); // очистить буфер клавиатуры
	int iKey = 67;
	int run = 1;
	while (run) {
		if (kbhit()) {
			iKey = _getch();
			switch (iKey) {
			case KEY_ARROW_RIGHT:
				itemMenu(sel, false); // сделать неактивным пункт меню
				if (sel < numMenu - 1) {
					sel = (menuitems)(sel + 1);
				} else {
					sel = MNUFILE; // прокрутка влево
				}
				itemMenu(sel, true); // выделить активный пункт меню
				showCursor(false);
				break;
			case KEY_ARROW_LEFT:
				itemMenu(sel, false); // сделать неактивным пункт меню
				if (sel > 0) {
					sel = (menuitems)(sel - 1);
				} else {
					sel = MNUEXIT; // прокрутка вправо
				}
				itemMenu(sel, true); // выделить активный пункт меню
				showCursor(false);
				break;
			case KEY_ENTER:
				// возвращаем курсор из строки меню в прежнюю позицию
				gotoxy(curspos.X, curspos.Y);
				// Установить цвет рабочих сообщений
				SetConsoleTextAttribute(hStdOut, workWindowAttributes);
				showCursor(true);
				switch (sel) {
				case MNUFILE:
					File();
					getCursorPosition(); // запомнить положение курсора
					break;
				case MNUDO:
					Do();
					getCursorPosition(); // запомнить положение курсора
					break;
				case MNUCLEAR:
					Clear();
					// после очистки курсор в левый верхний угол консоли
					///curspos = { 0, 1 };
					curspos.X = 0;
					curspos.Y = 1;
					break;
				case MNUEXIT:
					if (Exit()) {
						gotoxy(0, 0);
						cls(1);
						run = 0;
						break;///exit(0);
					}
					// запомнить положение курсора, если отменили выход
					getCursorPosition();
					break;
				}
				// очистить буфер клавиатуры
				fflush(stdin);
				// курсор в текущий пункт меню
				gotoxy(menu[sel].x, menu[sel].y);
				// спрятать курсор
				showCursor(false);
				break;
			case 120: // выход по клавише x
			case 88: // выход по клавише X
			case 27: // выход по клавише ESC
				gotoxy(0, 0);
				cls(1);
				run = 0;///exit(0);
			} // switch(iKey)
		} // if(kbhit())
	} // while(1)
}

// Текстовый курсор в точку x,y
void gotoxy(int x, int y)
{
	COORD cursorPos = { x, y };
	SetConsoleCursorPosition(hStdOut, cursorPos);
	///SetConsoleCursorPosition(hStdOut, {x,y});
}
// Запись текущего положения текстового курсора в глобальную переменную curspos
void getCursorPosition(void)
{
	GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
	curspos = csbInfo.dwCursorPosition;
}         
// Очистка тестовой области консоли. Если it == 0, то очистка со строки,
// следующей за строкой меню, иначе очистка с левого верхнего угла консоли
void cls(int it)
{
	int i;
	//string s(80, ' ');
	SetConsoleTextAttribute(hStdOut, workWindowAttributes);
	if (it == 0)
		gotoxy(0, consolRect.Top + 1);
	else
		gotoxy(0, consolRect.Top);
	for (i = consolRect.Top; i < curspos.Y + 1; i++) // очистка от
	// первой строки до строки с курсором
	//cout << s; // залить фон строки меню
	printf(line);
	gotoxy(0, 0);
}
/*
void mycls(HANDLE hConsole)
{
	COORD coordScreen = {0,0}; // начальное положение курсора
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	DWORD cCharsWritten;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y; // количество
	// символьных ячеек в буфере консоли
	FillConsoleOutputCharacter(hConsole,(TCHAR)' ',
	dwConSize, coordScreen, &cCharsWritten);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
	dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
}*/
// Выделить пункт меню с номером sel
void itemMenu(int sel, bool activate)
{
	WORD itemAttributes;
	if (activate)
		itemAttributes = activeItemAttributes;
	else
		itemAttributes = inactiveItemAttributes;
	gotoxy(menu[sel].x, menu[sel].y);
	SetConsoleTextAttribute(hStdOut, itemAttributes);
	printf(menu[sel].str);
}
// Скрыть/показать текстовый курсор в консоли
void showCursor(bool visible)
{
	CONSOLE_CURSOR_INFO ccInfo;
	ccInfo.bVisible = visible;
	ccInfo.dwSize = 20;
	SetConsoleCursorInfo(hStdOut, &ccInfo);
}
