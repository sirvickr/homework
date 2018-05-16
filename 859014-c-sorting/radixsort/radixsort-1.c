#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

// размер статического массива
#define ARRAY_LENGTH(ARR) (sizeof ARR / sizeof *ARR)
// случайное число в интервале [A, B]
#define RAND_RANGE(A,B) (A + rand() / (RAND_MAX / (B - A + 1) + 1));

static void swap(unsigned *a, unsigned *b) {
    unsigned tmp = *a;
    *a = *b;
    *b = tmp;
}
 
/* sort unsigned ints */
static void radix_sort_u(unsigned *from, unsigned *to, unsigned bit)
{
	if(!bit || to < from + 1)
		return;
 
	unsigned *ll = from, *rr = to - 1;
	for (;;) {
		/* find left most with bit, and right most without bit, swap */
		while (ll < rr && !(*ll & bit))
			ll++;
		while (ll < rr &&  (*rr & bit))
			rr--;
		if (ll >= rr)
			break;
		swap(ll, rr);
	}
 
	if (!(bit & *ll) && ll < to)
		ll++;
	bit >>= 1;
 
	radix_sort_u(from, ll, bit);
	radix_sort_u(ll, to, bit);
}
 
/* отсортировать знаковые целые: перевернуть старший бит, отсортировать как беззнаковые, перевернуть обратно */
static void radix_sort(int *a, const size_t len)
{
	size_t i;
	unsigned *x = (unsigned*) a;
 
	for (i = 0; i < len; i++) {
		x[i] ^= INT_MIN;
	}

	radix_sort_u(x, x + len, INT_MIN);

	for (i = 0; i < len; i++) {
		x[i] ^= INT_MIN;
	}
}

int main(int argc, char* argv[]) {

	size_t i;
	
	srand(time(NULL));
	int x[16];
	Int32 y[16];

	for (i = 0; i < ARRAY_LENGTH(x); i++) {
		x[i] = RAND_RANGE(-128,127)
		y[i].x = x[i];
	}

	radix_sort(x, ARRAY_LENGTH(x));

	for (i = 0; i < ARRAY_LENGTH(x); i++) {
		printf("%d%c", x[i], i + 1 < ARRAY_LENGTH(x) ? ' ' : '\n');
	}
	
	;
	
	/*for (i = 0; i < ARRAY_LENGTH(y); i++) {
		printf("%d%c", y[i].x, i + 1 < ARRAY_LENGTH(y) ? ' ' : '\n');
	}*/

	return 0;
}
