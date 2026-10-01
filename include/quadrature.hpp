#pragma once
#include <vector>
#include <cstddef>

struct GaussPoint1D {
    double xi;
    double weight;
};

class Quadrature1D {
public:
    // Returns 1-point or 2-point Gauss quadrature rules
    static std::vector<GaussPoint1D> getPoints(std::size_t num_points);
};
