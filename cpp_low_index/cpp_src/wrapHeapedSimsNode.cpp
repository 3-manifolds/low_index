#include "heapedSimsNode.h"

#include "pybind11/pybind11.h"

#include "pybind11/stl.h"

void addHeapedSimsNode(pybind11::module_ &m) {
    pybind11::class_<HeapedSimsNode, SimsNode>(m, "HeapedSimsNode");
}
