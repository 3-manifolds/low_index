"""
Enumerate all based covering spaces with bounded degree of a finite Cayley
complex such that the basepoint has minimal complexity.

The algorithm used here is the one presented in "Computations with Finitely
Presented Groups" by Charles Sims, stripped of all of the irrelevant machinery
and viewed in the more natural context of covering spaces.

Conventions:

- A finite Cayley complex is a 2-complex with 1 vertex, G edges and finitely
  many 2-cells.  Each edge is directed and labeled with a positive integer.  The
  attaching maps of the 2-cells are represented by cyclically reduced words in
  the edge labels and their negations.  A negative label indicates that the edge
  is traversed opposite to the edge's direction.  These cyclically reduced words
  are called relations.

- The vertices of a degree D finite cover of a Cayley complex are indexed as
  1, ..., D. The base point is the vertex with index 1.

- The number R of edge labels is called the rank in the code. That refers to the
  rank of the free group that would be used in the group presentation associated
  with the Cayley complex.  Note that when a word is specified the rank must
  also be specified, since the word is being viewed as representing an element
  of a specific free group.

- The 1-skeleton of a cover of a finite Cayley complex is a labeled directed
  graph in which each vertex has G outgoing edges and G incoming edges, one for
  each label.  If the cover has degree D then it can be described with a DxG
  matrix in which the (i, j) entry is the index of the terminal vertex of the
  edge with initial vertex i and label j.  This is the outgoing matrix.
  A related DxG matrix, the incoming matrix, is the one for which the (i,j)
  entry is the index of the initial vertex of the edge with terminal vertex i
  and label j.  For efficiency we use and maintain both of these, even though
  they carry the same information. All entries of these matrices will be
  non-zero in a complete covering graph.

- While constructing covers we need to work with subgraphs of the 1-skeletion.
  A subgraph can be represented by a DxG matrix in which some of the entries are
  0.  The (i,j) entry of the outgoing matrix is 0 if and only if there is no
  edge with initial vertex i and label j in the subgraph.  Similarly, the (i,j)
  entry of the incoming matrix is 0 if and only if there is no edge with
  terminal vertex i and label j.
"""

import subprocess

