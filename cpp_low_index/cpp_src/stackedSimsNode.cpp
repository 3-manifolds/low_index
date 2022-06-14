#include "stackedSimsNode.h"

template<typename T>
static
T* _MovePointer(T* const p, const size_t n)
{
    return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(p) + n);
}

StackedSimsNode::StackedSimsNode(const StackedSimsNode &other)
  : SimsNode(other)
{
    outgoing = _MovePointer(other.outgoing, other.size);
    incoming = _MovePointer(other.incoming, other.size);
    _lift_indices = _MovePointer(other._lift_indices, other.size);
    _lift_vertices = _MovePointer(other._lift_vertices, other.size);
    size = other.size;

    _CopyMemory(other);
}

StackedSimsNode::StackedSimsNode(
    const SimsNode &other,
    uint8_t * const memory)
 : SimsNode(other)
{
    const _MemoryLayout layout(*this);
    _ApplyMemoryLayout(layout, memory);
    _CopyMemory(other);
}
