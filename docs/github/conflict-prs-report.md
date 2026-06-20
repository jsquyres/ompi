# Triage report: stale, conflicting open PRs

This report covers every open pull request in
[`open-mpi/ompi`](https://github.com/open-mpi/ompi) that, as of the
analysis date, **both** (a) has merge conflicts against its base branch
and (b) has had no activity (GitHub `updatedAt`) in more than six
months.

- **Analysis date:** 2026-06-20
- **Staleness cutoff:** last activity before 2025-12-20
- **PRs in scope:** 20 (all target `main`; all currently
  `mergeable: CONFLICTING` / `mergeStateStatus: DIRTY`)

Each PR was inspected for scope (changed files), the discussion / review
/ CI state that stalled it, and — where determinable — whether the work
was later merged, superseded, or is still missing from the tree. Each
carries one of five assessment labels and a tailored draft "is this
still relevant?" ping comment that a maintainer can post as-is.

> **Caveat:** "last activity" is GitHub's `updatedAt`, which bumps on any
> event (commit, comment, label). Conflict status and "is it in the tree
> today" were spot-checked against the local checkout, but a few
> superseded/closeable calls note a residual item to diff before closing.

## Assessment labels

| Label | Meaning | Default action |
|-------|---------|----------------|
| **Superseded** | The change (or its motivating need) has since been handled another way on `main`. | Close (after a quick confirming diff). |
| **Abandoned** | Author stated, or the record strongly implies, the work is dead; not in the tree. | Close. |
| **Needs author input** | Plausibly still wanted, but blocked on an unresolved design question or the author's stated intent. | Ping author / maintainer decision. |
| **Salvageable** | Underlying need is still real and not superseded; mainly needs a rebase + finish. | Ping to rebase / find an owner. |
| **Still relevant** | Fixes a bug verifiably still present today; should be revived. | Revive / cherry-pick the fix. |

## Summary

| PR | Title | Author | Last activity | Assessment |
|---:|-------|--------|---------------|------------|
| [#11847](https://github.com/open-mpi/ompi/pull/11847) | Avoid claiming copyright for future years | bmwiedemann | 2024-04-10 | **Superseded** |
| [#12826](https://github.com/open-mpi/ompi/pull/12826) | mca/base: add a new MCA variable type for include lists | hjelmn | 2025-02-26 | **Superseded** |
| [#13195](https://github.com/open-mpi/ompi/pull/13195) | btl/uct: connections via a separate memory domain | hjelmn | 2025-04-18 | **Superseded** |
| [#12318](https://github.com/open-mpi/ompi/pull/12318) | Offload reduction operations to accelerator devices | devreal | 2024-02-23 | **Superseded / Abandoned** |
| [#6426](https://github.com/open-mpi/ompi/pull/6426) | Add MCA parameters to define the size of memcpy chunks | bosilca | 2022-03-25 | **Abandoned** |
| [#9131](https://github.com/open-mpi/ompi/pull/9131) | configury: allow multiple "-include \<file>" options | ggouaillardet | 2022-03-25 | **Abandoned** |
| [#9867](https://github.com/open-mpi/ompi/pull/9867) | Remove all ops with 3 buffers | bosilca | 2022-04-27 | **Needs author input** |
| [#9502](https://github.com/open-mpi/ompi/pull/9502) | Implementation of the MPI Continuations proposal [WIP] | devreal | 2025-02-19 | **Needs author input** |
| [#12569](https://github.com/open-mpi/ompi/pull/12569) | Add CUDA/HIP implementations of reduction operators | devreal | 2024-10-01 | **Needs author input** |
| [#12935](https://github.com/open-mpi/ompi/pull/12935) | part: multithreading deadlocks fixes and safety checks | Keluaa | 2024-11-28 | **Needs author input** |
| [#12570](https://github.com/open-mpi/ompi/pull/12570) | Make datatype copy stream-aware | devreal | 2024-05-23 | **Needs author input** |
| [#13133](https://github.com/open-mpi/ompi/pull/13133) | MPI_T Events | kingshuk00 | 2025-06-03 | **Needs author input** |
| [#13527](https://github.com/open-mpi/ompi/pull/13527) | pml/ob1: new protocol for intermediate-sized messages | hjelmn | 2025-12-01 | **Needs author input** |
| [#12309](https://github.com/open-mpi/ompi/pull/12309) | pmix: remove the MCA framework | ggouaillardet | 2024-02-09 | **Salvageable** |
| [#9812](https://github.com/open-mpi/ompi/pull/9812) | ompi_fortran_check_ignore_tkr.m4: fix fortran test errors | jsquyres | 2024-02-13 | **Salvageable** |
| [#12453](https://github.com/open-mpi/ompi/pull/12453) | mca/coll: any radix k for alltoall bruck algorithm | jiaxiyan | 2024-07-12 | **Salvageable** |
| [#12991](https://github.com/open-mpi/ompi/pull/12991) | coll/accelerator: allow selecting functions to register | edgargabriel | 2025-01-14 | **Salvageable** |
| [#13131](https://github.com/open-mpi/ompi/pull/13131) | Fix: MPI_Type_get_contents max array sizes | EmmanuelBRELLE | 2025-06-12 | **Salvageable** |
| [#13088](https://github.com/open-mpi/ompi/pull/13088) | RFC: equivalence of MPICH_ASYNC_PROGRESS | hominhquan | 2025-10-04 | **Salvageable** |
| [#12972](https://github.com/open-mpi/ompi/pull/12972) | part_persist: fix start_all() | ggouaillardet | 2024-12-12 | **Still relevant** |

**Tally:** Superseded 3 (+1 part-superseded), Abandoned 2, Needs author
input 7, Salvageable 6, Still relevant 1.

### Triage shortlist

- **Close now (superseded/abandoned):** #11847, #12826, #13195, #6426,
  #9131 — and #12318 as a monolith (keep only as reference if desired).
- **Revive promptly (real bug still in tree):** #12972 — the
  `start_all()` index-reuse bug is still present and issue
  [#12969](https://github.com/open-mpi/ompi/issues/12969) is still open;
  the one-line fix could be cherry-picked ahead of the larger refactor.
- **Ping author for a decision:** all **Needs author input** PRs, plus
  the **Salvageable** ones that need a rebase commitment.
- **Special case:** #13133 (MPI_T Events) overlaps the maintainer's own
  ongoing MPI_T events work — reconcile, don't just rebase.

---

## Superseded — recommend closing

### PR #11847 — Avoid claiming copyright for future years

- **Author / age / size:** bmwiedemann · opened 2023-08-04 · last activity 2024-04-10 · +1/-2 across 1 file · ready
- **What it touches:** A one-line change to `docs/conf.py` so the rendered docs copyright string ("2003-YYYY, The Open MPI Community") doesn't claim a future year when the build machine's clock is set ahead (relevant to reproducible / time-traveling builds). Purely documentation-metadata; no code-path impact.
- **Why it stalled:** Maintainers (jsquyres, rhc54) preferred deriving the year from an environment-variable override (matching OpenPMIx PR #3112) and/or freezing it at `make dist` rather than the PR's approach. The PR also repeatedly failed the Signed-off-by CI check. The author pinged in April 2024 with no resolution.
- **Assessment:** **Superseded.** The current `docs/conf.py` already derives the copyright year from `SOURCE_DATE_EPOCH` (the reproducible-builds standard) with a fallback to the current year — exactly what the discussion landed on. The PR's specific change is now moot.
- **Draft ping comment:**
  > Thanks for raising this back in 2023, and apologies it sat so long. This PR now has a merge conflict, but more importantly the underlying concern has since been addressed a different way: `docs/conf.py` now derives the copyright year from `SOURCE_DATE_EPOCH` (the reproducible-builds convention) with a fallback to the current year, which matches what the discussion here landed on. Unless you see a remaining gap, I think this can be closed as resolved — please let me know if you disagree and we'll reopen the discussion.

### PR #12826 — mca/base: add a new MCA variable type for include lists

- **Author / age / size:** hjelmn (Nathan Hjelm) · opened 2024-09-24 · last activity 2025-02-26 · +910/-26 across 15 files · ready
- **What it touches:** Formalizes a reusable `MCA_BASE_VAR_TYPE_INCLUDE_LIST` MCA variable type backed by a new `opal_include_list` object (plus a supporting `opal_serializable` class) in `opal/class/` and `opal/mca/base/mca_base_var.{c,h}`, centralizing the include/exclude-list parsing scattered across frameworks (regex support added later) and exposing it to MPI_T. Adds extensive unit tests under `test/class/` and `test/mca/`. The cited consumer was `btl_uct_memory_domains`.
- **Why it stalled:** bosilca called it "an overblown solution" for a capability the project has lived without; rhc54 asked for a concrete in-tree usage example. Discussion trailed off after Feb 2025.
- **Assessment:** **Superseded.** The motivating use case was instead solved by a component-local implementation (`opal/mca/btl/uct/btl_uct_include_list.h`, `mca_btl_uct_include_list_*`) that landed in the Feb 2025 btl/uct rework; `MCA_BASE_VAR_TYPE_INCLUDE_LIST` does not exist in the current `opal/mca/base/`. Reviving the generalized version would need fresh justification given the lukewarm review.
- **Draft ping comment:**
  > Hi @hjelmn — going through stale PRs. This one is about 21 months old and now has merge conflicts against `main`. The review thread raised some doubts about whether a generalized include-list variable type was warranted, and it looks like the btl/uct include-list need was ultimately handled with a component-local implementation (`btl_uct_include_list.h`) rather than this `opal/mca/base` type. Given that, is there still a use case you want to pursue here, or should we close this in favor of the component-local approach? Thanks!

### PR #13195 — btl/uct: allow connections to be formed using a separate memory domain

- **Author / age / size:** hjelmn (Nathan Hjelm) · opened 2025-04-14 · last activity 2025-04-18 · +459/-217 across 6 files · ready
- **What it touches:** Reworks the `btl/uct` component (`opal/mca/btl/uct/`: `btl_uct_component.c`, `btl_uct_endpoint.c`, `btl_uct_tl.c`, plus headers) so that when the active memory domain lacks a transport capable of forming endpoint-to-endpoint connections, an alternate transport (typically TCP) can establish those connections — keeping the BTL functional on memory domains that otherwise can't connect.
- **Why it stalled:** Only a brief bosilca/author exchange over a few days in April 2025, then silence.
- **Assessment:** **Superseded.** The same author's larger commit "btl/uct: complete re-work of the BTL" (`112b4f813b`, authored 2025-02-27) already lists, as its first change, "Disconnect the connection memory domain from the communication domain… The default is to use tcp" — i.e., this PR's exact feature. Worth a quick diff for any incremental fixes here that didn't make it into the rework before closing.
- **Draft ping comment:**
  > Hi @hjelmn — PR triage. This one is about 14 months old and now conflicts with `main`. It looks like its core idea — forming connections over a separate (e.g. TCP) memory domain — was folded into your larger "btl/uct: complete re-work of the BTL" commit that has since landed on `main`. Is there anything still unique in this PR that didn't make it into the rework, or has it been fully superseded and can be closed? Thanks!

### PR #12318 — Offload reduction operations to accelerator devices

- **Author / age / size:** devreal (Joseph Schuchart) · opened 2023-11 (commits back to 2023-03) · last activity 2024-02-23 · +10,915/-… across ~60 files · ready (explicitly WIP)
- **What it touches:** A large WIP that offloads `MPI_Allreduce` reductions to accelerators when buffers are in device memory. Adds CUDA/ROCm `op` components, stream-aware reduction/datatype-copy plumbing through the op API, device memory pools/allocators, 3buff op variants in `coll/base` allreduce, and accelerator-framework changes. Key areas: `ompi/mca/op/{cuda,rocm}`, `ompi/op`, `coll/base/*allreduce*`, `opal/.../accelerator`.
- **Why it stalled:** edgargabriel asked for it to be split into smaller reviewable PRs (accelerator framework / op framework / glue); the author agreed. AWS benchmarking found `ireduce`/`iallreduce` segfaults. The author then ran out of time, leaving it as a starting point for others.
- **Assessment:** **Superseded (in part) / Abandoned as a monolith.** The decomposition partially succeeded — dependency PR #12356 ("Add stream operations to accelerator components") merged 2024-06-12 — but the core never landed: no `ompi/mca/op/cuda` or `op/rocm` and no `ompi_op_reduce_stream` in the tree. Residual value is as a reference for the still-incomplete device-op work.
- **Draft ping comment:**
  > Thanks again for this substantial device-offload work. It's been about two and a half years since the last activity here (Feb 2024) and the branch now has merge conflicts against a much-changed tree. I see the accelerator-framework stream pieces were split out and merged as #12356, but the op/cuda + op/rocm components and the stream-aware reduction path in this PR never landed and aren't in the tree today. Given the agreed plan to break this into smaller reviewable pieces, would you prefer to keep this open as a reference, or should we close it in favor of fresh, scoped PRs for the remaining op-framework and component work? No rush — just triaging long-stale PRs.

---

## Abandoned — recommend closing

### PR #6426 — Add MCA parameters to define the size of memcpy chunks

- **Author / age / size:** bosilca · opened 2019-02-23 · last activity 2022-03-25 · +152/-58 across 4 files · ready
- **What it touches:** Adds MCA parameters to control the size of the (CUDA) memcpy chunks used by the OPAL datatype copy engine, and support for upper-level specialized "vector copy" functions. Modifies `opal/datatype/`: `opal_datatype_copy.c`, `opal_datatype_copy.h`, `opal_datatype_internal.h`, `opal_datatype_module.c`. Motivated by poor CUDA memcpy performance from a fixed 128k chunk size.
- **Why it stalled:** IBM CI failed early and was never fixed; after repeated pings, the author said (2021-01) he had no time and that the original use case disappeared once the communication path was delegated to UCX. The "critical" label was removed and it was punted to v6.0.
- **Assessment:** **Abandoned.** Author explicitly said the motivating use case went away (UCX took over the path); no memcpy-chunk MCA parameter exists in `opal_datatype_module.c` today, so it was never merged or superseded by equivalent work.
- **Draft ping comment:**
  > Hi @bosilca — as part of an open-PR triage sweep, I'm revisiting this one. It was opened back in February 2019 (over 7 years ago), last saw activity in 2022, and now has merge conflicts against `main`. Back in early 2021 you noted the original use case had largely disappeared once communication was delegated to UCX. Is the vector-copy / configurable-chunk-size work still something you'd want to pursue, or should we go ahead and close this? Happy to keep it open if you intend to rebase and revive it.

### PR #9131 — configury: allow multiple "-include \<file>" options

- **Author / age / size:** ggouaillardet · opened 2021-07-09 · last activity 2022-03-25 · +6/-2 across 1 file · DRAFT
- **What it touches:** A tiny configury fix to `config/opal_functions.m4` intended to let the compiler-flag handling tolerate multiple `-include <file>` options (refs issue #9129).
- **Why it stalled:** During review jsquyres questioned `--include` vs `-include`; the author then concluded in-thread that "Open MPI is just fine, and only PMIx has to be modified" — the real fix belonged upstream in PMIx. Left as a draft; an IBM PGI CI build also failed.
- **Assessment:** **Abandoned.** The author's own investigation determined the change wasn't needed in Open MPI, so the PR has no clear purpose; no equivalent `-include` handling exists in `opal_functions.m4` today.
- **Draft ping comment:**
  > Hi @ggouaillardet — triaging stale PRs. This draft is from July 2021 (nearly 5 years old), last touched in 2022, and now conflicts with `main`. Notably, in the review thread you concluded that Open MPI itself was fine and that the actual fix needed to go into PMIx instead. Given that, is there anything still worth keeping here, or should we close this draft? Let me know if you'd prefer to rebase it for some remaining reason.

---

## Needs author input — maintainer decision required

### PR #9867 — Remove all ops with 3 buffers

- **Author / age / size:** bosilca · opened 2022-01-12 · last activity 2022-04-27 · +2/-856 across 10 files · ready
- **What it touches:** Removes the `MPI_Op` machinery taking three buffer arguments (two sources + one target), on the rationale that nothing in the code base used it. Deletes 3buff handlers across `ompi/op/op.{c,h}`, `ompi/mca/op/op.h`, the op base (`functions.h`, `op_base_functions.c`, `op_base_frame.c`, `op_base_op_select.c`), and the AVX component. Deliberately held pending MPI Forum issue #530, which might reintroduce a need.
- **Why it stalled:** Carries a "WIP-DNM!" label; jsquyres approved and agreed the stale code should go (revivable if Forum #530 standardizes), but it was never merged and discussion stopped in early 2022.
- **Assessment:** **Needs author input.** Approved but now more invasive/stale: the 3buff code has since grown into new `aarch64` and `riscv64` op components that didn't exist in 2022, so a substantial rebase is needed; the status of Forum #530 should be reconfirmed before reviving or closing.
- **Draft ping comment:**
  > Hi @bosilca — doing some open-PR triage. This one is from January 2022 (over 4 years old), still carries the WIP-DNM label, and now has merge conflicts. It was approved at the time but parked pending MPI Forum issue #530. Two things worth noting: (1) the 3-buffer op code has since spread into new `aarch64` and `riscv64` op components, so this PR would need a substantial rebase to be complete; and (2) it would be good to reconfirm where Forum #530 landed. Do you still want to remove the 3-buffer ops, or should we close this and revisit if/when #530 progresses?

### PR #9502 — Implementation of the current state of MPI Continuations proposal [WIP]

- **Author / age / size:** devreal (Joseph Schuchart) · opened 2021-10 · last activity 2025-02-19 · +2462/-… across 32 files · ready (WIP in body)
- **What it touches:** Adds an Open MPI extension implementing the MPI Continuations proposal (`MPIX_Continue`, `MPIX_Continueall`, `MPIX_Continue_init`) for attaching callbacks to outstanding request completions, tracking the evolving MPI Forum working-group proposal. The bulk is a new `mpiext` extension plus hooks in the request test/wait paths. No continuation extension currently exists under `ompi/mpiext/`, so it has not landed.
- **Why it stalled:** Long-lived experimental PR tied to a still-unratified Forum proposal; CI repeatedly flagged a commit missing `Signed-off-by` (never fixed); most recent activity is design Q&A (Jan–Feb 2025), not a push to merge.
- **Assessment:** **Needs author input.** Substantial, author is a maintainer and was engaging in early 2025, but it depends on an unratified proposal, was never de-WIP'd, has an unfixed sign-off CI failure, and now conflicts. Unclear whether it should land as an experimental extension or wait on the Forum.
- **Draft ping comment:**
  > Hi @devreal — triaging long-running PRs. This MPI Continuations extension has been open since late 2021 (roughly 4.5 years) and now has merge conflicts against `main`. There's also a recurring CI sign-off failure on one of the commits that was never resolved, and the PR is still marked WIP in its description. I know this tracks an evolving MPI Forum proposal — what's the current status? Would you like to refresh it and land it as an experimental extension, keep it open, or close it for now and revisit when the proposal firms up? Thanks!

### PR #12569 — Add CUDA/HIP implementations of reduction operators

- **Author / age / size:** devreal (Joseph Schuchart) · opened 2024-05-23 · last activity 2024-10-01 · +8490/-52 across 29 files · ready
- **What it touches:** Adds GPU-side MPI reduction operators: new `ompi/mca/op/cuda/` and `ompi/mca/op/rocm/` components (macro-generated `.cu`/`.hip` kernels plus OMPI-agnostic layers), new Autoconf macros (`config/opal_check_cudart.m4`, `opal_check_nvcc.m4`, `opal_check_hipcc.m4`), and stream/device-ID variants of `ompi_op_reduce` in `ompi/op/op.h` and `ompi/mca/op/op.h`. Second part of the series following #12318; depended on #12356.
- **Why it stalled:** Large, partly-unresolved review thread; bosilca had a fundamental objection (per-op kernel-launch overhead is too costly; he favors a persistent GPU "service" model) and edgargabriel requested changes. Author signed off 2024-10-01 with "please provide a patch for changes you want. I've run out of time to spend on this." No activity since.
- **Assessment:** **Needs author input** (leaning Abandoned). Author stepped back, the device-op work is entirely absent from the tree, and a core reviewer disagrees with the basic design — it can't move without devreal returning or a new owner. Given size + unresolved architecture, a fresh rebased branch may be cleaner.
- **Draft ping comment:**
  > Hi @devreal — going through stale PRs. This one has been inactive since October 2024 (roughly 20 months) and now has merge conflicts against `main`; none of the device-op work has landed in the tree in the meantime. I know you noted at the time that you'd run out of time to spend on it. Could you let us know whether you (or anyone) still intend to carry this forward — including @bosilca's larger point about the kernel-launch/"persistent GPU service" design — or whether we should close it for now and reopen if the GPU-reduction effort is picked back up? Given the size and the unresolved architectural discussion, it may be cleaner to restart from a fresh, rebased branch. Thanks for the substantial work here regardless.

### PR #12935 — part: multithreading deadlocks fixes and safety checks

- **Author / age / size:** Keluaa (Luc Briand) · opened 2024-11-26 · last activity 2024-11-28 · +172/-148 across 3 files · ready
- **What it touches:** Fixes several multithreading bugs in the persistent partitioned-communication component (`ompi/mca/part/persist/`): adds a `req->part_ready` array so `MPI_Pready` and the progress thread no longer race on partition flags/`done_count`; serializes the two startup `ompi_comm_idup` calls to avoid an init-time deadlock; fixes a rare segfault from calling `mca_part_persist_free_req` outside the lock; adds error handling in `mca_part_persist_progress`.
- **Why it stalled:** No comments and no reviews at all — apparently never picked up by a reviewer.
- **Assessment:** **Needs author input** (and a reviewer). The part/persist subsystem is actively maintained (issue #12969 still open; PR #12972 reworked the same files), so these thread-safety fixes are plausibly still relevant, but the code was heavily restructured later, so a substantial rebase is needed and the `ompi_comm_idup` serialization (which the author notes slows init) warrants real review. Uncertain whether any of these specific races were independently fixed later.
- **Draft ping comment:**
  > Hi @Keluaa — we're triaging older PRs. This one has been open since November 2024 (about 19 months) without review, and unfortunately it now conflicts with `main` — the `part/persist` component has seen a number of changes since then. Partitioned-communications thread-safety is still an open area for us (see issue #12969), so these fixes may well still be valuable. Are you still able to rebase it onto current `main` and confirm the deadlocks/segfault you describe still reproduce there? If you're no longer in a position to carry it forward, let us know and we can decide whether to adopt the fixes or close the PR. Apologies it sat without a reviewer for so long.

### PR #12570 — Make datatype copy stream-aware

- **Author / age / size:** devreal (Joseph Schuchart) · opened 2024-05-23 · last activity 2024-05-23 · +87/-34 across 4 files · ready
- **What it touches:** The third extracted chunk of #12318. Adds `ompi_datatype_copy_content_same_ddt_stream` and an `opal_datatype_copy_content_same_ddt_stream` that accept a stream argument, submitting stream-aware memcpy/memmove (synchronizing the default stream if none given). Touches `ompi/datatype/ompi_datatype.h`, `opal/datatype/opal_datatype.h`, `opal/datatype/opal_datatype_copy.{c,h}`.
- **Why it stalled:** No review/discussion at all. Body says "Requires #12356, will rebase once that's merged." #12356 merged 2024-06-12, but the author never returned, and the parent effort (#12318) had gone dormant.
- **Assessment:** **Needs author input** (leaning Salvageable). The blocking dependency is now merged and the change is small and self-contained, but the new functions are absent from the tree and it was part of a stalled larger effort — needs the author to confirm intent and rebase.
- **Draft ping comment:**
  > This PR has been untouched since it was opened in May 2024 (just over two years) and now has merge conflicts. It was waiting on #12356, which has since been merged, so the stated blocker is cleared — but the work itself never got rebased and the new `*_copy_content_same_ddt_stream` routines aren't in the tree. Since this was the third chunk of the broader device-op effort in #12318, do you still intend to rebase and pursue it, or should it be closed alongside that stalled work? Happy to keep it open if you plan to come back to it.

### PR #13133 — MPI_T Events

- **Author / age / size:** kingshuk00 (Kingshuk Haldar) · opened 2025-03-10 · last activity 2025-06-03 · +2159/-165 across 50 files · ready
- **What it touches:** A long-running effort to implement the MPI 4.0/4.1 MPI_T Events interface, rebased from the original PR #8057 (commits back to 2018; hjelmn, cchambreau, hppritcha). Adds OPAL event/source infrastructure (`opal/mca/base/mca_base_event.{c,h}`, `mca_base_source.{c,h}`), wires events into PML ob1 and OSC rdma, and fills in the `ompi/mpi/tool/event_*.c` MPI_T binding bodies (today still stubs — e.g. `event_get_num.c` returns `*num_event = 0`). Adds an `ompi_datatype_lookup_by_opal_id` helper.
- **Why it stalled:** CI surfaced a deadlock (recursive locking of `mca_base_source_lock` during MCA var init); bosilca called the locking "very sketchy"; two unresolved design comments from #8057 were never settled. By 2025-06-03 the author and hppritcha agreed it was too ambitious for 6.0.x and the basic design should be reconsidered.
- **Assessment:** **Needs author input** (leaning redesign). The feature is wanted and the target code exists only as stubs (not superseded by a merged implementation), but maintainers said the design needs rethinking. **Overlaps the maintainer's own ongoing MPI_T events work** — there is a local `pr/mpi-t-events` branch and `howard/topic/mpi_t_events` remote branches, plus recently merged MPI_T category/event metadata work (e.g. #14077). Reconcile rather than rebase as-is.
- **Draft ping comment:**
  > Hi @kingshuk00 — doing some PR triage. This MPI_T Events PR has been idle since June 2025 (about a year now) and has gone stale: it no longer merges cleanly against `main` (merge conflicts). When it was last discussed, there was consensus that the underlying event/source design needed reconsideration before this could land, and there is now separate ongoing MPI_T events work on the maintainer side that this would need to be reconciled against. Could you let us know whether you still intend to drive this forward (in which case we should coordinate on the design and a rebase), or whether it would be cleaner to close it and fold the effort into the current MPI_T events work? Thanks for all the effort that has gone into this.

### PR #13527 — pml/ob1: introduce a new protocol for intermediate-sized messages

- **Author / age / size:** hjelmn (Nathan Hjelm) · opened 2025-11-21 · last activity 2025-12-01 · +670/-142 across 12 files · ready
- **What it touches:** Adds a "multi-eager" protocol to the ob1 PML for intermediate-sized messages: the sender fragments into multiple eager-sized messages sent in parallel; if matched to a posted receive, the receiver copies each fragment directly into the user buffer without extra buffering. Disabled by default; enabled when a BTL's new `multi_eager_limit` exceeds its `eager_limit`. Touches most of `ompi/mca/pml/ob1/` plus `opal/mca/btl/btl.h` and `btl_base_mca.c`. Inspired by OpenUCX's multi-send eager protocol.
- **Why it stalled:** bosilca pushed back hard: unwarranted changes to critical send/match code, concern it deprioritizes incoming traffic in matching, and skepticism vs. simply using a larger eager limit. hjelmn responded with PingPong/memory data (multi-eager ~74MB vs ~1.2GB RSS) and offered to split/revert the match-code reorg, but the exchange ended (2025-12-01) without resolution or a revision.
- **Assessment:** **Needs author input.** Newest PR in scope (CI was green), so not abandoned, but blocked on an unresolved design disagreement with a core reviewer and a promised refactor that wasn't pushed. Needs hjelmn to revise per feedback or reach agreement on whether the protocol belongs in ob1.
- **Draft ping comment:**
  > Hi @hjelmn — PR triage sweep. This multi-eager protocol PR has been idle since early December 2025 (roughly half a year) and now has merge conflicts against `main`, even though CI was green at the time. The review thread with @bosilca raised substantive concerns (changes to the critical send/match path, matching-priority effects, and whether the protocol's benefit can't be had with a larger eager limit), and you'd offered to split out or revert the match-code reorganization and provided supporting memory/latency data — but no revised version was pushed. Are you still planning to take this forward (rebase + the discussed refactor / a resolution with @bosilca), or should we close it for now? Given it touches hot ob1 code, we'd like to either move it toward a clean review or retire it rather than leave it stale.

---

## Salvageable — rebase + finish (find an owner)

### PR #12309 — pmix: remove the MCA framework

- **Author / age / size:** ggouaillardet · opened 2024-02-05 · last activity 2024-02-09 · +269/-418 across 67 files · DRAFT
- **What it touches:** A broad refactor removing the OPAL `pmix` MCA framework entirely (deleting `opal/mca/pmix/` and its base) and promoting PMIx to a "first class citizen" under a new `opal/pmix/` directory, since PMIx no longer needs to be pluggable (refs #12282). Sweeping `#include` updates across OMPI/OPAL/OSHMEM plus new `opal/pmix/{Makefile.am,pmix.c,pmix-internal.h,help-pmix-base.txt}`.
- **Why it stalled:** Removing the framework dropped the `pmix*` MCA parameters (notably `pmix_base_verbose`) from `ompi_info` — a potential "deal breaker for v5." rhc54 and ggouaillardet worked out a fix in-thread (add a `pmix_base_init` rather than declaring a framework), but it never made it into the PR; discussion ended 2024-02-09.
- **Assessment:** **Salvageable.** Nearly complete with active, constructive maintainer collaboration just before it went quiet; the refactor was never landed (`opal/mca/pmix/` still exists, `opal/pmix/` does not). Needs the agreed `ompi_info`/param fix and a rebase across 2+ years of churn in 67 files.
- **Draft ping comment:**
  > Hi @ggouaillardet — triaging stale PRs. This draft is from February 2024 (about 2.5 years old) and now has merge conflicts, which isn't surprising given it touches 67 files. The discussion with @rhc54 had converged on how to keep the `pmix_base_*` params (including `pmix_base_verbose`) visible in `ompi_info` via a small `pmix_base_init` instead of declaring a framework, but that fix never made it into the PR. Is removing the OPAL `pmix` MCA framework still on your roadmap? If so, are you able to rebase and apply the agreed param fix — otherwise should we close it for now?

### PR #9812 — ompi_fortran_check_ignore_tkr.m4: fix fortran test errors

- **Author / age / size:** jsquyres · opened 2022-01-02 · last activity 2024-02-13 · +257/-198 across 35 files · DRAFT
- **What it touches:** A multi-pronged fix to make the Fortran bindings work with the NAG compiler (issue #9795). Corrects bugs in three configure-time m4 tests (`ompi_fortran_check_ignore_tkr.m4`, `ompi_fortran_check_use_only.m4`, `ompi_fortran_get_alignment.m4`) and reworks the Fortran "sentinel" constants (MPI_BOTTOM, MPI_IN_PLACE, MPI_STATUS[ES]_IGNORE, …) to use `bind(C)` instead of relying on common blocks + 4-way symbol mangling. Adds `mpi-f08-sentinels.F90`, `mpi-sentinels.F90`, `mpif-status.h`; edits `gen-mpi-mangling.pl`, `gen-mpi-sizeof.pl`, and many `use-mpi-f08/*.F90` files.
- **Why it stalled:** The author (a maintainer) wrote that he "was unable to bring it to completion" — the module dependency graph kept producing NAG errors and IBM CI failures, and his availability dropped. Parked as a draft to preserve the work.
- **Assessment:** **Salvageable** (with significant rework). The underlying NAG bug is still real: the `bind(C, name="cmmon_")` test in `ompi_fortran_check_use_only.m4` and the common-block sentinels in `ompi/include/mpif-sentinels.h` remain unchanged in the tree. But the `mpi_f08` bindings were rewritten to be generated (Aug 2024) and the Perl mangling scripts this PR edits were removed/ported to Python (May 2025), so the specific patches no longer apply and the approach must be re-implemented against the new generator.
- **Draft ping comment:**
  > This draft has been sitting since early 2022 (last touched Feb 2024), and the tree has moved substantially underneath it: the `mpi_f08` bindings are now generated and the Perl mangling scripts this PR edits have since been removed or ported to Python, so it now has merge conflicts and would no longer apply cleanly. The NAG compiler issue it targets (#9795) does still appear to be unfixed, though — the common-block sentinels and the `use...only` configure test remain as they were. Do you intend to revive this against the new bindings generator, or should we close it and track the remaining NAG work fresh in #9795? Happy to either way; just want to clear stale drafts.

### PR #12453 — mca/coll: Add any radix k for alltoall bruck algorithm

- **Author / age / size:** jiaxiyan (Jessie Yang) · opened 2024-04-05 · last activity 2024-07-12 · +130/-45 across 4 files · ready
- **What it touches:** Extends `ompi_coll_base_alltoall_intra_bruck` in `ompi/mca/coll/base/coll_base_alltoall.c` to support an arbitrary radix *k* (rather than fixed radix-2 Bruck), with the radix wired through `coll_base_functions.h` and the tuned-collective decision logic (`coll_tuned_alltoall_decision.c`, `coll_tuned_decision_fixed.c`). Lets users trade latency vs. bandwidth on large-rank all-to-all; backward-compatible, defaulting to k=2.
- **Why it stalled:** Review found the code acceptable, but bosilca questioned the justification (the ~128-byte latency jump), which wenduwan attributed to EFA/libfabric behavior rather than the algorithm. The author then rotated to another project (2024-07-10); burlen's request for user-guide docs was never addressed.
- **Assessment:** **Salvageable.** The code area is unchanged (current Bruck is still fixed-radix; tuned decision still calls the 2-radix form), so the patch is conceptually applicable, but it needs a rebase, the requested user-guide docs, and a maintainer to re-engage on bosilca's open question.
- **Draft ping comment:**
  > Hi @jiaxiyan — we're triaging stale PRs. This one has been idle for almost two years (last activity July 2024) and now conflicts with `main`. The Bruck all-to-all code it modifies has shifted since then (including the recent Big Count pass on the coll framework), so it no longer applies cleanly. Is the variable-radix-k work still something you or the AWS team intend to pursue? If so, a rebase plus the user-guide/MCA-parameter documentation burlen asked for would be the next step, and it would be good to resolve @bosilca's open question about the source of the small-message latency jump. If not, we'll plan to close it — just let us know. Thanks!

### PR #12991 — coll/accelerator: allow to select functions to register

- **Author / age / size:** edgargabriel · opened 2024-12-19 · last activity 2025-01-14 · +123/-12 across 3 files · ready
- **What it touches:** Adds an MCA-parameter-driven mechanism to `coll/accelerator` (`coll_accelerator.h`, `coll_accelerator_component.c`, `coll_accelerator_module.c`) so the component can register only a selected subset of MPI collectives via a bitmask string, modeled on `coll/ucc`. Adds bitmask `#define`s for all collectives in anticipation of future ones; author notes the logic might later move to `coll/base`.
- **Why it stalled:** Carries a `⚠️ WIP-DNM!` label even though devreal approved it (2025-01-14); the conflict + DNM label left it sitting.
- **Assessment:** **Salvageable.** Approved and self-contained, and the target component is still actively developed (bcast/allgather/alltoall/reduce_scatter added in 2025), but the selective-registration feature never landed (current `coll_accelerator.h` has no bitmask defines). Needs a rebase and DNM removal.
- **Draft ping comment:**
  > Hi @edgargabriel — doing some PR triage. This one is about 18 months old now and has developed merge conflicts against `main`. It was actually approved by @devreal back in January 2025, but it still carries the `⚠️ WIP-DNM!` label and was never merged. The `coll/accelerator` component has continued to evolve since then (several new collectives were added). Is this still something you'd like to land? If so, could you rebase and drop the DNM label; otherwise, should we close it? Thanks!

### PR #13131 — [Fix] Provided MPI_type_get_content array sizes may exceed actual ones, do not use them

- **Author / age / size:** EmmanuelBRELLE (Brelle Emmanuel) · opened 2025-03-07 · last activity 2025-06-12 · +9/-1 across 1 file · ready
- **What it touches:** A tiny correctness fix in `ompi/mpi/c/type_get_contents.c`. Per MPI 4.1 (p.166), the user-supplied `max_integers`/`max_addresses`/`max_datatypes` may legitimately exceed the actual counts; the original code copied/destroyed datatypes based on the user-provided max rather than the real content count, risking out-of-bounds access / segfault. The fix uses the true counts from `ompi_datatype_get_args`.
- **Why it stalled:** bosilca questioned whether it was needed (pointing to a guard in `ompi_datatype_args.c`) and noted the error path also leaks memory; he said "let me take a stab at it." devreal left CHANGES_REQUESTED. Then quiet — bosilca's last activity is a bare COMMENTED review on 2025-06-12 with no resolution; no further pushes from the author.
- **Assessment:** **Salvageable** (the underlying bug is real). The current `type_get_contents.c` does now validate counts and use internal values, so the worst segfault may already be mitigated, but it's unclear whether bosilca's promised deeper fix in `ompi_datatype_get_args` ever landed, and the requested leak-free handling doesn't appear completed here. Confirm with maintainers whether the concern is already resolved upstream.
- **Draft ping comment:**
  > Hi @EmmanuelBRELLE — PR triage here. This fix has been idle since mid-June 2025 (over a year) and now has merge conflicts against `main`. The bug it targets (using the user-provided `max_*` sizes instead of the actual content counts in `MPI_Type_get_contents`) looks real, but the discussion stalled: @bosilca had suggested handling it more thoroughly inside `ompi_datatype_get_args` (also addressing a memory leak in the error path) and @devreal requested changes. Could you let us know whether you still want to pursue this — i.e. rebase and address the review feedback — or whether the underlying issue has since been fixed another way and this can be closed? It's a small, valuable fix, so we'd like to see it land or be properly retired.

### PR #13088 — RFC: Provide equivalence of MPICH_ASYNC_PROGRESS

- **Author / age / size:** hominhquan (Minh Quan Ho) · opened 2025-02-11 · last activity 2025-10-04 · +293/-43 across 16 files · ready
- **What it touches:** Re-enables an OPAL software async-progress thread (long-planned but never implemented) that runs `opal_progress()` on a dedicated thread, gated at configure (`--enable-progress-threads`) and at runtime via `opal_async_progress`/`mpi_async_progress` MCA params / `OPAL_ASYNC_PROGRESS=1`. Touches `opal/runtime/opal_progress.{c,h}`, `opal_params_core.*`, `config/opal_configure_options.m4`, request test/finalize paths, the smcuda BTL, and adds `docs/launching-apps/progress_thread.rst`. Follow-up to #13074.
- **Why it stalled:** Substantive, mostly-positive debate but no merge. devreal and bosilca were broadly in favor ("a step in the right direction"; "no harm for users who don't enable it") but raised concerns: thread binding, large unexplained OSU swings (3× slowdowns / 2× speedups), and smcuda's separate progress thread. jsquyres asked for docs (now added). It never got final approval; latest comment is an Oct 2025 benchmark suggestion from a third party.
- **Assessment:** **Salvageable / Still relevant.** The opt-in design is wanted by multiple maintainers and the target code (`opal_progress.c`) still has no async-progress thread, so it is not superseded. Mainly needs a rebase and a maintainer to push it over the line (possibly with the thread-binding follow-up).
- **Draft ping comment:**
  > Hi @hominhquan — PR triage. This async-progress RFC has gone quiet since around October 2025 and now has merge conflicts against `main`. The discussion was largely favorable — it's opt-in, low-risk for users who don't enable it, and you added the requested documentation — but it never got a final approval and merge, and there were open notes about thread binding and the smcuda BTL's separate progress thread. Could you rebase onto current `main` and confirm you're still interested in landing this? We think the opt-in async-progress capability is worth having; it just needs a conflict-free branch and a final review pass. If you'd rather not continue, let us know and we'll decide whether someone else picks it up or we close it.

---

## Still relevant — revive

### PR #12972 — part_persist: fix start_all()

- **Author / age / size:** ggouaillardet (Gilles Gouaillardet) · opened 2024-12-09 · last activity 2024-12-12 · +524/-526 across 3 files · ready
- **What it touches:** Two commits in `ompi/mca/part/persist/`. The first is a one-line-class bug fix to `mca_part_persist_start()`: the inner partition loop reused the outer loop's index `i`, corrupting the outer iteration (fixes issue #12969, "Partitioned communications hang"). The second is a pure refactor moving ~500 lines out of the header `part_persist.h` into a new `part_persist.c` (plus `part_persist_component.c`) for debuggability.
- **Why it stalled:** bosilca reviewed and noted that while it mostly moves code, he spotted several pre-existing issues worth revising; a rebase cleared an mpi4py CI failure. No activity after 2024-12-12 — simply not driven to completion.
- **Assessment:** **Still relevant / Salvageable.** The `start_all` bug it fixes is verifiably *still present* (`part_persist.h` lines ~500 and ~516 still reuse the same `i`), issue #12969 is still **open**, and a `part_persist.c` now exists but only holds the component struct — the refactor was never landed. The fix should be revived; the one-line bug-fix commit could be cherry-picked on its own ahead of the larger refactor.
- **Draft ping comment:**
  > Hi @ggouaillardet — triaging stale PRs. This has been idle since December 2024 (about 18 months) and now conflicts with `main`. Worth flagging that it's still relevant: the `mca_part_persist_start()` index-reuse bug it fixes is still present in `part_persist.h` today, and issue #12969 remains open. Could you rebase it? Given @bosilca's review caught some additional pre-existing issues in the moved code, one option is to land the small `start_all()` bug fix as its own commit first (so #12969 gets resolved promptly) and handle the header-to-source refactor separately. Let us know if you'd like a hand, or whether someone else should pick up the fix. Thanks!
