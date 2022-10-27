#include "coveringSubgraph.h"

#include <stdexcept>

CoveringSubgraph::CoveringSubgraph(
        const int rank,
        const int max_degree,
        const int num_relators)
  : rank(rank)
  , degree(0)
  , max_degree(max_degree)
  , num_edges(0)
  , num_relators(num_relators)
  , outgoing(rank * max_degree, 0)
  , incoming(rank * max_degree, 0)
  , _slot_index(0)
{
}

std::string
CoveringSubgraph::to_string() const
{
    std::string result;
    if (is_complete()) {
        result = "Covering with edges:";
    } else {
        result = "Partial covering with edges:";
    }

    for (int f = 0; f < degree; f++) {
        for (int n = 0; n < rank; n++) {
            if (const IntType t = outgoing[f * rank + n]) {
                result +=
                    "\n" +
                    std::to_string(f+1) + "--" +
                    std::to_string(n+1) + "->" +
                    std::to_string(static_cast<int>(t));
            }
        }
    }

    return result;
}

std::vector<std::vector<int>>
CoveringSubgraph::permutation_rep() const
{
    if (!is_complete()) {
        throw std::domain_error("The graph is not a covering.");
    }

    std::vector<std::vector<int>> result;
    result.reserve(rank);
    
    for (int l = 0; l < rank; l++) {
        result.push_back({});
        std::vector<int> &r = result.back();
        r.reserve(degree);
        for (int v = 0; v < degree; v++) {
            r.push_back(outgoing[v * rank + l] - 1);
        }
    }
            
    return result;
}

void
CoveringSubgraph::add_edge(
    const int letter,
    const int from_vertex,
    const int to_vertex)
{
    if (letter < 0) {
        _add_edge<false>(-letter, to_vertex,   from_vertex);
    } else {
        _add_edge<false>( letter, from_vertex, to_vertex);
    }
}

bool
CoveringSubgraph::verified_add_edge(
    const int letter,
    const int from_vertex,
    const int to_vertex)
{
    if (letter < 0) {
        return _add_edge<true>(-letter, to_vertex,   from_vertex);
    } else {
        return _add_edge<true>( letter, from_vertex, to_vertex);
    }
}

template<bool check>
bool
CoveringSubgraph::_add_edge(
    const int label,
    const int from_vertex,
    const int to_vertex)
{
    if (from_vertex > degree || to_vertex > degree) {
        degree++;
    }
    const size_t out_index = (from_vertex - 1) * rank + (label - 1);
    const size_t in_index  = (to_vertex   - 1) * rank + (label - 1);
    if (check) {
        if (outgoing[out_index] != 0 || incoming[in_index] != 0) {
            return false;
        }
    }
    outgoing[out_index] = to_vertex;
    incoming[in_index]  = from_vertex;
    num_edges++;
    return true;
}

