.. _building-and-installing-section-label:

Building and installing
=======================

.. note:: If you have checked out a *developer's copy* of Open MPI
   (i.e., you cloned from Git), you really need to read :doc:`the
   Developer's Guide </developers>` before attempting to build Open
   MPI. Really.

Open MPI uses a traditional ``configure`` script paired with ``make``
to build.  Typical installs can be of the pattern:

.. code-block:: sh
   :linenos:

   shell$ ./configure [...options...]
   shell$ make [-j N] all install
   # Use an integer value of N for parallel builds

There are many available ``configure`` options (see ``./configure --help``
for a full list); a summary of the more commonly used ones is included
below.

.. _install-filesystem-timestamp-warning-label:

.. warning:: If you are building Open MPI on a network filesystem, the
   machine you on which you are building *must* be time-synchronized
   with the file server.

   Specifically: Open MPI's build system *requires* accurate
   filesystem timestamps.  If your ``make`` output includes warning
   about timestamps in the future or runs GNU Automake, Autoconf,
   and/or Libtool, *this is not normal*, and you likely have an invalid
   build.

   You should remove the Open MPI source directory and start over
   (e.g., by re-extracting the Open MPI tarball): either switch to
   build on a local filesystem, or ensure that the time on your build
   machine is synchronized with the time on your file server before
   building again.

Note that ``configure`` will, by default, search for header files
and/or libraries for various optional features (e.g., various HPC
network API/support libraries).  If the relevant files are found, Open
MPI will built support for that feature.  If they are not found, Open
MPI will skip building support for that feature.

However, if you specify ``--with-FOO`` (where ``FOO`` is the
corresponding CLI option name for the feature) on the ``configure``
command line and Open MPI is unable to find relevant support for
``FOO``, ``configure`` will assume that it was unable to provide a
feature that was specifically requested and will abort so that a human
can resolve out the issue.

.. note:: Using ``--with-FOO`` to force Open MPI's ``configure``
          script to abort it if can't find support for a given feature
          may be preferable to unexpectedly discovering at run-time
          that Open MPI is missing support for a critical feature.

Additionally, if a search directory is specified for ``FOO`` in the
form ``--with-FOO=DIR``, Open MPI will:

#. Search for ``FOO``'s header files in ``DIR/include``.
#. Search for ``FOO``'s library files:

   #. If ``--with-FOO-libdir=<libdir>`` was specified, search in
      ``<libdir>``.
   #. Otherwise, search in ``DIR/lib``, and if they are not found
      there, search again in ``DIR/lib64``.

#. If both the relevant header files and libraries are found:

   #. Open MPI will build support for ``FOO``.
   #. If the root path where the FOO libraries are found is neither
      ``/usr`` nor ``/usr/local``, Open MPI will compile itself with
      RPATH flags pointing to the directory where ``FOO``'s libraries
      are located.  Open MPI does not RPATH ``/usr/lib[64]`` and
      ``/usr/local/lib[64]`` because many systems already search these
      directories for run-time libraries by default; adding RPATH for
      them could have unintended consequences for the search path
      ordering.

.. note:: The ``--with-FOO-libdir=DIR`` options are not usually
          needed; they are typically only needed when ``FOO``'s
          libraries are installed in an "unexpected" location.

          Also note the difference between ``--with-FOO=DIR`` and
          ``--with-FOO-subdir=DIR``: the former is a directory to
          which suffixes such as ``/include`` and ``/lib`` are added,
          whereas the latter is assumed to be a full library directory
          name (e.g., ``/opt/some_library/lib``).


Installation Options
--------------------

The following are general installation command line options that can
be used with ``configure``:

* ``--prefix=DIR``:
  Install Open MPI into the base directory named ``DIR``.  Hence, Open
  MPI will place its executables in ``DIR/bin``, its header files in
  ``DIR/include``, its libraries in ``DIR/lib``, etc.

* ``--disable-shared``:
  By default, Open MPI and OpenSHMEM build shared libraries, and all
  components are built as dynamic shared objects (DSOs). This switch
  disables this default; it is really only useful when used with
  ``--enable-static``.  Specifically, this option does *not* imply
  ``--enable-static``; enabling static libraries and disabling shared
  libraries are two independent options.

