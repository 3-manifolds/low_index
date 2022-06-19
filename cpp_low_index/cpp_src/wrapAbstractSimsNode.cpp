#include "abstractSimsNode.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addAbstractSimsNode(pybind11::module_ &m) {
    pybind11::class_<AbstractSimsNode, CoveringSubgraph>(m, "AbstractSimsNode");
}

} // Namespace low_index
