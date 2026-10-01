#include "shape_function.hpp"

std::vector<double> LinearShape1D::evaluate(double xi){
	return { 0.5 * (1-xi),
		 0.5 * (1+xi) };
};

std::vector<double> LinearShape1D::evaluateDerivatives(double xi){
	return {-0.5, 0.5};
};
