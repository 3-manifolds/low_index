#include "heapedSimsNode.h"

void
HeapedSimsNode::_allocate_memory()
{
    const _MemoryLayout layout(*this);
    _memory = std::make_unique<uint8_t[]>(layout.size);
    _apply_memory_layout(layout, _memory.get());
}

HeapedSimsNode::HeapedSimsNode(
    const RankType rank,
    const DegreeType max_degree,
    const unsigned int num_relators)
 : SimsNode(rank, max_degree, num_relators)
{
    _allocate_memory();
    _initialize_memory();
}

HeapedSimsNode::HeapedSimsNode(
    const SimsNode &other)
 : SimsNode(other)
{
    _allocate_memory();
    _copy_memory(other);
}

HeapedSimsNode::HeapedSimsNode(
    const HeapedSimsNode &other)
 : SimsNode(other)
{
    _allocate_memory();
    _copy_memory(other);
}
