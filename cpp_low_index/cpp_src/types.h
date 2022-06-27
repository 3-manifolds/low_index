#ifndef LOW_INDEX_TYPES_H
#define LOW_INDEX_TYPES_H

#include <cstdint>
#include <type_traits>
#include <vector>

namespace low_index {

/// Degree of a covering graph/index of a subgraph.
/// Also used to index vertices of a covering graph.
using DegreeType = uint8_t;

/// Rank of a finitely presented group.
using RankType = uint16_t;

/// Letters for generators in a finitely presented groups.
/// A negative number corresponds to an inverse generator.
using LetterType = std::make_signed<RankType>::type;

/// Length of a word.
using RelatorLengthType = uint16_t;

/// A word in a finitely presented group.
using Relator = std::vector<LetterType>;

}

#endif
