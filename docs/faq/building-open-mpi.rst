Building Open MPI
=================

.. JMS How can I create a TOC just for this page here at the top?

/////////////////////////////////////////////////////////////////////////

How do I build Open MPI?
------------------------

Many users can simply :doc:`read the Quick start guide </quickstart>`.

Beyond the Quick start document, note that the ``configure`` script
supports a *lot* of different command line options.  You may need to
specify configure options to tell Open MPI where to find support
software for specific network interconnects and back-end run-time
environments.  More generally, Open MPI supports a wide variety of
hardware and environments, but it sometimes needs to be told where
support libraries and header files are located.

Consult :doc:`the full Installation guide </installation>` and the
output of ``configure --help`` for specific instructions regarding
Open MPI's ``configure`` command line options.

/////////////////////////////////////////////////////////////////////////

Wow |mdash| I see a lot of errors during ``configure``.  Is that normal?
------------------------------------------------------------------------

If ``configure`` finishes successfully |mdash| meaning that it
generates a bunch of ``Makefiles`` at the end |mdash| then yes, it is
completely normal.

The Open MPI ``configure`` script tests for a lot of things, not all
of which are expected to succeed.  For example, if you do not have the
InfiniBand support libraries installed, you'll see failures about
trying to find the UCS library.  You'll also see errors and warnings
about various operating-system specific tests that are not aimed that
the operating system you are running.

These are all normal, expected, and nothing to be concerned about.  It
just means, for example, that Open MPI will not build UCX support.

/////////////////////////////////////////////////////////////////////////

What are the default build options for Open MPI?
------------------------------------------------

If you have obtained a developer's checkout from Git,
:doc:`you must consult these directions </developers>`.

The default options for building an Open MPI tarball are:

* Compile Open MPI with all optimizations enabled
* Build shared libraries
* Build components as standalone dynamic shared object (DSO) files
  (i.e., run-time plugins)
* Try to find support for all hardware and environments by looking
  for support libraries and header files in standard locations; skip
  them if not found

Open MPI's configure script has a large number of options, several of
which are of the form ``--with-FOO(=DIR)``, usually with a
corresponding ``--with-FOO-libdir=DIR`` option.  The ``(=DIR)`` part
means that specifying the directory is optional.

Consult :doc:`the full Installation guide </installation>` and the
output of ``configure --help`` for specific instructions regarding
Open MPI's ``configure`` command line options.

/////////////////////////////////////////////////////////////////////////

Open MPI was pre-installed on my machine; should I overwrite it with a new version?
-----------------------------------------------------------------------------------

Probably not.

Many systems come with some version of Open MPI either pre-installed
or available via the OS's packaging system (e.g., many Linuxes, BSD
variants, and MacOS).  If you download a newer version of Open MPI
from this web site, you probably do not want to *overwrite* the
system-installed Open MPI.  This is because the system-installed Open
MPI is typically under the control of some software package management
system (rpm, yum, etc.).

Instead, you probably want to install your new version of Open MPI to
another path, such as ``/opt/openmpi-version`` (or whatever is
appropriate for your system).

The :ref:`where to install
<faq-building-open-mpi-where-to-install-label>` FAQ question contains
some suggestions on where to install Open MPI.

/////////////////////////////////////////////////////////////////////////

.. _faq-building-open-mpi-where-to-install-label:

Where should I install Open MPI?
--------------------------------

A common environment to run Open MPI is in a "Beowulf"-class or
similar cluster (e.g., a bunch of 1U servers in a bunch of racks).
Simply stated, Open MPI can run on a group of servers or workstations
connected by a network.  As mentioned above, there are several
prerequisites, however (for example, you typically must have an
account on all the machines, you can ``ssh`` between the nodes without
using a password, etc.).

This raises the question for Open MPI system administrators: where to
install the Open MPI binaries, header files, etc.?  This discussion
mainly addresses this question for homogeneous clusters (i.e., where
all nodes and operating systems are the same), although elements of
this discussion apply to heterogeneous clusters as well.
Heterogeneous admins are encouraged to read this discussion and then
see the heterogeneous section of this FAQ.

There are two common approaches:

#. Have a common filesystem, such as NFS, between all the machines to
   be used.  Install Open MPI such that the installation directory is
   the *same value* on each node.  This will *greatly* simplify user's
   shell startup scripts (e.g., ``.bashrc``, ``.cshrc``, ``.profile``
   etc.)  |mdash| the ``PATH`` can be set without checking which
   machine the user is on.  It also simplifies the system
   administrator's job; when the time comes to patch or otherwise
   upgrade Open MPI, only one copy needs to be modified.

   For example, consider a cluster of four machines: ``inky``,
   ``blinky``, ``pinky``, and ``clyde``.

   * Install Open MPI on ``inky``'s local hard drive in the directory
     ``/opt/openmpi-VERSION``.  The system administrator then
     mounts ``inky:/opt/openmpi-VERSION`` on the remaining three
     machines, such that ``/opt/openmpi-VERSION`` on all machines
     is effectively "the same".  That is, the following directories
     all contain the Open MPI installation:

     .. code-block::

        inky:/opt/openmpi-VERSION
        blinky:/opt/openmpi-VERSION
        pinky:/opt/openmpi-VERSION
        clyde:/opt/openmpi-VERSION

   * Install Open MPI on ``inky``'s local hard drive in the directory
     ``/usr/local/openmpi-VERSION``.  The system administrator then
     mounts ``inky:/usr/local/openmpi-VERSION`` on *all four*
     machines in some other common location, such as
     ``/opt/openmpi-VERSION`` (a symbolic link can be installed on
     ``inky`` instead of a mount point for efficiency).  This strategy
     is typically used for environments where one tree is NFS
     exported, but another tree is typically used for the location of
     actual installation.  For example, the following directories all
     contain the Open MPI installation:

     .. code-block::

        inky:/opt/openmpi-VERSION
        blinky:/opt/openmpi-VERSION
        pinky:/opt/openmpi-VERSION
        clyde:/opt/openmpi-VERSION

     Notice that there are the same four directories as the previous
     example, but on ``inky``, the directory is *actually* located in
     ``/usr/local/openmpi-VERSION``.

   There is a bit of a disadvantage in this approach; each of the
   remote nodes have to incur NFS (or whatever filesystem is used)
   delays to access the Open MPI directory tree.  However, both the
   administration ease and low cost (relatively speaking) of using a
   networked file system usually greatly outweighs the cost.  Indeed,
   once an MPI application is past ``MPI_INIT``, it doesn't use the
   Open MPI binaries very much.

   .. note:: Open MPI, by default, uses a plugin system for loading
             functionality at run-time.  Most of Open MPI's plugins
             are opened during the call to ``MPI_INIT``.  This can
             cause a lot of filesystem traffic, which, if Open MPI is
             installed on a networked filesystem, may be noticable.

             Two common options to avoid this extra filesystem traffic
             are to build Open MPI to not use plugins (see :ref:`this
             FAQ entry for detail
             <faq-building-open-mpi-avoid-dso-label>`) or to install
             Open MPI locally (see below).

