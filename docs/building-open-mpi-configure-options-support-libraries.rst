.. This file is included by building-open-mpi.rst

Support libraries
^^^^^^^^^^^^^^^^^

The following are command line options for the support libraries that
are used by Open MPI that can be used with ``configure``:

* ``--with-libevent(=VALUE)``:
  This option specifies where to find the libevent support headers and
  library.  The following ``VALUE``\s are permitted:

  * ``internal``: Use Open MPI's internal copy of libevent.
  * ``external``: Use an external Libevent installation (rely on default
    compiler and linker paths to find it)
  * ``<no value>``: First, check for an external Libevent.  If not
    found, use the internal copy.
  * ``DIR``: Specify the location of a specific libevent
    installation to use

  libevent is a support library that provides event-based processing,
  timers, and signal handlers.  Open MPI requires libevent to build;
  passing --without-libevent will cause configure to abort.

* ``--with-libevent-libdir=DIR``:
  Look in directory for the libevent libraries.  This option is only
  usable when building Open MPI against an external libevent
  installation.  Just like other ``--with-FOO-libdir`` configure
  options, this option is only needed for special configurations.

* ``--with-hwloc(=VALUE)``: hwloc is a support library that provides
  processor and memory affinity information for NUMA platforms.  It is
  required by Open MPI.  Therefore, specifying ``--with-hwloc=no`` (or
  ``--without-hwloc``) is disallowed.  The following ``VALUE``\s are
  permitted:

  * ``internal``: Only use Open MPI's internal copy of hwloc.
  * ``external``: Only use an external hwloc installation (rely on
    default compiler and linker paths to find it).
  * ``<no value>``: First, check for an external hwloc.  If not
    found, use the internal copy.
  * ``DIR``: Only use the specific hwloc installation found in
    the specified directory.

* ``--with-hwloc-libdir=DIR``:
  Look in directory for the hwloc libraries.  This option is only
  usable when building Open MPI against an external hwloc
  installation.  Just like other ``--with-FOO-libdir`` configure options,
  this option is only needed for special configurations.

* ``--with-valgrind(=DIR)``:
  Directory where the valgrind software is installed.  If Open MPI
  finds Valgrind's header files, it will include additional support
  for Valgrind's memory-checking debugger.

  Specifically, it will eliminate a lot of false positives from
  running Valgrind on MPI applications.  There is a minor performance
  penalty for enabling this option.
