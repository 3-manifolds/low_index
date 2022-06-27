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


static const char *__doc_low_index_SimsTreeMultiThreaded =
R"doc(A class to recursively add edges to a SimsNode to find all covering
subgraphs up to a certain degree such that the given relators lift.

In other words, a class to find all low-index subgroup H of a finitely
presented group G with rank generators and short_relators and
long_relators as relations. The maximal index H in G is max_degree.

As explained in AbstractSimsNode, the short_relators are checked while
recursively adding edges and the long_relators only once the covering
subgraph is complete.

The class only stores the root of the tree that is to be visited
recursively.)doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_Node = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_Node_Node = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_Node_children = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_Node_complete_nodes = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_Node_root = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_SimsTreeMultiThreaded =
R"doc(Construct SimsTreeMultiThreaded with an empty root. That is a SimsNode
for the given rank and max_degree and no edges.)doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_StackedSimsNode = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_list =
R"doc(Find all complete covering subgraphs for which all relators lift. The
parameters do not affect the result, but do affect the performance.)doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_merge_vectors = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_mutex = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_node_index = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_nodes = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_num_working_threads = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_recurse = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_recursion_stop_requested = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_thread_num = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_thread_worker = R"doc()doc";

static const char *__doc_low_index_SimsTreeMultiThreaded_wake_up_threads = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

