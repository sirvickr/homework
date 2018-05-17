#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// для других типов нужны другие функции сравнения и вывода
#define ITEM_TYPE char*

// сортирующее дерево - пирамида, или куча (heap), хранится в массиве
// эти макросы служат для навигации вниз и вверх по дереву
#define HEAP_PARENT( i )	( ( ( ( i ) + 1 ) >> 1 ) - 1 )
#define HEAP_LEFT( i )		( ( ( ( ( i ) + 1 ) << 1 ) + 0 ) - 1 )
#define HEAP_RIGHT( i )		( ( ( ( ( i ) + 1 ) << 1 ) + 1 ) - 1 )

typedef int (*compare_t)(const void* a, const void* b);

// небольшая оптимизация для универсальной операции swap
static char* swap_buff = NULL;

// вывод массива строк в консоль
static void print_string_array(char **a, const size_t len, const char *legend) {
	size_t i;
	printf("%s", legend);
	for (i = 0; i < len; i++)
		printf("%s ", a[i]);
	printf("\n");
}

// подсчитать количество символов 'a' в строке
static int calc_a_count(const char* s) {
	int i, count = 0, slen = strlen(s);
	for(i = 0; i < slen; i++) {
		if(s[i] == 'a') {
			count++;
		}
	}
	return count;
}

// сравнить количество символов 'a' в двух строках
static int compare_a_count(const void* a, const void* b) {
	return calc_a_count(*(const char**)a) - calc_a_count(*(const char**)b);
}

// поменять местами два блока памяти
static void mem_swap(void* a, void* b, size_t size, void* temp_buff) {
	memcpy(temp_buff, a, size);
	memcpy(a, b, size);
	memcpy(b, temp_buff, size);
}

// спусить i-й элемент пирамиды в надлежащее его величине место
static void fix_down(void* base, int nel, int width, int i, compare_t compare)
{
	char* bytes = (char*)base;
	int l, r, largest;
	l = HEAP_LEFT(i);
	r = HEAP_RIGHT(i);
	if (l < nel && compare(&bytes[l * width], &bytes[i * width]) > 0)
		largest = l;
	else
		largest = i;
	if (r < nel && compare(&bytes[r * width], &bytes[largest * width]) > 0)
		largest = r;
	if (largest != i) {
		mem_swap(bytes + largest * width, bytes + i * width, width, swap_buff);
		fix_down(base, nel, width, largest, compare);
	}
}

// построить неубывающую пирамиду
static void build_heap(void* base, int nel, int width, compare_t compare)
{
	int i;
	for (i = (nel >> 1) - 1; i >= 0; i--) {
		fix_down(base, nel, width, i, compare);
	}
}

// функция пирамидальной сортировки
static void hsort(void* base, int nel, int width, compare_t compare)
{
	int i;
	build_heap(base, nel, width, compare);
	for (i = nel - 1; i > 0; i--) {
		mem_swap(base, (char*)base + i * width, width, swap_buff);
		fix_down(base, --nel, width, 0, compare);
	}
}

int main(int argc, char* argv[]) {
	const size_t MAX_BUF_LEN = 512;
	int len, slen, i;
	char buffer[MAX_BUF_LEN];
	
	// прочитать количество строк из стандартного потока ввода
	scanf("%d", &len);
	if (len <= 0) {
		fprintf(stderr, "length must be positive value\n");
		return 1;
	}

	// выделить память под исходный массив
	char** a = (char**)malloc(len * sizeof(char*));
	if (a == NULL) {
		fprintf(stderr, "failed to allocate memory for source array\n");
		return 2;
	}
	
	int error = 0;
	// заполнить его из стандартного потока ввода, выделяя память для каждоой строки
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
	// выполнить пирамидальную сортировку и вывести результат
	if (error == 0) {

		//print_string_array(a, len, "input: ");

		// чтобы не выделять каждый раз для операции swap()
		swap_buff = (char*)malloc(sizeof(ITEM_TYPE));
		
		hsort(a, len, sizeof(ITEM_TYPE), compare_a_count);
		
		free(swap_buff);
		
		// выводим результат
		print_string_array(a, len, "");

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
