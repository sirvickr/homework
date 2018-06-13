#include "input.h"
#include "codes.h"
#include <stdlib.h>
#include <string.h>

int box_init(InputBox* box, HANDLE handle, SMALL_RECT rect, char*** contents, int row_count) {
	int i, size;

	if(!contents || !row_count)
		return -1;

	memset(box, 0x00, sizeof(InputBox));
	box->handle = handle;
	box->rect = rect;
	box->size.X = rect.Right - rect.Left + 1;
	box->size.Y = rect.Bottom - rect.Top + 1;
	size = box->size.X * box->size.Y * sizeof(CHAR_INFO);

	box->bak = (CHAR_INFO*)malloc(size);
	if(!box->bak) {
		return -1;
	}
	memset(box->bak, 0x00, size);

	box->wnd = (CHAR_INFO*)malloc(size);
	if(!box->wnd) {
		free(box->bak);
		return -1;
	}
	memset(box->wnd, 0x00, size);

	box->max_width = (box->size.X - 2) / 2;
	/*box->buffer = (char*)malloc((box->max_width + 1) * sizeof(char));
	if(!box->buffer) {
		free(box->bak);
		free(box->wnd);
		return -1;
	}
	memset(box->buffer, 0x00, box->max_width * sizeof(char));*/

	box->row_count = row_count;
	#if 1 // используем внешнюю пам€ть
	box->contents = contents;
	#else
	box->contents = (char***)malloc(box->row_count * sizeof(char**));
	if(!box->contents) {
		free(box->bak);
		free(box->wnd);
		//free(box->buffer);
		return -1;
	}
	memset(box->contents, 0x00, box->row_count * sizeof(char**));
	for(i = 0; i < row_count; ++i) {
		box->contents[i] = (char**)malloc(COLUMNS * sizeof(char*));
		// надпись
		box->contents[i][TITLE] = (char*)malloc((MAX_TITLE + 1) * sizeof(char));
		strcpy(box->contents[i][TITLE], contents[i][TITLE]);
		// буфер ввода
		box->contents[i][BUFFER] = (char*)malloc((box->max_width + 1) * sizeof(char));
		memset(box->contents[i][BUFFER], ' ', (box->max_width + 1) * sizeof(char));
		if(contents[i][BUFFER]) {
			strncpy(box->contents[i][BUFFER], contents[i][BUFFER], box->max_width);
		}
		box->contents[i][BUFFER][box->max_width] = '\0';
	}
	#endif

	box->edit_attr = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE
				//| FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
				;
	return 0;
}

void box_clear(InputBox* box) {
	int i;
	if(box->bak) {
		free(box->bak);
		box->bak = NULL;
	}
	if(box->wnd) {
		free(box->wnd);
		box->wnd = NULL;
	}
	/*if(box->buffer) {
		free(box->buffer);
		box->buffer = NULL;
	}
	if(box->contents) {
		for(i = 0; i < box->row_count; ++i) {
			if(box->contents[i]) {
				if(box->contents[i][TITLE])
					free(box->contents[i][TITLE]);
				if(box->contents[i][BUFFER]);
					free(box->contents[i][BUFFER]);
				free(box->contents[i]);
			}
		}
		free(box->contents);
		box->contents = NULL;
	}*/
}

int box_save(InputBox* box) {
	COORD coord = { 0, 0 };
	int ok = ReadConsoleOutput(
	   box->handle,  // экранный буфер, из которого читаем
	   box->bak,     // буфер, в который копируем
	   box->size,    // размеры bak: колонки/строки
	   coord,  	     // верхн€€ лева€ €чейка назначени€ в bak
	   &box->rect);  // источник - пр€моугольник экранного буфера
	return ok ? 0 : -1;
}

