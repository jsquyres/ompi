To-Do Items
===========

Things that have changed in v5.0
--------------------------------

Need to update these docs to reflect:

* By default, components are now slurped up to be part of their
  project libraries, not DSOs.

* 32 bit support only built when we have C11 compiler

* What specifically does ``--disable-io-romio`` do?

* Do we still have AMCA files?

* Do we still have ``--tune`` files?

* Document this new breakpoint functionality:
  https://github.com/open-mpi/ompi/commit/f97d081cf9b540c5a79e00aecee17b25e8c123ad

Other random to-do items
------------------------

* https://github.com/open-mpi/ompi/issues/7668

* Add docs about mpirun launch vs. direct launch.

Josh Hursey notes
-----------------

Running MPI Applications Notes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Reviewing:
  https://ompi--8329.org.readthedocs.build/en/8329/faq/running-mpi-apps.html

10.8.21
 - Is aggressive mode really determined by the slot count provided by PRRTE? Or is it determined
   by a query to hwloc with a reference ot the number of processes per node. It just surprises
   me that this part of OMPI is controlled by PRRTE instead of something more generic that might
   work with, say, Slurm direct launch via srun.
