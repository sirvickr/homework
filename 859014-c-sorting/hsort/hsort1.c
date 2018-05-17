#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ITEM_TYPE char*
#define HEAP_PARENT( i )	( ( ( ( i ) + 1 ) >> 1 ) - 1 )
#define HEAP_LEFT( i )		( ( ( ( ( i ) + 1 ) << 1 ) + 0 ) - 1 )
#define HEAP_RIGHT( i )		( ( ( ( ( i ) + 1 ) << 1 ) + 1 ) - 1 )

typedef int (*compare_t)(const void* a, const void* b);

static char swap_buff[sizeof(ITEM_TYPE)];

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
	return calc_a_count(*(const char**)a) - calc_a_count(*(const char**)b);
}

static void mem_swap(void* a, void* b, size_t size, void* temp_buff) {
	memcpy(temp_buff, a, size);
	memcpy(a, b, size);
	memcpy(b, temp_buff, size);
}

static void fix_down(ITEM_TYPE* base, int n, int width, int i,  compare_t compare)
{
	char* bytes = (char*)base;
	int l, r, largest;
	l = HEAP_LEFT(i);
	r = HEAP_RIGHT(i);
//	if (l < n && compare(base[l], base[i]) > 0)
	if (l < n && compare(&bytes[l * width], &bytes[i * width]) > 0)
		largest = l;
	else
		largest = i;
//	if (r < n && compare(base[r], base[largest]) > 0)
	if (r < n && compare(&bytes[r * width], &bytes[largest * width]) > 0)
		largest = r;
	if (largest != i) {
		mem_swap(bytes + largest * width, bytes + i * width, width, swap_buff);
		fix_down(base, n, width, largest, compare);
	}
}

static void build_heap(ITEM_TYPE* a, int n, int width, compare_t compare)
{
	int i;
	for (i = (n >> 1) - 1; i >= 0; i--) {
		fix_down(a, n, width, i, compare);
	}
}

//static void hsort(void* base, int nel, int width, compare_t compare)
static void hsort(ITEM_TYPE* base, int nel, int width, compare_t compare)
{
	int i;
	build_heap(base, nel, width, compare);
	for (i = nel - 1; i > 0; i--) {
	//	str_swap(&base[0], &base[i]);
		mem_swap(base, (char*)base + i * width, width, swap_buff);
		nel--;
		fix_down(base, nel, width, 0, compare);
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
		hsort(a, len, sizeof(ITEM_TYPE), compare_a_count);
		
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
