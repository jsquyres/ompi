Building MPI applications
=========================

.. JMS How can I create a TOC just for this page here at the top?

/////////////////////////////////////////////////////////////////////////

In general, how do I build MPI applications with Open MPI?
----------------------------------------------------------

The Open MPI team *strongly* recommends that you simply use Open MPI's
"wrapper" compilers to compile your MPI applications.  That is,
instead of using (for example) ``gcc`` to compile your program, use
``mpicc``.

Open MPI provides a wrapper compiler for four languages:

.. list-table::
   :header-rows: 1

   * - Language
     - Wrapper compiler name

   * - C
     - ``mpicc``

   * - C++
     - ``mpiCC`` (or ``mpic++`` on case-insensitive filesystems).

   * - Fortran
     - ``mpifort``

Hence, if you expect to compile your program as:

.. code-block::
   :linenos:

   shell$ gcc my_mpi_application.c -o my_mpi_application

Simply use the following instead:

.. code-block::
   :linenos:

   shell$ mpicc my_mpi_application.c -o my_mpi_application

.. caution:: It is *absolutely not sufficient* to simply add ``-lmpi``
             to your link line and assume that you will obtain a valid
             Open MPI executable.

Note that Open MPI's wrapper compilers do not do any actual compiling
or linking; all they do is manipulate the command line and add in all
the relevant compiler / linker flags and then invoke the underlying
compiler / linker (hence, the name "wrapper" compiler).  More
specifically, if you run into a compiler or linker error, check your
source code and/or back-end compiler |mdash| it is *usually* not the
fault of the Open MPI wrapper compiler.

/////////////////////////////////////////////////////////////////////////

Wait |mdash| what is ``mpifort``?  Shouldn't I use ``mpif77`` and ``mpif90``?
-----------------------------------------------------------------------------

``mpifort`` is the new name for the Fortran wrapper compiler that
debuted in Open MPI v1.7.

