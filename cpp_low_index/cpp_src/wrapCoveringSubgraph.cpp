#include "coveringSubgraph.h"

#include "pybind11/pybind11.h"

void addCoveringSubgraph(pybind11::module_ &m) {
    pybind11::class_<CoveringSubgraph>(m, "CoveringSubgraph")
        .def(pybind11::init<int>())
        .def_property_readonly("rank", &CoveringSubgraph::GetRank);
}
