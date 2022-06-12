#ifndef _SIMS_NODE_H
#define _SIMS_NODE_H

#include "coveringSubgraph.h"

class SimsNode : public CoveringSubgraph
{
public:
    using RelatorLengthType = uint16_t;

    SimsNode(RankType rank,
             DegreeType max_degree,
             unsigned int num_relators = 0);

    const unsigned int num_relators;

    bool relators_lift(const std::vector<std::vector<int>> &relators) const;
    bool relators_may_lift(const std::vector<std::vector<int>> &relators);
    bool may_be_minimal() const;

    std::vector<SimsNode> get_children(
        const std::vector<std::vector<int>> &relators) const;

private:
    bool _may_be_minimal(DegreeType basepoint) const;

    std::vector<RelatorLengthType> _lift_indices;
    std::vector<DegreeType> _lift_vertices;

};

#endif
