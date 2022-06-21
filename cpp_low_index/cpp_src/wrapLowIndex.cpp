#include "lowIndex.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addLowIndex(pybind11::module_ &m) {

    m.attr("spin_short_strategy") = spin_short_strategy;
    m.attr("default_bloom_size") = default_bloom_size;
    
    m.def("parse_word",
          &parse_word,
          pybind11::arg("rank"),
          pybind11::arg("words"));

    using _Sig1 = std::vector<std::vector<std::vector<DegreeType>>>(*)(
        RankType,
        const std::vector<Relator> &,
        DegreeType,
        unsigned int,
        const std::string &,
        size_t bloom_size,
        unsigned int thread_num);

    using _Sig2 = std::vector<std::vector<std::vector<DegreeType>>>(*)(
        RankType,
        const std::vector<std::string> &,
        DegreeType,
        unsigned int,
        const std::string &,
        size_t bloom_size,
        unsigned int thread_num);

    m.def("permutation_reps",
          _Sig1(&permutation_reps),
          pybind11::arg("rank"),
          pybind11::arg("relators"),
          pybind11::arg("max_degree"),
          pybind11::arg("num_long_relators") = 0,
          pybind11::arg("strategy") = spin_short_strategy,
          pybind11::arg("bloom_size") = default_bloom_size,
          pybind11::arg("thread_num") = 0);

    m.def("permutation_reps",
          _Sig2(&permutation_reps),
          pybind11::arg("rank"),
          pybind11::arg("relators"),
          pybind11::arg("max_degree"),
          pybind11::arg("num_long_relators") = 0,
          pybind11::arg("strategy") = spin_short_strategy,
          pybind11::arg("bloom_size") = default_bloom_size,
          pybind11::arg("thread_num") = 0);
}

}
