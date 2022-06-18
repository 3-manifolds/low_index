#ifndef _COVERING_SUBGRAPH_H_
#define _COVERING_SUBGRAPH_H_

#include <vector>
#include <string>
#include <cstdint>

class CoveringSubgraph
{
public:
    // Index for vertices and degree of cover.
    using DegreeType = uint8_t;
    // Rank of the group.
    using RankType = uint16_t;
    // Letters in the group, used to label edges.
    using LetterType = std::make_signed<RankType>::type;

    RankType rank() const { return _rank; }
    DegreeType degree() const { return _degree; }
    DegreeType max_degree() const { return _max_degree; }
    unsigned int num_edges() const { return _num_edges; }

    bool is_complete() const {
        return _num_edges == _rank * _degree;
    }

    void add_edge(LetterType letter,
                  DegreeType from_vertex,
                  DegreeType to_vertex);
    bool verified_add_edge(LetterType letter,
                           DegreeType from_vertex,
                           DegreeType to_vertex);

    std::string to_string() const;

    std::vector<std::vector<int>> permutation_rep() const;

    DegreeType act_by(LetterType letter, DegreeType vertex) const;

    std::pair<LetterType, DegreeType> first_empty_slot() const;

protected:
    CoveringSubgraph(
        RankType rank,
        DegreeType max_degree);

    CoveringSubgraph(
        const CoveringSubgraph &other);

    uint8_t * _memory_start() const {
        return _outgoing;
    }
    
private:
    CoveringSubgraph& operator=(const CoveringSubgraph& other) = delete;

    template<bool check_incoming, bool check_outgoing>
    inline
    bool _add_edge(LetterType label, DegreeType from_vertex, DegreeType to_vertex);

    const RankType _rank;
    const DegreeType _max_degree;

protected:
    DegreeType *_outgoing;
    DegreeType *_incoming;

private:
    DegreeType _degree;
    unsigned int _num_edges;

    mutable int _slot_index;
};

#endif
