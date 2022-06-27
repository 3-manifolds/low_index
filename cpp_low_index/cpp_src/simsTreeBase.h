#ifndef LOW_INDEX_SIMS_TREE_BASE_H
#define LOW_INDEX_SIMS_TREE_BASE_H

#include "simsNode.h"

namespace low_index {

/// Recurse to find all low-index subgroups.
class SimsTreeBase
{
public:
    /// List all subgroups.
    std::vector<SimsNode> list() { return _list(); }

    virtual ~SimsTreeBase() = default;
    
protected:
    SimsTreeBase(
        RankType rank,
        DegreeType max_degree,
        const std::vector<Relator> &short_relators,
        const std::vector<Relator> &long_relators)
      : _root(rank, max_degree, short_relators.size())
      , _short_relators(short_relators)
      , _long_relators(long_relators)
    {
    }

    virtual std::vector<SimsNode> _list() = 0;
   
    const SimsNode _root;
    const std::vector<Relator> _short_relators;
    const std::vector<Relator> _long_relators;
};

}

#endif
