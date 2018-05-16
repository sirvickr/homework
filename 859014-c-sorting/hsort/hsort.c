#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

// вывести массив на экран (с подписью и в нужном формате)
static void print_array(char **a, const size_t len, const char *legend) {
	size_t i;
	printf(legend);
	for (i = 0; i < len; i++)
		printf("\"%s\" ", a[i]);
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

static void swap(char** a, char** b) {
	char* s = *a;
	*a = *b;
	*b = s;
}

#if 0
static void fix_down(char** a, size_t k, size_t n) {
	size_t i;
	while(k * 2 <= n) {
		i = 2 * k;
		if(i < n && calc_a_count(a[i]) < calc_a_count(a[i + 1]))
			i++;
		if(!(calc_a_count(a[k]) < calc_a_count(a[i])))
			break;
		swap(&a[k], &a[i]);
		k = i;
	}
}
#else
void fix_down(char** a, int root, int bottom)
{
  int maxChild; // индекс максимального потомка
  int done = 0; // флаг того, что куча сформирована
  // Пока не дошли до последнего ряда
  while ((root * 2 <= bottom) && (!done)) 
  {
    if (root * 2 == bottom)    // если мы в последнем ряду,
      maxChild = root * 2;    // запоминаем левый потомок
    // иначе запоминаем больший потомок из двух
    else if (calc_a_count(a[root * 2]) > calc_a_count(a[root * 2 + 1]))
      maxChild = root * 2;
    else
      maxChild = root * 2 + 1;
    // если элемент вершины меньше максимального потомка
    if (calc_a_count(a[root]) < calc_a_count(a[maxChild])) 
    {
		swap(&a[root], &a[maxChild]);
		root = maxChild;
    }
    else // иначе
      done = 1; // пирамида сформирована
  }
}
#endif
//static void hsort(void* base, size_t nel, size_t width, int (*compare)(const void* a, const void* b)) {
#if 0
static void hsort(char** base, size_t nel, size_t width, int (*compare)(const void* a, const void* b)) {
	int i, n = nel;
	for(i = n / 2; i >= 1; i--) {
		fix_down(base, i, n);
	}
	while(n > 1) {
		swap(&base[1], &base[n]);
		fix_down(base, 1, --n);
	}
}
#else
static void hsort(char** base, size_t nel, size_t width, int (*compare)(const void* a, const void* b)) {
	int i;
	for(i = nel / 2 - 1; i >= 0; i--) {
		fix_down(base, i, nel);
	}
	for(i = nel - 1; i >= 1; i--) {
		swap(&base[0], &base[i]);
		fix_down(base, 0, i - 1);
	}
}
#endif

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
