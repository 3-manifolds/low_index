#include "simsNode.h"

SimsNode::SimsNode(
        const CoveringSubgraph::RankType rank,
        const CoveringSubgraph::DegreeType max_degree,
        const unsigned int num_relators)
 : CoveringSubgraph(rank, max_degree)
 , num_relators(num_relators)
 , _lift_indices(num_relators * max_degree, 0)
 , _lift_vertices(num_relators * max_degree, 0)
{
}

bool
SimsNode::relators_may_lift(const std::vector<std::vector<int>> &relators)
{
    for (size_t n = 0; n < relators.size(); n++) {
        for (unsigned int v = 0; v < degree; v++) {
            const size_t j = n * max_degree + v;
            
            DegreeType vertex = _lift_vertices[j];
            if (vertex == 255) {
                continue;
            }
            if (vertex == 0) {
                vertex = v + 1;
            }
            RelatorLengthType i;
            RelatorLengthType index = _lift_indices[j];
            RelatorLengthType save;
            int label;
            for (i = index; i < relators[n].size(); i++) {
                label = relators[n][i];
                save = vertex;
                vertex = act_by(label, vertex);
                if (vertex == 0) {
                    _lift_vertices[j] = save;
                    _lift_indices[j] = i;
                    break;
                }
            }

            if (i >= relators[n].size() - 1) {
                if (vertex == 0) {
                    if (!verified_add_edge(label, save, v + 1)) {
                        return false;
                    }
                    // Note that there is an "if child._is_complete(): return self.relators_may_lift(child, relators)"
                    // in covers.pxi - which I am skipping here.
                    // I hope that is correct.
                    vertex = v + 1;
                }
                if (vertex == v + 1) {
                    _lift_vertices[j] = 255;
                    _lift_indices[j] = relators[n].size();
                } else {
                    return false;
                }
            }
        }
    }

    return true;
}

bool
SimsNode::relators_lift(const std::vector<std::vector<int>> &relators) const
{
    for (const std::vector<int> &relator : relators) {
        for (DegreeType v = 1; v <= degree; v++) {
            DegreeType vertex = v;
            for (const int letter : relator) {
                vertex = act_by(letter, vertex);
                if (vertex == 0) {
                    throw std::domain_error("relators_lift: The graph is not a covering.");
                }
            }
            if (vertex != v) {
                return false;
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
