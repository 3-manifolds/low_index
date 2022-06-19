#include "lowIndex.h"

#include "simsTree.h"

#include <algorithm>

namespace low_index {

static
Relator
_spin_relator(
    const Relator &r,
    const size_t i)
{
    Relator result;
    result.insert(result.end(), r.begin() + i, r.end());
    result.insert(result.end(), r.begin(), r.begin() + i);
    return result;
}
    
static
void
_spin_relator(
    const Relator &r,
    std::vector<Relator> * const result)
{
    for (size_t i = 0; i < r.size(); i++) {
        result->push_back(_spin_relator(r, i));
    }
}
    
static
std::vector<Relator>
_compute_short_relators_spin_short(
    const std::vector<Relator>::const_iterator &begin,
    const std::vector<Relator>::const_iterator &end,
    DegreeType max_degree)
{
    size_t total_length = 0;
    for (auto it = begin; it != end; it++) {
        total_length += it->size();
    }
    const size_t n = end - begin;
    const size_t avg = (total_length + n - 1) / n;
    const size_t max_len = std::max<size_t>(avg, max_degree);

    std::vector<Relator> result;
    
    for (auto it = begin; it != end; it++) {
        if (it->size() <= max_len) {
            _spin_relator(*it, &result);
        } else {
            result.push_back(*it);
        }
    }

    return result;
}
        
static
std::vector<Relator>
_compute_short_relators(
    const std::vector<Relator>::const_iterator &begin,
    const std::vector<Relator>::const_iterator &end,
    DegreeType max_degree,
    const std::string &strategy)
{
    if (strategy == "spinShort") {
        return _compute_short_relators_spin_short(
            begin, end, max_degree);
    }

    return { begin, end };
}
    
std::pair<std::vector<Relator>, std::vector<Relator>>
compute_short_and_long_relators(
    RankType rank,
    const std::vector<Relator> &relators,
    DegreeType max_degree,
    unsigned int num_long_relators,
    const std::string &strategy)
{
    if (num_long_relators >= relators.size()) {
        return { { }, relators };
    }
    const size_t num_short_relators = relators.size() - num_long_relators;
    
    std::vector<Relator> sorted_relators(relators);
    std::stable_sort(
        sorted_relators.begin(),
        sorted_relators.end(),
        [](const Relator &a, const Relator &b) {
            return a.size() < b.size();});
    
    const auto first_long_relator_iter =
        sorted_relators.begin() + num_short_relators;

    return {
        _compute_short_relators(
            sorted_relators.begin(),
            first_long_relator_iter,
            max_degree,
            strategy),
        { first_long_relator_iter, sorted_relators.end() } };
}
    
std::vector<std::vector<std::vector<DegreeType>>>
permutation_reps(
    const RankType rank,
    const std::vector<Relator> &relators,
    const DegreeType max_degree,
    const unsigned int num_long_relators,
    const std::string &strategy,
    const size_t bloom_size,
    const unsigned int thread_num)
{
    const std::pair<std::vector<Relator>, std::vector<Relator>> rels =
        compute_short_and_long_relators(
            rank, relators, max_degree, num_long_relators, strategy);
    
    SimsTree t(rank, max_degree, rels.first, rels.second);

    std::vector<std::vector<std::vector<DegreeType>>> result;

    for (const SimsNode &n : t.list(bloom_size, thread_num)) {
        result.push_back(n.permutation_rep());
    }

    return result;
}

}
