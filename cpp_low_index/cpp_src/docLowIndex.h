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


static const char *__doc_low_index_permutation_reps =
R"doc(Given a finitely presented group G, return a permutation
representation for each subgroup of index up to max_degree.

The number of generators of G is given by rank and the relators by
short_relators and long_relators. Each relator is a list of non-zero
integers with 1, ..., rank corresponding to the generators of G and
-1, ..., -rank corresponding to the inverse generators.

The result is a list of permutation representation (see CoveringSubgraph
for equivalence to a covering space of the Cayley complex or coset
table). Each permutation representation consists of a list of
permutations, one for each generator of G. Such a permutation sigma
acts on 0, ..., degree - 1 where degree <= max_degree is the index of
the corresponding subgroup. sigma is encoded as list [ sigma(0), ...,
sigma(degree - 1) ].

Note that the function expects a partition of the relators into two.
The result is independent of this partition but the performance of the
algorithm very much depends on this choice. The underlying reason lies
in the search tree of incomplete CoveringSubgraph's the algorithm has to
traverse. Only one set of relators (short_relators) is used to prune
the the tree while the other set of relators (long_relators) is only
checked when the algorithm hit a leaf node (corresponding to a
complete CoveringSubgraph). Heuristically, short relators (especially
ones shorter than max_degree) prune the tree dramatically. However,
checking a long relator (much longer than max_degree) against an
incomplete CoveringSubgraph is often inconclusive and barely prunes the
tree while being quite costly.

Some applications might have a natural of relators into short and
long, e.g., the relators in the fundamental group corresponding to
long Dehn-fillings of a triangulation tend to be long.

Note that while cyclically shifting a relator word or taking its
inverse does not change the finitely presented group, it does make a
difference when trying to lift a word in an incomplete CoveringSubgraph.
To increase the opportunities of prunning the tree, this function can
(and will by default) expand the set of given relator words based on
the strategy argument. See spin_short for a description of the default
strategy.

The number of threads being used can be forced by num_threads. Note
that num_threads = 0 automatically determines the number of threads by
using the number of cores reported by the operating system.
num_threads = 1 forces the single-threaded implementation.)doc";

static const char *__doc_low_index_permutation_reps_2 =
R"doc(An overload of permutation_reps that takes the relators as SnapPy-
style words.

If rank <= 26, the letters a, b, c, ... correspond to the generators
and A, B, C, ... correspond to the inverse generators.

If rank > 26, the generators are encoded as x1, x2, ... and the
inverse generators as X1, X2, ...)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

