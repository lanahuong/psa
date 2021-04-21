#!/usr/bin/env python3

import FlusSolver
import numpy as np

phi0 = np.array([[0.1, 0.2], [0.3, 0.4]], dtype=np.complex, order='F')
potential = np.array([[0.1, 0.2], [0.3, 0.4]], dtype=np.float64, order='F')
print("Initial object:")

mc = FlusSolver.SchemeCTCS(phi0, potential, 0.1, 0.1, 0.1)
mc.step()
print("Object in the C++ class (constructor with argument):")
print(mc.phitdt)
