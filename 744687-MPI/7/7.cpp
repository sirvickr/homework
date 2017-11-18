#include "stdafx.h"
#include "mpi.h"

using namespace std;

float* create_vector(int count, float& norm) {
	srand(time(NULL));
	float* v = new float[count];
	cout << "source:";
	norm = 0;
	for (int i = 0; i < count; i++) {
		v[i] = (rand() % 20) - 10;
		norm += fabs(v[i]);
		cout << " " << v[i];
	}
	cout << " (norm = " << norm << ")" << endl;
	return v;
}

float sum_abs(float* v, int count) {
	float result = 0;
	for (int i = 0; i < count; i++)
		result += fabs(v[i]);
	return result;
}

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "Russian");

	MPI_Init(&argc, &argv);

	int proc_count, rank;
	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int root = 0;
	const int num_elements_per_proc = 4;

	float* x = NULL;
	float check_norm;

	if (rank == root) {
		x = create_vector(num_elements_per_proc * proc_count, check_norm);
	}

	float* sub_vector = new float[num_elements_per_proc];
	// раздать подвектора по процессам
	MPI_Scatter(x, num_elements_per_proc, MPI_FLOAT, sub_vector, num_elements_per_proc, MPI_FLOAT, root, MPI_COMM_WORLD);
	// посчитать свой локальный результат
	float local_norm = sum_abs(sub_vector, num_elements_per_proc);
	// просуммировать локальные результаты и сохранить в процессе[root]
	float global_norm;
	MPI_Reduce(&local_norm, &global_norm, 1, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);
	if (rank == root) {
		cout << "norm = " << global_norm;
		if (check_norm == global_norm)
			cout << " (correct)";
		else
			cout << " (mistake)";
		cout << endl;
	}

	delete[] sub_vector;
	sub_vector = NULL;
	delete[] x;
	x = NULL;

	MPI_Finalize();

	return 0;
}
