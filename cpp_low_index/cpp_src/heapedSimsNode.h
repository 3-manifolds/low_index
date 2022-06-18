#ifndef _HEAPED_SIMS_NODE_H_
#define _HEAPED_SIMS_NODE_H_

#include "simsNode.h"

#include <memory>

namespace low_index {

class HeapedSimsNode : public SimsNode
{
public:
    HeapedSimsNode(RankType rank,
                   DegreeType max_degree,
                   unsigned int num_relators = 0);
    HeapedSimsNode(const SimsNode &other);
    HeapedSimsNode(const HeapedSimsNode &other);
    HeapedSimsNode(HeapedSimsNode &&other);

private:
    void _allocate_memory();
    
    std::unique_ptr<uint8_t[]> _memory;
};

} // Namespace low_index

#endif
