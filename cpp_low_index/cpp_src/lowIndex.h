#ifndef LOW_INDEX_LOW_INDEX_H
#define LOW_INDEX_LOW_INDEX_H

#include "types.h"

#include <utility>
#include <string>

namespace low_index {

extern const std::string spin_short_strategy;
constexpr size_t default_bloom_size = 2000;
    
Relator
parse_word(
    RankType rank,
    const std::string &word);

std::pair<std::vector<Relator>, std::vector<Relator>>
compute_short_and_long_relators(
    RankType rank,
    const std::vector<Relator> &relators,
    DegreeType max_degree,
    unsigned int num_long_relators = 0,
    const std::string &strategy = spin_short_strategy);

// thread_num = 0 means that the number of cores will be used
// as number of threads.
std::vector<std::vector<std::vector<DegreeType>>>
permutation_reps(
    RankType rank,
    const std::vector<Relator> &relators,
    DegreeType max_degree,
    unsigned int num_long_relators = 0,
    const std::string &strategy = spin_short_strategy,
    size_t bloom_size = default_bloom_size,
    unsigned int thread_num = 0);

std::vector<std::vector<std::vector<DegreeType>>>
permutation_reps(
    RankType rank,
    const std::vector<std::string> &relators,
    DegreeType max_degree,
    unsigned int num_long_relators = 0,
    const std::string &strategy = spin_short_strategy,
    size_t bloom_size = default_bloom_size,
    unsigned int thread_num = 0);

}

#endif
