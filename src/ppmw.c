#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include "pprimes.h"
#include "ppmw.h"

#define mprintf(...) if (rank == 0) { printf(__VA_ARGS__); }

void ppmw_proc(long start, long end, int rank, int size) {
	//discover primes up to the specified number.
	for (long num = start; num <= end; num++) {
		long numToCheck = (long)sqrt(num) + 1;
		//basic preliminary checks to determine if we can simply skip this num.
		if (!pp_prelim_check(num)) {
			MPI_Barrier(MPI_COMM_WORLD);
			continue;
		}

		long *chunks;
		long step;
		long* pkgs;
		long received[3];
		int* results;

		//package structure:
		//0 = chunk start, 1 = step (iterate over that many numbers), 2 = num
		if (rank == 0) {
			pp_chunkify(numToCheck, size, &step, &chunks);
			pp_create_pkgs(size, step, num, &chunks, &pkgs);
		}

		///scatter the chunks to everyone to check subsets.
		MPI_Scatter(pkgs, 3, MPI_LONG, received, 3,
					MPI_LONG, 0, MPI_COMM_WORLD);

		long iterateUpTo = received[0] + received[1];
	
		bool prime = pp_isprime(received[2], received[0], iterateUpTo);
		int is_prime = (int)prime; //MPI does not like bool directly

		if (rank == 0) {
			results = malloc(size * sizeof(int));
		}
	
		MPI_Gather(&is_prime, 1, MPI_INT, results, 1,
				   MPI_INT, 0, MPI_COMM_WORLD);

		if (rank == 0) {
			//is it a prime? check result from each node.
			for (int c = 0; c < size; c++) {
				if (prime == true && results[c] == 0) {
					prime = false;
					break;
				}
			}

			//print to stdout if found prime
			if (prime) {
				printf("%ld\n", num);
			}
   
			//clean up for the next iteration
			pp_free_long(&chunks);
			pp_free_int(&results);
			pp_free_long(&pkgs);
		}
	
		MPI_Barrier(MPI_COMM_WORLD);
	}
}
