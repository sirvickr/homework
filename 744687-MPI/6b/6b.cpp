#include "stdafx.h"
#include "mpi.h"

using namespace std;

const int max_size = 1000;
const int size_step = 50;
const int average_length = 100000;
int packet[max_size] = { 0 };

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "Russian");

	MPI_Init(&argc, &argv);

	int proc_count, rank;
	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (2 == proc_count) {

		int peer = rank ^ 1;

		const int MSG_DATA = 0;

		int curr_size = size_step;
		int ping_pong = 0;
		double total_time = 0;

		do {
			double start_time = MPI_Wtime();
			MPI_Bsend(packet, curr_size, MPI_INT, peer, MSG_DATA, MPI_COMM_WORLD);
			MPI_Recv(packet, curr_size, MPI_INT, peer, MSG_DATA, MPI_COMM_WORLD, &status);
			double duration = MPI_Wtime() - start_time;
			total_time += duration;
			ping_pong++;
			if (ping_pong == average_length) {
				cout << curr_size << "\t" << fixed << total_time * 1000000 / average_length << " mks" << endl;
				ping_pong = 0;
				total_time = 0;
				curr_size += size_step;
			}
		} while (curr_size <= max_size);

	}
	else if (0 == rank) {
		cerr << "��������� 2 ��������" << endl;
	}

	MPI_Finalize();

	return 0;
}
