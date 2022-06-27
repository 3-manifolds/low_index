#ifndef LOW_INDEX_SIMS_TREE_MULTI_THREADED_H
#define LOW_INDEX_SIMS_TREE_MULTI_THREADED_H

#include "simsTreeBase.h"

#include <atomic>
#include <condition_variable>

namespace low_index {

/// A class to recursively add edges to a SimsNode to find all covering
/// subgraphs up to a certain degree such that the given relators lift.
///
/// In other words, a class to find all low-index subgroup H of a finitely
/// presented group G with rank generators and short_relators and long_relators
/// as relations. The maximal index H in G is max_degree.
///
/// As explained in AbstractSimsNode, the short_relators are checked while
/// recursively adding edges and the long_relators only once the covering
/// subgraph is complete.
///
/// The class only stores the root of the tree that is to be visited
/// recursively.
///
class SimsTreeMultiThreaded : public SimsTreeBase
{
public:
    /// Construct SimsTreeMultiThreaded with an empty root.
    /// That is a SimsNode for the given rank and max_degree and no edges.
    SimsTreeMultiThreaded(
        RankType rank,
        DegreeType max_degree,
        const std::vector<Relator> &short_relators,
        const std::vector<Relator> &long_relators,
        unsigned int num_threads);

    /// Find all complete covering subgraphs for which all relators lift.
    /// The parameters do not affect the result, but do affect the performance.
protected:
    std::vector<SimsNode> _list() override;

private:
    class _Node {
    public:
        _Node(const SimsNode &root)
          : root(root)
        { }
        // The node to process.
        const SimsNode root;

        // Filled by worker thread with complete nodes.
        std::vector<SimsNode> complete_nodes;
        // Filled by worker thread with nodes that still need to be
        // recursed if worker thread was prompted to stop recursing.
        std::vector<_Node> children;
    };

    void _recurse(
        const class StackedSimsNode &n,
        _Node * result);

    void _thread_worker();

    static void _merge_vectors(
        const std::vector<_Node> &nodes,
        std::vector<SimsNode> * result);

    const unsigned int _num_threads;

    std::vector<_Node> *_nodes;
    size_t _node_index;

    std::mutex _mutex;
    std::atomic_bool _recursion_stop_requested;
    unsigned int _num_working_threads;
    std::condition_variable _wake_up_threads;
};

} // Namespace low_index

#endif
