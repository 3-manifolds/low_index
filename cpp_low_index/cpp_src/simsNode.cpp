#include "simsNode.h"

namespace low_index {

SimsNode::SimsNode(
        const RankType rank,
        const DegreeType max_degree,
        const unsigned int num_relators)
 : CoveringSubgraph(rank, max_degree)
 , _num_relators(num_relators)
{
}

SimsNode::SimsNode(
    const SimsNode &other)
 : CoveringSubgraph(other)
 , _num_relators(other._num_relators)
{
}

template<typename T>
static
size_t
_align(const size_t n)
{
    return ((n + alignof(T) - 1 ) / alignof(T)) * alignof(T);
}

SimsNode::_MemoryLayout::_MemoryLayout(
    const SimsNode &node)
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
SimsNode::_apply_memory_layout(
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
SimsNode::_initialize_memory()
{
    memset(_memory_start(), 0, _memory_size);
    for (size_t n = 0; n < _num_relators; n++) {
        for (DegreeType v = 0; v < max_degree(); v++) {
            const size_t j = n * max_degree() + v;
            _lift_vertices[j] = v + 1;
        }
    }
}

void
SimsNode::_copy_memory(const SimsNode &other)
{
    memcpy(_memory_start(), other._memory_start(), _memory_size);
}

bool
SimsNode::relators_may_lift(const std::vector<Relator> &relators)
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
SimsNode::_relator_may_lift(
    const Relator &relator,
    const size_t n,
    const DegreeType v)
{
    const size_t j = n * max_degree() + v;

    constexpr DegreeType finished =
        std::numeric_limits<DegreeType>::max();
    
    DegreeType vertex = _lift_vertices[j];
    if (vertex == finished) {
        return true;
    }
    DegreeType next_vertex;
    for (RelatorLengthType i = _lift_indices[j]; true; i++) {
        next_vertex = act_by(relator[i], vertex);
        if (i == relator.size() - 1) {
            break;
        }
        if (next_vertex == 0) {
            _lift_vertices[j] = vertex;
            _lift_indices[j] = i;
            return true;
        }
        vertex = next_vertex;
    }

    if (next_vertex == v + 1) {
        _lift_vertices[j] = finished;
        return true;
    }

    if (next_vertex == 0) {
        if (verified_add_edge(relator.back(), vertex, v + 1)) {
            _lift_vertices[j] = finished;
            return true;
        }
    }

    return false;
}

bool
SimsNode::relators_lift(const std::vector<Relator> &relators) const
{
    for (const Relator &relator : relators) {
        for (DegreeType v = 1; v <= degree(); v++) {
            DegreeType vertex = v;
            for (const int letter : relator) {
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
SimsNode::may_be_minimal() const
{
    for (DegreeType basepoint = 2; basepoint <= degree(); basepoint++) {
        if (!_may_be_minimal(basepoint)) {
            return false;
        }
    }
    return true;
}

bool
SimsNode::_may_be_minimal(const DegreeType basepoint) const
{
    DegreeType std_to_alt[degree()+1];
    memset(std_to_alt, 0, sizeof(std_to_alt));

    DegreeType alt_to_std[degree()+1];
    memset(alt_to_std, 0, sizeof(alt_to_std));

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
