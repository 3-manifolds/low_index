"""

Test script.

Assumes build with::

  python setup.py build --build-lib lib_dev

"""

import sys
sys.path.insert(0, './lib_dev')
import cpp_low_index

cs = cpp_low_index.CoveringSubgraph(8)
print(cs.rank)

