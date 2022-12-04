#include "Mesh.hpp"

struct Mesh2D : public Mesh
{
	double spacial_width;

	vector<double> left_side_boundary_conditions;

	vector<double> right_side_boundary_conditions;

	vector<double> top_width_boundary_conditions;

	vector<double> bottom_width_boundary_conditions;

	vector<double> all_node_locations;

	std::function<double()> SideBoundary;

	std::function<double()> WidthBoundary;
};