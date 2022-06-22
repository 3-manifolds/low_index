#include "simsTree.h"
#include "docSimsTree.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addSimsTree(pybind11::module_ &m) {
    pybind11::class_<SimsTree>(m, "SimsTree",
                               DOC(low_index, SimsTree))
        .def(pybind11::init<RankType,
                            DegreeType,
                            const std::vector<Relator> &,
                            const std::vector<Relator> &>(),
             pybind11::arg("rank"),
             pybind11::arg("max_degree"),
             pybind11::arg("short_relators"),
             pybind11::arg("long_relators"),
             DOC(low_index, SimsTree, SimsTree))
        .def("list",
             &SimsTree::list,
             pybind11::arg("bloom_size"),
             pybind11::arg("thread_num"),
             DOC(low_index, SimsTree, list));
}

}
