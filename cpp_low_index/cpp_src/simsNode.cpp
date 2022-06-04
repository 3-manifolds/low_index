#include <iostream>

#include "simsNode.h"

SimsNode::SimsNode(
        const CoveringSubgraph::RankType rank,
        const CoveringSubgraph::DegreeType max_degree,
        const unsigned int num_relators)
 : CoveringSubgraph(rank, max_degree, num_relators)
 , _lift_indices(num_relators * max_degree, 0)
 , _lift_vertices(num_relators * max_degree, 0)
{
}

std::vector<SimsNode>
SimsNode::get_children(const std::vector<std::vector<int>> &relators) const
{
    std::vector<SimsNode> children;
    
    const std::pair<int, DegreeType> slot = first_empty_slot();

    if (slot == std::pair<int, DegreeType>{ 0, 0 }) {
        return {};
    }

    std::vector<int> targets;

    if (degree < max_degree) {
        targets.push_back(degree + 1);
    }

    if (slot.first > 0) {
        for (int n = 0; n < degree; n++) {
            if (incoming[n * rank + slot.first - 1] == 0) {
                targets.push_back(n+1);
            }
        }
    } else {
        for (int n = 0; n < degree; n++) {
            if (outgoing[n * rank - slot.first - 1] == 0) {
                targets.push_back(n+1);
            }
        }
    }

    children.reserve(targets.size());

    for (const int n : targets) {
        SimsNode new_subgraph(*this);
        new_subgraph.add_edge(slot.first, slot.second, n);
        if (relators_may_lift(&new_subgraph, relators)) {
            if (new_subgraph.may_be_minimal()) {
                children.push_back(std::move(new_subgraph));
            }
        }
    }

    return children;
}

bool
SimsNode::relators_may_lift(SimsNode * child,
                            const std::vector<std::vector<int>> &relators) const
{
    for (size_t n = 0; n < relators.size(); n++) {
        for (unsigned int v = 0; v < child->degree; v++) {

            const size_t j = n * max_degree + v;
            
            DegreeType vertex = _lift_vertices[j];
            if (vertex == 255) {
                continue;
            }
            if (vertex == 0) {
                vertex = v + 1;
            }
            unsigned int i;
            DegreeType index = _lift_indices[j];
            DegreeType save;
            int label;
            for (i = index; i < relators[n].size(); i++) {
                label = relators[n][i];
                save = vertex;
                if (label > 0) {
                    vertex = outgoing[rank * (vertex - 1) + label - 1];
                } else {
                    vertex = incoming[rank * (vertex - 1) - label - 1];
                }
                if (vertex == 0) {
                    child->_lift_vertices[j] = save;
                    child->_lift_indices[j] = i;
                    break;
                }
            }

            if (i >= relators[n].size() - 1) {
                if (vertex == 0) {
                    if (!child->verified_add_edge(label, save, v + 1)) {
                        return false;
                    }
                    if (child->is_complete()) {
                        return relators_may_lift(child, relators);
                    }
                    vertex = v + 1;
                }
                if (vertex == v + 1) {
                    child->_lift_vertices[j] = 255;
                    child->_lift_indices[j] = relators[n].size();
                } else {
                    return false;
                }
            }
        }
    }

    return true;
}

bool
SimsNode::may_be_minimal() const
{
    for (DegreeType basepoint = 2; basepoint <= degree; basepoint++) {
        if (!_may_be_minimal(basepoint)) {
            return false;
        }
    }
    return true;
}

bool
SimsNode::_may_be_minimal(const DegreeType basepoint) const
{
    DegreeType std_to_alt[degree+1];
    memset(std_to_alt, 0, sizeof(std_to_alt));

    DegreeType alt_to_std[degree+1];
    memset(alt_to_std, 0, sizeof(alt_to_std));

    std_to_alt[basepoint] = 1;
    alt_to_std[1] = basepoint;

    DegreeType max_index = 1;

    for (DegreeType slot_vertex = 1; slot_vertex <= degree; slot_vertex++) {
        for (RankType l = 0; l < rank; l++) {
            for (const std::vector<DegreeType> &edges : { outgoing, incoming }) {
                const DegreeType a = edges[
                    (slot_vertex - 1) * rank + l];
                const DegreeType b = edges[
                    (alt_to_std[slot_vertex] - 1) * rank + l];
                if (a == 0 or b == 0) {
                    return true;
                }
                DegreeType &c = std_to_alt[b];
                if (c == 0) {
                    max_index++;
                    c = max_index;
                    alt_to_std[max_index] = b;
                }
                if (c < a) {
                    return false;
                }
                if (c > a) {
                    return true;
                }
            }
        }
    }

    return true;
}
