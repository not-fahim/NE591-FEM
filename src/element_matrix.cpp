#include "element_matrix.hpp"
#include "element_mapping.hpp"
#include "shape_function.hpp"
#include "quadrature.hpp"

std::vector<std::vector<double>> ElementMatrix1D::computeStiffness(	
		const std::vector<double>& node_x,
		double k,
		std::size_t num_gauss_points
		) {

	std::size_t num_nodes = node_x.size();
	std::vector<std::vector<double>> Ke(num_nodes, std::vector<double> (num_nodes, 0.0));
	
	auto gauss_points = Quadrature1D::getPoints(num_gauss_points);
	for( const auto& gp : gauss_points) {
		double J = ElementMapping1D::computeJacobian(node_x, gp.xi);
		auto dN_dx = ElementMapping1D::computePhysicalDerivatives(node_x, gp.xi);

		for (std::size_t i = 0; i < num_nodes; i++) {
			for (std::size_t j = 0; j<num_nodes; j++) {
				Ke[i][j] +=gp.weight*k*dN_dx[i]* dN_dx[j] * J;
			}
		}
	}
	return Ke;
}


std::vector<double> ElementMatrix1D::computeForce(
        const std::vector<double>& node_x,
        const std::function<double(double)>& f,
        std::size_t num_gauss_points
){

	std::size_t num_nodes = node_x.size();
	std::vector<double> fe(num_nodes, 0.0);

	auto gauss_points = Quadrature1D::getPoints(num_gauss_points);

	for(const auto& gp: gauss_points) {
		double J = ElementMapping1D::computeJacobian(node_x, gp.xi);
		double x_phys = ElementMapping1D::referenceToPhysical(node_x, gp.xi);
		auto N = LinearShape1D::evaluate(gp.xi);

		double f_val = f(x_phys);
		for(std::size_t i=0; i< num_nodes; i++) {
			fe[i] += gp.weight * f_val * N[i] * J;
		}
	}
	return fe;
}
