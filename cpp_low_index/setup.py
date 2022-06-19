import sys

from pybind11 import get_cmake_dir
# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension
from setuptools import setup

__version__ = "0.0.1"


ext_modules = [
    Pybind11Extension("cpp_low_index",
        [
            "cpp_src/coveringSubgraph.cpp",
            "cpp_src/simsNode.cpp",
            "cpp_src/stackedSimsNode.cpp",
            "cpp_src/abstractSimsNode.cpp",
            "cpp_src/simsTree.cpp",
            "cpp_src/wrapCoveringSubgraph.cpp",
            "cpp_src/wrapSimsNode.cpp",
            "cpp_src/wrapAbstractSimsNode.cpp",
            "cpp_src/wrapSimsTree.cpp",
            "cpp_src/wrapModule.cpp"
         ],
        cxx_std=11, # C++17 implies macOS 10.14
        # Example: passing in the version to the compiled code
        define_macros = [('VERSION_INFO', __version__)],
        ),
]


setup(
    name="cpp_low_index",
    version=__version__,
    author="MMN",
    author_email="",
    url="",
    description="A test project using pybind11",
    long_description="",
    ext_modules=ext_modules,
    extras_require={"test": "pytest"},
    # Currently, build_ext only provides an optional "highest supported C++
    # level" feature, but in the future it may provide more features.
    zip_safe=False,
    python_requires=">=3.6",
)
