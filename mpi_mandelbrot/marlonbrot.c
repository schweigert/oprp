#include <complex>
#include <iostream>
#include <omp.h>
#include <mpi.h>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv){
	MPI_Init(&argc, &argv);

	int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if (argc <= 3) {
		cout << "Error!";
		exit(1);
	}

	int max_row, max_column, max_n;
	max_row = atoi(argv[1]);
	max_column = atoi(argv[2]);
	max_n = atoi(argv[3]);

	char **mat = (char**)malloc(sizeof(char*)*max_row);

	for (int i=0; i<max_row;i++) {
		mat[i]=(char*)malloc(sizeof(char)*max_column);
	}

	#pragma omp parallel for schedule(dynamic)
	for(int r = world_rank;r < max_row; ++r){
		if (r % world_size != world_rank) {
			continue;
		}
		#pragma omp parallel for schedule(dynamic)
		for(int c = 0; c < max_column; ++c){
			complex<float> z;
			int n = 0;
			while(abs(z) < 2 && ++n < max_n)
				z = (z * z) + decltype(z)(
					(float)c * 2 / max_column - 1.5,
					(float)r * 2 / max_row - 1
				);
			mat[r][c]=(n == max_n ? '#' : '.');
		}
	}

	if (world_rank == 0) {
		for(int r = world_rank;r < max_row; ++r){
			for(int c = 0; c < max_column; ++c){
				if (r % world_size == world_rank) {
					continue;
				}
				MPI_Recv(mat[r], max_column, MPI_CHAR, r % world_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
		for(int r = 0; r < max_row; ++r){
			for(int c = 0; c < max_column; ++c)
				std::cout << mat[r][c];
			cout << '\n';
		}
	} else {
		for(int r = world_rank;r < max_row; ++r){
			for(int c = 0; c < max_column; ++c){
				if (r % world_size != world_rank) {
					continue;
				}
				MPI_Send(mat[r], max_column, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			}
		}
	}

	MPI_Finalize();
}
