#ifndef LOW_INDEX_SIMS_TREE_H
#define LOW_INDEX_SIMS_TREE_H

#include "simsTreeBase.h"

namespace low_index {

/// A single-threaded implementation of SimsTreeBase.

/// It simply recursively adds edges to a SimsNode (starting with the
/// SimsNode with no edges) to find all covering graphs up to the given
/// degree.
///
class SimsTree : public SimsTreeBase
{
public:
    /// See SimsTreeBase for arguments.
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