* ``--enable-static``:
  Build MPI and OpenSHMEM as static libraries, and statically link in
  all components.  Note that this option does *not* imply
  ``--disable-shared``; enabling static libraries and disabling shared
  libraries are two independent options.

  Be sure to read the description of ``--without-memory-manager``,
  below; it may have some effect on ``--enable-static``.

* ``--disable-wrapper-rpath``:
  By default, the wrapper compilers (e.g., ``mpicc``) will enable
  "rpath" support in generated executables on systems that support it.
  That is, they will include a file reference to the location of Open
  MPI's libraries in the application executable itself.  This means
  that the user does not have to set ``LD_LIBRARY_PATH`` to find Open
  MPI's libraries (e.g., if they are installed in a location that the
  run-time linker does not search by default).

  On systems that utilize the GNU ``ld`` linker, recent enough versions
  will actually utilize "runpath" functionality, not "rpath".  There
  is an important difference between the two:

  #. "rpath": the location of the Open MPI libraries is hard-coded into
     the MPI/OpenSHMEM application and cannot be overridden at
     run-time.
  #. "runpath": the location of the Open MPI libraries is hard-coded into
     the MPI/OpenSHMEM application, but can be overridden at run-time
     by setting the ``LD_LIBRARY_PATH`` environment variable.

  For example, consider that you install Open MPI vA.B.0 and
  compile/link your MPI/OpenSHMEM application against it.  Later, you
  install Open MPI vA.B.1 to a different installation prefix (e.g.,
  ``/opt/openmpi/A.B.1`` vs. ``/opt/openmpi/A.B.0``), and you leave the old
  installation intact.

  In the rpath case, your MPI application will always use the
  libraries from your A.B.0 installation.  In the runpath case, you
  can set the ``LD_LIBRARY_PATH`` environment variable to point to the
  A.B.1 installation, and then your MPI application will use those
  libraries.

  Note that in both cases, however, if you remove the original A.B.0
  installation and set ``LD_LIBRARY_PATH`` to point to the A.B.1
  installation, your application will use the A.B.1 libraries.

  This rpath/runpath behavior can be disabled via
  ``--disable-wrapper-rpath``.

  If you would like to keep the rpath option, but not enable runpath
  a different ``configure`` option is avalabile
  ``--disable-wrapper-runpath``.

