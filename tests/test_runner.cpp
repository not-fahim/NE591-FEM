#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <vector>
#include <cstdio>
#include "mesh.hpp"
#include "input_parser.hpp"
#include "boundary_conditions.hpp"
#include "shape_function.hpp"
#include "element_mapping.hpp"
#include "element_matrix.hpp"
#include "dof_manager.hpp"

void testNode() {
    Node n(0, {1.5});
    assert(n.getId() == 0);
    assert(n.getDimension() == 1);
    assert(n.getCoords()[0] == 1.5);
    std::cout << "[PASS] Node test\n";
}

void testElement() {
    Element e(0, {0, 1});
    assert(e.getId() == 0);
    assert(e.numNodes() == 2);
    assert(e.getNodeIds()[0] == 0);
    assert(e.getNodeIds()[1] == 1);
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
    bool success = InputParser::parse_mesh(test_filename, domain);

    assert(success == true);
    assert(domain.getNodes().size() == 3);
    assert(domain.getElements().size() == 2);
    assert(domain.getNodes()[1].getCoords()[0] == 0.5);
    assert(domain.getElements()[1].getNodeIds()[0] == 1);
    assert(domain.getElements()[1].getNodeIds()[1] == 2);

    std::remove(test_filename.c_str());

    std::cout << "[PASS] InputParser test\n";
}

