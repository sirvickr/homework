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

	const int N = 4;

	MPI_Datatype DataType;

	const int block_count = N;
	int array_of_blocklengths[block_count] = { 0 };
	for (int i = 0; i < block_count; i++)
		array_of_blocklengths[i] = N - i;
	int array_of_displacements[block_count];
	for (int i = 0; i < block_count; i++)
		array_of_displacements[i] = i * N + i;

	MPI_Type_indexed(
		block_count,
		array_of_blocklengths,
		array_of_displacements,
		MPI_INT,
		&DataType
	);

	MPI_Type_commit(&DataType);

	const int root = 0;
	int A[N][N] = { 0 };
	if (root == rank) {
		srand(time(NULL));
		for (int i = 0; i < N; i++) {
			for (int j = i; j < N; j++) {
				A[i][j] = rand() % 10;
			}
		}
	}
	MPI_Bcast(&A[0][0], 1, DataType, root, MPI_COMM_WORLD);
	cout << rank << ".A:\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << " " << A[i][j];
		}
		cout << '\n';
	}

	MPI_Type_free(&DataType);
	MPI_Finalize();

	return 0;
}
