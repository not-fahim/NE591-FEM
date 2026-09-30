#include <iostream>
#include <cassert>
#include "mesh.hpp"

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

void testMesh() {
    Mesh domain;
    domain.addNode(0, {0.0});
    domain.addNode(1, {1.0});
    domain.addElement(0, {0, 1});

    assert(domain.getNodes().size() == 2);
    assert(domain.getElements().size() == 1);
    assert(domain.getNodes()[1].coords[0] == 1.0);
    std::cout << "[PASS] Mesh test\n";
}

int main() {
    std::cout << "--- Running Mesh Tests ---\n";
    
    testNode();
    testElement();
    testMesh();

    std::cout << "All tests passed successfully!\n";
    return 0;
}
