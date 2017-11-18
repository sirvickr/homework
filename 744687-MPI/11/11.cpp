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
	MPI_Datatype ColumnType;
	MPI_Type_vector(
		N / 2, // count
		N,     // blocklength
		N * 2, // stride)
		MPI_INT,
		&ColumnType
	);
	MPI_Type_commit(&ColumnType);

	if (0 == rank) {
		int A[N][N];
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
		MPI_Send(&A[0][0], 1, ColumnType, 1, MSG_TAG_DATA, MPI_COMM_WORLD);
		if (proc_count > 2) {
			MPI_Send(&A[1][0], 1, ColumnType, 2, MSG_TAG_DATA, MPI_COMM_WORLD);
		}
	}
	else if (rank == 1) {
		int B[M][N] = { 0 };
		MPI_Recv(&B[0][0], M * N, MPI_INT, 0, MSG_TAG_DATA, MPI_COMM_WORLD, &status);
		cout << "B:" << endl;
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				cout << " " << B[i][j];
			}
			cout << endl;
		}
	}
	else if (rank == 2) {
		int C[M][N] = { 0 };
		MPI_Recv(&C[0][0], M * N, MPI_INT, 0, MSG_TAG_DATA, MPI_COMM_WORLD, &status);
		cout << "C:" << endl;
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				cout << " " << C[i][j];
			}
			cout << endl;
		}
	}

	MPI_Type_free(&ColumnType);
	MPI_Finalize();

	return 0;
}
