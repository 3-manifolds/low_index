#ifndef _SIMS_TREE_H
#define _SIMS_TREE_H

#include "simsNode.h"

#include <deque>

class SimsTree
{
public:
    SimsTree(
        const SimsNode &root,
        const std::vector<std::vector<int>> &short_relators,
        const std::vector<std::vector<int>> &long_relators);

    SimsTree(
        int rank,
        int max_degree,
        const std::vector<std::vector<int>> &short_relators,
        const std::vector<std::vector<int>> &long_relators);

    SimsNode root;

    std::vector<SimsNode> list();

    std::deque<SimsNode> bloom(size_t n);
    
private:
    const std::vector<std::vector<int>> short_relators;
    const std::vector<std::vector<int>> long_relators;

    void _recurse(const SimsNode &n, std::vector<SimsNode> *nodes);
};

#endif
