#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "pprimes.h"
#include "ppmw.h"

#define mprintf(...) if (rank == 0) { printf(__VA_ARGS__); }

void ppmw_proc(long start, int rank, int size) {
  //master: handles communication between workers and prime detection.
  //printf("Starting at %ld\n", start);

  //discover primes up to the specified number.
  for (long num = start;; num++) {
    mprintf("Current number: %ld\n", num);

    //basic preliminary checks to determine if we can simply skip this num.
    if (!pp_prelim_check(num)) {
      MPI_Barrier(MPI_COMM_WORLD);
      continue;
    }

    long *chunks;
    long step;
    long len;

    if (rank == 0)
      len = pp_chunkify(num, size, &step, &chunks);

    //if more than one chunk, possibly time to parallelize.
    //yes, we must check the rest.
    //here, len should == # of nodes.
    //int rc;
    //0 = chunk start, 1 = step (iterate over that many numbers), 2 = num
    long* pkgs;
    long received[3];
    int* results;

    if (rank == 0) {
      pp_create_pkgs(size, rank, step, num, &chunks, &pkgs);
    }

    //rc = MPI_Send(package, 3, MPI_LONG, node, PRIME_CALC, MPI_COMM_WORLD);
    printf("[%d,%ld] scatter\n", rank, num);
    MPI_Scatter(pkgs, 3, MPI_LONG, received, 3, MPI_LONG, 0, MPI_COMM_WORLD);
    printf("[%d,%ld] received\n", rank, num);
    //package structure: 0 = num start, 1 = nums to iterate, 2 = num to check for prime
    printf("[%d,%ld] %ld, %ld, %ld\n", rank, num, received[2], received[0], received[0] + received[1]);
    bool prime = pp_isprime(received[2], received[0], received[0] + received[1]);
    int is_prime = (int)prime;
    printf("[%d,%ld] is prime done\n", rank, num);
    if (rank == 0) {
      results = malloc(size * sizeof(int));
    }

    printf("[%d,%ld] : gather\n", rank, num);
    MPI_Gather(&is_prime, 1, MPI_INT, results, size, MPI_INT, 0, MPI_COMM_WORLD);
    //MPI_Barrier(MPI_COMM_WORLD);

    printf("[%d,%ld]: gathered\n", rank, num);
    mprintf("hurrrr\n");
    if (rank == 0) {
      printf("entered master prime check\n");
      //is it a prime?
      for (int c = 0; c < size; c++) {
	if (prime == true && results[c] == 0) {
	  prime = false;
	  break;
	}
      }

      if (prime) {
	printf("%ld\n", num);
      }

      //free
      printf("freeing stuff\n");
      pp_free_int(&results);
      pp_free_long(&pkgs);
    }

    printf("[%d,%ld] onward\n", rank, num);
   
    //clean up for the next iteration
    if (rank == 0) pp_free_long(&chunks);
    MPI_Barrier(MPI_COMM_WORLD);
  }
}
