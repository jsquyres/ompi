Running MPI jobs with Slurm
===========================

.. JMS How can I create a TOC just for this page here at the top?

/////////////////////////////////////////////////////////////////////////

.. _faq-slurm-support-label:

Does Open MPI support running jobs under Slurm?
-----------------------------------------------

Yes.

Open MPI supports two modes of launching parallel MPI jobs under
Slurm:

#. Using ``mpirun``.  Open MPI's ``mpirun`` will detect that it is
   inside of a Slurm job, and will automatically utilize the SLURM
   infrastructure for launching and controlling the individual MPI
   processes.

   Using this method, you get the full power and extensive features of
   Open MPI's ``mpirun`` command (see the ``mpirun(1)`` man page for
   details).

#. Using ``srun``.  Assuming that Slurm installed its Open MPI plugin,
   you can use ``srun`` to "direct launch" Open MPI applications
   without the use of Open MPI's ``mpirun`` command.

   Using direct launch can be *slightly* faster when launching very,
   very large MPI processes (i.e., thousands or millions of MPI
   processes in a single job).  But it has significantly fewer
   features than Open MPI's ``mpirun``.

   .. note:: In versions of Open MPI prior to |ompi_series|, using
             ``srun`` for direct launch was faster than using
             ``mpirun``.  **This is no longer true.**

Unless there is a strong reason to use ``srun`` for direct launch, the
Open MPI team recommends using ``mpirun`` for launching under Slurm jobs.

/////////////////////////////////////////////////////////////////////////

What's the difference between using ``mpirun`` and ``srun``?
------------------------------------------------------------

.. error:: JMS Ralph to provide content here.

/////////////////////////////////////////////////////////////////////////

How do I run use ``mpirun`` to launch jobs under Slurm?
-------------------------------------------------------

Pretty much exactly as you would if you were not in a Slurm job.

For example, you can launch Open MPI's ``mpirun`` in an interactive
Slurm allocation (via the ``salloc`` command) or you can submit a
script to Slurm (via the ``sbatch`` command) that includes an
invocation of the ``mpirun`` command.

Regardless of how ``mpirun`` is invoked, if it detects that it is
running in a Slurm job, ``mpirun`` automatically obtains both the list
of hosts and how many processes to start on each host from Slurm
directly.  Hence, it is unnecessary to specify the ``--hostfile``,
``--host``, or ``-np`` options to ``mpirun``.  Open MPI will also use
Slurm-native mechanisms to launch and kill processes --
``ssh`` is not required.

For example:

.. code-block:: sh
   :linenos:

   # Allocate a Slurm job with 4 nodes
   shell$ salloc -N 4
   # Now run an Open MPI job on all the nodes allocated by Slurm
   shell$ mpirun my_mpi_application

This will run the 4 MPI processes on the nodes that were allocated by
Slurm.

Or, if submitting a script:

.. code-block:: sh
   :linenos:

   shell$ cat my_script.sh
   #!/bin/sh
   mpirun my_mpi_application
   shell$ sbatch -N 4 my_script.sh
   srun: jobid 1234 submitted
   shell$

Similar to the ``salloc`` case, no command line options specifing
number of MPI processes were necessary, since Open MPI will obtain
that information directly from Slurm at run time.

/////////////////////////////////////////////////////////////////////////

How do I use ``srun`` to directly launch Open MPI applications?
---------------------------------------------------------------

.. note:: Per :ref:`this FAQ entry <faq-slurm-support-label>`, the
          Open MPI team generally recommends using ``mpirun`` for
          launching MPI jobs.

First, you must ensure that Slurm was built and installed with PMI-2
support.

.. note:: Please ask your friendly neighborhood Slurm developer to
          support PMIx.  PMIx is the current generation of run-time
          support API; PMI-2 is the legacy / antiquated API.  Open MPI
          *only* supports PMI-2 for SLURM.  :-)

Yes, if you have configured OMPI ``--with-pmi=foo``, where ``foo`` is
the path to the directory where ``pmi2.h`` is located.

.. error:: JMS Ralph -- what else do we need to say here?

Open MPI applications can then be launched directly via the ``srun``
command.  For example:

.. code-block:: sh
   :linenos:

   shell$ srun -N 4 my_mpi_application

Or you can use ``sbatch`` with a script:

.. code-block:: sh
   :linenos:

   shell$ cat my_script.sh
   #!/bin/sh
   srun my_mpi_application
   shell$ sbatch -N 4 my_script.sh
   srun: jobid 1235 submitted
   shell$

Similar using ``mpirun`` inside of an ``sbatch`` batch script, no
``srun`` command line options specifing number of processes were
necessary, because ``sbatch`` set all the relevant Slurm-level
parameters about number of processes, cores, partition, etc.

/////////////////////////////////////////////////////////////////////////

I use Slurm on a cluster with the OpenFabrics or UCX network stacks.  Do I need to do anything special?
-------------------------------------------------------------------------------------------------------

Yes.

You need to ensure that Slurm sets up the locked memory
limits properly.

.. error:: JMS Need to point to general web pages about setting locked
           memory limits.

           They used to be at
           ``category=openfabrics#ib-locked-pages`` and
           ``category=openfabrics#ib-locked-pages-more``.

           This should probably be in a general networking section --
           not specific to verbs/openib.

/////////////////////////////////////////////////////////////////////////

My job fails / performs poorly when using mpirun under Slurm 20.11
------------------------------------------------------------------

There were some changes in Slurm behavior that were introduced in
Slurm 20.11.0 and subsequently reverted out in Slurm 20.11.3.

SchedMD (the makers of Slurm) strongly suggest that all Open MPI users
avoid using Slurm versions 20.11.0 through 20.11.2.

Indeed, you will likely run into problems using just about any version
of Open MPI these problematic Slurm releases.

.. important:: Please either downgrade to an older version or upgrade
               to a newer version of Slurm.
