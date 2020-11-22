General questions
=================

.. JMS How can I create a TOC just for this page here at the top?

/////////////////////////////////////////////////////////////////////////

What is MPI?  What is Open MPI?
-------------------------------

MPI stands for the Message Passing Interface.  Written by the MPI
Forum (a large committee comprised of a cross-section between industry
and research representatives), MPI is a standardized API typically
used for parallel and/or distributed computing.  The MPI standard has
been published multiple times:

* MPI-1.0 (published in 1994).
* MPI-2.0 (published in 1996).  MPI-2.0 is, for the most part,
  additions and extensions to the original MPI-1.0 specification.

  * MPI-2.1 and MPI-2.2 were subsequently published, and contain
    minor fixes, changes, and additions compared to MPI-2.0.

* MPI-3.0 (published in 2012).

  * MPI-3.1 was subsequently published, and contains minor fixes, changes, and
    additions compared to MPI-3.0.

All MPI specifications documents can be downloaded from the official
`MPI Forum web site <https://www.mpi-forum.org/>`_.

Open MPI is an open source, freely available implementation of the MPI
specifications.  The Open MPI software achieves high performance; the
Open MPI project is quite receptive to community input.


/////////////////////////////////////////////////////////////////////////

Where can I learn about MPI?  Are there tutorials available?
------------------------------------------------------------

There are many resources available on the internet for learning MPI.

* The definitive reference for MPI is `the MPI Forum Web site
  <https://www.mpi-forum.org/>`_.  It has copies of the MPI standards
  documents and all of the errata.

* The `"Introduction to MPI" and "Intermediate MPI" tutorials
  <https://www.citutor.org/browse.php>`_ are excellent web-based MPI
  instruction offered by the NCSA.  **This is a great place for
  beginners.**

* `Searching for "MPI tutorial" on Google
  <https://www.google.com/search?q=MPI+tutorial>`_ turns up a wealth
  of information across the internet.


/////////////////////////////////////////////////////////////////////////

What are the goals of the Open MPI Project?
-------------------------------------------

We have several top-level goals:

* Create a free, open source, peer-reviewed, production-quality
  complete MPI implementation.
* Provide extremely high, competitive performance (latency,
  bandwidth, ...pick your favorite metric).
* Directly involve the HPC community with external development
  and feedback (vendors, 3rd party researchers, users, etc.).
* Provide a stable platform for 3rd party research and commercial
  development.
* Support a wide variety of HPC platforms and environments.

In short, we want to work *with* and *for* the HPC community to make a
world-class MPI implementation that can be used on a huge number and
kind of systems.


/////////////////////////////////////////////////////////////////////////

Do you allow external involvement?
----------------------------------

**Absolutely!**

Bringing together smart researchers and developers to work on a common
product is not only a good idea, it's the open source model.  Merging
the multiple MPI implementation teams has worked extremely well for us
over the past year &mdash; extending this concept to the HPC open
source community is the next logical step.

The component architecture that Open MPI is founded upon (see the
"Publications" link for papers about this) is designed to foster 3rd
party collaboration by enabling independent developers to use Open MPI
as a production quality research platform.  Although Open MPI is a
relatively large code base, it is rarely necessary to learn much more
than the interfaces for the component type which you are implementing.
Specifically, the component architecture was designed to allow small,
discrete implementations of major portions of MPI functionality (e.g.,
point-to-point messaging, collective communications, run-time
environment support, etc.).

We encourage the following forms of collaboration:

* Peer review of the Open MPI code base
* Discussion with Open MPI developers on public mailing lists
* Direct involvement from HPC software and hardware vendors
* 3rd parties writing and providing their own Open MPI components


/////////////////////////////////////////////////////////////////////////

How is the Open MPI software licensed?
--------------------------------------

The Open MPI code base is licensed under :doc:`the new BSD license
</license>`.

That being said, although we are an open source project, we recognize
that everyone does not provide free, open source software.  Our
collaboration models allow (and encourage!)  3rd parties to write and
distribute their own components &mdash; perhaps with a different license,
and perhaps even as closed source.  This is all perfectly acceptable
(and desirable!).


/////////////////////////////////////////////////////////////////////////

I want to redistribute Open MPI.  Can I?
----------------------------------------

Absolutely.

*NOTE:* We are not lawyers and this is not legal advice.

Please read the :doc:`Open MPI license </license>` (the BSD license).
It contains extremely liberal provisions for redistribution.


/////////////////////////////////////////////////////////////////////////

How are 3rd party contributions handled?
----------------------------------------

We love code contributions!

All code contributions are submitted as pull requests on the `Open MPI
GitHub project <https://github.com/open-mpi/ompi/>`_.

We need to have an established intellectual property pedigree of the
code in Open MPI.  This means being able to ensure that all code
included in Open MPI is free, open source, and able to be distributed
under :doc:`the BSD license </license>`.

We enforce this policy by requiring all git commits to include a
"Signed-off-by" token in the commit message, indicating your agreement
to the `Open MPI Contributor's Declaration
<https://github.com/open-mpi/ompi/wiki/Administrative-rules#contributors-declaration>`_.

This prevents a situation where intellectual property gets into the
Open MPI code base and then someone later claims that we owe them
money for it.  Open MPI is a free, open source code base.  And we
intend it to remain that way.

The :doc:`Contributing to Open MPI <contributing>` FAQ
section contains more information on this issue.


/////////////////////////////////////////////////////////////////////////

How did Open MPI get started?
-----------------------------

The lead developers for LAM/MPI, LA-MPI, and FT-MPI kept bumping into
each other at various HPC conferences in 2003.  At each conference,
our lunch/dinner-table conversations got more and more technically
involved when it finally dawned on us that we are doing a *lot* of the
same things in each of our respective implementations.  Although each
MPI implementation focused on different areas of excellence, we all
shared the same common core values:

* A full MPI implementation
* Production quality code &mdash; it has to "just work"
* A desire to explore lots of things that an MPI implementation can do
  that we've never had the time/resources to investigate because we
  are bound to bug fixing, etc.

Hence, we decided to collaborate and pool our resources.  At SC2003,
we decided to start an entire new code base &mdash; leaving all the cruft
and legacy code of our prior implementations behind.  Take the best,
leave the rest.  The source tree's first commit was on November 22,
2003; development work started in earnest on January 5, 2004.  Since
then, we have met together as a group once a month (for at least a
week) to meet our goal of a world-class MPI implementation, bar none.

One of the UTK developers moved back to the University of Stuttgart in
late 2004, which effectively brought their team into the project.
