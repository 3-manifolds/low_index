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


static const char *__doc_low_index_SimsTree =
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

static const char *__doc_low_index_SimsTree_SimsTree =
R"doc(Construct SimsTree with an empty root. That is a SimsNode for the
given rank and max_degree and no edges.)doc";

static const char *__doc_low_index_SimsTree_SimsTree_2 = R"doc()doc";

static const char *__doc_low_index_SimsTree_bloom = R"doc()doc";

static const char *__doc_low_index_SimsTree_list =
R"doc(Find all complete covering subgraphs for which all relators lift. The
parameters do not affect the result, but do affect the performance.

The number of threads can be determined automatically to match the
number of CPU cores (using std::thread::hardware_concurrency()) by
setting thread_num = 0. To force a certain number of threads, set
thread_num to a positive value. In particular, setting thread_num = 1
forces the single-threaded execution.

When using multi-threading, there are two stages. First a work queue
of SimsNodes is generated by single threaded. The work queue is then
(embarassingly parallelly) executed by multiple threads. The size of
the work queue generated is given by bloom_size.

If bloom_size is too large, the single-threaded stage will take a long
time. If bloom_size is too small, there is a risk that one of tasks is
taking much longer and the other threads run out of work.)doc";

static const char *__doc_low_index_SimsTree_list_multi_threaded = R"doc()doc";

static const char *__doc_low_index_SimsTree_list_single_threaded = R"doc()doc";

static const char *__doc_low_index_SimsTree_long_relators = R"doc()doc";

static const char *__doc_low_index_SimsTree_recurse = R"doc()doc";

static const char *__doc_low_index_SimsTree_root = R"doc()doc";

static const char *__doc_low_index_SimsTree_short_relators = R"doc()doc";

static const char *__doc_low_index_SimsTree_thread_worker = R"doc()doc";

static const char *__doc_low_index_StackedSimsNode = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

