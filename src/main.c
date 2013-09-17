#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "ppmw.h"

int main(int argc, char **argv) {
	int size, rank;

	//get the number to start and end at.
	char *extra;
	long start = strtol(argv[1], &extra, 10);
	long end = strtol(argv[2], &extra, 10);
  
	MPI_Init(&argc, &argv);  
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
	ppmw_proc(start, end, rank, size);

	MPI_Finalize();
}



