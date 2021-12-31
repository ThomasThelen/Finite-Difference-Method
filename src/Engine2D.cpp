#pragma once
#include "Engine2D.hpp"

void Engine2D::CreateInitialState(Mesh2D &mesh)
{
	vector<vector<tuple<double, double>>> instance_container;
	for (int i = 0; i < (mesh.c_length / mesh.spatial_step_size); i++)
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
	mesh.number_of_nodes = (mesh.c_length / mesh.spatial_step_size);
	mesh.all_node_locations.push_back(0);
	for (int i = 0; i < mesh.number_of_nodes; i++)
	{
		mesh.all_node_locations.push_back(mesh.all_node_locations.back() + mesh.spatial_step_size);
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
	double check = 2 * (mesh.thermal_conductivity*time_step) / (mesh.spatial_step_size*mesh.spatial_step_size);
	if (check <= 0.5)
	{
		cout << endl << endl;
		cout << "Under the current configuration, unstale results may occur." << endl;
		cout << "To Ensure convergence, please satisfy the following equation." << endl;
		cout << "2*(mesh.thermal_conductivity*time_step)/(mesh.spatial_step_size*mesh.spatial_step_size) <=0.5" << endl;
		cout << endl;
		cout << "This can be achieved by modifying any of the following:" << endl;
		cout << "    -thermal_conductivity" << endl;
		cout << "    -time_step" << endl;
		cout << "    -spatial_step_size" << endl;
		cout << endl;
		cout << " It is currently at " << (mesh.thermal_conductivity*time_step) / (mesh.spatial_step_size*mesh.spatial_step_size) << endl;
		return false;
	}
	return true;
}
void Engine2D::Solve(Mesh2D &mesh)
{
	CheckStability(mesh);

	double alpha = (mesh.thermal_conductivity*time_step) / (mesh.spatial_step_size);
	double beta=alpha;
	vector<double> Temperature;
	// !Iterate over the states
	for (int i = 1; i <= number_of_states; i++)
	{
		// !Iterate through each row on the panel
		for (int j = 1; j < (mesh.x_length / mesh.spatial_step_size); j++)
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
