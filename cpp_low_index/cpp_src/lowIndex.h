#ifndef LOW_INDEX_LOW_INDEX_H
#define LOW_INDEX_LOW_INDEX_H

#include "types.h"

#include <utility>
#include <string>

/// low_index is a module to compute low-index subgroups.
namespace low_index {

/// Generate more relator words by cyclically permuting the
/// given short relator words.
extern const std::string spin_short_strategy;

/// Given a finitely presented group of with rank generators
/// and relators short_relators and long_relators return a permutation
/// representation for each subgroup of index up to max_degree.
///
/// thread_num = 0 means that the number of cores will be used
/// as number of threads.
std::vector<std::vector<std::vector<DegreeType>>>
permutation_reps(
    RankType rank,
    const std::vector<Relator> &short_relators,
    const std::vector<Relator> &long_relators,
    DegreeType max_degree,
    const std::string &strategy = spin_short_strategy,
    unsigned int thread_num = 0);

/// An overload of permutation_reps that takes the relators as
/// SnapPy-style words.
std::vector<std::vector<std::vector<DegreeType>>>
permutation_reps(
    RankType rank,
    const std::vector<std::string> &short_relators,
    const std::vector<std::string> &long_relators,
    DegreeType max_degree,
    const std::string &strategy = spin_short_strategy,
    unsigned int thread_num = 0);

}

#endif
