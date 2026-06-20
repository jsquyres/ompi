# Triage report: all stale open PRs, grouped by why they're still open

This report covers **every open pull request in
[`open-mpi/ompi`](https://github.com/open-mpi/ompi) that has had no
activity in more than six months** — the same staleness window used in
[`conflict-prs-report.md`](conflict-prs-report.md), but here including
*all* such PRs, not only the ones with merge conflicts.

- **Analysis date:** 2026-06-20
- **Staleness criterion:** `updatedAt < 2025-12-20` (no activity in >6 months)
- **PRs in scope:** 33 (all target `main`) — 20 conflicting + 13 still cleanly mergeable
- **Companion files:**
  [`conflict-prs-report.md`](conflict-prs-report.md) (the 20 conflicting
  ones, with draft ping comments) and
  [`still-open-prs-by-outcome.md`](still-open-prs-by-outcome.md) (the
  same 33 PRs cross-referenced by recommended action).

Each PR is placed in exactly one **reason-still-open** group. Each entry
gives a one-paragraph summary of the PR, why it never merged, and a
recommended outcome (**Merge** / **Update** / **Abandon**). The
recommended-outcome view is in the companion file.

> **Scope note.** "Older than 6 months" is interpreted here as *no
> activity in >6 months* (GitHub `updatedAt`), to stay consistent with
> the conflict report this extends. A broader reading — PRs *created*
> >6 months ago — adds 16 more PRs that are old but have been touched
> recently (still under active review); those are listed in the
> [appendix](#appendix-old-but-recently-active-prs-not-analyzed-above)
> and can be folded in on request.

## Summary (all 33)

| PR | Title | Author | Last activity | Reason still open | Outcome |
|---:|-------|--------|---------------|-------------------|---------|
| [#11847](https://github.com/open-mpi/ompi/pull/11847) | Avoid claiming copyright for future years | bmwiedemann | 2024-04-10 | Superseded by merged work | Abandon |
| [#12285](https://github.com/open-mpi/ompi/pull/12285) | test/datatype: do not use installed components | ggouaillardet | 2024-02-01 | Superseded by merged work | Abandon |
| [#12303](https://github.com/open-mpi/ompi/pull/12303) | framework: add MCA_BASE_FRAMEWORK_FLAG_NOCOMPONENT | ggouaillardet | 2024-02-05 | Superseded by merged work | Abandon |
| [#12826](https://github.com/open-mpi/ompi/pull/12826) | mca/base: new MCA variable type for include lists | hjelmn | 2025-02-26 | Superseded by merged work | Abandon |
| [#13195](https://github.com/open-mpi/ompi/pull/13195) | btl/uct: connections via a separate memory domain | hjelmn | 2025-04-18 | Superseded by merged work | Abandon |
| [#13450](https://github.com/open-mpi/ompi/pull/13450) | Add Open Cluster Scheduler (fka SGE) to docs | dgruber | 2025-11-03 | Superseded by merged work | Abandon |
| [#6426](https://github.com/open-mpi/ompi/pull/6426) | MCA parameters for memcpy chunk size | bosilca | 2022-03-25 | Author abandoned | Abandon |
| [#9131](https://github.com/open-mpi/ompi/pull/9131) | configury: allow multiple "-include \<file>" | ggouaillardet | 2022-03-25 | Author abandoned | Abandon |
| [#12318](https://github.com/open-mpi/ompi/pull/12318) | Offload reduction operations to accelerators | devreal | 2024-02-23 | Author abandoned | Abandon |
| [#13516](https://github.com/open-mpi/ompi/pull/13516) | [WIP] env vars for libfabric cxi provider | copilot-swe-agent | 2025-11-14 | Author abandoned (bot stub) | Abandon |
| [#9812](https://github.com/open-mpi/ompi/pull/9812) | ompi_fortran_check_ignore_tkr.m4: fix fortran tests | jsquyres | 2024-02-13 | Parked WIP / DNM | Update |
| [#9867](https://github.com/open-mpi/ompi/pull/9867) | Remove all ops with 3 buffers | bosilca | 2022-04-27 | Parked WIP / DNM | Update |
| [#10470](https://github.com/open-mpi/ompi/pull/10470) | [WIP] A new shared memory collectives component | devreal | 2022-09-29 | Parked WIP / DNM | Abandon |
| [#12586](https://github.com/open-mpi/ompi/pull/12586) | Rework the MPI connect/accept code | bosilca | 2024-06-20 | Parked WIP / DNM | Update |
| [#12725](https://github.com/open-mpi/ompi/pull/12725) | opal_thread integration with libevent | npe9 | 2024-09-10 | Parked WIP / DNM | Abandon |
| [#12309](https://github.com/open-mpi/ompi/pull/12309) | pmix: remove the MCA framework | ggouaillardet | 2024-02-09 | Awaiting author revision | Update |
| [#12453](https://github.com/open-mpi/ompi/pull/12453) | mca/coll: any radix k for alltoall bruck | jiaxiyan | 2024-07-12 | Awaiting author revision | Update |
| [#12972](https://github.com/open-mpi/ompi/pull/12972) | part_persist: fix start_all() | ggouaillardet | 2024-12-12 | Awaiting author revision | Update |
| [#13088](https://github.com/open-mpi/ompi/pull/13088) | RFC: equivalence of MPICH_ASYNC_PROGRESS | hominhquan | 2025-10-04 | Awaiting author revision | Update |
| [#13131](https://github.com/open-mpi/ompi/pull/13131) | Fix: MPI_Type_get_contents max array sizes | EmmanuelBRELLE | 2025-06-12 | Awaiting author revision | Update |
| [#13188](https://github.com/open-mpi/ompi/pull/13188) | configury: short float detection with clang | ggouaillardet | 2025-10-08 | Awaiting author revision | Update |
| [#13216](https://github.com/open-mpi/ompi/pull/13216) | OSC/UCX: explicitly flush default endpoints | MamziB | 2025-04-29 | Awaiting author revision | Update |
| [#13526](https://github.com/open-mpi/ompi/pull/13526) | btl/uct: update btl default limits | hjelmn | 2025-12-01 | Awaiting author revision | Update |
| [#12569](https://github.com/open-mpi/ompi/pull/12569) | Add CUDA/HIP reduction operators | devreal | 2024-10-01 | Unresolved design disagreement | Abandon |
| [#13133](https://github.com/open-mpi/ompi/pull/13133) | MPI_T Events | kingshuk00 | 2025-06-03 | Unresolved design disagreement | Abandon |
| [#13147](https://github.com/open-mpi/ompi/pull/13147) | Extract install prefix from the shared library | bosilca | 2025-03-19 | Unresolved design disagreement | Update |
| [#13433](https://github.com/open-mpi/ompi/pull/13433) | interlib: fix thread safety issue | ggouaillardet | 2025-10-13 | Unresolved design disagreement | Update |
| [#13527](https://github.com/open-mpi/ompi/pull/13527) | pml/ob1: protocol for intermediate-sized messages | hjelmn | 2025-12-01 | Unresolved design disagreement | Update |
| [#11551](https://github.com/open-mpi/ompi/pull/11551) | ulfm/agree: Restore hierarchical tree building | abouteiller | 2023-03-31 | Never reviewed | Update |
| [#12935](https://github.com/open-mpi/ompi/pull/12935) | part: multithreading deadlocks fixes | Keluaa | 2024-11-28 | Never reviewed | Update |
| [#9502](https://github.com/open-mpi/ompi/pull/9502) | MPI Continuations proposal [WIP] | devreal | 2025-02-19 | Blocked on upstream (MPI Forum) | Update |
| [#12570](https://github.com/open-mpi/ompi/pull/12570) | Make datatype copy stream-aware | devreal | 2024-05-23 | Blocked on dependency (now cleared) | Update |
| [#12991](https://github.com/open-mpi/ompi/pull/12991) | coll/accelerator: select functions to register | edgargabriel | 2025-01-14 | Approved but never merged | Merge |

**Reason tally:** Superseded 6 · Author abandoned 4 · Parked WIP/DNM 5 ·
Awaiting author revision 8 · Unresolved design disagreement 5 · Never
reviewed 2 · Blocked on dependency/upstream 2 · Approved but never
merged 1.

**Outcome tally:** Abandon 14 · Update 18 · Merge 1.

---

## Superseded by merged work — recommend Abandon

The change, or the need that motivated it, has since been handled
another way on `main` (verified absent / handled in the local tree).

### [#11847 — Avoid claiming copyright for future years](https://github.com/open-mpi/ompi/pull/11847)
- **What it is:** One-line `docs/conf.py` change so the rendered docs copyright string doesn't claim a future year when the build clock is set ahead (reproducible-builds concern).
- **Why it hasn't merged:** Maintainers preferred deriving the year from an environment override; the PR also kept failing the Signed-off-by check.
- **Outcome — Abandon:** `docs/conf.py` already derives the year from `SOURCE_DATE_EPOCH` with a current-year fallback; the concern is resolved.

### [#12285 — test/datatype: do not use installed components](https://github.com/open-mpi/ompi/pull/12285)
- **What it is:** One-line `test/datatype/Makefile.am` tweak to stop datatype unit tests from loading MCA components from a separate installed tree (motivated by #12282, a v5 build loading stale v4 components).
- **Why it hasn't merged:** The author walked back the premise in-thread; bosilca noted the blunt empty-MCA-path approach would break all-shared-library builds. Discussion pivoted to the real root cause (a PMIx-framework finalization crash).
- **Outcome — Abandon:** The change isn't in the tree, the approach was deemed wrong, and the root cause was instead fixed by removing `ompi/mca/pmix` entirely.

### [#12303 — framework: add MCA_BASE_FRAMEWORK_FLAG_NOCOMPONENT](https://github.com/open-mpi/ompi/pull/12303)
- **What it is:** Adds a framework flag letting a framework skip registering/opening components, applied to the `pmix` framework base to avoid a finalization crash from a stale loaded PMIx component (#12282).
- **Why it hasn't merged:** rhc54 argued it was the wrong fix; the author agreed and opened follow-up #12309 ("pmix: remove the MCA framework").
- **Outcome — Abandon:** `ompi/mca/pmix` no longer exists in the tree, so the per-framework flag is dead-on-arrival. (Caveat: successor #12309 is itself still open, but the framework was removed regardless.)

### [#12826 — mca/base: add a new MCA variable type for include lists](https://github.com/open-mpi/ompi/pull/12826)
- **What it is:** A reusable `MCA_BASE_VAR_TYPE_INCLUDE_LIST` variable type (+ `opal_include_list`/`opal_serializable` classes) to centralize include/exclude-list parsing, with unit tests; cited consumer was `btl_uct_memory_domains`.
- **Why it hasn't merged:** bosilca called it "an overblown solution"; rhc54 wanted a concrete in-tree consumer. Discussion trailed off.
- **Outcome — Abandon:** The btl/uct need was met by a component-local `btl_uct_include_list.h`; the generalized type isn't in `opal/mca/base/`. (See conflict report.)

### [#13195 — btl/uct: allow connections via a separate memory domain](https://github.com/open-mpi/ompi/pull/13195)
- **What it is:** Reworks `btl/uct` so an alternate transport (typically TCP) can form endpoint connections when the active memory domain can't.
- **Why it hasn't merged:** Brief review, then silence.
- **Outcome — Abandon:** The same author's larger "complete re-work of the BTL" commit (`112b4f813b`) already landed this exact capability on `main`. (Quick confirming diff suggested before closing.)

### [#13450 — Add Open Cluster Scheduler (fka SGE) support to documentation](https://github.com/open-mpi/ompi/pull/13450)
- **What it is:** Docs-only addition to `docs/launching-apps/gridengine.rst` mentioning Open Cluster Scheduler (former SGE / Altair lineage).
- **Why it hasn't merged:** rhc54 noted this content belongs in PRRTE and ported it upstream (openpmix/prrte#2300).
- **Outcome — Abandon:** The contribution was re-homed to PRRTE, which is where these launcher docs are now maintained; merging here would duplicate it.

---

## Author abandoned — recommend Abandon

Author explicitly stepped away, or the record shows the premise died;
nothing landed.

### [#6426 — Add MCA parameters to define the size of memcpy chunks](https://github.com/open-mpi/ompi/pull/6426)
- **What it is:** MCA parameters for CUDA memcpy chunk size + upper-level "vector copy" hooks in `opal/datatype/`.
- **Why it hasn't merged:** IBM CI failed and was never fixed; author said (2021) the use case disappeared once the path was delegated to UCX.
- **Outcome — Abandon:** 7+ years stale; no memcpy-chunk parameter exists in the tree.

### [#9131 — configury: allow multiple "-include \<file>" options](https://github.com/open-mpi/ompi/pull/9131)
- **What it is:** Tiny `config/opal_functions.m4` fix to tolerate multiple `-include` options (refs #9129).
- **Why it hasn't merged:** The author concluded in-thread that Open MPI was fine and the real fix belonged in PMIx; left as a draft.
- **Outcome — Abandon:** Premise invalidated by the author; no equivalent handling needed in the tree.

### [#12318 — Offload reduction operations to accelerator devices](https://github.com/open-mpi/ompi/pull/12318)
- **What it is:** Large WIP offloading `MPI_Allreduce` reductions to CUDA/ROCm when buffers are in device memory; adds op components, stream-aware plumbing, device pools.
- **Why it hasn't merged:** Maintainers asked to split it up (one piece, #12356, merged); AWS found segfaults; author ran out of time.
- **Outcome — Abandon (as a monolith):** Core never landed (no `op/cuda`/`op/rocm`, no `ompi_op_reduce_stream`). Keep only as a reference for fresh scoped PRs.

### [#13516 — [WIP] Add list of environment variables for libfabric cxi provider](https://github.com/open-mpi/ompi/pull/13516)
- **What it is:** An **empty draft auto-created by the GitHub Copilot SWE agent** (`app/copilot-swe-agent`); only a boilerplate "Initial plan" commit, zero file changes.
- **Why it hasn't merged:** The bot never produced any content; jsquyres asked who created it; no response since.
- **Outcome — Abandon:** Nothing to land; no human owner.

---

## Parked WIP / DNM — author-declared work-in-progress that stalled

### [#9812 — ompi_fortran_check_ignore_tkr.m4: fix fortran test errors](https://github.com/open-mpi/ompi/pull/9812)
- **What it is:** Multi-pronged fix for the NAG Fortran compiler (#9795): corrects three configure m4 tests and reworks the Fortran sentinel constants to use `bind(C)` instead of common blocks + 4-way mangling.
- **Why it hasn't merged:** The author (a maintainer) couldn't tame the module dependency graph (NAG errors, IBM CI failures) and parked it as a draft.
- **Outcome — Update:** The NAG bug is still real (common-block sentinels + `use…only` test unchanged), but `mpi_f08` bindings are now generated and the Perl mangling scripts it edits were removed/ported to Python — so the approach must be re-implemented against the new generator.

### [#9867 — Remove all ops with 3 buffers](https://github.com/open-mpi/ompi/pull/9867)
- **What it is:** Removes the 3-buffer `MPI_Op` machinery across `ompi/op`, the op base, and the AVX component, since nothing used it.
- **Why it hasn't merged:** Carries a WIP-DNM label; approved but deliberately parked pending MPI Forum issue #530.
- **Outcome — Update:** Reconfirm Forum #530's status; the 3buff code has since spread into new `aarch64`/`riscv64` op components, so a substantial rebase is needed.

### [#10470 — [WIP] A new shared memory collectives component](https://github.com/open-mpi/ompi/pull/10470)
- **What it is:** New `coll/smdirect` component — a `coll/sm` clone using XPMEM cross-process mapping for direct buffer-to-buffer collectives (bcast/barrier/reduce/allreduce), meant to eventually replace `coll/sm`.
- **Why it hasn't merged:** WIP draft with an open to-do list, depended on #10127, author silent since 2022.
- **Outcome — Abandon:** `coll/sm` was already removed (2024, #12496), `coll/smdirect` was never created, and the direct-copy goals overlap the now-maintained `coll/xhc`/`coll/acoll`.

### [#12586 — Rework the MPI connect/accept code](https://github.com/open-mpi/ompi/pull/12586)
- **What it is:** Attempts to fix the inter-job connect/accept (DPM) path in `ompi/dpm/dpm.c` where `add_procs()` is effectively called with zero new procs; adds TCP handshake diagnostics.
- **Why it hasn't merged:** The author posted it as an explicit incomplete "band-aid" and said the proper fix is to drop `ilist` and use `mlist`; no reviews.
- **Outcome — Update:** The bug is real and still present (`dpm.c` still uses the suspect `ilist` logic); needs the proper `mlist`-based rework and cross-PML validation.

### [#12725 — opal_thread integration with libevent](https://github.com/open-mpi/ompi/pull/12725)
- **What it is:** Makes libevent threading use OPAL's pluggable thread API instead of hard-wired pthread locking; adds `opal/util/evthread.c`.
- **Why it hasn't merged:** Self-described untested WIP that consistently broke the mpi4py spawn CI; the embedded-PMIx-has-its-own-libevent coexistence problem is unsolved; author silent since 2024-09.
- **Outcome — Abandon:** Nothing landed (`evthread.c` absent, `event.h` macro unchanged); a fresh design-first effort would beat reviving this branch.

---

## Awaiting author revision — review feedback not yet addressed

These have a clear path forward; they need the author to push updates
(and in several cases clear a CHANGES_REQUESTED review).

### [#12309 — pmix: remove the MCA framework](https://github.com/open-mpi/ompi/pull/12309)
- **What it is:** Broad refactor removing the OPAL `pmix` MCA framework and promoting PMIx to `opal/pmix/` (67 files).
- **Why it hasn't merged:** Removing the framework dropped `pmix_base_*` params (e.g. `pmix_base_verbose`) from `ompi_info`; rhc54 + author agreed on a `pmix_base_init` fix in-thread, but it was never committed. Draft.
- **Outcome — Update:** Apply the agreed param fix and rebase across 2+ years of churn; the refactor never landed.

### [#12453 — mca/coll: Add any radix k for alltoall bruck algorithm](https://github.com/open-mpi/ompi/pull/12453)
- **What it is:** Extends the Bruck all-to-all to an arbitrary radix *k* (default 2), wired through the tuned-collective decision logic.
- **Why it hasn't merged:** Author rotated to another project; burlen's request for user-guide docs went unanswered; bosilca's question about the small-message latency jump (EFA/libfabric vs. algorithm) is open.
- **Outcome — Update:** Code area unchanged (still fixed-radix); needs rebase, docs, and resolution of the latency-jump question.

### [#12972 — part_persist: fix start_all()](https://github.com/open-mpi/ompi/pull/12972)
- **What it is:** Fixes an index-reuse bug in `mca_part_persist_start()` (the inner partition loop clobbers the outer index — fixes #12969) plus a header→source refactor of `part_persist`.
- **Why it hasn't merged:** bosilca's review caught additional pre-existing issues in the moved code; never driven to completion.
- **Outcome — Update (high priority):** The bug is **still present** in the tree and #12969 is still open. Consider landing the one-line `start_all()` fix as its own commit first, then the refactor.

### [#13088 — RFC: Provide equivalence of MPICH_ASYNC_PROGRESS](https://github.com/open-mpi/ompi/pull/13088)
- **What it is:** Re-enables an opt-in OPAL software async-progress thread (configure + MCA params + env var), with new docs.
- **Why it hasn't merged:** Largely favorable review ("a step in the right direction"; opt-in, low-risk) but never got final approval; open notes on thread binding, large OSU swings, and smcuda's separate progress thread.
- **Outcome — Update:** Not superseded (`opal_progress.c` still has no async thread); needs a rebase and a final review pass.

### [#13131 — Fix: MPI_Type_get_contents max array sizes may exceed actual ones](https://github.com/open-mpi/ompi/pull/13131)
- **What it is:** Small correctness fix in `ompi/mpi/c/type_get_contents.c` to use the real content counts (not the user-supplied `max_*`) when copying/destroying datatypes, avoiding OOB access.
- **Why it hasn't merged:** bosilca offered a deeper fix in `ompi_datatype_get_args` (also addressing an error-path leak); devreal left CHANGES_REQUESTED; author didn't revise.
- **Outcome — Update:** Confirm whether bosilca's deeper fix landed; if not, rebase and address the review.

### [#13188 — configury: enhance short float detection with clang](https://github.com/open-mpi/ompi/pull/13188)
- **What it is:** Improves `_Float16` configure detection for clang (which can need `--rtlib=compiler-rt`) by making the link test actually invoke the runtime; updates the shortfloat README.
- **Why it hasn't merged:** Two CHANGES_REQUESTED reviews (kawashima-fj on README accuracy, bwbarrett later); author didn't revise. Draft.
- **Outcome — Update:** Real clang false-positive (the m4 still uses the old simple link test); needs the README rework and a de-draft.

### [#13216 — OSC/UCX: explicitly flush default endpoints](https://github.com/open-mpi/ompi/pull/13216)
- **What it is:** Fixes a finalize-time hang / bus error in osc/ucx by flushing default endpoints before they're destroyed; entirely in `osc_ucx_component.c`.
- **Why it hasn't merged:** Draft with CHANGES_REQUESTED; an active review debated flush placement / the added barrier / the single-module guard, but revisions were never pushed.
- **Outcome — Update:** Bug is unfixed in the tree (endpoints still destroyed without a prior flush); needs the requested revisions and a de-draft.

### [#13526 — btl/uct: update btl default limits](https://github.com/open-mpi/ompi/pull/13526)
- **What it is:** Raises `btl/uct` default put/get/send limits so module defaults don't cap higher transport-reported limits (fixes ob1 not issuing large put/get).
- **Why it hasn't merged:** bosilca CHANGES_REQUESTED on the `btl_rndv_eager_limit` bump (receiver memory pressure at scale); author agreed to drop it but hasn't pushed.
- **Outcome — Update:** Measured performance fix with an agreed path; needs the revision to clear the review.

---

## Unresolved design disagreement — needs a maintainer-level decision

### [#12569 — Add CUDA/HIP implementations of reduction operators](https://github.com/open-mpi/ompi/pull/12569)
- **What it is:** GPU-side MPI reduction operators: new `op/cuda` and `op/rocm` components + stream/device-ID variants of `ompi_op_reduce` (follows #12318).
- **Why it hasn't merged:** bosilca objects to the basic design (per-op kernel-launch overhead; favors a persistent GPU "service"); edgargabriel requested changes; author stepped back ("run out of time").
- **Outcome — Abandon:** Nothing in the tree, core reviewer disagrees with the architecture; cleaner to restart from a fresh rebased branch if the effort is revived.

### [#13133 — MPI_T Events](https://github.com/open-mpi/ompi/pull/13133)
- **What it is:** Implements the MPI 4.0/4.1 MPI_T Events interface (rebased from #8057): OPAL event/source infrastructure + filling in the `ompi/mpi/tool/event_*.c` stubs.
- **Why it hasn't merged:** CI deadlock from recursive locking; bosilca called the locking "very sketchy"; author + hppritcha agreed the design needs reconsidering and it was too ambitious for 6.0.x.
- **Outcome — Abandon / reconcile:** Overlaps the maintainer's own ongoing MPI_T events work (`pr/mpi-t-events`, `howard/topic/mpi_t_events`, recent #14077). Fold the effort into that work rather than rebasing as-is.

### [#13147 — Extract the install prefix from the shared library](https://github.com/open-mpi/ompi/pull/13147)
- **What it is:** Derives the install prefix at runtime from the loaded shared library (new `installdirs/runtime` component) so a relocated install works without explicit prefixes; part of a cross-project effort with PMIx/PRRTE companions.
- **Why it hasn't merged:** Extended bosilca/rhc54 disagreement over whether it adds value beyond the existing single-`OPAL_PREFIX` path and whether it mis-sets prefixes for external PMIx/PRRTE; never converged.
- **Outcome — Update:** Needs a maintainer decision on value + the external-PMIx/PRRTE risk, and coordinated landing with the companions.

### [#13433 — interlib: fix thread safety issue](https://github.com/open-mpi/ompi/pull/13433)
- **What it is:** Caches `getenv("OMPI_SHOW_MODEL_CALLBACK")` at init instead of calling the non-thread-safe `getenv()` inside the PMIx `model_callback()` in `interlib.c` (refs #13428).
- **Why it hasn't merged:** Author + rhc54 agree it's only a band-aid (the real fix is the glibc change); the author deferred the merge-vs-close decision and no one acted.
- **Outcome — Update (decide):** Technically complete and trivially mergeable, but contested value — a maintainer needs to accept the band-aid or close it.

### [#13527 — pml/ob1: introduce a new protocol for intermediate-sized messages](https://github.com/open-mpi/ompi/pull/13527)
- **What it is:** Adds a "multi-eager" protocol to ob1 (sender fragments into parallel eager messages; receiver copies fragments straight into the user buffer); off by default.
- **Why it hasn't merged:** bosilca pushed back on changes to the critical send/match path and questioned the benefit vs. a larger eager limit; the author's promised split/revert wasn't pushed.
- **Outcome — Update:** Touches hot ob1 code; needs the discussed refactor or a design resolution with bosilca before it can move.

---

## Never reviewed — slipped through triage

### [#11551 — ulfm/agree: Restore hierarchical tree building](https://github.com/open-mpi/ompi/pull/11551)
- **What it is:** Re-enables a disabled (`#if 0`) hierarchical tree-building path in the ULFM agreement algorithm, porting it off removed ORTE APIs onto PMIx queries.
- **Why it hasn't merged:** Opened as a draft, received zero reviews, author never followed up.
- **Outcome — Update:** Still relevant (the block is verified still `#if 0` with old `orte_*` refs); needs un-drafting, review, and a multi-node ULFM test.

### [#12935 — part: multithreading deadlocks fixes and safety checks](https://github.com/open-mpi/ompi/pull/12935)
- **What it is:** Fixes several races/deadlocks in `part/persist` (Pready vs. progress-thread race, init-time `ompi_comm_idup` deadlock, out-of-lock free segfault).
- **Why it hasn't merged:** No reviews at all — never picked up.
- **Outcome — Update:** part/persist is actively worked (issue #12969 open); the fixes are plausibly valuable but the component was restructured, so a substantial rebase + real review (esp. the idup serialization) is needed.

---

## Blocked on dependency / upstream

### [#9502 — Implementation of the MPI Continuations proposal [WIP]](https://github.com/open-mpi/ompi/pull/9502)
- **What it is:** An Open MPI extension implementing `MPIX_Continue`/`MPIX_Continueall`/`MPIX_Continue_init` (callbacks on request completion), tracking the MPI Forum proposal.
- **Why it hasn't merged:** Depends on a still-unratified Forum proposal; never de-WIP'd; a commit has an unfixed missing `Signed-off-by`.
- **Outcome — Update:** Author (a maintainer) was engaging in early 2025; decide whether to land it as an experimental extension now or keep tracking the Forum.

### [#12570 — Make datatype copy stream-aware](https://github.com/open-mpi/ompi/pull/12570)
- **What it is:** Adds stream-aware `*_copy_content_same_ddt_stream` routines (the third extracted chunk of #12318).
- **Why it hasn't merged:** Stated dependency on #12356 ("will rebase once merged"); #12356 merged 2024-06-12 but the author never returned.
- **Outcome — Update:** Blocker is cleared and the change is small/self-contained; needs the author to confirm intent and rebase.

---

## Approved but never merged

### [#12991 — coll/accelerator: allow to select functions to register](https://github.com/open-mpi/ompi/pull/12991)
- **What it is:** MCA-parameter-driven selective registration of collectives in `coll/accelerator` (bitmask string, modeled on `coll/ucc`).
- **Why it hasn't merged:** Approved by devreal (2025-01-14) but carries a self-imposed `⚠️ WIP-DNM!` label and was never merged.
- **Outcome — Merge:** Approved and self-contained, and the component is still actively developed; rebase, drop the DNM label, and land it.

---

## Appendix: old-but-recently-active PRs (not analyzed above)

For completeness: these 16 PRs were **created** more than 6 months ago
but have been **touched within the last 6 months** (so they fall outside
the "no activity in >6 months" staleness window used above). They are
the difference between the 33 analyzed here and the 49 open PRs created
before 2025-12-20. Several are long-lived but still under active review.
Say the word and I'll fold them into the analysis.

| PR | Title | Author | Created | Last activity | Mergeable |
|---:|-------|--------|---------|---------------|-----------|
| [#6625](https://github.com/open-mpi/ompi/pull/6625) | Remove some more symbols removed in MPI 3.0 | ggouaillardet | 2019-04-30 | 2026-05-08 | CONFLICTING |
| [#8597](https://github.com/open-mpi/ompi/pull/8597) | decide CONVERTOR_CUDA flag based on first byte of buffer | markalle | 2021-03-12 | 2026-01-09 | CONFLICTING |
| [#8842](https://github.com/open-mpi/ompi/pull/8842) | consolidated per-host affinity display in ompi_display_comm | markalle | 2021-04-22 | 2026-01-12 | CONFLICTING |
| [#9934](https://github.com/open-mpi/ompi/pull/9934) | tuned decision for single node communicators | bosilca | 2022-01-27 | 2026-01-07 | CONFLICTING |
| [#11122](https://github.com/open-mpi/ompi/pull/11122) | fs/ceph: creation of the fs/ceph component | ggouaillardet | 2022-11-29 | 2026-05-05 | CONFLICTING |
| [#11628](https://github.com/open-mpi/ompi/pull/11628) | Topic/improved alltoallv | bosilca | 2023-04-27 | 2026-05-14 | MERGEABLE |
| [#12119](https://github.com/open-mpi/ompi/pull/12119) | Allow MPI_Comm_disconnect on intracoms | bosilca | 2023-11-23 | 2026-02-15 | MERGEABLE |
| [#12122](https://github.com/open-mpi/ompi/pull/12122) | mpi-ext.h: include \<mpi.h> | jsquyres | 2023-11-23 | 2026-02-06 | MERGEABLE |
| [#12253](https://github.com/open-mpi/ompi/pull/12253) | v5.0.x: Propagate the error up to the user | jsquyres | 2024-01-19 | 2026-02-06 | MERGEABLE |
| [#12259](https://github.com/open-mpi/ompi/pull/12259) | Fix incorrect TCP connections | bosilca | 2024-01-21 | 2026-01-12 | MERGEABLE |
| [#12458](https://github.com/open-mpi/ompi/pull/12458) | fortran: do not link Fortran libs with unnecessary libraries | ggouaillardet | 2024-04-09 | 2026-05-01 | CONFLICTING |
| [#12745](https://github.com/open-mpi/ompi/pull/12745) | configury: fix IGNORE_TKR check | ggouaillardet | 2024-08-12 | 2026-02-06 | MERGEABLE |
| [#12796](https://github.com/open-mpi/ompi/pull/12796) | Created the part direct module | mdosanjh | 2024-09-05 | 2026-02-07 | CONFLICTING |
| [#13039](https://github.com/open-mpi/ompi/pull/13039) | part-persist: implement message aggregation | AxelSchneewind | 2025-01-15 | 2026-05-28 | MERGEABLE |
| [#13280](https://github.com/open-mpi/ompi/pull/13280) | Updated ABI generation code and new libraries | hppritcha | 2025-05-27 | 2026-06-18 | CONFLICTING |
| [#13519](https://github.com/open-mpi/ompi/pull/13519) | ucx: meet at the pmix fence before disconnecting | jeking3 | 2025-11-14 | 2026-01-07 | MERGEABLE |

> Note: #12253 targets `v5.0.x`; all others here target `main`.
