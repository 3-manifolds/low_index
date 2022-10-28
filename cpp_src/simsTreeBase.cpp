#include "simsTreeBase.h"

#include <limits>
#include <stdexcept>

namespace low_index {

SimsTreeBase::SimsTreeBase(
    const RankType rank,
    const DegreeType max_degree,
    const std::vector<Relator> &short_relators,
    const std::vector<Relator> &long_relators)
  : _root(rank, max_degree, short_relators.size())
  , _short_relators(short_relators)
  , _long_relators(long_relators)
{
    for (const Relator &relator : short_relators) {
        if (!(relator.size() < std::numeric_limits<RelatorLengthType>::max())) {
            throw std::domain_error(
                "Length of a relator can be at most " +
                std::to_string(
                    static_cast<int>(
                        std::numeric_limits<RelatorLengthType>::max())));
        }
    }
}

SimsTreeBase::~SimsTreeBase() = default;

std::vector<SimsNode>
SimsTreeBase::list() {
    return _list();
}

}
