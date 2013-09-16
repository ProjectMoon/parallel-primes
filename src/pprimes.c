#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include "pprimes.h"

long pp_chunkify(long num, long nodes, long *step, long **chunks) {
  long chunkSize = (long)floor((double)num / nodes);

  if (chunkSize < 1) {
    chunkSize = 1;
  }

  //only bother to parallelize when number is bigger than
  //the amount of nodes we have.
  if (num > nodes) {
    *chunks = malloc(nodes * sizeof(long));
    for (long c = 0; c < nodes; c++) {
      (*chunks)[c] = c * chunkSize;
    }

    *step = chunkSize;
    return nodes;
  }
  else {
    *chunks = malloc(1 * sizeof(long));
    (*chunks)[0] = num;
    *step = num;
    return 1;
  }
}

void pp_create_pkgs(int nodes, long step, long num,
		    long** chunks_ptr, long** pkgs_ptr) {
  long* chunks = *chunks_ptr;
  int node = 0;
  
  *pkgs_ptr = malloc(3 * nodes  * sizeof(long));

  for (int c = 0; c < 3 * nodes; c += 3) {
    (*pkgs_ptr)[c] = chunks[node]; //start num
    (*pkgs_ptr)[c + 1] = step; //how many to iterate over
    (*pkgs_ptr)[c + 2] = num; //num checking for primality
    node++;
  }
}

bool pp_prelim_check(long num) {
  if (num < 2) {
    return false;
  }
  
  if (num > 2 && num % 2 == 0) {
    return false;
  }

  return true;
}

bool pp_isprime(long num, long chunkStart, long chunkEnd) {
  if (num == 0 || num == 1) {
    return false;
  }

  if (num == 2) {
    return true;
  }
  
  for (long c = chunkStart; c <= chunkEnd; c++) {
    if (c > 0) {
      if ((c != 1 && c != num) && num % c == 0) {
	return false;
      }
    }
  }

  return true;
}

void pp_free_long(long** arr_ptr) {
  if (arr_ptr) {
    free(*arr_ptr);
    *arr_ptr = NULL;
  }
}

void pp_free_int(int** arr_ptr) {
  if (arr_ptr) {
    free(*arr_ptr);
    *arr_ptr = NULL;
  }
}

void pp_debug(void) {
  int i = 0;
  printf("process ready for debugging (%d)\n", (int)(getpid()));
  while (i == 0) {
    sleep(5);
  }
}
