#include "input_parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "input_parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

//helper function to map string values to BCType enum
static bool parse_bc_type(const std::string& str, BCType& type) {
    std::string upper = str;
    for (char& c : upper) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }

    if (upper == "DIRICHLET" || upper == "0") {
        type = BCType::Dirichlet;
        return true;
    } else if (upper == "NEUMANN" || upper == "1") {
        type = BCType::Neumann;
        return true;
    }
    return false;
}

bool InputParser::parse_mesh(const std::string& filename, Mesh& mesh) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open input file " << filename << "\n";
        return false;
    }

    std::string line;
    std::string keyword;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);
        if (!(ss >> keyword)) {
            continue;
        }

        if (keyword == "NODES") {
            std::size_t count = 0;
            if (!(ss >> count)) {
                std::cerr << "Error: Missing node count after NODES keyword\n";
                return false;
            }

            for (std::size_t i = 0; i < count; ++i) {
                std::size_t id;
                double x;
                if (!(file >> id >> x)) {
                    std::cerr << "Error: Failed to read node at index " << i << "\n";
                    return false;
                }
                mesh.addNode(id, {x});
            }
        } 
        else if (keyword == "ELEMENTS") {
            std::size_t count = 0;
            if (!(ss >> count)) {
                std::cerr << "Error: Missing element count after ELEMENTS keyword\n";
                return false;
            }

            for (std::size_t i = 0; i < count; ++i) {
                std::size_t id, n1, n2;
                if (!(file >> id >> n1 >> n2)) {
                    std::cerr << "Error: Failed to read element at index " << i << "\n";
                    return false;
                }
                mesh.addElement(id, {n1, n2});
            }
        }
    }

    file.close();
    return true;
}

bool InputParser::parse_bc(const std::string& filename, std::vector<BoundaryCondition>& BCs) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open input file " << filename << "\n";
        return false;
    }

    std::string line;
    std::string keyword;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);
        if (!(ss >> keyword)) {
            continue;
        }

        if (keyword == "BOUNDARY_CONDITIONS") {
            std::size_t count = 0;
            if (!(ss >> count)) {
                std::cerr << "Error: Missing count after BOUNDARY_CONDITIONS keyword\n";
                return false;
            }

            BCs.reserve(BCs.size() + count);

            for (std::size_t i = 0; i < count; ++i) {
                BoundaryCondition bc;
                std::string type_str;

                // Read values into variables and validate stream status
                if (!(file >> bc.id >> type_str >> bc.node_id >> bc.value >> bc.nx)) {
                    std::cerr << "Error: Corrupt or missing data for boundary condition entry " 
                              << (i + 1) << " of " << count << "\n";
                    return false;
                }

                // Validate and map the string representation to BCType enum
                if (!parse_bc_type(type_str, bc.type)) {
                    std::cerr << "Error: Invalid boundary condition type '" << type_str 
                              << "' at entry " << (i + 1) << "\n";
                    return false;
                }

                BCs.push_back(bc);
            }
        } 
    }

    file.close();
    return true;
}