#. If you are concerned with networked filesystem costs of accessing
   the Open MPI binaries, you can install Open MPI on the local hard
   drive of each node in your system.  Again, it is *highly* advisable
   to install Open MPI in the *same* directory on each node so that
   each user's ``PATH`` can be set to the same value, regardless of
   the node that a user has logged on to.

   This approach will save some network latency of accessing the Open
   MPI binaries, but is typically only used where users are very
   concerned about squeezing every single cycle out of their machines,
   or are running at extreme scale where a networked filesystem may
   get overwhelmed by filesystem requests for Open MPI binaries when
   running very large parallel jobs.

/////////////////////////////////////////////////////////////////////////

.. _faq-building-open-mpi-install-overwrite:

Should I install a new version of Open MPI over an old version?
---------------------------------------------------------------

We do not recommend this.

Before discussing specifics, here are some definitions that are
necessary to understand:

* **Source tree:** The tree where the Open MPI source code is located.
  It is typically the result of expanding an Open MPI distribution
  source code bundle, such as a tarball.
* **Build tree:** The tree where Open MPI was built.  It is always
  related to a specific source tree, but may actually be a different
  tree (since Open MPI supports VPATH builds).  Specifically, this is
  the tree where you invoked ``configure``, ``make``, etc. to build
  and install Open MPI.
* **Installation tree:** The tree where Open MPI was installed.  It is
  typically the "prefix" argument given to Open MPI's ``configure``
  script; it is the directory from which you run installed Open MPI
  executables.

In its default configuration, an Open MPI installation consists of
several shared libraries, header files, executables, and plugins
(dynamic shared objects |mdash| DSOs).  These installation files act
together as a single entity.  The specific filenames and
contents of these files are subject to change between different
versions of Open MPI.

.. important:: Installing one version of Open MPI does not uninstall
               another version.

If you install a new version of Open MPI over an older version, this
may not remove or overwrite all the files from the older version.
Hence, you may end up with an incompatible muddle of files from two
different installations |mdash| which can cause problems.

The Open MPI team recommends one of the following methods for
upgrading your Open MPI installation:

* Install newer versions of Open MPI into a different directory. For
  example, install into ``/opt/openmpi-a.b.c`` and
  ``/opt/openmpi-x.y.z`` for versions a.b.c and x.y.z, respectively.
* Completely uninstall the old version of Open MPI before installing
  the new version.  The ``make uninstall`` process from Open MPI a.b.c
  build tree should completely uninstall that version from the
  installation tree, making it safe to install a new version (e.g.,
  version x.y.z) into the same installation tree.
* Remove the old installation directory entirely and then install the
  new version.  For example ``rm -rf /opt/openmpi`` *(assuming that
  there is nothing else of value in this tree!)* The installation of
  Open MPI x.y.z will safely re-create the ``/opt/openmpi`` tree.
  This method is preferable if you no longer have the source and build
  trees to Open MPI a.b.c available from which to ``make
  uninstall``.
* Go into the Open MPI a.b.c installation directory and manually
  remove all old Open MPI files.  Then install Open MPI x.y.z into the
  same installation directory.  This can be a somewhat painful,
  annoying, and error-prone process.  *We do not recommend it.*
  Indeed, if you no longer have access to the original Open MPI a.b.c
  source and build trees, it may be far simpler to download Open MPI
  version a.b.c again from the Open MPI web site, configure it with
  the same installation prefix, and then run ``make uninstall``.  Or
  use one of the other methods, above.

/////////////////////////////////////////////////////////////////////////

.. _faq-building-open-mpi-avoid-dso-label:

Can I disable Open MPI's use of plugins?
----------------------------------------

Yes.

Open MPI uses plugins for much of its functionality.  Specifically,
Open MPI looks for and loads plugins as dynamically shared objects
(DSOs) during the call to ``MPI_INIT``.  However, these plugins can be
compiled and installed in several different ways:

