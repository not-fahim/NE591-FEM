#pragma once
#include <vector>

class ElementMapping1D{
public:
	static double referenceToPhysical(const std::vector<double>& node_x, double xi);

	static double computeJacobian(const std::vector<double>& node_x, double xi);

	static std::vector<double> computePhysicalDerivatives(const std::vector<double>& node_x, double xi);
};
	
