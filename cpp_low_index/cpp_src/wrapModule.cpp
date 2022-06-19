#include "pybind11/pybind11.h"

namespace low_index {

void addCoveringSubgraph(pybind11::module_ &m);
void addAbstractSimsNode(pybind11::module_ &m);
void addSimsNode(pybind11::module_ &m);
void addSimsTree(pybind11::module_ &m);

}

PYBIND11_MODULE(cpp_low_index, m) {
    using namespace low_index;

    m.doc() = R"pbdoc(
cpp_low_index
-------------

A C++ reimplementation of `low_index`.
)pbdoc";

    addCoveringSubgraph(m);
    addAbstractSimsNode(m);
    addSimsNode(m);
    addSimsTree(m);
}