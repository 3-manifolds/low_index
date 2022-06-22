#ifndef LOW_INDEX_STACKED_SIMS_TREE_H
#define LOW_INDEX_STACKED_SIMS_TREE_H

#include "abstractSimsNode.h"

#include <memory>

namespace low_index {

/// A non-abstract SimsNode optimized to avoid any heap allocations
/// on copy.
///
/// An initial StackedSimsNode can be constructed through the SimsNodeStack
/// and then nested copies can be created and destroyed (by going out of
/// scope):
///
///         SimsNodeStack stack(mySimsNode);
///         StackedSimsNode n0(stack.GetNode()); // copy initial SimsNode.
///         // Add edges to n0
///         StackedSimsNode n1(n0);
///         // Add edges to n1
///         StackedSimsNode n2(n1);
///         ...
///
/// Because of the optimization, there are certain restrictions about how these
/// nodes are allowed to be instantiated:
///  1. There can be at most rank * max_degree (i.e., max number of edges)
///     nested copies.
///  2. The nested copies and SimsNodeStack need to be destroyed in the
///     opposite order they were created (enforced by compiler).
///  3. You cannot make more than one (non-nested) copy of a StackedSimsNode at
///     any time:
///         StackedSimsNode n1(n0);
///         StackedSimsNode n2(n0); // Bad, n2 aliases n1
///     Correct:
///         {
///             StackedSimsNode n1(n0);
///             ...
///         }
///         {
///             StackedSimsNode n1(n0);
///             ...
///         }
///  4. You cannot "new StackedSimsNode(...)" or move a StackedSimsNode
///     (enforced by compiler). Thus, you cannot put a StackedSimsNode into
///     an STL container. Create a SimsNode from a StackedSimsNode to store
///     in an STL container.
///
class StackedSimsNode : public AbstractSimsNode
{
public:
    /// Make a copy of this StackedSimsNode.
    StackedSimsNode(const StackedSimsNode &other);

private:
    void * operator new(size_t size) = delete;

    friend class SimsNodeStack;
    // Create StackedSimsNode storing the SimsNode data in the given
    // memory.
    // SimsNodeStack calls this function with a pointer
    // to the first "stack frame".
    StackedSimsNode(const AbstractSimsNode &other, uint8_t * memory);
};

/// An RAII class allocating memory to store the StackedSimsNode's data
/// upfront.
class SimsNodeStack
{
public:
    /// Create from an AbstractSimsNode.
    SimsNodeStack(const AbstractSimsNode &node);

    /// A StackedSimsNode copy of the node used during construction.
    const StackedSimsNode &get_node() const {
        return _node;
    };

private:
    void * operator new(size_t size) = delete;

    // Compute the memory needed to store data for enough StackedSimsNode's
    static size_t _compute_memory_size(const AbstractSimsNode &node);

    // The memory used to store the data for the StackedSimsNode's
    std::unique_ptr<uint8_t[]> _memory;
    // The initial StackedSimsNode.
    StackedSimsNode _node;
};

} // Namespace low_index

#endif
