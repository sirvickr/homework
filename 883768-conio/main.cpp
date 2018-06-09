//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "menu.h"
#include "dict.h"

SMALL_RECT menu_rect = { 0, 0, 99, 9 };

const int item_count = 6; // ���������� ������� ����
// ��������� (x,y), ���������, ��������� �� �������
ITEM menu_items[item_count] = {
#if 1
	{ 0, 0, "Edit", Edit },
	{ 0, 0, "Search", Search },
	{ 0, 0, "Sort", Sort },
	{ 0, 0, "Save", Save },
	{ 0, 0, "Help", Help },
	{ 0, 0, "Exit", Exit },
#else
	{ 1,  0, "����", File },
	{ 11, 0, "��������", Do },
	{ 21, 0, "��������", Clear },
	{ 31, 0, "�������", Help },
	{ 41, 0, "�����", Exit },
#endif
};

int Run();

#pragma argsused
int main(int argc, char* argv[])
{
	return Run();
}

int Run() {
	MENU menu;
	//setlocale(LC_CTYPE, "rus"); // ����� ������� ��������� ������������ ����������
	//SetConsoleTitle("�����-������� �������");
	SetConsoleTitle("Dictionary");
	memset(&menu, 0x00, sizeof(menu));
	InitMenu(&menu, menu_items, item_count, MENU_ORIENT_HORZ, &menu_rect);
//	InitMenu(&menu, menu_items, item_count, MENU_ORIENT_HORZ, NULL);
	DrawMenu(&menu);
	ClearMenu(&menu);
/*#if 1
	//��������� ����������� ������������ ���������� ������
	HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle("Demo Console Cls"); // ��������� �������
	printf("Hello\n");
	system("pause"); // ��������� ����� � ����
	SetConsoleTextAttribute(wHnd,
		BACKGROUND_RED |
		BACKGROUND_GREEN |
		BACKGROUND_BLUE |
		FOREGROUND_RED);
	mycls(wHnd); // ������� ����
	int iKey = 67;
	// ���� �� ��� ��� ���� �� ������ ������� ESC
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
