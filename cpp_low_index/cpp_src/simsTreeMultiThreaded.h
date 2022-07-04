#ifndef LOW_INDEX_SIMS_TREE_MULTI_THREADED_H
#define LOW_INDEX_SIMS_TREE_MULTI_THREADED_H

#include "simsTreeBase.h"

#include <atomic>
#include <condition_variable>
#include <mutex>

namespace low_index {

/// A multi-threaded implementation of SimsTreeBase.
///
/// Note that this implementation lists the SimsNode's in the same order
/// as SimsTree (independent of the number of threads).
///
class SimsTreeMultiThreaded : public SimsTreeBase
{
public:
    /// See SimsTreeBase for basic arguments.
    ///
    /// num_threads is the number of threads spawned to list the SimsNode and
    /// has to be positive.
    SimsTreeMultiThreaded(
        RankType rank,
        DegreeType max_degree,
        const std::vector<Relator> &short_relators,
        const std::vector<Relator> &long_relators,
        unsigned int num_threads);

protected:
    std::vector<SimsNode> _list() override;

private:
    /// Multi-threaded implementation
    ///
    /// Each worker thread is trying to pull the next SimsNode off a queue
    /// to recursively add edges to the SimsNode to find complete covering
    /// graphs. When a worker thread notices that the last item has just been
    /// pulled from the queue, it requests that a different thread stops
    /// recursing to add all its nodes still requiring processing to the queue
    /// instead.
    ///
    /// Before the worker threads are started, the queue is initialized with
    /// just the root SimsNode which has no edges. The worker threads
    /// terminate when the last item has been pulled from the queue and no
    /// other thread is currently recursing a SimsNode.
    ///
    /// The queue actually does not store SimsNode's but _Node's where _Node
    /// is a structure wrapping a SimsNode. Note that while access the queue
    /// is protected by a mutex, a thread can release the mutex once it has
    /// pulled off the _Node from the queue (by increasing the index pointing
    /// the current _Node in the queue). To make it possible that a thread
    /// can write the complete or partial result of recursing a SimsNode
    /// without locking, each _Node contains fields where these results can
    /// be stored. That is, a _Node can store the complete covering graphs
    /// and the potentially incomplete SimsNode's still requiring processing
    /// (when there was a request to stop recursing). The latter SimsNode's
    /// are again stored as a vector of _Node's.
    ///
    /// In other words, the _Node's form a rooted tree. Once the threads
    /// have finished, this tree is a collapsed version of the entire
    /// search tree (branches that yield no complete covering graphs might
    /// have been removed and intermediate incomplete SimsNode's skipped).
    /// All complete covering graphs can be recovered (in the same order as
    /// SimsTree::list()) by a depth-first traversal of _Node's, collecting
    /// the _Node::complete_nodes before traversing the children.
    ///
    /// For any _Node and at any time, there is at most one thread that
    /// is recursing the _Node and writing to it.
    ///
    /// The queue consists of a pointer to a vector of _Node's and an
    /// index into that vector. The index is also used to raise a flag
    /// exactly once everytime a thread notices that the last _Node from
    /// the qeue was just pulled. This flag is then picked up by one other
    /// thread to stop recursing and refill the queue. That is, the other
    /// thread fills _Node::children and swaps the queue to point to it,
    /// resetting the index to zero.

    /// A node in the collapsed search tree.
    class _Node {
    public:
        _Node(const SimsNode &root)
          : root(root)
        { }
        /// SimsNode to recurse.
        const SimsNode root;

        /// Filled by _recurse with complete nodes.
        std::vector<SimsNode> complete_nodes;
        /// Filled by _recurse with nodes that still need to be
        /// recursed (if this thread was prompted to stop recursing).
        std::vector<_Node> children;
    };

    /// Recurse _Node::root and fill _Node::complete_nodes and
    /// _Node::children.
    void _recurse(
        _Node * node);
    void _recurse(
        const class StackedSimsNode &n,
        _Node * result);

    void _thread_worker();

    /// Collect all completed nodes from _Node's tree.
    static void _merge_vectors(
        const std::vector<_Node> &nodes,
        std::vector<SimsNode> * result);

    /// Number of threads to use.
    const unsigned int _num_threads;

    /// Signal that there is new work on the queue or that no thread
    /// is recursing and threads should terminate.
    std::condition_variable _wake_up_threads;
    /// Flag to request a thread to stop recursing and
    /// fill work queue instead. Atomic so that only
    /// one thread will stop recursing anytime it has been raised.
    std::atomic_bool _recursion_stop_requested;

    /// Mutex to protect _nodes, _node_index and _num_working_threads.
    std::mutex _mutex;

    /// The current queue of _Node's.
    std::vector<_Node> *_nodes;
    /// Index into _nodes.
    size_t _node_index;
    /// The number of theads currently busy recursing a _Node.
    unsigned int _num_working_threads;
};

} // Namespace low_index

#endif
