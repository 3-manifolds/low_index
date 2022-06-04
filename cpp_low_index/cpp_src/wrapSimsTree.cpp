#include "simsTree.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

void addSimsTree(pybind11::module_ &m) {
    pybind11::class_<SimsTree>(m, "SimsTree")
        .def(pybind11::init<int, int, const std::vector<std::vector<int>>&>())
        .def("list", &SimsTree::list);
}
