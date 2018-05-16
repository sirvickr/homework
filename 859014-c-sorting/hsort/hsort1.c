#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

// вывести массив на экран (с подписью и в нужном формате)
static void print_array(const int *a, const size_t len, const char *legend) {
	size_t i;
	printf(legend);
	for (i = 0; i < len; i++)
		printf("%d ", a[i]);
	printf("\n");
}

static void swap(int* a, int* b) {
	int t = *a;
	*a = *b;
	*b = t;
}

void fixDown(int *a, int root, int bottom)
{
  int maxChild; // индекс максимального потомка
  int done = 0; // флаг того, что куча сформирована
  // Пока не дошли до последнего ряда
  while ((root * 2 <= bottom) && (!done)) 
  {
    if (root * 2 == bottom)    // если мы в последнем ряду,
      maxChild = root * 2;    // запоминаем левый потомок
    // иначе запоминаем больший потомок из двух
    else if (a[root * 2] > a[root * 2 + 1])
      maxChild = root * 2;
    else
      maxChild = root * 2 + 1;
    // если элемент вершины меньше максимального потомка
    if (a[root] < a[maxChild]) 
    {
		swap(&a[root], &a[maxChild]);
		root = maxChild;
    }
    else // иначе
      done = 1; // пирамида сформирована
  }
}

void heapSort(int *a, int nel) 
{
	int i;
	// Формируем нижний ряд пирамиды
	for (i = (nel / 2) - 1; i >= 0; i--)
		fixDown(a, i, nel);
	// Просеиваем через пирамиду остальные элементы
	for (i = nel - 1; i >= 1; i--)
	{
		swap(&a[0], &a[i]);
		/*int temp = a[0];
		a[0] = a[i];
		a[i] = temp;*/
		fixDown(a, 0, i - 1);
	}
}

#define ITEM_COUNT 4
int main(int argc, char* argv[]) {
	int i;
	int a[ITEM_COUNT] = { 5, 0, 2, 1 };
	// Заполнение массива случайными числами
	//for (i = 0; i<ITEM_COUNT; i++)
	//  a[i] = rand() % 20 - 10;
	// Вывод элементов массива до сортировки
	for (i = 0; i<ITEM_COUNT; i++)
		printf("%d ", a[i]);
	printf("\n");
	heapSort(a, ITEM_COUNT); // вызов функции сортировки
	// Вывод элементов массива после сортировки
	for (i = 0; i<ITEM_COUNT; i++)
		printf("%d ", a[i]);
	printf("\n");
	return 0;
}
