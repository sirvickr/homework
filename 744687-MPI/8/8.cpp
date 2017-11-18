#include "stdafx.h"
#include "mpi.h"

using namespace std;

float* create_vector(int count) {
	float* v = new float[count];
	cout << "source:";
	for (int i = 0; i < count; i++) {
		v[i] = (rand() % 20) - 10;
		cout << " " << v[i];
	}
	cout << endl;
	return v;
}

float inner_product(float* a, float* b, int count) {
	float result = 0;
	for (int i = 0; i < count; i++) {
		result += a[i] * b[i];
	}
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

	float* a = NULL;
	float* b = NULL;

	float check_result = 0;
	if (rank == root) {
		srand(time(NULL));
		int count = num_elements_per_proc * proc_count;
		a = create_vector(count);
		b = create_vector(count);
		check_result = inner_product(a, b, count);
		cout << "result to check: " << check_result << endl;
	}

	float* sub_a = new float[num_elements_per_proc];
	float* sub_b = new float[num_elements_per_proc];
	// раздать подвектора по процессам
	MPI_Scatter(a, num_elements_per_proc, MPI_FLOAT, sub_a, num_elements_per_proc, MPI_FLOAT, root, MPI_COMM_WORLD);
	MPI_Scatter(b, num_elements_per_proc, MPI_FLOAT, sub_b, num_elements_per_proc, MPI_FLOAT, root, MPI_COMM_WORLD);
	// посчитать свой локальный результат
	float local_result = inner_product(sub_a, sub_b, num_elements_per_proc);
	// просуммировать локальные результаты и сохранить в процессе[root]
	float global_result;
	MPI_Reduce(&local_result, &global_result, 1, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);
	if (rank == root) {
		cout << "inner product = " << global_result;
		if (check_result == global_result)
			cout << " (correct)";
		else
			cout << " (mistake)";
		cout << endl;
	}

	delete[] a;     a = NULL;
	delete[] b;     b = NULL;
	delete[] sub_a; sub_a = NULL;
	delete[] sub_b; sub_b = NULL;

	MPI_Finalize();

	return 0;
}
