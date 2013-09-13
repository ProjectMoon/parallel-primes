#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "ppmw.h"

int main(int argc, char **argv) {
  int size, rank;

  //get the number to start at.
  char *end;
  long start = strtol(argv[1], &end, 10);
  
  MPI_Init(&argc, &argv);  
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    ppmw_proc(start, rank, size);
  }

  MPI_Finalize();
}



