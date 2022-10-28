#include "words.h"
#include "docWords.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addWords(pybind11::module_ &m) {
    m.def("parse_word",
          &parse_word,
          pybind11::arg("rank"),
          pybind11::arg("word"),
          DOC(low_index, parse_word));

    m.def("spin_short",
          &spin_short,
          pybind11::arg("relators"),
          pybind11::arg("max_degree"),
          DOC(low_index, spin_short));
}

}
