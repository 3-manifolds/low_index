#include "stackedSimsNode.h"

template<typename T>
static
T* _move_pointer(T* const p, const size_t n)
{
    return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(p) + n);
}

StackedSimsNode::StackedSimsNode(const StackedSimsNode &other)
  : SimsNode(other)
{
    _outgoing = _move_pointer(other._outgoing, other._memory_size);
    _incoming = _move_pointer(other._incoming, other._memory_size);
    _lift_indices = _move_pointer(other._lift_indices, other._memory_size);
    _lift_vertices = _move_pointer(other._lift_vertices, other._memory_size);
    _memory_size = other._memory_size;

    _copy_memory(other);
}

StackedSimsNode::StackedSimsNode(
    const SimsNode &other,
    uint8_t * const memory)
 : SimsNode(other)
{
    const _MemoryLayout layout(*this);
    _apply_memory_layout(layout, memory);
    _copy_memory(other);
}
