#pragma once
#include <functional>

struct Mesh
{
	//! Length of the object.
	double x_length;

	//! The step size between nodes.
	double spatial_step_size;

	//! The thermal conductivity of the mesh.
	double thermal_conductivity;

	//! Number of nodes on the mesh -> POSSIBLY DELETE.
	int number_of_nodes;

	//! A pointer to the user defined boundary condition function.
	std::function<double(double, int)> DirchletBoundaryEquation;
	std::function<double(double)> InitialDistribution;
};