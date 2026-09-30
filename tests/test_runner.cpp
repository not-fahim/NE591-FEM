#include <iostream>
#include <fstream>
#include <cassert>
#include "mesh.hpp"
#include "input_parser.hpp"

void testNode() {
    Node n(0, {1.5});
    assert(n.id == 0);
    assert(n.getDimension() == 1);
    assert(n.coords[0] == 1.5);
    std::cout << "[PASS] Node test\n";
}

void testElement() {
    Element e(0, {0, 1});
    assert(e.id == 0);
    assert(e.numNodes() == 2);
    assert(e.node_ids[0] == 0);
    assert(e.node_ids[1] == 1);
    std::cout << "[PASS] Element test\n";
}

void testInputParser() {
    const std::string test_filename = "temp_input.dat";
    std::ofstream out(test_filename);
    out << "# Test Master Input File\n"
        << "NODES 3\n"
        << "0 0.0\n"
        << "1 0.5\n"
        << "2 1.0\n"
        << "ELEMENTS 2\n"
        << "0 0 1\n"
        << "1 1 2\n";
    out.close();

    Mesh domain;
    bool success = InputParser::parse(test_filename, domain);

    assert(success == true);
    assert(domain.getNodes().size() == 3);
    assert(domain.getElements().size() == 2);
    assert(domain.getNodes()[1].coords[0] == 0.5);
    assert(domain.getElements()[1].node_ids[0] == 1);
    assert(domain.getElements()[1].node_ids[1] == 2);

    std::remove(test_filename.c_str());

    std::cout << "[PASS] InputParser test\n";
}

int main() {
    std::cout << "--- Running Mesh & Input Tests ---\n";
    
    testNode();
    testElement();
    testInputParser();

    std::cout << "All tests passed successfully!\n";
    return 0;
}
