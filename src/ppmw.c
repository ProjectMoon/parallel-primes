#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "pprimes.h"
#include "ppmw.h"

//Message channels (aka MPI tags)
const int PRIME_CALC = 1; //communicate info about primes.

void ppmw_master(long start, int rank, int size) {
  //master: handles communication between workers and prime detection.
  //printf("Starting at %ld\n", start);

  //discover primes up to the specified number.
  for (long num = start;; num++) {
    //printf("Current number: %ld\n", num);

    //basic preliminary checks to determine if we can simply skip this num.
    if (!pp_prelim_check(num)) {
      continue;
    }

    long *chunks;
    long step;
    long len = pp_chunkify(num, size, &step, &chunks);

    //if more than one chunk, possibly time to parallelize.
    if (len > 1) {
      //solve the first chunk here to see if we even need to bother sending off.
      bool prime = pp_isprime(num, chunks[0], chunks[1] - 1);

      if (prime) {
	//yes, we must check the rest.
	//here, len should == # of nodes.
	//int rc;
	//0 = chunk start, 1 = step (iterate over that many numbers), 2 = num
	long* pkgs;
	long received[3];
	int* results = malloc(size * sizeof(int));
	ppmw_create_pkg(size, rank, step, num, &chunks, &pkgs);
	//rc = MPI_Send(package, 3, MPI_LONG, node, PRIME_CALC, MPI_COMM_WORLD); 
	MPI_Scatter(pkgs, 3, MPI_LONG, received, 3, MPI_LONG, 0, MPI_COMM_WORLD);

	//package structure: 0 = num start, 1 = nums to iterate, 2 = num to check for prime	
	bool prime = pp_isprime(received[2], received[0], received[0] + received[1]);
	int is_prime = (int)prime;
	MPI_Gather(&is_prime, 1, MPI_INT, results, size, MPI_INT, 0, MPI_COMM_WORLD);

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
	if (results) {
	  free(results);
	  results = NULL;
	}

	if (pkgs) {
	  free(pkgs);
	  pkgs = NULL;
	}
      }

      //finally, is it a prime?
      if (prime) {
	printf("%ld\n", num);
      }
    }
    else {
      //in this special case (1 chunk), the value from chunkify is what
      //we should go up to.
      bool prime = pp_isprime(num, 0, chunks[0]);
      if (prime) {
	printf("%ld\n", num);
      }
    }

    //clean up for the next iteration
    pp_free_chunks(&chunks);
  }
}

void ppmw_create_pkg(int nodes, int node, long step, long num, long** chunks_ptr, long** pkgs_ptr) {
  long* chunks = *chunks_ptr;
  long* pkgs = *pkgs_ptr;

  pkgs = malloc(3 * nodes  * sizeof(long));

  for (int c = 0; c < (3 * nodes) - 3; c++) {
    pkgs[c] = chunks[node]; //start num
    pkgs[c + 1] = step; //how many to iterate over
    pkgs[c + 3] = num; //num checking for primality
  }
}

void ppmw_worker(int rank, int size) {
 
}