#. **As DSOs:** In this mode (the default), each of Open MPI's plugins
   are compiled as a separate DSO that is dynamically loaded at run
   time.

   * **Advantage:** this approach is highly flexible |mdash| it gives
     system developers and administrators fine-grained approach to
     install new plugins to an existing Open MPI installation, and
     also allows the removal of old plugins (i.e., forcibly
     disallowing the use of specific plugins) simply by removing the
     corresponding DSO(s).
   * **Disadvantage:** this approach causes additional filesystem
     traffic (mostly during ``MPI_INIT``).  If Open MPI is installed
     on a networked filesystem, this can cause noticeable network
     traffic when a large parallel job starts, for example.

#. **As part of a larger library:** In this mode, Open MPI "slurps up"
   the plugins and includes them in libmpi (and other libraries).
   Hence, *all* plugins are included in the main Open MPI libraries
   that are loaded by the system linker before an MPI process even
   starts.

   * **Advantage:** Significantly less filesystem traffic than the DSO
     approach.  This model can be much more performant on network
     installations of Open MPI.
   * **Disadvantage:** Much less flexible than the DSO approach;
     system administrators and developers have significantly less
     ability to add/remove plugins from the Open MPI installation at
     run-time.  Note that you still have *some* ability to add/remove
     plugins (see below), but there are limitations to what can be
     done.

To be clear: Open MPI's plugins can be built either as standalone DSOs
or included in Open MPI's main libraries (e.g., ``libmpi``).
Additionally, Open MPI's main libraries can be built either as static
or shared libraries.

You can therefore choose to build Open MPI in one of several different
ways:


#. ``--disable-mca-dso``: Using the ``--disable-mca-dso`` switch to
   Open MPI's ``configure`` script will cause all plugins to be built
   as part of Open MPI's main libraries |mdash| they will *not* be
   built as standalone DSOs.  However, Open MPI will still look for
   DSOs in the filesystem at run-time.  Specifically: this option
   *significantly* decreases (but does not eliminate) filesystem
   traffic during ``MPI_INIT``, but does allow the flexibility of
   adding new plugins to an existing Open MPI installation.

   Note that the ``--disable-mca-dso`` option does not affect whether
   Open MPI's main libraries are built as static or shared.