int box_restore(InputBox* box) {
	COORD coord = { 0, 0 };
	int ok = WriteConsoleOutput(
		box->handle, // экранный буфер, в который пишем
		box->bak,    // буфер, из которого копируем
		box->size,   // размеры bak: колонки/строки
		coord,    	 // исходна€ верхн€€ лева€ €чейка в bak
		&box->rect); // приЄмник - пр€моугольник экранного буфера
	if (!ok) {
		return -1;
	}
	return ok ? 0 : -1;
}

int draw_background(InputBox* box) {
	COORD coord = { 0, 0 };
	int i, j, index = 0, ok, len;
	WORD background = BACKGROUND_BLUE | BACKGROUND_GREEN;
	for(i = 0; i < box->size.Y; ++i) {
		for(j = 0; j < box->size.X; ++j) {
			index = (i * box->size.X) + j;
			box->wnd[index].Attributes = background;
			box->wnd[index].Char.AsciiChar = ' ';
		}
	}
	for(i = 1; i <= box->row_count; ++i) {
		for(j = 1; j < box->max_width - 1; ++j) {
			index = (i * box->size.X) + j;
			box->wnd[index].Attributes = background; // box->edit_attr;
			len = strlen(box->contents[i - 1][TITLE]);
			if(j <= len) {
				box->wnd[index].Char.AsciiChar = box->contents[i - 1][TITLE][j-1];
			} else {
				box->wnd[index].Char.AsciiChar = ' ';
			}
		}
		for(j = box->max_width; j < box->size.X - 1; ++j) {
			index = (i * box->size.X) + j;
			box->wnd[index].Attributes = box->edit_attr;
			box->wnd[index].Char.AsciiChar = ' ';
		}
	}
	ok = WriteConsoleOutput(
		box->handle, // экранный буфер, в который пишем
		box->wnd,    // буфер, из которого копируем
		box->size,   // размеры wnd: колонки/строки
		coord,    	 // исходна€ верхн€€ лева€ €чейка в wnd
		&box->rect); // приЄмник - пр€моугольник экранного буфера
	return ok ? 0 : -1;
}

void gotoxy(InputBox* box, int x, int y)
{
	COORD cursorPos;
	cursorPos.X = x;
	cursorPos.Y = y;
	SetConsoleCursorPosition(box->handle, cursorPos);
}

