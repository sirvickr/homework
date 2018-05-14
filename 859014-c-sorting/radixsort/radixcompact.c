#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

typedef struct RadixItem {
	unsigned int count;
	unsigned int index;
	unsigned int shift;
} RadixItem;

typedef struct ArrayItem {
	unsigned int value;
	unsigned int digit;
} ArrayItem;

#define RADIX 10

static unsigned  A[10] = { 12, 10, 45, 29, 74, 32, 11, 47, 22, 27 };
static ArrayItem B[10];

RadixItem radix[RADIX] = {0};

// размер статического массива
#define ARRAY_LENGTH(ARR) (sizeof ARR / sizeof *ARR)
// случайное число в интервале [A, B]
#define RAND_RANGE(A,B) (A + rand() / (RAND_MAX / (B - A + 1) + 1));

int main(int argc, char* argv[]) {

	size_t i, j;
	
	printf("A:");
	for (i = 0; i < ARRAY_LENGTH(A); i++) {
		printf("%d%c", A[i], i + 1 < ARRAY_LENGTH(A) ? ' ' : '\n');
	}
	int rCurr = 1, rNext; // текущий и следующий разряды
	int r;
	for (j = 0; j < 2; j++) {
		rNext = rCurr * RADIX;

		memset(radix, 0x00, sizeof(radix));
		for (i = 0; i < ARRAY_LENGTH(A); i++) {
			B[i].value = A[i];
			printf("%d%c", A[i], i + 1 < ARRAY_LENGTH(A) ? ' ' : '\n');
		}
		
		printf("разряд %d\n", rCurr);
		for (i = 0; i < ARRAY_LENGTH(A); i++) {
			B[i].digit = (A[i] % rNext) / rCurr;
			printf("%d%c", B[i].digit, i + 1 < ARRAY_LENGTH(A) ? ' ' : '\n');
			radix[B[i].digit].count++;
		}
		
		int total = 0;
		for (r = 1; r < RADIX; r++) {
			total += radix[r - 1].count;
			radix[r].index = total;
		}
		
		for (i = 0; i < ARRAY_LENGTH(A); i++) {
			A[radix[B[i].digit].index + radix[B[i].digit].shift] = B[ i ].value;
			radix[B[i].digit].shift++;
		}

		printf("radix:\nindex\tcount\tindex\tshift\n");
		for (i = 0; i < RADIX; i++) {
			printf("%d\t%d\t%d\t%d\n", i, radix[i].count, radix[i].index, radix[i].shift);
		}
		
		printf("A:");
		for (i = 0; i < ARRAY_LENGTH(A); i++) {
			printf("%d%c", A[i], i + 1 < ARRAY_LENGTH(A) ? ' ' : '\n');
		}
		
		rCurr = rNext;
	}
	
	return 0;
}
