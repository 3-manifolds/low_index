#include "coveringSubgraph.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

void addCoveringSubgraph(pybind11::module_ &m) {
    pybind11::class_<CoveringSubgraph>(m, "CoveringSubgraph")
        .def_property_readonly("rank", &CoveringSubgraph::rank)
	.def_property_readonly("degree", &CoveringSubgraph::degree)
        .def("is_complete", &CoveringSubgraph::is_complete)
        .def("add_edge", &CoveringSubgraph::add_edge)
        .def("verified_add_edge", &CoveringSubgraph::verified_add_edge)
        .def("__str__", &CoveringSubgraph::to_string)
        .def("permutation_rep", &CoveringSubgraph::permutation_rep)
        .def("act_by", &CoveringSubgraph::act_by)
        .def("first_empty_slot", &CoveringSubgraph::first_empty_slot);
}
