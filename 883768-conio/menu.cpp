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

// ��������� �� ������� void f(void) - ��� ����� ��������� ������ ����
typedef int(*FUN) (void);
// ��������� ��� �������� ����
typedef struct {
	int x, y; // ������� � ������ �������
	char *str; // ������������ ������ ����
	FUN cb; // �������, ����������� � ������ ����
} ITEM;
// ���������� ����������, ������������ � �������� ����
HANDLE hStdOut; // ���������� ����������� ����
CONSOLE_SCREEN_BUFFER_INFO csbInfo;// ���������� � ���������� ���� � ��������� csbInfo
SMALL_RECT consolRect; // ���������� ����� ����������� ����
COORD curspos={0,1}; // ���������� ���������� �������
WORD workWindowAttributes = 158;// �������� ������� �������
WORD inactiveItemAttributes = 31; // �������� ����� ����������� ������ ����
WORD activeItemAttributes = 160; // �������� ����� ��������� ������ ����
// ���������� �������� ����
enum menuitems { MNUFILE, MNUDO, MNUCLEAR, MNUEXIT };
extern const int numMenu = 4; // ���������� ������� ����
ITEM menu[numMenu] = { // ��������� (x,y), ���������, ��������� �� �������
#if 1
	{ 1,  0, " File     ", File },
	{ 11, 0, " Action   ", Do },
	{ 21, 0, " Clear    ", Clear },
	{ 31, 0, " Exit     ", Exit }
#else
	{ 1,  0, " ����     ", File },
	{ 11, 0, " �������� ", Do },
	{ 21, 0, " �������� ", Clear },
	{ 31, 0, " �����    ", Exit }
#endif
};
// ����� ����� ���������� " ���� ", " �������� ", " �������� ", " ����� "
// ������ ���� ��������� � ������������ � �� X - ������������ � ������� menu[]

#define LINE_LEN 80
char line[LINE_LEN] = { ' ' };
// ���������� ����
void DrawMenu() {
	memset(line, ' ', LINE_LEN);
	line[LINE_LEN - 1] = '\0';
	// ����� �������� ������ ����
	menuitems sel = MNUFILE;
	SetConsoleTextAttribute(hStdOut, inactiveItemAttributes);
	//string s(80, ' ');
	//cout << s; // ������ ��� ������ ����
	printf(line); fflush(stdout);
	for (int i = 0; i < numMenu; i++) { // ���������� ��������� ������� ����
		gotoxy(menu[i].x, menu[i].y);
		printf(menu[i].str);
	}
	fflush(stdout);
	itemMenu(sel, true); // �������� ����� ����
	fflush(stdin); // �������� ����� ����������
	int iKey = 67;
	int run = 1;
	while (run) {
		if (kbhit()) {
			iKey = _getch();
			switch (iKey) {
			case KEY_ARROW_RIGHT:
				itemMenu(sel, false); // ������� ���������� ����� ����
				if (sel < numMenu - 1) {
					sel = (menuitems)(sel + 1);
				} else {
					sel = MNUFILE; // ��������� �����
				}
				itemMenu(sel, true); // �������� �������� ����� ����
				showCursor(false);
				break;
			case KEY_ARROW_LEFT:
				itemMenu(sel, false); // ������� ���������� ����� ����
				if (sel > 0) {
					sel = (menuitems)(sel - 1);
				} else {
					sel = MNUEXIT; // ��������� ������
				}
				itemMenu(sel, true); // �������� �������� ����� ����
				showCursor(false);
				break;
			case KEY_ENTER:
				// ���������� ������ �� ������ ���� � ������� �������
				gotoxy(curspos.X, curspos.Y);
				// ���������� ���� ������� ���������
				SetConsoleTextAttribute(hStdOut, workWindowAttributes);
				showCursor(true);
				switch (sel) {
				case MNUFILE:
					File();
					getCursorPosition(); // ��������� ��������� �������
					break;
				case MNUDO:
					Do();
					getCursorPosition(); // ��������� ��������� �������
					break;
				case MNUCLEAR:
					Clear();
					// ����� ������� ������ � ����� ������� ���� �������
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
					// ��������� ��������� �������, ���� �������� �����
					getCursorPosition();
					break;
				}
				// �������� ����� ����������
				fflush(stdin);
				// ������ � ������� ����� ����
				gotoxy(menu[sel].x, menu[sel].y);
				// �������� ������
				showCursor(false);
				break;
			case 120: // ����� �� ������� x
			case 88: // ����� �� ������� X
			case 27: // ����� �� ������� ESC
				gotoxy(0, 0);
				cls(1);
				run = 0;///exit(0);
			} // switch(iKey)
		} // if(kbhit())
	} // while(1)
}

// ��������� ������ � ����� x,y
void gotoxy(int x, int y)
{
	COORD cursorPos = { x, y };
	SetConsoleCursorPosition(hStdOut, cursorPos);
	///SetConsoleCursorPosition(hStdOut, {x,y});
}
// ������ �������� ��������� ���������� ������� � ���������� ���������� curspos
void getCursorPosition(void)
{
	GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
	curspos = csbInfo.dwCursorPosition;
}         
// ������� �������� ������� �������. ���� it == 0, �� ������� �� ������,
// ��������� �� ������� ����, ����� ������� � ������ �������� ���� �������
void cls(int it)
{
	int i;
	//string s(80, ' ');
	SetConsoleTextAttribute(hStdOut, workWindowAttributes);
	if (it == 0)
		gotoxy(0, consolRect.Top + 1);
	else
		gotoxy(0, consolRect.Top);
	for (i = consolRect.Top; i < curspos.Y + 1; i++) // ������� ��
	// ������ ������ �� ������ � ��������
	//cout << s; // ������ ��� ������ ����
	printf(line);
	gotoxy(0, 0);
}
/*
void mycls(HANDLE hConsole)
{
	COORD coordScreen = {0,0}; // ��������� ��������� �������
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	DWORD cCharsWritten;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y; // ����������
	// ���������� ����� � ������ �������
	FillConsoleOutputCharacter(hConsole,(TCHAR)' ',
	dwConSize, coordScreen, &cCharsWritten);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
	dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
}*/
// �������� ����� ���� � ������� sel
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
// ������/�������� ��������� ������ � �������
void showCursor(bool visible)
{
	CONSOLE_CURSOR_INFO ccInfo;
	ccInfo.bVisible = visible;
	ccInfo.dwSize = 20;
	SetConsoleCursorInfo(hStdOut, &ccInfo);
}
