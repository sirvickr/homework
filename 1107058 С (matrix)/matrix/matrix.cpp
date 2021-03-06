﻿#include "pch.h"
#include <stdlib.h>
#include <stdio.h>

#define A_SIZE 5
#define B_SIZE 7
#define C_SIZE 4

// Предварительное объявление функции
int main_positive_product(int* matrix, int dimSize);

// Основная функция - точка входа программы
int main()
{
	// Выделение памяти под массивы для 
	// хранения элементов исходных матриц
	int* A = (int*)malloc(sizeof(int) * A_SIZE * A_SIZE);
	int* B = (int*)malloc(sizeof(int) * B_SIZE * B_SIZE);
	int* C = (int*)malloc(sizeof(int) * C_SIZE * C_SIZE);

	// Ввод матрицы A
	printf("A[%d][%d]:\n", A_SIZE, A_SIZE);
	for (int i = 0; i < A_SIZE; ++i)
	{
		for (int j = 0; j < A_SIZE; ++j)
		{
			printf("input A[%d][%d]:", i, j);
			// Смещаем При вводе указатель на &A[i][j]
			scanf("%d", A + A_SIZE * i + j);
		}
	}
	// Ввод матрицы B
	printf("B[%d][%d]:\n", B_SIZE, B_SIZE);
	for (int i = 0; i < B_SIZE; ++i)
	{
		for (int j = 0; j < B_SIZE; ++j)
		{
			printf("input B[%d][%d]:", i, j);
			// Смещаем При вводе указатель на &B[i][j]
			scanf("%d", B + B_SIZE * i + j);
		}
	}
	
	// Ввод матрицы C
	printf("C[%d][%d]:\n", C_SIZE, C_SIZE);
	for (int i = 0; i < C_SIZE; ++i)
	{
		for (int j = 0; j < C_SIZE; ++j)
		{
			printf("input C[%d][%d]:", i, j);
			// Смещаем При вводе указатель на &C[i][j]
			scanf("%d", C + C_SIZE * i + j);
		}
	}

	// Вычисляем произведения положительных элементов 
	// главной диагонали каждой из исходных матриц
	int P_A = main_positive_product(A, A_SIZE);
	int P_B = main_positive_product(B, B_SIZE);
	int P_C = main_positive_product(C, C_SIZE);

	// Вычисляем результат выражения
	int P = P_A + P_B - P_C;

	// Выводим результат
	printf("%d + %d - %d = %d\n", P_A, P_B, P_C, P);

	// Ожидаем реакции пользователя
	printf("Press any key to finish\n");
	getchar();

	// Освобождаем выделенную память
	free(A);
	free(B);
	free(C);
}

/* Возвращает произведение положительных
   элементов главной диагонали квадратной матрицы
   Параметры:
   matrix - указатель на первый элемент массива,
			содержащего элементы квадратной матрицы
   dimSize - количество строк/столбцов матрицы
*/
int main_positive_product(int* matrix, int dimSize)
{
	int result = 1;
	for (int i = 0; i < dimSize; ++i)
	{
		int m = matrix[dimSize * i + i];
		if (m > 0)
		{
			result *= m;
		}
	}
	return result;
}
