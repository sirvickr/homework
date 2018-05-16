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

static void swap(char** a, char** b) {
	char* s = *a;
	*a = *b;
	*b = s;
}

static int intersection_count(const char* a, size_t n, const char* b, size_t m) {
	int i, j, start;
	int count = 0;
	//printf(" intersection_count: %s %s\n", a, b);
	for (start = 0; start < m; start++) {
		//printf(" %c", b[start]);
		if(a[0] == b[start]) {
			//printf("+\t");
			for (i = 0, j = start; i < n, j < m; i++, j++) {
				if(a[i] != b[j]) {
					//printf("-");
					count = 0;
					break;
				} else {
					//printf("%c", a[i]);
					count++;
				}
			}
			if(j == m) {
				break;
			}
		}
		//printf("\n");
	}
	return count;
}

static int strjoin(const char* first, size_t n, char* b, size_t m, char* result, int res_size) {
	int i, j, start;
	// длина общей подстроки
	int count, counts[2] = {0, 0};

	char* a = (char*)malloc((n + 1) * sizeof(char));
	strcpy(a, first);
	//printf("strjoin: %s (%u) %s (%u): %s size %d\n", a, n, b, m, result, res_size);

	// проверяем глубину стыковки первого слова в конец второго
	counts[0] = intersection_count(a, n, b, m);
	// проверяем глубину стыковки второго слова в конец первого
	counts[1] = intersection_count(b, m, a, n);
	// выбираем наиболее подходящую стыковку
	if(counts[0] >= counts[1])
		count = counts[0];
	else
		count = counts[1];
	int total = m + n - count;
	//printf("\ncounts = %d %d count %d total %d\n", counts[0], counts[1], count, total);
	if(total > 0) {
		if(total > res_size)
			result = (char*)realloc(result, (total + 1) * sizeof(char));
		if(counts[0] >= counts[1]) {
			strcpy(result, b);
			strncat(result, a + count, n - count);
		} else {
			strcpy(result, a);
			strncat(result, b + count, m - count);
		}
	}
	free(a);
	//printf("result = %s\n", result);
	return total;
}

static int superstr(char** a, size_t n) {
	int i, j, k, slen, count = 0;
	
	int size = strlen(a[0]);
	char* s = (char*)malloc((size + 1) * sizeof(char));
	strcpy(s, a[0]);
	
	for (i = 1; i < n; i++) {
		size = strjoin(s, strlen(s), a[i], strlen(a[i]), s, size);
	}

	count = strlen(s);
	
	free(s);
	
	return count;
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
		
		int count = superstr(a, len);

		// выводим результат
		printf("%d\n", count);
		
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
