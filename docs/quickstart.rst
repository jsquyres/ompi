Quick start
===========

This chapter skips many details and offers examples that will probably
work in many environments.

.. caution:: Note that this is a "Quick start" -- it does not attempt
             to be comprehensive or describe how to install and run in
             all supported environments.  The examples below may
             therefore not work exactly as shown in your environment.

             Please consult the rest of this documentation for more
             details.

Installation
------------

In many cases, Open MPI can be built and installed by simply
indicating the installation directory on the command line:

.. code-block:: sh

   shell$ tar xf openmpi-<version>.tar.bz2
   shell$ cd openmpi-<version>
   shell$ ./configure --prefix=<path> |& tee config.out

   # ...lots of output...

   # Note that parallel builds are supported;
   # you can specify "-j N" on the "make" command line.
   shell$ make -j 8 2>&1 | tee make.out

   # ...lots of output...

   shell$ make install 2>&1 | tee install.out

   # ...lots of output...

Note that there are many, many configuration options to the
``./configure`` step.  Some of them may be needed for your particular
HPC network interconnect type and/or computing environmnet; see the
:doc:`building Open MPI section </building-open-mpi>` for desciptions
of the options available.

If your installation prefix path is not writable by a regular user,
you may need to use ``sudo`` or ``su`` to run the ``make install``
step.  For example:

.. code-block:: sh

   shell$ sudo make install 2>&1 | tee install.out
   [sudo] password for jsquyres: <enter your password here>

   # ...lots of output...

Finally, note that VPATH builds are fully supported.  For example:

.. code-block:: sh

   shell$ tar xf openmpi-<version>.tar.bz2
   shell$ cd openmpi-<version>
   shell$ mkdir build
   shell$ cd build
   shell$ ../configure --prefix=<path> 2>&1 | tee config.out
   # ...etc.


Compiling MPI applications
--------------------------

Open MPI provides "wrapper" compilers for C, C++, and Fortran that
automatically insert the correct command line flags to tell the
compiler and linker where Open MPI's header and library files are
located.  That is, the "wrapper" compilers simply insert a few command
line flags and then invoke the underlying compiler to actually compile
/ link your application.

You can therefore use ``mpicc``, ``mpic++``, and ``mpifort`` just as
you would use commands like ``gcc``, ``g++``, and ``gfortran`` (or
whatever your C, C++, and Fortran compilers are).

.. code-block:: sh

   shell$ mpicc mpi-hello-world.c -o hello
   shell$

There is no need to specify additional flags such as ``-lmpi``.


Running MPI applications
------------------------

Running in a non-scheduled environment
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In general, Open MPI requires the following to launch and run MPI
applications:

#. You must be able to login to remote nodes non-interactively (e.g.,
   without entering a password or passphrase).
#. Open MPI's executables must be findable (e.g., in your ``PATH``).
#. Open MPI's libraries must be findable (e.g., in your
   ``LD_LIBRARY_PATH``).

.. error:: JMS Provide links here to more comprehensive docs on
           running?

Assuming you are running in a non-scheduled environment (e.g., you are
not using a scheduler such as SLURM), construct a text "hostfile"
listing the nodes on which you wish to run::

   shell$ cat my-hostfile.txt
   node1.example.com
   node2.example.com
   node3.example.com
   node4.example.com

Further assuming that you have 10 processor cores on each of the
machines listed in your hostfile, you can use ``mpirun`` to launch 40
copies of your MPI application executable (``-np`` signifies the
number of processes)::

   shell$ mpirun -np 40 --hostfile my-hostfile.txt hello
   Hello world, I am 0 of 40 (running on node1.example.com)
   Hello world, I am 1 of 40 (running on node1.example.com)
   Hello world, I am 2 of 40 (running on node1.example.com)
   ...etc.
   Hello world, I am 39 of 40 (running on node4.example.com)

Running in a scheduled environment
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you're running in a scheduled environment (e.g., in a SLURM job, or
PBS Pro, or LSF, or any other scheduler), the ``-np`` and
``--hostfile`` options may be unnecessary.  For example, in a SLURM
job, the number of processes has already been specified (when
launching the SLURM job), and SLURM picks the hosts where the job will
run.  For example::

   shell$ cat my-slurm-script.sh
   #!/bin/sh
   mpirun hello

   shell$ sbatch -n 40 my-slurm-script.sh
   Submitted batch job 1234
   shell$

   # ... after job 1234 completes ...

   shell$ cat slurm-1234.out
   Hello world, I am 0 of 40 (running on node1.example.com)
   Hello world, I am 1 of 40 (running on node1.example.com)
   Hello world, I am 2 of 40 (running on node1.example.com)
   ...etc.
   Hello world, I am 39 of 40 (running on node4.example.com)