* ``--enable-dlopen``:
  Build all of Open MPI's components as standalone Dynamic Shared
  Objects (DSO's) that are loaded at run-time (this is the default).
  The opposite of this option, ``--disable-dlopen``, causes two things:

  #. All of Open MPI's components will be built as part of Open MPI's
     normal libraries (e.g., ``libmpi``).
  #. Open MPI will not attempt to open any DSO's at run-time.

  Note that this option does *not* imply that OMPI's libraries will be
  built as static objects (e.g., ``libmpi.a``).  It only specifies the
  location of OMPI's components: standalone DSOs or folded into the
  Open MPI libraries.  You can control whether Open MPI's libraries
  are build as static or dynamic via ``--enable|disable-static`` and
  ``--enable|disable-shared``.

* ``--disable-show-load-errors-by-default``:
  Set the default value of the ``mca_base_component_show_load_errors``
  MCA variable: the ``--enable`` form of this option sets the MCA
  variable to true, the ``--disable`` form sets the MCA variable to
  false.  The MCA ``mca_base_component_show_load_errors`` variable can
  still be overridden at run time via the usual MCA-variable-setting
  mechanisms; this configure option simply sets the default value.

  The ``--disable`` form of this option is intended for Open MPI
  packagers who tend to enable support for many different types of
  networks and systems in their packages.  For example, consider a
  packager who includes support for both the FOO and BAR networks in
  their Open MPI package, both of which require support libraries
  (``libFOO.so`` and ``libBAR.so``).  If an end user only has BAR
  hardware, they likely only have ``libBAR.so`` available on their
  systems -- not ``libFOO.so``.  Disabling load errors by default will
  prevent the user from seeing potentially confusing warnings about
  the FOO components failing to load because ``libFOO.so`` is not
  available on their systems.

  Conversely, system administrators tend to build an Open MPI that is
  targeted at their specific environment, and contains few (if any)
  components that are not needed.  In such cases, they might want
  their users to be warned that the FOO network components failed to
  load (e.g., if ``libFOO.so`` was mistakenly unavailable), because Open
  MPI may otherwise silently failover to a slower network path for MPI
  traffic.

* ``--with-platform=FILE``:
  Load configure options for the build from ``FILE``.  Options on the
  command line that are not in ``FILE`` are also used.  Options on the
  command line and in ``FILE`` are replaced by what is in ``FILE``.

* ``--with-libmpi-name=STRING``:
  Replace ``libmpi.*`` and ``libmpi_FOO.*`` (where ``FOO`` is one of the
  fortran supporting libraries installed in lib) with ``libSTRING.*``
  and ``libSTRING_FOO.*``. This is provided as a convenience mechanism
  for third-party packagers of Open MPI that might want to rename
  these libraries for their own purposes. This option is *not*
  intended for typical users of Open MPI.

* ``--enable-mca-no-build=LIST``:
  Comma-separated list of ``<type>-<component>`` pairs that will not be
  built. For example, ``--enable-mca-no-build=btl-portals,oob-ud`` will
  disable building the portals BTL and the ud OOB component.


.. _install-network-support-label:

Networking support / options
----------------------------

The following are command line options for various network types that
can be used with ``configure``:

* ``--with-fca=DIR``:
  Specify the directory where the Mellanox FCA library and
  header files are located.

  FCA is the support library for Mellanox switches and HCAs.

* ``--with-hcoll=DIR``:
  Specify the directory where the Mellanox hcoll library and header
  files are located.  This option is generally only necessary if the
  hcoll headers and libraries are not in default compiler/linker
  search paths.

  hcoll is the support library for MPI collective operation offload on
  Mellanox ConnectX-3 HCAs (and later).

* ``--with-knem=DIR``:
  Specify the directory where the knem libraries and header files are
  located.  This option is generally only necessary if the knem headers
  and libraries are not in default compiler/linker search paths.

  knem is a Linux kernel module that allows direct process-to-process
  memory copies (optionally using hardware offload), potentially
  increasing bandwidth for large messages sent between messages on the
  same server.  See `the Knem web site
  <https://knem.gforge.inria.fr/>`_ for details.

* ``--with-libfabric=DIR``:
  Specify the directory where the OpenFabrics Interfaces ``libfabric``
  library and header files are located.  This option is generally only
  necessary if the libfabric headers and libraries are not in default
  compiler/linker search paths.

  Libfabric is the support library for OpenFabrics Interfaces-based
  network adapters, such as Cisco usNIC, Intel True Scale PSM, Cray
  uGNI, etc.

* ``--with-libfabric-libdir=DIR``:
  Look in directory for the libfabric libraries.  By default, Open MPI
  will look in ``DIR/lib`` and ``DIR/lib64``, which covers most cases.
  This option is only needed for special configurations.

* ``--with-portals4=DIR``:
  Specify the directory where the Portals4 libraries and header files
  are located.  This option is generally only necessary if the Portals4
  headers and libraries are not in default compiler/linker search
  paths.

  Portals is a low-level network API for high-performance networking
  on high-performance computing systems developed by Sandia National
  Laboratories, Intel Corporation, and the University of New Mexico.
  The Portals 4 Reference Implementation is a complete implementation
  of Portals 4, with transport over InfiniBand verbs and UDP.

* ``--with-portals4-libdir=DIR``:
  Location of libraries to link with for Portals4 support.

* ``--with-portals4-max-md-size=SIZE`` and
  ``--with-portals4-max-va-size=SIZE``:
  Set configuration values for Portals 4

* ``--with-psm=<directory>``:
  Specify the directory where the QLogic InfiniPath / Intel True Scale
  PSM library and header files are located.  This option is generally
  only necessary if the PSM headers and libraries are not in default
  compiler/linker search paths.

  PSM is the support library for QLogic InfiniPath and Intel TrueScale
  network adapters.

* ``--with-psm-libdir=DIR``:
  Look in directory for the PSM libraries.  By default, Open MPI will
  look in ``DIR/lib`` and ``DIR/lib64``, which covers most cases.  This
  option is only needed for special configurations.

* ``--with-psm2=DIR``:
  Specify the directory where the Intel Omni-Path PSM2 library and
  header files are located.  This option is generally only necessary
  if the PSM2 headers and libraries are not in default compiler/linker
  search paths.

  PSM is the support library for Intel Omni-Path network adapters.

* ``--with-psm2-libdir=DIR``:
  Look in directory for the PSM2 libraries.  By default, Open MPI will
  look in ``DIR/lib`` and ``DIR/lib64``, which covers most cases.  This
  option is only needed for special configurations.

* ``--with-ucx=DIR``:
  Specify the directory where the UCX libraries and header files are
  located.  This option is generally only necessary if the UCX headers
  and libraries are not in default compiler/linker search paths.

* ``--with-ucx-libdir=DIR``:
  Look in directory for the UCX libraries.  By default, Open MPI will
  look in ``DIR/lib`` and ``DIR/lib64``, which covers most cases.  This
  option is only needed for special configurations.

* ``--with-usnic``:
  Abort configure if Cisco usNIC support cannot be built.


Run-time system support
-----------------------

The following are command line options for various runtime systems that
can be used with ``configure``:

* ``--enable-mpirun-prefix-by-default``:
  This option forces the ``mpirun`` command to always behave as if
  ``--prefix $prefix`` was present on the command line (where ``$prefix``
  is the value given to the ``--prefix`` option to configure).  This
  prevents most ``rsh``/``ssh``-based users from needing to modify their
  shell startup files to set the ``PATH`` and/or ``LD_LIBRARY_PATH`` for
  Open MPI on remote nodes.  Note, however, that such users may still
  desire to set ``PATH`` -- perhaps even in their shell startup files --
  so that executables such as ``mpicc`` and ``mpirun`` can be found
  without needing to type long path names.

* ``--enable-orte-static-ports``:
   Enable ORTE static ports for TCP OOB (default: enabled).

* ``--with-alps``:
  Force the building of for the Cray Alps run-time environment.  If
  Alps support cannot be found, configure will abort.

* ``--with-lsf=DIR``:
  Specify the directory where the LSF libraries and header files are
  located.  This option is generally only necessary if the LSF headers
  and libraries are not in default compiler/linker search paths.

  LSF is a resource manager system, frequently used as a batch
  scheduler in HPC systems.

* ``--with-lsf-libdir=DIR``:
  Look in directory for the LSF libraries.  By default, Open MPI will
  look in ``DIR/lib`` and ``DIR/lib64``, which covers most cases.  This
  option is only needed for special configurations.

* ``--with-slurm``:
  Force the building of SLURM scheduler support.

* ``--with-sge``:
  Specify to build support for the Oracle Grid Engine (OGE) resource
  manager and/or the Open Grid Engine.  OGE support is disabled by
  default; this option must be specified to build OMPI's OGE support.

  The Oracle Grid Engine (OGE) and open Grid Engine packages are
  resource manager systems, frequently used as a batch scheduler in
  HPC systems.  It used to be called the "Sun Grid Engine", which is
  why the option is still named ``--with-sge``.

* ``--with-tm=DIR``:
  Specify the directory where the TM libraries and header files are
  located.  This option is generally only necessary if the TM headers
  and libraries are not in default compiler/linker search paths.

  TM is the support library for the Torque and PBS Pro resource
  manager systems, both of which are frequently used as a batch
  scheduler in HPC systems.

.. _install-misc-support-libraries-label:

Miscellaneous support libraries
-------------------------------

The following are command line options for miscellaneous support
libraries that are used by Open MPI that can be used with
``configure``:

* ``--with-libevent(=VALUE)``:
  This option specifies where to find the libevent support headers and
  library.  The following ``VALUE``\s are permitted:

  * ``internal``: Use Open MPI's internal copy of libevent.
  * ``external``: Use an external Libevent installation (rely on default
    compiler and linker paths to find it)
  * ``<no value>``:  Same as ``internal``.
  * ``DIR``: Specify the location of a specific libevent
    installation to use

  By default (or if ``--with-libevent`` is specified with no ``VALUE``),
  Open MPI will build and use the copy of libevent that it has in its
  source tree.  However, if the ``VALUE`` is ``external``, Open MPI will
  look for the relevant libevent header file and library in default
  compiler / linker locations.  Or, ``VALUE`` can be a directory tree
  where the libevent header file and library can be found.  This
  option allows operating systems to include Open MPI and use their
  default libevent installation instead of Open MPI's bundled
  libevent.

  libevent is a support library that provides event-based processing,
  timers, and signal handlers.  Open MPI requires libevent to build;
  passing --without-libevent will cause configure to abort.

* ``--with-libevent-libdir=DIR``:
  Look in directory for the libevent libraries.  This option is only
  usable when building Open MPI against an external libevent
  installation.  Just like other ``--with-FOO-libdir`` configure
  options, this option is only needed for special configurations.

* ``--with-hwloc(=VALUE)``:
  hwloc is a support library that provides processor and memory
  affinity information for NUMA platforms.  It is required by Open
  MPI.  Therefore, specifying ``--with-hwloc=no`` (or ``--without-hwloc``)
  is disallowed.

  By default (i.e., if ``--with-hwloc`` is not specified, or if
  ``--with-hwloc`` is specified without a value), Open MPI will first try
  to find/use an hwloc installation on the current system.  If Open
  MPI cannot find one, it will fall back to build and use the internal
  copy of hwloc included in the Open MPI source tree.

  Alternatively, the ``--with-hwloc`` option can be used to specify
  where to find the hwloc support headers and library.  The following
  ``VALUE``\s are permitted:

  * ``internal``: Only use Open MPI's internal copy of hwloc.
  * ``external``: Only use an external hwloc installation (rely on
    default compiler and linker paths to find it).
  * ``DIR``: Only use the specific hwloc installation found in
    the specified directory.

* ``--with-hwloc-libdir=DIR``:
  Look in directory for the hwloc libraries.  This option is only
  usable when building Open MPI against an external hwloc
  installation.  Just like other ``--with-FOO-libdir`` configure options,
  this option is only needed for special configurations.

* ``--disable-hwloc-pci``:
  Disable building hwloc's PCI device-sensing capabilities.  On some
  platforms (e.g., SusE 10 SP1, x86-64), the libpci support library is
  broken.  Open MPI's configure script should usually detect when
  libpci is not usable due to such brokenness and turn off PCI
  support, but there may be cases when configure mistakenly enables
  PCI support in the presence of a broken libpci.  These cases may
  result in ``make`` failing with warnings about relocation symbols in
  libpci.  The ``--disable-hwloc-pci`` switch can be used to force Open
  MPI to not build hwloc's PCI device-sensing capabilities in these
  cases.

  Similarly, if Open MPI incorrectly decides that libpci is broken,
  you can force Open MPI to build hwloc's PCI device-sensing
  capabilities by using ``--enable-hwloc-pci``.

  hwloc can discover PCI devices and locality, which can be useful for
  Open MPI in assigning message passing resources to MPI processes.

* ``--with-libltdl=DIR``:
  Specify the directory where the GNU Libtool libltdl libraries and
  header files are located.  This option is generally only necessary
  if the libltdl headers and libraries are not in default
  compiler/linker search paths.

  Note that this option is ignored if ``--disable-dlopen`` is specified.

* ``--disable-libompitrace``:
  Disable building the simple ``libompitrace`` library (see note above
  about libompitrace)

* ``--with-valgrind(=DIR)``:
  Directory where the valgrind software is installed.  If Open MPI
  finds Valgrind's header files, it will include additional support
  for Valgrind's memory-checking debugger.

  Specifically, it will eliminate a lot of false positives from
  running Valgrind on MPI applications.  There is a minor performance
  penalty for enabling this option.


MPI Functionality
-----------------

The following are command line options to set the default for various
MPI API behaviors that can be used with ``configure``:

* ``--with-mpi-param-check(=VALUE)``:
  Whether or not to check MPI function parameters for errors at
  runtime.  The following ``VALUE``\s are permitted:

  * ``always``: MPI function parameters are always checked for errors
  * ``never``: MPI function parameters are never checked for errors
  * ``runtime``: Whether MPI function parameters are checked depends on
    the value of the MCA parameter ``mpi_param_check`` (default: yes).
  * ``yes``: Synonym for "always" (same as ``--with-mpi-param-check``).
  * ``no``: Synonym for "never" (same as ``--without-mpi-param-check``).

  If ``--with-mpi-param`` is not specified, ``runtime`` is the default.

* ``--disable-mpi-thread-multiple``:
  Disable the MPI thread level ``MPI_THREAD_MULTIPLE`` (it is enabled by
  default).

* ``--enable-mpi-java``:
  Enable building of an **EXPERIMENTAL** Java MPI interface (disabled
  by default).  You may also need to specify ``--with-jdk-dir``,
  ``--with-jdk-bindir``, and/or ``--with-jdk-headers``.

  .. warning:: Note that this Java interface is **INCOMPLETE**
     (meaning: it does not support all MPI functionality) and **LIKELY
     TO CHANGE**.  The Open MPI developers would very much like to
     hear your feedback about this interface.

  :doc:`See the Java section </java>` for many more details.

* ``--enable-mpi-fortran(=VALUE)``:
  By default, Open MPI will attempt to build all 3 Fortran bindings:
  ``mpif.h``, the ``mpi`` module, and the ``mpi_f08`` module.  The following
  ``VALUE``\s are permitted:

  * ``all``: Synonym for ``yes``.
  * ``yes``: Attempt to build all 3 Fortran bindings; skip
    any binding that cannot be built (same as
    ``--enable-mpi-fortran``).
  * ``mpifh``: Only build ``mpif.h`` support.
  * ``usempi``: Only build ``mpif.h`` and ``mpi`` module support.
  * ``usempif08``:  Build ``mpif.h``, ``mpi`` module, and ``mpi_f08``
     module support.
  * ``none``: Synonym for ``no``.
  * ``no``: Do not build any MPI Fortran support (same as
    ``--disable-mpi-fortran``).  This is mutually exclusive
    with building the OpenSHMEM Fortran interface.

* ``--enable-mpi-ext(=LIST)``:
  Enable Open MPI's non-portable API extensions.  ``LIST`` is a
  comma-delmited list of extensions.  If no ``LIST`` is specified, all
  of the extensions are enabled.

  See the "Open MPI API Extensions" section for more details.

* ``--disable-mpi-io``:
  Disable built-in support for MPI-2 I/O, likely because an
  externally-provided MPI I/O package will be used. Default is to use
  the internal framework system that uses the ompio component and a
  specially modified version of ROMIO that fits inside the romio
  component

* ``--disable-io-romio``:
  Disable the ROMIO MPI-IO component

* ``--with-io-romio-flags=FLAGS``:
  Pass ``FLAGS`` to the ROMIO distribution configuration script.  This
  option is usually only necessary to pass
  parallel-filesystem-specific preprocessor/compiler/linker flags back
  to the ROMIO system.

* ``--disable-io-ompio``:
  Disable the ompio MPI-IO component

* ``--enable-sparse-groups``:
  Enable the usage of sparse groups. This would save memory
  significantly especially if you are creating large
  communicators. (Disabled by default)


OpenSHMEM Functionality
-----------------------

The following are command line options to set the default for various
OpenSHMEM API behaviors that can be used with ``configure``:

* ``--disable-oshmem``:
  Disable building the OpenSHMEM implementation (by default, it is
  enabled).

* ``--disable-oshmem-fortran``:
  Disable building only the Fortran OpenSHMEM bindings. Please see
  the "Compiler Notes" section herein which contains further
  details on known issues with various Fortran compilers.


Miscellaneous Functionality
---------------------------

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
  behavior; see `the Open MPI FAQ <https://www.open-mpi.org/faq/>`_
  for more information.

There are many other options available -- see ``./configure --help``.

.. _install-configure-compilers-and-flags-label:

Changing the compilers that Open MPI uses to build itself uses the
standard Autoconf mechanism of setting special environment variables
either before invoking configure or on the configure command line.
The following environment variables are recognized by configure:

* ``CC``: C compiler to use
* ``CFLAGS``: Compile flags to pass to the C compiler
* ``CPPFLAGS``: Preprocessor flags to pass to the C compiler
* ``CXX``: C++ compiler to use
* ``CXXFLAGS``: Compile flags to pass to the C++ compiler
* ``CXXCPPFLAGS``: Preprocessor flags to pass to the C++ compiler
* ``FC``: Fortran compiler to use
* ``FCFLAGS``: Compile flags to pass to the Fortran compiler
* ``LDFLAGS``: Linker flags to pass to all compilers
* ``LIBS``: Libraries to pass to all compilers (it is rarely
  necessary for users to need to specify additional ``LIBS``)
* ``PKG_CONFIG``: Path to the ``pkg-config`` utility

For example:

.. code-block:: sh
   :linenos:

   shell$ ./configure CC=mycc CXX=myc++ FC=myfortran ...

.. note:: We generally suggest using the above command line form for
   setting different compilers (vs. setting environment variables and
   then invoking ``./configure``).  The above form will save all
   variables and values in the ``config.log`` file, which makes
   post-mortem analysis easier if problems occur.

Note that the flags you specify must be compatible across all the
compilers.  In particular, flags specified to one language compiler
must generate code that can be compiled and linked against code that
is generated by the other language compilers.  For example, on a 64
bit system where the compiler default is to build 32 bit executables:

.. code-block:: sh
   :linenos:

   # Assuming the GNU compiler suite
   shell$ ./configure CFLAGS=-m64 ...

will produce 64 bit C objects, but 32 bit objects for Fortran.  These
codes will be incompatible with each other, and Open MPI will not build
successfully.  Instead, you must specify building 64 bit objects for
*all* languages:

.. code-block:: sh
   :linenos:

   # Assuming the GNU compiler suite
   shell$ ./configure CFLAGS=-m64 CXXFLAGS=-m64 FCFLAGS=-m64 ...

The above command line will pass ``-m64`` to all the compilers, and
therefore will produce 64 bit objects for all languages.

.. warning:: Note that setting ``CFLAGS`` (etc.) does *not* affect the
             flags used by the wrapper compilers.  In the above,
             example, you may also need to add ``-m64`` to various
             ``--with-wrapper-FOO`` options:

             .. code-block::
                :linenos:

                shell$ ./configure CFLAGS=-m64 CXXFLAGS=-m64 FCFLAGS=-m64 \
                   --with-wrapper-cflags=-m64 \
                   --with-wrapper-cxxflags=-m64 \
                   --with-wrapper-fcflags=-m64 ...

             Failure to do this will result in MPI applications
             failing to compile / link properly.

Note that if you intend to compile Open MPI with a ``make`` other than
the default one in your ``PATH``, then you must either set the ``$MAKE``
environment variable before invoking Open MPI's ``configure`` script, or
pass ``MAKE=your_make_prog`` to configure.  For example:

.. code-block:: sh
   :linenos:

   shell$ ./configure MAKE=/path/to/my/make ...

This could be the case, for instance, if you have a shell alias for
``make``, or you always type ``gmake`` out of habit.  Failure to tell
``configure`` which non-default ``make`` you will use to compile Open MPI
can result in undefined behavior (meaning: don't do that).

Note that you may also want to ensure that the value of
``LD_LIBRARY_PATH`` is set appropriately (or not at all) for your build
(or whatever environment variable is relevant for your operating
system).  For example, some users have been tripped up by setting to
use a non-default Fortran compiler via the ``FC`` environment variable,
but then failing to set ``LD_LIBRARY_PATH`` to include the directory
containing that non-default Fortran compiler's support libraries.
This causes Open MPI's ``configure`` script to fail when it tries to
compile / link / run simple Fortran programs.

It is required that the compilers specified be compile and link
compatible, meaning that object files created by one compiler must be
able to be linked with object files from the other compilers and
produce correctly functioning executables.

Open MPI supports all the ``make`` targets that are provided by GNU
Automake, such as:

* ``all``: build the entire Open MPI package
* ``install``: install Open MPI
* ``uninstall``: remove all traces of Open MPI from the installation tree
* ``clean``: clean out the build tree

Once Open MPI has been built and installed, it is safe to run ``make
clean`` and/or remove the entire build tree.

VPATH and parallel builds are fully supported.

Generally speaking, the only thing that users need to do to use Open
MPI is ensure that ``PREFIX/bin`` is in their ``PATH`` and
``PREFIX/lib`` is in their ``LD_LIBRARY_PATH``.  Users may need to
ensure to set the ``PATH`` and ``LD_LIBRARY_PATH`` in their shell
setup files (e.g., ``.bashrc``, ``.cshrc``) so that non-interactive
``ssh``-based logins will be able to find the Open MPI executables.
