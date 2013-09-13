#ifndef PP_MW_H
#define PP_MW_H

/*
 * ppmw.h - parallel primes master/worker header
 *
 * This header file contains the prototypes, etc for the master and workers,
 * which do the actual MPI work and attempt to discover prime numbers.
 */

/*
 * ppmw_proc - master/worker functin
 *
 * The master/worker process is effectively the "main" of the program that takes
 * of distributing the chunks out and checking them for primality.
 */
void ppmw_proc(long start, int rank, int size);

#endif
