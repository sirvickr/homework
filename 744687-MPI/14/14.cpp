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

	const int root = 0;
	const int M = 10;
	int x[M];
	const int N = 8;
	double y[M];
	const int buff_size = sizeof(x) + sizeof(y);
	char buff[buff_size];
	int pos = 0;

	if (root == rank) {
		srand(time(NULL));
		for (int i = 0; i < M; i++) {
			x[i] = rand() % 10;
		}
		for (int i = 0; i < N; i++) {
			y[i] = (rand() % 10) + (rand() % 10) / 10.0 - 20;
		}
		MPI_Pack(&x, M, MPI_INT, buff, buff_size, &pos, MPI_COMM_WORLD);
		MPI_Pack(&y, N, MPI_DOUBLE, buff, buff_size, &pos, MPI_COMM_WORLD);
	}

	MPI_Bcast(&buff[0], buff_size, MPI_PACKED, root, MPI_COMM_WORLD);

	if (root != rank) {
		MPI_Unpack(buff, buff_size, &pos, &x, M, MPI_INT, MPI_COMM_WORLD);
		MPI_Unpack(buff, buff_size, &pos, &y, N, MPI_DOUBLE, MPI_COMM_WORLD);
	}

	cout << "[" << rank << "] x:";
	for (int i = 0; i < M; i++)
		cout << setw(3) << x[i];
	cout << setw(8) << "y:";
	for (int i = 0; i < N; i++)
		cout << setw(7) << y[i];
	cout << endl;

	MPI_Finalize();

	return 0;
}
