Things that have changed in v5.0
================================

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
========================

* https://github.com/open-mpi/ompi/issues/7668

* Add docs about mpirun launch vs. direct launch.

Josh Hursey notes
=================

Running MPI Applications Notes
-------------------------------
Reviewing:
  https://ompi--8329.org.readthedocs.build/en/8329/faq/running-mpi-apps.html

10.8.1
 - Should we mention OMPIHOME for the .ini file for PRRTE?

10.8.7
 - Change "(such as Slur, PBS/Torque, SGE, LoadLeveler),"
   to     "(such as Slurm, PBS/Torque, Grid Engine, LSF),"

10.8.10
 - bullet 2 - formatting of PATH and LD_LIBRARY_PATH in first sentence

10.8.16
 - FYI: This is what we have in the PRTE man page for hostfiles/host
   https://github.com/openpmix/prrte/blame/master/src/tools/prte/prte-map.1.md#L236
 - Bullet 2: The example will launch 3 processes, but not necessarily 1 per node as
   the number of slots is determined by the core count on each node. I would change
   the hostfile to specify 'slots=1' just to keep it simple. Then it would launch
   a single copy of hostname on each of the hosts.
 - Last sentence, formatting of `host02.exmple.com`

10.8.17
 - Bullet 1: "specified hosts, even if it causes oversubscription"
   We will not oversubscribe the host. If there are at least 4 slots available on host 'a'
   then we will use 4 slots on that node. If there are not 4 slots on host 'a' then
   an oversubscription error is thrown.

10.8.19:
 - "use-hwthreads-as-cpus" (this is no "-use-hwthreads-as-cores") has been removed and replaced with the `:HWTCPUS` qualifier to `--map-by` ("--map-by :HWTCPUS")
 - "max slots" : PRRTE doesn't currently has a bug open (I'm working) to fix max_slots in the
    hostfile. https://github.com/openpmix/prrte/issues/770
    That said, max slots is used to limit the degree of oversubscription on a node. So I would
    keep that note if it is helpful to OMPI users.

10.8.20:
 - "max slots" yeah we have it, but it's broken (see above)
 - Bullet 1: "rmaps_base_schedule_policy" is now "rmaps_default_mapping_policy" and can still take 'slot' as an option. In ORTE in v4.1 "rmaps_base_schedule_policy" is actually an alias for "rmaps_base_mapping_policy".
 - Bullet 1: "--byslot" is now "--map-by slot". It will throw a deprecation warning if used.
 - Bullet 2: "rmaps_base_schedule_policy" is now "rmaps_default_mapping_policy"
 - Bullet 2: "--bynode" is now "--map-by node". It will throw a deprecation warning if used.
 - The last paragraph is confusing if we think about 'slots' vs 'max_slots' when mapping by node.
   We will round robin on the nodes filling to the 'slots' limit. After which mpirun will throw
   an oversubscription error. If they then add the ':OVERSUBSCRIBE' qualifier to the '--map-by'
   option then mpirun will continue to round robin until it hits the 'max_slots' limit.
   The confusion in the paragraph is that you are using the term "maximum slot count" which
   implies 'max_slots' but the example does not have the ':OVERSUBSCRIBE' modifier.

10.8.21
 - First example, typo in line 1 "number of lots" => "number of slots"
 - First example, a "dual-processor-core host" would have 2 cores. and there is no indication of
   2 slots in the hostfile. I find that sentence confusing (and redundant to itself?). Do we mean:
   "For example, if you want to run 4 processes on a host with 2 cores that would be oversubscribing the host as you are attempting to assign more processes than processing elements (defaulting to cores)."
 - Is aggressive mode really determined by the slot count provided by PRRTE? Or is it determined
   by a query to hwloc with a reference ot the number of processes per node. It just surprises
   me that this part of OMPI is controlled by PRRTE instead of something more generic that might
   work with, say, Slurm direct launch via srun.

10.8.23
 - Ref: https://github.com/openpmix/prrte/issues/709#issuecomment-797186098
 - PRRTE is dropping the '-tv' and '--debug' options. Ralph and I have been talking with
   Totalview and DDT on an email thread. What we concluded so far:
   - These options are more trouble than they are worth, and both debugger groups are fine
     with dropping them and providing guidance on how to just run the tool directly.
   - PRRTE will throw an error if either of these options are specified.
   - We should document the direct method of calling totalview, however that gets complex with
     the MPIR-Shim wrapper.
     - I don't have any details on which release of DDT will start supporting PMIx, but we will
       need both an MPIR-Shim and non-MPIR-Shim version for those using older and newer products,
       respectively.
 - For now document the direct use of the totalview debugger - below suggested from Totalview
----
shell$ totalview [<tv-options>] -args <program> [<program-options>]
# Making it easy to do the following in the shell to re-run the program with the debugger:
shell$ totalview -args !!
----

10.8.24
 - Ref: https://github.com/openpmix/prrte/issues/709#issuecomment-797186098
 - It's ARM not Alinea now: https://www.arm.com/products/development-tools/server-and-hpc/forge/ddt
 - PRRTE is dropping the '--debug' option. Do just keep the part about 'For reference'
 - This will also need to be updated with the MPIR-Shim option.
   - I don't have any details on which release of DDT will start supporting PMIx, but we will
     need both an MPIR-Shim and non-MPIR-Shim version for those using older and newer products,
     respectively.

10.8.26
 - What kind of text are you looking for? Something like this:
----
The LSF allocation is detected by the presence of the `LSB_JOBID` environment variable. It
then uses the lsbatch API to query the allocation and launch the `prte` daemons on the
nodes in the allocation.
----

10.8.27
 - I have no idea about Grid Engine. Sorry :(

10.8.29
 - Can you move 'OMPI_COMM_WORLD_LOCAL_SIZE' to be just above 'OMPI_COMM_WORLD_LOCAL_RANK'?
   - It makes it symmetrical with OMPI_COMM_WORLD_SIZE/RANK
