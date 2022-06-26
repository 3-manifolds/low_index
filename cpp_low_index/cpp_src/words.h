#ifndef LOW_INDEX_WORDS_H
#define LOW_INDEX_WORDS_H

#include "types.h"

#include <string>

namespace low_index {

Relator
parse_word(
    RankType rank,
    const std::string &word);

std::vector<Relator>
spin(const std::vector<Relator> &relators,
     DegreeType max_degree);

}

#endif