#. ``--enable-static``: Using this option to Open MPI's ``configure``
   script will cause the building of static libraries (e.g.,
   ``libmpi.a``).  This option automatically implies
   ``--disable-mca-dso``.

   Note that ``--enable-shared`` is also the default; so if you use
   ``--enable-static``, Open MPI will build *both* static and shared
   libraries that contain all of Open MPI's plugins (i.e.,
   ``libmpi.so`` and ``libmpi.a``).  If you want *only* static
   libraries (that contain all of Open MPI's plugins), be sure to
   *also* use ``--disable-shared``.

#. ``--disable-dlopen``: Using this option to Open MPI's ``configure``
   script will do two things:

   #. Imply ``--disable-mca-dso``, meaning that all plugins will be
      slurped into Open MPI's libraries.
   #. Cause Open MPI to not look for / open *any* DSOs at run time.


   Specifically: this option makes Open MPI not incur any additional
   filesystem traffic during ``MPI_INIT`.  Note that the
   ``--disable-dlopen`` option does not affect whether Open MPI's main
   libraries are built as static or shared.

/////////////////////////////////////////////////////////////////////////

How do I build an optimized version of Open MPI?
------------------------------------------------

If you have obtained a developer's checkout from Git
:doc:`you must consult these directions </developers>`.

Building Open MPI from an official distribution tarball defaults to
building an optimized version.  There is no need to do anything
special.

/////////////////////////////////////////////////////////////////////////

Are VPATH and/or parallel builds supported?
-------------------------------------------

Yes, both VPATH and parallel builds are supported.  This allows Open
MPI to be built in a different directory than where its source code
resides (helpful for multi-architecture builds).  Open MPI uses
Automake for its build system, so

For example:

.. code-block::
   :linenos:

   shell$ gtar zxf openmpi-1.2.3.tar.gz
   shell$ cd openmpi-1.2.3
   shell$ mkdir build
   shell$ cd build
   shell$ ../configure ...
   <... lots of output ...>
   shell$ make -j 4

Running ``configure`` from a different directory from where it
actually resides triggers the VPATH build (i.e., it will configure and
built itself from the directory where ``configure`` was run, not from
the directory where ``configure`` resides).

Some versions of ``make`` support parallel builds.  The example above
shows GNU make's ``-j`` option, which specifies how many compile
processes may be executing at any given time.  We, the Open MPI Team,
have found that doubling or quadrupling the number of processors in a
machine can *significantly* speed up an Open MPI compile (since
compiles tend to be much more IO bound than CPU bound).

/////////////////////////////////////////////////////////////////////////

Do I need any special tools to build Open MPI?
----------------------------------------------

If you are building Open MPI from an official distribution tarball,
you need a C compiler and ``make``.  If you are building the Fortran
MPI bindings, you will need a Fortran compiler as well.  You do *not*
need any special version of the GNU Autotools (Autoconf, Automake,
Libtool).

If you are building Open MPI from a Git checkout, you need some
additional tools.  :doc:`See the Developer's Guide </developers>` for
more details.

/////////////////////////////////////////////////////////////////////////

How do I build Open MPI as a static library?
--------------------------------------------

As noted above, Open MPI defaults to building shared libraries and
building components as dynamic shared objects (DSOs, i.e., run-time
plugins).  Changing this build behavior is controlled via command line
options to Open MPI's ``configure`` script.

**Building static libraries:** You can disable building shared libraries
and enable building static libraries with the following options:

.. code-block::
   :linenos:

   shell$ ./configure --enable-static --disable-shared ...

Similarly, you can build *both* static and shared libraries by simply
specifying ``--enable-static`` (and *not* specifying
``--disable-shared``), if desired.

**Including components in libraries:** Instead of building components
as DSOs, they can also be "rolled up" and included in their respective
libraries (e.g., ``libmpi``).  This is controlled with the
``--enable-mca-static`` option.  Some examples:

.. code-block::
   :linenos:

   shell$ ./configure --enable-mca-static=pml ...
   shell$ ./configure --enable-mca-static=pml,btl-openib,btl-self ...

Specifically, entire frameworks and/or individual components can be
specified to be rolled up into the library in a comma-separated list
as an argument to ``--enable-mca-static``.

/////////////////////////////////////////////////////////////////////////

When I run 'make', it looks very much like the build system is going into a loop.
---------------------------------------------------------------------------------

Open MPI uses the GNU Automake software to build itself.  Automake
uses a tightly-woven set of file timestamp-based dependencies to
compile and link software.  This behavior, frequently paired with
messages similar to:

.. code-block::
   :linenos:

   Warning: File `Makefile.am' has modification time 3.6e+04 s in the future

:ref:`See the warning about building Open MPI on a networked
filesystem <install-filesystem-timestamp-warning-label>` where the
time on the file server does not match the time on the client in the
Installation guide for more details.

/////////////////////////////////////////////////////////////////////////

.. _faq-building-open-mpi-sed-errors-label:

Configure issues warnings about sed and unterminated commands
-------------------------------------------------------------

Some users have reported seeing warnings like this in the
final output from ``configure``:

.. code-block::
   :linenos:

   *** Final output
   configure: creating ./config.status
   config.status: creating ompi/include/ompi/version.h
   sed: file ./confstatA1BhUF/subs-3.sed line 33: unterminated `s' command
   sed: file ./confstatA1BhUF/subs-4.sed line 4: unterminated `s' command
   config.status: creating orte/include/orte/version.h

These messages *usually* indicate a problem in the user's local shell
configuration.  Ensure that when you run a new shell, no output is
sent to stdout.  For example, if the output of this simple shell
script is more than just the hostname of your computer, you need to go
check your shell startup files to see where the extraneous output is
coming from (and eliminate it):

.. code-block:: sh
   :linenos:

   #!/bin/sh
   hostname
   exit 0

/////////////////////////////////////////////////////////////////////////

Open MPI configured ok, but I get ``Makefile:602: *** missing separator`` kinds of errors when building
-------------------------------------------------------------------------------------------------------

This is *usually* an indication that ``configure`` succeeded but
really shouldn't have.

:ref:`See this FAQ entry <faq-building-open-mpi-sed-errors-label>` for
one possible cause.

/////////////////////////////////////////////////////////////////////////

Open MPI seems to default to building with the GNU compiler set.  Can I use other compilers?
--------------------------------------------------------------------------------------------

Yes.

Open MPI uses a standard Autoconf ``configure`` script to probe the
current system and figure out how to build itself.  One of the choices
it makes it which compiler set to use.  Since Autoconf is a GNU
product, it defaults to the GNU compiler set.  However, this is easily
overridden on the ``configure`` command line.

:ref:`See the section and compilers and flags
<install-configure-compilers-and-flags-label>` in the Installation
guide for more details.

/////////////////////////////////////////////////////////////////////////

Can I pass specific flags to the compilers / linker used to build Open MPI?
---------------------------------------------------------------------------

Yes.

Open MPI uses a standard Autoconf ``configure`` script to set itself up
for building.  As such, there are a number of command line options
that can be passed to ``configure`` to customize flags that are passed
to the underlying compiler to build Open MPI.

:ref:`See the section and compilers and flags
<install-configure-compilers-and-flags-label>` in the Installation
guide for more details.

/////////////////////////////////////////////////////////////////////////

I'm trying to build with the Intel compilers, but Open MPI eventually fails to compile with really long error messages.  What do I do?
--------------------------------------------------------------------------------------------------------------------------------------

A common mistake when building Open MPI with the Intel compiler suite
is to accidentally specify the Intel C compiler as the C++ compiler.
Specifically, recent versions of the Intel compiler renamed the C++
compiler ``icpc`` (it used to be ``icc``, the same as the C compiler).
Users accustomed to the old name tend to specify ``icc`` as the C++
compiler, which will then cause a failure late in the Open MPI build
process because a C++ code will be compiled with the C compiler.  Bad
Things then happen.

The solution is to be sure to specify that the C++ compiler is
``icpc``, not ``icc``.  For example:

.. code-block::
   :linenos:

   shell$ ./configure CC=icc CXX=icpc F77=ifort FC=ifort ...

For Googling purposes, here's some of the error messages that may be
issued during the Open MPI compilation of C++ codes with the Intel C compiler
(``icc``), in no particular order:

.. code-block::
   :linenos:

   IPO Error: unresolved : _ZNSsD1Ev
   IPO Error: unresolved : _ZdlPv
   IPO Error: unresolved : _ZNKSs4sizeEv
   components.o(.text+0x17): In function `ompi_info::open_components()':
   : undefined reference to `std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string()'
   components.o(.text+0x64): In function `ompi_info::open_components()':
   : undefined reference to `std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string()'
   components.o(.text+0x70): In function `ompi_info::open_components()':
   : undefined reference to `std::string::size() const'
   components.o(.text+0x7d): In function `ompi_info::open_components()':
   : undefined reference to `std::string::reserve(unsigned int)'
   components.o(.text+0x8d): In function `ompi_info::open_components()':
   : undefined reference to `std::string::append(char const*, unsigned int)'
   components.o(.text+0x9a): In function `ompi_info::open_components()':
   : undefined reference to `std::string::append(std::string const&)'
   components.o(.text+0xaa): In function `ompi_info::open_components()':
   : undefined reference to `std::string::operator=(std::string const&)'
   components.o(.text+0xb3): In function `ompi_info::open_components()':
   : undefined reference to `std::basic_string<char, std::char_traits<char>, std::allocator<char> >::~basic_string()'

There are many more error messages, but the above should be sufficient
for someone trying to find this FAQ entry via a web crawler.

/////////////////////////////////////////////////////////////////////////

When I build with the Intel compiler suite, linking user MPI applications with the wrapper compilers results in warning messages. What do I do?
-----------------------------------------------------------------------------------------------------------------------------------------------

When Open MPI was built with some versions of the Intel compilers on
some platforms, you may see warnings similar to the following when
compiling MPI applications with Open MPI's wrapper compilers:

.. code-block::
   :linenos:

   shell$ mpicc hello.c -o hello
   libimf.so: warning: warning: feupdateenv is not implemented and will always fail
   shell$

This warning is generally harmless, but it can be alarming to some
users.  To remove this warning, pass either the ``-shared-intel`` or
``-i-dynamic`` options when linking your MPI application (the specific
option depends on your version of the Intel compilers; consult your
local documentation):

.. code-block::
   :linenos:

   shell$ mpicc hello.c -o hello -shared-intel
   shell$

You can also :ref:`change the default behavior of Open MPI's wrapper
compilers <install-wrapper-flags-label>` to automatically include this
``-shared-intel`` flag so that it is unnecessary to specify it on the
command line when linking MPI applications.

/////////////////////////////////////////////////////////////////////////

I'm trying to build with the IBM XL compilers, but Open MPI eventually fails to compile.  What do I do?
-------------------------------------------------------------------------------------------------------

Unfortunately there are some problems between Libtool (which Open MPI
uses for library support) and older versions of the IBM XL compilers
when creating shared libraries.  The only workaround is to disable
shared libraries and build Open MPI statically. For example:

.. code-block::
   :linenos:

   shell$ ./configure CC=xlc CXX=xlc++ FC=xlf90 --disable-shared --enable-static ...

For Googling purposes, here's an error message that may be
issued when the build fails:

.. code-block::
   :linenos:

   xlc: 1501-216 command option --whole-archive is not recognized - passed to ld
   xlc: 1501-216 command option --no-whole-archive is not recognized - passed to ld
   xlc: 1501-218 file libopen-pal.so.0 contains an incorrect file suffix
   xlc: 1501-228 input file libopen-pal.so.0 not found

/////////////////////////////////////////////////////////////////////////

How do I build Open MPI with support for my favorite network type?
------------------------------------------------------------------

To build support for high-speed interconnect networks, you generally
only have to specify the directory where its support header files and
libraries were installed to Open MPI's ``configure`` script.  You can
specify where multiple packages were installed if you have support for
more than one kind of interconnect |mdash| Open MPI will build support
for as many as it can.

:ref:`See the networking support section of the Installation
guide <install-network-support-label>` for information on how to
specify the correct options to ``configure``.

You can verify that ``configure`` found everything properly by examining
its output |mdash| it will test for each network's header files and
libraries and report whether it will build support (or not) for each
of them.  Examining ``configure``'s output is the *first* place you
should look if you have a problem with Open MPI not correctly
supporting a specific network type.

If ``configure`` indicates that support for your networks will be
included, after you build and install Open MPI, you can run the
``ompi_info`` command and look for components for your networks.

For example, the following shows that Open MPI was built with the
OpenFabrics Interconnect (OFI, also known as Libfabric) and UCX
support:

.. code-block::
   :linenos:

   shell$ ompi_info | egrep ': ofi|ucx'
                 MCA rml: ofi (MCA v2.1.0, API v3.0.0, Component v4.0.0)
                 MCA mtl: ofi (MCA v2.1.0, API v2.0.0, Component v4.0.0)
                 MCA pml: ucx (MCA v2.1.0, API v2.0.0, Component v4.0.0)
                 MCA osc: ucx (MCA v2.1.0, API v2.0.0, Component v4.0.0)

/////////////////////////////////////////////////////////////////////////

How do I build Open MPI with support for Slurm / XGrid?
-------------------------------------------------------

Slurm support is built automatically; there is nothing that
you need to do.

XGrid support is built automatically if the XGrid tools are installed.

/////////////////////////////////////////////////////////////////////////

How do I build Open MPI with support for SGE / OGE?
---------------------------------------------------

Support for the Sun Grid Engine (SGE) |mdash| later renamed to the
Oracle Gid Engine (OGE), and then forked into a few open source
dristributions |mdash| is available, but is not enabled by default.

You can explicitly request the SGE support with
the ``--with-sge`` command line switch to the Open MPI ``configure``
script. For example:

.. code-block::
   :linenos:

   shell$ ./configure --with-sge

After Open MPI is installed, you should see one component named
``gridengine``:

.. code-block::
   :linenos:

   shell$ ompi_info | grep gridengine
                 MCA ras: gridengine (MCA v2.0, API v2.0, Component v1.3)

/////////////////////////////////////////////////////////////////////////

How do I build Open MPI with support for PBS Pro / Open PBS / Torque?
---------------------------------------------------------------------

Support for PBS Pro, Open PBS, and Torque must be explicitly requested
with the ``--with-tm`` command line switch to Open MPI's ``configure``
script.

.. code-block::
   :linenos:

   shell$ ./configure --with-tm=/path/to/pbs-or-torque-install

After Open MPI is installed, you should see two components named
``tm``:

.. code-block::
   :linenos:

   shell$ ompi_info | grep tm
                 MCA pls: tm (MCA v1.0, API v1.0, Component v1.0)
                 MCA ras: tm (MCA v1.0, API v1.0, Component v1.0)

/////////////////////////////////////////////////////////////////////////

How do I build Open MPI with support for IBM Platform LSF?
----------------------------------------------------------

Note that only IBM Platform LSF 7.0.2 and later is supported.

Support for LSF will be automatically built if the LSF libraries and
headers are in the default path. If not, support must be explicitly
requested with the ``--with-lsf`` command line switch to Open MPI's
``configure`` script.

.. code-block::
   :linenos:

   shell$ ./configure --with-lsf=/path/to/lsf-install

After Open MPI is installed, you should see a component named
``lsf``:

.. code-block::
   :linenos:

   shell$ ompi_info | grep lsf
                 MCA ess: lsf (MCA v2.0, API v1.3, Component v1.3)
                 MCA ras: lsf (MCA v2.0, API v1.3, Component v1.3)
                 MCA plm: lsf (MCA v2.0, API v1.3, Component v1.3)

/////////////////////////////////////////////////////////////////////////

How do I build Open MPI with processor / memory affinity support?
-----------------------------------------------------------------

Open MPI automatically uses the `Hardware Locality (hwloc)
<https://www.open-mpi.org/projects/hwloc/>`_ library for processor and
memory affinity.

Just like other dependent libraries that Open MPI uses, ``configure``
will search for hwloc's headers and libraries.  If it does not find
them, Open MPI will use the embedded copy of hwloc that it carries in
its source tree.

Some systems support processor and memory affinity better than others;
consult hwloc's documentation for more details.

/////////////////////////////////////////////////////////////////////////

How do I build Open MPI with CUDA-aware support?
------------------------------------------------

CUDA-aware support means that the MPI library can send and receive GPU buffers
directly.

If you are using the UCX library for MPI networking transport, you
need to ensure that UCX was built with CUDA support (i.e., with the
NVIDIA ``gdrcopy`` library).  For example:

.. code-block::
   :linenos:

   shell$ ./configure \
       --prefix=/path/to/ucx-cuda-install \
       --with-cuda=/path/to/cuda-install \
       --with-gdrcopy=/path/to/gdrcopy-install
   shell$ make -j8 install

Then be sure that Open MPI's ``configure`` script finds this specific
UCX installation, potentiallt by specifying
``--with-ucx=/path/to/ucx-cuda-install``:

.. code-block::
   :linenos:

   shell$ ./configure \
       --with-cuda=/path/to/cuda-install \
       --with-ucx=/path/to/ucx-cuda-install ...
   shell$ make -j8 install

If you are not using UCX, Open MPI dynamically loads the
``libcuda.so`` library at run time |mdash| there is no need to specify
a path to it at ``configure`` time.  Specifically: all Open MPI needs
to build itself with CUDA support is the path to the ``cuda.h`` header
file.

The ``configure`` process is similar to above; you just don't specify
``--with-ucx``.  You can have ``configure`` search for ``cuda.h`` in
default locations:

.. code-block::
   :linenos:

   shell$ ./configure --with-cuda

Or you can tell ``configure`` where to find the specific ``cuda.h``
that you want it to use:

.. code-block::
   :linenos:

   shell$ ./configure --with-cuda=/usr/local/cuda-v6.0/cuda

Note that you cannot configure with ``--disable-dlopen`` as that will
break the ability of the Open MPI library to dynamically load ``libcuda.so``.

:doc:`See this FAQ section </faq/cuda>` for detals on how to use the
CUDA support.";

/////////////////////////////////////////////////////////////////////////

How do I <em>not</em> build a specific plugin / component for Open MPI?
-----------------------------------------------------------------------

The ``--enable-mca-no-build`` option to Open MPI's ``configure``
script enables you to specify a list of components that you want to
skip building.  This allows you to not include support for specific
features in Open MPI if you do not want to.

It takes a single argument: a comma-delimited list of
``framework-component`` pairs inidicating which specific components you do
not want to build.  For example:

.. code-block::
   :linenos:

   shell$ ./configure --enable-mca-no-build=threads-qthreads,pml-monitoring

Note that this option is really only useful for components that would
otherwise be built.  For example, if you are on a machine without
Libfabric support, it is not necessary to specify:

.. code-block::
   :linenos:

   shell$ ./configure --enable-mca-no-build=cm-ofi

because the ``configure`` script will naturally see that you do not have
support for GM and will automatically skip the ``gm`` BTL component.";

/////////////////////////////////////////////////////////////////////////

Does Open MPI support MPI_REAL16 and MPI_COMPLEX32?
---------------------------------------------------

It depends.  Note that these datatypes are optional in the MPI
standard.

Open MPI supports the ``MPI_REAL16`` and ``MPI_COMPLEX32`` types only
if:

* An integer C type can be found that has the same size (measured
  in bytes) as the Fortran ``REAL*16`` type.
* The bit representation is the same between the C type and the
  Fortran type.

/////////////////////////////////////////////////////////////////////////

Can I re-locate my Open MPI installation without re-configuring/re-compiling/re-installing from source?
-------------------------------------------------------------------------------------------------------

Yes.

.. note:: Open MPI hard-codes some directory paths in its executables
          based on installation paths specified by the ``configure``
          script.  For example, if you configure with an installation
          prefix of ``/opt/openmpi/``, Open MPI encodes in its
          executables that it should be able to find its help files in
          ``/opt/openmpi/share/openmpi``.

The "installdirs" functionality in Open MPI lets you change any of
these hard-coded directory paths at run time (*assuming* that you have
already adjusted your ``PATH`` and/or ``LD_LIBRARY_PATH`` environment
variables to the new location where Open MPI now resides).  There are
three methods:

#. Move an existing Open MPI installation to a new prefix: Set the
   ``OPAL_PREFIX`` environment variable before launching Open MPI.
   For example, if Open MPI had initially been installed to
   ``/opt/openmpi`` and the entire ``openmpi`` tree was later moved to
   ``/home/openmpi``, setting ``OPAL_PREFIX`` to ``/home/openmpi``
   will enable Open MPI to function properly.

#. "Stage" an Open MPI installation in a temporary location: When
   *creating* self-contained installation packages, systems such as
   RPM install Open MPI into temporary locations.  The package system
   then bundles up everything under the temporary location into a
   package that can be installed into its real location later.  For
   example, when *creating* an RPM that will be installed to
   ``/opt/openmpi``, the RPM system will transparently prepend a
   "destination directory" (or "destdir") to the installation
   directory.  As such, Open MPI will think that it is installed in
   ``/opt/openmpi``, but it is actually temporarily installed in (for
   example) ``/var/rpm/build.1234/opt/openmpi``.  If it is necessary
   to *use* Open MPI while it is installed in this staging area, the
   ``OPAL_DESTDIR`` environment variable can be used; setting
   ``OPAL_DESTDIR`` to ``/var/rpm/build.1234`` will automatically
   prefix every directory such that Open MPI can function properly.

#. Overriding individual directories: Open MPI uses the GNU-specified
   directories (per Autoconf/Automake), and can be overridden by
   setting environment variables directly related to their common
   names.  The list of environment variables that can be used is:

   * ``OPAL_PREFIX``
   * ``OPAL_EXEC_PREFIX``
   * ``OPAL_BINDIR``
   * ``OPAL_SBINDIR``
   * ``OPAL_LIBEXECDIR``
   * ``OPAL_DATAROOTDIR``
   * ``OPAL_DATADIR``
   * ``OPAL_SYSCONFDIR``
   * ``OPAL_SHAREDSTATEDIR``
   * ``OPAL_LOCALSTATEDIR``
   * ``OPAL_LIBDIR``
   * ``OPAL_INCLUDEDIR``
   * ``OPAL_INFODIR``
   * ``OPAL_MANDIR``
   * ``OPAL_PKGDATADIR``
   * ``OPAL_PKGLIBDIR``
   * ``OPAL_PKGINCLUDEDIR``

   Note that not all of the directories listed above are used by Open
   MPI; they are listed here in entirety for completeness.

   Also note that several directories listed above are defined in
   terms of other directories.  For example, the ``$bindir`` is
   defined by default as ``$prefix/bin``.  Hence, overriding the
   ``$prefix`` (via ``OPAL_PREFIX``) will automatically change the
   first part of the ``$bindir`` (which is how method 1 described
   above works).  Alternatively, ``OPAL_BINDIR`` can be set to an
   absolute value that ignores ``$prefix`` altogether.

/////////////////////////////////////////////////////////////////////////

How do I statically link to the libraries of Intel compiler suite?
------------------------------------------------------------------

The Intel compiler suite, by default, dynamically links its runtime libraries
against the Open MPI binaries and libraries. This can cause problems if the Intel
compiler libraries are installed in non-standard locations. For example, you might
get errors like:

.. code-block::
   :linenos:

   error while loading shared libraries: libimf.so: cannot open shared object file:
   No such file or directory

To avoid such problems, you can pass flags to Open MPI's configure
script that instruct the Intel compiler suite to statically link its
runtime libraries with Open MPI:

.. code-block::
   :linenos:

   shell$ ./configure CC=icc CXX=icpc FC=ifort LDFLAGS=-Wc,-static-intel ...

/////////////////////////////////////////////////////////////////////////

Why do I get errors about hwloc or libevent not found?
------------------------------------------------------

Sometimes you may see errors similar to the following when attempting to build Open MPI:

.. code-block::
   :linenos:

   ...
   PPFC     profile/pwin_unlock_f08.lo
   PPFC     profile/pwin_unlock_all_f08.lo
   PPFC     profile/pwin_wait_f08.lo
   FCLD     libmpi_usempif08.la
   ld: library not found for -lhwloc
   collect2: error: ld returned 1 exit status
   make``2``: *** ``libmpi_usempif08.la`` Error 1

This error can happen when a number of factors occur together:

#. If Open MPI's ``configure`` script chooses to use an "external"
   installation of `hwloc <https://www.open-mpi.org/projects/hwloc/>`_
   and/or `Libevent <https://libevent.org/>`_ (i.e., outside of Open
   MPI's source tree).
#. If Open MPI's ``configure`` script chooses C and Fortran compilers
   from different suites/installations.

Put simply: if the default search library search paths differ between
the C and Fortran compiler suites, the C linker may find a
system-installed ``libhwloc`` and/or ``libevent``, but the Fortran linker
may not.

This may tend to happen more frequently starting with Open MPI v4.0.0
on Mac OS because:

#. In v4.0.0, Open MPI's ``configure`` script was changed to "prefer"
   system-installed versions of hwloc and Libevent (vs. preferring the
   hwloc and Libevent that are bundled in the Open MPI distribution
   tarballs).
#. In MacOS, it is common for `Homebrew <https://brew.sh/>`_ or
   `MacPorts <https://www.macports.org/>`_ to install:
   * hwloc and/or Libevent
   * gcc and gfortran

For example, as of July 2019, Homebrew:

* Installs hwloc v2.0.4 under ``/usr/local``
* Installs the Gnu C and Fortran compiler suites v9.1.0 under
  ``/usr/local``.  *However*, the C compiler executable is named ``gcc-9``
  (not ``gcc``!), whereas the Fortran compiler executable is
  named ``gfortran``.

These factors, taken together, result in Open MPI's ``configure``
script deciding the following:

* The C compiler is ``gcc`` (which is the MacOS-installed C
  compiler).
* The Fortran compiler is ``gfortran`` (which is the
  Homebrew-installed Fortran compiler).
* There is a suitable system-installed hwloc in ``/usr/local``, which
  can be found -- by the C compiler/linker -- without specifying any
  additional linker search paths.

The careful reader will realize that the C and Fortran compilers are
from two entirely different installations.  Indeed, their default
library search paths are different:

* The MacOS-installed ``gcc`` will search ``/usr/local/lib`` by
  default.
* The Homebrew-installed ``gfortran`` will *not* search
  ``/usr/local/lib`` by default.

Hence, since the majority of Open MPI's source code base is in C, it
compiles/links against hwloc successfully.  But when Open MPI's
Fortran code for the ``mpi_f08`` module is compiled and linked, the
Homebrew-installed ``gfortran`` -- which does not search
``/usr/local/lib`` by default -- cannot find ``libhwloc``, and the link
fails.

There are a few different possible solutions to this issue:

#. The best solution is to always ensure that Open MPI uses a C and
   Fortran compiler from the same suite/installation.  This will
   ensure that both compilers/linkers will use the same default
   library search paths, and all behavior should be consistent.  For
   example, the following instructs Open MPI's ``configure`` script to
   use ``gcc-9`` for the C compiler, which (as of July 2019) is the
   Homebrew executable name for its installed C compiler:

   .. code-block:: sh
      :linenos:

      shell$ ./configure CC=gcc-9 ...

      # You can be precise and specify an absolute path for the C
      # compiler, and/or also specify the Fortran compiler:
      shell$ ./configure CC=/usr/local/bin/gcc-9 FC=/usr/local/bin/gfortran ...

   Note that this will likely cause ``configure`` to *not* find the
   Homebrew-installed hwloc, and instead fall back to using the
   bundled hwloc in the Open MPI source tree.

#. Alternatively, you can simply force ``configure`` to select the
   bundled versions of hwloc and libevent, which avoids the issue
   altogether:

   .. code-block:: sh
      :linenos:

      shell$ ./configure --with-hwloc=internal --with-libevent=internal ...

#. Finally, you can tell ``configure`` exactly where to find the
   external hwloc library.  This can have some unintended
   consequences, however, because it will prefix both the C and
   Fortran linker's default search paths with ``/usr/local/lib``:

   .. code-block:: sh
      :linenos:

      shell$ ./configure --with-hwloc-libdir=/usr/local/lib ...

Be sure to :ref:`see this section of the Installation guide
<install-misc-support-libraries-label>` and :ref:`this FAQ entry
<faq-building-open-mpi-intext-hwloc-libevent-label>` for more
information about the bundled hwloc and/or Libevent
vs. system-installed versions.

/////////////////////////////////////////////////////////////////////////

.. _faq-building-open-mpi-intext-hwloc-libevent-label:

Should I use the bundled hwloc and Libevent, or system-installed versions?
--------------------------------------------------------------------------

From a performance perspective, there is no significant reason
to choose the bundled vs. system-installed hwloc and Libevent
installations.  Specifically: both will likely give the same
performance.

There are other reasons to choose one or the other, however.

First, some background: Open MPI has internally used `hwloc
<https://www.open-mpi.org/projects/hwloc/>`_ and `Libevent
<https://libevent.org/>`_ for almost its entire life.  Years ago, it
was not common for hwloc and/or Libevent to be available on many
systems, so the Open MPI community decided to bundle entire copies of
the hwloc and Libevent source code in Open MPI distribution tarballs.

This system worked well: Open MPI used the bundled copies of hwloc and
Libevent which a) guaranteed that those packages would be available
(vs. telling users that they had to separately download/install those
packages before installing Open MPI), and b) guaranteed that the
versions of hwloc and Libevent were suitable for Open MPI's
requirements.

