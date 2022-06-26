#include "lowIndex.h"

#include "words.h"
#include "simsTree.h"
#include "simsTreeMultiThreaded.h"

#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <thread>

namespace low_index {

const std::string spin_short_strategy = "spin_short";

std::vector<std::vector<std::vector<DegreeType>>>
permutation_reps(
    const RankType rank,
    const std::vector<Relator> &short_relators,
    const std::vector<Relator> &long_relators,
    const DegreeType max_degree,
    const std::string &strategy,
    const unsigned int thread_num)
{
    const std::vector<Relator> all_short_relators =
        (strategy == spin_short_strategy)
            ? spin(short_relators, max_degree)
            : short_relators;

    std::vector<std::vector<std::vector<DegreeType>>> result;

    std::unique_ptr<SimsTreeBase> t;

    const unsigned int resolved_thread_num =
        (thread_num > 0)
            ? thread_num
            : std::thread::hardware_concurrency();

    if (resolved_thread_num > 1) {
        t.reset(
            new SimsTreeMultiThreaded(
                rank, max_degree, all_short_relators, long_relators,
                resolved_thread_num));
    } else {
        t.reset(
            new SimsTree(
                rank, max_degree, all_short_relators, long_relators));
    }

    for (const SimsNode &n : t->list()) {
        result.push_back(n.permutation_rep());
    }

    return result;
}

static
std::vector<Relator>
parse_words(
    const RankType rank,
    const std::vector<std::string> &words)
{
    std::vector<Relator> result;
    result.reserve(words.size());
    for (const std::string &word : words) {
        result.push_back(parse_word(rank, word));
    }
    return result;
}

std::vector<std::vector<std::vector<DegreeType>>>
permutation_reps(
    const RankType rank,
    const std::vector<std::string> &short_relators,
    const std::vector<std::string> &long_relators,
    const DegreeType max_degree,
    const std::string &strategy,
    const unsigned int thread_num)
{
    return permutation_reps(
        rank,
        parse_words(rank, short_relators),
        parse_words(rank, long_relators),
        max_degree,
        strategy,
        thread_num);
}

}