cdef class CoveringSubgraph:
    cdef public int rank
    cdef public int degree
    cdef public int max_degree
    cdef int num_edges
    cdef int num_relators
    cdef unsigned char* outgoing
    cdef unsigned char* incoming
    cdef unsigned int slot_index

    def __cinit__(self, int rank, int max_degree, int num_relators=0,
                  bytes outgoing=b'', bytes incoming=b'',
                  bytes lift_indices=b'', bytes lift_vertices=b''):
        cdef int n = 0, degree = 1, size
        cdef unsigned char b
        self.degree = 1
        self.num_edges = 0
        self.rank = rank
        self.max_degree = max_degree
        self.num_relators = num_relators
        self.slot_index = 0
        size = rank*max_degree
        self.outgoing = <unsigned char *>PyMem_Malloc(size)
        memset(self.outgoing, 0, size)
        self.incoming = <unsigned char *>PyMem_Malloc(size)
        memset(self.incoming, 0, size)
        if outgoing:
            while n < size:
                b = outgoing[n]
                if b > degree:
                    degree = b
                self.outgoing[n] = b
                c = incoming[n]
                self.incoming[n] = c
                if c > degree:
                    degree = c
                n += 1
                if b > 0:
                    self.num_edges += 1
            self.degree = degree

    def __dealloc__(self):
        PyMem_Free(self.outgoing)
        PyMem_Free(self.incoming)

    def __str__(self):
        cdef int t
        if self.is_complete():
            result = 'Covering with edges:\n'
        else:
            result = 'Partial covering with edges:\n'
        for f in range(self.degree):
            for n in range(self.rank):
                 t = self.outgoing[f*self.rank + n]
                 if t:
                     result += '%d--%d->%d\n'%(f + 1, n + 1, t)
        return result[:-1]

    def __reduce__(self):
        cdef int size = self.rank*self.max_degree
        return (self.__class__,
                (self.rank, self.max_degree, self.num_relators,
                     self.outgoing[:size], self.incoming[:size]))

    def _data(self):
        print('out:', [n for n in self.outgoing[:self.rank*self.degree]])
        print('in:', [n for n in self.incoming[:self.rank*self.degree]])

    def __key__(self):
        return self.outgoing[:self.rank*self.degree]

    def __hash__(self):
        return hash(self.__key__())

    def __eq__(CoveringSubgraph self, CoveringSubgraph other):
        if self.rank != other.rank:
            return False
        if self.degree != other.degree:
            return False
        for i in range(self.rank*self.degree):
            if self.outgoing[i] != other.outgoing[i]:
                return False
        return True

    def __lt__(self, other):
        return self.__key__() < other.__key__()

    cpdef permutation_rep(self):
        """
        Return the permutation representation associated to this cover as a list
        of lists.  This representation gives the right action on vertices
        obtained by lifting each of the directed loops in 1-vertex graph which
        is the target of the covering map.  Each permutation is represented
        as a list of integers in [0, degree - 1] and there is one permutation
        for each generator of the group.
        """
        cdef int l, v, rank = self.rank, degree = self.degree
        cdef list rep = []
        if not self._is_complete():
            raise ValueError('The graph is not a covering.')
        for l in range(self.rank):
            rep.append([self.outgoing[v*rank + l] - 1 for v in range(degree)])
        return rep

    cpdef is_complete(self):
        return self.num_edges == self.rank*self.degree

    cdef inline _is_complete(self):
        return self.num_edges == self.rank*self.degree

    cdef add_edge(self, int letter, int from_vertex, int to_vertex):
        """
        Add an edge.
        """
        cdef int out_index, in_index, label = letter
        if letter < 0:
            label, from_vertex, to_vertex = -letter, to_vertex, from_vertex
        if from_vertex > self.degree or to_vertex > self.degree:
            self.degree += 1
        out_index = (from_vertex - 1)*self.rank + label - 1
        in_index = (to_vertex - 1)*self.rank + label - 1
        self.outgoing[out_index] = to_vertex
        self.incoming[in_index] = from_vertex
        self.num_edges += 1

    cdef verified_add_edge(self, int letter, int from_vertex, int to_vertex):
        """
        Add an edge provied that both ends of the edge will go into
        empty slots.  Return True if the edge is added, False if not.
        """
        cdef int out_index, in_index, label = letter
        if letter < 0:
            label, from_vertex, to_vertex = -letter, to_vertex, from_vertex
        if from_vertex > self.degree or to_vertex > self.degree:
            self.degree += 1
        out_index = (from_vertex - 1)*self.rank + label - 1
        in_index = (to_vertex - 1)*self.rank + label - 1
        if self.outgoing[out_index] != 0 or self.incoming[in_index] != 0:
            return False
        self.outgoing[out_index] = to_vertex
        self.incoming[in_index] = from_vertex
        self.num_edges += 1
        return True

    cdef remove_edge(self, int letter, int from_vertex, int to_vertex):
        """
        Add an edge.
        """
        cdef int out_index, in_index, label = letter
        if letter < 0:
            label, from_vertex, to_vertex = -letter, to_vertex, from_vertex
        if from_vertex > self.degree or to_vertex > self.degree:
            self.degree += 1
        out_index = (from_vertex - 1)*self.rank + label - 1
        in_index = (to_vertex - 1)*self.rank + label - 1
        self.outgoing[out_index] = 0
        self.incoming[in_index] = 0
        self.num_edges -= 1

    cdef act_by(self, int letter, int vertex):
        if letter > 0:
            return self.outgoing[(vertex - 1)*self.rank + letter - 1]
        elif letter < 0:
            return self.incoming[(vertex - 1)*self.rank - letter - 1]

    cpdef lift(self, ReducedWord word, int vertex):
        cdef int initial = vertex
        cdef int length = 0
        cdef int saved = vertex
        assert word.rank == self.rank
        for n in range(word.length):
            vertex = self.act_by(word.buffer[word.start + n], vertex)
            if vertex == 0:
                break
            saved = vertex
            length += 1
        return saved, length

    cpdef lift_inverse(self, ReducedWord word, int vertex):
        cdef int length = 0
        cdef int saved = vertex
        assert word.rank == self.rank
        cdef int end = word.start + word.length - 1
        for n in range(word.length):
            vertex = self.act_by(-word.buffer[end - n], vertex)
            if vertex == 0:
                break
            saved = vertex
            length += 1
        return saved, length

    cpdef first_empty_slot(self):
        cdef int v, l
        cdef div_t qr
        cdef unsigned char *incoming = self.incoming
        cdef unsigned char *outgoing = self.outgoing
        for n in range(self.rank*self.degree):
            if outgoing[n] == 0:
                qr = div(n, self.rank)
                v = qr.quot
                l = qr.rem
                return (v + 1, l + 1)
            if incoming[n] == 0:
                qr = div(n, self.rank)
                v = qr.quot
                l = qr.rem
                return (v + 1, -(l + 1))
        return (0,0)

    cdef inline int _first_empty_slot(CoveringSubgraph self):
        cdef int n, v, l, max_edges = self.rank*self.degree
        cdef div_t qr
        cdef unsigned char *incoming = self.incoming
        cdef unsigned char *outgoing = self.outgoing
        if max_edges == self.num_edges:
            return 0
        n = self.slot_index
        while n < max_edges:
            if outgoing[n] == 0:
                qr = div(n, self.rank)
                v = qr.quot
                l = qr.rem
                self.slot_index = n
                return ((l + 1) << 8) | (v + 1)
            if incoming[n] == 0:
                qr = div(n, self.rank)
                v = qr.quot
                l = qr.rem
                self.slot_index = n
                return (-(l + 1) << 8) | (v + 1)
            n += 1
        return 0

