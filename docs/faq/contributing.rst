Contributing to Open MPI
========================

.. JMS How can I create a TOC just for this page here at the top?

/////////////////////////////////////////////////////////////////////////

Can I contribute to Open MPI?
-----------------------------

**YES!**

There are many ways to contribute.  Here are a few:

#. Subscribe to `the mailing lists
   <https://www.open-mpi.org/community/lists/>`_ and become active in
   the discussions.
#. Obtain `a source code checkout <https://www.open-mpi.org/source/>`_
   of Open MPI's code base and start looking through the code (be sure
   to see the :doc:`Developers guide </developers>`) for technical
   details about the code base).
#. Write your own components and contribute them back to the main code
   base.
#. Contribute bug fixes and feature enhancements to the main code
   base.
#. Provide testing resources:

   #. For Github Pull Request Continuous Integration (CI)
   #. For nightly snapshot builds and testing


/////////////////////////////////////////////////////////////////////////

I found a bug!  How do I report it?
-----------------------------------

First check that this is not already a known issue by checking the FAQ
and the `mailing list archives
<https://www.open-mpi.org/community/lists>`_ If you can't find your
problem mentioned anywhere, it is most helpful if you can create a
"recipe" to replicate the bug.

Please see the `Getting Help
<https://www.open-mpi.org/community/help/>`_ page for more details on
submitting bug reports.


/////////////////////////////////////////////////////////////////////////

What if I don't want my contribution to be free / open source?
--------------------------------------------------------------

No problem.

While we are creating free / open-source software, and we would prefer
if everyone's contributions to Open MPI were also free / open-source,
we certainly recognize that other organizations have different goals
from us.  Such is the reality of software development in today's
global economy.

As such, it is perfectly acceptable to make non-free / non-open-source
contributions to Open MPI.

We obviously cannot accept such contributions into the main code base,
but you are free to distribute plugins, enhancements, etc. as you see
fit.  Indeed, the :doc:`the BSD license </license>` is extremely
liberal in its redistribution provisions.


/////////////////////////////////////////////////////////////////////////

I want to fork the Open MPI code base.  Can I?
----------------------------------------------

Yes... but we'd prefer if you didn't.

Although :doc:`Open MPI's license </license>` allows third parties to
fork the code base, we would strongly prefer if you did not.  Forking
is not necessarily a Bad Thing, but history has shown that creating
too many forks in MPI implementations leads to massive user and system
administrator confusion.  We have personally seen parallel
environments loaded with tens of MPI implementations, each only
slightly different from the others.  The users then become responsible
for figuring out which MPI they want / need to use, which can be a
daunting and confusing task.

We do periodically have short forks.  Specifically, sometimes an
origanization needs to release a version of Open MPI with a specific
feature.

If you're thinking of forking the Open MPI code base, please let us
know -- let's see if we can work something out so that it is not
necessary.


/////////////////////////////////////////////////////////////////////////

Rats!  My contribution was not accepted into the main Open MPI code base.  What now?
------------------------------------------------------------------------------------

If your contribution was not accepted into the main Open MPI
code base, there are likely to be good reasons for it (perhaps
technical, perhaps due to licensing restrictions, etc.).

If you wrote a standalone component, you can still distribute this
component independent of the main Open MPI distribution.  Open MPI
components can be installed into existing Open MPI installations.  As
such, you can distribute your component -- even if it is closed source
(e.g., distributed as binary-only) -- via any mechanism you choose,
such as on a web site, FTP site, etc.
