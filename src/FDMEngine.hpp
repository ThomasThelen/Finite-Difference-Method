/*! \file FDMEngine.hpp
* \brief Contains the methods for the simulation.
*
* The two classes are Mesh and Engine
*/

#pragma once
#include <vector>
#include <tuple>
#include <iterator>
#include <iostream>
#include <fstream>


using std::vector;
using std::tuple;
using std::cout;
using std::endl;
using std::iterator;
using std::get;


/*! \class Mesh
* \A class to hold mesh information.
*  Holds physical information about the object being simulated.
*/

class Mesh
{
public:
	// !Length of the object.
	double spacial_length;

	// !The step size between nodes.
	double spacial_step_size;

	// !The thermal conductivity of the mesh.
	double thermal_conductivity;

	// !Number of nodes on the mesh -> POSSIBLY DELETE.
	int number_of_nodes;

	// !A pointer to the user defined boundary condition function.
	double(*DirchletBoundaryEquation)(double, int);
	double(*InitialDistribution)(double);
};

class Mesh1D : public Mesh
{
public:
	double spacial_width;
	vector<double> all_node_locations;
};

class Mesh2D : public Mesh
{
public:
	double spacial_width;

	vector<double> left_side_boundary_conditions;

	vector<double> right_side_boundary_conditions;

	vector<double> top_width_boundary_conditions;

	vector<double> bottom_width_boundary_conditions;

	vector<double> all_node_locations;

	double(*SideBoundary)(void);

	double(*WidthBoundary)(void);


};



//
class Engine
{
public:
	double number_of_states;

	double time_length;

	double time_step;

	double initial_temperature = 0;

	double last_temperature = 0;

	// CHANGE NAME
	vector<double> times;

	//Contains the position and Temperature data at a specific time.
	vector<tuple<double, double>> current_configuration; 

	// Contains all of the configurations over time.
	vector< vector<tuple<double, double>>> results; 
	
	// !Initiates the simulation

	// !Fills the first element of results with the initial state.
	void CreateInitialState(Mesh&);
	// !Create a csv file with the simulation results.
	// !This is the FTCS numerical approximation.
	// !Retrieve a temperature at a given state and node from the results vector.
	//virtual void CreateMesh(Mesh&);
	// !Retrieves a record from the results vector
	tuple<double, double> Engine::GetRecordb(int, int) const;
	// !Retrieves a temperature in the forward or backward position.
	void CreateTime();
};


class Engine1D : public Engine
{
public:
	/*! Starts the simulation. 
		@arg Mesh1D The mesh that will be used during the simulation

	*/
	void StartSimulation(Mesh1D&);


	/*! Fills in important properties for the mesh. 
		@arg Mesh1DAn empty mesh object whose are going to be set by CreateMesh

	*/
	void CreateMesh(Mesh1D&);


	/*! Creates the initial state of the experiment
		@arg Mesh1D

	*/
	void CreateInitialState(Mesh1D&);

	void CreateCSV(Mesh1D);

	void Solve(Mesh1D&);

	double RetrieveTemperature(int, int);

	double GetPreviousTemperature(int, double, char);
};


class Engine2D : public Engine
{
public:
	vector<vector<tuple<double, double>>> Instance;

	vector<vector<vector<tuple<double, double>>>> Results;

	void CreateMesh(Mesh2D&);

	void StartSimulation(Mesh2D &mesh);

	void CreateInitialState(Mesh2D&);

	void CreateCSV(Mesh2D);

	void Solve(Mesh2D&);

	bool CheckStability(Mesh2D);

	double RetrieveTemperature(int, int, int);

	double GetPreviousTemperature(int, int, double, char);
};
