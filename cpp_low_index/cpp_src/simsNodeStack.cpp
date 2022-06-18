#include "simsNodeStack.h"

size_t
SimsNodeStack::_compute_memory_size(const SimsNode &node)
{
    const StackedSimsNode::_MemoryLayout layout(node);
    return node.max_degree() * node.rank() * layout.size;
}

SimsNodeStack::SimsNodeStack(const SimsNode &node)
  : _memory(std::make_unique<uint8_t[]>(_compute_memory_size(node)))
  , _node(node, _memory.get())
{
}
