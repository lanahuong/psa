import numpy
from setuptools import setup, Extension

from distutils.sysconfig import get_python_inc

module1 = Extension('_flus',
                    include_dirs = ['./include/armanpy/', numpy.get_include(), get_python_inc()],
                    libraries = ['m', 'z', 'armadillo'],
                    extra_compile_args=["-fopenmp", "-Ofast", "-march=native", "-mtune=native"],
                    extra_link_args=['-lomp'],
                    sources = ['flus.i', '../src/flus/SchemeFTCS.cpp', '../src/flus/SchemeBTCS.cpp', '../src/flus/SchemeCTCS.cpp', '../src/flus/Solver.cpp'],
                    swig_opts = ["-c++", "-Wall", "-I.", "-I./include/armanpy/", "-threads"])

setup (name = 'package_test',
       py_modules = ['flus'],
       version = '1.0',
       description = 'This is a test package',
       ext_modules = [module1])

