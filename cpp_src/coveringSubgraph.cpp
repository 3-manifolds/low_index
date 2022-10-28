#include "coveringSubgraph.h"

#include <stdexcept>
#include <cstdlib>

namespace low_index {

CoveringSubgraph::CoveringSubgraph(
        const RankType rank,
        const DegreeType max_degree)
  : _rank(rank)
  , _max_degree(max_degree)
  // _outgoing and _incoming set by subclass
  , _degree(1)
  , _num_edges(0)
  , _slot_index(0)
{
}

CoveringSubgraph::CoveringSubgraph(const CoveringSubgraph &other)
  : _rank(other._rank)
  , _max_degree(other._max_degree)
  // _outgoing and _incoming set by subclass
  , _degree(other._degree)
  , _num_edges(other._num_edges)
  , _slot_index(other._slot_index)
{
}

std::string
CoveringSubgraph::to_string() const
{
    std::string padding;
    for (unsigned int i = 0; i < _num_edges; i++) {
        padding += "    ";
    }
    
    std::string result = padding;
    if (is_complete()) {
        result += "Covering";
    } else {
        result += "Partial covering";
    }
    result += " of degree " + std::to_string(_degree);
    result += " with " + std::to_string(_num_edges) + " edges";

    for (DegreeType v = 0; v < _degree; v++) {
        for (RankType n = 0; n < _rank; n++) {
            const size_t j = v * _rank + n;
            const DegreeType t = _outgoing[j];
            const DegreeType s = _incoming[j];

            if (t != 0 || s != 0) {
                result += "\n" + padding;
                if (t != 0) {
                    result +=
                        std::to_string(v+1) + "--( " +
                        std::to_string(n+1) + ")->" +
                        std::to_string(static_cast<int>(t));
                } else {
                    result +=
                        "         ";
                }
                result += "      ";
                if (s != 0) {
                    result +=
                        std::to_string(v+1) + "--(" +
                        std::to_string(-static_cast<int>(n+1)) + ")->" +
                        std::to_string(static_cast<int>(s));
                }
            }
        }
    }

    return result;
}

std::vector<std::vector<DegreeType>>
CoveringSubgraph::permutation_rep() const
{
    if (!is_complete()) {
        throw std::domain_error("The graph is not a covering.");
    }

    std::vector<std::vector<DegreeType>> result;
    result.reserve(_rank);
    
    for (RankType l = 0; l < _rank; l++) {
        std::vector<DegreeType> r;
        r.reserve(_degree);
        for (DegreeType v = 0; v < _degree; v++) {
            r.push_back(_outgoing[v * _rank + l] - 1);
        }
        result.push_back(std::move(r));
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
        _add_edge<false, false>(-letter, to_vertex,   from_vertex);
    } else {
        _add_edge<false, false>( letter, from_vertex, to_vertex);
    }
}

bool
CoveringSubgraph::verified_add_edge(
    const LetterType letter,
    const DegreeType from_vertex,
    const DegreeType to_vertex)
{
    if (letter < 0) {
        return _add_edge<false, true>(-letter, to_vertex,   from_vertex);
    } else {
        return _add_edge<true, false>( letter, from_vertex, to_vertex);
    }
}

template<bool check_incoming, bool check_outgoing>
bool
CoveringSubgraph::_add_edge(
    const LetterType label,
    const DegreeType from_vertex,
    const DegreeType to_vertex)
{
    const unsigned int out_index = (from_vertex - 1) * _rank + (label - 1);
    const unsigned int in_index  = (to_vertex   - 1) * _rank + (label - 1);
    if (check_incoming) {
        if (_incoming[in_index] != 0) {
            return false;
        }
    }
    if (check_outgoing) {
        if (_outgoing[out_index] != 0) {
            return false;
        }
    }
    
    _outgoing[out_index] = to_vertex;
    _incoming[in_index]  = from_vertex;
    _num_edges++;
    if (from_vertex > _degree || to_vertex > _degree) {
        _degree++;
    }
    return true;
}

DegreeType
CoveringSubgraph::act_by(const LetterType letter, const DegreeType vertex) const
{
    if (letter > 0) {
        return _outgoing[(vertex - 1) * _rank + letter - 1];
    } else {
        return _incoming[(vertex - 1) * _rank - letter - 1];
    }
}

std::pair<LetterType, DegreeType>
CoveringSubgraph::first_empty_slot() const
{
    const unsigned int max_edges = _rank * _degree;

    if (max_edges == _num_edges) {
        return { 0, 0 };
    }

    for(unsigned int n = _slot_index; n < max_edges; n++) {
        if (_outgoing[n] == 0) {
            const std::div_t qr = std::div(
                static_cast<int>(n), static_cast<int>(_rank));
            _slot_index = n;
            return { qr.rem + 1, qr.quot + 1 };
        }
        if (_incoming[n] == 0) {
            const std::div_t qr = std::div(
                static_cast<int>(n), static_cast<int>(_rank));
            _slot_index = n;
            return { -(qr.rem + 1), qr.quot + 1 };
        }
    }

    return {0, 0};
}

} // Namespace low_index
