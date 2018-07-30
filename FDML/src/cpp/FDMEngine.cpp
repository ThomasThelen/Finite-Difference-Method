#include "stdafx.h"
#include "FDMEngine.hpp"



void Engine::CreateTime()
{
	cout << "Creating time domain..." << endl;
	times.push_back(0);
	while (times.back()< time_length)
	{
		times.push_back(times.back() + time_step);
	}
	cout << "Time Created" << endl;
}
double Engine1D::GetPreviousTemperature(int current_state, double node_location, char direction)
{
	if (direction == 'f')
	{
		return RetrieveTemperature(current_state - 1, node_location + 1);
	}
	if (direction == 'b')
	{
		return RetrieveTemperature(current_state - 1, node_location - 1);
	}
	if (direction == 'n')
	{
		return RetrieveTemperature(current_state - 1, node_location);
	}
	else
	{
		cout << "Error Obtaining Temperature" << endl;
		return 0;
	}
}

tuple<double, double> Engine::GetRecordb(int state_position, int record_position) const
{
	vector<tuple<double, double>> state;
	tuple<double, double > record;
	state = results.at(state_position);
	record = state.at(record_position);
	return record;
}

void Engine1D::StartSimulation(Mesh1D &mesh)
{
	number_of_states = time_length / time_step;
	CreateMesh(mesh);
	CreateTime();
	CreateInitialState(mesh);
	Solve(mesh);
}

void Engine1D::CreateInitialState(Mesh1D &mesh)
{
	//Fill 0<x<L
	number_of_states = time_length / time_step;
	// !Place first initial condition
	current_configuration.push_back(std::make_tuple(mesh.all_node_locations.front(), mesh.DirchletBoundaryEquation(0,1)));
	// !Iterate over the nodes which are not boundary conditions. (starts at node 1 and stops at # of nodes-1
	for (float j = 1; j <= mesh.number_of_nodes- 1; ++j)
	{
		current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(j), mesh.InitialDistribution(0)));
	}
	current_configuration.push_back(std::make_tuple(mesh.all_node_locations.back(), mesh.DirchletBoundaryEquation(0, 2))); // Once filled, set the last B.C.
	results.push_back(current_configuration); // Add to the vector of states
	current_configuration.clear(); // Clear the current state
}
void Engine1D::CreateMesh(Mesh1D &mesh)
{
	mesh.number_of_nodes = (mesh.spacial_length / mesh.spacial_step_size);
	cout << "Computing Nodal Localtions" << endl;
	mesh.all_node_locations.push_back(0);

	//Store the location of each node
	for (int i = 0; i < mesh.number_of_nodes; i++)
	{
		mesh.all_node_locations.push_back(mesh.all_node_locations.back() + mesh.spacial_step_size);
	}
	cout << "Mesh Created" << endl;
}
void Engine1D::Solve(Mesh1D &mesh)
{
	if ((mesh.thermal_conductivity*time_step) / (mesh.spacial_step_size*mesh.spacial_step_size)>0.5)
	{
		cout << endl << endl;
		cout << "Under the current configuration, unstale results may occur." << endl;
		cout << "To Ensure convergence, please satisfy the following equation." << endl;
		cout << "0.5<(thermal_conductivity*time_step)/(spacial_step_size^2)" << endl;
		cout << endl;
		cout << "This can be achieved by modifying any of the following:" << endl;
		cout << "    -thermal_conductivity" << endl;
		cout << "    -time_step" << endl;
		cout << "    -spacial_step_size" << endl;
		cout << endl;
		cout << " It is currently at " << (mesh.thermal_conductivity*time_step) / (mesh.spacial_step_size*mesh.spacial_step_size) << endl;
	}

	vector<double> Temperature;
	double constants = (mesh.thermal_conductivity*time_step) / (mesh.spacial_step_size*mesh.spacial_step_size),
		current_node = 1; // Always start at 1; 0 is set by the boundary condition

	int state_location = 1; // Always start at state 1; 0 is set by the intitial condition

	for (double i = 0; i <= number_of_states; i++) // Iterate over time. Start at 0 because the time step may be < or > 1
	{
		Temperature.push_back(mesh.DirchletBoundaryEquation(i, 1)); //Make this call the BC function
		current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(0), Temperature.front())); // Set the boundary condition
		for (int j = 1; j<mesh.number_of_nodes; ++j) // Start at the 2nd node (first is set above by the bountary condition)
		{
			cout << GetPreviousTemperature(state_location, current_node, 'f') << " + " << (1 - 2 * constants) * GetPreviousTemperature(state_location, current_node, 'n') << " +" << GetPreviousTemperature(state_location, current_node, 'b')*constants << endl;
			double new_temp = constants*GetPreviousTemperature(state_location, current_node, 'f') + (1 - 2 * constants) * GetPreviousTemperature(state_location, current_node, 'n') + GetPreviousTemperature(state_location, current_node, 'b')*constants; // Get the temperature at the current node #; passes the current state.
			Temperature.push_back(new_temp);
			current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(current_node), Temperature.at(current_node))); // Use current_node to access the approriate physical node location. 
			++current_node;
		}
		++state_location;
		Temperature.push_back(mesh.DirchletBoundaryEquation(i, 2));
		current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(current_node), 0));
		//++this->time;
		cout << endl << endl << endl << endl << endl;
		current_node = 1;
		results.push_back(current_configuration); // Push the current state configuration vector into the results vector.
		current_configuration.clear(); // Prepare for the next configuration by clearing the current one
		Temperature.clear(); // Clear the temperature vector
	}
	CreateCSV(mesh);
}
void Engine1D::CreateCSV(Mesh1D mesh)
{
	std::ofstream dataFile;
	dataFile.open("Results.csv");
	tuple<double, double> record;
	//Iterate records with GetRecord

	for (int i = 0; i < number_of_states; i++)
	{
		for (int j = 0; j < mesh.all_node_locations.size(); j++)
		{
			record = GetRecordb(i, j);
			dataFile << get<0>(record) << "," << get<1>(record) << endl;
		}

	}
	dataFile.close();
}
double Engine1D::RetrieveTemperature(int state_position, int record_position)
{
	vector<tuple<double, double>> state;
	tuple<double, double > record;
	state = results.at(state_position);
	record = state.at(record_position);
	return get<1>(record);
}

