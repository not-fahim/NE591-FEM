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

void testFullPipeline() {
    const std::string test_filename = "master_input.dat";

    // Generate a single master input file containing Mesh and BC definitions
    std::ofstream out(test_filename);
    out << "# Single Input File for FE Pipeline Test\n"
        << "NODES 3\n"
        << "0 0.0\n"
        << "1 1.0\n"
        << "2 2.0\n"
        << "ELEMENTS 2\n"
        << "0 0 1\n"
        << "1 1 2\n"
        << "BOUNDARY_CONDITIONS 2\n"
        << "1 Dirichlet 0 100.0 -1.0\n"
        << "2 Neumann 2 10.0 1.0\n";
    out.close();

    // Parse Mesh from the single input file
    Mesh domain;
    bool mesh_parsed = InputParser::parse_mesh(test_filename, domain);
    assert(mesh_parsed && "Mesh parsing failed");
    assert(domain.getNodes().size() == 3);
    assert(domain.getElements().size() == 2);

    //Parse Boundary Conditions from the same input file
    std::vector<BoundaryCondition> bcs;
    bool bc_parsed = InputParser::parse_bc(test_filename, bcs);
    assert(bc_parsed && "Boundary condition parsing failed");
    assert(bcs.size() == 2);

    //Clean up temporary file
    std::remove(test_filename.c_str());

    //Setup Degree of Freedom Manager
    DofManager dof;
    dof.setup(domain, bcs);

    assert(dof.isDirichlet(0) == true);
    assert(std::abs(dof.getDirichletValue(0) - 100.0) < 1e-12);
    assert(dof.getDof(0) == -1);
    assert(dof.getNumFreeDofs() == 2); // Nodes 1 and 2 are free DOFs

    //Execute Element Mapping, Shape Functions, and Matrix Assembly
    const double k_material = 1.0;
    auto f_source = [](double x) { return 2.0 * x; };

    for (const auto& elem : domain.getElements()) {
        std::vector<double> elem_coords = domain.getElementNodeCoords1D(elem);

        //heck Jacobian (Length L = 1.0 -> J = 0.5)
        double J = ElementMapping1D::computeJacobian(elem_coords, 0.0);
        assert(std::abs(J - 0.5) < 1e-12);

        //Check Physical Derivatives
        auto dN_dx = ElementMapping1D::computePhysicalDerivatives(elem_coords, 0.0);
        assert(std::abs(dN_dx[0] - (-1.0)) < 1e-12);
        assert(std::abs(dN_dx[1] - 1.0) < 1e-12);

        //Compute Element Stiffness using updated ElementMatrix1D signature
        auto Ke = ElementMatrix1D::computeStiffness(elem, domain, k_material, 2);
        assert(std::abs(Ke[0][0] - 1.0) < 1e-12);
        assert(std::abs(Ke[0][1] - (-1.0)) < 1e-12);

        //Compute Element Force Vector including BC application
        auto fe = ElementMatrix1D::computeForce(elem, domain, f_source, 2, dof, k_material);
        assert(fe.size() == 2);

        //Verify DOF mapping for element nodes
        int dof0 = dof.getDof(elem.getNodeIds()[0]);
        int dof1 = dof.getDof(elem.getNodeIds()[1]);

        if (elem.getId() == 0) {
            assert(dof0 == -1); //Node 0 (Dirichlet)
            assert(dof1 >= 0);  //Node 1 (Free)

            // For Element 0, Node 0 has u(0) = 100.0
            // Ke = [[1, -1], [-1, 1]], raw_fe = [1/3, 2/3]
            // fe[0] = 1/3 - 1.0 * 100 = -299/3
            // fe[1] = 2/3 - (-1.0) * 100 = 302/3
            assert(std::abs(fe[0] - (1.0 / 3.0 - 100.0)) < 1e-10);
            assert(std::abs(fe[1] - (2.0 / 3.0 + 100.0)) < 1e-10);
        } else if (elem.getId() == 1) {
            assert(dof0 >= 0);  // Node 1 (Free)
            assert(dof1 >= 0);  // Node 2 (Free)

            // Element 1 has no Dirichlet nodes, so force is purely from source f(x)=2x
            // x in [1, 2] -> fe[0] = 4/3, fe[1] = 5/3
            assert(std::abs(fe[0] - (4.0 / 3.0)) < 1e-10);
            assert(std::abs(fe[1] - (5.0 / 3.0)) < 1e-10);
        }
    }

    std::cout << "[PASS] Full End-to-End FE Pipeline Test\n";
}

int main() {
    std::cout << "--- Executing Integrated Pipeline Test ---\n";
    testFullPipeline();
    std::cout << "All integration checks passed successfully!\n";
    return 0;
}