cdef class SimsNode(CoveringSubgraph):
    cdef unsigned char* state_info
    cdef unsigned char* lift_indices
    cdef unsigned char* lift_vertices
    cdef list children

    def __cinit__(self, int rank, int max_degree, int num_relators=0,
                  bytes outgoing=b'', bytes incoming=b'',
                  bytes lift_indices=b'', bytes lift_vertices=b''):
        cdef int n
        self.children = []
        if num_relators > 0:
            # Maintain per-vertex state for each relator and its inverse.
            size = num_relators*max_degree
            self.state_info = <unsigned char *>PyMem_Malloc(2*size)
            memset(self.state_info, 0, 2*size)
            self.lift_indices = self.state_info
            self.lift_vertices = &self.state_info[size]
            if lift_indices:
                for n, c in enumerate(lift_indices):
                    self.state_info[n] = c
            if lift_vertices:
                for n, c in enumerate(lift_vertices):
                    self.state_info[size + n] = c

    def __dealloc__(self):
        if self.num_relators:
            PyMem_Free(self.state_info)

    def __str__(self):
        cdef int t
        result = 'SimsNode with edges:\n'
        for f in range(self.degree):
            for n in range(self.rank):
                 t = self.outgoing[f*self.rank + n]
                 if t:
                     result += '%d--%d->%d\n'%(f + 1, n + 1, t)
        return result[:-1]

    def __reduce__(self):
        cdef int size = self.rank*self.max_degree
        data = (self.rank, self.max_degree, self.num_relators,
                self.outgoing[:size], self.incoming[:size])
        if self.num_relators:
            data = data + (self.lift_indices[:size], self.lift_vertices[:size])
        return (self.__class__, data)

    cdef clone(self):
        cdef result = SimsNode(self.rank, self.max_degree, self.num_relators)
        self._copy_in_place(result)
        return result

    cdef _copy_in_place(SimsNode self, SimsNode other):
        # These must have the same rank, max_degree, and num_relators.
        # We do not check!  The children are not copied.
        cdef int size = self.rank*self.max_degree
        other.degree = self.degree
        other.num_edges = self.num_edges
        other.num_relators = self.num_relators
        other.slot_index = self.slot_index
        memcpy(other.outgoing, self.outgoing, size)
        memcpy(other.incoming, self.incoming, size)
        if self.num_relators > 0:
            size = 2*self.num_relators*self.max_degree
            memcpy(other.state_info, self.state_info, size)

    cdef sprout(self, NodeManager manager):
        """
        Find the first empty edge slot in this based partial covering.  For each
        possible way to add an edge in that slot, create a new node containing
        the subgraph obtained by adding that edge.  Return the list of new nodes.
        """
        cdef int n, v, l, i, slot
        cdef int rank=self.rank
        cdef int degree = self.degree
        cdef int max_degree = self.max_degree
        cdef unsigned char *incoming = self.incoming
        cdef unsigned char *outgoing = self.outgoing
        cdef SimsNode new_subgraph
        cdef list relators = manager.tree.relators
        self.children = []
        slot = self._first_empty_slot()
        if slot == 0:
            return
        v = slot & 0xff
        l = slot >> 8
        targets = []
        # Add an edge to a new vertex if allowed. (It will be processed last.)
        if degree < max_degree:
            targets.append((l, v, degree + 1))
        # Add edges with from this slot to all possible target slots.
        if l > 0:
            i = 0
            for n in range(degree):
                t = incoming[i + l - 1]
                i += rank
                if t == 0:
                    targets.append((l, v, n+1))
        else:
            i = 0
            for n in range(degree):
                t = outgoing[i - l - 1]
                i += rank
                if t == 0:
                    targets.append((l, v, n+1))
        for l, v, n in targets:
            new_subgraph = manager.get_node()
            self._copy_in_place(new_subgraph)
            new_subgraph.add_edge(l, v, n)
            if (self.relators_may_lift(new_subgraph, relators)
                and new_subgraph.may_be_minimal()):
                self.children.append(new_subgraph)
            else:
                manager.recycle(new_subgraph)

    cpdef relators_lift(self, relators):
        cdef CyclicallyReducedWord w
        cdef int v, end, length
        for w in relators:
            for v in range(self.degree):
                end, length = self.lift(w, v + 1)
                if end != v + 1 or length < w.length:
                    return False
        return True

    cdef relators_may_lift(self, SimsNode child, list relators):
        """
        Check that when any relator is lifted to any vertex of a child graph it
        either lifts to a loop or runs into a missing edge. This subgraph uses
        its saved state as the starting point for checking the child.
        """
        cdef CyclicallyReducedWord w
        cdef char label
        cdef unsigned char index, vertex, save, length
        cdef int n = 0, v, i = 0, j, ok
        cdef int rank = child.rank, max_degree = child.max_degree
        for w in relators:
            length = w.length
            for v in range(child.degree):
                # Check whether relator n lifts to a loop at vertex v + 1.
                j = n*max_degree + v
                vertex = self.lift_vertices[j]
                if vertex == 255:
                    # We already checked that the relator lifts to a loop.
                    continue
                if vertex == 0:
                    # The state is uninitialized.
                    vertex = v + 1
                index = self.lift_indices[j]
                for i in range(index, length):
                    label = w.buffer[w.start + i]
                    save = vertex
                    if label > 0:
                        vertex = child.outgoing[rank*(vertex - 1) + label - 1]
                    else:
                        vertex = child.incoming[rank*(vertex - 1) - label - 1]
                    if vertex == 0:
                        # We hit a missing edge - save the state and go on.
                        child.lift_vertices[j] = save
                        child.lift_indices[j] = i
                        break
                if i == length - 1:
                    # The relator lifts, except the last edge may be missing.
                    if vertex == 0:
                        # The last edge is missing - add it now.
                        ok = child.verified_add_edge(label, save, v+1)
                        if not ok:
                            return False
                        if child._is_complete():
                            # Check that all relators lift for the complete graph.
                            return self.relators_may_lift(child, relators)
                        vertex = v + 1
                    if vertex == v + 1:
                        # The relator lifts.  Record that information.
                        child.lift_vertices[j] = 255
                        child.lift_indices[j] = length
                    else:
                        # No.  Discard this child.
                        return False
            n += 1
        return True

    cdef may_be_minimal(self):
        """
        Return False if the subgraph can provably not be extended to a cover
        which is minimal in its conjugacy class, True otherwise.

        The ordering of based coverings is given as follows.  Given a covering
        with rank R and degree D, consider all pairs (N, L) where N is a vertex
        index in [1, D] and L is a signed label in [-R, -1] or [1, R].  Order
        the signed labels as 1, -1, 2, -2, ..., R, -R.  Use that to lex order
        the 2*D*R pairs (N, L).  For each pair (N, L) assign an edge E(N, L) and
        a number s(N, L) as follows:

            * if L > 0, E(N, L) is the edge with initial vertex N and
              label L and s(N, L) is index of its terminal vertex.

            * if L < 0, E(N, L) is the edge with terminal vertex N and
              label -L and s(N, L) is the index of its initial vertex.

        The list [s(N, L) for all (N, L)] is the complexity of the covering;
        complexities are ordered lexicographically.

        Any conjugate of the subgroup corresponding to a covering graph can be
        obtained by changing the basepoint.  A choice of basepoint determines an
        indexing of the vertices where the index of a vertex v is the next
        available index at the moment when the first E(N, L) having v as its
        terminal (initial) vertex if L > 0 (L < 0) is added.  We only enumerate
        based coverings for which the basepoint is chosen to minimize
        complexity.  The subgroups corresponding to these coverings will then by
        unique up to conjugacy.

        Even when working with a proper subgraph of a covering graph (containing
        the basepoint) it may be possible to deduce that the subgraph cannot be
        extended to a minimal covering.  In such a case the subgraph should not
        be added to the Sims tree.  If this test is done at each stage of the
        construction of the tree, the complete graphs in the final tree will
        automatically be minimal.

        This method iterates through all vertices of the subgraph, constructing
        as much as possible of the vertex ordering determined by taking that
        vertex as a basepoint.  It returns False as soon as it encounters an
        edge which would result in a higher complexity. If no such edge is found
        for any choice of basepoint it returns True.
        """
        cdef unsigned char std_to_alt[256]
        cdef unsigned char alt_to_std[256]
        cdef int basepoint, max_index
        cdef int slot_vertex, slot_label
        cdef int degree = self.degree, rank = self.rank
        cdef int a, b, c
        cdef unsigned char *outgoing = self.outgoing
        cdef unsigned char *incoming = self.incoming
        for basepoint in range(2, degree + 1):
            # We are working with the standard indexing (determined by putting
            # the basepoint at vertex 1) and an alternate indexing determined by
            # a different basepoint.  We construct mappings between the two
            # indexings and store them in the arrays std_to_alt and
            # alt_to_std. (For convenience when dealing with 1-based indices,
            # just ignore the 0 entry).
            memset(std_to_alt, 0, degree + 1)
            # It is not necessary to clear alt_to_std
            #memset(alt_to_std, 0, degree + 1)
            # Initial state.
            std_to_alt[basepoint] = 1
            alt_to_std[1] = basepoint
            max_index = 1
            slot_vertex = 1
            slot_label = 0
            # Iterate over all possible slots.
            while slot_vertex <= degree and slot_label < 2*self.rank:
                # Check that the slot is filled with repect to both indexings.
                sign = slot_label & 0x1
                l = slot_label >> 1
                if sign == 0: # positive label
                    a = outgoing[(slot_vertex - 1)*rank + l]
                    b = outgoing[(alt_to_std[slot_vertex] - 1)*rank + l]
                else: # negative label
                    a = incoming[(slot_vertex - 1)*rank + l]
                    b = incoming[(alt_to_std[slot_vertex] - 1)*rank + l]
                if a == 0 or b == 0:
                    # The slot was empty in one indexing, so we cannot decide.
                    break
                # Update the mappings.
                if std_to_alt[b] == 0:
                    # This edge is the first, with respect to the alternate
                    # indexing, that is incident to the vertex with standard
                    # index b.  We now know its alternate index.
                    max_index += 1
                    std_to_alt[b] = max_index
                    alt_to_std[max_index] = b
                #Compare the old and new indices of the other end of the edge
                c = std_to_alt[b]
                if c < a:
                    # The new basepoint is better - discard this graph.
                    return False
                if c > a:
                    # The old basepoint is better - try the next one.
                    break
                # Increment the slot.
                slot_label += 1
                if slot_label == 2*rank:
                    slot_label = 0
                    slot_vertex += 1
        return True

