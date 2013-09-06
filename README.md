Simple parallel number finder designed to run on the Jotunn cluster at the
University of Iceland. Written for assignment #1 of the HPC B course Fall 2013.
After assignment is done, updates to this are planned to further optimize it
as a learning exercise (and to hopefully collect a bunch of data for later
data mining use...).

To install (on Jotunn)
======================
Instructions for installing parallel-primes on the Jotunn cluster at the
University of Iceland:

1. Clone this repository.
2. Make a file called `user.in` in the root directory. The file must contain
   two lines. The first line is your Jotunn login username (usually same as
   your Ugla login name). The second line must contain the path you want to
   use for remote builds. See example below.
3. run `make` to compile the program on Jotunn. The makefile will sync files
   to Jotunn and build the program there.

Example user.in file
--------------------
If your name was `guy`, this might be what your `user.in` file looks like:

```
guy
/home/guy/parallel-primes/
```

Dependencies
------------
The following things are required for the makefile's remote build to work:

* make
* ssh
* rsync

Additionally, it is recommended that you configure your Jotunn login to use
key authentication. Otherwise you will continually have to type your password.
For public key authentication to work with Jotunn, your `.ssh` directory must
be permission `700` and the `authorized_keys` file permission `600`.

To run (on Jotunn)
==================
To run parallel-primes once it's been compiled on Jotunn:

1. Log in to Jotunn.
2. Go to the directory you specified in `user.in`.
3. To run without scheduling, use `mpirun`. The program takes one 
   argument: the number to start at.
4. To run with scheduling, wait until there's a job script in the repo.

To install/run (elsewhere)
==========================
If you find yourself outside the University of Iceland, follow these
instructions:

1. Find a supercomputer.
2. Follow install/run instructions as above, except change the hostname
   in the Makefile to where your supercomputer is located.

