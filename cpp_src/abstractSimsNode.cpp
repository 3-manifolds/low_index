#include "abstractSimsNode.h"

#include <limits>
#include <stdexcept>
#include <cstring>
#include <iostream>

namespace low_index {

AbstractSimsNode::AbstractSimsNode(
        const RankType rank,
        const DegreeType max_degree,
        const unsigned int num_relators)
 : CoveringSubgraph(rank, max_degree)
 , _num_relators(num_relators)
{
}

AbstractSimsNode::AbstractSimsNode(
    const AbstractSimsNode &other)
 : CoveringSubgraph(other)
 , _num_relators(other._num_relators)
{
}

// Make sure n is a multiple of the alignment of T.
template<typename T>
static
size_t
_align(const size_t n)
{
    return ((n + alignof(T) - 1 ) / alignof(T)) * alignof(T);
}

AbstractSimsNode::_MemoryLayout::_MemoryLayout(
    const AbstractSimsNode &node)
{
    // DegreeType *outgoing;
    size_t t = outgoing_offset;
    t += node.rank() * node.max_degree() * sizeof(DegreeType);

    // DegreeType *incoming;
    t = _align<DegreeType>(t);
    incoming_offset = t;
    t += node.rank() * node.max_degree() * sizeof(DegreeType);

    // RelatorLengthType *lift_indices;
    t = _align<RelatorLengthType>(t);
    lift_indices_offset = t;
    t += node.num_relators() * node.max_degree() * sizeof(RelatorLengthType);

    // DegreeType *lift_vertices;
    t = _align<DegreeType>(t);
    lift_vertices_offset = t;
    t += node.num_relators() * node.max_degree() * sizeof(DegreeType);

    size = _align<uint64_t>(t);
}

void
AbstractSimsNode::_apply_memory_layout(
    const _MemoryLayout &layout,
    uint8_t * const memory)
{
    _outgoing =
        reinterpret_cast<DegreeType*>(
            memory + layout.outgoing_offset);
    _incoming =
        reinterpret_cast<DegreeType*>(
            memory + layout.incoming_offset);
    _lift_indices =
        reinterpret_cast<RelatorLengthType*>(
            memory + layout.lift_indices_offset);
    _lift_vertices =
        reinterpret_cast<DegreeType*>(
            memory + layout.lift_vertices_offset);
    _memory_size = layout.size;
}

void
AbstractSimsNode::_initialize_memory()
{
    std::memset(_memory_start(), 0, _memory_size);

    // The lift of vertex v + 1 by the empty word is just v + 1.
    // Use this to initialize _lift_vertices.
    for (size_t n = 0; n < _num_relators; n++) {
        for (DegreeType v = 0; v < max_degree(); v++) {
            const size_t j = n * max_degree() + v;
            _lift_vertices[j] = v + 1;
        }
    }
}

void
AbstractSimsNode::_copy_memory(const AbstractSimsNode &other)
{
    std::memcpy(_memory_start(), other._memory_start(), _memory_size);
}

bool
AbstractSimsNode::relators_may_lift(const std::vector<Relator> &relators,
				    const std::pair<LetterType, DegreeType> slot,
				    const DegreeType target)
{
    for (size_t n = 0; n < relators.size(); n++) {
        for (DegreeType v = 0; v < degree(); v++) {
	    DegreeType endVertex = _lift_vertices[n*max_degree() + v];
	    /// If this is being called immediately after adding an edge then the
	    /// lift state of a relator cannot have changed unless the lift of its
	    /// longest liftable initial segment is an endpoint of the new edge.
	    if (target != 0 && endVertex != slot.second && endVertex != target) {
	        continue;
	    }
	    if (!_relator_may_lift(relators[n], n, v)) {
                return false;
	    }
        }
    }
    return true;
}

bool
AbstractSimsNode::_relator_may_lift(
    const Relator &relator,
    const size_t n,
    const DegreeType v)
{
    const size_t j = n * max_degree() + v;

    constexpr DegreeType finished =
        std::numeric_limits<DegreeType>::max();

    DegreeType vertex = _lift_vertices[j];
    // We already determined in an earlier run of _relator_may_lift
    // that this relator lifts.
    if (vertex == finished) {
        return true;
    }

    // Continue lifting the relator where we left of.
    DegreeType next_vertex;
    for (RelatorLengthType i = _lift_indices[j]; true; i++) {
        // Result of lifting the edge given by the next letter in
        // the relator.
        next_vertex = act_by(relator[i], vertex);
        if (i == relator.size() - 1) {
            // We are at the last letter of the relator.
            // This case is handled specially below.
            break;
        }
        if (next_vertex == 0) {
            // The is no edge yet corresponding to the next letter by which
            // we lift the vertex. Store how far we were able to lift the
            // relator for the next call to _relator_may_lift.
            _lift_vertices[j] = vertex;
            _lift_indices[j] = i;
            return true;
        }
        // Move on to the next vertex before looking at the next
        // letter in the relator.
        vertex = next_vertex;
    }

    // We are at the last letter in the relator.

    if (next_vertex == v + 1) {
        // We were able to lift the relator to a loop.  Record this fact and
        // return true.
        _lift_vertices[j] = finished;
        return true;
    }

    if (next_vertex == 0) {
        // The relator almost lifted completely, but the last edge that should
        // appear in the lift was missing from the graph.  The only way that the
        // relator could lift to a loop would be if the missing edge joined
        // the last vertex of the lift to the initial vertex v+1. We attempt
        // to add an edge using the empty slot at the end of the lift and the
        // appropriate slot at the initial vertex v + 1.
        //
        // Note that it is only possible to add such an edge if both of those
        // slots are empty.  We know that the slot at the end of the lift is
        // empty, but the slot at the initial vertex might have already been
        // filled by an edge that was added earlier.  So we must call
        // verified_add_edge here to avoid corrupting the structure of the
        // CoveringSubgraph.
        if (verified_add_edge(relator.back(), vertex, v + 1)) {
            // Record that the relator lifts to a loop and return true.
            _lift_vertices[j] = finished;
            return true;
        }
    }

    // In all other cases return false to signal that the relator does not lift
    // to a loop.
    return false;
}

bool
AbstractSimsNode::relators_lift(const std::vector<Relator> &relators) const
{
    for (const Relator &relator : relators) {
        for (DegreeType v = 1; v <= degree(); v++) {
            // Start with vertex v.
            DegreeType vertex = v;
            for (const int letter : relator) {
                // Traverse the edges labeled by the letters in the relator.
                vertex = act_by(letter, vertex);
                if (vertex == 0) {
                    throw std::domain_error(
                        "relators_lift: The graph is not a covering.");
                }
            }
            if (vertex != v) {
                return false;
            }
        }
    }

    return true;
}

bool
AbstractSimsNode::may_be_minimal() const
{
    // Return False if the subgraph can provably not be extended to a cover
    // which is minimal in its conjugacy class, True otherwise.
    //
    // The ordering of based coverings is given as follows.  Given a covering
    // with rank R and degree D, consider all pairs (N, L) where N is a vertex
    // index in [1, D] and L is a signed label in [-R, -1] or [1, R].  Order
    // the signed labels as 1, -1, 2, -2, ..., R, -R.  Use that to lex order
    // the 2*D*R pairs (N, L).  For each pair (N, L) assign an edge E(N, L) and
    // a number s(N, L) as follows:
    //
    //     * if L > 0, E(N, L) is the edge with initial vertex N and
    //       label L and s(N, L) is index of its terminal vertex.
    //
    //     * if L < 0, E(N, L) is the edge with terminal vertex N and
    //       label -L and s(N, L) is the index of its initial vertex.
    //
    // The list [s(N, L) for all (N, L)] is the complexity of the covering;
    // complexities are ordered lexicographically.
    //
    // Any conjugate of the subgroup corresponding to a covering graph can be
    // obtained by changing the basepoint.  A choice of basepoint determines an
    // indexing of the vertices where the index of a vertex v is the next
    // available index at the moment when the first E(N, L) having v as its
    // terminal (initial) vertex if L > 0 (L < 0) is added.  We only enumerate
    // based coverings for which the basepoint is chosen to minimize
    // complexity.  The subgroups corresponding to these coverings will then by
    // unique up to conjugacy.
    //
    // Even when working with a proper subgraph of a covering graph (containing
    // the basepoint) it may be possible to deduce that the subgraph cannot be
    // extended to a minimal covering.  In such a case the subgraph should not
    // be added to the Sims tree.  If this test is done at each stage of the
    // construction of the tree, the complete graphs in the final tree will
    // automatically be minimal.
    //
    // This method iterates through all vertices of the subgraph, constructing
    // as much as possible of the vertex ordering determined by taking that
    // vertex as a basepoint.  It returns False as soon as it encounters an
    // edge which would result in a higher complexity. If no such edge is found
    // for any choice of basepoint it returns True.

    for (DegreeType basepoint = 2; basepoint <= degree(); basepoint++) {
        if (!_may_be_minimal(basepoint)) {
            return false;
        }
    }
    return true;
}

bool
AbstractSimsNode::_may_be_minimal(const DegreeType basepoint) const
{
    // Unfortunately, MS Visual Studio does not support
    // C99-style Variable Length Arrays, so we allocate a fixed length array.
    constexpr size_t m = std::numeric_limits<DegreeType>::max() + 1;

    static_assert(m < 1000,
                  "Allocating too large array on the stack.");
    
    // We are working with the standard indexing (determined by putting
    // the basepoint at vertex 1) and an alternate indexing determined by
    // a different basepoint.  We construct mappings between the two
    // indexings and store them in the arrays std_to_alt and
    // alt_to_std. (For convenience when dealing with 1-based indices,
    // just ignore the 0 entry).
    DegreeType std_to_alt[m];
    std::memset(std_to_alt, 0, sizeof(DegreeType) * (degree() + 1));

    DegreeType alt_to_std[m];
    // It is not necessary to clear alt_to_std
    std::memset(alt_to_std, 0, sizeof(DegreeType) * (degree() + 1));

    // Initial state.
    std_to_alt[basepoint] = 1;
    alt_to_std[1] = basepoint;

    DegreeType max_index = 1;

    // Iterate over all possible slots
    for (DegreeType slot_vertex = 1; slot_vertex <= degree(); slot_vertex++) {
        for (RankType l = 0; l < rank(); l++) {
            for (const DegreeType * const edges : { _outgoing, _incoming }) {
                // Check that the slot is filled with repect to both indexings.
                const DegreeType a = edges[
                    (slot_vertex - 1) * rank() + l];
                const DegreeType b = edges[
                    (alt_to_std[slot_vertex] - 1) * rank() + l];
                if (a == 0 || b == 0) {
                    // The slot was empty in one indexing, so we cannot decide.
                    return true;
                }
                // Update the mappings.
                DegreeType &c = std_to_alt[b];
                if (c == 0) {
                    // This edge is the first, with respect to the alternate
                    // indexing, that is incident to the vertex with standard
                    // index b.  We now know its alternate index.
                    max_index++;
                    c = max_index;
                    alt_to_std[max_index] = b;
                }
                // Compare the old and new indices of the other end of the edge.
                if (c < a) {
                    // The new basepoint is better - discard this graph.
                    return false;
                }
                if (c > a) {
                    // The old basepoint is better.
                    return true;
                }
            }
        }
    }

    return true;
}

} // Namespace low_index
