#ifndef LOW_INDEX_LOW_INDEX_H
#define LOW_INDEX_LOW_INDEX_H

#include "types.h"

#include <utility>
#include <string>

namespace low_index {

std::pair<std::vector<Relator>, std::vector<Relator>>
compute_short_and_long_relators(
    RankType rank,
    const std::vector<Relator> &relators,
    DegreeType max_degree,
    unsigned int num_long_relators = 0,
    const std::string &strategy = "spinShort");

std::vector<std::vector<std::vector<DegreeType>>>
permutation_reps(
    RankType rank,
    const std::vector<Relator> &relators,
    DegreeType max_degree,
    unsigned int num_long_relators = 0,
    const std::string &strategy = "spinShort",
    size_t bloom_size = 1000,
    unsigned int thread_num = 0);
}

#endif
