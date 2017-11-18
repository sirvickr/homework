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

	const int MSG_REQUEST = 1;
	const int MSG_REPLY = 2;

	const int N = 3;
	if (0 == rank) {
		int A[N][N];
		int B[N][N];
		srand(time(NULL));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				A[i][j] = rand() % 10;
				B[i][j] = 0;
			}
		}
		B[0][0] = A[0][0];
		cout << "source:" << endl;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << " " << A[i][j];
			}
			cout << endl;
		}
		for (int i = 1; i < proc_count; i++) {
			MPI_Send(A[i], N, MPI_INT, i, MSG_REQUEST, MPI_COMM_WORLD);
		}
		for (int i = 1; i < proc_count; i++) {
			MPI_Recv(&B[i][i], 1, MPI_INT, i, MSG_REPLY, MPI_COMM_WORLD, &status);
		}
		cout << "answer:" << endl;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << " " << B[i][j];
			}
			cout << endl;
		}
	}
	else {
		int x[N];
		MPI_Recv(x, N, MPI_INT, 0, MSG_REQUEST, MPI_COMM_WORLD, &status);
		MPI_Send(&x[rank], 1, MPI_INT, 0, MSG_REPLY, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
