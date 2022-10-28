#include "simsNode.h"

#include <limits>
#include <stdexcept>

namespace low_index {

void
SimsNode::_allocate_memory()
{
    const _MemoryLayout layout(*this);
    // C++11:
    _memory.reset(new uint8_t[layout.size]);
    // C++14 and later:
//  _memory = std::make_unique<uint8_t[]>(layout.size);
    _apply_memory_layout(layout, _memory.get());
}

SimsNode::SimsNode(
    const RankType rank,
    const DegreeType max_degree,
    const unsigned int num_relators)
 : AbstractSimsNode(rank, max_degree, num_relators)
{
    // Note that between the smallest and largest value a signed
    // integral type can have, the largest value has the smaller
    // absolute value.
    if (!(rank <= std::numeric_limits<LetterType>::max())) {
        throw std::domain_error(
            "rank can be at most " +
            std::to_string(static_cast<int>(
                               std::numeric_limits<LetterType>::max())));
    }
    if (!(max_degree < std::numeric_limits<DegreeType>::max())) {
        throw std::domain_error(
            "max_degree has to be smaller than " +
            std::to_string(static_cast<int>(
                               std::numeric_limits<DegreeType>::max())));
    }
    // In mutlti-threaded mode, we don't have a lot of stack space
    // (e.g., 512kB on MacOS). To avoid a crash when running out of stack,
    // limit the number of edges. Each stack frame is probably hundreds of
    // bytes.
    constexpr int max_num_edges = 1000;
    if (!(rank * max_degree <= max_num_edges)) {
        throw std::domain_error(
            "product of rank and gree can be at most " +
            std::to_string(max_num_edges));
    }

    _allocate_memory();
    _initialize_memory();
}

SimsNode::SimsNode(
    const AbstractSimsNode &other)
 : AbstractSimsNode(other)
{
    _allocate_memory();
    _copy_memory(other);
}

SimsNode::SimsNode(const SimsNode &other)
 : AbstractSimsNode(other)
{
    // Bail if invalid. This can happen if we moved from this SimsNode.
    if (!other._memory) {
        return;
    }
    _allocate_memory();
    _copy_memory(other);
}

SimsNode::SimsNode(SimsNode &&other)
 : AbstractSimsNode(other)
 , _memory(std::move(other._memory))
{
    // std::move changed owner ship of the heap allocated memory to
    // this node. Now we just need to point to the same memory than
    // the other node did.
    _outgoing = other._outgoing;
    _incoming = other._incoming;
    _lift_indices = other._lift_indices;
    _lift_vertices = other._lift_vertices;
    _memory_size = other._memory_size;
}

} // Namespace low_index