void testParseBC() {
    const std::string test_filename = "temp_bc.dat";
    
    // 1. Test successful parsing of Dirichlet and Neumann boundary conditions
    std::ofstream out(test_filename);
    out << "# Test Boundary Conditions File\n"
        << "BOUNDARY_CONDITIONS 2\n"
        << "1 Dirichlet 0 100.0 -1.0\n"
        << "2 Neumann 2 0.0 1.0\n";
    out.close();

    std::vector<BoundaryCondition> bcs;
    bool success = InputParser::parse_bc(test_filename, bcs);

    assert(success == true);
    assert(bcs.size() == 2);

    // Verify Dirichlet BC fields
    assert(bcs[0].id == 1);
    assert(bcs[0].type == BCType::Dirichlet);
    assert(bcs[0].node_id == 0);
    assert(std::abs(bcs[0].value - 100.0) < 1e-12);
    assert(std::abs(bcs[0].nx - (-1.0)) < 1e-12);

    // Verify Neumann BC fields
    assert(bcs[1].id == 2);
    assert(bcs[1].type == BCType::Neumann);
    assert(bcs[1].node_id == 2);
    assert(std::abs(bcs[1].value - 0.0) < 1e-12);
    assert(std::abs(bcs[1].nx - 1.0) < 1e-12);

    std::remove(test_filename.c_str());

    // 2. Test guard against invalid BC type string
    std::ofstream bad_type_out(test_filename);
    bad_type_out << "BOUNDARY_CONDITIONS 1\n"
                 << "1 UnknownType 0 50.0 1.0\n";
    bad_type_out.close();

    std::vector<BoundaryCondition> bad_bcs;
    bool bad_type_success = InputParser::parse_bc(test_filename, bad_bcs);
    assert(bad_type_success == false);

    std::remove(test_filename.c_str());

    // 3. Test guard against truncated / missing tokens
    std::ofstream truncated_out(test_filename);
    truncated_out << "BOUNDARY_CONDITIONS 2\n"
                  << "1 Dirichlet 0 100.0\n"; // Missing 'nx' and 2nd record
    truncated_out.close();

    std::vector<BoundaryCondition> trunc_bcs;
    bool trunc_success = InputParser::parse_bc(test_filename, trunc_bcs);
    assert(trunc_success == false);

    std::remove(test_filename.c_str());

    std::cout << "[PASS] BoundaryCondition Parser test\n";
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

void testElementMapping() {
    // 1D Element spanning physical domain x = [2.0, 6.0] (Length = 4.0)
    std::vector<double> node_x = {2.0, 6.0};

    // 1. Verify Jacobian J = Le / 2 = 4.0 / 2 = 2.0
    double J = ElementMapping1D::computeJacobian(node_x, 0.0);
    assert(std::abs(J - 2.0) < 1e-12);

    // 2. Verify Reference to Physical coordinate mapping
    // xi = -1 -> x = 2.0
    assert(std::abs(ElementMapping1D::referenceToPhysical(node_x, -1.0) - 2.0) < 1e-12);
    // xi = 0  -> x = 4.0
    assert(std::abs(ElementMapping1D::referenceToPhysical(node_x, 0.0) - 4.0) < 1e-12);
    // xi = 1  -> x = 6.0
    assert(std::abs(ElementMapping1D::referenceToPhysical(node_x, 1.0) - 6.0) < 1e-12);

    // 3. Verify Physical Derivatives: dN1/dx = -0.5 / 2.0 = -0.25, dN2/dx = 0.5 / 2.0 = 0.25
    auto dN_dx = ElementMapping1D::computePhysicalDerivatives(node_x, 0.0);
    assert(std::abs(dN_dx[0] - (-0.25)) < 1e-12);
    assert(std::abs(dN_dx[1] - 0.25) < 1e-12);

    std::cout << "[PASS] Element Mapping test\n";
}

void testElementStiffness() {
    Mesh mesh;
    mesh.addNode(0, {0.0});
    mesh.addNode(1, {2.0}); // Element length L = 2.0
    mesh.addElement(0, {0, 1});

    const auto& elem = mesh.getElements()[0];
    double k = 1.0;

    auto Ke = ElementMatrix1D::computeStiffness(elem, mesh, k, 2);

    // Assert analytical values K_e = [[0.5, -0.5], [-0.5, 0.5]]
    assert(std::abs(Ke[0][0] - 0.5) < 1e-12);
    assert(std::abs(Ke[0][1] - (-0.5)) < 1e-12);
    assert(std::abs(Ke[1][0] - (-0.5)) < 1e-12);
    assert(std::abs(Ke[1][1] - 0.5) < 1e-12);

    std::cout << "[PASS] Element Stiffness Matrix test\n";
}

void testElementForce() {
    Mesh mesh;
    mesh.addNode(0, {0.0});
    mesh.addNode(1, {2.0}); // Element length L = 2.0
    mesh.addElement(0, {0, 1});

    const auto& elem = mesh.getElements()[0];
    double k = 1.0;

    // --- 1. Test without Dirichlet BCs ---
    std::vector<BoundaryCondition> no_bcs;
    DofManager dof_no_bc;
    dof_no_bc.setup(mesh, no_bcs);

    // Constant forcing function f(x) = 5.0 -> raw fe = [5.0, 5.0]
    auto f_constant = [](double) { return 5.0; };
    auto fe_const = ElementMatrix1D::computeForce(elem, mesh, f_constant, 2, dof_no_bc, k);

    assert(std::abs(fe_const[0] - 5.0) < 1e-12);
    assert(std::abs(fe_const[1] - 5.0) < 1e-12);

    // Spatially varying forcing function f(x) = x -> raw fe = [2/3, 4/3]
    auto f_linear = [](double x) { return x; };
    auto fe_lin = ElementMatrix1D::computeForce(elem, mesh, f_linear, 2, dof_no_bc, k);

    assert(std::abs(fe_lin[0] - (2.0 / 3.0)) < 1e-12);
    assert(std::abs(fe_lin[1] - (4.0 / 3.0)) < 1e-12);

    // --- 2. Test WITH Dirichlet BC on Node 0 (u(0) = 100.0) ---
    BoundaryCondition dirichlet_bc;
    dirichlet_bc.id = 1;
    dirichlet_bc.type = BCType::Dirichlet;
    dirichlet_bc.node_id = 0;
    dirichlet_bc.value = 100.0;
    dirichlet_bc.nx = -1.0;

    std::vector<BoundaryCondition> bcs = {dirichlet_bc};
    DofManager dof_bc;
    dof_bc.setup(mesh, bcs);

    // Expected calculation with f(x) = 5.0 and g_0 = 100.0:
    // Ke = [[0.5, -0.5], [-0.5, 0.5]]
    // fe[0] = 5.0 - (Ke[0][0] * 100.0) = 5.0 - 50.0 = -45.0
    // fe[1] = 5.0 - (Ke[1][0] * 100.0) = 5.0 - (-50.0) = 55.0
    auto fe_bc = ElementMatrix1D::computeForce(elem, mesh, f_constant, 2, dof_bc, k);

    assert(std::abs(fe_bc[0] - (-45.0)) < 1e-12);
    assert(std::abs(fe_bc[1] - 55.0) < 1e-12);

    std::cout << "[PASS] Element Force Vector test\n";
}

void testDofManager() {
    // Construct mesh in memory directly
    Mesh mesh;
    mesh.addNode(0, {0.0});
    mesh.addNode(1, {1.0});
    mesh.addNode(2, {2.0});
    mesh.addNode(3, {3.0});

    mesh.addElement(0, {0, 1});
    mesh.addElement(1, {1, 2});
    mesh.addElement(2, {2, 3});

    // Define Boundary Conditions
    BoundaryCondition bc1;
    bc1.id = 1;
    bc1.type = BCType::Dirichlet;
    bc1.node_id = 0;
    bc1.value = 100.0;
    bc1.nx = -1.0;

    BoundaryCondition bc2;
    bc2.id = 2;
    bc2.type = BCType::Dirichlet;
    bc2.node_id = 3;
    bc2.value = 50.0;
    bc2.nx = 1.0;

    std::vector<BoundaryCondition> bcs = {bc1, bc2};

    DofManager dof;
    dof.setup(mesh, bcs);

    // Total free degrees of freedom (Nodes 1 and 2 are free)
    assert(dof.getNumFreeDofs() == 2);

    // Verify Dirichlet boundary node lookups (Node 0 & Node 3)
    assert(dof.isDirichlet(0) == true);
    assert(dof.getDof(0) == -1);
    assert(std::abs(dof.getDirichletValue(0) - 100.0) < 1e-12);

    assert(dof.isDirichlet(3) == true);
    assert(dof.getDof(3) == -1);
    assert(std::abs(dof.getDirichletValue(3) - 50.0) < 1e-12);

    // Free node index assignments (Node 1 & Node 2)
    assert(dof.isDirichlet(1) == false);
    assert(dof.isDirichlet(2) == false);

    int dof1 = dof.getDof(1);
    int dof2 = dof.getDof(2);

    assert(dof1 >= 0 && dof1 < 2);
    assert(dof2 >= 0 && dof2 < 2);
    assert(dof1 != dof2);

    // Verify reverse mapping (dof_id -> node_id)
    assert(dof.getNode(dof1) == 1);
    assert(dof.getNode(dof2) == 2);

    std::cout << "[PASS] DofManager test\n";
}

int main() {
    std::cout << "--- Running Finite Element Unit Tests ---\n";

    testNode();
    testElement();
    testInputParser();
    testParseBC();
    testShapeFunctions();
    testElementMapping();
    testElementStiffness();
    testElementForce();
    testDofManager();

    std::cout << "All tests passed successfully!\n";
    return 0;
}
