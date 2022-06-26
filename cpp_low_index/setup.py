import sys
from setuptools import setup, Extension

# Version 1.1 is used for the cython implemention
# Version 1.2 is used for the C++/pybind11 implementation
__version__ = "1.2"

sources = [
    "cpp_src/lowIndex.cpp",
    "cpp_src/words.cpp",
    "cpp_src/coveringSubgraph.cpp",
    "cpp_src/simsNode.cpp",
    "cpp_src/stackedSimsNode.cpp",
    "cpp_src/abstractSimsNode.cpp",
    "cpp_src/simsTree.cpp",
    "cpp_src/simsTreeMultiThreaded.cpp",
    # The pybind11 headers are somewhat heavy - compiling all pieces
    # of the python wrapping in the same translation unit speeds up
    # compilation significantly.
    "cpp_src/wrapAll.cpp"
]

if sys.platform.startswith('win'):
    extra_compile_args = ['/Ox', '/std:c++11']
else:
    extra_compile_args = ['-O3', '-std=c++11']

ext_modules = [
    Extension(
        # Rename to low_index eventually, replacing the cython implementation
        name = 'cpp_low_index._low_index',
        sources = sources,
        extra_compile_args = extra_compile_args)
]


setup(
    # Rename to low_index eventually, replacing the cython implementation
    name="cpp_low_index",
    version=__version__,
    author = 'Marc Culler and Nathan M. Dunfield and Matthias Goerner',
    author_email = 'culler@uic.edu, nathan@dunfield.info, enischte@gmail.com',
    url="",
    description="A C++ implementation of Sims LOW INDEX algorithm",
    long_description="",
    packages = ['cpp_low_index'],
    package_dir = {'cpp_low_index':'python_src'}, 
    ext_modules=ext_modules,
#    extras_require={"test": "pytest"},
    # Currently, build_ext only provides an optional "highest supported C++
    # level" feature, but in the future it may provide more features.
    zip_safe=False,
    python_requires=">=3.6"
)
