#include "simsTreeMultiThreaded.h"

#include "stackedSimsNode.h"

#include <thread>

namespace low_index {

SimsTreeMultiThreaded::SimsTreeMultiThreaded(
    const RankType rank,
    const DegreeType max_degree,
    const std::vector<Relator> &short_relators,
    const std::vector<Relator> &long_relators,
    const unsigned int num_threads)
  : SimsTreeBase(rank, max_degree, short_relators, long_relators)
  , _num_threads(num_threads)
  , _recursion_stop_requested(false)
  , _nodes(nullptr)
  , _node_index(0)
  , _num_working_threads(0)
{
}

// Recurse a SimsNode, similar to SimsTree::_recurse but writing the result
// to _Node and checking _recursion_stop_requested to stop recursing.
void
SimsTreeMultiThreaded::_recurse(
    const StackedSimsNode &n,
    _Node * const result)
{
    if(n.is_complete()) {
        if (!n.relators_lift(_long_relators)) {
            return;
        }
        SimsNode copy(n);
        if (!copy.relators_may_lift(_short_relators, {0,0}, 0)) {
            return;
        }
        result->complete_nodes.push_back(std::move(copy));
        return;
    }

    const std::pair<LetterType, DegreeType> slot = n.first_empty_slot();
    const DegreeType m = std::min<DegreeType>(n.degree() + 1, n.max_degree());
    for (DegreeType v = 1; v <= m; v++) {
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

        if (!result->children.empty()) {
            // This thread responded to the recursion stop requested
            // earlier - all nodes that still need to be recursed
            // are added to children.
            result->children.emplace_back(new_subgraph);
            continue;
        }

        // No benefit of stopping recursion when we are at the complete
        // leaves of the search tree.
        if (!n.is_complete()) {
            // Check whether the stop recursion flag was set.
            // Use exchange so that only one thread responds to it.
            if (_recursion_stop_requested.exchange(false)) {
                // Record SimsNode as needing to be recursed.
                result->children.emplace_back(new_subgraph);
                continue;
            }
        }

        _recurse(new_subgraph, result);
    }
}

void
SimsTreeMultiThreaded::_recurse(
    _Node * const node)
{
    // Allocate all the memory needed to recurse the SimsNode.
    SimsNodeStack stack(node->root);
    _recurse(stack.get_node(), node);
}

void
SimsTreeMultiThreaded::_thread_worker()
{
    while(true) {
        // All logic to determine whether the queue is empty,
        // pull off the next node or keep track of the number of
        // working threads is protected by the mutex.
        std::unique_lock<std::mutex> lk(_mutex);

        const size_t index = _node_index;
        const size_t n = _nodes->size();

        if (index < n) {
            // There was work on the queue.

            _num_working_threads++;

            // Pull off the next node from the queue.
            _node_index++;
            std::vector<_Node> &nodes = *_nodes;

            // Release lock and recurse the node.
            lk.unlock();
            _Node &node = nodes[index];
            _recurse(&node);
            const bool has_children = !node.children.empty();
            lk.lock();

            if (has_children) {
                // This thread stopped recursing and filled
                // _Node::children instead.
                //
                // Swap the queue to _Node::children.
                //
                // Note that this is safe: the flag to request
                // stop recursing was raised only once when the
                // queue ran empty and at most one thread responds
                // to it.
                _nodes = &node.children;
                _node_index = 0;

                // Note that _num_working_threads is decreased after we have
                // refilled the queue:
                // decreasing _num_working_threads to 0 without a new queue
                // could trigger the threads to terminate.
            }

            _num_working_threads--;

            if (has_children || _num_working_threads == 0) {
                // Wake up threads because there is new work
                // or there are no more threads recursing and the threads need
                // to terminate.
                _wake_up_threads.notify_all();
            }
        } else {
            if (_num_working_threads == 0) {
                // No _Node's left in the queue and no thread is recursing.
                // Terminate.
                break;
            }

            if (index == n) {
                // The last node was just pulled off the queue, raise the
                // flag to stop recursing to refill the queue (no thread
                // might respond because they are all close to finishing,
                // but that is fine).
                // Increase _node_index to raise the
                // flag only once until the queue has been refilled.
                _node_index++;
                _recursion_stop_requested = true;
            }

            // Sleep until either the work queue has been refilled or
            // _num_working_threads changed.
            _wake_up_threads.wait(lk);
        }
    }
}

void
SimsTreeMultiThreaded::_merge_vectors(
    const std::vector<_Node> &nodes,
    std::vector<SimsNode> * const result)
{
    for (const auto &node : nodes) {
        for (const SimsNode &complete_node : node.complete_nodes) {
            result->push_back(complete_node);
        }
        _merge_vectors(node.children, result);
    }
}

std::vector<SimsNode>
SimsTreeMultiThreaded::_list()
{
    // The root _Node containing a SimsNode without any edges.
    std::vector<_Node> root_nodes{_Node(_root)};
    // Fill the queue.
    _nodes = &root_nodes;

    // Start the threads
    std::vector<std::thread> threads;
    threads.reserve(_num_threads);
    for (unsigned int i = 0; i < _num_threads; i++) {
        threads.emplace_back(&SimsTreeMultiThreaded::_thread_worker, this);
    }

    // Wait for all threads to finish.
    for (std::thread &t : threads) {
        t.join();
    }

    // Traverse the _Node tree to find all complete covering
    // graphs.
    std::vector<SimsNode> result;
    _merge_vectors(root_nodes, &result);
    return result;
}

} // Namespace low_index
