General notes
=============

The following abbreviated list of release notes applies to this code
base as of this writing (April 2020):

* Open MPI now includes two public software layers: MPI and OpenSHMEM.
  Throughout this document, references to Open MPI implicitly include
  both of these layers. When distinction between these two layers is
  necessary, we will reference them as the "MPI" and "OpenSHMEM"
  layers respectively.

* OpenSHMEM is a collaborative effort between academia, industry, and
  the U.S. Government to create a specification for a standardized API
  for parallel programming in the Partitioned Global Address Space
  (PGAS).  For more information about the OpenSHMEM project, including
  access to the current OpenSHMEM specification, please visit
  http://openshmem.org/.

  .. note:: This OpenSHMEM implementation will only work in Linux
            environments with a restricted set of supported networks.

* Open MPI includes support for a wide variety of supplemental
  hardware and software package.  When configuring Open MPI, you may
  need to supply additional flags to the ``configure`` script in order
  to tell Open MPI where the header files, libraries, and any other
  required files are located.  As such, running ``configure`` by itself
  may not include support for all the devices (etc.) that you expect,
  especially if their support headers / libraries are installed in
  non-standard locations.  Network interconnects are an easy example
  to discuss -- Libfabric and OpenFabrics networks, for example, both
  have supplemental headers and libraries that must be found before
  Open MPI can build support for them.  You must specify where these
  files are with the appropriate options to configure.  See the
  listing of configure command-line switches, below, for more details.

* The majority of Open MPI's documentation is here in this document.
  The man pages are also installed by default.

* Note that Open MPI documentation uses the word "component"
  frequently; the word "plugin" is probably more familiar to most
  users.  As such, end users can probably completely substitute the
  word "plugin" wherever you see "component" in our documentation.
  For what it's worth, we use the word "component" for historical
  reasons, mainly because it is part of our acronyms and internal API
  function calls.

* Open MPI has taken some steps towards `Reproducible Builds
  <https://reproducible-builds.org/>`_.  Specifically, Open MPI's
  ``configure`` and ``make`` process, by default, records the build date
  and some system-specific information such as the hostname where Open
  MPI was built and the username who built it.  If you desire a
  Reproducible Build, set the ``$SOURCE_DATE_EPOCH``, ``$USER`` and
  ``$HOSTNAME`` environment variables before invoking ``configure`` and
  ``make``, and Open MPI will use those values instead of invoking
  ``whoami`` and/or ``hostname``, respectively.  See
  https://reproducible-builds.org/docs/source-date-epoch/ for
  information on the expected format and content of the
  ``$SOURCE_DATE_EPOCH`` variable.


.. _platform-notes-section-label:

Platform Notes
--------------

.. error:: **JMS We should have a canonical list of:**

   *  *required* 3rd-party package versions supported (PRRTE, hwloc,
      libevent)
   * back-end run-time systems supported (behind PRRTE)
   * OS's and compilers supported
   * network interconnects supported.

* Systems that have been tested are:

  * Linux (various flavors/distros), 64 bit (x86, ppc, aarch64),
    with gcc (>=4.8.x+), clang (>=3.6.0), Absoft (fortran), Intel,
    and Portland (be sure to also see :ref:`the Compiler Notes
    section <compiler-notes-section-label>`)
  * macOS (10.14-10.15, 11.0), 64 bit (x86_64) with XCode compilers

* Other systems have been lightly (but not fully) tested:

  * Linux (various flavors/distros), 32 bit, with gcc
  * Cygwin 32 & 64 bit with gcc
  * ARMv6, ARMv7
  * Other 64 bit platforms.
  * OpenBSD.  Requires configure options ``--enable-mca-no-build=patcher``
    and ``--disable-dlopen`` with this release.
  * Problems have been reported when building Open MPI on FreeBSD 11.1
    using the clang-4.0 system compiler. A workaround is to build
    Open MPI using the GNU compiler.

