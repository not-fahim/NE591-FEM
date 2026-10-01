#include "element_mapping.hpp"
#include "shape_function.hpp"

double ElementMapping1D::referenceToPhysical(const std::vector<double>& node_x, double xi) {
	auto N = LinearShape1D::evaluate(xi);
	return (N[0] * node_x[0] + N[1] * node_x[1]);
};


double ElementMapping1D::computeJacobian(const std::vector<double>& node_x, double xi) {
	auto dN_dxi = LinearShape1D::evaluateDerivatives(xi);
	return dN_dxi[0] * node_x[0] + dN_dxi[1] * node_x[1];
}

std::vector<double> ElementMapping1D::computePhysicalDerivatives(const std::vector<double>& node_x, double xi){

	double J = computeJacobian(node_x, xi);
	auto dN_dxi = LinearShape1D::evaluateDerivatives(xi);
	std::vector<double> dN_dx(dN_dxi.size());
	for (std::size_t i = 0; i<dN_dxi.size(); ++i) {
		dN_dx[i] = dN_dxi[i] / J;
	}
	return dN_dx;
}
