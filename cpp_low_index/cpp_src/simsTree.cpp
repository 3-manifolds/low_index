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
}

std::vector<SimsNode>
SimsTree::_list()
{
    // Allocate all memory needed to recurse up front.
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
        // Even though the graph is complete and thus relators_may_lift
        // won't add edges itself, it is still marked as non-const.
        // So make a copy - allocating memory on the heap, which we need
        // to do anyway to add the result to _complete_nodes.
        SimsNode copy(n);
        if (!copy.relators_may_lift(_short_relators, {0,0}, 0)) {
            return;
        }
        _complete_nodes.push_back(std::move(copy));
        return;
    }

    // Find vertex and letter so that no edge labeled by letter starts at the
    // vertex.
    const std::pair<LetterType, DegreeType> slot = n.first_empty_slot();
    const DegreeType m = std::min<DegreeType>(n.degree() + 1, n.max_degree());
    // Iterate through vertices where this edge could end.
    for (DegreeType v = 1; v <= m; v++) {
        // If there is already an edge with the given label ending at v,
        // we can add an edge to v.
        if (n.act_by(-slot.first, v) != 0) {
            continue;
        }
        StackedSimsNode new_subgraph(n);
        new_subgraph.add_edge(slot.first, slot.second, v);
        if (!new_subgraph.relators_may_lift(_short_relators, slot, v)) {
            continue;
        }
        if (!new_subgraph.may_be_minimal()) {
            continue;
        }
        _recurse(new_subgraph);
    }
}

} // Namespace low_index
