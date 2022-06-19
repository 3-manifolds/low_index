#ifndef LOW_INDEX_STACKED_SIMS_TREE_H
#define LOW_INDEX_STACKED_SIMS_TREE_H

#include "abstractSimsNode.h"

#include <memory>

namespace low_index {

class StackedSimsNode : public AbstractSimsNode
{
public:
    StackedSimsNode(const StackedSimsNode &other);

private:
    void * operator new(size_t size) = delete;

    friend class SimsNodeStack;
    StackedSimsNode(const AbstractSimsNode &other, uint8_t * memory);
};

class SimsNodeStack
{
public:
    SimsNodeStack(const AbstractSimsNode &node);

    const StackedSimsNode &get_node() const {
        return _node;
    };

private:
    void * operator new(size_t size) = delete;

    static size_t _compute_memory_size(const AbstractSimsNode &node);

    std::unique_ptr<uint8_t[]> _memory;
    StackedSimsNode _node;
};

} // Namespace low_index

#endif
