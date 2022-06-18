#include "heapedSimsNode.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

void addHeapedSimsNode(pybind11::module_ &m) {
    pybind11::class_<HeapedSimsNode, SimsNode>(m, "HeapedSimsNode")
        .def(pybind11::init<SimsNode::RankType,
                            SimsNode::DegreeType,
                            unsigned int>(),
             pybind11::arg("rank"),
             pybind11::arg("max_degree"),
             pybind11::arg("num_relators") = 0,
             pybind11::doc(
                 R"pbdoc(SimsNode is a partial covering subgraph with checks to see whether relators lift)pbdoc"));

}
