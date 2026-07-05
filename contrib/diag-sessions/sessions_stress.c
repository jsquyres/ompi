/*
 * Throwaway diagnostic reproducer for the intermittent macOS MPI Sessions
 * deadlock (see .github/workflows/diag-macos-sessions.yaml).
 *
 * Each iteration performs a full MPI_Session_init / MPI_Session_finalize
 * cycle.  Because the session refcount returns to zero every cycle, each
 * iteration tears the OMPI/OPAL/PMIx/PRRTE runtime (and its session
 * directory) all the way down and builds it back up -- exactly the churn
 * that stresses the shared-memory-backing-file / session-directory race that
 * hangs hello_sessions_c (which does this twice) on the 3-vCPU macos-15
 * runner.
 *
 * Run under mpirun with multiple ranks so the shared-memory backing file is
 * actually created/attached across local peers, e.g.:
 *
 *   mpirun --map-by ppr:1:core ./sessions_stress 100
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int iters = (argc > 1) ? atoi(argv[1]) : 100;

    for (int i = 0; i < iters; i++) {
        MPI_Session session;
        int rc = MPI_Session_init(MPI_INFO_NULL, MPI_ERRORS_RETURN, &session);
        if (MPI_SUCCESS != rc) {
            fprintf(stderr, "iteration %d: MPI_Session_init failed rc=%d\n",
                    i, rc);
            return 2;
        }
        rc = MPI_Session_finalize(&session);
        if (MPI_SUCCESS != rc) {
            fprintf(stderr, "iteration %d: MPI_Session_finalize failed rc=%d\n",
                    i, rc);
            return 3;
        }
    }

    printf("completed %d MPI_Session init/finalize cycles\n", iters);
    return 0;
}
