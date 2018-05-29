#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

// Все три функции принимают число строк, число столбцов и указатель на начало блока памяти с матрицей

// Вывод матрицы на экран
void printRealMatrix(size_t m, size_t n, double* matrix) {
	cout << setfill(' ') << left;
	for(size_t i = 0; i < m; ++i) {
		for(size_t j = 0; j < n; ++j)
			cout << fixed << setprecision(3) << setw(11) <<  matrix[i * n + j];
		cout << endl;
	}
	cout << right << setfill('0') << "(Cols=" << n << " (" << oct << setw(3) << n << dec
		 << "), Rows=" << m << " (" << oct << setw(3) << m << dec << "))" << endl;
}

// Сумма элементов матрицы из "нижней треугольной области"
double lowTriSum(size_t m, size_t n, double* matrix) {
	double result = 0;
	for(size_t i = 0; i < m; ++i)
		for(size_t j = 0; j <= i && j < n; ++j)
			result += matrix[i * n + j];
	return result;
}

// Ввод элементов матрицы из потока (например, из файла), который передаётся в последнем параметре
double* inputMatrx(size_t& m, size_t& n, istream& input) {
	double* result = NULL;
	if(input >> m >> n) {
		result = new double[m * n];
		for(size_t i = 0; i < m; ++i) {
			size_t j;
			for(j = 0; j < n; ++j)
				if(!(input >> result[i * n + j]))
					break;
			if(j < n) {
				cerr << "Not enough input data" << endl;
				delete[] result;
				result = NULL;
				break;
			}
		}
	}
	return result;
}

int main() {
	const char* inFileName = "input1.txt";
	// Пытаемся открыть файл
	ifstream input(inFileName);
	if(!input) {
		cerr << "Failed to open file \"" << inFileName << "\"" << endl;
		return 1;
	}
	// Читаем матрицу из файла
	size_t m = 0, n = 0;
	double* matrix = inputMatrx(m, n, input);
	if(!matrix)
		return 1;
	// Работаем с матрицей
	printRealMatrix(m, n, (double*)matrix);
	double sum = lowTriSum(m, n, (double*)matrix);
	cout << "Sum=" << sum << endl;
	return 0;
}