*It supports compiling all versions of Fortran*, and *utilizing all
MPI Fortran interfaces* (``mpif.h``, ``use mpi``, and ``use
mpi_f08``).  There is no need to distinguish between "Fortran 77"
(which hasn't existed for 30+ years) or "Fortran 90" |mdash| just use
``mpifort`` to compile all your Fortran MPI applications and don't
worry about what dialect it is, nor which MPI Fortran interface it
uses.

Other MPI implementations also support a wrapper compiler named
``mpifort``; you do not lose any portability by using ``mpifort``.
Please help us move the whole world to this simpler wrapper compiler
name, and completely eliminate the use of ``mpif77`` and ``mpif90``.

.. important:: ``mpif77`` and ``mpif90`` were deprecated back in Open
               MPI v1.7.

.. warning:: Although ``mpif77`` and ``mpif90`` still exist in Open
             MPI for legacy reasons, they will likely be removed in
             some (undetermined) future release.  It is in your
             interest to convert to ``mpifort`` now.

Also note that the ``mpif77`` and ``mpif90`` names are literally just
symolic links to ``mpifort`` under the covers.  Meaning: you're using
``mpifort`` whether you realize it or not.

Basically, the 1980's called; they want their ``mpif77`` wrapper
compiler back.  *Let's let them have it.*

/////////////////////////////////////////////////////////////////////////

.. _faq-building-mpi-apps-wrapper-compiler-alternatives-label:

I can't / don't want to use Open MPI's wrapper compilers. What do I do?
-----------------------------------------------------------------------

We repeat the above statement: the Open MPI Team *strongly* recommends
that you use the wrapper compilers to compile and link MPI
applications.

If you find yourself saying, "But I don't *want* to use wrapper
compilers!", please humor us and try them.  See if they work for you.
Be sure to let us know if they do *not* work for you.

Many people base their "wrapper compilers suck!" mentality on bad
behavior from poorly-implemented wrapper compilers in the mid-1990's.
Things are *much* better these days; wrapper compilers can handle
almost any situation, and are far more reliable than you attempting to
hard-code the Open MPI-specific compiler and linker flags manually.

That being said, there *are* some situations |mdash| very, very few
situations |mdash| where using wrapper compilers can be problematic
|mdash| such as nesting multiple wrapper compilers of multiple
projects.  Hence, Open MPI provides a workaround to find out what
command line flags you need to compile MPI applications.  There are
generally two sets of flags that you need: compile flags and link
flags.

You can use one of two methods to dynamically discover what compiler /
linker flags are necessary on your system:

#. Use the ``--showme`` flags to Open MPI's wrapper compilers, or
#. Use the ``pkg-config(1)`` system.

Here's an example showing the use of the ``--showme`` wrapper compiler
flags:

.. code-block:: sh
   :linenos:

   # Show the flags necessary to compile MPI C applications
   shell$ mpicc --showme:compile

   # Show the flags necessary to link MPI C applications
   shell$ mpicc --showme:link

The ``--showme:*`` flags work with all Open MPI wrapper compilers
(specifically: ``mpicc``, ``mpiCC`` / ``mpicxx`` / ``mpic++``,
``mpifort``, and if you really must use them, ``mpif77``, ``mpif90``).

.. warning:: It is almost never a good idea to hard-code these results
             in a Makefile (or other build system).  It is almost
             always best to run (for example) ``mpicc
             --showme:compile`` in a dynamic fashion to find out what
             you need.  For example, GNU Make allows running commands
             and assigning their results to variables:

             .. code-block:: make
                :linenos:

                MPI_COMPILE_FLAGS = $(shell mpicc --showme:compile)
                MPI_LINK_FLAGS = $(shell mpicc --showme:link)

                my_app: my_app.c
                        $(CC) $(MPI_COMPILE_FLAGS) my_app.c $(MPI_LINK_FLAGS) -o my_app

And here's an example showing how to use ``pkg-config`` |mdash| you
may need to add ``$prefix/lib/pkgconfig`` to the ``PKG_CONFIG_PATH``
environment variable for Open MPI's config files to be found:

.. code-block:: sh
   :linenos:

   # Show the flags necessary to compile MPI C applications
   shell$ export PKG_CONFIG_PATH=/opt/openmpi/lib/pkgconfig
   shell$ pkg-config ompi-c --cflags

   # Show the flags necessary to link MPI C applications
   shell$ pkg-config ompi-c --libs

Open MPI supplies multiple ``pkg-config(1)`` configuration files; one
for each different wrapper compiler (language):

* ``ompi``: Synonym for ``ompi-c``; Open MPI applications using the C
  MPI bindings
* ``ompi-c``: Open MPI applications using the C MPI bindings
* ``ompi-cxx``: Open MPI applications using the C MPI bindings
* ``ompi-fort``: Open MPI applications using the Fortran MPI bindings


/////////////////////////////////////////////////////////////////////////

.. _faq-building-mpi-apps-override-wrapper-flags-label:

How do I override the flags specified by Open MPI's wrapper compilers?
----------------------------------------------------------------------

The Open MPI wrapper compilers are driven by text files that contain,
among other things, the flags that are passed to the underlying
compiler.  These text files are generated automatically for Open MPI
and are customized for the compiler set that was selected when Open
MPI was configured; it is *not* recommended that users edit these
files.

However, there are cases where it may be necessary or desirable to
edit these files and add to or subtract from the flags that Open MPI
selected.  These files are installed in ``$pkgdatadir``, which defaults
to ``$prefix/share/openmpi/WRAPPER_NAME-wrapper-data.txt``.  A
few environment variables are available for run-time replacement of
the wrapper's default values (from the text files):

.. note:: You may need to scroll right in the following table.

.. list-table::
   :header-rows: 1

   * - Wrapper compiler
     - Compiler
     - Preprocessor flags
     - Compiler flags
     - Linker flags
     - Linker library flags
     - Data file

   * - ``mpicc``
     - ``OMPI_CC``
     - ``OMPI_CPPFLAGS``
     - ``OMPI_CFLAGS``
     - ``OMPI_LDFLAGS``
     - ``OMPI_LIBS``
     - ``mpicc-wrapper-data.txt``


   * - ``mpic++``
     - ``OMPI_CXX``
     - ``OMPI_CPPFLAGS``
     - ``OMPI_CXXFLAGS``
     - ``OMPI_LDFLAGS``
     - ``OMPI_LIBS``
     - ``mpic++-wrapper-data.txt``

   * - ``mpiCC``
     - ``OMPI_CXX``
     - ``OMPI_CPPFLAGS``
     - ``OMPI_CXXFLAGS``
     - ``OMPI_LDFLAGS``
     - ``OMPI_LIBS``
     - ``mpiCC-wrapper-data.txt``

   * - ``mpifort``
     - ``OMPI_FC``
     - ``OMPI_CPPFLAGS``
     - ``OMPI_FCFLAGS``
     - ``OMPI_LDFLAGS``
     - ``OMPI_LIBS``
     - ``mpifort-wrapper-data.txt``

.. caution:: Note that changing the underlying compiler may not work at
             all.

             For example, C++ and Fortran compilers are notoriously
             binary incompatible with each other (sometimes even
             within multiple releases of the same compiler).  If you
             compile/install Open MPI with C++ compiler XYZ and then
             use the ``OMPI_CXX`` environment variable to
             change the ``mpicxx`` wrapper compiler to use
             the ABC C++ compiler, your application code may not
             compile and/or link.  The traditional method of using
             multiple different compilers with Open MPI is to install
             Open MPI multiple times; each installation should be
             built/installed with a different compiler.  This is
             annoying, but it is beyond the scope of Open MPI to be
             able to fix.

Note that the values of these fields can be directly influenced by
passing flags to Open MPI's ``configure`` script.  :ref:`See this
section in the Installation guide <install-wrapper-flags-label>` for
more details.

The files cited in the above table use fairly simplistic "key=value"
data formats.  The following are several fields that are likely to be
interesting for end-users:

* ``project_short``: Prefix for all environment variables.  See
  below.

* ``compiler_env``: Specifies the base name of the environment
  variable that can be used to override the wrapper's underlying
  compiler at run-time.  The full name of the environment variable is
  of the form ``<project_short>_<compiler_env>``; see table above.

* ``compiler_flags_env``: Specifies the base name of the environment
  variable that can be used to override the wrapper's compiler flags
  at run-time.  The full name of the environment variable is of the
  form ``<project_short>_<compiler_flags_env>``; see table above.

* ``compiler``: The executable name of the underlying compiler.

* ``extra_includes``: Relative to ``$installdir``, a list of directories
  to also list in the preprocessor flags to find header files.

* ``preprocessor_flags``: A list of flags passed to the preprocessor.

* ``compiler_flags``: A list of flags passed to the compiler.

* ``linker_flags``: A list of flags passed to the linker.

* ``libs``: A list of libraries passed to the linker.

* ``required_file``: If non-empty, check for the presence of this file
  before continuing.  If the file is not there, the wrapper will abort
  saying that the language is not supported.

* ``includedir``: Directory containing Open MPI's header files.  The
  proper compiler "include" flag is prepended to this directory and
  added into the preprocessor flags.

* ``libdir``: Directory containing Open MPI's library files.  The
  proper compiler "include" flag is prepended to this directory and
  added into the linker flags.

* ``module_option``: This field only appears in ``mpifort``.  It is
  the flag that the Fortran compiler requires to declare where module
  files are located.

/////////////////////////////////////////////////////////////////////////

.. _faq-building-mpi-apps-showme-label:

How can I tell what the wrapper compiler default flags are?
-----------------------------------------------------------

If the corresponding environment variables are not set, the
wrappers will add ``-I$includedir`` and ``-I$includedir/openmpi`` (which
usually map to ``$prefix/include`` and ``$prefix/include/openmpi``,
respectively) to the ``xFLAGS`` area, and add ``-L$libdir`` (which usually
maps to ``$prefix/lib``) to the ``xLDFLAGS`` area.

To obtain the values of the other flags, there are two main methods:

#. Use the ``--showme`` option to any wrapper compiler.  For example:
   (lines broken here for readability):

   .. code-block::
      :linenos:

      shell$ mpicc prog.c -o prog --showme
      gcc -I/path/to/openmpi/include -I/path/to/openmpi/include/openmpi/ompi \
          prog.c -o prog -L/path/to/openmpi/lib -lmpi \
          -lopen-rte -lopen-pal -lutil -lnsl -ldl -Wl,--export-dynamic -lm

   This shows a coarse-grained method for getting the entire command
   line, but does not tell you what each set of flags are (``xFLAGS``,
   ``xCPPFLAGS``, ``xLDFLAGS``, and ``xLIBS``).

#. Use the ``ompi_info`` command.  For example:

   .. code-block::
      :linenos:

      shell$ ompi_info --all | grep wrapper
         Wrapper extra CFLAGS:
       Wrapper extra CXXFLAGS:
         Wrapper extra FFLAGS:
        Wrapper extra FCFLAGS:
        Wrapper extra LDFLAGS:
           Wrapper extra LIBS: -lutil -lnsl -ldl -Wl,--export-dynamic -lm

This installation is *only* adding options in the ``xLIBS`` areas of the
wrapper compilers; all other values are blank (remember: the ``-I``'s
and ``-L``'s are implicit).

Note that the ``--parsable`` option can be used to obtain
machine-parsable versions of this output.  For example:

   .. code-block::
      :linenos:

      shell$ ompi_info --all --parsable | grep wrapper:extra
      option:wrapper:extra_cflags:
      option:wrapper:extra_cxxflags:
      option:wrapper:extra_fflags:
      option:wrapper:extra_fcflags:
      option:wrapper:extra_ldflags:
      option:wrapper:extra_libs:-lutil -lnsl  -ldl  -Wl,--export-dynamic -lm

/////////////////////////////////////////////////////////////////////////

Why does ``mpicc --showme <some flags>`` not show any MPI-relevant flags?
-------------------------------------------------------------------------

The output of commands similar to the following may be somewhat
surprising:

.. code-block::
   :linenos:

   shell$ mpicc -g --showme
   gcc -g
   shell$

Where are all the MPI-related flags, such as the necessary ``-I``,
``-L``, and ``-l`` flags?

The short answer is that these flags are not included in the wrapper
compiler's underlying command line unless the wrapper compiler sees a
filename argument.  Specifically (output artificially wrapped below for
readability):

.. code-block::
   :linenos:

   shell$ mpicc -g --showme
   gcc -g
   shell$ mpicc -g foo.c --showme
   gcc -I/opt/openmpi/include/openmpi -I/opt/openmpi/include -g foo.c \
      -Wl,-u,_munmap -Wl,-multiply_defined,suppress -L/opt/openmpi/lib -lmpi \
      -lopen-rte -lopen-pal -ldl

The second command had the filename ``foo.c`` in it, so the wrapper
added all the relevant flags.  This behavior is specifically to allow
behavior such as the following:

.. code-block::
   :linenos:

   shell$ mpicc --version --showme
   gcc --version
   shell$ mpicc --version
   i686-apple-darwin8-gcc-4.0.1 (GCC) 4.0.1 (Apple Computer, Inc. build 5363)
   Copyright (C) 2005 Free Software Foundation, Inc.
   This is free software; see the source for copying conditions.  There is NO
   warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   shell$

That is, the wrapper compiler does not behave differently when
constructing the underlying command line if ``--showme`` is used or
not.  The *only* difference is whether the resulting command line is
displayed or executed.

Hence, this behavior allows users to pass arguments to the underlying
compiler without intending to actually compile or link (such as
passing ``--version`` to query the underlying compiler's version).  If the
wrapper compilers added more flags in these cases, some underlying
compilers emit warnings.

/////////////////////////////////////////////////////////////////////////

Are there ways to just *add* flags to the wrapper compilers?
------------------------------------------------------------

Yes!

Open MPI's ``configure`` script allows you to add command line flags to
the wrappers on a permanent basis.  :ref:`See this
section in the Installation guide <install-wrapper-flags-label>` for
more details.

These configure options can be handy if you have some optional
compiler/linker flags that you need both Open MPI and all MPI
applications to be compiled with.  Rather than trying to get all your
users to remember to pass the extra flags to the compiler when
compiling their applications, you can specify them with the ``configure``
options shown above, thereby silently including them in the Open MPI
wrapper compilers |mdash| your users will therefore be using the correct
flags without ever knowing it.

/////////////////////////////////////////////////////////////////////////

Why do the wrapper compilers add ``-rpath`` (or similar) flags by default?
--------------------------------------------------------------------------

Prior to v1.7.4, the Open MPI wrapper compilers did not
automatically add ``-rpath`` (or similar) flags when linking MPI
application executables.

Due to popular user request, Open MPI changed its policy starting with
v1.7.4: by default on supported systems, Open MPI's wrapper compilers
*do* insert ``-rpath`` (or similar) flags when linking MPI applications.
You can see the exact flags added by the ``--showme`` functionality
described in :ref:`this FAQ entry <faq-building-mpi-apps-showme-label>`.

This behavior can be disabled by configuring Open MPI with the
``--disable-wrapper-rpath`` CLI option.

/////////////////////////////////////////////////////////////////////////

Can I build 100% static MPI applications?
-----------------------------------------

.. caution:: Fully static linking is not for the weak, and it is not
             recommended.  But it is possible, with some caveats.

#. You must have static libraries available for *everything* that your
   program links to.  This includes Open MPI; you must have used the
   ``--enable-static`` option to Open MPI's ``configure`` or otherwise
   have available the static versions of the Open MPI libraries (note
   that Open MPI static builds default to including all of its plugins
   *in* its libraries |mdash| as opposed to having each plugin in its
   own dynamic shared object file.  So *all* of Open MPI's code will
   be contained in the static libraries |mdash| even what are normally
   contained in Open MPI's plugins).  Note that some popular Linux
   libraries do not have static versions by default (e.g., libnuma),
   or require additional RPMs to be installed to get the equivalent
   libraries.

#. Open MPI must have been built without a memory manager.  This means
   that Open MPI must have been configured with the
   ``--without-memory-manager`` flag.  This is irrelevant on some
   platforms for which Open MPI does not have a memory manager, but on
   some platforms it is necessary (Linux).  It is harmless to use this
   flag on platforms where Open MPI does not have a memory manager.
   Not having a memory manager means that Open MPI's
   ``mpi_leave_pinned`` behavior for OS-bypass networks such as
   InfiniBand will not work.

#. On some systems (Linux), you may see linker warnings about some
   files requiring dynamic libraries for functions such as
   ``gethostname`` and ``dlopen``.  These are ok, but do mean that you
   need to have the shared libraries installed.  You can disable all
   of Open MPI's ``dlopen`` behavior (i.e., prevent it from trying to
   open any plugins) by specifying the ``--disable-dlopen`` flag to
   Open MPI's ``configure`` script).  This will eliminate the linker
   warnings about ``dlopen``.

For example, this is how to configure Open MPI to build static
libraries on Linux:

.. code-block::
   :linenos:

   shell$ ./configure --without-memory-manager --enable-static ...

Some systems may have additional constraints about their support
libraries that require additional steps to produce working 100% static
MPI applications.  For example, any library that has its own run time
plugin system (i.e., that opens dynamically shared objects |mdash|
"DSOs" |mdash| at run time) will have additional complications in
producting 100% static builds.

In such cases, you generally want to run ``mpicc ... --showme`` to see
the compiler / linker commands that Open MPI's wrapper commands will
use, and then augment those commands with linker arguments for the
static versions of the DSO plugins that you will need at run time.

For example, if you have ``libfoo.a`` that dynamically loads
``plugin.so`` at run time, you'll need to have a ``plugin.a`` and
|mdash| assuming the GNU linker |mdash| add arguments similar to the
following:

* ``-static``: Tell the linker to generate a static executable.
* ``-Wl,--whole-archive -lfoo /path/to/plugin.a
  -Wl,--no-whol-archive``: Tell the linker to include the entire
  ``foo`` library and the entire ``plugin.a`` archive in the
  executable.

You can either add these arguments on the command line manually, or you can
modify the default behavior of the wrapper compilers to hide this
complexity from end users (but be aware that if you modify the wrapper
compilers' default behavior, *all* users will be creating static
applications!).

/////////////////////////////////////////////////////////////////////////

Why does my legacy MPI application fail to compile with Open MPI v4.0.0 (and beyond)?
-------------------------------------------------------------------------------------

Starting with v4.0.0, Open MPI |mdash| by default |mdash| removes the
prototypes for MPI symbols that were deprecated in 1996 and finally
removed from the MPI standard in MPI-3.0 (2012).

:doc:`See this FAQ category </faq/removed-mpi-constructs>` for much
more information, including how to easily update your MPI application
to avoid these problems.
