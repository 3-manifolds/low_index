// The pybind11 headers are somewhat heavy - compiling all pieces
// of the python wrapping in the same translation unit speeds up
// compilation significantly.

#include "wrapModule.cpp"
#include "wrapCoveringSubgraph.cpp"
#include "wrapAbstractSimsNode.cpp"
#include "wrapSimsNode.cpp"
#include "wrapSimsTreeBase.cpp"
#include "wrapSimsTree.cpp"
#include "wrapSimsTreeMultiThreaded.cpp"
#include "wrapLowIndex.cpp"
#include "wrapWords.cpp"