void Engine2D::StartSimulation(Mesh2D &mesh)
{
	number_of_states = time_length / time_step;
	CreateMesh(mesh);
	CreateTime();
	CreateInitialState(mesh);
	Solve(mesh);
}
void Engine2D::CreateInitialState(Mesh2D &mesh)
{
	vector<vector<tuple<double, double>>> instance_container;
	for (int i = 0; i < (mesh.spacial_length / mesh.spacial_step_size); i++)
	{
		current_configuration.push_back(std::make_tuple(mesh.all_node_locations.front(), mesh.left_side_boundary_conditions.at(0))); //Put the first I.C in.
		for (float j = 1; j <= mesh.number_of_nodes - 1; ++j) // Iterate over the nodes which are not boundary conditions. (starts at node 1 and stops at # of nodes-1
		{
			current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(j), mesh.InitialDistribution(0)));
		}
		current_configuration.push_back(std::make_tuple(mesh.all_node_locations.back(), mesh.right_side_boundary_conditions.at(0))); // Once filled, set the last B.C.
		Instance.push_back(current_configuration);
		current_configuration.clear(); // Clear the current state
	}

	Results.push_back(Instance); // Add to the vector of states
	Instance.clear();
}
void Engine2D::CreateMesh(Mesh2D& mesh)
{
	// Fill side1 length BC -> Left side
	// Fill side2 length BC -> Right side
	// Fill width1 BC ->Top
	// Fill width2 BC ->bottom
	mesh.number_of_nodes = (mesh.spacial_length / mesh.spacial_step_size);
	mesh.all_node_locations.push_back(0);
	for (int i = 0; i < mesh.number_of_nodes; i++)
	{
		mesh.all_node_locations.push_back(mesh.all_node_locations.back() + mesh.spacial_step_size);
	}
	// !Create a boundary condition for each state
	for (int state = 0; state <= number_of_states; state++)
	{
		mesh.left_side_boundary_conditions.push_back(mesh.SideBoundary());
		mesh.right_side_boundary_conditions.push_back(mesh.SideBoundary());
		// !First and last states
		if (state== 0 || state==number_of_states)
		{
			mesh.top_width_boundary_conditions.push_back(mesh.WidthBoundary());
			mesh.bottom_width_boundary_conditions.push_back(mesh.WidthBoundary());
		}
	}
}
bool Engine2D::CheckStability(Mesh2D mesh)
{
	double check = 2 * (mesh.thermal_conductivity*time_step) / (mesh.spacial_step_size*mesh.spacial_step_size);
	if (check <= 0.5)
	{
		cout << endl << endl;
		cout << "Under the current configuration, unstale results may occur." << endl;
		cout << "To Ensure convergence, please satisfy the following equation." << endl;
		cout << "2*(mesh.thermal_conductivity*time_step)/(mesh.spacial_step_size*mesh.spacial_step_size) <=0.5" << endl;
		cout << endl;
		cout << "This can be achieved by modifying any of the following:" << endl;
		cout << "    -thermal_conductivity" << endl;
		cout << "    -time_step" << endl;
		cout << "    -spacial_step_size" << endl;
		cout << endl;
		cout << " It is currently at " << (mesh.thermal_conductivity*time_step) / (mesh.spacial_step_size*mesh.spacial_step_size) << endl;
		return false;
	}
	return true;
}
void Engine2D::Solve(Mesh2D &mesh)
{
	CheckStability(mesh);

	double alpha = (mesh.thermal_conductivity*time_step) / (mesh.spacial_step_size);
	double beta=alpha;
	vector<double> Temperature;
	// !Iterate over the states
	for (int i = 1; i <= number_of_states; i++)
	{
		// !Iterate through each row on the panel
		for (int j = 1; j < (mesh.spacial_length / mesh.spacial_step_size); j++)
		{
			// SET THE FIRST NODE FROM BC
			Temperature.push_back(5);
			current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(0), Temperature.at(0)));
			// !Iterate over the node
			for (int k = 1; k < mesh.number_of_nodes; k++)
			{
				Temperature.push_back(alpha*(GetPreviousTemperature(i, j, k, 'f') + GetPreviousTemperature(i, j, k, 'b')) +(beta*(GetPreviousTemperature(i, j,k, 'f') + GetPreviousTemperature(i, j, k, 'b'))) + ((1 - 2 * alpha - 2 * beta)*GetPreviousTemperature(i, j, k, 'n')));
				current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(k), Temperature.at(k)));
			}
			Temperature.push_back(5);
			current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(mesh.all_node_locations.size()-1), Temperature.at(mesh.number_of_nodes)));
			Instance.push_back(current_configuration);
			current_configuration.clear();
		}
		Results.push_back(Instance);
		Instance.clear();
	}
	CreateCSV(mesh);
}

