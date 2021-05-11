import numpy
from setuptools import setup, Extension

from distutils.sysconfig import get_python_inc

module1 = Extension(
    "_flus",
    include_dirs=["./include/armanpy/", numpy.get_include(), get_python_inc()],
    libraries=["m", "z", "armadillo"],
    extra_compile_args=["-Ofast", "-march=native", "-mtune=native",
                        "-Wno-delete-non-virtual-dtor", "-Wno-missing-field-initializers", "-Wno-deprecated-declarations"], #silencing swig warnings
    extra_link_args=["-larmadillo"],
    sources=[
        "flus.i",
        "../src/flus/SchemeFTCS.cpp",
        "../src/flus/SchemeBTCS.cpp",
        "../src/flus/SchemeCTCS.cpp",
        "../src/flus/Solver.cpp",
    ],
    swig_opts=["-c++", "-Wall", "-Wextra", "-I.",
               "-I./include/armanpy/", "-threads"],
)

setup(
    name="package_test",
    py_modules=["flus"],
    version="1.0",
    description="This is a test package",
    ext_modules=[module1],
)
