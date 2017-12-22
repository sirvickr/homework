#include <stdio.h>
#include <math.h>

#define MIN_N 2
#define MAX_N 24

#define MIN_X 0
#define MAX_X ((int)5e7)

#define MIN_S ((int)-1e9)
#define MAX_S ((int)1e9)

void FindSumSeq(int N);

int main()
{
	int N;
	printf("input: ");
	scanf("%d", &N);
	FindSumSeq(N);
	return 0;
}

void FindSumSeq(int N) {
	int i, total, index;
	int S;
	int X[MAX_N];
	if (N < MIN_N || N > MAX_N) {
		printf("\nvalid range: %d <= N <= %d\n\n", MIN_N, MAX_N);
		return;
	}
	for (i = 0; i < N; ++i) {
		scanf("%d", &X[i]);
		if (X[i] < MIN_X || X[i] > MAX_X) {
			printf("\nvalid range: %d <= X <= %d\n\n", MIN_X, MAX_X);
			return;
		}
	}
	scanf("%d", &S);
	if (S < MIN_S || S > MAX_S) {
		printf("\nvalid range: %d <= S <= %d\n\n", MIN_S, MAX_S);
		return;
	}
	// переберем все возможные суммы,
	// т.е. все возможные сочетания знаков слагаемых
	total = pow(2, N);
	for (index = 0; index < total; ++index) {
		// в двоичном преставлении индекса суммы,
		// единицы соответствуют слагаемым со знаком "-",
		// а нули - слагаемым со знаком "+", например, в случае
		// входной последовательности из 3 элементов { 15, 25, 30 }:
		// для 0-й суммы (000): +15 +25 +30 = 70;
		// для 1-й суммы (001): -15 +25 +30 = 40;
		// для 2-й суммы (010): +15 -25 +30 = 20;
		// ...
		// для 7-й суммы (111): -15 -25 -30 = ;
		// всего 2^3 = 8 возможных вариантов суммирования
		int sum = 0;
		for (i = 0; i < N; ++i) {
			if ((index >> i) & 1) {
				sum -= X[i];
			} else {
				sum += X[i];
			}
		}
		if (sum == S) {
			break;
		}
	} // for (index)
	if (index < total) {
		printf("%d", (index & 1) ? -X[0] : X[0]);
		for (i = 1; i < N; ++i)
			printf("%+d", ((index >> i) & 1) ? -X[i] : X[i]);
		printf("=%d\n", S);
	} else {
		printf("no solution\n");
	}
}
