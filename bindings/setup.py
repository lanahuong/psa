from setuptools import setup, Extension

module1 = Extension('_FlusSolver',
                    include_dirs=['./include/armanpy/'],
                    libraries=['m', 'z', 'armadillo'],
                    sources=['FlusSolver.i', '../src/flus/SchemeFTCS.cpp', '../src/flus/SchemeBTCS.cpp', '../src/flus/SchemeCTCS.cpp',
                             '../src/flus/Solver.cpp'],
                    swig_opts=["-c++", "-Wall", "-I.", "-I./include/armanpy/"])

setup(name='package_test',
      py_modules=['FlusSolver'],
      version='1.0',
      description='PSA Solver',
      ext_modules=[module1])