double Engine2D::RetrieveTemperature(int current_state, int y_position, int node_position)
{
	vector<vector<tuple<double, double>>> instance_container;
	vector<tuple<double, double>> temperature_dist;
	tuple<double, double > record;
	instance_container = Results.at(current_state);
	temperature_dist =instance_container.at(y_position);
	record = temperature_dist.at(node_position);
	return get<1>(record);
}

double Engine2D::GetPreviousTemperature(int current_state, int y_position, double node_location, char direction)
{

	if (direction == 'f')
	{
		return RetrieveTemperature(current_state-1, y_position - 1, node_location + 1);
	}
	if (direction == 'b')
	{
		return RetrieveTemperature(current_state-1, y_position - 1, node_location - 1);
	}
	if (direction == 'n')
	{
		return RetrieveTemperature(current_state-1, y_position - 1, node_location);
	}
	else
	{
		cout << "Error Obtaining Temperature" << endl;
		return 0;
	}
}

void Engine2D::CreateCSV(Mesh2D mesh)
{
	std::ofstream dataFile;
	dataFile.open("Results.csv");
	tuple<double, double> record;
	//Iterate records with GetRecord

	for (int i = 0; i < number_of_states; i++)
	{
		for (int j = 0; j < mesh.all_node_locations.size(); j++)
		{
			record = GetRecordb(i, j);
			dataFile << get<0>(record) << "," << get<1>(record) << endl; // CORRECT THIS
		}
	}
	dataFile.close();
}
