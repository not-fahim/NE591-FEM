#include "mesh.hpp"
#include <iostream>

// --- Node Implementation ---
Node::Node(std::size_t id, const std::vector<double>& coordinates) 
    : id(id), coords(coordinates) {}

std::size_t Node::getDimension() const {
    return coords.size();
}

// --- Element Implementation ---
Element::Element(std::size_t id, const std::vector<std::size_t>& connectivity) 
    : id(id), node_ids(connectivity) {}

std::size_t Element::numNodes() const {
    return node_ids.size();
}

// --- Mesh Implementation ---
void Mesh::addNode(std::size_t id, const std::vector<double>& coords) {
    nodes.emplace_back(id, coords);
}

void Mesh::addElement(std::size_t id, const std::vector<std::size_t>& node_ids) {
    elements.emplace_back(id, node_ids);
}

const std::vector<Node>& Mesh::getNodes() const { 
    return nodes; 
}

const std::vector<Element>& Mesh::getElements() const { 
    return elements; 
}

void Mesh::printSummary() const {
    std::cout << "Mesh contains " << nodes.size() << " nodes and " 
              << elements.size() << " elements.\n";
}