void showCursor(HANDLE handle, int visible)
{
	CONSOLE_CURSOR_INFO ccInfo;
	ccInfo.bVisible = visible;
	ccInfo.dwSize = 20;
	SetConsoleCursorInfo(handle, &ccInfo);
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

void trim(char *str) {
	 int len = strlen(str);
	 int beg = 0;
	 while(str[len - 1] == ' ')
		str[--len] = 0;
	 while(str[beg] == ' ')
	 	str[beg++] = 0;
	 if(beg)
	 	memmove(str, str + beg, len - beg + 1);
}

int box_draw(InputBox* box) {
	int i, left, top, run = 1;
	int *pos, *len;
	if(0 == box->row_count)
		return -1;
	box->row = 0;
	pos = (int*)malloc(box->row_count * sizeof(int));
	memset(pos, 0x00, box->row_count * sizeof(int));
	len = (int*)malloc(box->row_count * sizeof(int));
	memset(len, 0x00, box->row_count * sizeof(int));

	if(-1 == draw_background(box))
		return -1;
	// координаты начала ввода
	left = box->rect.Left + box->max_width; // 1;
	top = box->rect.Top + 1;
	// Draw wnd
	/*SetConsoleTextAttribute(box->handle, pm->activeItemAttributes);
	gotoxy(box, left + 0, top + 0);
	printf("                         ");
	gotoxy(box, left + 0, top);
	printf("                         ");
	gotoxy(box, left + 0, top + 2);
	printf("                         ");
	gotoxy(box, left, top);
	SetConsoleTextAttribute(box->handle, box->inactiveItemAttributes);
	printf("                       ");   */
	SetConsoleTextAttribute(box->handle, box->edit_attr);
	// Input
	gotoxy(box, left, top);
	showCursor(box->handle, 1);
	// буфер ввода
	//memset(box->contents[box->row][BUFFER], ' ', (box->max_width + 1) * sizeof(char));
	box->contents[box->row][BUFFER][box->max_width] = '\0';
	//char tmp[2];
	//tmp[1] = '\0';
	for(i = 0; i < box->row_count; ++i) {
		len[i] = strlen(box->contents[i][BUFFER]);
		gotoxy(box, left, top + i);
		printf("%s", box->contents[i][BUFFER]);
	}
	gotoxy(box, left, top + box->row);
	while (run) {
		if (kbhit()) {
			int ch = _getch();
			//tmp[0] = ch;
			//CharToOemA(tmp, tmp);
			//OemToCharA(tmp, tmp);
			//ch = tmp[0];
			switch (ch) {
			case KEY_HOME:
				pos[box->row] = 0;
				gotoxy(box, left + pos[box->row], top);
				break;
			case KEY_ARROW_LEFT:
				if(pos[box->row] > 0) {
					pos[box->row]--;
					gotoxy(box, left + pos[box->row], top);
				}
				break;
			case KEY_ARROW_RIGHT:
				if(pos[box->row] < len[box->row]) {
					pos[box->row]++;
					gotoxy(box, left + pos[box->row], top);
				}
				break;
			case KEY_END:
				pos[box->row] = len[box->row];
				gotoxy(box, left + pos[box->row], top);
				break;
			//case KEY_ARROW_UP: case KEY_ARROW_DOWN:
			//	break;
			case KEY_ENTER:
				for(i = 0; i < box->row_count; ++i)
					trim(box->contents[i][BUFFER]);
				//printf("\n%s", box->contents[box->row][BUFFER]);
				run = 0;
				break;
			case KEY_BACKSPACE:
				if(pos[box->row] > 0) {
					for(i = pos[box->row]; i < len[box->row]; ++i)
						box->contents[box->row][BUFFER][i - 1] = box->contents[box->row][BUFFER][i];
					box->contents[box->row][BUFFER][len[box->row] - 1] = ' ';
					pos[box->row]--;
					len[box->row]--;
				}
				break;
			case KEY_DEL:
				if(pos[box->row] < len[box->row]) {
					for(i = pos[box->row]; i < len[box->row]; ++i)
						box->contents[box->row][BUFFER][i] = box->contents[box->row][BUFFER][i + 1];
					box->contents[box->row][BUFFER][len[box->row] - 1] = ' ';
					len[box->row]--;
				}
				break;
			case KEY_TAB:
				if(box->row_count && ++box->row == box->row_count)
					box->row =0;
				break;
			case KEY_ESC:
				run = 0;
				break;
			default:
				if(accept_input(ch)) {
					if(len[box->row] < box->max_width) {
						if(pos[box->row] < len[box->row]) {
							for(i = len[box->row]; i > pos[box->row]; --i)
								box->contents[box->row][BUFFER][i] = box->contents[box->row][BUFFER][i - 1];
							len[box->row]++;
							box->contents[box->row][BUFFER][pos[box->row]] = ch;
						} else {
							box->contents[box->row][BUFFER][pos[box->row]] = ch;
							pos[box->row]++;
							len[box->row]++;
						}
						//printf("%c", ch);
					}
				}
				break;
			} // switch(iKey)
			#if 0 // отладка
			gotoxy(box, left, top + 10);
			printf("%c\t%d\tpos %d\tlen %d", ch, ch, pos[box->row], len[box->row]);
			#endif
			gotoxy(box, left, top + box->row);
			printf("%s", box->contents[box->row][BUFFER]);
			gotoxy(box, left + pos[box->row], top + box->row);
		} // if(kbhit())
	} // while(run)

	free(pos);
	
	if(-1 == box_restore(box)) {
		box_clear(&box);
		return -1;
	}
	return 0;
}

