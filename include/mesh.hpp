#pragma once
#include <vector>
#include <cstddef> // Required for std::size_t

struct Node {
    std::size_t id;
    std::vector<double> coords;

    Node(std::size_t id, const std::vector<double>& coordinates);
    
    [[nodiscard]] std::size_t getDimension() const;
};

struct Element {
    std::size_t id;
    std::vector<std::size_t> node_ids; // Using size_t for indices

    Element(std::size_t id, const std::vector<std::size_t>& connectivity);
    
    [[nodiscard]] std::size_t numNodes() const;
};

class Mesh {
private:
    std::vector<Node> nodes;
    std::vector<Element> elements;

public:
    void addNode(std::size_t id, const std::vector<double>& coords);
    void addElement(std::size_t id, const std::vector<std::size_t>& node_ids);

    [[nodiscard]] const std::vector<Node>& getNodes() const;
    [[nodiscard]] const std::vector<Element>& getElements() const;
    
    void printSummary() const;
};
