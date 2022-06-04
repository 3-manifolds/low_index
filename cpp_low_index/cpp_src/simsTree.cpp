#include <iostream>

#include "simsTree.h"

SimsTree::SimsTree(
    const int rank,
    const int max_degree,
    const std::vector<std::vector<int>> &relators)
  : root(rank, max_degree, relators.size())
  , relators(relators)
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
SimsTree::_recurse(const SimsNode &n, std::vector<SimsNode> *node)
{
    std::cout << n.to_string() << "\n" << std::endl;
    
    if(n.is_complete()) {
        node->push_back(n);
    } else {
        for (const SimsNode &child : n.get_children(relators)) {
            _recurse(child, node);
        }
    }
}
