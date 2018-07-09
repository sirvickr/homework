#define __EDIT_H__
#define __DICTIONARY_DB_CODES_H__
#include "common.h"
#include <conio.h>

int edit_window()
{
	window(18, 8, 58, 12);
	textbackground(CYAN);
	textcolor(WHITE);
	clrscr();
	gotoxy(2, 2);

	cprintf("");

	//cprintf("     База данных пуста!   ");
	//window(38, 11, 40, 11);
	textattr(BLUE * 16 | WHITE);
	cprintf("OK");
	//window(1, 1, 80, 25);
	textattr(BLUE * 16 | WHITE);
	while(getch() != KEY_ENTER)
		;
	//window(18, 8, 58, 12);
	//textattr(BLACK * 16 | WHITE);
	clrscr();
	return -1;
}


