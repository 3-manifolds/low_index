#ifndef LOW_INDEX_WORDS_H
#define LOW_INDEX_WORDS_H

#include "types.h"

#include <string>

/// Helpers for the low_index module.

namespace low_index {

/// Parse a SnapPy-style word. See overloads of permutation_reps for the
/// conventions.
Relator
parse_word(
    RankType rank,
    const std::string &word);

/// Given a set of relators, determine their average length.
/// For all relators shorter than this average or shorter than
/// max_degree, add all their cyclic shifts to the result.
std::vector<Relator>
spin_short(const std::vector<Relator> &relators,
           DegreeType max_degree);

}

#endif
