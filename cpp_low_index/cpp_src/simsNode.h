#ifndef LOW_INDEX_SIMS_NODE_H
#define LOW_INDEX_SIMS_NODE_H

#include "coveringSubgraph.h"

namespace low_index {

class SimsNode : public CoveringSubgraph
{
public:

    // Name new_relators_lift to make clear that relators_may_lift always
    // has to be called with the same relators?
    bool relators_lift(const std::vector<Relator> &relators) const;
    bool relators_may_lift(const std::vector<Relator> &relators);
    bool may_be_minimal() const;

    unsigned int num_relators() const { return _num_relators; }

protected:
    SimsNode(RankType rank,
             DegreeType max_degree,
             unsigned int num_relators);

    SimsNode(const SimsNode &other);

    struct _MemoryLayout
    {
        _MemoryLayout(const SimsNode &node);

        static constexpr size_t outgoing_offset = 0;
        size_t incoming_offset;
        size_t lift_indices_offset;
        size_t lift_vertices_offset;
        size_t size;
    };

    void _apply_memory_layout(const _MemoryLayout &layout,
                              uint8_t * memory);
    void _initialize_memory();
    void _copy_memory(const SimsNode &other);

private:
    bool _relator_may_lift(
        const Relator &relator,
        size_t n,
        DegreeType v);

    bool _may_be_minimal(DegreeType basepoint) const;

    const unsigned int _num_relators;

public:
    size_t _memory_size;
    RelatorLengthType *_lift_indices;
    DegreeType *_lift_vertices;
};

} // Namespace low_index

#endif
