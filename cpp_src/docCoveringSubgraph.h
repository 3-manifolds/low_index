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


static const char *__doc_low_index_CoveringSubgraph =
R"doc(CoveringSubgraph

Describes a graph with: - Vertices indexed by 1, 2, ..., degree().
Note that degree() can be at most max_degree(). In other words, we
need to specify the maximal degree when instantiating a
CoveringSubgraph. The vertex indices are of the integral type
DegreeType. The value 0 and the maximum value are reservered for
special purposes, see CoveringSubgraph::act_by and
AbstractSimsNode::_relator_may_lift. - Directed edges labeled by
letters 1, 2, ..., rank(). Note that there can be at most one directed
edge with the same label and same starting vertex or same ending
vertex, respectively. If a directed edge labeled by the letter l is
flipped, we label it by the inverse letter -l.

We call such a graph complete, if there a directed edge for each
vertex 1, 2, ..., degree() and each letter 1, 2, ..., rank().

Let G be a finitely presented group. It might help to think of G as a
Cayley complex with 1 vertex, one edge for each generator and one cell
for each relator of G. Or think of each generator as a face-pairing of
a fundamental polyhedron with the relators coming from edge and other
relations.

Thinking of the letters as generators the G, consider a complete graph
such that for each vertex and relator we end up at the same vertex
when traversing the (labeled) edges in the order given by the relator.

Such a complete graph can be interpreted in the following ways: 1. A
coset table for a subgroup H of G. Each vertex corresponds to a coset
in G/H. To multiply a coset by a letter l, look at the end of an edge
labeled by l and starting at the given coset. 2. A representation G ->
S_degree. The image of a letter l is the permutation obtained by
letting all edges labeled l take the vertices to the vertices. 3. A
covering space of the Cayley complex with the graph as 1-skeleton. 4.
A covering space of a 3-manifold. Take a copy of the fundamental
polyhedron for each vertex. For each edge labeled l, lift the face-
pairing so that it pairs a face of the copy corresponding to the start
vertex to a face of the copy corresponding to the end vertex.

Internally, such a (not necessarily complete) graph is stored as two
matrices of size degree() * rank() (we allocate memory for
max_degree() * rank()) for the outgoing and incoming edges. That is,
if there is a directed edge from i to j labeled by l (with l > 0),
then the entry at (i, l) in the outgoing matrix is j and the one at
(j, l) in the incoming matrix is i. A matrix entry takes the value
zero if no edge with the respective label starts or ends,
respectively, at the respective vertex.

This is an abstract class. In particular, it is the responsibility of
the subclasses to allocate memory for the graph and then set
CoveringSubgraph::_outgoing and CoveringSubgraph::_incoming to point
to the allocated memory.)doc";

static const char *__doc_low_index_CoveringSubgraph_CoveringSubgraph = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_CoveringSubgraph_2 = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_act_by =
R"doc(If there is an edge starting at the given vertex labeled by the given
letter, return its end vertex. Otherwise, return 0.)doc";

static const char *__doc_low_index_CoveringSubgraph_add_edge =
R"doc(Add an edge from a vertex to a vertex labeled by letter which can be
negative.

In order for the degree to grow, one of the given vertices is allowed
to go one step above the current degree (in which case the degree is
increased).)doc";

static const char *__doc_low_index_CoveringSubgraph_add_edge_2 = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_degree = R"doc(Current number of vertices in the graph.)doc";

static const char *__doc_low_index_CoveringSubgraph_degree_2 = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_first_empty_slot =
R"doc(Find the first (in a particular order that is important for
AbstractSimsNode::may_be_minimal) pair of (letter, vertex) (letter
might be negative) such that there is no edge from that vertex labeled
by that letter.)doc";

static const char *__doc_low_index_CoveringSubgraph_incoming = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_is_complete = R"doc(Is graph complete in the above sense.)doc";

static const char *__doc_low_index_CoveringSubgraph_max_degree = R"doc(Maximal number of vertices graph can have.)doc";

static const char *__doc_low_index_CoveringSubgraph_max_degree_2 = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_memory_start = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_num_edges =
R"doc(Number of directed edges (counting only those labeled by positive
letters).)doc";

static const char *__doc_low_index_CoveringSubgraph_num_edges_2 = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_operator_assign = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_outgoing = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_permutation_rep =
R"doc(Give the representation into the symmetric group S_degree. That is,
for each letter, give the corresponding permutation on the numbers 1,
..., degree().)doc";

static const char *__doc_low_index_CoveringSubgraph_rank = R"doc(Rank. That is number of (positive) letters used to label the edges.)doc";

static const char *__doc_low_index_CoveringSubgraph_rank_2 = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_slot_index = R"doc()doc";

static const char *__doc_low_index_CoveringSubgraph_to_string = R"doc(String representation - particularly useful for debugging.)doc";

static const char *__doc_low_index_CoveringSubgraph_verified_add_edge =
R"doc(Add an edge but only if there is no edge ending at the same vertex
with the same letter. Returns true on success.

Note that this is not checking whether there is an edge already
starting at the same vertex with the same letter.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

