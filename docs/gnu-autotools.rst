Installing the GNU Autootools for Open MPI Developer Builds
===========================================================

.. note:: You only need to read/care about this section if you are building Open MPI from a Git clone.  End users installing an Open MPI distribution tarball do *not* need to have the GNU Autotools installed.

Autotools versions
------------------

The following tools are required for developers to compile Open MPI from its repository sources (users who download Open MPI tarballs do not need these tools - they are only required for developers working on the internals of Open MPI itself):

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

.. JMS Remove Pandoc, above?

The table below lists the versions that are used to make nightly snapshot and official release Open MPI tarballs. Other versions of the tools may work for some (but almost certainly not all) platforms, but the ones listed below are the versions that we know work across an extremely wide variety of platforms and environments.

To strengthen the above point: the core Open MPI developers typically
use very, very recent versions of the GNU tools.  There are known bugs
in older versions of the GNU tools that Open MPI no longer compensates
for (it seemed senseless to indefinitely support patches for ancient
versions of Autoconf, for example).  You **will** have problems if you
do not use recent versions of the GNU Autotools.

If you need newer versions, you are *strongly* encouraged to heed the
advice described below.

.. note:: The ``autogen.pl`` and ``configure.ac`` scripts tend to be a bit lenient and enforce slightly older minimum versions than the ones listed below. This is because such older versions still make usable Open MPI builds on many platforms - especially Linux on x86_64 with GNU compilers - and are convenient for developers whose Linux distro may not have as recent as the versions listed below (but are recent enough to produce a working version for their platform).

   To be clear: the versions listed below are required to support a wide variety of platforms and environments, and are used to make nightly and official release tarballs. When building Open MPI, YMMV when using versions older than those listed below - especially if you are not building on Linux x86_64 with the GNU compilers. Using older versions is unsupported. If you run into problems, upgrade to at least the versions listed below.

.. list-table::
    :header-rows: 1

    * - Open MPI release
      - M4 versions
      - Autoconf versions
      - Automake versions
      - Libtool versions
      - Flex versions
      - Pandoc versions
      - Sphinx versions

    * - v1.0
      - NA
      - 2.58 - 2.59
      - 1.7 - 1.9.6
      - 1.5.16 - 1.5.22
      - 2.5.4
      -	NA
      - NA
    * - v1.1
      - NA
      - 2.59
      - 1.9.6
      - 1.5.16 - 1.5.22
      - 2.5.4
      - NA
      - NA
    * - v1.2
      - NA
      - 2.59
      - 1.9.6
      - 1.5.22 - 2.1a
      - 2.5.4
      - NA
      - NA
    * - v1.3
      - 1.4.11
      - 2.63
      - 1.10.1
      - 2.2.6b
      - 2.5.4
      - NA
      - NA
    * - v1.4
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
    * - v1.6
      - 1.4.16
      - 2.68
      - 1.11.3
      - 2.4.2
      - 2.5.35
      - NA
      - NA
    * - v1.7
      - 1.4.16
      - 2.69
      - 1.12.2
      - 2.4.2
      - 2.5.35
      - NA
      - NA
    * - v1.8
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
    * - v2.x through v4.y
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

.. JMS Remove Pandoc, above?

Here are some random notes about the GNU Autotools:

#. Other version combinations may work, but are untested and unsupported. In particular, developers tend to use higher versions of Autotools for master/development work, and they usually work fine.

#. The v1.4 and v1.5 series had their Automake versions updated on 10 July 2011 (from 1.10.1 to 1.10.3, and 1.11 to 1.11.1, respectively) due to CVE-2009-4029. This applies to all new snapshot tarballs produced after this date, and the v1.4 series as of v1.4.4, and the v1.5 series as of 1.5.4.

#. If Autoconf 2.60 (and higher) is used, Automake 1.10 (and higher) must be used.

#. The ``master`` branch and all release branches starting with v1.2 require the use of Libtool 2.x (or higher) so that Open MPI can build the Fortran 90 module as a shared library. If (and only if) you intend to not build the Fortran 90 library or your Fortran 77 and Fortran 90 compilers have the same name (e.g., gfortran), you can use Libtool 1.5.22 to build Open MPI v1.0 through v1.2.x.

#. There was a period of time where OMPI nightly trunk snapshot tarballs were made with `a Libtool 2.0 development snapshot <https://www.open-mpi.org/source/libtool.tar.gz>`_. This is now deprecated; Open MPI uses official Libtool releases (no official Open MPI releases used the Libtool 2.0 development snapshot).


Checking your versions
----------------------

You can check what versions of the Autotools you have installed with
the following:

.. code-block:: sh
   :linenos:

   shell$ m4 --version
   shell$ autoconf --version
   shell$ automake --version
   shell$ libtoolize --version

Installing the GNU Autotools from source
----------------------------------------

.. note:: Most operating system packaging systems (to include Homebrew and MacPorts on MacOS) install recent-enough versions of the GNU Autotools.  You should generally only install the GNU Autotools manually if you can't use your operating system packaging system to install them for you.

The GNU Autotools sources can be can be downloaded from:

* https://ftp.gnu.org/gnu/autoconf/
* https://ftp.gnu.org/gnu/automake/
* https://ftp.gnu.org/gnu/libtool/
* And if you need it: https://ftp.gnu.org/gnu/m4/

It is certainly easiest to download/build/install all four
of these tools together.  But note that Open MPI has no specific m4
requirements; it is only listed here because Autoconf requires
minimum versions of GNU m4.  Hence, you may or may not *need* to
actually install a new version of GNU m4.  That being said, if you
are confused or don't know, just install the latest GNU m4 with the
rest of the GNU Autotools and everything will work out fine.


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

.. warning:: It is *strongly* encouraged that you do **not** install your new
   versions over the OS-installed versions.  This could cause
   other things on your system to break.  Instead, install into
   ``$HOME/local``, or ``/usr/local``, or wherever else you tend to
   install "local" kinds of software.

   In doing so, be sure to prefix your $path with the directory where
   they are installed.  For example, if you install into
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
   shell$ make; make install

.. important:: If you are using the ``csh`` or ``tcsh`` shells, be
   sure to run the ``rehash`` command after you install each
   package.

.. code-block:: sh
   :linenos:

   shell$ cd AUTOCONF_DIRECTORY
   shell$ ./configure --prefix=PREFIX
   shell$ make; make install

.. code-block:: sh
   :linenos:

   shell$ cd AUTOMAKE_DIRECTORY
   shell$ ./configure --prefix=PREFIX
   shell$ make; make install

.. code-block:: sh
   :linenos:

   shell$ cd LIBTOOL_DIRECTORY
   shell$ ./configure --prefix=PREFIX
   shell$ make; make install

m4, Autoconf and Automake build and install very quickly; Libtool
will take a minute or two.
