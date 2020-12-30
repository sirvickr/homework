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

// точка входа программы
int main( int argc, char* argv[] )
{
	char s[MAX_BUF_SIZE] = "s(abc)de(fgq)(we)rty";
	fprintf(stderr, "s = %s\n", s);
	int state = STATE_INITIAL;
	char* pc = s;
	char* target = NULL;
	while(*pc != '\0') {
		fprintf(stderr, "%c:\t", *pc);
		switch(state) {
		case STATE_INITIAL:
			fprintf(stderr, "i\t");
			if(*pc == '(') {
				fprintf(stderr, "->b");
				if(target == NULL)
					target = pc;
				state = STATE_BRACKET;
			} else if(target != NULL && target != pc) {
				fprintf(stderr, " copy: %c->%c", *target, *pc);
				*target = *pc;
				target++;
			}
			break;
		case STATE_BRACKET:
			fprintf(stderr, "b");
			if(*pc == ')') {
				fprintf(stderr, "->i");
				state = STATE_INITIAL;
			}
			break;
		}
		fprintf(stderr, "\n");
		pc++;
		///break;
	}
	if(target != NULL)
		*target = '\0';
	fprintf(stderr, "s = %s\n", s);

	return 0;
}
