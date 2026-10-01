#include "quadrature.hpp"
#include <cmath>
#include <stdexcept>

std::vector<GaussPoint1D> Quadrature1D::getPoints(std::size_t num_points) {
    if (num_points == 1) {
        return {{0.0, 2.0}};
    } else if (num_points == 2) {
        double xi_val = 1.0 / std::sqrt(3.0);
        return {{-xi_val, 1.0}, {xi_val, 1.0}};
    }
    throw std::invalid_argument("Unsupported quadrature order");
}
