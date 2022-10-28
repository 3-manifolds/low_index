#include "simsTreeBase.h"
#include "docSimsTreeBase.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

namespace low_index {

void addSimsTreeBase(pybind11::module_ &m) {
    pybind11::class_<SimsTreeBase>(
            m, "SimsTreeBase", DOC(low_index, SimsTreeBase))
        .def("list", &SimsTreeBase::list,
             DOC(low_index, SimsTreeBase, list));
}

}
