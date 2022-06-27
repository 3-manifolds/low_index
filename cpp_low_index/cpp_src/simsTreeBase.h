#ifndef LOW_INDEX_SIMS_TREE_BASE_H
#define LOW_INDEX_SIMS_TREE_BASE_H

#include "simsNode.h"

namespace low_index {

/// Recurse to find all low-index subgroups.
class SimsTreeBase
{
public:
    /// List all subgroups.
    std::vector<SimsNode> list();

    virtual ~SimsTreeBase();
    
protected:
    SimsTreeBase(
        RankType rank,
        DegreeType max_degree,
        const std::vector<Relator> &short_relators,
        const std::vector<Relator> &long_relators);

    // Implements list()
    virtual std::vector<SimsNode> _list() = 0;
   
    const SimsNode _root;
    const std::vector<Relator> _short_relators;
    const std::vector<Relator> _long_relators;
};

}

#endif
