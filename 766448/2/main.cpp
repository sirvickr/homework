#include <iostream>
#include <string>
#include <iomanip>
#include <time.h>

#pragma hdrstop
#pragma argsused

using namespace std;

const int n = 3;
const int m = 4;

void PrintArray(const int A[n][m], const char* title = NULL) {
	if(title)
		cout << title << endl;
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; ++j) {
			cout << " " << setw(3) << A[i][j];
		}
		cout << endl;
	}
}

void FillArray(int A[n][m], int a, int b) {
	srand(time(NULL) * 1000);
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; ++j) {
			A[i][j] = (rand() % (b - a)) + a;
		}
	}
}

void PermutateArray(int A[n][m]) {
	int colMax[m];
	for(int j = 0; j < m; ++j) {
		int tmp = A[0][j]; // проходим по столбцу, находим максимумы
		for(int i = 1; i < n; ++i)
			if(tmp < A[i][j])
				tmp = A[i][j];
		colMax[j] = tmp;
	}
	for(int j = 0; j < m; ++j) { // сортируем массив по максимумам столбцов
		int pos = j;
		int tmp = colMax[j];
		for(int k = j + 1; k < m; ++k) {
			if(colMax[k] < tmp) {
				pos = k;
				tmp = colMax[k];
			}
		}
		colMax[pos] = colMax[j]; // меняем местами максимумы столбцов
		colMax[j] = tmp;
		for(int i = 0; i < n; ++i) { // меняем местами сами столбцы
			tmp = A[i][pos];
			A[i][pos] = A[i][j];
			A[i][j] = tmp;
		}
	}
}

int main(int argc, char* argv[])
{
	int A[n][m];
	FillArray(A, 10, 50);
	PrintArray(A, "source array");
	PermutateArray(A);
	PrintArray(A, "sorted array");
	cout << "press enter to leave";
	string bye;
	getline(cin, bye);
	char ch = getc(stdin);
	return 0;
}

