#include "input_parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool InputParser::parse(const std::string& filename, Mesh& mesh) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open input file " << filename << "\n";
        return false;
    }

    std::string line;
    std::string keyword;

    while (std::getline(file, line)) {
        // Ignore empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);
        ss >> keyword;

        if (keyword == "NODES") {
            std::size_t count;
            ss >> count;

            for (std::size_t i = 0; i < count; ++i) {
                std::size_t id;
                double x;
                file >> id >> x;
                mesh.addNode(id, {x});
            }
        } 
        else if (keyword == "ELEMENTS") {
            std::size_t count;
            ss >> count;

            for (std::size_t i = 0; i < count; ++i) {
                std::size_t id, n1, n2;
                file >> id >> n1 >> n2;
                mesh.addElement(id, {n1, n2});
            }
        }
        // Future sections like MATERIALS or BCs will be added here
    }

    file.close();
    return true;
}
