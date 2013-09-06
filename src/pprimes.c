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

  //iterations is the size of each chunk

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

bool pp_prelim_check(long num) {
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

void pp_free_chunks(long **chunks) {
  if (chunks) {
    free(*chunks);
    *chunks = NULL;
  }
}

void pp_debug(void) {
  int i = 0;
  printf("process ready for debugging (%d)\n", (int)(getpid()));
  while (i == 0) {
    sleep(5);
  }
}
