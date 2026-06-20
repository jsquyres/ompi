# Stale open PRs, cross-referenced by recommended outcome

This is a cross-reference of the same 33 stale open PRs analyzed in
[`still-open-prs.md`](still-open-prs.md) (open PRs in
[`open-mpi/ompi`](https://github.com/open-mpi/ompi) with no activity in
>6 months, as of 2026-06-20), regrouped by **what we should do** with
each one rather than by why it stalled.

Three outcomes:

- **Merge** — substantively ready; land it after a clean rebase / final push.
- **Update** — has real value but needs rework, finishing, or an explicit author/maintainer decision before it can land.
- **Abandon** — close it: superseded, abandoned, or no longer relevant.

**Tally:** Merge 1 · Update 18 · Abandon 14.

The "Reason still open" column links back to the grouping in the
companion report; "Conflicts?" flags PRs that also have merge conflicts
(detailed with draft ping comments in
[`conflict-prs-report.md`](conflict-prs-report.md)).

---

## ✅ Merge — land it (1)

| PR | Title | Author | Reason still open | Conflicts? | Next step |
|---:|-------|--------|-------------------|:----------:|-----------|
| [#12991](https://github.com/open-mpi/ompi/pull/12991) | coll/accelerator: allow to select functions to register | edgargabriel | Approved but never merged | yes | Rebase, drop the self-imposed `WIP-DNM` label, merge (already approved by devreal). |

---

## 🔧 Update — has value, needs work or a decision (18)

Grouped by the kind of action needed.

### Author just needs to push the agreed revision / clear a CHANGES_REQUESTED review

| PR | Title | Author | Reason still open | Conflicts? | Next step |
|---:|-------|--------|-------------------|:----------:|-----------|
| [#13526](https://github.com/open-mpi/ompi/pull/13526) | btl/uct: update btl default limits | hjelmn | Awaiting author revision | no | Drop the `btl_rndv_eager_limit` bump per bosilca; then merge. |
| [#13216](https://github.com/open-mpi/ompi/pull/13216) | OSC/UCX: explicitly flush default endpoints | MamziB | Awaiting author revision | no | Address flush-placement / barrier / single-module-guard review; de-draft. |
| [#13188](https://github.com/open-mpi/ompi/pull/13188) | configury: short float detection with clang | ggouaillardet | Awaiting author revision | no | Rework README accuracy per kawashima-fj; clear bwbarrett's review; de-draft. |
| [#13131](https://github.com/open-mpi/ompi/pull/13131) | Fix: MPI_Type_get_contents max array sizes | EmmanuelBRELLE | Awaiting author revision | yes | Confirm bosilca's deeper fix didn't land; rebase + address review. |
| [#13088](https://github.com/open-mpi/ompi/pull/13088) | RFC: equivalence of MPICH_ASYNC_PROGRESS | hominhquan | Awaiting author revision | yes | Rebase; final review pass (thread binding, smcuda). Opt-in, low-risk. |
| [#12453](https://github.com/open-mpi/ompi/pull/12453) | mca/coll: any radix k for alltoall bruck | jiaxiyan | Awaiting author revision | yes | Rebase, add user-guide docs, resolve latency-jump question. |
| [#12309](https://github.com/open-mpi/ompi/pull/12309) | pmix: remove the MCA framework | ggouaillardet | Awaiting author revision | yes | Apply agreed `pmix_base_init` param fix; rebase 67 files. |

### Bug fix worth reviving (real, still-present bug)

| PR | Title | Author | Reason still open | Conflicts? | Next step |
|---:|-------|--------|-------------------|:----------:|-----------|
| [#12972](https://github.com/open-mpi/ompi/pull/12972) | part_persist: fix start_all() | ggouaillardet | Awaiting author revision | yes | **Priority** — bug still in tree, #12969 open. Land the one-line fix first, refactor separately. |
| [#13216](https://github.com/open-mpi/ompi/pull/13216) | (also a bug fix — see above) | MamziB | Awaiting author revision | no | Finalize-time osc/ucx hang still unfixed. |

### Needs a maintainer-level design decision

| PR | Title | Author | Reason still open | Conflicts? | Next step |
|---:|-------|--------|-------------------|:----------:|-----------|
| [#13527](https://github.com/open-mpi/ompi/pull/13527) | pml/ob1: protocol for intermediate-sized messages | hjelmn | Unresolved design disagreement | yes | Resolve with bosilca (hot send/match path) or do the promised refactor. |
| [#13433](https://github.com/open-mpi/ompi/pull/13433) | interlib: fix thread safety issue | ggouaillardet | Unresolved design disagreement | no | Accept the band-aid and merge, or close — make the call. |
| [#13147](https://github.com/open-mpi/ompi/pull/13147) | Extract install prefix from the shared library | bosilca | Unresolved design disagreement | no | Decide value vs. existing path + external-PMIx/PRRTE risk; land with companions. |
| [#9867](https://github.com/open-mpi/ompi/pull/9867) | Remove all ops with 3 buffers | bosilca | Parked WIP / DNM | yes | Reconfirm MPI Forum #530; rebase across new aarch64/riscv64 op components. |
| [#9502](https://github.com/open-mpi/ompi/pull/9502) | MPI Continuations proposal [WIP] | devreal | Blocked on upstream (Forum) | yes | Decide: land as experimental extension now, or keep tracking the Forum. |

### Needs rework / finishing (and a reviewer)

| PR | Title | Author | Reason still open | Conflicts? | Next step |
|---:|-------|--------|-------------------|:----------:|-----------|
| [#12586](https://github.com/open-mpi/ompi/pull/12586) | Rework the MPI connect/accept code | bosilca | Parked WIP / DNM | no | Author-declared band-aid; do the proper `mlist`-based rework + cross-PML test. |
| [#12570](https://github.com/open-mpi/ompi/pull/12570) | Make datatype copy stream-aware | devreal | Blocked on dependency (cleared) | yes | Dependency #12356 merged; rebase the small self-contained change. |
| [#12935](https://github.com/open-mpi/ompi/pull/12935) | part: multithreading deadlocks fixes | Keluaa | Never reviewed | yes | Substantial rebase + real review (idup serialization); part/persist actively worked. |
| [#11551](https://github.com/open-mpi/ompi/pull/11551) | ulfm/agree: Restore hierarchical tree building | abouteiller | Never reviewed | no | Still relevant (block still `#if 0`); de-draft, review, multi-node ULFM test. |
| [#9812](https://github.com/open-mpi/ompi/pull/9812) | ompi_fortran_check_ignore_tkr.m4: fix fortran tests | jsquyres | Parked WIP / DNM | yes | NAG bug still real; re-implement against the new generated bindings. |

---

## 🗑️ Abandon — close it (14)

### Superseded — handled another way on `main`

| PR | Title | Author | Reason still open | Conflicts? | Why close |
|---:|-------|--------|-------------------|:----------:|-----------|
| [#11847](https://github.com/open-mpi/ompi/pull/11847) | Avoid claiming copyright for future years | bmwiedemann | Superseded by merged work | yes | `conf.py` now uses `SOURCE_DATE_EPOCH`. |
| [#12285](https://github.com/open-mpi/ompi/pull/12285) | test/datatype: do not use installed components | ggouaillardet | Superseded by merged work | no | Approach deemed wrong; root cause fixed by removing `mca/pmix`. |
| [#12303](https://github.com/open-mpi/ompi/pull/12303) | framework: add NOCOMPONENT flag | ggouaillardet | Superseded by merged work | no | `mca/pmix` framework removed; flag is dead-on-arrival. |
| [#12826](https://github.com/open-mpi/ompi/pull/12826) | mca/base: new MCA var type for include lists | hjelmn | Superseded by merged work | yes | btl/uct shipped a component-local include list instead. |
| [#13195](https://github.com/open-mpi/ompi/pull/13195) | btl/uct: connections via separate memory domain | hjelmn | Superseded by merged work | yes | Folded into the landed btl/uct re-work (`112b4f813b`). |
| [#13450](https://github.com/open-mpi/ompi/pull/13450) | Add Open Cluster Scheduler to docs | dgruber | Superseded by merged work | no | Re-homed to PRRTE (openpmix/prrte#2300). |

### Author abandoned / dead premise

| PR | Title | Author | Reason still open | Conflicts? | Why close |
|---:|-------|--------|-------------------|:----------:|-----------|
| [#6426](https://github.com/open-mpi/ompi/pull/6426) | MCA parameters for memcpy chunk size | bosilca | Author abandoned | yes | Use case gone (UCX); 7+ years stale. |
| [#9131](https://github.com/open-mpi/ompi/pull/9131) | configury: allow multiple "-include" | ggouaillardet | Author abandoned | yes | Author said the fix belongs in PMIx, not OMPI. |
| [#12318](https://github.com/open-mpi/ompi/pull/12318) | Offload reductions to accelerators | devreal | Author abandoned | yes | Monolith never landed; keep only as reference for scoped PRs. |
| [#13516](https://github.com/open-mpi/ompi/pull/13516) | [WIP] env vars for libfabric cxi provider | copilot-swe-agent | Author abandoned (bot stub) | no | Empty Copilot-bot draft, zero changes, no owner. |

### Parked prototype not worth reviving

| PR | Title | Author | Reason still open | Conflicts? | Why close |
|---:|-------|--------|-------------------|:----------:|-----------|
| [#10470](https://github.com/open-mpi/ompi/pull/10470) | [WIP] new shared memory collectives component | devreal | Parked WIP / DNM | no | `coll/sm` removed; goals overlap `coll/xhc`/`coll/acoll`. |
| [#12725](https://github.com/open-mpi/ompi/pull/12725) | opal_thread integration with libevent | npe9 | Parked WIP / DNM | no | Untested, breaks CI, unsolved PMIx/libevent coexistence. |

### Design rejected / superseded by ongoing work

| PR | Title | Author | Reason still open | Conflicts? | Why close |
|---:|-------|--------|-------------------|:----------:|-----------|
| [#12569](https://github.com/open-mpi/ompi/pull/12569) | Add CUDA/HIP reduction operators | devreal | Unresolved design disagreement | yes | Core reviewer rejects the architecture; author stepped back. Restart fresh if revived. |
| [#13133](https://github.com/open-mpi/ompi/pull/13133) | MPI_T Events | kingshuk00 | Unresolved design disagreement | yes | Design needs rethink; overlaps maintainer's ongoing MPI_T events work — reconcile there. |

---

## At-a-glance matrix (outcome × reason)

| Reason still open ↓ / Outcome → | Merge | Update | Abandon |
|---|:---:|:---:|:---:|
| Superseded by merged work | | | 6 |
| Author abandoned | | | 4 |
| Parked WIP / DNM | | 3 | 2 |
| Awaiting author revision | | 8 | |
| Unresolved design disagreement | | 3 | 2 |
| Never reviewed | | 2 | |
| Blocked on dependency / upstream | | 2 | |
| Approved but never merged | 1 | | |
| **Total** | **1** | **18** | **14** |
