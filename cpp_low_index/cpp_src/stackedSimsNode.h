#ifndef _STACKED_SIMS_TREE_H
#define _STACKED_SIMS_TREE_H

#include "simsNode.h"

class StackedSimsNode : public SimsNode
{
public:
    StackedSimsNode(const StackedSimsNode &other);

private:
    void * operator new(size_t size) = delete;

    friend class SimsNodeStack;
    StackedSimsNode(const SimsNode &other, uint8_t * memory);
};

#endif
