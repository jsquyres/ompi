Java
====

.. JMS How can I create a TOC just for this page here at the top?

/////////////////////////////////////////////////////////////////////////

Why is there a Java interface in Open MPI?
------------------------------------------

.. error:: JMS Much of this section can probably be folded into
           :doc:`this page </java>`.

Because there is an increasing interest in using Java for HPC.
Also, MPI can benefit from Java because its widespread use makes it
likely to find new uses beyond traditional HPC applications.

/////////////////////////////////////////////////////////////////////////

What MPI coverage is provided by the Java interface?
----------------------------------------------------

Complete MPI-3.1 coverage is provided with a few exceptions:

* The bindings for the ``MPI_Neighbor_alltoallw`` and
  ``MPI_Ineighbor_alltoallw`` functions are not implemented.

* Also excluded are functions that incorporate the concepts of
  explicit virtual memory addressing, such as
  ``MPI_Win_shared_query``.

/////////////////////////////////////////////////////////////////////////

What releases contain the Java interface?
-----------------------------------------

The Java bindings are integrated into Open MPI starting from
the v1.7 series.  Beginning with the v2.0 series, the Java bindings
include coverage of MPI-3.1.

/////////////////////////////////////////////////////////////////////////

Is there documentation for the Java interface?
----------------------------------------------

The Java API documentation is generated at build time in
``$prefix/share/doc/openmpi/javadoc``.

`This Cisco blog post
<https://blogs.cisco.com/performance/java-bindings-for-open-mpi>`_ has
quite a bit of information about the Open MPI Java bindings.

The following paper serves as a reference for the API, and in addition
provides details of the internal implementation to justify some of the
design decisions:

* O. Vega-Gisbert, J. E. Roman, and J. M. Squyres.  `Design and
  implementation of Java bindings in Open MPI
  <http://personales.upv.es/jroman/preprints/ompi-java.pdf>`_


/////////////////////////////////////////////////////////////////////////

How do I build the Java interface?
----------------------------------

In order to use Java bindings in Open MPI, they must be enabled
during configuration. If the JDK can be found in a standard location,
the simplest way to do this is:

.. code-block::
   :linenos:

   shell$ ./configure --enable-mpi-java ...

Otherwise, it is necessary to indicate where the JDK binaries and headers
are located:

.. code-block::
   :linenos:

   shell$ ./configure --enable-mpi-java --with-jdk-bindir=</path/to/jdk/bindir>
                   --with-jdk-headers=</path/to/jdk/headers> ...

/////////////////////////////////////////////////////////////////////////

How do I build and run my Java MPI application?
-----------------------------------------------

For convenience, the ``mpijavac`` wrapper compiler has been
provided for compiling Java-based MPI applications. It ensures that
all required MPI libraries and class paths are defined. You can see
the actual command line using the ``--showme`` option, if you are
interested.

Once your application has been compiled, you can run it with the
standard ``mpirun`` command line:

.. code-block::
   :linenos:

   shell$ mpirun ``options`` java ``your-java-options ...`` your-app-class

For convenience, ``mpirun`` has been updated to detect the ``java``
command and ensure that the required MPI libraries and class paths are
defined to support execution. You therefore do *not* need to specify the
Java library path to the MPI installation, nor the MPI class path. Any
class path definitions required for your application should be
specified either on the command line or via the ``CLASSPATH``
environmental variable. Note that the local directory will be added to
the class path if nothing is specified.

As always, the ``java`` executable, all required libraries, and your
application classes must be available on all nodes.

/////////////////////////////////////////////////////////////////////////

Where did the Java interface come from?
---------------------------------------

Initially, we based the Open MPI interface on the mpiJava code
originally developed at Syracuse and Indiana Universities, and
subsequently maintained by HLRS. However, we later totally rewrote the
Java bindings and extended the API to cover MPI-3 functionality.

/////////////////////////////////////////////////////////////////////////

Does the Java interface impact performance of my non-Java application?
----------------------------------------------------------------------

The Java interface in Open MPI is logically separated from, and
completely transparent to, all other Open MPI users and has zero
performance impact on the rest of the code/bindings.

/////////////////////////////////////////////////////////////////////////

How does MPI perform under Java?
--------------------------------

We have tested the Java bindings against some benchmarks and the
results are satisfactory (having a slight overhead with respect to
C/Fortran code).

We suggest users experiencing performance problems to `contact the
OMPI users mailing list
<https://www.open-mpi.org/community/lists/ompi.php>`_.

/////////////////////////////////////////////////////////////////////////

What are the known limitations of the Java interface?
-----------------------------------------------------

"There exist issues with the Omnipath
(PSM2) interconnect involving Java. The problems definitely exist in
PSM2 v10.2; we have not tested previous versions.

As of November 2016, there is not yet a PSM2 release that completely
fixes the issue.

The following ``mpirun`` command options will disable PSM2:

.. code-block::
   :linenos:

   shell$ mpirun ... --mca mtl ^psm2 java ...your-java-options... your-app-class
