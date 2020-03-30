Jeff+Josh discussion points:

DO NOT SAVE THIS FILE IN THE FINAL PR

- Name of script?
  - ompirun:
    - PRO: clearly OMPI-specific
    - PRO: clearly separate this from "real" mpirun
  - mpirun:
    - PRO: clearly separate this from prun
    - PRO: be able to add other OMPI-specific behavior in front of prun
    - CON: putting a script in front of the main executable (tool complications?)
  - ...?

- need to test "make dist"
  --> Works fine.

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
