"""

Test script.

Assumes build with::

  python setup.py build --build-lib lib_dev

"""

import sys
sys.path.insert(0, './lib_dev')
import cpp_low_index
print(cpp_low_index.__doc__)
print(cpp_low_index.CoveringSubgraph.__init__.__doc__)

cs = cpp_low_index.CoveringSubgraph(8, 4)
print(cs.rank)

cs.add_edge(2, 1, 1)
cs.add_edge(3, 1, 2)
cs.add_edge(4, 2, 3)
print(cs.verified_add_edge(2, 1, 2))
print(cs.verified_add_edge(2, 2, 3))

print(cs.to_string())
print(cs.is_complete())

cs = cpp_low_index.CoveringSubgraph(1, 1)
cs.add_edge(1, 1, 1)
print(cs.permutation_rep())