cdef class NodeManager:
    cdef SimsTree tree
    cdef list stack, cache
    cdef int rank, max_degree, num_relators

    def __init__(self, SimsTree tree):
        self.tree = tree
        self.rank = tree.rank
        self.max_degree = tree.max_degree
        self.num_relators = tree.num_relators
        self.cache = []
        self.stack = []
        self.push(tree.root)

    cdef get_node(NodeManager self):
        """
        If possible, reuse cached SimsNodes to avoid needless construction
        and destruction of nodes.
        """
        if self.cache:
            return self.cache.pop()
        return SimsNode(self.rank, self.max_degree, self.num_relators)

    cdef inline stack_is_empty(NodeManager self):
        return len(self.stack) == 0

    cdef inline peek(NodeManager self):
        return self.stack[-1]

    cdef push(NodeManager self, SimsNode node):
        node.sprout(self)
        self.stack.append(node)

    cdef pop(NodeManager self):
        cdef SimsNode node = self.stack.pop()
        self.cache.append(node)
        return node

    cdef recycle(NodeManager self, SimsNode node):
        self.cache.append(node)

cdef class SimsTreeIterator:
    cdef int rank, max_degree, num_relators
    cdef NodeManager manager

    def __init__(self, SimsTree tree):
        self.rank = tree.rank
        self.max_degree = tree.max_degree
        self.num_relators = tree.num_relators
        self.manager = NodeManager(tree)

    def __next__(self):
        cdef node = self._next()
        if node is None:
            raise StopIteration
        return node

    cdef inline _next(self):
        cdef list sprouts
        cdef NodeManager manager = self.manager
        cdef SimsNode top, node
        while True:
            if self.manager.stack_is_empty():
                return None
            # Peek at the top of the stack.
            top = self.manager.peek()
            # If the top graph is complete, pop it, cache it, and clone it.
            if top._is_complete():
                node = self.manager.pop()
                return node.clone()
            # If there are unvisited children, visit the next one.
            sprouts = top.children
            if len(sprouts) > 0:
                self.manager.push(sprouts.pop())
            # Otherwise, ascend to a node with unvisited children.
            # The popped nodes will be recycled.
            else:
                while True:
                    if self.manager.stack_is_empty():
                        return None
                    # Take a peek to see if there are unvisited children.
                    top = manager.peek()
                    if len(top.children) == 0:
                        self.manager.pop()
                    else:
                        break

