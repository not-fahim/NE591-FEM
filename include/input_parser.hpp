#pragma once
#include "mesh.hpp"
#include "boundary_conditions.hpp"
#include <string>

class InputParser {
public:
    // Reads the unified input file and populates simulation parameters/mesh
    static bool parse_mesh(const std::string& filename, Mesh& mesh);
    static bool parse_bc(const std::string& filename, std::vector<BoundaryCondition>& bc); 
};
