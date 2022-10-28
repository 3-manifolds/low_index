#include "lowIndex.h"

#include "docLowIndex.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addLowIndex(pybind11::module_ &m) {

    m.attr("spin_short_strategy") = spin_short_strategy;

    {
        using Signature = std::vector<std::vector<std::vector<DegreeType>>>(*)(
            RankType,
            const std::vector<Relator> &,
            const std::vector<Relator> &,
            DegreeType,
            const std::string &,
            unsigned int num_threads);

        m.def("permutation_reps",
              Signature(&permutation_reps),
              pybind11::arg("rank"),
              pybind11::arg("short_relators"),
              pybind11::arg("long_relators"),
              pybind11::arg("max_degree"),
              pybind11::arg("strategy") = spin_short_strategy,
              pybind11::arg("num_threads") = 0,
              DOC(low_index, permutation_reps));
    }

    {
        using Signature = std::vector<std::vector<std::vector<DegreeType>>>(*)(
            RankType,
            const std::vector<std::string> &,
            const std::vector<std::string> &,
            DegreeType,
            const std::string &,
            unsigned int num_threads);

        m.def("permutation_reps",
              Signature(&permutation_reps),
              pybind11::arg("rank"),
              pybind11::arg("short_relators"),
              pybind11::arg("long_relators"),
              pybind11::arg("max_degree"),
              pybind11::arg("strategy") = spin_short_strategy,
              pybind11::arg("num_threads") = 0,
              DOC(low_index, permutation_reps_2));
    }
}

}
