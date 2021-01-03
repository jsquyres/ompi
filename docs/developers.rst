Developer's guide
=================

This section is here for those who are building/exploring Open MPI in its
source code form, most likely through a developer's tree (i.e., a Git
clone).

Obtaining a Git clone
---------------------

Open MPI's Git repositories are hosted at GitHub.

#. First, you will need a Git client. We recommend getting the latest version available. If you do not have the command "git" in your path, you will likely need to download and install Git.
#. `ompi <https://github.com/open-mpi/ompi/>`_ is the main Open MPI repository where most active development is done.  Git clone this repository.  For example:

.. code-block::
   :linenos:

   shell$ git clone --recursive https://github.com/open-mpi/ompi.git
   Cloning into 'ompi'...
   remote: Counting objects: 256644, done.
   remote: Total 256644 (delta 0), reused 0 (delta 0)
   Receiving objects: 100% (256644/256644), 61.98 MiB | 2.15 MiB/s, done.
   Resolving deltas: 100% (213377/213377), done.
   Checking connectivity... done.
   shell$

Note that Git is natively capable of using many forms of web proxies. If your network setup requires the user of a web proxy, `consult the Git documentation for more details <https://git-scm.com/>`_.

.. note:: Prior to October 2014, Open MPI was maintained in a Subversion repository. This Subversion repository had two read-only mirrors: a Mercurial mirror at bitbucket.org and a Git mirror at github.com. These two mirrors are now defunct and will no longer be updated.

   If you are using either of these mirrors, you should stop using them and switch to the main Open MPI Git repository at GitHub.


Developer Build Prerequisites
-----------------------------

Compilers
^^^^^^^^^

Although it should probably be assumed, you'll need a C/C++ compiler.

You'll also need a Fortran compiler if you want to build the Fortran MPI bindings, and a Java compiler if you want to build the (unofficial) Java MPI bindings.

GNU Autotools
^^^^^^^^^^^^^

When building Open MPI from its repository sources, the GNU Autotools must be installed.

.. note:: The GNU Autotools are *not* required when building Open MPI from distribution tarballs.  Open MPI distribution tarballs are bootstrapped such that end-users do not need to have the GNU Autotools installed.

You can generally install GNU Autoconf, Automake, and Libtool via your Linux distro native package system, or via Homebrew or MacPorts on MacOS.  This usually "just works."

If you run into problems with the GNU Autotools, or need to download / build them manually, :doc:`see the GNU Autotool section of the Open MPI developer's docs </gnu-autotools>` for much more detail on how to do this.

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

If you do not have Flex installed and cannot easily install it via your operating system's packaging system (to include Homebrew or MacPorts on MacOS), see `the Flex Github repository
<https://github.com/westes/flex>`_.


Pandoc
^^^^^^

.. JMS THIS MAY/WILL NEED TO CHANGE IF WE SWITCH TO SPHINX

The Pandoc tool is used to generate Open MPI's man pages.
Specifically: Open MPI's man pages are written in Markdown; Pandoc is
the tool that converts that Markdown to nroff (i.e., the format of man
pages).

.. warning:: You must have Pandoc >=v1.12 when building Open MPI from a developer's
   tree.  If configure cannot find Pandoc >=v1.12, it will abort.

If you need to install Pandoc, check your operating system-provided
packages (to include MacOS Homebrew and MacPorts).  `The Pandoc
project web site <https://pandoc.org/>`_ itself also offers binaries
for their releases.


Sphinx
^^^^^^

.. JMS Need to write more here

Sphinx...

* Installable via Python ``pip``
* https://www.sphinx-doc.org/


Developer Builds: Compiler Pickyness by Default
-----------------------------------------------

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

Once you have run ``autogen.pl`` successfully, you can configure and build Open MPI just like end users do with official distribution Open MPI tarballs.

:ref:`See the general "Install Open MPI" documentation for more details. <building-and-installing-section-label>`



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
OSHMEM depends on OMPI, OMPI depends on OPAL.  For example, MPI executables are linked with:

.. code-block:: sh
   :linenos:

   shell$ mpicc myapp.c -o myapp
   # This actually turns into:
   shell$ cc myapp.c -o myapp -lmpi -lopen-rte -lopen-pal ...

More system-level libraries may listed after ``-lopal``, but you get the
idea.

Strictly speaking, these are not "layers" in the classic software
engineering sense (even though it is convenient to refer to them as
such).  They are listed above in dependency order, but that does not
mean that, for example, the OMPI code must go through the
OPAL code in order to reach the operating system or a network
interface.

As such, this code organization more reflects abstractions and
software engineering, not a strict hierarchy of functions that must be
traversed in order to reach a lower layer.  For example, OMPI can
directly call the operating system as necessary (and not go through OPAL).  Indeed,
many top-level MPI API functions are quite performance sensitive; it
would not make sense to force them to traverse an arbitrarily deep
call stack just to move some bytes across a network.

Note that Open MPI also uses some third-party libraries for core functionality:

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
* ``config``: M4 scripts supporting the top-level ``configure`` script ``mpi.h``
* ``etc``: Some miscellaneous text files
* ``docs``: Source code for Open MPI documentation
* ``examples``: Trivial MPI / OpenSHMEM example programs
* ``3rd-party``: Included copies (via Git submodules in Git clones) of required core libraries

Each of the three main source directories (``oshmem``, ``ompi``, and
``opal``) generate a top-level library named ``liboshmem``, ``libmpi``, and
``libopen-pal``, respectively.  They can be built as either static or shared
libraries.  Executables are also produced in subdirectories of some of
the trees.

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
