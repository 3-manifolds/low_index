#include "abstractSimsNode.h"
#include "docAbstractSimsNode.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addAbstractSimsNode(pybind11::module_ &m) {
    pybind11::class_<AbstractSimsNode, CoveringSubgraph>(
                    m, "AbstractSimsNode",
                    DOC(low_index, AbstractSimsNode))
        .def("relators_lift", &AbstractSimsNode::relators_lift,
             DOC(low_index, AbstractSimsNode, relators_lift))
        .def("relators_may_lift", &AbstractSimsNode::relators_may_lift,
             DOC(low_index, AbstractSimsNode, relators_may_lift))
        .def("may_be_minimal", &AbstractSimsNode::may_be_minimal,
             DOC(low_index, AbstractSimsNode, may_be_minimal))
        .def_property_readonly("num_relators", &AbstractSimsNode::num_relators,
                               DOC(low_index, AbstractSimsNode, num_relators));
}

} // Namespace low_index
