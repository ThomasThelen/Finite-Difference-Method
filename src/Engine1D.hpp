#pragma once
#include "Engine.hpp"
#include "Mesh1D.hpp"

class Engine1D : public Engine
{
public:
	/*! Starts the simulation.
		@arg Mesh1D The one-dimensional mesh that will be used during the simulation
	*/
	void StartSimulation(int time_length, int time_step, Mesh1D& mesh);


	/*! Fills in important properties for the mesh.
		@arg Mesh1D An empty mesh object whose are going to be set by CreateMesh

	*/
	void CreateMesh(Mesh1D&);


	/*! Creates the initial state of the experiment
		@arg Mesh1D

	*/
	void CreateInitialState(Mesh1D&);

	void CreateCSV(Mesh1D);

	void Solve(Mesh1D&);

	double RetrieveTemperature(int, int);

	double GetPreviousTemperature(int, int, char);
};
