#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

typedef union Int32 {
	unsigned int x;
	unsigned char bytes[4];
} Int32;

typedef struct RadixItem {
	unsigned int count;
	unsigned int index;
	unsigned int shift;
} RadixItem;

// основание системы счисления
#define RADIX 256

// вспомогательный массив для параметров сортировки
static RadixItem radix[RADIX] = {0};

// вывести массив на экран (с подписью и в нужном формате)
static void print_array(int *a, const size_t len, const char *legend, const char *fmt) {
	size_t i;
	printf(legend);
	for (i = 0; i < len; i++)
		printf(fmt, a[i]);
	printf("\n");
}

// сортировка беззнаковых целых (основная функция)
static void radix_sort_u(unsigned *a, const size_t len) {
	size_t i, j, r;

	// вспомогательный массив для копирования исходных данных
	Int32 *b = (Int32*)malloc(len * sizeof(Int32));
	if (b == NULL) {
		fprintf(stderr, "failed to allocate memory for auxiliary array\n");
		return;
	}
	
	// внешний цикл - перебор байтов от младшего с старшему
	for (j = 0; j < 4; j++) {

		// на каждой итерации, т.е. для каждого из 4 разрядов, ("256-чных цифр")
		// сортируем исходный массив по очередной такой "цифре"
		memset(radix, 0x00, sizeof(radix));
		for (i = 0; i < len; i++) {
			// перед обработкой каждого разряда копируем исходный массив во вспомогательный
			b[i].x = a[i];
			// для каждого i-го элемента массива,
			// b[i].bytes[j] содержит j-й байт, т.е.j-ю цифру числа, 
			// но не в привычном нам 10-тичном формате, а в 256-чном
			// (поскольку число как бы "записано" 256-чными цифрами)
			// таким образом, подсчитываем общее количество исходных чисел, 
			// попадающих в элемент массива radix, соответствующий данной цифре
			// ("цифры" в данном случае - от 0 до 255)
			radix[b[i].bytes[j]].count++;
		}
		
		/*printf("байт %u:", j);
		for (i = 0; i < len; i++)
			printf(" %02X", b[i].bytes[j]);
		printf("\n");*/
		
		// дополнительный проход по radix для подсчёта начального индекса исходного массива, 
		// куда мы будем копировать группу из count элементов (внутри группы он отсортированы по текущему разряду)
		size_t total = 0;
		for (r = 1; r < RADIX; r++) {
			total += radix[r - 1].count;
			radix[r].index = total;
		}

		// выполняем последовательное копирование каждого элемента в группе,
		// из промежуточного массива в исходный
		// начальный индекс, куда копируем первый элемент группы, вычислен выше,
		// остальные элементы укладываются за ним последовательно (shift++)
		for (i = 0; i < len; i++) {
			a[radix[b[i].bytes[j]].index + radix[b[i].bytes[j]].shift] = b[ i ].x;
			radix[b[i].bytes[j]].shift++;
		}

		/*printf("radix:\nindex\tcount\tindex\tshift\n");
		for (i = 0; i < RADIX; i++) {
			printf("%d\t%d\t%d\t%d\n", i, radix[i].count, radix[i].index, radix[i].shift);
		}
		
		printf("a:");
		for (i = 0; i < len; i++)
			printf("%d ", a[i]);
		printf("\n");*/
	} // for (j)
	
	free(b);
}

// функция-обёртка для сортировки целых со знаком
static void radix_sort(int *a, const size_t len) {
	size_t i;

	//print_array(a, len, "dec:", " %d");
	//print_array(a, len, "hex:", " %08X");

	unsigned *x = (unsigned*) a;

	// перевёртываем знаковый бит в дополнительном коде, 
	// чтобы он не мешал сортировать беззнаковые целые
	for (i = 0; i < len; i++) {
		x[i] ^= INT_MIN;
	}

	// сортируем полученный массив беззнаковых целых
	radix_sort_u(x, len);

	for (i = 0; i < len; i++) {
		x[i] ^= INT_MIN;
	}

	// возвращаем знаковый бит, т.е.возвращаем исходные значения со знаком
	print_array(a, len, "", "%d ");
}

int main(int argc, char* argv[]) {
	size_t len, i;
	if (argc < 3) {
		fprintf(stderr, "expecting array size and items\n");
		return 1;
	}
	len = (size_t)atoi(argv[1]);
	if (argc < len + 2) {
		fprintf(stderr, "not all array items specified\n");
		return 2;
	}
	// похоже, что данные введены корректно, выделяем память и заполняем исходный массив
	int *a = (int*)malloc(len * sizeof(int));
	if (a == NULL) {
		fprintf(stderr, "failed to allocate memory for source array\n");
		return 3;
	}
	
	for (i = 0; i < len; i++) {
		a[i] = atoi(argv[i + 2]);
	}

	radix_sort(a, len);
	
	free(a);
	
	return 0;
}
