#ifndef LOW_INDEX_SIMS_TREE_BASE_H
#define LOW_INDEX_SIMS_TREE_BASE_H

#include "simsNode.h"
#include <algorithm>

namespace low_index {

/// A base class for algorithms to find all covering graphs of a given
/// finitely presented group G up to a given degree.
///
/// The constructor of an implementation of this class will at least take
/// the number of generators of G (rank), the max_degree and the relators of
/// the group partitioned into short_relators and long_relators.
///
/// As explained for permutation_reps, short_relators are
/// checked when recursing the incomplete covering subgraphs and long_relators
/// are only checked at the end.
///
class SimsTreeBase
{
public:
    /// Find all complete covering subgraphs for the given group G.
    ///
    /// Call this only once - some implementations store state so
    /// the second call will give the wrong result.
    ///
    std::vector<SimsNode> list();

    virtual ~SimsTreeBase();
    
protected:
    /// Create root SimsNode with no edges.
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
