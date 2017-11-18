#include "stdafx.h"
#include "mpi.h"

using namespace std;

float* create_vector(int count) {
	srand(time(NULL));
	float* v = new float[count];
	cout << "source:";
	for (int i = 0; i < count; i++) {
		v[i] = (rand() % 20) - 10;
		cout << " " << v[i];
	}
	cout << endl;
	return v;
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

	struct {
		float value;
		int   index;
	} in, out, check;

	int total_count = num_elements_per_proc * proc_count;

	if (rank == root) {
		x = create_vector(total_count);
		float* it = min_element(x, x + total_count);
		check.value = *it;
		check.index = it - x;
		cout << "result to check: min[" << check.index << "]: " << check.value << endl;
	}

	float* sub_x = new float[num_elements_per_proc];
	// раздать подвектора по процессам
	MPI_Scatter(x, num_elements_per_proc, MPI_FLOAT, sub_x, num_elements_per_proc, MPI_FLOAT, root, MPI_COMM_WORLD);
	// посчитать свой локальный минимум
	in.value = sub_x[0];
	in.index = 0;
	for (int i = 1; i < num_elements_per_proc; i++)
		if (in.value > sub_x[i]) {
			in.value = sub_x[i];
			in.index = i;
		}
	cout << "local[" << rank << "] = " << in.value << " (" << in.index << ")\n";
	in.index = rank * num_elements_per_proc + in.index;
	// объединить локальные результаты и сохранить в процессе[root]
	MPI_Reduce(&in, &out, 1, MPI_FLOAT_INT, MPI_MINLOC, root, MPI_COMM_WORLD);
	if (rank == root) {
		float min_val = out.value;
		cout << "result: min[" << out.index << "] = " << min_val;
		if (check.value == min_val && check.index == out.index)
			cout << " (correct)";
		else
			cout << " (mistake)";
		cout << endl;
	}

	delete[] sub_x;
	sub_x = NULL;
	delete[] x;
	x = NULL;

	MPI_Finalize();

	return 0;
}
