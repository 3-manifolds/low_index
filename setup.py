import sys
import shutil
from setuptools import setup, Command, Extension

sources = [
    "cpp_src/lowIndex.cpp",
    "cpp_src/words.cpp",
    "cpp_src/coveringSubgraph.cpp",
    "cpp_src/simsNode.cpp",
    "cpp_src/stackedSimsNode.cpp",
    "cpp_src/abstractSimsNode.cpp",
    "cpp_src/simsTreeBase.cpp",
    "cpp_src/simsTree.cpp",
    "cpp_src/simsTreeMultiThreaded.cpp",
    # The pybind11 headers are somewhat heavy - compiling all pieces
    # of the python wrapping in the same translation unit speeds up
    # compilation significantly.
    "cpp_src/wrapAll.cpp"
]

if sys.platform.startswith('win'):
    extra_compile_args = ['/Ox', '/std:c++14', '/MT']
else:
    extra_compile_args = ['-O3', '-std=c++11']

ext_modules = [
    Extension(
        name = 'low_index._low_index',
        sources = sources,
        extra_compile_args = extra_compile_args)
]

class Clean(Command):
    user_options = []
    def initialize_options(self):
        pass
    def finalize_options(self):
        pass
    def run(self):
        for dir in ['build', 'dist', 'low_index.egg-info']:
            shutil.rmtree(dir, ignore_errors=True)
setup(
    packages = ['low_index'],
    package_dir = {'low_index':'python_src'}, 
    ext_modules=ext_modules,
    cmdclass = {'clean':Clean}
)
