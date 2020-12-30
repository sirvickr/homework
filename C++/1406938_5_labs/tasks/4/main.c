/* 264. Даны натуральное число n, символы S1, ..., Sn.
 *      Исключить из последовательности S1, ..., Sn руппы символов, расположенные между скобками (,).
 *      Сами скобки тоже должны быть исключены.
 *      Предполагается, что внутри каждой пары скобок нет других скобок.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STATE_INITIAL 0
#define STATE_BRACKET 1
#define MAX_BUF_SIZE 1024

char s[MAX_BUF_SIZE];

// точка входа программы
int main( int argc, char* argv[] )
{
	printf("input string: ");
	scanf ("%1023s", s); 
	int state = STATE_INITIAL;
	// указатель для пробега по строке - по нему будем считывать каждый символ
	char* pc = s;
	// указатель для затирания символов, которые стоят в скобках
	char* target = NULL;
	// проходим по строке один раз, анализируя каждый символ
	while(*pc != '\0') {
		switch(state) {
		case STATE_INITIAL:
			// находимся снаружи скобок
			if(*pc == '(') {
				// нашли скобку - ставим сюда указатель для последующего затирания символов
				if(target == NULL)
					target = pc;
				state = STATE_BRACKET;
			} else if(target != NULL && target != pc) {
				// затираем символы, которые были в скобках
				*target = *pc;
				target++;
			}
			break;
		case STATE_BRACKET:
			// находимся внутри скобок
			if(*pc == ')') {
				state = STATE_INITIAL;
			}
			break;
		}
		pc++;
		///break;
	}
	if(target != NULL)
		*target = '\0'; // обрезаем строку
	fprintf(stderr, "result: \"%s\"\n", s);

	return 0;
}
