#ifndef _COVERING_SUBGRAPH_H_
#define _COVERING_SUBGRAPH_H_

class CoveringSubgraph
{
public:
    CoveringSubgraph(int rank);

    int GetRank() const { return _rank; }
    
private:
    const int _rank;
};

#endif
