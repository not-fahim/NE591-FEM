#pragma once
#include <vector>
#include <cstddef>

class Node {
private:
    std::size_t id;
    std::vector<double> coords;
public:
    Node(std::size_t id, const std::vector<double>& coordinates);
    std::size_t getId() const { return id; }
    const std::vector<double>& getCoords() const { return coords; }
    std::size_t getDimension() const;
};

class Element {
private:
    std::size_t id;
    std::vector<std::size_t> node_ids;
public:
    Element(std::size_t id, const std::vector<std::size_t>& connectivity);
    std::size_t getId() const { return id; }
    const std::vector<std::size_t>& getNodeIds() const { return node_ids; }
    std::size_t numNodes() const;
};

class Mesh {
private:
    std::vector<Node> nodes;
    std::vector<Element> elements;
public:
    void addNode(std::size_t id, const std::vector<double>& coords);
    void addElement(std::size_t id, const std::vector<std::size_t>& node_ids);

    const std::vector<Node>& getNodes() const;
    const std::vector<Element>& getElements() const;

    // Helper to fetch 1D physical coordinates (x) for an element's nodes
    std::vector<double> getElementNodeCoords1D(const Element& element) const;

    void printSummary() const;
};

