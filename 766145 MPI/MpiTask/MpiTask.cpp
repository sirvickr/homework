// Нахождение минимальных значений по строкам матрицы
#include "stdafx.h"
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[])
{
	const int root = 0;
	const int N = 4;

	int proc_count, rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (proc_count == N) {
		double A[N][N]; // исходная матрица
		double a[N] = { 0 }; // строка матрицы
		double value = 0; // локальный результат (минимальный элемент строки)
		double b[N] = { 0 }; // буфер для локальных результатов

		float check_result = 0;
		if (rank == root) {
			srand(static_cast<uint32_t>(time(NULL)));
			cout << "A:" << endl;
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					A[i][j] = (rand() % 100) - 50;
					cout << setw(4) << A[i][j];
				}
				cout << endl;
			}
		}
		// раздать строки по процессам
		MPI_Scatter(
			&A[0][0], N, MPI_DOUBLE,
			&a[0], N, MPI_DOUBLE,
			root, MPI_COMM_WORLD);
		// посчитать локальный минимум своей строки
		value = a[0];
		for (int i = 1; i < N; i++)
			if (value > a[i])
				value = a[i];
		// собрать локальные результаты в главный процесс
		MPI_Gather(
			&value, 1, MPI_DOUBLE,
			&b[0], 1, MPI_DOUBLE,
			root, MPI_COMM_WORLD);

		if (rank == root) {
			cout << "min values for each row:\n";
			for (int i = 0; i < N; i++) {
				cout << setw(2) << i + 1 << ")" << setw(4) << b[i] << '\n';
			}
		}
	}
	else {
		cerr << "launch " << N << " processes" << endl;
	}

	MPI_Finalize();

	return 0;
}
