Tools
=====

.. JMS How can I create a TOC just for this page here at the top?

/////////////////////////////////////////////////////////////////////////

What is special about MPI performance analysis?
-----------------------------------------------

The synchronization among the MPI processes can be a key performance
concern.  For example, if a serial program spends a lot of time in
function ``foo()``, you should optimize ``foo()``.  In contrast, if an
MPI process spends a lot of time in ``MPI_Recv()``, not only is the
optimization target probably not ``MPI_Recv()``, but you should in
fact probably be looking at some other process altogether.  You should
ask, "What is happening on other processes when this process has the
long wait?"

Another issue is that a parallel program (in the case of MPI, a
multi-process program) can generate much more performance data than a
serial program due to the greater number of execution threads.
Managing that data volume can be a challenge.

/////////////////////////////////////////////////////////////////////////

What are "profiling" and "tracing"?
-----------------------------------

These terms are sometimes used to refer to two different kinds
of performance analysis.

In profiling, one aggregates statistics at run time |mdash| e.g., total
amount of time spent in MPI, total number of messages or bytes sent,
etc.  Data volumes are small.

In tracing, an event history is collected.  It is common to display
such event history on a timeline display.  Tracing data can provide
much interesting detail, but data volumes are large.

/////////////////////////////////////////////////////////////////////////

How do I sort out busy wait time from idle wait, user time from system time, and so on?
---------------------------------------------------------------------------------------

Don't.

MPI synchronization delays, which are key performance inhibitors you
will probably want to study, can show up as user or system time, all
depending on the MPI implementation, the type of wait, what run-time
settings you have chosen, etc.  In many cases, it makes most sense for
you just to distinguish between time spent inside MPI from time spent
outside MPI.

Elapsed wall clock time will probably be your key metric.  Exactly how
the MPI implementation spends time waiting is less important.

/////////////////////////////////////////////////////////////////////////

What is PMPI?
-------------

PMPI refers to the MPI standard profiling interface.

Each standard MPI function can be called with an ``MPI_`` or ``PMPI_``
prefix.  For example, you can call either ``MPI_Send()`` or
``PMPI_Send()``.  This feature of the MPI standard allows one to write
functions with the ``MPI_`` prefix that call the equivalent ``PMPI_``
function.  Specifically, a function so written has the behavior of the
standard function plus any other behavior one would like to add.  This
is important for MPI performance analysis in at least two ways.

First, many performance analysis tools take advantage of PMPI.  They
capture the MPI calls made by your program.  They perform the
associated message-passing calls by calling PMPI functions, but also
capture important performance data.

Second, you can use such wrapper functions to customize MPI behavior.
E.g., you can add barrier operations to collective calls, write out
diagnostic information for certain MPI calls, etc.

OMPI generally layers the various function interfaces as follows:

* Fortran ``MPI_`` interfaces are weak symbols for...
* Fortran ``PMPI_`` interfaces, which call...
* C ``MPI_`` interfaces, which are weak symbols for...
* C ``PMPI_`` interfaces, which provide the specified functionality.

Since OMPI generally implements MPI functionality for all languages in
C, you only need to provide profiling wrappers in C, even if your
program is in another programming language.  Alternatively, you may
write the wrappers in your program's language, but if you provide
wrappers in both languages then both sets will be invoked.

There are a handful of exceptions.  For example,
``MPI_ERRHANDLER_CREATE()`` in Fortran does not call
``MPI_Errhandler_create()``.  Instead, it calls some other low-level
function.  Thus, to intercept this particular Fortran call, you need a
Fortran wrapper.

Be sure you make the library dynamic.  A static library can experience
the linker problems described in the Complications section of the
Profiling Interface chapter of the MPI standard.
