#ifndef _SIMS_NODE_H
#define _SIMS_NODE_H

#include "coveringSubgraph.h"

class SimsNode : public CoveringSubgraph
{
public:
    SimsNode(RankType rank,
             DegreeType max_degree,
             unsigned int num_relators = 0);

    bool relators_may_lift(SimsNode * child,
                           const std::vector<std::vector<int>> &relators) const;
    bool may_be_minimal() const;

    std::vector<SimsNode> get_children(
        const std::vector<std::vector<int>> &relators) const;

private:
    bool _may_be_minimal(DegreeType basepoint) const;
    
    std::vector<unsigned int> _lift_indices;
    std::vector<DegreeType> _lift_vertices;

};

#endif
