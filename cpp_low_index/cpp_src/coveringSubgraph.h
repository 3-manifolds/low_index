#ifndef LOW_INDEX_COVERING_SUBGRAPH_H
#define LOW_INDEX_COVERING_SUBGRAPH_H

#include "types.h"

#include <vector>
#include <string>

namespace low_index {

///
/// \class CoveringSubgraph
///
/// Described the following graph:
/// - The vertices are indexed by 1, 2, ..., degree().
///   Note that degree() can be at most max_degree(). In other words,
///   we need to specify the maximal degree when instantiating a
///   CoveringSubgraph.
///   The vertex indices are of the integral type DegreeType.
///   The value 0 and the maximum value are reservered for special
///   purposes, see CoveringSubgraph::act_by and
///   AbstractSimsNode::_relator_may_lift.
/// - The directed edges are labeled by letters 1, 2, ..., rank().
///   Note that there can be at most one directed edge with the
///   same label and same starting vertex or same ending vertex,
///   respectively. If a directed edge labeled by the letter l is
///   flipped, we label it by the inverse letter -l.
/// We call such a graph complete, if there a directed edge for
/// each vertex 1, 2, ..., degree() and each letter 1, 2, ..., rank().
///
/// Let G be a finitely presented group. It might help to think of G
/// as a Cayley complex with 1 vertex, one edge for each generator and
/// one cell for each relator of G. Or think of each generator as a
/// face-pairing of a fundamental polyhedron with the relators coming
/// from edge and other relations.
///
/// Thinking of the letters as generators the G, consider a complete
/// graph such that for each vertex and relator we end up at the same
/// vertex when traversing the (labeled) edges in the order given by
/// the relator.
///
/// Such a complete graph can be interpreted in the following ways:
/// 1. A coset table for a subgroup H of G. Each vertex corresponds
///    to a coset in G/H. To multiply a coset by a letter l, look at the
///    end of an edge labeled by l and starting at the given coset.
/// 2. A representation G -> S_degree. The image of a letter l is
///    the permutation obtained by letting all edges labeled l take
///    the vertices to the vertices.
/// 3. A covering space of the Cayley complex with the graph as 1-skeleton.
/// 4. A covering space of a 3-manifold. Take a copy of the fundamental
///    polyhedron for each vertex. For each edge labeled l, lift the
///    face-pairing so that it pairs a face of the copy corresponding
///    to the start vertex to a face of the copy corresponding to the
///    end vertex.
///
/// Internally, such a (not necessarily complete) graph is stored as two
/// matrices of size degree() * rank() (we allocate memory for
/// max_degree() * rank()) for the outgoing and incoming edges.
/// That is, if there is a directed edge from i to j labeled by l
/// (with l > 0), then the entry at (i, l) in the outgoing
/// matrix is j and the one at (j, l) in the incoming matrix is i.
/// A matrix entry takes the value zero if no edge with the respective
/// label starts or ends, respectively, at the respective vertex.
///
/// This is an abstract class. In particular, it is the responsibility
/// of the subclasses to allocate memory for the graph and then set
/// CoveringSubgraph::_outgoing and CoveringSubgraph::_incoming to point
/// to the allocated memory.
///
class CoveringSubgraph
{
public:
    /// Rank. That is number of (positive) letters used to label
    /// the edges.
    RankType rank() const { return _rank; }
    /// Current number of vertices in the graph.
    DegreeType degree() const { return _degree; }
    /// Maximal number of vertices graph can have.
    DegreeType max_degree() const { return _max_degree; }
    /// Number of directed edges (counting only those labeled by
    /// positive letters).
    unsigned int num_edges() const { return _num_edges; }

    /// Is graph complete in the above sense.
    bool is_complete() const {
        return _num_edges == _rank * _degree;
    }

    /// Add an edge from a vertex to a vertex labeled by letter
    /// which can be negative.
    ///
    /// In order for the degree to grow, one of the given vertices is
    /// allowed to go one step above the current degree (in which case
    /// the degree is increased).
    void add_edge(LetterType letter,
                  DegreeType from_vertex,
                  DegreeType to_vertex);

    /// Add an edge but only if there is no edge ending at the
    /// same vertex with the same letter. Return true on success.
    ///
    /// Note that this is not checking whether there is an edge
    /// already starting at the same vertex with the same letter.
    bool verified_add_edge(LetterType letter,
                           DegreeType from_vertex,
                           DegreeType to_vertex);

    /// If there is an edge starting at the given vertex labeled
    /// by the given letter, return its end vertex. Otherwise, return
    /// 0.
    DegreeType act_by(LetterType letter, DegreeType vertex) const;

    /// Find the first (in a particular order that is important for
    /// AbstractSimsNode::may_be_minimal) pair of (letter, vertex)
    /// (letter might be negative) such that there is no edge from
    /// that vertex labeled by that letter.
    std::pair<LetterType, DegreeType> first_empty_slot() const;

    /// Give the representation into the symmetric group S_degree.
    /// That is, for each letter, give the corresponding permutation
    /// on the numbers 1, ..., degree().
    std::vector<std::vector<DegreeType>> permutation_rep() const;

    /// String representation - particularly useful for debugging.
    std::string to_string() const;

protected:
    // Initializes a graph with no edges.
    //
    // Note that it is left to the c'tor of a subclass to allocate
    // memory for the graph and assign _incoming and _outgoing.
    CoveringSubgraph(
        RankType rank,
        DegreeType max_degree);

    // Copies fields such as rank from another graph.
    //
    // Same note as for the other c'tor applies.
    CoveringSubgraph(
        const CoveringSubgraph &other);

    // Where the memory for the graph starts. Use in a subclass
    // that is copying a graph by copying the associated memory.
    uint8_t * _memory_start() const {
        return _outgoing;
    }
    
private:
    // Follow rule-of-three/rule-of-five: either implement or delete
    // assignment operator.
    CoveringSubgraph& operator=(const CoveringSubgraph& other) = delete;

    // Implements add_edge and verify_add_edge
    template<bool check_incoming, bool check_outgoing>
    inline
    bool _add_edge(LetterType label, DegreeType from_vertex, DegreeType to_vertex);

    const RankType _rank;
    const DegreeType _max_degree;

protected:
    // To be set by subclass
    DegreeType *_outgoing;
    DegreeType *_incoming;

private:
    DegreeType _degree;
    unsigned int _num_edges;

    // A cache storing the last value of first_empty_slot.
    mutable int _slot_index;
};

} // Namespace low_index

#endif
