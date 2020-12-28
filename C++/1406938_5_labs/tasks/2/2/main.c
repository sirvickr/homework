/* 179 (а) Даны натуральные числа n, Q1, ... , Qn.
 * Найти те члены Qi, последовательности Q1, ... , Qn, которые являются удвонными нечётным числами.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// точка входа программы
int main( int argc, char* argv[] )
{
	int n, q;
	scanf ("%d", &n);
	printf("n = %d\n", n);
	for(int i = 0; i < n; i++) {
		scanf ("%d", &q);
		if((q & 1) == 0 && ((q / 2) & 1) == 1) {
			printf("%d\n", q);
		}
	}

	return 0;
}
