#include "simsTreeMultiThreaded.h"
#include "docSimsTreeMultiThreaded.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addSimsTreeMultiThreaded(pybind11::module_ &m) {
    pybind11::class_<SimsTreeMultiThreaded, SimsTreeBase>(
            m, "SimsTreeMultiThreaded",
            DOC(low_index, SimsTreeMultiThreaded))
        .def(pybind11::init<RankType,
                            DegreeType,
                            const std::vector<Relator> &,
                            const std::vector<Relator> &,
                            unsigned int>(),
             pybind11::arg("rank"),
             pybind11::arg("max_degree"),
             pybind11::arg("short_relators"),
             pybind11::arg("long_relators"),
             pybind11::arg("num_threads"),
             DOC(low_index, SimsTreeMultiThreaded, SimsTreeMultiThreaded));
}

}
