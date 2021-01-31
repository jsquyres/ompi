Developer's guide
=================

This section is here for those who are building/exploring Open MPI in its
source code form, most likely through a developer's tree (i.e., a Git
clone).

Obtaining a Git clone
---------------------

Open MPI's Git repositories are hosted at GitHub.

#. First, you will need a Git client. We recommend getting the latest
   version available. If you do not have the command ``git`` in your
   path, you will likely need to download and install Git.
#. `ompi <https://github.com/open-mpi/ompi/>`_ is the main Open MPI
   repository where most active development is done.  Git clone this
   repository.  Note that the use of the ``--recursive`` CLI option is
   necessary because Open MPI uses Git submodules:

   .. code-block::
      :linenos:

      shell$ git clone --recursive https://github.com/open-mpi/ompi.git

Note that Git is natively capable of using many forms of web
proxies. If your network setup requires the user of a web proxy,
`consult the Git documentation for more details
<https://git-scm.com/>`_.

.. note:: Prior to October 2014, Open MPI was maintained in a
          Subversion repository. This Subversion repository had two
          read-only mirrors: a Mercurial mirror at bitbucket.org and a
          Git mirror at github.com. These two mirrors are now defunct
          and will no longer be updated.

          If you are using either of these mirrors, you should stop
          using them and switch to the main Open MPI Git repository at
          GitHub.


Prerequisites
-------------

Compilers
^^^^^^^^^

Although it should probably be assumed, you'll need a C/C++ compiler.

You'll also need a Fortran compiler if you want to build the Fortran MPI bindings, and a Java compiler if you want to build the (unofficial) Java MPI bindings.

GNU Autotools
^^^^^^^^^^^^^

When building Open MPI from its repository sources, the GNU Autotools
must be installed.

.. note:: The GNU Autotools are *not* required when building Open MPI
          from distribution tarballs.  Open MPI distribution tarballs
          are bootstrapped such that end-users do not need to have the
          GNU Autotools installed.

You can generally install GNU Autoconf, Automake, and Libtool via your
Linux distro native package system, or via Homebrew or MacPorts on
MacOS.  This usually "just works."

If you run into problems with the GNU Autotools, or need to download /
build them manually, :ref:`see the GNU Autotool section of the Open
MPI developer's docs <gnu-autotools-section-label>` for much more
detail on how to do this.


Flex
^^^^

Flex is used during the compilation of a developer's checkout (it is
not used to build official distribution tarballs).  Other flavors of
lex are *not* supported: given the choice of making parsing code
portable between all flavors of lex and doing more interesting work on
Open MPI, we greatly prefer the latter.

Note that no testing has been performed to see what the minimum
version of Flex is required by Open MPI.  We suggest that you use
v2.5.35 at the earliest.

For now, Open MPI will allow developer builds with Flex 2.5.4.  This
is primarily motivated by the fact that RedHat/Centos 5 ships with
Flex 2.5.4.  It is likely that someday Open MPI developer builds will
require Flex version >=2.5.35.

Note that the ``flex``-generated code generates some compiler warnings
on some platforms, but the warnings do not seem to be consistent or
uniform on all platforms, compilers, and flex versions.  As such, we
have done little to try to remove those warnings.

If you do not have Flex installed and cannot easily install it via
your operating system's packaging system (to include Homebrew or
MacPorts on MacOS), see `the Flex Github repository
<https://github.com/westes/flex>`_.


Pandoc
^^^^^^

.. error:: **JMS THIS MAY/WILL NEED TO CHANGE IF WE SWITCH TO SPHINX**

The Pandoc tool is used to generate Open MPI's man pages.
Specifically: Open MPI's man pages are written in Markdown; Pandoc is
the tool that converts that Markdown to nroff (i.e., the format of man
pages).

.. warning:: You must have Pandoc >=v1.12 when building Open MPI from
   a developer's tree.  If configure cannot find Pandoc >=v1.12, it
   will abort.

