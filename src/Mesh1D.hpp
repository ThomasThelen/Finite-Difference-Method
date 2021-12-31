#pragma once
#include "Mesh.hpp"
#include <vector>
struct Mesh1D : public Mesh
{
public:
	double spatial_width;
	std::vector<double> all_node_locations;
};
