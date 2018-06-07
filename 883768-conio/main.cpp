//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "menu.h"

extern HANDLE hStdOut; // дескриптор консольного окна
extern CONSOLE_SCREEN_BUFFER_INFO csbInfo; // информаци€ о консольном окне
extern SMALL_RECT consolRect; // координаты углов консоли
extern WORD workWindowAttributes; // атрибуты рабочей области консоли

#pragma argsused
int main(int argc, char* argv[])
{
//	setlocale(LC_CTYPE, "rus"); // вызов функции настройки национальных параметров
	//SetConsoleTitle("ѕример создани€ строки меню");
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
	consolRect = csbInfo.srWindow; // координаты углов консоли
	SetConsoleTextAttribute(hStdOut, workWindowAttributes);
	system("CLS"); // установка атрибутов цвета рабочей области
	DrawMenu(); // рисуем меню в верхней строке консоли
/*#if 1
	//получение дескриптора стандартного устройства вывода
	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle("Demo Console Cls"); // «аголовок консоли
	printf("Hello\n");
	system("pause"); // Ќаблюдаем текст в окне
	SetConsoleTextAttribute(wHnd,
		BACKGROUND_RED |
		BACKGROUND_GREEN |
		BACKGROUND_BLUE |
		FOREGROUND_RED);
	mycls(wHnd); // очищаем окно
	int iKey = 67;
	// ÷икл до тех пор пока не нажата клавиша ESC
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
//---------------------------------------------------------------------------
 