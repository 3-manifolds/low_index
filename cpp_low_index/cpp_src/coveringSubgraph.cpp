#include "coveringSubgraph.h"

#include <stdexcept>
#include <cstdlib>

CoveringSubgraph::CoveringSubgraph(
        const CoveringSubgraph::RankType rank,
        const CoveringSubgraph::DegreeType max_degree)
  : rank(rank)
  , degree(1)
  , max_degree(max_degree)
  , num_edges(0)
  , outgoing(rank * max_degree, 0)
  , incoming(rank * max_degree, 0)
  , _slot_index(0)
{
}

std::string
CoveringSubgraph::to_string() const
{
    std::string padding;
    for (unsigned int i = 0; i < num_edges; i++) {
        padding += "    ";
    }
    
    std::string result = padding;
    if (is_complete()) {
        result += "Covering";
    } else {
        result += "Partial covering";
    }
    result += " of degree " + std::to_string(degree);
    result += " with " + std::to_string(num_edges) + " edges";

    for (unsigned int f = 0; f < degree; f++) {
        for (unsigned int n = 0; n < rank; n++) {
            const DegreeType t = outgoing[f * rank + n];
            const DegreeType s = incoming[f * rank + n];

            if (t != 0 || s != 0) {
                result += "\n" + padding;
                if (t != 0) {
                    result +=
                        std::to_string(f+1) + "--( " +
                        std::to_string(n+1) + ")->" +
                        std::to_string(static_cast<int>(t));
                } else {
                    result +=
                        "         ";
                }
                result += "      ";
                if (s != 0) {
                    result +=
                        std::to_string(f+1) + "--(" +
                        std::to_string(-static_cast<int>(n+1)) + ")->" +
                        std::to_string(static_cast<int>(s));
                }
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
    
    for (unsigned int l = 0; l < rank; l++) {
        result.push_back({});
        std::vector<int> &r = result.back();
        r.reserve(degree);
        for (unsigned int v = 0; v < degree; v++) {
            r.push_back(outgoing[v * rank + l] - 1);
        }
    }
            
    return result;
}

void
CoveringSubgraph::add_edge(
    const LetterType letter,
    const DegreeType from_vertex,
    const DegreeType to_vertex)
{
    if (letter < 0) {
        if(!_add_edge<true>(-letter, to_vertex,   from_vertex)) {
            throw std::domain_error("Bad add edge.");
        }
    } else {
        if(!_add_edge<true>( letter, from_vertex, to_vertex)) {
            throw std::domain_error("Bad add edge.");
        }
    }
}

bool
CoveringSubgraph::verified_add_edge(
    const LetterType letter,
    const DegreeType from_vertex,
    const DegreeType to_vertex)
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
    const LetterType label,
    const DegreeType from_vertex,
    const DegreeType to_vertex)
{
    if (from_vertex > degree || to_vertex > degree) {
        degree++;
    }
    const unsigned int out_index = (from_vertex - 1) * rank + (label - 1);
    const unsigned int in_index  = (to_vertex   - 1) * rank + (label - 1);
    if (check) {
        if (outgoing[out_index] != 0 || incoming[in_index] != 0) {
            return false;
        }
    }

    if (out_index >= outgoing.size()) {
        throw std::domain_error("Bad1");
    }
    if (in_index >= incoming.size()) {
        throw std::domain_error("Bad1");
    }
    
    outgoing[out_index] = to_vertex;
    incoming[in_index]  = from_vertex;
    num_edges++;
    return true;
}

CoveringSubgraph::DegreeType
CoveringSubgraph::act_by(const LetterType letter, const DegreeType vertex) const
{
    if (letter > 0) {
        return outgoing[(vertex - 1) * rank + letter - 1];
    } else {
        return incoming[(vertex - 1) * rank - letter - 1];
    }
}

std::pair<CoveringSubgraph::LetterType, CoveringSubgraph::DegreeType>
CoveringSubgraph::first_empty_slot() const
{
    const unsigned int max_edges = rank * degree;

    if (max_edges == num_edges) {
        return { 0, 0 };
    }

    for(unsigned int n = _slot_index; n < max_edges; n++) {
        if (outgoing[n] == 0) {
            const std::div_t qr = std::div(
                static_cast<int>(n), static_cast<int>(rank));
            _slot_index = n;
            return { qr.rem + 1, qr.quot + 1 };
        }
        if (incoming[n] == 0) {
            const std::div_t qr = std::div(
                static_cast<int>(n), static_cast<int>(rank));
            _slot_index = n;
            return { -(qr.rem + 1), qr.quot + 1 };
        }
    }

    return {0, 0};
}
