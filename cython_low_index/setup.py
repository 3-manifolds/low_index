from setuptools import setup, Command, Extension
import os
import sys
import re
import shutil
from glob import glob

class Clean(Command):
    user_options = []
    def initialize_options(self):
        pass 
    def finalize_options(self):
        pass
    def run(self):
        for dir in ['build', 'dist', 'low_index.egg-info']:
            shutil.rmtree(dir, ignore_errors=True)
        for file in glob('cython_src/*.c'):
            os.remove(file)

if sys.platform.startswith('win'):
    extra_compile_args = ['/Ox']
else:
    extra_compile_args = ['-O3']

low_index = Extension(
    name = 'low_index._low_index',
    sources = ['cython_src/_low_index.pyx'],
    extra_compile_args = extra_compile_args
)

setup(ext_modules = [low_index], cmdclass = {'clean':Clean,})
