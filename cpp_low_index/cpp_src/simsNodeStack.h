#ifndef _SIMS_NODE_STACK_H_
#define _SIMS_NODE_STACK_H_

#include "stackedSimsNode.h"

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

#endif
