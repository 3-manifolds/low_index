#include "simsTreeBase.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addSimsTreeBase(pybind11::module_ &m) {
    pybind11::class_<SimsTreeBase>(m, "SimsTreeBase")
        .def("list", &SimsTreeBase::list);
}

}