If you need to install Pandoc, check your operating system-provided
packages (to include MacOS Homebrew and MacPorts).  `The Pandoc
project web site <https://pandoc.org/>`_ itself also offers binaries
for their releases.


Sphinx
^^^^^^

.. error:: **JMS Need to write more here**

Sphinx...

* Installable via Python ``pip``
* https://www.sphinx-doc.org/


Compiler Pickyness by Default
-----------------------------

If you are building Open MPI from a Git clone (i.e., there is a
``.git`` directory in your build tree), the default build includes
extra compiler pickyness, which will result in more compiler warnings
than in non-developer builds.  Getting these extra compiler warnings
is helpful to Open MPI developers in making the code base as clean as
possible.

Developers can disable this picky-by-default behavior by using the
``--disable-picky`` configure option.  Also note that extra-picky
compiles do *not* happen automatically when you do a VPATH build
(e.g., if ``.git`` is in your source tree, but not in your build
tree).

Prior versions of Open MPI would automatically activate a lot of
(performance-reducing) debugging code by default if ``.git`` was found
in your build tree.  This is no longer true.  You can manually enable
these (performance-reducing) debugging features in the Open MPI code
base with these configure options:

* ``--enable-debug``
* ``--enable-mem-debug``
* ``--enable-mem-profile``

.. note:: These options are really only relevant to those who are
   developing Open MPI itself.  They are not generally helpful for
   debugging general MPI applications.


Running ``autogen.pl``
----------------------

You can now run OMPI's top-level ``autogen.pl`` script.  This script
will invoke the GNU Autoconf, Automake, and Libtool commands in the
proper order and setup to run OMPI's top-level ``configure`` script.

Running ``autogen.pl`` may take a few minutes, depending on your
system.  It's not very exciting to watch.  :smile:

If you have a multi-processor system, enabling the multi-threaded
behavior in Automake 1.11 (or newer) can result in ``autogen.pl``
running faster.  Do this by setting the ``AUTOMAKE_JOBS`` environment
variable to the number of processors (threads) that you want it to use
before invoking ``autogen``.pl.  For example (you can again put this
in your shell startup files):

.. code-block:: sh
   :linenos:

   # For bash/sh:
   export AUTOMAKE_JOBS=4
   # For csh/tcsh:
   set AUTOMAKE_JOBS 4

