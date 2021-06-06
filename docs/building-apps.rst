Building Open MPI applications
==============================

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

   shell$ mpicc hello_world_mpi.c -o hello_world_mpi -g
   shell$

For OpenSHMEM applications:

.. code-block:: sh

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

   shell$ gcc hello_world_mpi.c -o hello_world_mpi -g \
               `pkg-config ompi-c --cflags --libs`
   shell$

See :ref:`this FAQ item
<faq-building-mpi-apps-wrapper-compiler-alternatives-label>` for more
information on Open MPI's wrapper compilers and the use of
``pkg-config``.
