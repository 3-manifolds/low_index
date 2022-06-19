#include "lowIndex.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addLowIndex(pybind11::module_ &m) {

    m.def("permutation_reps",
          &permutation_reps,
          pybind11::arg("rank"),
          pybind11::arg("relators"),
          pybind11::arg("max_degree"),
          pybind11::arg("num_long_relators") = 0,
          pybind11::arg("strategy") = "spinShort",
          pybind11::arg("bloom_size") = 1000,
          pybind11::arg("thread_num") = 0);
}

}
