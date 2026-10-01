#pragma once
#include <vector>
#include <cstddef>
#include <functional>

class ElementMatrix1D {
public:
    // Computes the local stiffness matrix K_e (2x2 for 1D linear element)
    static std::vector<std::vector<double>> computeStiffness(
        const std::vector<double>& node_x,
        double k,
        std::size_t num_gauss_points = 2
    );

    static std::vector<double> computeForce(
        const std::vector<double>& node_x,
        const std::function<double(double)>& f,
        std::size_t num_gauss_points = 2
    );
};
