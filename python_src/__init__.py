from ._low_index import *

# Version 1.1 was used for the cython implemention
# Version 1.2 (and later) is used for the C++/pybind11 implementation
__version__ = '1.2.1'

def version():
    return __version__
