#ifndef _TYPES_H_
#define _TYPES_H_

#include <cstdint>
#include <type_traits>
#include <vector>

namespace low_index {

// Index for vertices and degree of cover.
using DegreeType = uint8_t;
// Rank of a group.
using RankType = uint16_t;
// Letters in a group - thus also used to label edges.
using LetterType = std::make_signed<RankType>::type;

// Type for length of a relator.
using RelatorLengthType = uint16_t;
// Type for relator.
using Relator = std::vector<LetterType>;

}

#endif
