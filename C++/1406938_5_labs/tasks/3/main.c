/* Написать программу, выполняющую следующие действия:
 * 1. Заполнение массива X вещественными числами, образующими арифметическую последовательность. 
 *    Количество элементов в массиве (N) и шаг последовательности (S) заданы. Начальный элемент вводит пользователь.
 * 2. Расчет заданной функции F(X) над элементами массива X и вывод значения функции пользователю. 
 *    При выводе значение функции представить в экспоненциальном формате.
 * 3. Расчет элементов массива Y по формуле Yi=G(Xi) и вывод их пользователю в виде
 *    y1	3.543
 *    y2	6.422
 *    y3	-14.223
 *    При выводе округлять значения Yi до 3-х знаков после запятой.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SEQUENCE_ITEM_COUNT 11
#define SEQUENCE_STEP 18.2f

double G(double x) {
	if(x < 15)
		return 5 * x * x + 21 * x;
	if (x > 60)
		return 1 - x / (x + 1);
	return x - 11 / (x - 5);
}

// точка входа программы
int main( int argc, char* argv[] )
{
	double x;
	printf("input x0: ");
	scanf ("%lf", &x);
	double X[SEQUENCE_ITEM_COUNT] = { x };
	double Y[SEQUENCE_ITEM_COUNT];
	double F = 0.0f;
	for(int i = 1; i < SEQUENCE_ITEM_COUNT; i++) {
		x += SEQUENCE_STEP;
		X[i] = x;
		Y[i] = G(X[i]);
		F += X[i];
	}
	printf("F = %e\n", F);
	for(int i = 0; i < SEQUENCE_ITEM_COUNT; i++) {
		printf("y%d\t%.3f\n", i + 1, Y[i]);
	}

	return 0;
}
