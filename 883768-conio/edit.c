#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#define __EDIT_H__
#define __DICTIONARY_DB_CODES_H__
#include "common.h"

#define WND_LEFT 18
#define WND_TOP 8
#define WND_WIDTH (INPUT_LENGTH * 2 - 5)
#define WND_HEIGHT (INPUT_COUNT + 2)

char contents[INPUT_COUNT][INPUT_LENGTH + 1];

static void trim(char *str) {
	 int len = strlen(str);
	 int beg = 0;
	 while(str[len - 1] == ' ')
		str[--len] = 0;
	 while(str[beg] == ' ')
		str[beg++] = 0;
	 if(beg)
		memmove(str, str + beg, len - beg + 1);
}

static int accept_input(char ch) {
	if(isalnum(ch))
		return 1;
	if(ispunct(ch) || ' ' == ch)
		return 1;
	if((-96 <= ch && ch <= -81) || (-32 <= ch && ch <= -17) || -15 == ch)
		return 1;
	return 0;
}

int edit_window(int reset)
{
	int result = 0;
	int i, j, row = 0, left = WND_WIDTH - INPUT_LENGTH - 1, top = 2, run = 1;
	int pos[INPUT_COUNT] = { 0 };
	int len[INPUT_COUNT] = { 0 };
	char line[WND_WIDTH * WND_HEIGHT + 1];
	memset(line, ' ', sizeof(line));
	// ������� �������
	memset(line,                                CHAR_BORDER_HORZ, WND_WIDTH);
	// ������ �������
	memset(line + WND_WIDTH * (WND_HEIGHT - 1), CHAR_BORDER_HORZ, WND_WIDTH);
	// ������������ �������
	for(i = 1; i < WND_HEIGHT - 1; ++i) {
		line[(i + 0) * WND_WIDTH - 0] = CHAR_BORDER_VERT;
		line[(i + 1) * WND_WIDTH - 1] = CHAR_BORDER_VERT;
	}
	// ����
	line[0] = CHAR_BORDER_LEFT_TOP;
	line[WND_WIDTH - 1] = CHAR_BORDER_RIGHT_TOP;
	line[WND_WIDTH * (WND_HEIGHT - 1)] = CHAR_BORDER_LEFT_BOTTOM;
	line[WND_WIDTH * WND_HEIGHT - 1] = CHAR_BORDER_RIGHT_BOTTOM;
	line[WND_WIDTH * WND_HEIGHT] = '\0';
	// ��������� ����
	window(WND_LEFT, WND_TOP, WND_LEFT + WND_WIDTH - 1, WND_TOP + WND_HEIGHT);
	textbackground(CYAN);
	textcolor(WHITE);
	cprintf(line);
	// �������
	gotoxy(3, 2);
	cprintf("Word");
	gotoxy(3, 3);
	cprintf("Part");
	gotoxy(3, 4);
	cprintf("Translate");

	//cprintf("     ���� ������ �����!   ");
	//window(38, 11, 40, 11);
	//textattr(BLUE * 16 | WHITE);

	textattr(LIGHTGREEN * 16 | WHITE);
	if(reset) {
		for(i = 0; i < INPUT_COUNT; ++i) {
			memset(contents[i], ' ', INPUT_LENGTH);
			contents[i][INPUT_LENGTH] = '\0';
			gotoxy(left, top + i);
			cprintf("%s", contents[i]);
		}
	} else {
		for(i = 0; i < INPUT_COUNT; ++i) {
			len[i] = 0;//strlen(contents[i]);
			for(j = 0; j < INPUT_LENGTH; ++j) {
				char next = contents[i][len[i]];
				if(' ' == next || '\0' == next)
					break;
				len[i]++;
			}
			pos[i] = len[i];
			gotoxy(left, top + i);
			cprintf("%s", contents[i]);
		}
	}

	gotoxy(left + pos[0], top);

	//window(1, 1, 80, 25);

	//while(getch() != KEY_ENTER)
	//	;
	//gotoxy(1, 1);

	while (run) {
		if (kbhit()) {
			int ch = _getch();
			switch (ch) {
			case KEY_HOME:
				pos[row] = 0;
				gotoxy(left + pos[row], top);
				break;
			case KEY_ARROW_LEFT:
				if(pos[row] > 0) {
					pos[row]--;
					gotoxy(left + pos[row], top);
				}
				break;
			case KEY_ARROW_RIGHT:
				if(pos[row] < len[row]) {
					pos[row]++;
					gotoxy(left + pos[row], top);
				}
				break;
			case KEY_END:
				pos[row] = len[row];
				gotoxy(left + pos[row], top);
				break;
			//case KEY_ARROW_UP: case KEY_ARROW_DOWN:
			//	break;
			case KEY_ENTER:
				for(i = 0; i < INPUT_COUNT; ++i)
					trim(contents[i]);
				//printf("\n%s", contents[row]);
				result = 1;
				run = 0;
				break;
			case KEY_BACKSPACE:
				if(pos[row] > 0) {
					for(i = pos[row]; i < len[row]; ++i)
						contents[row][i - 1] = contents[row][i];
					contents[row][len[row] - 1] = ' ';
					pos[row]--;
					len[row]--;
				}
				break;
			case KEY_DEL:
				if(pos[row] < len[row]) {
					for(i = pos[row]; i < len[row]; ++i)
						contents[row][i] = contents[row][i + 1];
					contents[row][len[row] - 1] = ' ';
					len[row]--;
				}
				break;
			case KEY_TAB:
				if(++row == INPUT_COUNT)
					row = 0;
				break;
			case KEY_ESC:
				result = 0;
				run = 0;
				break;
			default:
				if(accept_input(ch)) {
					if(len[row] < INPUT_LENGTH) {
						if(pos[row] < len[row]) {
							for(i = len[row]; i > pos[row]; --i)
								contents[row][i] = contents[row][i - 1];
							contents[row][pos[row]] = ch;
							pos[row]++;
							len[row]++;
						} else {
							contents[row][pos[row]] = ch;
							pos[row]++;
							len[row]++;
						}
						//printf("%c", ch);
					}
				}
				break;
			} // switch(iKey)

			gotoxy(left, top + row);
			cprintf("%s", contents[row]);
			gotoxy(left + pos[row], top + row);
		} // if (kbhit())
	} // while
	//window(18, 8, 58, 12);
	//textattr(BLACK * 16 | WHITE);
	//clrscr();
	return result;
}

