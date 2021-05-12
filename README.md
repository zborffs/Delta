Delta
=====
- this repo contains code for a Delta Robot
- application software running on target is written in C++
- modeling and control prototyping is done in both Matlab and Julia
  - Matlab for generating symbolic expressions
  - Julia for performing numerical simulations
- design is done in OpenSCAD
- find comprehensive treatment of julia/matlab/openscad related things in readme under ```tools/``` directory

Dependencies (C/C++)
--------------------
- yaml-cpp
- spdlog
- gtest
- OpenCV
- Matplot++
- Eigen
- Cloak-lib
- mlpack

Target
------
Jetson Xavier NX

Host
----
MacBook Pro
