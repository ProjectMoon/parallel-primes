#ifndef PP_MW_H
#define PP_MW_H

/*
 * ppmw.h - parallel primes master/worker header
 *
 * This header file contains the prototypes, etc for the master and workers,
 * which do the actual MPI work and attempt to discover prime numbers.
 */

/*
 * ppmw_master - master process function
 *
 * The master takes care of delegating out chunks to check, as well as checking
 * one chunk itself. It receives answers back from the workers and decides
 * whether or not the current number is a prime.
 */
void ppmw_master(long start, int rank, int size);

/*
 * ppmw_worker - worker process function
 *
 * The workers wait for a message from the master telling them to check a
 * set of numbers for primality. They then return the result (true/false)
 * back to the master.
 */
void ppmw_worker(int rank, int size);

void ppmw_create_pkg(int nodes, int node, long step, long num, long** chunks_ptr, long** pkgs_ptr);
#endif
