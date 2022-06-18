#ifndef _SIMS_TREE_H
#define _SIMS_TREE_H

#include "heapedSimsNode.h"

#include <atomic>

class StackedSimsNode;

class SimsTree
{
public:
    SimsTree(
        SimsNode::RankType rank,
        SimsNode::DegreeType max_degree,
        const std::vector<SimsNode::Relator> &short_relators,
        const std::vector<SimsNode::Relator> &long_relators);

    std::vector<HeapedSimsNode> list(
        size_t bloom_size,
        unsigned int thread_num) const;

private:
    SimsTree(
        const HeapedSimsNode &root,
        const std::vector<SimsNode::Relator> &short_relators,
        const std::vector<SimsNode::Relator> &long_relators);

    void _recurse(
        const StackedSimsNode &n,
        std::vector<HeapedSimsNode> *nodes) const;
    std::vector<HeapedSimsNode> _bloom(size_t n) const;

    std::vector<HeapedSimsNode> _list_single_threaded() const;
    std::vector<HeapedSimsNode> _list_multi_threaded(
        size_t bloom_size, unsigned int thread_num) const;

    void _thread_worker(
        const std::vector<HeapedSimsNode> &branches,
        std::atomic_size_t * index,
        std::vector<std::vector<HeapedSimsNode>> * nested_result) const;
    
    const std::vector<SimsNode::Relator> _short_relators;
    const std::vector<SimsNode::Relator> _long_relators;
    const HeapedSimsNode _root;
};

#endif
