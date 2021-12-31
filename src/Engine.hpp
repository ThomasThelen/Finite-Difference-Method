#pragma once
#include "Mesh.hpp"
#include <vector>
#include <tuple>
#include <iterator>
#include <iostream>
#include <fstream>

using std::vector;
using std::tuple;
using std::cout;
using std::endl;
using std::get;

/*! \class Engine
* \A base class for solution engines
*  Contains basic information that's used across n-dimensional solvers.
*/

class Engine
{
public:
	size_t number_of_states;

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
	tuple<double, double> GetRecordb(int, int) const;
	// !Retrieves a temperature in the forward or backward position.
	void CreateTime();
};