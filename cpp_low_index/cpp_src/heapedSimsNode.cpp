#include "heapedSimsNode.h"

void
HeapedSimsNode::_AllocateMemory()
{
    const _MemoryLayout layout(*this);
    _memory = std::make_unique<uint8_t[]>(layout.size);
    _ApplyMemoryLayout(layout, _memory.get());
}

HeapedSimsNode::HeapedSimsNode(
    const RankType rank,
    const DegreeType max_degree,
    const unsigned int num_relators)
 : SimsNode(rank, max_degree, num_relators)
{
    _AllocateMemory();
    _InitializeMemory();
}

HeapedSimsNode::HeapedSimsNode(
    const SimsNode &other)
 : SimsNode(other)
{
    _AllocateMemory();
    _CopyMemory(other);
}

HeapedSimsNode::HeapedSimsNode(
    const HeapedSimsNode &other)
 : SimsNode(other)
{
    _AllocateMemory();
    _CopyMemory(other);
}
