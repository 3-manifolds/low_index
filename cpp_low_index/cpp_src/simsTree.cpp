#include "simsTree.h"
#include "stackedSimsNode.h"
#include "simsNodeStack.h"

#include <list>
#include <thread>
#include <atomic>

SimsTree::SimsTree(
    const HeapedSimsNode &root,
    const std::vector<std::vector<int>> &short_relators,
    const std::vector<std::vector<int>> &long_relators)
  : _short_relators(short_relators)
  , _long_relators(long_relators)
  , _root(root)
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

void
SimsTree::_recurse(
    const StackedSimsNode &n,
    std::vector<HeapedSimsNode> *nodes) const
{
    if(n.is_complete()) {
        if (n.relators_lift(_long_relators)) {
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
                if (new_subgraph.relators_may_lift(_short_relators)) {
                    if (new_subgraph.may_be_minimal()) {
                        _recurse(new_subgraph, nodes);
                    }
                }
            }
        }
    }
}

std::vector<HeapedSimsNode>
SimsTree::_bloom(const size_t n) const
{
    std::list<HeapedSimsNode> r = { _root };

    auto it = r.begin();
    bool has_incomplete_node = false;

    while (r.size() < n) {
        if (it == r.end()) {
            if (it == r.begin()) {
                break;
            }
            if (!has_incomplete_node) {
                break;
            }
            it = r.begin();
            has_incomplete_node = false;
        }

        if (it->is_complete()) {
            ++it;
        } else {
            const std::pair<CoveringSubgraph::LetterType,
                            CoveringSubgraph::DegreeType> slot =
                it->first_empty_slot();
            const CoveringSubgraph::DegreeType m =
                std::min<CoveringSubgraph::DegreeType>(
                    it->degree() + 1,
                    it->max_degree());
            for (CoveringSubgraph::DegreeType v = 1; v <= m; v++) {
                if (it->act_by(-slot.first, v) == 0) {
                    HeapedSimsNode new_subgraph(*it);
                    new_subgraph.add_edge(slot.first, slot.second, v);
                    if (new_subgraph.relators_may_lift(_short_relators)) {
                        if (new_subgraph.may_be_minimal()) {
                            r.insert(it, new_subgraph);
                            has_incomplete_node = true;
                        }
                    }
                }
            }
            it = r.erase(it);
        }
    }

    return std::vector<HeapedSimsNode>(r.begin(), r.end());
}

std::vector<HeapedSimsNode>
SimsTree::list(
    const size_t bloom_size,
    const unsigned int thread_num) const
{
    if (bloom_size <= 1 || thread_num <= 1) {
        return _list_single_threaded();
    } else {
        return _list_multi_threaded(bloom_size, thread_num);
    }
}

std::vector<HeapedSimsNode>
SimsTree::_list_single_threaded() const
{
    std::vector<HeapedSimsNode> nodes;

    SimsNodeStack stack(_root);

    _recurse(stack.GetNode(), &nodes);

    return nodes;
}

static
std::vector<HeapedSimsNode>
_merge_vectors(
    std::vector<std::vector<HeapedSimsNode>> &&vecs)
{
    std::vector<HeapedSimsNode> result;

    for (std::vector<HeapedSimsNode> &vec : vecs) {
        for (HeapedSimsNode &node : vec) {
            result.push_back(std::move(node));
        }
    }
    
    return result;
}

std::vector<HeapedSimsNode>
SimsTree::_list_multi_threaded(
    const size_t bloom_size,
    const unsigned int thread_num) const
{
    const std::vector<HeapedSimsNode> branches = _bloom(bloom_size);
    std::vector<std::vector<HeapedSimsNode>> nested_result(branches.size());

    std::atomic_size_t index(0);

    std::vector<std::thread> threads;
    threads.reserve(branches.size());
    for (unsigned int i = 0; i < thread_num; i++) {
        threads.emplace_back(
            &SimsTree::_thread_worker,
            this, branches, &index, &nested_result);
    }

    for (std::thread &t : threads) {
        t.join();
    }

    return _merge_vectors(std::move(nested_result));
}

void
SimsTree::_thread_worker(
    const std::vector<HeapedSimsNode> &branches,
    std::atomic_size_t * const index,
    std::vector<std::vector<HeapedSimsNode>> * nested_result) const
{
    while(true) {
        const size_t i = (*index)++;

        if (i >= branches.size()) {
            break;
        }

        const SimsTree t(branches[i], _short_relators, _long_relators);
        (*nested_result)[i] = t._list_single_threaded();
    }
}

