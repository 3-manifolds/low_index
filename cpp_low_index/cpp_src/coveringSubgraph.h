#ifndef _COVERING_SUBGRAPH_H_
#define _COVERING_SUBGRAPH_H_

#include <vector>
#include <string>
#include <cstdint>
#include <memory>

class CoveringSubgraph
{
public:
    // Index for vertices and degree of cover.
    using DegreeType = uint8_t;
    // Rank of the group.
    using RankType = uint16_t;
    // Letters in the group, used to label edges.
    using LetterType = std::make_signed<RankType>::type;

protected:
    CoveringSubgraph(
        RankType rank,
        DegreeType max_degree);

    CoveringSubgraph(
        const CoveringSubgraph &other);

public:
    const RankType rank;
    DegreeType degree;
    const DegreeType max_degree;
    unsigned int num_edges;

public:
    DegreeType *outgoing;
    DegreeType *incoming;

    int GetRank() const { return rank; }
    
    bool is_complete() const {
        return num_edges == rank * degree;
    }

    void add_edge(LetterType letter, DegreeType from_vertex, DegreeType to_vertex);
    bool verified_add_edge(LetterType letter, DegreeType from_vertex, DegreeType to_vertex);

    std::string to_string() const;

    std::vector<std::vector<int>> permutation_rep() const;

    DegreeType act_by(LetterType letter, DegreeType vertex) const;

    std::pair<LetterType, DegreeType> first_empty_slot() const;

protected:
    mutable int _slot_index;

    
    template<bool check>
    bool _add_edge(LetterType label, DegreeType from_vertex, DegreeType to_vertex);
};

#endif
