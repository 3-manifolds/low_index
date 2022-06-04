#include "pybind11/pybind11.h"

void addCoveringSubgraph(pybind11::module_ &m);

PYBIND11_MODULE(low_index, m) {
    addCoveringSubgraph(m);
}
