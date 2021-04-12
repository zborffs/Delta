Delta Robot Tools
=================
- ```cad/``` directory contains OpenSCAD code for delta robot design
  - each file contains a single component that's either 3-D printable, purchaseable, or the assemblage of smaller components
  - each component can be created in isolation, and its dimensions and placement can be manipulated by the assembly module
- ```jl/``` directory contains Julia code
  - DeltaRobot library contains helper functions for simulating systems, analyzing those systems, and synthesizing controllers
  - each script outside the DeltaRobot library makes use of the library to do the simulating, analysis, and synthesis on different plants
- ```symbolic/``` directory contains symbolic Matlab code
  - each script derives the symbolic expressions for a different plant

Julia
-----
- Check to make sure both Lagrangian and Hamiltonian are constant throughout the simulation
  - each returns to zero at each simulation point, but non-zero through interpolation points.

Matlab
------
### Spherical Pendulum Singularity
- the spherical pendulum equation derived from wikipedia contains a singularity with phi2 being equal to zero. 
- this can be offset if we do a change of variables with phi2 = phi2' - pi. This way the singularity is above the base
of the robot, it will never be encountered during simulation.
