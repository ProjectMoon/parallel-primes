#ifndef P_PRIMES_H
#define P_PRIMES_H

#include <stdbool.h>

/*
 * pprimes.h - parallel primes header.
 *
 * pprimes.h contains the function prototypes for testing of prime numbers
 * and determining how to divide the prime data among the nodes of the
 * cluster.
 */

/*
 * chunkify - divide up the iterations for the number to be investigated.
 * The division is created based on the number of nodes available (nodes param).
 * chunks is an array of integers, with each value being the start of a new
 * iteration. The number returned is the length of the chunks array.
 * chunks should be passed in by address, i.e. have *chunks, pass in &chunks.
 *
 * num - the number to check for primeness
 * nodes - the number of compute nodes available
 * step - how many numbers to check on each chunk.
 * chunks - array of start numbers to distribute to workers.
 */
long pp_chunkify(long num, long nodes, long *step, long **chunks);

/*
 * create_pkgs - Create packages for delivery
 *
 * This function creates "packages" (or payloads) to deliver from master
 * to the workers. The package is an 1D array of three numbers:
 * - start number
 * - numbers to iterate over
 * - number to check for primality.
 *
 * Every 3 numbers represent one package that will get delivered to
 * a parallel process. The array is meant for use with the Scatter
 * function.
 */
void pp_create_pkgs(int nodes, long step, long num,
		    long** chunks_ptr, long** pkgs_ptr);

/*
 * free - free up a 1D dynamic array, e.g. chunks or pkgs array.
 * Array must be passed in by address.
 */
void pp_free_long(long** arr_ptr);
void pp_free_int(int** arr_ptr);

/*
 * prelim_check - perform some very basic, very fast checks to determine if the
 * given number is not a prime. includes: is number divisible by 2?
 */
bool pp_prelim_check(long num);

/*
 * isprime - determine if a number is a prime within the given chunk. The
 * function does simple iteration-modulus checking to determine if the number
 * is prime within this chunk. Note that this function returning true does
 * not mean the number is actually prime. It only means the number is prime
 * as far as this particular chunk of iteration data is concerned. The end
 * parameter is inclusive.
 */
bool pp_isprime(long num, long chunkStart, long chunkEnd);

/*
 * debug - block the process for debugging purposes. Puts the process into
 * infinite sleep. Attach using GDB and then break out of the loop by
 * changing the variable i to something non-zero. Step up from there.
 */
void pp_debug(void);
#endif
