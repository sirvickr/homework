#include "stdafx.h"
#include "mpi.h"

using namespace std;

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "Russian");

	MPI_Init(&argc, &argv);

	int proc_count, rank;
	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int MSG_TAG_MATRIX = 1;
	const int MSG_TAG_VECTOR = 1;

	const int N = 3;

	const int root = 0;
	int A[N][N] = { 0 };
	int a[N] = { 0 }; // row of A
	int x[N] = { 0 };
	int y[N] = { 0 };
	int value = 0; // local row result
	if (root == rank) {
		srand(time(NULL));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				A[i][j] = (rand() % 10) - 5;
			}
			x[i] = rand() % 10;
		}
		cout << "A:" << endl;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << setw(4) << A[i][j];
			}
			cout << endl;
		}
		cout << "x:" << endl;
		for (int i = 0; i < N; i++)
			cout << setw(4) << x[i];
		cout << endl;
	}

	MPI_Bcast(&x[0], N, MPI_INT, root, MPI_COMM_WORLD);

	MPI_Scatter(
		&A[0][0], N, MPI_INT,
		&a[0], N, MPI_INT,
		root, MPI_COMM_WORLD);

	for (int i = 0; i < N; i++) {
		value += a[i] * x[i];
	}

	MPI_Gather(
		&value, 1, MPI_INT,
		&y[0], 1, MPI_INT,
		root, MPI_COMM_WORLD);

	if (root == rank) {
		cout << "y:" << endl;
		for (int i = 0; i < N; i++)
			cout << setw(4) << y[i];
		cout << "\n";
	}

	MPI_Finalize();

	return 0;
}
