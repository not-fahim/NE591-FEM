#include <iostream>
#include <fstream>
#include <cassert>
#include "mesh.hpp"
#include "input_parser.hpp"
#include "shape_function.hpp"

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

void testShapeFunctions() {
    // 1. Check values at left node (xi = -1) -> N1 = 1, N2 = 0
    auto N_left = LinearShape1D::evaluate(-1.0);
    assert(std::abs(N_left[0] - 1.0) < 1e-12);
    assert(std::abs(N_left[1] - 0.0) < 1e-12);

    // 2. Check values at right node (xi = 1) -> N1 = 0, N2 = 1
    auto N_right = LinearShape1D::evaluate(1.0);
    assert(std::abs(N_right[0] - 0.0) < 1e-12);
    assert(std::abs(N_right[1] - 1.0) < 1e-12);

    // 3. Check values at element midpoint (xi = 0) -> N1 = 0.5, N2 = 0.5
    auto N_mid = LinearShape1D::evaluate(0.0);
    assert(std::abs(N_mid[0] - 0.5) < 1e-12);
    assert(std::abs(N_mid[1] - 0.5) < 1e-12);

    // 4. Check Partition of Unity (N1 + N2 = 1) at arbitrary point xi = 0.35
    auto N_arb = LinearShape1D::evaluate(0.35);
    assert(std::abs((N_arb[0] + N_arb[1]) - 1.0) < 1e-12);

    // 5. Check derivatives
    auto dN = LinearShape1D::evaluateDerivatives(0.0);
    assert(std::abs(dN[0] - (-0.5)) < 1e-12);
    assert(std::abs(dN[1] - 0.5) < 1e-12);

    std::cout << "[PASS] Shape Function test\n";
}

int main() {
    std::cout << "--- Running Mesh & Input Tests ---\n";
    
    testNode();
    testElement();
    testInputParser();
    testShapeFunctions();

    std::cout << "All tests passed successfully!\n";
    return 0;
}
