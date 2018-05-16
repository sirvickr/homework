#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ITEM_TYPE char*
#define HEAP_PARENT( i )	( ( ( ( i ) + 1 ) >> 1 ) - 1 )
#define HEAP_LEFT( i )		( ( ( ( ( i ) + 1 ) << 1 ) + 0 ) - 1 )
#define HEAP_RIGHT( i )		( ( ( ( ( i ) + 1 ) << 1 ) + 1 ) - 1 )

typedef int (*compare_t)(const void* a, const void* b);

static void print_array(char **a, const size_t len, const char *legend) {
	size_t i;
	printf("%s", legend);
	for (i = 0; i < len; i++)
		printf("%s ", a[i]);
	printf("\n");
}

static int calc_a_count(const char* s) {
	int i, count = 0, slen = strlen(s);
	for(i = 0; i < slen; i++) {
		if(s[i] == 'a') {
			count++;
		}
	}
	return count;
}

static int compare_a_count(const void* a, const void* b) {
	return calc_a_count((const char*)a) - calc_a_count((const char*)b);
}

static void str_swap(char** a, char** b) {
	char* s = *a;
	*a = *b;
	*b = s;
}

static void fix_down(ITEM_TYPE* a, int n, int i,  compare_t compare)
{
	int l, r, largest;
	l = HEAP_LEFT(i);
	r = HEAP_RIGHT(i);
	if (l < n && compare(a[l], a[i]) > 0)
		largest = l;
	else
		largest = i;
	if (r < n && compare(a[r], a[largest]) > 0)
		largest = r;
	if (largest != i) {
		str_swap(&a[largest], &a[i]);
		fix_down(a, n, largest, compare);
	}
}

static void build_heap(ITEM_TYPE* a, int n, compare_t compare)
{
	for (int i = (n >> 1) - 1; i >= 0; i--) {
		fix_down(a, n, i, compare);
	}
}

//static void hsort(void* base, int nel, int width, compare_t compare)
static void hsort(ITEM_TYPE* base, int nel, int width, compare_t compare)
{
	build_heap(base, nel, compare);
	for (int i = nel - 1; i > 0; i--) {
		str_swap(&base[0], &base[i]);
		nel--;
		fix_down(base, nel, 0, compare);
	}
}

int main(int argc, char* argv[]) {
	const size_t MAX_BUF_LEN = 512;
	int len, slen, i;
	char buffer[MAX_BUF_LEN];
	
	scanf("%d", &len);
	if (len <= 0) {
		fprintf(stderr, "length must be positive value\n");
		return 1;
	}

	// похоже, что данные введены корректно, выделяем память под исходный массив
	char** a = (char**)malloc(len * sizeof(char*));
	if (a == NULL) {
		fprintf(stderr, "failed to allocate memory for source array\n");
		return 2;
	}
	
	int error = 0;
	// заполняем его из стандартного потока ввода
	for (i = 0; i < len; i++) {
		//printf("input a[%d]: ", i);
		scanf("%511s", buffer);
		slen = strlen(buffer);
		a[i] = "";
		if(slen) {
			a[i] = (char*)malloc((slen + 1) * sizeof(char));
			if(a[i] == NULL) {
				error = i + 1;
				break;
			}
			strcpy(a[i], buffer);
		}
	}
	if (error == 0) {

		//print_array(a, len, "input: ");
		hsort(a, len, sizeof(char*), compare_a_count);
		
		// выводим результат
		print_array(a, len, "");

	} else {
		fprintf(stderr, "failed to allocate memory for a[%d]\n", error - 1);
		return 3;
	}
	
	// освобождаем память исходного массива
	for (i = 0; i < len; i++)
		if(a[i])
			free(a[i]);
	free(a);

	return 0;
}
