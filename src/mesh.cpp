#include "mesh.hpp"
#include <iostream>

// --- Node Implementation ---
Node::Node(std::size_t id, const std::vector<double>& coordinates) 
    : id(id), coords(coordinates) {}

std::size_t Node::getDimension() const {
    return coords.size();
}

// --- Element Implementation ---
Element::Element(std::size_t id, const std::vector<size_t>& connectivity) 
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

std::vector<double> Mesh::getElementNodeCoords1D(const Element& element) const {
    std::vector<double> node_x;
    node_x.reserve(element.numNodes());
    for (std::size_t node_id : element.getNodeIds()) {
        node_x.push_back(nodes[node_id].getCoords()[0]);
    }
    return node_x;
}

void Mesh::printSummary() const {
    std::cout << "Mesh contains " << nodes.size() << " nodes and " 
              << elements.size() << " elements.\n";
}
