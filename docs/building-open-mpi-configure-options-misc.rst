.. This file is included by building-open-mpi.rst

Miscellaneous Functionality
^^^^^^^^^^^^^^^^^^^^^^^^^^^

The following are command line options that don't fit any any of the
above categories that can be used with ``configure``:

* ``--without-memory-manager``:
  Disable building Open MPI's memory manager.  Open MPI's memory
  manager is usually built on Linux based platforms, and is generally
  only used for optimizations with some OpenFabrics-based networks (it
  is not *necessary* for OpenFabrics networks, but some performance
  loss may be observed without it).

  However, it may be necessary to disable the memory manager in order
  to build Open MPI statically.

* ``--with-ft=TYPE``:
  Specify the type of fault tolerance to enable.  Options: LAM
  (LAM/MPI-like), cr (Checkpoint/Restart).  Fault tolerance support is
  disabled unless this option is specified.

* ``--enable-peruse``:
  Enable the PERUSE MPI data analysis interface.

* ``--enable-heterogeneous``:
  Enable support for running on heterogeneous clusters (e.g., machines
  with different endian representations).  Heterogeneous support is
  disabled by default because it imposes a minor performance penalty.

  .. danger:: The heterogeneous functionality is currently broken --
              do not use.

.. _install-wrapper-flags-label:

* ``--with-wrapper-cflags=CFLAGS``
* ``--with-wrapper-cxxflags=CXXFLAGS``
* ``--with-wrapper-fcflags=FCFLAGS``
* ``--with-wrapper-ldflags=LDFLAGS``
* ``--with-wrapper-libs=LIBS``:
  Add the specified flags to the default flags that are used in Open
  MPI's "wrapper" compilers (e.g., ``mpicc`` -- see below for more
  information about Open MPI's wrapper compilers).  By default, Open
  MPI's wrapper compilers use the same compilers used to build Open
  MPI and specify a minimum set of additional flags that are necessary
  to compile/link MPI applications.  These configure options give
  system administrators the ability to embed additional flags in
  OMPI's wrapper compilers (which is a local policy decision).  The
  meanings of the different flags are:

  ``CFLAGS``: Flags passed by the ``mpicc`` wrapper to the C compiler
  ``CXXFLAGS``: Flags passed by the ``mpic++`` wrapper to the C++ compiler
  ``FCFLAGS``: Flags passed by the ``mpifort`` wrapper to the Fortran compiler
  ``LDFLAGS``: Flags passed by all the wrappers to the linker
  ``LIBS``: Flags passed by all the wrappers to the linker

  There are other ways to configure Open MPI's wrapper compiler
  behavior; see :doc:`the Open MPI FAQ </faq/index>` for more
  information.

There are many other options available -- see ``./configure --help``.
