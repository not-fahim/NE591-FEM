#pragma once

#include <vector>
#include <functional>
#include <cstddef>
#include "mesh.hpp"
#include "dof_manager.hpp"

class ElementMatrix1D {
public:
    static std::vector<std::vector<double>> computeStiffness(
        const Element& element,
        const Mesh& mesh,
        double k,
        std::size_t num_gauss_points
    );

    static std::vector<double> computeForce(
        const Element& element,
        const Mesh& mesh,
        const std::function<double(double)>& f,
        std::size_t num_gauss_points,
        const DofManager& dof_manager,
        double k
    );
};

