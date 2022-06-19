#include "stackedSimsNode.h"

namespace low_index {

template<typename T>
static
T* _move_pointer(T* const p, const size_t n)
{
    return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(p) + n);
}

StackedSimsNode::StackedSimsNode(const StackedSimsNode &other)
  : AbstractSimsNode(other)
{
    _outgoing = _move_pointer(other._outgoing, other._memory_size);
    _incoming = _move_pointer(other._incoming, other._memory_size);
    _lift_indices = _move_pointer(other._lift_indices, other._memory_size);
    _lift_vertices = _move_pointer(other._lift_vertices, other._memory_size);
    _memory_size = other._memory_size;

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
    return (node.max_degree() * node.rank() + 1) * layout.size;
}

SimsNodeStack::SimsNodeStack(const AbstractSimsNode &node)
  : _memory(std::make_unique<uint8_t[]>(_compute_memory_size(node)))
  , _node(node, _memory.get())
{
}

} // Namespace low_index
