#include "simsTree.h"

SimsTree::SimsTree(
    const SimsNode &root,
    const std::vector<std::vector<int>> &short_relators,
    const std::vector<std::vector<int>> &long_relators)
  : root(root)
  , short_relators(short_relators)
  , long_relators(long_relators)
{
}

SimsTree::SimsTree(
    const int rank,
    const int max_degree,
    const std::vector<std::vector<int>> &short_relators,
    const std::vector<std::vector<int>> &long_relators)
  : SimsTree(
      SimsNode(rank, max_degree, short_relators.size()),
      short_relators,
      long_relators)
{
}

std::vector<SimsNode>
SimsTree::list()
{
    std::vector<SimsNode> nodes;

    _recurse(root, &nodes);
    
    return nodes;
}

void
SimsTree::_recurse(const SimsNode &n, std::vector<SimsNode> *nodes)
{
    if(n.is_complete() && n.relators_lift(long_relators)) {
        nodes->push_back(n);
    } else {
        const std::pair<CoveringSubgraph::LetterType,
                        CoveringSubgraph::DegreeType> slot =
            n.first_empty_slot();
        const CoveringSubgraph::DegreeType m =
            std::min<CoveringSubgraph::DegreeType>(n.degree + 1,
                                                n.max_degree);
        for (CoveringSubgraph::DegreeType v = 1; v <= m; v++) {
            if (n.act_by(-slot.first, v) == 0) {
                SimsNode new_subgraph(n);
                new_subgraph.add_edge(slot.first, slot.second, v);
                if (new_subgraph.relators_may_lift(short_relators)) {
                    if (new_subgraph.may_be_minimal()) {
                        _recurse(new_subgraph, nodes);
                    }
                }
            }
        }
    }
}

std::deque<SimsNode>
SimsTree::bloom(const size_t n)
{
    std::deque<SimsNode> result = { root };

    bool keepGoing = true;

    while (result.size() < n && keepGoing) {
        keepGoing = false;

        result.front();
    }

    return result;
}
