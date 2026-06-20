# Open issues older than 6 months — triage overview

Triage of open `open-mpi/ompi` issues opened more than 6 months ago (created before 2025-12-20), as of 2026-06-20. Classification and status are a **reasoned determination** from each issue's text, discussion, labels, and linked/merged PRs, plus spot-checks of current `main` — not a per-branch build-and-reproduce test. Treat "Close" as *recommended for maintainer confirmation*, not automatic.

Issues are grouped by **why they are still open**. See [issues-overview.md](issues-overview.md) for totals and cross-tabs, and the companion files: [bugs](issues-bugs.md) · [feature requests](issues-feature-requests.md) · [questions](issues-questions.md) · [other](issues-other.md).


## Totals


**685 open issues** opened before 2025-12-20 were triaged.


### Recommended action

| Recommendation | Count | % |
|----------------|------:|--:|
| 🗑️ Close | 338 | 49% |
| ❓ Needs info | 126 | 18% |
| ✅ Keep open | 221 | 32% |

### By issue type

| Type | Count | File |
|------|------:|------|
| Bug | 399 | [issues-bugs.md](issues-bugs.md) |
| Feature request | 71 | [issues-feature-requests.md](issues-feature-requests.md) |
| Question | 148 | [issues-questions.md](issues-questions.md) |
| Documentation | 32 | [issues-other.md](issues-other.md) |
| Other | 35 | [issues-other.md](issues-other.md) |

### By reason still open

| Reason still open | Count |
|-------------------|------:|
| Unresolved, still valid | 242 |
| Fixed, not closed | 78 |
| Feature now exists | 12 |
| Answered | 97 |
| Awaiting reporter | 22 |
| Cannot reproduce / needs info | 85 |
| Obsolete / moot | 98 |
| Duplicate / tracked elsewhere | 10 |
| Wontfix / out of scope | 41 |

## Type × recommendation

| Type | 🗑️ Close | ❓ Needs info | ✅ Keep open | Total |
|------|------:|------:|------:|------:|
| Bug | 160 | 90 | 149 | 399 |
| Feature request | 31 | 5 | 35 | 71 |
| Question | 120 | 28 | 0 | 148 |
| Documentation | 17 | 1 | 14 | 32 |
| Other | 10 | 2 | 23 | 35 |
| **Total** | **338** | **126** | **221** | **685** |

## Reason × recommendation

| Reason still open | 🗑️ Close | ❓ Needs info | ✅ Keep open |
|-------------------|------:|------:|------:|
| Unresolved, still valid | 5 | 26 | 211 |
| Fixed, not closed | 74 | 1 | 3 |
| Feature now exists | 11 | 0 | 1 |
| Answered | 96 | 0 | 1 |
| Awaiting reporter | 1 | 21 | 0 |
| Cannot reproduce / needs info | 11 | 74 | 0 |
| Obsolete / moot | 97 | 1 | 0 |
| Duplicate / tracked elsewhere | 6 | 1 | 3 |
| Wontfix / out of scope | 37 | 2 | 2 |

## Notes

- "Older than 6 months" = issue **created** before 2025-12-20 (open issue age), distinct from the PR reports which used *last activity*.
- A "Close" recommendation means the evidence says it's resolved/obsolete/answered/duplicate; a maintainer should glance before closing.
- "Needs info" issues are stalled awaiting the reporter — good candidates for a ping-then-close-if-silent policy.

