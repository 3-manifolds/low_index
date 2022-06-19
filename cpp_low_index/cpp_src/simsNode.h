#ifndef LOW_INDEX_SIMS_NODE_H
#define LOW_INDEX_SIMS_NODE_H

#include "abstractSimsNode.h"

#include <memory>

namespace low_index {

class SimsNode : public AbstractSimsNode
{
public:
    SimsNode(RankType rank,
                   DegreeType max_degree,
                   unsigned int num_relators = 0);
    SimsNode(const AbstractSimsNode &other);
    SimsNode(const SimsNode &other);
    SimsNode(SimsNode &&other);

private:
    void _allocate_memory();
    
    std::unique_ptr<uint8_t[]> _memory;
};

} // Namespace low_index

#endif
