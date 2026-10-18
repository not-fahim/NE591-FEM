#pragma once
#include <vector>
#include <cstddef>

enum class BCType {Dirichlet, Neumann};

struct BoundaryCondition {
	BCType type;
	std::size_t node_id;
	double value;
	double nx; //outward normal vector, +1 on right, -1 on left
	std::size_t id;	
};



