#include "dof_manager.hpp"

void DofManager::setup(const Mesh& mesh, const std::vector<BoundaryCondition>& bcs) {
	const std::size_t num_nodes = mesh.getNodes().size();

	node_to_dof.assign(num_nodes, -1);
	dirichlet_val.assign(num_nodes, 0.0);

	std::vector<bool> is_dirichlet(num_nodes, false);
	for (const auto& bc : bcs) {
		if (bc.type == BCType::Dirichlet) {
			is_dirichlet[bc.node_id] = true;
			dirichlet_val[bc.node_id] = bc.value;
		}
	}

	num_free_dofs = 0;
	for (std::size_t i = 0; i < num_nodes; ++i) {
		if (!is_dirichlet[i]) {
			num_free_dofs++;
		}
	}

	dof_to_node.resize(num_free_dofs);

	int current_dof = 0;
	for (std::size_t node_id = 0; node_id < num_nodes; ++node_id) {
		if (!is_dirichlet[node_id]) {
			node_to_dof[node_id] = current_dof;
			dof_to_node[current_dof] = static_cast<int>(node_id);
			current_dof++;
		}
	}
}