You generally need to run autogen.pl whenever the top-level file
``configure.ac`` changes, or any files in the ``config/`` or
``<project>/config/`` directories change (these directories are where
a lot of "include" files for Open MPI's ``configure`` script live).

You do *NOT* need to re-run ``autogen.pl`` if you modify a
``Makefile.am``.


Building Open MPI
-----------------

Once you have run ``autogen.pl`` successfully, you can configure and
build Open MPI just like end users do with official distribution Open
MPI tarballs.

:ref:`See the general "Install Open MPI" documentation for more
details. <building-and-installing-section-label>`


Open MPI terminology
--------------------

Open MPI is a large project containing many different
sub-systems and a relatively large code base.  Let's first cover some
fundamental terminology in order to make the rest of the discussion
easier.

Open MPI has multiple main sections of code:

* *OSHMEM:* The OpenSHMEM API and supporting logic
* *OMPI:* The MPI API and supporting logic
* *OPAL:* The Open Portable Access Layer (utility and "glue" code)

There are strict abstraction barriers in the code between these
sections.  That is, they are compiled into separate libraries:
``liboshmem``, ``libmpi``, ``libopal`` with a strict dependency order:
OSHMEM depends on OMPI, OMPI depends on OPAL.  For example, MPI
executables are linked with:

.. code-block:: sh
   :linenos:

   shell$ mpicc myapp.c -o myapp
   # This actually turns into:
   shell$ cc myapp.c -o myapp -lmpi -lopen-rte -lopen-pal ...

More system-level libraries may listed after ``-lopal``, but you get
the idea.

Strictly speaking, these are not "layers" in the classic software
engineering sense (even though it is convenient to refer to them as
such).  They are listed above in dependency order, but that does not
mean that, for example, the OMPI code must go through the
OPAL code in order to reach the operating system or a network
interface.

As such, this code organization more reflects abstractions and
software engineering, not a strict hierarchy of functions that must be
traversed in order to reach a lower layer.  For example, OMPI can
directly call the operating system as necessary (and not go through
OPAL).  Indeed, many top-level MPI API functions are quite performance
sensitive; it would not make sense to force them to traverse an
arbitrarily deep call stack just to move some bytes across a network.

Note that Open MPI also uses some third-party libraries for core
functionality:

* PMIx
* PRRTE
* Libevent
* Hardware Locality ("hwloc")

These will be discussed elsewhere.

Here's a list of terms that are frequently used in discussions about
the Open MPI code base:

* *MCA:* The Modular Component Architecture (MCA) is the foundation
  upon which the entire Open MPI project is built.  It provides all the
  component architecture services that the rest of the system uses.
  Although it is the fundamental heart of the system, its
  implementation is actually quite small and lightweight |mdash| it is
  nothing like CORBA, COM, JINI, or many other well-known component
  architectures.  It was designed for HPC |mdash| meaning that it is small,
  fast, and reasonably efficient |mdash| and therefore offers few services
  other than finding, loading, and unloading components.

* *Framework:* An MCA _framework_ is a construct that is created
  for a single, targeted purpose.  It provides a public interface that
  is used by external code, but it also has its own internal services.
  :ref:`See the list of Open MPI frameworks in this version of Open MPI
  <internal-frameworks-section-label>`.  An MCA
  framework uses the MCA's services to find and load _components_ at run-time
  |mdash| implementations of the framework's interface.  An easy example
  framework to discuss is the MPI framework named ``btl``, or the Byte
  Transfer Layer.  It is used to send and receive data on different
  kinds of networks.  Hence, Open MPI has ``btl`` components for shared
  memory, InfiniBand, various protocols over Ethernet, etc.

* *Component:* An MCA _component_ is an implementation of a
  framework's interface.  Another common word for component is
  "plugin". It is a standalone collection of code that can be bundled
  into a plugin that can be inserted into the Open MPI code base, either
  at run-time and/or compile-time.

* *Module:* An MCA _module_ is an instance of a component (in the
  C++ sense of the word "instance"; an MCA component is analogous to a
  C++ class). For example, if a node running an Open MPI application has
  multiple ethernet NICs, the Open MPI application will contain one TCP
  ``btl`` component, but two TCP ``btl`` modules.  This difference between
  components and modules is important because modules have private state;
  components do not.

Frameworks, components, and modules can be dynamic or static. That is,
they can be available as plugins or they may be compiled statically
into libraries (e.g., ``libmpi``).



Source code tree layout
-----------------------

There are a few notable top-level directories in the source
tree:

* The main sub-projects:
    * ``oshmem``: Top-level OpenSHMEM code base
    * ``ompi``: The Open MPI code base
    * ``opal``: The OPAL code base
* ``config``: M4 scripts supporting the top-level ``configure`` script
  ``mpi.h``
* ``etc``: Some miscellaneous text files
* ``docs``: Source code for Open MPI documentation
* ``examples``: Trivial MPI / OpenSHMEM example programs
* ``3rd-party``: Included copies (via Git submodules in Git clones) of
  required core libraries

Each of the three main source directories (``oshmem``, ``ompi``, and
``opal``) generate a top-level library named ``liboshmem``,
``libmpi``, and ``libopen-pal``, respectively.  They can be built as
either static or shared libraries.  Executables are also produced in
subdirectories of some of the trees.

Each of the sub-project source directories have similar (but not
identical) directory structures under them:

* ``class``: C++-like "classes" (using the OPAL class system)
  specific to this project
* ``include``: Top-level include files specific to this project
* ``mca``: MCA frameworks and components specific to this project
* ``runtime``: Startup and shutdown of this project at runtime
* ``tools``: Executables specific to this project (currently none in
  OPAL)
* ``util``: Random utility code

There are other top-level directories in each of the
sub-projects, each having to do with specific logic and code for that
project.  For example, the MPI API implementations can be found under
``ompi/mpi/LANGUAGE``, where
``LANGUAGE`` is ``c``, ``fortran``.

The layout of the ``mca`` trees are strictly defined.  They are of the
form:

.. code-block::
    :linenos:

    PROJECT/mca/FRAMEWORK/COMPONENT

To be explicit: it is forbidden to have a directory under the ``mca``
trees that does not meet this template (with the exception of ``base``
directories, explained below).  Hence, only framework and component
code can be in the ``mca`` trees.

That is, framework and component names must be valid directory names
(and C variables; more on that later).  For example, the TCP BTL
component is located in the following directory:

.. code-block:: sh
    :linenos:

    # In v1.6.x and earlier:
    ompi/mca/btl/tcp/

    # In v1.7.x and later:
    opal/mca/btl/tcp/

The name ``base`` is reserved; there cannot be a framework or component
named ``base``. Directories named ``base`` are reserved for the
implementation of the MCA and frameworks.  Here are a few examples (as
of the v5.0 source tree):

.. code-block:: sh
    :linenos:

    # Main implementation of the MCA
    opal/mca/base

    # Implementation of the btl framework
    opal/mca/btl/base

    # Implementation of the sysv framework
    oshmem/mcs/sshmem/sysv

    # Implementation of the pml framework
    ompi/mca/pml/base

Under these mandated directories, frameworks and/or components may have
arbitrary directory structures, however.


Installing the GNU Autootools
-----------------------------

.. _gnu-autotools-section-label:

There is enough detail in building the GNU Autotools that it warrants
its own section.

.. note:: As noted above, you only need to read/care about this
          section if you are building Open MPI from a Git clone.  End
          users installing an Open MPI distribution tarball do *not*
          need to have the GNU Autotools installed.

Autotools versions
^^^^^^^^^^^^^^^^^^

The following tools are required for developers to compile Open MPI
from its repository sources (users who download Open MPI tarballs do
not need these tools - they are only required for developers working
on the internals of Open MPI itself):

.. list-table::
    :header-rows: 1

    * - Software package
      - Notes
      - URL

    * - GNU m4
      - See version chart below
      - https://ftp.gnu.org/gnu/m4/
    * - GNU Autoconf
      - See version chart below
      - https://ftp.gnu.org/gnu/autoconf/
    * - GNU Automake
      - See version chart below
      - https://ftp.gnu.org/gnu/automake/
    * - GNU Libtool
      - See version chart below
      - https://ftp.gnu.org/gnu/libtool/

The table below lists the versions that are used to make nightly
snapshot and official release Open MPI tarballs. Other versions of the
tools may work for some (but almost certainly not all) platforms, but
the ones listed below are the versions that we know work across an
extremely wide variety of platforms and environments.

To strengthen the above point: the core Open MPI developers typically
use very, very recent versions of the GNU tools.  There are known bugs
in older versions of the GNU tools that Open MPI no longer compensates
for (it seemed senseless to indefinitely support patches for ancient
versions of Autoconf, for example).

.. warning:: You **will** have problems if you do not use recent
             versions of the GNU Autotools.

That being said, ``autogen.pl`` and ``configure.ac`` scripts tend to
be a bit lenient and enforce slightly older minimum versions than the
ones listed below. This is because such older versions still make
usable Open MPI builds on many platforms - especially Linux on x86_64
with GNU compilers - and are convenient for developers whose Linux
distro may not have as recent as the versions listed below (but are
recent enough to produce a working version for their platform).

To be clear: the versions listed below are required to support a wide
variety of platforms and environments, and are used to make nightly
and official release tarballs. When building Open MPI, YMMV when using
versions older than those listed below |mdash| especially if you are
not building on Linux x86_64 with the GNU compilers.

Using older versions is unsupported. If you run into problems, upgrade
to at least the versions listed below.

.. note:: You may need to scroll right in the following table.

.. list-table::
    :header-rows: 1

    * - Open MPI
      - M4
      - Autoconf
      - Automake
      - Libtool
      - Flex
      - Pandoc
      - Sphinx

    * - v1.0.x
      - NA
      - 2.58 - 2.59
      - 1.7 - 1.9.6
      - 1.5.16 - 1.5.22
      - 2.5.4
      -	NA
      - NA
    * - v1.1.x
      - NA
      - 2.59
      - 1.9.6
      - 1.5.16 - 1.5.22
      - 2.5.4
      - NA
      - NA
    * - v1.2.x
      - NA
      - 2.59
      - 1.9.6
      - 1.5.22 - 2.1a
      - 2.5.4
      - NA
      - NA
    * - v1.3.x
      - 1.4.11
      - 2.63
      - 1.10.1
      - 2.2.6b
      - 2.5.4
      - NA
      - NA
    * - v1.4.x
      - 1.4.11
      - 2.63
      - 1.10.3
      - 2.2.6b
      - 2.5.4
      - NA
      - NA
    * - v1.5.x for x=0-4
      - 1.4.13
      - 2.65
      - 1.11.1
      - 2.2.6b
      - 2.5.4
      - NA
      - NA
    * - v1.5.x for x>=5
      - 1.4.16
      - 2.68
      - 1.11.3
      - 2.4.2
      - 2.5.35
      - NA
      - NA
    * - v1.6.x
      - 1.4.16
      - 2.68
      - 1.11.3
      - 2.4.2
      - 2.5.35
      - NA
      - NA
    * - v1.7.x
      - 1.4.16
      - 2.69
      - 1.12.2
      - 2.4.2
      - 2.5.35
      - NA
      - NA
    * - v1.8.x
      - 1.4.16
      - 2.69
      - 1.12.2
      - 2.4.2
      - 2.5.35
      - NA
      - NA
    * - v1.10.x
      - 1.4.16
      - 2.69
      - 1.12.2
      - 2.4.2
      - 2.5.35
      - NA
      - NA
    * - v2.0.x through v4.y
      - 1.4.17
      - 2.69
      - 1.15
      - 2.4.6
      - 2.5.35
      - NA
      - NA
    * - Git master
      - 1.4.17
      - 2.69
      - 1.15
      - 2.4.6
      - 2.5.35
      - 1.12
      - 3.4.1

.. error:: **JMS Remove Pandoc, above?**

Here are some random notes about the GNU Autotools:

#. Other version combinations may work, but are untested and
   unsupported. In particular, developers tend to use higher versions
   of Autotools for master/development work, and they usually work
   fine.

#. The v1.4 and v1.5 series had their Automake versions updated on 10
   July 2011 (from 1.10.1 to 1.10.3, and 1.11 to 1.11.1, respectively)
   due to CVE-2009-4029. This applies to all new snapshot tarballs
   produced after this date, and the v1.4 series as of v1.4.4, and the
   v1.5 series as of 1.5.4.

#. If Autoconf 2.60 (and higher) is used, Automake 1.10 (and higher)
   must be used.

#. The ``master`` branch and all release branches starting with v1.2
   require the use of Libtool 2.x (or higher) so that Open MPI can
   build the Fortran 90 module as a shared library. If (and only if)
   you intend to not build the Fortran 90 library or your Fortran 77
   and Fortran 90 compilers have the same name (e.g., gfortran), you
   can use Libtool 1.5.22 to build Open MPI v1.0 through v1.2.x.

#. There was a period of time where Open MPI nightly snapshot tarballs
   were made with `a Libtool 2.0 development snapshot
   <https://www.open-mpi.org/source/libtool.tar.gz>`_. This has
   long-since been deprecated; Open MPI uses official Libtool releases
   (no official Open MPI releases used the Libtool 2.0 development
   snapshot).


Checking your versions
^^^^^^^^^^^^^^^^^^^^^^

You can check what versions of the Autotools you have installed with
the following:

.. code-block:: sh
   :linenos:

   shell$ m4 --version
   shell$ autoconf --version
   shell$ automake --version
   shell$ libtoolize --version

Installing the GNU Autotools from source
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. note:: Most operating system packaging systems (to include Homebrew
          and MacPorts on MacOS) install recent-enough versions of the
          GNU Autotools.  You should generally only install the GNU
          Autotools manually if you can't use your operating system
          packaging system to install them for you.

The GNU Autotools sources can be can be downloaded from:

* https://ftp.gnu.org/gnu/autoconf/
* https://ftp.gnu.org/gnu/automake/
* https://ftp.gnu.org/gnu/libtool/
* And if you need it: https://ftp.gnu.org/gnu/m4/

It is certainly easiest to download/build/install all four of these
tools together.  But note that Open MPI has no specific m4
requirements; it is only listed here because Autoconf requires minimum
versions of GNU m4.  Hence, you may or may not *need* to actually
install a new version of GNU m4.  That being said, if you are confused
or don't know, just install the latest GNU m4 with the rest of the GNU
Autotools and everything will work out fine.


Build and Install Ordering
^^^^^^^^^^^^^^^^^^^^^^^^^^

You must build and install the GNU Autotools in the following order:

#. m4
#. Autoconf
#. Automake
#. Libtool

You *must* install the last three tools (Autoconf, Automake, Libtool)
into the same prefix directory.  These three tools are somewhat
inter-related, and if they're going to be used together, they MUST
share a common installation prefix.

You can install m4 anywhere as long as it can be found in the path;
it may be convenient to install it in the same prefix as the other
three.  Or you can use any recent-enough m4 that is in your path.

.. warning:: It is *strongly* encouraged that you do **not** install
   your new versions over the OS-installed versions.  This could cause
   other things on your system to break.  Instead, install into
   ``$HOME/local``, or ``/usr/local``, or wherever else you tend to
   install "local" kinds of software.

   In doing so, be sure to prefix your ``$PATH`` with the directory
   where they are installed.  For example, if you install into
   ``$HOME/local``, you may want to edit your shell startup file
   (``.bashrc``, ``.cshrc``, ``.tcshrc``, etc.) to have something
   like:

   .. code-block:: sh
      :linenos:

      # For bash/sh:
      export PATH=$HOME/local/bin:$PATH
      # For csh/tcsh:
      set path = ($HOME/local/bin $path)

   Ensure to set your ``$PATH`` *BEFORE* you configure/build/install
   the four packages.

All four packages require two simple commands to build and
install (where ``PREFIX`` is the prefix discussed in 3, above).

.. code-block:: sh
   :linenos:

   shell$ cd M4_DIRECTORY
   shell$ ./configure --prefix=PREFIX
   shell$ make all install

.. important:: If you are using a shell that does not automatically
               re-index the ``$PATH`` (e.g., the ``csh`` or ``tcsh``
               shells), be sure to run the ``rehash`` command before
               you install the next package so that the executables
               that were just installed can be found by the next
               package.

.. code-block:: sh
   :linenos:

   # Make $PATH be re-indexed if necessary, e.g., via "rehash"
   shell$ cd AUTOCONF_DIRECTORY
   shell$ ./configure --prefix=PREFIX
   shell$ make all install

.. code-block:: sh
   :linenos:

   # Make $PATH be re-indexed if necessary, e.g., via "rehash"
   shell$ cd AUTOMAKE_DIRECTORY
   shell$ ./configure --prefix=PREFIX
   shell$ make all install

.. code-block:: sh
   :linenos:

   # Make $PATH be re-indexed if necessary, e.g., via "rehash"
   shell$ cd LIBTOOL_DIRECTORY
   shell$ ./configure --prefix=PREFIX
   shell$ make all install
