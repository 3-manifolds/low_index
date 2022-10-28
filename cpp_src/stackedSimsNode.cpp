#include "stackedSimsNode.h"

namespace low_index {

// Move pointer by n bytes.
template<typename T>
static
T* _move_pointer(T* const p, const size_t n)
{
    return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(p) + n);
}

StackedSimsNode::StackedSimsNode(const StackedSimsNode &other)
  : AbstractSimsNode(other)
{
    // The pointers in the other StackedSimsNode point into
    // one "stack frame" in SimsNodeStack::_memory.
    // Offset all the pointers to point into the next "stack frame".
    _outgoing = _move_pointer(other._outgoing, other._memory_size);
    _incoming = _move_pointer(other._incoming, other._memory_size);
    _lift_indices = _move_pointer(other._lift_indices, other._memory_size);
    _lift_vertices = _move_pointer(other._lift_vertices, other._memory_size);
    // Carry over _memory size.
    _memory_size = other._memory_size;

    // And copy the data to our "stack frame"
    _copy_memory(other);
}

StackedSimsNode::StackedSimsNode(
    const AbstractSimsNode &other,
    uint8_t * const memory)
  : AbstractSimsNode(other)
{
    const _MemoryLayout layout(*this);
    _apply_memory_layout(layout, memory);
    _copy_memory(other);
}

size_t
SimsNodeStack::_compute_memory_size(const AbstractSimsNode &node)
{
    const StackedSimsNode::_MemoryLayout layout(node);
    // Enough memory for the initial node and the nested
    // copies.
    const size_t n = 1 + node.max_degree() * node.rank();
    return n * layout.size;
}

SimsNodeStack::SimsNodeStack(const AbstractSimsNode &node)
  // C++11:
  : _memory(new uint8_t[_compute_memory_size(node)])
  // C++14 and later:
//: _memory(std::make_unique<uint8_t[]>(_compute_memory_size(node)))
  , _node(node, _memory.get())
{
}

} // Namespace low_index
