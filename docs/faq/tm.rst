PBS / Torque
============

.. JMS How can I create a TOC just for this page here at the top?

/////////////////////////////////////////////////////////////////////////

.. _faq-tm-support-label:

How do I run jobs under Torque / PBS Pro?
-----------------------------------------

The short answer is just to use ``mpirun`` as normal.

When properly configured, Open MPI obtains both the list of hosts and
how many processes to start on each host from Torque / PBS Pro
directly.  Hence, it is unnecessary to specify the ``--hostfile``,
``--host``, or ``-np`` options to ``mpirun``.  Open MPI will use
PBS/Torque-native mechanisms to launch and kill processes (``ssh`` is
not required).

For example:

.. code-block:: sh
   :linenos:

   # Allocate a PBS job with 4 nodes
   shell$ qsub -I -lnodes=4

   # Now run an Open MPI job on all the nodes allocated by PBS/Torque
   shell$ mpirun my_mpi_application

This will run the MPI processes on the nodes that were allocated by
PBS/Torque.  Or, if submitting a script:

.. code-block:: sh
   :linenos:

   shell$ cat my_script.sh
   #!/bin/sh
   mpirun my_mpi_application
   shell$ qsub -l nodes=4 my_script.sh

/////////////////////////////////////////////////////////////////////////

How does Open MPI get the list of hosts from Torque / PBS Pro?

Open MPI has changed how it obtains hosts from Torque / PBS Pro over
time:

#. *v1.0 and v1.1 series:* The list of hosts allocated to a Torque /
   PBS Pro job is obtained directly from the scheduler using the
   internal TM API.

#. *v1.2 series:* Due to scalability limitations in how the TM API was
   used in the v1.0 and v1.1 series, Open MPI was modified to read the
   $PBS_NODEFILE to obtain hostnames.  Specifically, reading the
   $PBS_NODEFILE is _much_ faster at scale than how the v1.0 and v1.1
   series used the TM API.

.. error:: RALPH: Is this still true?  And even if it is, is it worth
           putting here in the OMPI docs?

It is possible that future versions of Open MPI may switch back to
using the TM API in a more scalable fashion, but there isn't currently
a huge demand for it (reading the ``$PBS_NODEFILE`` works just fine).

Note that the TM API is used to *launch* processes in all versions of
Open MPI; the only thing that has changed over time is how Open MPI
obtains hostnames.

/////////////////////////////////////////////////////////////////////////

What happens if ``$PBS_NODEFILE`` is modified?
----------------------------------------------

Bad Things will happen.

We've had reports from some sites that system administrators modify
the ``$PBS_NODEFILE`` in each job according to local policies.  This
will currently cause Open MPI to behave in an unpredictable fashion.
As long as no new hosts are added to the hostfile, it *usually* means
that Open MPI will incorrectly map processes to hosts, but in some
cases it can cause Open MPI to fail to launch processes altogether.

The best course of action is to not modify the ``$PBS_NODEFILE.``

/////////////////////////////////////////////////////////////////////////

How do I determine if Open MPI is configured for Torque/PBS Pro?
----------------------------------------------------------------

If you are configuring and installing Open MPI yourself, and you want
to ensure that you are building the components of Open MPI required for
Torque/PBS Pro support, include the ``--with-tm`` option on the ``configure``
command line.

See :doc:`the section on building Open MPI </faq/building-open-mpi>`
for further details on building Open MPI.

The ``ompi_info`` command can be used to determine whether or not an
installed Open MPI includes Torque/PBS Pro support:

.. code-block::
   :linenos:

   shell$ ompi_info | grep ras

If the Open MPI installation includes support for Torque/PBS Pro, you
should see a line similar to that below. Note the MCA version
information varies depending on which version of Open MPI is
installed.

.. code-block::
   :linenos:

       MCA ras: tm (MCA v2.1.0, API v2.0.0, Component v3.0.0)
