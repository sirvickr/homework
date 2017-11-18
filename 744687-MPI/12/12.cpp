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

	const int MSG_TAG_DATA = 1;

	const int N = 8;
	const int M = N / 2;
	MPI_Datatype DataType;
	const int block_count = 2;
	int array_of_blocklengths[block_count] = { N, N };
	MPI_Aint array_of_displacements[block_count] = { 0, N * N * sizeof(int) / 2 };
	MPI_Datatype array_of_types[block_count] = { MPI_INT, MPI_INT };

	MPI_Type_create_struct(
		block_count,
		array_of_blocklengths,
		array_of_displacements,
		array_of_types,
		&DataType
	);

	MPI_Type_commit(&DataType);

	int A[N][N];
	int B[2][N] = { 0 };
	if (0 == rank) {
		srand(time(NULL));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				A[i][j] = rand() % 10;
			}
		}
		cout << "A:" << endl;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << " " << A[i][j];
			}
			cout << endl;
		}
		int dest_count = min(proc_count, M);
		for (int i = 1; i < dest_count; i++) {
			MPI_Send(&A[i][0], 1, DataType, i, MSG_TAG_DATA, MPI_COMM_WORLD);
		}
	} else if (rank < M) {
		MPI_Recv(&B[0][0], 2 * N, MPI_INT, 0, MSG_TAG_DATA, MPI_COMM_WORLD, &status);
		cout << rank << ".B:" << endl;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < N; j++) {
				cout << " " << B[i][j];
			}
			cout << endl;
		}
	}

	MPI_Type_free(&DataType);
	MPI_Finalize();

	return 0;
}
