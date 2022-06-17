#include <iostream>

#include "simsTree.h"
#include "stackedSimsNode.h"
#include "simsNodeStack.h"
#include "stackedSimsNode.h"

SimsTree::SimsTree(
    const HeapedSimsNode &root,
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
      HeapedSimsNode(rank, max_degree, short_relators.size()),
      short_relators,
      long_relators)
{
}

std::vector<HeapedSimsNode>
SimsTree::list()
{
    std::vector<HeapedSimsNode> nodes;

    SimsNodeStack stack(root);
    
    _recurse(stack.GetNode(), &nodes);

    return nodes;
}

void
SimsTree::_recurse(const StackedSimsNode &n, std::vector<HeapedSimsNode> *nodes)
{
    if(n.is_complete()) {
        if (n.relators_lift(long_relators)) {
            nodes->push_back(n);
        }
    } else {
        const std::pair<CoveringSubgraph::LetterType,
                        CoveringSubgraph::DegreeType> slot =
            n.first_empty_slot();
        const CoveringSubgraph::DegreeType m =
            std::min<CoveringSubgraph::DegreeType>(
                n.degree() + 1,
                n.max_degree());
        for (CoveringSubgraph::DegreeType v = 1; v <= m; v++) {
            if (n.act_by(-slot.first, v) == 0) {
                StackedSimsNode new_subgraph(n);
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

std::deque<HeapedSimsNode>
SimsTree::bloom(const size_t n)
{
    std::deque<HeapedSimsNode> result = { root };

    bool keepGoing = true;

    while (result.size() < n && keepGoing) {
        keepGoing = false;

        result.front();
    }

    return result;
}
