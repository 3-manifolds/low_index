#include "simsNode.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

void addSimsNode(pybind11::module_ &m) {
    pybind11::class_<SimsNode, CoveringSubgraph>(m, "SimsNode");
}
