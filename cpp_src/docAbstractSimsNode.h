/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_low_index_AbstractSimsNode =
R"doc(A covering subgraph with additional methods to check whether the given
relators lift and whether the covering subgraph is minimal in the
conjugation class with respect to a certain ordering.

We say that a relator lifts to a covering subgraph if starting with
any vertex and traversing the edges in the order such that their
labels spell out the relator word.

Note that we have two methods to check whether a set of relators
lifts. The method relators_may_lift makes use of an acceleration
structure to repeatedly query the same relators and can be called on
an incomplete subgraph. Also note that relators_may_lift can add edges
to the graph itself - called a deducation. relators_lift does not use
any acceleration structure and can only be called on a complete
subgraph.

We call the relators passed to relators_may_lift the "short relators"
and the ones passed to relators_lift the "long relators" - even though
it is not a requirement that the "short relators" are actually shorter
than the "long relators", but the low index algorithm tends to perform
better when the "short relators" are indeed short.

Similarly to CoveringSubgraph, this is an abstract class with
AbstractSimsNode::_lift_indices and AbstractSimsNode::_lift_vertices
being managed by a subclass of AbstractSimsNode.)doc";

static const char *__doc_low_index_AbstractSimsNode_AbstractSimsNode = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_AbstractSimsNode_2 = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_MemoryLayout = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_MemoryLayout_MemoryLayout = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_MemoryLayout_incoming_offset = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_MemoryLayout_lift_indices_offset = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_MemoryLayout_lift_vertices_offset = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_MemoryLayout_size = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_apply_memory_layout = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_copy_memory = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_initialize_memory = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_lift_indices = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_lift_vertices = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_may_be_minimal =
R"doc(We regard two complete covering subgraphs that differ only by
reindexing of the vertices as equivalent. We want to only list one
complete covering subgraph for each such conjugacy class. There is an
order on covering subgraphs.

This method returns false if and only if no completion of this
covering subgraph to a complete graph can be minimal in its conjugacy
class with respect to the above order.

Note that this method only works correctly if the edges were added in
a particular order, namely, by always picking an edge starting at
CoveringSubgraph::first_empty_slot next.

If the subgraph is complete, the answer is definite. That is, if the
subgraph is complete, then the answer is true if and only if the given
relators lift.)doc";

static const char *__doc_low_index_AbstractSimsNode_may_be_minimal_2 = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_memory_size = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_num_relators =
R"doc(How many relators are supported by the acceleration structure. In
other words, the number of "short relators".)doc";

static const char *__doc_low_index_AbstractSimsNode_num_relators_2 = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_relator_may_lift = R"doc()doc";

static const char *__doc_low_index_AbstractSimsNode_relators_lift =
R"doc(Check that given "long" relators lift. Requires that the subgraph is
complete.)doc";

static const char *__doc_low_index_AbstractSimsNode_relators_may_lift =
R"doc(Check that the given "short" relators lift. Does not require that the
subgraph is complete. The method is using the acceleration structure.
Thus, the vector of relators given to this class must always be the
same and its length must match the num_relators argument given when
the class was instantiated.

If the subgraph is complete, the answer is definite. That is, if the
subgraph is complete then the answer is true if and only if the given
relators lift.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