* The run-time systems that are currently supported are:

  * ssh / rsh
  * PBS Pro, Torque
  * Platform LSF (tested with v9.1.1 and later)
  * SLURM
  * Cray XE, XC, and XK
  * Oracle Grid Engine (OGE) 6.1, 6.2 and open source Grid Engine


.. _compiler-notes-section-label:

Compiler Notes
--------------

* Open MPI requires a C99-capable compiler to build.

* On platforms other than x86-64, AArc64 (64-bit ARM), and PPC, Open
  MPI requires a compiler that either supports C11 atomics or the GCC
  ``__atomic`` atomics (e.g., GCC >= v4.7.2).

* 32-bit platforms are only supported with a recent compiler that
  supports C11 atomics. This includes GCC 4.9.x+ (although GCC 6.x or
  newer is recommened), the Intel compiler suite 16, and clang 3.1.

* Mixing compilers from different vendors when building Open MPI
  (e.g., using the C/C++ compiler from one vendor and the Fortran
  compiler from a different vendor) has been successfully employed by
  some Open MPI users (discussed on the Open MPI user's mailing list),
  but such configurations are not tested and not documented.  For
  example, such configurations may require additional compiler /
  linker flags to make Open MPI build properly.

  A not-uncommon case for this is when building on MacOS with the
  system-default GCC compiler (i.e., ``/usr/bin/gcc``), but a 3rd party
  gfortran (e.g., provided by Homebrew, in ``/usr/local/bin/gfortran``).
  Since these compilers are provided by different organizations, they
  have different default search paths.  For example, if Homebrew has
  also installed a local copy of Libevent (a 3rd party package that
  Open MPI requires), the MacOS-default ``gcc`` linker will find it
  without any additional command line flags, but the Homebrew-provided
  gfortran linker will not.  In this case, it may be necessary to
  provide the following on the configure command line:

  .. code-block:: sh
     :linenos:

     shell$ ./configure FCFLAGS=-L/usr/local/lib ...

  This ``-L`` flag will then be passed to the Fortran linker when
  creating Open MPI's Fortran libraries, and it will therefore be able
  to find the installed Libevent.

* In general, the latest versions of compilers of a given vendor's
  series have the least bugs.  We have seen cases where Vendor XYZ's
  compiler version A.B fails to compile Open MPI, but version A.C
  (where C>B) works just fine.  If you run into a compile failure, you
  might want to double check that you have the latest bug fixes and
  patches for your compiler.

* Users have reported issues with older versions of the Fortran PGI
  compiler suite when using Open MPI's (non-default) ``--enable-debug``
  configure option.  Per the above advice of using the most recent
  version of a compiler series, the Open MPI team recommends using the
  latest version of the PGI suite, and/or not using the ``--enable-debug``
  configure option.  If it helps, here's what we have found with some
  (not comprehensive) testing of various versions of the PGI compiler
  suite:

  * pgi-8 : NO known good version with ``--enable-debug``
  * pgi-9 : 9.0-4 known GOOD
  * pgi-10: 10.0-0 known GOOD
  * pgi-11: NO known good version with ``--enable-debug``
  * pgi-12: 12.10 known BAD with ``-m32``, but known GOOD without ``-m32``
    (and 12.8 and 12.9 both known BAD with ``--enable-debug``)
  * pgi-13: 13.9 known BAD with ``-m32``, 13.10 known GOOD without ``-m32``
  * pgi-15: 15.10 known BAD with ``-m32``

* Similarly, there is a known Fortran PGI compiler issue with long
  source directory path names that was resolved in 9.0-4 (9.0-3 is
  known to be broken in this regard).

* Open MPI does not support the PGI compiler suite on OS X or MacOS.
  See issues below for more details:
  * https://github.com/open-mpi/ompi/issues/2604
  * https://github.com/open-mpi/ompi/issues/2605

* OpenSHMEM Fortran bindings do not support the "no underscore"
  Fortran symbol convention. IBM's ``xlf`` compilers build in that mode
  by default.  As such, IBM's ``xlf`` compilers cannot build/link the
  OpenSHMEM Fortran bindings by default. A workaround is to pass
  ``FC="xlf -qextname"`` at configure time to force a trailing
  underscore. See https://github.com/open-mpi/ompi/issues/3612 for
  more details.

* MPI applications that use the ``mpi_f08`` module on PowerPC platforms
  (tested ppc64le) will likely experience runtime failures if:

   * they are using a GNU linker (ld) version after v2.25.1 and before
     v2.28,
     *and*
   * they compiled with PGI (tested 17.5) or XL (tested v15.1.5)
     compilers.  This was noticed on Ubuntu 16.04 which uses the
     2.26.1 version of ``ld`` by default. However, this issue impacts
     any OS using a version of ``ld`` noted above. This GNU linker
     regression will be fixed in version 2.28.  `Here is a link to the
     GNU bug on this issue
     <https://sourceware.org/bugzilla/show_bug.cgi?id=21306>`_.  The
     XL compiler will include a fix for this issue in a future
     release.

* On NetBSD-6 (at least AMD64 and i386), and possibly on OpenBSD,
  Libtool misidentifies properties of f95/g95, leading to obscure
  compile-time failures if used to build Open MPI.  You can work
  around this issue by ensuring that libtool will not use f95/g95
  (e.g., by specifying ``FC=<some_other_compiler>``, or otherwise ensuring
  a different Fortran compiler will be found earlier in the path than
  ``f95``/``g95``), or by disabling the Fortran MPI bindings with
  ``--disable-mpi-fortran``.

* On OpenBSD/i386, if you configure with
  ``--enable-mca-no-build=patcher``, you will also need to add
  ``--disable-dlopen``.  Otherwise, odd crashes can occur
  nondeterministically.

* Absoft 11.5.2 plus a service pack from September 2012 (which Absoft
  says is available upon request), or a version later than 11.5.2
  (e.g., 11.5.3), is required to compile the Fortran ``mpi_f08``
  module.

* Open MPI does not support the Sparc v8 CPU target.  However,
  as of Solaris Studio 12.1, and later compilers, one should not
  specify ``-xarch=v8plus`` or ``-xarch=v9``.  The use of the options
  ``-m32`` and ``-m64`` for producing 32 and 64 bit targets, respectively,
  are now preferred by the Solaris Studio compilers.  GCC may
  require either ``-m32`` or ``-mcpu=v9 -m32``, depending on GCC version.

* If one tries to build OMPI on Ubuntu with Solaris Studio using the C++
  compiler and the ``-m32`` option, you might see a warning:

  .. code-block::
     :linenos:

     CC: Warning: failed to detect system linker version, falling back to custom linker usage

  And the build will fail.  One can overcome this error by either
  setting ``LD_LIBRARY_PATH`` to the location of the 32 bit libraries
  (most likely /lib32), or giving ``LDFLAGS="-L/lib32 -R/lib32"`` to the
  ``configure`` command.  Officially, Solaris Studio is not supported on
  Ubuntu Linux distributions, so additional problems might be
  incurred.

* Open MPI does not support the ``gccfss`` compiler (GCC For SPARC
  Systems; a now-defunct compiler project from Sun).

* At least some versions of the Intel 8.1 compiler seg fault while
  compiling certain Open MPI source code files.  As such, it is not
  supported.

* It has been reported that the Intel 9.1 and 10.0 compilers fail to
  compile Open MPI on IA64 platforms.  As of 12 Sep 2012, there is
  very little (if any) testing performed on IA64 platforms (with any
  compiler).  Support is "best effort" for these platforms, but it is
  doubtful that any effort will be expended to fix the Intel 9.1 /
  10.0 compiler issuers on this platform.

* Early versions of the Intel 12.1 Linux compiler suite on x86_64 seem
  to have a bug that prevents Open MPI from working.  Symptoms
  including immediate segv of the wrapper compilers (e.g., ``mpicc``) and
  MPI applications.  As of 1 Feb 2012, if you upgrade to the latest
  version of the Intel 12.1 Linux compiler suite, the problem will go
  away.

* The Portland Group compilers prior to version 7.0 require the
  ``-Msignextend`` compiler flag to extend the sign bit when converting
  from a shorter to longer integer.  This is is different than other
  compilers (such as GNU).  When compiling Open MPI with the Portland
  compiler suite, the following flags should be passed to Open MPI's
  ``configure`` script:

  .. code-block:: sh
     :linenos:

     shell$ ./configure CFLAGS=-Msignextend CXXFLAGS=-Msignextend \
            --with-wrapper-cflags=-Msignextend \
            --with-wrapper-cxxflags=-Msignextend ...

  This will both compile Open MPI with the proper compile flags and
  also automatically add ``-Msignextend`` when the C and C++ MPI wrapper
  compilers are used to compile user MPI applications.

* It has been reported that Pathscale 5.0.5 and 6.0.527 compilers
  give an internal compiler error when trying to build Open MPI.

* As of July 2017, the Pathscale compiler suite apparently has no
  further commercial support, and it does not look like there will be
  further releases.  Any issues discovered regarding building /
  running Open MPI with the Pathscale compiler suite therefore may not
  be able to be resolved.

* Using the Absoft compiler to build the MPI Fortran bindings on Suse
  9.3 is known to fail due to a Libtool compatibility issue.

* MPI Fortran API support has been completely overhauled since the
  Open MPI v1.5/v1.6 series.

  There is now only a single Fortran MPI wrapper compiler and a
  single Fortran OpenSHMEM wrapper compiler: ``mpifort`` and ``oshfort``,
  respectively.  ``mpif77`` and ``mpif90`` still exist, but they are
  symbolic links to ``mpifort``.

  Similarly, Open MPI's ``configure`` script only recognizes the ``FC``
  and ``FCFLAGS`` environment variables (to specify the Fortran
  compiler and compiler flags, respectively).  The ``F77`` and ``FFLAGS``
  environment variables are **IGNORED**.

  .. important:: As a direct result, it is **STRONGLY** recommended
     that you specify a Fortran compiler that uses file suffixes to
     determine Fortran code layout (e.g., free form vs. fixed).  For
     example, with some versions of the IBM XLF compiler, it is
     preferable to use ``FC=xlf`` instead of ``FC=xlf90``, because
     ``xlf`` will automatically determine the difference between free
     form and fixed Fortran source code.

  However, many Fortran compilers allow specifying additional
  command-line arguments to indicate which Fortran dialect to use.
  For example, if ``FC=xlf90``, you may need to use ``mpifort --qfixed ...``
  to compile fixed format Fortran source files.

  You can use either ``ompi_info`` or ``oshmem_info`` to see with which
  Fortran compiler Open MPI was configured and compiled.

  There are up to three sets of Fortran MPI bindings that may be
  provided (depending on your Fortran compiler):

  #. ``mpif.h``: This is the first MPI Fortran interface that was
     defined in MPI-1.  It is a file that is included in Fortran
     source code.  Open MPI's ``mpif.h`` does not declare any MPI
     subroutines; they are all implicit.

  #. ``mpi`` module: The ``mpi`` module file was added in MPI-2.  It
     provides strong compile-time parameter type checking for MPI
     subroutines.

  #. ``mpi_f08`` module: The ``mpi_f08`` module was added in MPI-3.  It
     provides many advantages over the ``mpif.h`` file and ``mpi`` module.
     For example, MPI handles have distinct types (vs. all being
     integers).  See the MPI-3 document for more details.

  .. important:: The ``mpi_f08`` module is **STRONGLY** recommended
     for all new MPI Fortran subroutines and applications.  Note that
     the ``mpi_f08`` module can be used in conjunction with the other
     two Fortran MPI bindings in the same application (only one
     binding can be used per subroutine/function, however).  Full
     interoperability between ``mpif.h``/``mpi`` module and
     ``mpi_f08`` module MPI handle types is provided, allowing
     ``mpi_f08`` to be used in new subroutines in legacy MPI
     applications.

  Per the OpenSHMEM specification, there is only one Fortran OpenSHMEM
  binding provided:

  * ``shmem.fh``: All Fortran OpenSHMEM programs should include
    ``shmem.f``, and Fortran OpenSHMEM programs that use constants
    defined by OpenSHMEM **MUST** include ``shmem.fh``.

  The following notes apply to the above-listed Fortran bindings:

  * All Fortran compilers support the ``mpif.h``/``shmem.fh``-based
    bindings, with one exception: the ``MPI_SIZEOF`` interfaces will
    only be present when Open MPI is built with a Fortran compiler
    that supports the ``INTERFACE`` keyword and ``ISO_FORTRAN_ENV``.  Most
    notably, this excludes the GNU Fortran compiler suite before
    version 4.9.

  * The level of support provided by the ``mpi`` module is based on your
    Fortran compiler.

    If Open MPI is built with a non-GNU Fortran compiler, or if Open
    MPI is built with the GNU Fortran compiler >= v4.9, all MPI
    subroutines will be prototyped in the ``mpi`` module.  All calls to
    MPI subroutines will therefore have their parameter types checked
    at compile time.

    If Open MPI is built with an old ``gfortran`` (i.e., < v4.9), a
    limited ``mpi`` module will be built.  Due to the limitations of
    these compilers, and per guidance from the MPI-3 specification,
    all MPI subroutines with "choice" buffers are specifically *not*
    included in the ``mpi`` module, and their parameters will not be
    checked at compile time.  Specifically, all MPI subroutines with
    no "choice" buffers are prototyped and will receive strong
    parameter type checking at run-time (e.g., ``MPI_INIT``,
    ``MPI_COMM_RANK``, etc.).

    Similar to the ``mpif.h`` interface, ``MPI_SIZEOF`` is only supported
    on Fortran compilers that support ``INTERFACE`` and
    ``ISO_FORTRAN_ENV``.

  * The ``mpi_f08`` module has been tested with the Intel Fortran
    compiler and gfortran >= 4.9.  Other modern Fortran compilers
    likely also work.

    Many older Fortran compilers do not provide enough modern Fortran
    features to support the ``mpi_f08`` module.  For example, ``gfortran``
    < v4.9 does provide enough support for the ``mpi_f08`` module.

  You can examine the output of the following command to see all
  the Fortran features that are/are not enabled in your Open MPI
  installation:

  .. code-block:: sh
     :linenos:

     shell$ ompi_info | grep -i fort


General Run-Time Support Notes
------------------------------

* The Open MPI installation must be in your ``PATH`` on all nodes (and
  potentially ``LD_LIBRARY_PATH`` or ``DYLD_LIBRARY_PATH``, if
  ``libmpi``/``libshmem`` is a shared library), unless using the
  ``--prefix`` or ``--enable-mpirun-prefix-by-default`` functionality (see
  below).

* Open MPI's run-time behavior can be customized via Modular Component
  Architecture (MCA) parameters (see below for more information on how
  to get/set MCA parameter values).  Some MCA parameters can be set in
  a way that renders Open MPI inoperable (see notes about MCA
  parameters later in this file).  In particular, some parameters have
  required options that must be included.

  * If specified, the ``btl`` parameter must include the ``self``
    component, or Open MPI will not be able to deliver messages to the
    same rank as the sender.  For example: ``mpirun --mca btl tcp,self
    ...``
  * If specified, the ``btl_tcp_if_exclude`` parameter must include the
    loopback device (``lo`` on many Linux platforms), or Open MPI will
    not be able to route MPI messages using the TCP BTL.  For example:
    ``mpirun --mca btl_tcp_if_exclude lo,eth1 ...``

* Running on nodes with different endian and/or different datatype
  sizes within a single parallel job is supported in this release.
  However, Open MPI does not resize data when datatypes differ in size
  (for example, sending a 4 byte ``MPI_DOUBLE`` and receiving an 8 byte
  ``MPI_DOUBLE`` will fail).


MPI Functionality and Features
------------------------------

* All MPI-3.1 functionality is supported.

* Note that starting with Open MPI v4.0.0, prototypes for several
  legacy MPI-1 symbols that were deleted in the MPI-3.0 specification
  are no longer available by default in ``mpi.h``.  Specifically,
  several MPI-1 symbols were deprecated in the 1996 publishing of the
  MPI-2.0 specification.  These deprecated symbols were eventually
  removed from the MPI-3.0 specification in
  2012.

  The symbols that now no longer appear by default in Open MPI's
  ``mpi.h`` are:

  * ``MPI_Address`` (replaced by ``MPI_Get_address``)
  * ``MPI_Errhandler_create`` (replaced by ``MPI_Comm_create_errhandler``)
  * ``MPI_Errhandler_get`` (replaced by ``MPI_Comm_get_errhandler``)
  * ``MPI_Errhandler_set`` (replaced by ``MPI_Comm_set_errhandler``)
  * ``MPI_Type_extent`` (replaced by ``MPI_Type_get_extent``)
  * ``MPI_Type_hindexed`` (replaced by ``MPI_Type_create_hindexed``)
  * ``MPI_Type_hvector`` (replaced by ``MPI_Type_create_hvector``)
  * ``MPI_Type_lb`` (replaced by ``MPI_Type_get_extent``)
  * ``MPI_Type_struct`` (replaced by ``MPI_Type_create_struct``)
  * ``MPI_Type_ub`` (replaced by ``MPI_Type_get_extent``)
  * ``MPI_LB`` (replaced by ``MPI_Type_create_resized``)
  * ``MPI_UB`` (replaced by ``MPI_Type_create_resized``)
  * ``MPI_COMBINER_HINDEXED_INTEGER``
  * ``MPI_COMBINER_HVECTOR_INTEGER``
  * ``MPI_COMBINER_STRUCT_INTEGER``
  * ``MPI_Handler_function`` (replaced by ``MPI_Comm_errhandler_function``)

  Although these symbols are no longer prototyped in ``mpi.h``, they
  are still present in the MPI library in Open MPI |ompi_series|. This
  enables legacy MPI applications to link and run successfully with
  Open MPI |ompi_series|, even though they will fail to compile.

  .. warning:: Future releases of Open MPI beyond the |ompi_series|
     series may remove these symbols altogether.

  .. warning:: The Open MPI team **STRONGLY** encourages all MPI
     application developers to stop using these constructs that were
     first deprecated over 20 years ago, and finally removed from the
     MPI specification in MPI-3.0 (in 2012).

  .. important:: :doc:`The Open MPI FAQ </faq/removed-mpi-constructs>`
     contains examples of how to update legacy MPI applications using
     these deleted symbols to use the "new" symbols.

  All that being said, if you are unable to immediately update your
  application to stop using these legacy MPI-1 symbols, you can
  re-enable them in ``mpi.h`` by configuring Open MPI with the
  ``--enable-mpi1-compatibility`` flag.

* Rank reordering support is available using the TreeMatch library. It
  is activated for the graph and ``dist_graph`` communicator topologies.

* When using MPI deprecated functions, some compilers will emit
  warnings.  For example:

  .. code-block::
     :linenos:

     shell$ cat deprecated_example.c
     #include <mpi.h>
     void foo(void) {
         MPI_Datatype type;
         MPI_Type_struct(1, NULL, NULL, NULL, &type);
     }
     shell$ mpicc -c deprecated_example.c
     deprecated_example.c: In function 'foo':
     deprecated_example.c:4: warning: 'MPI_Type_struct' is deprecated (declared at /opt/openmpi/include/mpi.h:1522)
     shell$

* ``MPI_THREAD_MULTIPLE`` is supported with some exceptions.

  The following PMLs support ``MPI_THREAD_MULTIPLE``:

  #. ``cm``, when used with the following MTLs:

     #. ``ofi`` (Libfabric)
     #. ``portals4``

  #. ``ob1``, when used with the following BTLs:

     #. ``self``
     #. ``sm``
     #. ``smcuda``
     #. ``tcp``
     #. ``ugni``
     #. ``usnic``

  #. ``ucx``

  Currently, MPI File operations are not thread safe even if MPI is
  initialized for ``MPI_THREAD_MULTIPLE`` support.

* ``MPI_REAL16`` and ``MPI_COMPLEX32`` are only supported on platforms
  where a portable C datatype can be found that matches the Fortran
  type ``REAL*16``, both in size and bit representation.

* The "libompitrace" library is bundled in Open MPI and is installed
  by default (it can be disabled via the ``--disable-libompitrace``
  flag).  This library provides a simplistic tracing of select MPI
  function calls via the MPI profiling interface.  Linking it in to
  your application via (e.g., via ``-lompitrace``) will automatically
  output to stderr when some MPI functions are invoked:

  .. code-block::
     :linenos:

     shell$ cd examples/
     shell$ mpicc hello_c.c -o hello_c -lompitrace
     shell$ mpirun -np 1 hello_c
     MPI_INIT: argc 1
     Hello, world, I am 0 of 1
     MPI_BARRIER[0]: comm MPI_COMM_WORLD
     MPI_FINALIZE[0]
     shell$

  Keep in mind that the output from the trace library is going to
  ``stderr``, so it may output in a slightly different order than the
  ``stdout`` from your application.

  This library is being offered as a "proof of concept" / convenience
  from Open MPI.  If there is interest, it is trivially easy to extend
  it to printf for other MPI functions.  Pull requests on github.com
  would be greatly appreciated.


OpenSHMEM Functionality and Features
------------------------------------

All OpenSHMEM-1.3 functionality is supported.


MPI Collectives
---------------

* The ``cuda`` coll component provides CUDA-aware support for the
  reduction type collectives with GPU buffers. This component is only
  compiled into the library when the library has been configured with
  CUDA-aware support.  It intercepts calls to the reduction
  collectives, copies the data to staging buffers if GPU buffers, then
  calls underlying collectives to do the work.


OpenSHMEM Collectives
---------------------

* The ``fca`` scoll component: the Mellanox Fabric Collective
  Accelerator (FCA) is a solution for offloading collective operations
  from the MPI process onto Mellanox QDR InfiniBand switch CPUs and
  HCAs.

* The ``basic`` scoll component: Reference implementation of all
  OpenSHMEM collective operations.


Network Support
---------------

* There are several main MPI network models available: ``ob1``, ``cm``,
  and ``ucx``.  ``ob1`` uses BTL ("Byte Transfer Layer")
  components for each supported network.  ``cm`` uses MTL ("Matching
  Transport Layer") components for each supported network.  ``ucx`` uses
  the OpenUCX transport.

  * ``ob1`` supports a variety of networks that can be used in
    combination with each other:

    * OpenFabrics: InfiniBand, iWARP, and RoCE
    * Loopback (send-to-self)
    * Shared memory
    * TCP
    * SMCUDA
    * Cisco usNIC
    * uGNI (Cray Gemini, Aries)
    * shared memory (XPMEM, Linux CMA, Linux KNEM, and
      copy-in/copy-out shared memory)

  * ``cm`` supports a smaller number of networks (and they cannot be
    used together), but may provide better overall MPI performance:

    * Intel Omni-Path PSM2 (version 11.2.173 or later)
    * Intel True Scale PSM (QLogic InfiniPath)
    * OpenFabrics Interfaces ("libfabric" tag matching)
    * Portals 4

  * UCX is the `Unified Communication X (UCX) communication
    library <https://www.openucx.org/>`_.  This is an open-source
    project developed in collaboration between industry, laboratories,
    and academia to create an open-source production grade
    communication framework for data centric and high-performance
    applications.  The UCX library can be downloaded from repositories
    (e.g., Fedora/RedHat yum repositories).  The UCX library is also
    part of Mellanox OFED and Mellanox HPC-X binary distributions.

    UCX currently supports:

    * OpenFabrics Verbs (including InfiniBand and RoCE)
    * Cray's uGNI
    * TCP
    * Shared memory
    * NVIDIA CUDA drivers

  While users can manually select any of the above transports at run
  time, Open MPI will select a default transport as follows:

  #. If InfiniBand devices are available, use the UCX PML.
  #. If PSM, PSM2, or other tag-matching-supporting Libfabric
     transport devices are available (e.g., Cray uGNI), use the ``cm``
     PML and a single appropriate corresponding ``mtl`` module.
  #. Otherwise, use the ``ob1`` PML and one or more appropriate ``btl``
     modules.

  Users can override Open MPI's default selection algorithms and force
  the use of a specific transport if desired by setting the ``pml`` MCA
  parameter (and potentially the ``btl`` and/or ``mtl`` MCA parameters) at
  run-time:

  .. code-block:: sh
     :linenos:

     shell$ mpirun --mca pml ob1 --mca btl [comma-delimted-BTLs] ...
     # or
     shell$ mpirun --mca pml cm --mca mtl [MTL] ...
     # or
     shell$ mpirun --mca pml ucx ...

  There is a known issue when using UCX with very old Mellanox
  Infiniband HCAs, in particular HCAs preceding the introduction of
  the ConnectX product line, which can result in Open MPI crashing in
  MPI_Finalize.  This issue is addressed by UCX release 1.9.0 and
  newer.

* The main OpenSHMEM network model is ``ucx``; it interfaces directly
  with UCX.

* In prior versions of Open MPI, InfiniBand and RoCE support was
  provided through the ``openib`` BTL and ``ob1`` PML plugins.  Starting
  with Open MPI 4.0.0, InfiniBand support through the ``openib`` plugin
  is both deprecated and superseded by the ``ucx`` PML component.  The
  ``openib`` BTL was removed in Open MPI v5.0.0.

  While the ``openib`` BTL depended on ``libibverbs``, the UCX PML depends
  on the UCX library.

  Once installed, Open MPI can be built with UCX support by adding
  ``--with-ucx`` to the Open MPI configure command. Once Open MPI is
  configured to use UCX, the runtime will automatically select the
  ``ucx`` PML if one of the supported networks is detected (e.g.,
  InfiniBand).  It's possible to force using UCX in the ``mpirun`` or
  ``oshrun`` command lines by specifying any or all of the following mca
  parameters: ``--mca pml ucx`` for MPI point-to-point operations,
  ``--mca spml ucx`` for OpenSHMEM support, and ``--mca osc ucx`` for MPI
  RMA (one-sided) operations.

* The ``usnic`` BTL is support for Cisco's usNIC device ("userspace NIC")
  on Cisco UCS servers with the Virtualized Interface Card (VIC).
  Although the usNIC is accessed via the OpenFabrics Libfabric API
  stack, this BTL is specific to Cisco usNIC devices.

* uGNI is a Cray library for communicating over the Gemini and Aries
  interconnects.

* Linux ``knem`` support is used when the ``sm`` (shared memory) BTL is
  compiled with knem support (see the ``--with-knem`` configure option)
  and the ``knem`` Linux module is loaded in the running kernel.  If the
  ``knem`` Linux kernel module is not loaded, the ``knem`` support is (by
  default) silently deactivated during Open MPI jobs.

  See https://knem.gforge.inria.fr/ for details on Knem.

* Linux Cross-Memory Attach (CMA) or XPMEM is used by the ``sm`` shared
  memory BTL when the CMA/XPMEM libraries are installed,
  respectively.  Linux CMA and XPMEM are similar (but different)
  mechanisms for Open MPI to utilize single-copy semantics for shared
  memory.

* The OFI MTL does not support sending messages larger than the active
  Libfabric provider's ``max_msg_size``.  If you receive an error
  message about sending too large of a message when using the OFI MTL,
  please reach out to your networking vendor to ask them to support a
  larger ``max_msg_size`` for tagged messages.

Open MPI Extensions
-------------------

An MPI "extensions" framework is included in Open MPI, but is not
enabled by default.

:doc:`See the Open MPI API Extensions </extensions>` section for more
information on compiling and using MPI extensions.
