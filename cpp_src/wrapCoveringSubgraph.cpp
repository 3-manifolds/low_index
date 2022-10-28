#include "coveringSubgraph.h"
#include "docCoveringSubgraph.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addCoveringSubgraph(pybind11::module_ &m) {
    pybind11::class_<CoveringSubgraph>(m, "CoveringSubgraph",
                                       DOC(low_index, CoveringSubgraph))
        .def_property_readonly("rank", &CoveringSubgraph::rank,
                               DOC(low_index, CoveringSubgraph, rank))
	.def_property_readonly("degree", &CoveringSubgraph::degree,
                               DOC(low_index, CoveringSubgraph, degree))
	.def_property_readonly("max_degree", &CoveringSubgraph::degree,
                               DOC(low_index, CoveringSubgraph, max_degree))
        .def("is_complete", &CoveringSubgraph::is_complete,
             DOC(low_index, CoveringSubgraph, is_complete))
        .def("add_edge", &CoveringSubgraph::add_edge,
             DOC(low_index, CoveringSubgraph, add_edge))
        .def("verified_add_edge", &CoveringSubgraph::verified_add_edge,
             DOC(low_index, CoveringSubgraph, verified_add_edge))
        .def("__str__", &CoveringSubgraph::to_string,
             DOC(low_index, CoveringSubgraph, to_string))
        .def("permutation_rep", &CoveringSubgraph::permutation_rep,
             DOC(low_index, CoveringSubgraph, permutation_rep))
        .def("act_by", &CoveringSubgraph::act_by,
             DOC(low_index, CoveringSubgraph, act_by))
        .def("first_empty_slot", &CoveringSubgraph::first_empty_slot,
             DOC(low_index, CoveringSubgraph, first_empty_slot));
}

} // Namespace low_index
