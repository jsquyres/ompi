Examples
========

Compiling Open MPI applications
-------------------------------

Open MPI provides "wrapper" compilers that should be used for
compiling MPI and OpenSHMEM applications:

+---------+--------------------------+
| C       | ``mpicc``, ``oshcc``     |
+---------+--------------------------+
| C++     | ``mpiCC``, ``oshCC`` (or |
|         | ``mpic++`` if your       |
|         | filesystem is            |
|         | case-insensitive)        |
+---------+--------------------------+
| Fortran | ``mpifort``, ``oshfort`` |
+---------+--------------------------+

For example:

.. code-block:: sh
   :linenos:

   shell$ mpicc hello_world_mpi.c -o hello_world_mpi -g
   shell$

For OpenSHMEM applications:

.. code-block:: sh
   :linenos:

   shell$ oshcc hello_shmem.c -o hello_shmem -g
   shell$

All the wrapper compilers do is add a variety of compiler and linker
flags to the command line and then invoke a back-end compiler.  To be
specific: the wrapper compilers do not parse source code at all; they
are solely command-line manipulators, and have nothing to do with the
actual compilation or linking of programs.  The end result is an MPI
executable that is properly linked to all the relevant libraries.

.. caution:: It is *absolutely not sufficient* to simply add ``-lmpi``
             to your link line and assume that you will obtain a valid
             Open MPI executable.

Customizing the behavior of the wrapper compilers is possible (e.g.,
changing the compiler [not recommended] or specifying additional
compiler/linker flags); see :doc:`this section of the Open MPI FAQ
</faq/building-mpi-apps>` for more information.

Alternatively, Open MPI also installs ``pkg-config(1)`` configuration
files under ``$libdir/pkgconfig``.  If ``pkg-config`` is configured to find
these files, then compiling / linking Open MPI programs can be
performed like this:

.. code-block:: sh
   :linenos:

   shell$ gcc hello_world_mpi.c -o hello_world_mpi -g \
               `pkg-config ompi-c --cflags --libs`
   shell$

See :ref:`this FAQ item
<faq-building-mpi-apps-wrapper-compiler-alternatives-label>` for more
information on Open MPI's wrapper compilers and the use of
``pkg-config``.


Running Open MPI Applications
-----------------------------

Open MPI supports both ``mpirun`` and ``mpiexec`` (they are exactly
equivalent) to launch MPI applications.  For example:

.. code-block:: sh
   :linenos:

   shell$ mpirun -np 2 hello_world_mpi
   # or
   shell$ mpiexec -np 1 hello_world_mpi : -np 1 hello_world_mpi

are equivalent.

Note that the ``mpirun(1)`` command supports a *large* number of
options.  Be sure to see the ``mpirun(1)`` man page for much more
information.

The ``rsh`` launcher (which defaults to using ``ssh``) accepts a
``--hostfile`` parameter (the option ``--machinefile`` is equivalent); you
can specify a ``--hostfile`` parameter indicating a standard
``mpirun``-style hostfile (one hostname per line):

.. code-block:: sh
   :linenos:

   shell$ mpirun --hostfile my_hostfile -np 2 hello_world_mpi

If you intend to run more than one process on a node, the hostfile can
use the "slots" attribute.  If "slots" is not specified, a count of 1
is assumed.  For example, using the following hostfile:

.. code-block:: sh
   :linenos:

   shell$ cat my_hostfile
   node1.example.com
   node2.example.com
   node3.example.com slots=2
   node4.example.com slots=4
   shell$ mpirun --hostfile my_hostfile -np 8 hello_world_mpi

will launch ``MPI_COMM_WORLD`` rank 0 on node1, rank 1 on node2, ranks 2
and 3 on node3, and ranks 4 through 7 on node4.

Other starters, such as the resource manager / batch scheduling
environments, do not require hostfiles (and will ignore the hostfile
if it is supplied).  They will also launch as many processes as slots
have been allocated by the scheduler if no "-np" argument has been
provided.  For example, running a SLURM job with 8 processors:

.. code-block:: sh
   :linenos:

   shell$ salloc -n 8 mpirun a.out

The above command will reserve 8 processors and run 1 copy of mpirun,
which will, in turn, launch 8 copies of a.out in a single
``MPI_COMM_WORLD`` on the processors that were allocated by SLURM.

Note that the values of component parameters can be changed on the
``mpirun`` / ``mpiexec`` command line.  This is explained in the section
below, "The Modular Component Architecture (MCA)".

Open MPI supports ``oshrun`` to launch OpenSHMEM applications. For
example:

.. code-block:: sh
   :linenos:

   shell$ oshrun -np 2 hello_world_oshmem

OpenSHMEM applications may also be launched directly by resource
managers such as SLURM. For example, when OMPI is configured
``--with-pmix`` and ``--with-slurm``, one may launch OpenSHMEM
applications via ``srun``:

.. code-block:: sh
   :linenos:

   shell$ srun -N 2 hello_world_oshmem
