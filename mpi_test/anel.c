#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
	int rank, size;
	MPI_Int(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Status status;

	int count = rank;
	int tag = 0;

	if (rank == 0) {
		MPI_Send(count, 1 MPI_INT, (rank + 1) % size, tag, MPI_COMM_WORLD);
		MPI_Recv(&count, 1, MPI_INT, size - 1, tag, MPI_COMM_WORLD, &status);
	} else {
		MPI_Recv(&count, 1, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
		MPI_Send(count + rank, 1 MPI_INT, (rank + 1) % size, tag, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	printf("%d\n", count);
	return 0;
}
