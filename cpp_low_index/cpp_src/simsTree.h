#ifndef _SIMS_TREE_H
#define _SIMS_TREE_H

#include "heapedSimsNode.h"

class StackedSimsNode;

#include <deque>

class SimsTree
{
public:
    SimsTree(
        const HeapedSimsNode &root,
        const std::vector<std::vector<int>> &short_relators,
        const std::vector<std::vector<int>> &long_relators);

    SimsTree(
        int rank,
        int max_degree,
        const std::vector<std::vector<int>> &short_relators,
        const std::vector<std::vector<int>> &long_relators);

    HeapedSimsNode root;

    std::vector<HeapedSimsNode> list();

    std::deque<HeapedSimsNode> bloom(size_t n);
    
private:
    const std::vector<std::vector<int>> short_relators;
    const std::vector<std::vector<int>> long_relators;

    void _recurse(const StackedSimsNode &n, std::vector<HeapedSimsNode> *nodes);
};

#endif
