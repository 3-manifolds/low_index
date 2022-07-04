#ifndef LOW_INDEX_ABSTRACT_SIMS_NODE_H
#define LOW_INDEX_ABSTRACT_SIMS_NODE_H

#include "coveringSubgraph.h"

namespace low_index {

///
/// A class to list covering subgraphs up to conjugacy for a finitely
/// presented group G.
///
/// This class provides methods to check whether the covering subgraph
/// is minimal in its conjugation class (see may_be_minimal for the
/// ordering used) and whether the relators in G lift, that is whether
/// the given words act trivially on the covering subgraph and the action
/// of the free group F on the graph factors through G.
///
/// We can think of a covering subgraph passing the minimality and relators
/// lift checks as:
/// 1. The canonical representative of a conjugacy class of representations
///    G -> S_degree. We obtain the canonical subgroup H for a conjugacy
///    class of subgroups as preimage of all permutations fixing vertex 1.
/// 2. A coset table for subgroup H of G.
/// 3. Think of G as the fundamental group of a Cayley complex, that is
///    take the Cayley complex for the free group F and add a 2-cell for
///    each relator of G. A complete covering graph is a covering space
///    of the Cayley complex.
///
/// Note that we have two methods to check whether a set of relators
/// lifts. The method relators_may_lift makes use of an acceleration
/// structure to repeatedly query the same relators and can be called
/// on an incomplete subgraph. Also note that relators_may_lift can
/// add edges to the graph itself - called a deduction.
/// relators_lift does not use any acceleration
/// structures and can only be called on a complete subgraph.
///
/// We call the relators passed to relators_may_lift the "short relators"
/// and the ones passed to relators_lift the "long relators" - even
/// though it is not a requirement that the "short relators" are
/// actually shorter than the "long relators", but the low index
/// algorithm tends to perform better when the "short relators" are
/// indeed short.
///
/// Similarly to CoveringSubgraph, this is an abstract class
/// with AbstractSimsNode::_lift_indices and AbstractSimsNode::_lift_vertices
/// being managed by a subclass of AbstractSimsNode.
///
class AbstractSimsNode : public CoveringSubgraph
{
public:
    /// Check that given "long" relators lift. Requires that the
    /// subgraph is complete.
    bool relators_lift(const std::vector<Relator> &relators) const;

    /// Check that the given "short" relators lift. Does not
    /// require that the subgraph is complete. The method is using
    /// the acceleration structure. Thus, the vector of relators
    /// given to this class must always be the same and its length
    /// must match the num_relators argument given when the class
    /// was instantiated.
    ///
    /// If the subgraph is complete, the answer is definite. That
    /// is, if the subgraph is complete then the answer is true
    /// if and only if the given relators lift.
    // bool relators_may_lift(const std::vector<Relator> &relators);
    bool relators_may_lift(const std::vector<Relator> &relators,
			   const std::pair<LetterType, DegreeType> slot,
			   const DegreeType target);

    /// We regard two complete covering subgraphs that differ only
    /// by reindexing of the vertices as equivalent. We want to only
    /// list one complete covering subgraph for each such conjugacy
    /// class. There is an order on covering subgraphs.
    /// 
    /// This method returns false if and only if there is a completion
    /// of this covering subgraph that can be minimal in its conjugacy
    /// class with respect to the above order.
    ///
    /// Note that this method only works correctly if the edges were
    /// added in a particular order, namely, by always picking an
    /// edge starting at CoveringSubgraph::first_empty_slot next.
    ///
    /// If the subgraph is complete, the answer is definite. That
    /// is, if the subgraph is complete, then the answer is true
    /// if and only if the given relators lift.
    bool may_be_minimal() const;

    /// How many relators are supported by the acceleration structure.
    /// In other words, the number of "short relators".
    unsigned int num_relators() const { return _num_relators; }

protected:
    AbstractSimsNode(RankType rank,
                     DegreeType max_degree,
                     unsigned int num_relators);

    AbstractSimsNode(const AbstractSimsNode &other);

    // Computes the amount of memory a subclass needs to allocate
    // and the necessary offsets to set _outging, _incoming, ...
    struct _MemoryLayout
    {
        // _MemoryLayout can be called from the constructor
        // of a subclass of AbstractSimsNode after one of
        // AbstractSimsNode::AbstractSimsNode constructors has been
        // called.
        _MemoryLayout(const AbstractSimsNode &node);

        // Offsets for fields.
        static constexpr size_t outgoing_offset = 0;
        size_t incoming_offset;
        size_t lift_indices_offset;
        size_t lift_vertices_offset;

        // Amount of memory needed to store the graph and acceleration
        // structure of an AbstractSimsNode.
        //
        // The size is rounded up for alignment. That is, adding size
        // to an aligned pointer uint8_t* again gives a pointer aligned
        // to be able to store the data of an AbstractSimsNode.
        size_t size;
    };

    // Populate the pointers _outgoing, ... to point into the given
    // memory.
    void _apply_memory_layout(const _MemoryLayout &layout,
                              uint8_t * memory);
    // Call after _apply_memory_layout to initialize the memory to
    // correspond to an empty graph.
    void _initialize_memory();
    // Call after _apply_memory_layout to copy the graph and acceleration
    // structure from another AbstractSimsNode.
    void _copy_memory(const AbstractSimsNode &other);

private:
    // Helper for relators_may_lift checking a single relator.
    bool _relator_may_lift(
        const Relator &relator,
        size_t n,
        DegreeType v);

    // Helper for may_be_minimal. It checks whether moving the given
    // basepoint to vertex 1 would produce a covering subgraph
    // that is smaller than this covering subgraph with respect to
    // the covering subgraph order.
    //
    // More precisely, it returns false if every completion of the
    // covering subgraph where the given basepoint is moved to vertex
    // 1 would be smaller than every completion of this covering
    // subgraph.
    //
    bool _may_be_minimal(DegreeType basepoint) const;

    const unsigned int _num_relators;

protected:
    // Amount of memory needed - see _MemoryLayout::size for details.
    size_t _memory_size;
    // For each relator and vertex, store how far the relator could be lifted
    // before hitting a vertex not having an edge (yet) labeled by the next
    // letter in the relator.
    RelatorLengthType *_lift_indices;
    // For each relator and vertex, store the result of lifting the vertex
    // by the subword of the relator containing _lift_indices letters.
    //
    // Note that the maximum value of DegreeType is reserved to indicate
    // that lifting a vertex by a relator was possible (all necessary edges
    // exist) and ended up at the same vertex.
    DegreeType *_lift_vertices;
};

} // Namespace low_index

#endif
