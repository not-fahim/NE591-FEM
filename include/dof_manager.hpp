#pragma once
#include <vector>
#include <cstddef>
#include "mesh.hpp"
#include "boundary_conditions.hpp"

class DofManager {
private:
	std::size_t num_free_dofs = 0;
	std::vector<int> node_to_dof;  // node_id -> dof index (-1 for dirichlet)
	std::vector<int> dof_to_node;  // dof_id -> node_id
	std::vector<double> dirichlet_val; // node_id -> boundary value

public:
	DofManager() = default;
	void setup(const Mesh& mesh, const std::vector<BoundaryCondition>& bcs);

	[[nodiscard]] std::size_t getNumFreeDofs() const noexcept { return num_free_dofs; }
	[[nodiscard]] int getDof(std::size_t node_id) const noexcept { return node_to_dof[node_id]; }
	[[nodiscard]] int getNode(std::size_t dof_id) const noexcept { return dof_to_node[dof_id]; }
	[[nodiscard]] bool isDirichlet(std::size_t node_id) const noexcept { return node_to_dof[node_id] == -1; }
	[[nodiscard]] double getDirichletValue(std::size_t node_id) const noexcept { return dirichlet_val[node_id]; }

	// Raw pointer access for high-performance inner assembly loops
	[[nodiscard]] const int* getNodeToDofData() const noexcept { return node_to_dof.data(); }
	[[nodiscard]] const int* getDofToNodeData() const noexcept { return dof_to_node.data(); }
};
