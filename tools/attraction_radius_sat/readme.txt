README.tools for MagChain 24Nov2011 package.

****** attradius.cpp ******
  Description: Simple routine to create the required file containing the attraction radius for each object in the simulation according to the magnetic coupling parameter.
  
  Compilation: type "make" into the source folder.
  Requirements: C++ compiler.


Files generated:

1) A two-column file with a 1-line header (attradius.dat).
first column: number of beads in the chain-like aggregate.
second column: value of the attraction radius in particle diameters.

2) log file with the input parameters used (log.txt).
