#ifndef LOW_INDEX_SIMS_TREE_H
#define LOW_INDEX_SIMS_TREE_H

#include "simsNode.h"

#include <atomic>

namespace low_index {

class StackedSimsNode;

class SimsTree
{
public:
    SimsTree(
        RankType rank,
        DegreeType max_degree,
        const std::vector<Relator> &short_relators,
        const std::vector<Relator> &long_relators);

    /// Set thread_num = 0 to have the number of threads
    /// determined automatically using
    /// std::thread::hardware_concurrency().
    std::vector<SimsNode> list(
        size_t bloom_size,
        unsigned int thread_num) const;

private:
    SimsTree(
        const SimsNode &root,
        const std::vector<Relator> &short_relators,
        const std::vector<Relator> &long_relators);

    void _recurse(
        const StackedSimsNode &n,
        std::vector<SimsNode> *nodes) const;
    std::vector<SimsNode> _bloom(size_t n) const;

    std::vector<SimsNode> _list_single_threaded() const;
    std::vector<SimsNode> _list_multi_threaded(
        size_t bloom_size, unsigned int thread_num) const;

    void _thread_worker(
        const std::vector<SimsNode> &branches,
        std::atomic_size_t * index,
        std::vector<std::vector<SimsNode>> * nested_result) const;
    
    const std::vector<Relator> _short_relators;
    const std::vector<Relator> _long_relators;
    const SimsNode _root;
};

} // Namespace low_index

#endif
