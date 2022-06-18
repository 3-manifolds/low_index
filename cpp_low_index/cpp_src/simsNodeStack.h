#ifndef LOW_INDEX_SIMS_NODE_STACK_H
#define LOW_INDEX_SIMS_NODE_STACK_H

#include "stackedSimsNode.h"

namespace low_index {

class SimsNodeStack
{
public:
    SimsNodeStack(const SimsNode &node);

    const StackedSimsNode &get_node() const {
        return _node;
    };

private:
    void * operator new(size_t size) = delete;

    static size_t _compute_memory_size(const SimsNode &node);

    std::unique_ptr<uint8_t[]> _memory;
    StackedSimsNode _node;
};

} // Namespace low_index

#endif
