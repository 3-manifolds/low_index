#include "simsNodeStack.h"

size_t
SimsNodeStack::_ComputeMemorySize(const SimsNode &node)
{
    const StackedSimsNode::_MemoryLayout layout(node);
    return node.max_degree * node.rank * layout.size;
}

SimsNodeStack::SimsNodeStack(const SimsNode &node)
  : _memory(std::make_unique<uint8_t[]>(_ComputeMemorySize(node)))
  , _node(node, _memory.get())
{
}
