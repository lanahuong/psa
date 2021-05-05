#!/usr/bin/env python3

import flus
import numpy as np
import matplotlib.pyplot as plt
from  matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D

def normalize(m, size):
    norm = np.sqrt(np.sum(m * m))
    mapSizeX, mapSizeY = size,size
    nbMeshX, nbMeshY = size,size
    dx = mapSizeX * 2 / nbMeshX
    dy = mapSizeY * 2 / nbMeshY
    ds = np.sqrt(dx * dy)
    return m / (norm * ds)


def createMapGaussian(size):
    coordX = 20
    coordY = 20
    width = 2 * size/5
    kx = -100 * size/5
    ky = 100 * size/5
    mapSizeX, mapSizeY = size,size
    nbMeshX, nbMeshY = size,size
    x, y = np.meshgrid(
        np.linspace(-mapSizeX, mapSizeX, nbMeshX),
        np.linspace(-mapSizeY, mapSizeY, nbMeshY),
    )
    g = np.exp(-(((x - coordX) ** 2 + (y - coordY) ** 2) / (width ** 2)))
    return normalize(g,size)*np.exp(1j * (kx * x + ky * y))



def run_anim(dx, dy, dt, temporal_supersampling, size, frames_count, out_name):
    mat = createMapGaussian(size)
    solver = flus.SchemeCTCS( np.asfortranarray(mat), np.asfortranarray(np.zeros((size, size))),dx,dy, dt/temporal_supersampling  )
    mat = np.absolute(mat)
    fig = plt.figure()
    plot = plt.matshow(mat, fignum=0)
    def init():
        plot.set_data(mat)
        return plot
    def update(j):
        print(j)
        solver.step_n(temporal_supersampling)
        mat = np.absolute(solver.get_phitdt())
        plot.set_data(mat)
        return [plot]
    anim = FuncAnimation(fig, update, init_func = init, frames=frames_count)
    anim.save(out_name, writer='imagemagick', fps=60)
    #plt.show()

#run_anim(1e-4,1e-4,1e-3, 10, 50, 100, "anim_cassée3.gif")
run_anim(0.1,0.1, 1e18, 5, 100, 130, "anim_passable.gif")
