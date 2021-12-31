#include "Engine1D.hpp"

double Engine1D::GetPreviousTemperature(int current_state, int node_location, char direction)
{
	// Check if we're checking the value at the next node location
	if (direction == 'f')
	{
		return RetrieveTemperature(current_state - 1, node_location + 1);
	}
	// Check if we're going back one node length
	if (direction == 'b')
	{
		return RetrieveTemperature(current_state - 1, node_location - 1);
	}
	// Check if we're at the current node
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

void Engine1D::StartSimulation(int time_length, int time_step, Mesh1D& mesh)
{
	this->time_length = time_length;
	this->time_step = time_step;
	number_of_states = static_cast<size_t>(time_length / time_step);
	CreateMesh(mesh);
	CreateTime();
	CreateInitialState(mesh);
	Solve(mesh);
}

void Engine1D::CreateInitialState(Mesh1D& mesh)
{
	// !Place first initial condition
	current_configuration.push_back(std::make_tuple(mesh.all_node_locations.front(), mesh.DirchletBoundaryEquation(0, 1)));
	// !Iterate over the nodes which are not boundary conditions. (starts at node 1 and stops at # of nodes-1
	for (auto j = 1; j <= mesh.number_of_nodes - 1; ++j)
	{
		current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(j), mesh.InitialDistribution(0)));
	}
	current_configuration.push_back(std::make_tuple(mesh.all_node_locations.back(), mesh.DirchletBoundaryEquation(0, 2))); // Once filled, set the last B.C.
	results.push_back(current_configuration); // Add to the vector of states
	current_configuration.clear(); // Clear the current state
}
void Engine1D::CreateMesh(Mesh1D& mesh)
{
	mesh.number_of_nodes = static_cast<int>((mesh.x_length / mesh.spatial_step_size));
	cout << "Computing the localtions for all of the nodes (points where calculations are made)" << endl;
	mesh.all_node_locations.push_back(0);

	//Store the location of each node
	for (int i = 0; i < mesh.number_of_nodes; i++)
	{
		mesh.all_node_locations.push_back(mesh.all_node_locations.back() + mesh.spatial_step_size);
	}
	cout << "Mesh Created" << endl;
}
void Engine1D::Solve(Mesh1D& mesh)
{
	if ((mesh.thermal_conductivity * time_step) / (mesh.spatial_step_size * mesh.spatial_step_size) > 0.5)
	{
		cout << endl << endl;
		cout << "Under the current configuration, unstale results may occur." << endl;
		cout << "To Ensure convergence, please satisfy the following equation." << endl;
		cout << "0.5<(thermal_conductivity*time_step)/(spatial_step_size^2)" << endl;
		cout << endl;
		cout << "This can be achieved by modifying any of the following:" << endl;
		cout << "    -thermal_conductivity" << endl;
		cout << "    -time_step" << endl;
		cout << "    -spatial_step_size" << endl;
		cout << endl;
		cout << " It is currently at " << (mesh.thermal_conductivity * time_step) / (mesh.spatial_step_size * mesh.spatial_step_size) << endl;
	}

	vector<double> temperature;
	temperature.reserve(number_of_states);
	double constants = (mesh.thermal_conductivity * time_step) / (mesh.spatial_step_size * mesh.spatial_step_size);
	int current_node = 1; // Always start at 1; 0 is set by the boundary condition

	int state_location = 1; // Always start at state 1; 0 is set by the intitial condition

	for (double i = 0; i <= number_of_states; i++) // Iterate over time. Start at 0 because the time step may be < or > 1
	{
		cout << i << endl;
		temperature.push_back(mesh.DirchletBoundaryEquation(i, 1)); //Set the boundary condition
		current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(0), temperature.front())); // Set the boundary condition
		for (int j = 1; j < mesh.number_of_nodes; ++j) // Start at the 2nd node (first is set above by the bountary condition)
		{
			cout << GetPreviousTemperature(state_location, current_node, 'f') << " + " << (1 - 2 * constants) * GetPreviousTemperature(state_location, current_node, 'n') << " +" << GetPreviousTemperature(state_location, current_node, 'b') * constants << endl;
			double new_temp = constants * GetPreviousTemperature(state_location, current_node, 'f') + (1 - 2 * constants) * GetPreviousTemperature(state_location, current_node, 'n') + GetPreviousTemperature(state_location, current_node, 'b') * constants; // Get the temperature at the current node #; passes the current state.
			temperature.push_back(new_temp);
			current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(current_node), temperature.at(current_node))); // Use current_node to access the approriate physical node location. 
			++current_node;
		}
		++state_location;
		temperature.push_back(mesh.DirchletBoundaryEquation(i, 2));
		current_configuration.push_back(std::make_tuple(mesh.all_node_locations.at(current_node), 0));
		cout << endl << endl << endl << endl << endl;
		current_node = 1;
		results.push_back(current_configuration); // Push the current state configuration vector into the results vector.
		current_configuration.clear(); // Prepare for the next configuration by clearing the current one
		temperature.clear(); // Clear the temperature vector
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