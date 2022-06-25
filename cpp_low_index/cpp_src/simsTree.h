#ifndef LOW_INDEX_SIMS_TREE_H
#define LOW_INDEX_SIMS_TREE_H

#include "simsTreeBase.h"

namespace low_index {

/// A class to recursively add edges to a SimsNode to find all covering
/// subgraphs up to a certain degree such that the given relators lift.
///
/// In other words, a class to find all low-index subgroup H of a finitely
/// presented group G with rank generators and short_relators and long_relators
/// as relations. The maximal index H in G is max_degree.
///
/// As explained in AbstractSimsNode, the short_relators are checked while
/// recursively adding edges and the long_relators only once the covering
/// subgraph is complete.
///
class SimsTree : public SimsTreeBase
{
public:
    /// Construct SimsTree with an empty root.
    /// That is a SimsNode for the given rank and max_degree and no edges.
    SimsTree(
        RankType rank,
        DegreeType max_degree,
        const std::vector<Relator> &short_relators,
        const std::vector<Relator> &long_relators);

protected:
    std::vector<SimsNode> _list() override;

private:
    void _recurse(const class StackedSimsNode &n);

    std::vector<SimsNode> _complete_nodes;
};

} // Namespace low_index

#endif
