#include "simsNode.h"
#include "docSimsNode.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addSimsNode(pybind11::module_ &m) {
    pybind11::class_<SimsNode, AbstractSimsNode>(
                    m, "SimsNode",
                    DOC(low_index, SimsNode))
        .def(pybind11::init<RankType,
                            DegreeType,
                            unsigned int>(),
             pybind11::arg("rank"),
             pybind11::arg("max_degree"),
             pybind11::arg("num_relators") = 0,
             DOC(low_index, SimsNode, SimsNode))
        .def(pybind11::init<const SimsNode&>(),
             DOC(low_index, SimsNode, SimsNode_3));
}

} // Namespace low_index
