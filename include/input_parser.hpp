#pragma once
#include "mesh.hpp"
#include <string>

class InputParser {
public:
    // Reads the unified input file and populates simulation parameters/mesh
    static bool parse(const std::string& filename, Mesh& mesh);
};
