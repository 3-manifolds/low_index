#include "lowIndex.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addLowIndex(pybind11::module_ &m) {

    m.attr("spin_short_strategy") = spin_short_strategy;

    m.def("parse_word",
          &parse_word,
          pybind11::arg("rank"),
          pybind11::arg("words"));

    m.def("compute_short_and_long_relators",
          &compute_short_and_long_relators,
          pybind11::arg("rank"),
          pybind11::arg("relators"),
          pybind11::arg("max_degree"),
          pybind11::arg("num_long_relators") = 0,
          pybind11::arg("strategy") = spin_short_strategy);

    {
        using Signature = std::vector<std::vector<std::vector<DegreeType>>>(*)(
            RankType,
            const std::vector<Relator> &,
            DegreeType,
            unsigned int,
            const std::string &,
            unsigned int thread_num);

        m.def("permutation_reps",
              Signature(&permutation_reps),
              pybind11::arg("rank"),
              pybind11::arg("relators"),
              pybind11::arg("max_degree"),
              pybind11::arg("num_long_relators") = 0,
              pybind11::arg("strategy") = spin_short_strategy,
              pybind11::arg("thread_num") = 0);
    }

    {
        using Signature = std::vector<std::vector<std::vector<DegreeType>>>(*)(
            RankType,
            const std::vector<std::string> &,
            DegreeType,
            unsigned int,
            const std::string &,
            unsigned int thread_num);

        m.def("permutation_reps",
              Signature(&permutation_reps),
              pybind11::arg("rank"),
              pybind11::arg("relators"),
              pybind11::arg("max_degree"),
              pybind11::arg("num_long_relators") = 0,
              pybind11::arg("strategy") = spin_short_strategy,
              pybind11::arg("thread_num") = 0);
    }
}

}
