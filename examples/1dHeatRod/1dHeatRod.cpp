// 1dHeatRod.cpp : Simulation of heat flowing in a one dimensional rod with a time & position dependant boundary condition.

#include "../../src/Engine1D.hpp"
#include "../../src/Mesh1D.hpp"

// A boundary condition that varies in space and time
double BC(double x, int time)
{
	return .2+x * (time +.1);
}

// A function that defines the initial temperature at some x position. Used to
// populate the initial conditions on the rod (everything inbetween the boundaries)
double TempDistribution(double position) {
	return position + 1;
}

int main()
{
	// Create a 1 dimensional mesh with
	// Length: 10 meters
	// Step size: 1 meter
	// Thermal Conductivity: 0.01
	Mesh1D mesh;
	mesh.x_length = 10;
	mesh.spatial_step_size = 1;
	mesh.thermal_conductivity = 0.01;
	mesh.DirchletBoundaryEquation = BC;
	mesh.InitialDistribution = TempDistribution;
	// Create the engine
	Engine1D engine;
	
	// Start the simulation using the mesh above
	engine.StartSimulation(10, 1, mesh);
	return 0;
}