cdef class SimsTree:
    """
    A "tree" of SimsNodes constructed by Sims algorithm.  Each
    SimsNode in the tree is constructed by adding edges to its parent in
    such a way that every relation that can be lifted lifts to a loop.  (It is
    allowed for a relation to fail to lift because an edge is missing from the
    subgraph.)

    Implementation note: The collection of all graphs produced by Sims algorithm
    can be viewed as a tree, where the children of a node are obtained by adding
    one edge at the first empty slot.  However, only the tips of the tree are of
    interest.  So we can implement the "tree" as a python list.  Each pass
    through the loop in the (currently unused) bloom method generates a new list
    by replacing each subgraph by a list of subgraphs obtained by adding edges.
    This turns out to be very memory intensive, since these lists grow very
    large before finally collapsing at the end of the computation.  We also
    provide an iterator which iterates through the tips of the tree while using
    a constant amount of memory.  The depth of the tree is bounded by the number
    of edges in a cover of the maximal allowed degree.  Traversal of the tree in
    depth-first order only requires a stack of size equal to the maximum depth.

    >>> from low_index import *
    >>> t = SimsTree(rank=1, max_degree=3).list()
    >>> len(t)
    3
    >>> for g in t: print(g)
    ...
    SimsNode with edges:
    1--1->2
    2--1->3
    3--1->1
    SimsNode with edges:
    1--1->2
    2--1->1
    SimsNode with edges:
    1--1->1
    >>> t = SimsTree(rank=2, max_degree=3).list()
    >>> len(t)
    11
    >>> print(t[7])
    SimsNode with edges:
    1--1->1
    1--2->2
    2--1->3
    2--2->3
    3--1->2
    3--2->1
    >>> S7_relators = ['aaaaaaa', 'bb', 'abababababab', 'AbabAbabAbab',\
                       'AAbaabAAbaab', 'AAAbaaabAAAbaaab']
    >>> len(SimsTree(2, 20, S7_relators).list())
    4
    >>> len(SimsTree(2, 20, S7_relators, num_long_relators=1).list())
    4
    """
    cdef public int rank
    cdef public int max_degree
    cdef public SimsNode root
    cdef public list nodes
    cdef public list relators
    cdef public list relator_strings
    cdef public list long_relators
    cdef public str strategy
    cdef int num_relators
    cdef int num_long_relators

    def __init__(self, int rank=1, int max_degree=1, relators=None,
                     strategy="spin_short", root=None, int num_long_relators=0):
        self.rank = rank
        self.max_degree = max_degree
        self.strategy = strategy
        self.num_long_relators = num_long_relators
        relators = [] if relators is None else sorted(relators, key=len)
        if num_long_relators:
            self.long_relators = [CyclicallyReducedWord(r, self.rank)
                                    for r in relators[-num_long_relators:]]
            relators = relators[:-num_long_relators]
        self.relator_strings = [r for r in relators]
        if strategy == 'spin_short':
            relators = self.spin_short_relators(relators)
        self.relators = [CyclicallyReducedWord(r, self.rank) for r in relators]
        self.num_relators = len(self.relators)
        if isinstance(root, bytes):
            self.root = pickle.loads(root)
        elif isinstance(root, SimsNode):
            self.root = root
        else:
            self.root = SimsNode(rank=rank, max_degree=max_degree,
                                 num_relators=self.num_relators)
        self.nodes = [self.root]

    def __iter__(self):
        return SimsTreeIterator(self)

    def __reduce__(self):
        relators = [str(r) for r in self.relators]
        return (self.__class__,
                (self.rank, self.max_degree, relators, self.strategy,
                pickle.dumps(self.root))
                )

    def plant(self, SimsNode node):
        """
        Construct a SimsTree with the given node as its root, using the
        same rank, max_degree, relators and strategy.
        """
        cdef list relators = [str(r) for r in self.relators]
        return SimsTree(self.rank, self.max_degree, relators, root=node)

    cdef spin(self, str word):
        return sorted(set(word[k:] + word[:k] for k in range(len(word))))

    def spin_short_relators(self, relators):
        result = []
        if relators:
            avg = sum(len(r) for r in relators) / len(relators)
            for r in relators:
                if len(r) <= max(avg, self.max_degree):
                    result += self.spin(r)
                else:
                    result.append(r)
        return sorted(result, key=len)

    def list(self, use_mp=True):
        nodes = self.list_mp() if use_mp else self.list_1p()
        if self.num_long_relators:
            return [N for N in nodes if N.relators_lift(self.long_relators)]
        return nodes

    cdef list_1p(self):

        """
        Return a list created from this tree's iterator.  We call the
        C implementation of the iterator's next method directly. Does
        not check long relators.
	"""
	
        cdef list result = []
        cdef SimsTreeIterator iterator = SimsTreeIterator(self)
        cdef SimsNode node
        while True:
            node = iterator._next()
            if node is None:
                break
            result.append(node)
        return result

    cdef list_mp(self, int target_size=1000):
        """
        Compute the list of covers with a pool of worker processes. Does
        not check long relators.
        """
        result = []
        relator_strings = [str(r) for r in self.relator_strings]
        args = [sys.executable, multi.__file__, str(self.rank),
                str(self.max_degree), str(target_size)] + relator_strings
        with subprocess.Popen(args, stdout=subprocess.PIPE) as proc:
            while proc:
                n = proc.stdout.read(4)
                if n:
                    size = int.from_bytes(n, 'big')
                    result.append(pickle.loads(proc.stdout.read(size)))
                else:
                    break
        return result

    # This method uses too much memory to be used in the list method, but is
    # used to seed the multiprocessing list method.

    cpdef bloom(self, int target_size=1000):
        """
        Return a list of nodes created by iteratively replacing each node
        with a sublist consisting of its children.  Return the list as soon
        as it exceeds the target size.
        """
        cdef SimsNode tip
        cdef int count = 0
        cdef list new_nodes
        cdef list sprouts
        cdef NodeManager manager = NodeManager(self)
        while True:
            count = 0
            new_nodes = []
            for tip in self.nodes:
                tip.sprout(manager)
                sprouts = tip.children
                count += len(sprouts)
                if sprouts:
                    new_nodes += sprouts
                elif tip._is_complete():
                    new_nodes.append(tip)
            if count == 0:
                break
            self.nodes = new_nodes
            if target_size and len(new_nodes) > target_size:
                break
        return self.nodes
