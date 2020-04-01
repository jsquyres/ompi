Jeff+Josh discussion points:

# DO NOT SAVE THIS FILE IN THE FINAL PR

- Name of script?
  - ompirun:
    - PRO: clearly OMPI-specific
    - PRO: clearly separate this from "real" mpirun
  - mpirun:
    - PRO: clearly separate this from prun
    - PRO: be able to add other OMPI-specific behavior in front of prun
    - CON: putting a script in front of the main executable (tool complications?)
  - ...?

- do we want to think about this as:
  - this script: upper-level OMPI-specific CLI-handling logic
  - prun: lower layer, very little/no OMPI-specific CLI-handling logic

- are there Python bindings directly into PRRTE (or PMIx) that this
  could be a 100%-Python application?
  - Is that useful?  Or are we just moving the complexity from C to Python?
  - CAUTION: calling C from Python probably requires some additional
    packages on the installation system.  This would suggest this
    should be an optinal step for a few OMPI release versions before
    making this mandatory / The Way.

- demo for community

- we need values for the .ini file

----------------------------------

# Current thoughts:

- Possibly a problem: requirement of Python3
    - Would be good to keep this optional for first release

- Let's keep the name `ompirun`
    - If desired, someday we can move the sym link for `mpirun` from `prun` to `ompirun`

- Future:
    - Better integration with other run time systems, like `jsrun`, `srun`, etc.
    - MAYBE: better integration with Python directly into prun.  But the value of that would need to be defined better.

# Proposal

- Overall themes
    - Put as much string / CLI parsing up in Python as possible
    - Put as much OMPI-specific stuff in OMPI-specific code (i.e., out of PRRTE)

- Jeff
    - `mpirun` will be a sym link:
        - if you have python3, it'll be a sym link to `ompirun`
            - and you get `--net` and `--tuned` functionality
        - if you do not have python3, it'll be a sym link to another script
            - if it finds `--net` or `--tuned`, emit a specific error and exit
            - if not, it just effectively does `exec $*` (watch out for spacing and quoting)

    - `ompirun` will handle `--tuned`
        - read in file and put them in `new_argv`
        - if same param found on CLI, CLI version overrides

    - make sym links for:
        - `ompirun-show`
        - `ompirun-showonly`
        - look at argv[0] to determine what to do

- Josh
    - Work on `make dist` to check all the frameworks / components in the `.ini` file

- Goal to show this to Ralph by end of this week to get his feedback
