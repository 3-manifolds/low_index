#include "pybind11/pybind11.h"

void addCoveringSubgraph(pybind11::module_ &m);

PYBIND11_MODULE(cpp_low_index, m) {
    m.doc() = R"pbdoc(
cpp_low_index
-------------

A C++ reimplementation of `low_index`.
)pbdoc";
    addCoveringSubgraph(m);
}
