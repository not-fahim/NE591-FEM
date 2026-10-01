#include "element_mapping.hpp"
#include "shape_function.hpp"

static double referenceToPhysical(const std::vector<double> node_x, double xi) {
	auto N = LinearShape1D(xi);
	return (N[0] * node_x[0] + N[1] * node_x[1]);
};

