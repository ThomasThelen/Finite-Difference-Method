![alt text](https://github.com/ThomasThelen/Finite-Difference-Method/raw/master/banner.png)

[![Project Status: Inactive – The project has reached a stable, usable state but is no longer being actively developed; support/maintenance will be provided as time allows.](https://www.repostatus.org/badges/latest/inactive.svg)](https://www.repostatus.org/#inactive)  [![license](https://img.shields.io/github/license/mashape/apistatus.svg)]()
[![CodeFactor](https://www.codefactor.io/repository/github/thomasthelen/finite-difference-method/badge)](https://www.codefactor.io/repository/github/thomasthelen/finite-difference-method)


This repository hosts a small library/engine for solving spacial differential equations using the finite difference method. This was created as a fun way to explore numerical methods as a stepping stone for the Finite Elements Method. Feel free to use this code as an example of how to numerically solve partial differential equations.

## How it Works

Spacial variables such as the object length, conductivity, and step size across the mesh are kept in the `Mesh` class. An example of filling out relevant `Mesh` information is showen below.

`Mesh1D mesh;`

`mesh.spacial_length = 10;`

`mesh.spacial_step_size = 1;`

`mesh.thermal_conductivity = 0.01;`


The boundary condition is represented by a function that takes two parameters: x and y. The boundary condition can be changed by changing the equation inside. For example, a periodic boundary condition can be represented by 

```
double BC(double x, int y)
{
	return sin(x)*cos(y);
}
```

The boundary condition also belongs to the mesh.

`mesh.DirchletBoundaryEquation = BC;`

`Engine1D engine;`

`engine.StartSimulation(mesh);`
