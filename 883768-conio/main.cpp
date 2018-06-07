//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "menu.h"

extern HANDLE hStdOut; // ���������� ����������� ����
extern CONSOLE_SCREEN_BUFFER_INFO csbInfo; // ���������� � ���������� ����
extern SMALL_RECT consolRect; // ���������� ����� �������
extern WORD workWindowAttributes; // �������� ������� ������� �������

#pragma argsused
int main(int argc, char* argv[])
{
//	setlocale(LC_CTYPE, "rus"); // ����� ������� ��������� ������������ ����������
	//SetConsoleTitle("������ �������� ������ ����");
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdOut, &csbInfo);
	consolRect = csbInfo.srWindow; // ���������� ����� �������
	SetConsoleTextAttribute(hStdOut, workWindowAttributes);
	system("CLS"); // ��������� ��������� ����� ������� �������
	DrawMenu(); // ������ ���� � ������� ������ �������
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
//---------------------------------------------------------------------------
 