# Finite-Difference-Method
A Finite Difference Method Engine in C++


This repository hosts a small library/engine for solving spacial differential equations using the finite difference method. 

## Example
An example that uses the library can be found in the `1dHeatRod` folder in `examples`. It solves the 1 dimensioanl heat equation in a rod. The file of interest with the implementation is `1dHeatRod.cpp`. 


`Mesh1D mesh;`

`mesh.spacial_length = 10;`

`mesh.spacial_step_size = 1;`

`mesh.thermal_conductivity = 0.01;`

`mesh.DirchletBoundaryEquation = BC;`

`Engine1D engine;`

`engine.StartSimulation(mesh);`