In the last few years, two things have changed:

#. hwloc and Libevent are now installed on many more systems by
   default.
#. The hwloc and Libevent APIs have stabilized such that a wide
   variety of hwloc/Libevent release versions are suitable for Open MPI's
   requirements.

While not *all* systems have hwloc and Libevent available by default
(cough cough MacOS cough cough), it is now common enough that -- with
the suggestion from Open MPI's downstream packagers -- starting with
v4.0.0, Open MPI "prefers" system-installed hwloc and Libevent
installations over its own bundled copies.

Meaning: if ``configure`` finds a suitable system-installed hwloc and/or
Libevent, ``configure`` will choose to use those installations instead of
the bundled copies in the Open MPI source tree.

That being said, there definitely are obscure technical corner cases
and philosophical reasons to force the choice of one or the other.  As
such, Open MPI provides ``configure`` command line options that can be
used to specify exact behavior in searching for hwloc and/or Libevent.

:ref:`This this section of the Installation guide
<install-misc-support-libraries-label>` discusses the various options
to ``configure`` that allow forcing the use of the internal / bundled
copies of hwloc and Libevent or the external / system-installed versions.

From Open MPI's perspective, it is always safe to use the bundled
copies.  If there is ever a problem or conflict with the
system-supplied Libevent or hwloc, you can specify
``--with-hwloc=internal`` and/or ``--with-libevent=internal``, and
this will likely solve your problem.

Additionally, note that Open MPI's ``configure`` will check some version
and functionality aspects from system-installed hwloc / Libevent, and
may still choose the bundled copies over system-installed copies
(e.g., the system-installed version is too low, the system-installed
version is not thread safe, ... etc.).
