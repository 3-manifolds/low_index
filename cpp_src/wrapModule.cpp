#include "pybind11/pybind11.h"

#include <thread>

namespace low_index {

void addLowIndex(pybind11::module_ &m);
void addWords(pybind11::module_ &m);
void addCoveringSubgraph(pybind11::module_ &m);
void addAbstractSimsNode(pybind11::module_ &m);
void addSimsNode(pybind11::module_ &m);
void addSimsTreeBase(pybind11::module_ &m);
void addSimsTree(pybind11::module_ &m);
void addSimsTreeMultiThreaded(pybind11::module_ &m);

}

PYBIND11_MODULE(_low_index, m) {
    using namespace low_index;

    m.doc() = R"pbdoc(
low_index
-------------

A module to enumerate low-index subgroups.
)pbdoc";

    addLowIndex(m);
    addWords(m);
    addCoveringSubgraph(m);
    addAbstractSimsNode(m);
    addSimsNode(m);
    addSimsTreeBase(m);
    addSimsTree(m);
    addSimsTreeMultiThreaded(m);

    m.def("hardware_concurrency",
          &std::thread::hardware_concurrency,
          R"doc(The number of cores reported by the operating system.)doc");
}
