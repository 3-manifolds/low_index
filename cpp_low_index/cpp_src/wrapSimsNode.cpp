#include "simsNode.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addSimsNode(pybind11::module_ &m) {
    pybind11::class_<SimsNode, CoveringSubgraph>(m, "SimsNode");
}

} // Namespace low_index
