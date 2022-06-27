#include "simsTree.h"
#include "stackedSimsNode.h"

namespace low_index {

SimsTree::SimsTree(
    const RankType rank,
    const DegreeType max_degree,
    const std::vector<Relator> &short_relators,
    const std::vector<Relator> &long_relators)
  : SimsTreeBase(rank, max_degree, short_relators, long_relators)
{
    for (const Relator &relator : short_relators) {
        if (!(relator.size() < std::numeric_limits<RelatorLengthType>::max())) {
            throw std::domain_error(
                "Length of a relator can be at most " +
                std::to_string(
                    static_cast<int>(
                        std::numeric_limits<RelatorLengthType>::max())));
        }
    }
}

std::vector<SimsNode>
SimsTree::_list()
{
    SimsNodeStack stack(_root);
    _recurse(stack.get_node());
    return std::move(_complete_nodes);
}

void
SimsTree::_recurse(const StackedSimsNode &n)
{
    if(n.is_complete()) {
        if (!n.relators_lift(_long_relators)) {
            return;
        }
        SimsNode copy(n);
        if (!copy.relators_may_lift(_short_relators)) {
            return;
        }
        _complete_nodes.push_back(std::move(copy));
        return;
    }

    const std::pair<LetterType, DegreeType> slot = n.first_empty_slot();
    const DegreeType m = std::min<DegreeType>(n.degree() + 1, n.max_degree());
    for (DegreeType v = 1; v <= m; v++) {
        if (n.act_by(-slot.first, v) != 0) {
            continue;
        }
        StackedSimsNode new_subgraph(n);
        new_subgraph.add_edge(slot.first, slot.second, v);
        if (!new_subgraph.relators_may_lift(_short_relators)) {
            continue;
        }
        if (!new_subgraph.may_be_minimal()) {
            continue;
        }
        _recurse(new_subgraph);
    }
}

} // Namespace low_index
