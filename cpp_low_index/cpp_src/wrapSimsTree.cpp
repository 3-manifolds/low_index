#include "simsTree.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

void addSimsTree(pybind11::module_ &m) {
    pybind11::class_<SimsTree>(m, "SimsTree")
        .def(pybind11::init<SimsNode::RankType,
                            SimsNode::DegreeType,
                            const std::vector<SimsNode::Relator> &,
                            const std::vector<SimsNode::Relator> &>(),
             pybind11::arg("rank"),
             pybind11::arg("max_degree"),
             pybind11::arg("short_relators"),
             pybind11::arg("long_relators"))
        .def("list", &SimsTree::list);
}
