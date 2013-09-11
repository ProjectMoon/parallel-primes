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
	for (int node = 1; node < size; node++) {
	  int rc;
	  //0 = chunk start, 1 = step (iterate over that many numbers), 2 = num
	  long package[3];
	  package[0] = chunks[node];
	  package[1] = step;
	  package[2] = num;
	  //rc = MPI_Send(package, 3, MPI_LONG, node, PRIME_CALC, MPI_COMM_WORLD);
	  rc = MPI_Bcast(package, 3, MPI_LONG, 0, MPI_COMM_WORLD);
	}

	//wait for workers to finish
	//this causes apparently random segfaults!!
	//MPI_Barrier(MPI_COMM_WORLD);

	//receive back all results. one false is enough to say num
	//is not prime.
	for (int src = 1; src < size; src++) {
	  int nodePrime, rc;
	  //MPI_Status stat;
	  //rc = MPI_Recv(&nodePrime, 1, MPI_INT, src, PRIME_CALC, MPI_COMM_WORLD, &stat);
	  rc = MPI_Bcast(&nodePrime, 1, MPI_INT, 0, MPI_COMM_WORLD);
	  
	  //only care if master didn't de-prime already.
	  if (prime == true && nodePrime != 1) {
	    prime = false;
	  }
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

void ppmw_worker(int rank, int size) {
  //the master node is always 0.
  const int MASTER = 0;
  
  while (true) { //change to check for shutdown message later.
    //MPI_Status stat;
    int rc;
    long start, step, num;
    long package[3];

    //receive incoming package
    //rc = MPI_Recv(package, 3, MPI_LONG, MASTER, PRIME_CALC, MPI_COMM_WORLD, &stat);
    rc = MPI_Bcast(package, 3, MPI_LONG, MASTER, MPI_COMM_WORLD);
    start = package[0];
    step = package[1];
    num = package[2];
    
    bool prime = pp_isprime(num, start, start + step);

    //wait for others to finish.
    //this causes apparently random segfaults!
    //MPI_Barrier(MPI_COMM_WORLD);

    //for some reason MPI does not like sending bool directly.
    //it appears to get translated as 0 when master takes it back.
    int i = (int)prime;
    MPI_Send(&i, 1, MPI_INT, MASTER, PRIME_CALC, MPI_COMM_WORLD);
  }
}
