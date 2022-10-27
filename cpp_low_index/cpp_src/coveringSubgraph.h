#ifndef _COVERING_SUBGRAPH_H_
#define _COVERING_SUBGRAPH_H_

#include <vector>
#include <string>

class CoveringSubgraph
{
public:
    using IntType = unsigned char;

    CoveringSubgraph(int rank,
                     int max_degree,
                     int num_relators = 0);

    const int rank;
    int degree;
    const int max_degree;
    int num_edges;
    const int num_relators;
    std::vector<IntType> outgoing;
    std::vector<IntType> incoming;

    int GetRank() const { return rank; }
    
    bool is_complete() const {
        return num_edges == rank * degree;
    }

    void add_edge(int letter, int from_vertex, int to_vertex);
    bool verified_add_edge(int letter, int from_vertex, int to_vertex);

    std::string to_string() const;

    std::vector<std::vector<int>> permutation_rep() const;
    
protected:
    int _slot_index;

private:
    template<bool check>
    bool _add_edge(int label, int from_vertex, int to_vertex);
};

#endif
