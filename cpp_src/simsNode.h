#ifndef LOW_INDEX_SIMS_NODE_H
#define LOW_INDEX_SIMS_NODE_H

#include "abstractSimsNode.h"

#include <memory>

namespace low_index {

/// A non-abstract SimsNode.
///
/// It has value semantics (except that operator= has not been implemented
/// yet since it was needed yet). It can be created with
/// "SimsNode myNode(...);" or "new SimsNode(...)" and can be copied or moved
/// and thus be used in an STL container or from python.
///
/// Creation of a SimsNode requires a heap allocation and is thus slower than
/// StackedSimsNode.
///
class SimsNode : public AbstractSimsNode
{
public:
    /// Create SimsNode for a covering graph that can have up to max_degree
    /// vertices with edges labeled by rank many letters.
    /// num_relators is the number of "short relators" that can be checked
    /// relator_may_lift.
    SimsNode(RankType rank,
             DegreeType max_degree,
             unsigned int num_relators = 0);

    /// Copy a different subclass of SimsNode.
    SimsNode(const AbstractSimsNode &other);
    /// Copy a SimsNode.
    SimsNode(const SimsNode &other);
    /// Move this SimsNode. Calling methods on the SimsNode we copied from
    /// will be unsafe.
    SimsNode(SimsNode &&other);

    // No operator=, but could be added if needed.
    
private:
    void _allocate_memory();

    // Memory on heap to store the graph.
    std::unique_ptr<uint8_t[]> _memory;
};

} // Namespace low_index

#endif
