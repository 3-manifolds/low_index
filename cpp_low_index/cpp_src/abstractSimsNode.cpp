#include "abstractSimsNode.h"

#include <limits>
#include <stdexcept>
#include <cstring>

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
AbstractSimsNode::relators_may_lift(const std::vector<Relator> &relators)
{
    for (size_t n = 0; n < relators.size(); n++) {
        for (DegreeType v = 0; v < degree(); v++) {
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
        // Result of lifting the vertex by the next letter in
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
        // We were able to lift the entire relator and arrived back
        // at the vetex we started - mark vertex and relator as
        // checked.
        _lift_vertices[j] = finished;
        return true;
    }

    if (next_vertex == 0) {
        // There is no edge yet from the current vertex labeled by the
        // last letter. We know that this edge has to end at the
        // original vertex v + 1 for the relator to lift, so try to add
        // that edge.
        //
        // Note that next_vertex == 0 implies that there is no
        // edge labeled by the last letter starting from the current vertex.
        // However, there might be already an edge with this letter ending
        // at vertex v + 1. That would be a problem, so call verified
        // edge.
        if (verified_add_edge(relator.back(), vertex, v + 1)) {
            // Mark vertex and relator as checked.
            _lift_vertices[j] = finished;
            return true;
        }
    }

    // All other cases mean the relator does not lift.
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
    DegreeType std_to_alt[degree()+1];
    std::memset(std_to_alt, 0, sizeof(std_to_alt));

    DegreeType alt_to_std[degree()+1];
    std::memset(alt_to_std, 0, sizeof(alt_to_std));

    std_to_alt[basepoint] = 1;
    alt_to_std[1] = basepoint;

    DegreeType max_index = 1;

    for (DegreeType slot_vertex = 1; slot_vertex <= degree(); slot_vertex++) {
        for (RankType l = 0; l < rank(); l++) {
            for (const DegreeType * const edges : { _outgoing, _incoming }) {
                const DegreeType a = edges[
                    (slot_vertex - 1) * rank() + l];
                const DegreeType b = edges[
                    (alt_to_std[slot_vertex] - 1) * rank() + l];
                if (a == 0 or b == 0) {
                    return true;
                }
                DegreeType &c = std_to_alt[b];
                if (c == 0) {
                    max_index++;
                    c = max_index;
                    alt_to_std[max_index] = b;
                }
                if (c < a) {
                    return false;
                }
                if (c > a) {
                    return true;
                }
            }
        }
    }

    return true;
}

} // Namespace low_index
