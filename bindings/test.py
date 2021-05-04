#!/usr/bin/env python3

import flus
import numpy as np
import matplotlib.pyplot as plt
from  matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D


size = 30

def normalize(m):
    norm = np.sqrt(np.sum(m * m))
    mapSizeX, mapSizeY = size,size
    nbMeshX, nbMeshY = size,size
    dx = mapSizeX * 2 / nbMeshX
    dy = mapSizeY * 2 / nbMeshY
    ds = np.sqrt(dx * dy)
    return m / (norm * ds)


def createMapGaussian():
    coordX = 10
    coordY = 10
    width = 10
    kx = 1
    ky = 1
    mapSizeX, mapSizeY = size,size
    nbMeshX, nbMeshY = size,size
    x, y = np.meshgrid(
        np.linspace(-mapSizeX, mapSizeX, nbMeshX),
        np.linspace(-mapSizeY, mapSizeY, nbMeshY),
    )
    g = np.exp(-(((x - coordX) ** 2 + (y - coordY) ** 2) / (width ** 2)))
    return normalize(g)*np.exp(1j * (kx * x + ky * y))

dx, dy, dt = 1e-5,1e-5,1e-5

solver = flus.SchemeCTCS(
        np.asfortranarray(createMapGaussian()),
        np.asfortranarray(np.zeros((size, size))),
        dx,
        dy,
        dt,
    )

mat = solver.get_phitdt()
mat = np.absolute(mat)

fig = plt.figure()
plot = plt.matshow(mat, fignum=0)

def init():
    plot.set_data(mat)
    return plot

def update(j):
    solver.step()
    mat = solver.get_phitdt()
    mat = np.absolute(mat)
    plot.set_data(mat)
    return [plot]

anim = FuncAnimation(fig, update, init_func = init, frames=100)
plt.show()




