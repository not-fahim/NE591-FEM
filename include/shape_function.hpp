#pragma once
#include <vector>

class LinearShape1D{

public:
	static std::vector<double> evaluate(double xi);
	static std::vector<double> evaluateDerivatives(double xi);
};

