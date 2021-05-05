#!/usr/bin/env python3

import flus
import numpy as np
import matplotlib.pyplot as plt
from  matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d import Axes3D


from PIL import Image, ImageDraw

import sys
import os.path
import numpy as np

def load_image(iname):
    if not os.path.isfile(iname):
        sys.exit("The input field '{}' was not found. Aborting...".format(iname))
        return None
    M = np.array(Image.open(iname))
    return img_to_bw(M)

def img_to_bw(image):
    x_size = len(image[0])
    y_size = len(image)
    mat = []

    if image[0][0].size == 1: # already good picture with 1 channel
        return image

    for y in range(y_size): # there are more channel so we average them
        mat.append([])
        for x in range(x_size):
            r = int(image[y][x][0])  # image[x][y] = [r, v, b, alpha]
            v = int(image[y][x][1])
            b = int(image[y][x][2])
            m = (r + v + b) / (3*256)
            mat[y].append(m)
    return np.array(mat)
# EOF





def normalize(m, size):
    norm = np.sqrt(np.sum(m * m))
    mapSizeX, mapSizeY = size,size
    nbMeshX, nbMeshY = size,size
    dx = mapSizeX * 2 / nbMeshX
    dy = mapSizeY * 2 / nbMeshY
    ds = np.sqrt(dx * dy)
    return m / (norm * ds)


def createMapGaussian_a(size):
    coordX = 40
    coordY = 40
    width =  size/5
    kx = 10000 * size/5
    ky = 10000 * size/5
    mapSizeX, mapSizeY = size,size
    nbMeshX, nbMeshY = size,size
    x, y = np.meshgrid(
        np.linspace(-mapSizeX, mapSizeX, nbMeshX),
        np.linspace(-mapSizeY, mapSizeY, nbMeshY),
    )
    g = np.exp(-(((x - coordX) ** 2 + (y - coordY) ** 2) / (width ** 2)))
    return normalize(g,size)*np.exp(1j * (kx * x + ky * y))

def createMapGaussian_b(size):
    coordX = -40
    coordY = -40
    width =  size/5
    kx = 10000 * size/5
    ky = -10000 * size/5
    mapSizeX, mapSizeY = size,size
    nbMeshX, nbMeshY = size,size
    x, y = np.meshgrid(
        np.linspace(-mapSizeX, mapSizeX, nbMeshX),
        np.linspace(-mapSizeY, mapSizeY, nbMeshY),
    )
    g = np.exp(-(((x - coordX) ** 2 + (y - coordY) ** 2) / (width ** 2)))
    return normalize(g,size)*np.exp(1j * (kx * x + ky * y))



np.set_printoptions(threshold=sys.maxsize)
def run_anim(dx, dy, dt, temporal_supersampling, size, frames_count, out_name):
    mat = createMapGaussian_a(size) + createMapGaussian_b(size)

    potential = np.asfortranarray(np.zeros((size, size)))
    #potential = 1e40 * np.asfortranarray(load_image("maze_128.png"))
    potential =  10e25 *np.asfortranarray(load_image("maze_256.png"))
    #print(potential)

    solver = flus.SchemeCTCS( np.asfortranarray(mat), potential,dx,dy, dt/temporal_supersampling  )
    print(solver.phitdt_norm())
    mat = np.absolute(mat)
    fig = plt.figure()
    plot = plt.matshow(mat, fignum=0)
    def init():
        plot.set_data(mat)
        return plot
    def update(j):
        print(j)
        #print(solver.phitdt_norm())
        solver.step_n(temporal_supersampling)
        mat = (solver.get_phitdt_absolute())
        plot.set_data(mat)
        return [plot]
    anim = FuncAnimation(fig, update, init_func = init, frames=frames_count)
    anim.save(out_name, writer='imagemagick', fps=60)
    #plt.show()

run_anim(0.125,0.125, 1e2, 10, 256, 1000, "anim_passable.gif")
#run_anim(0.125,0.125, 1e3, 10, 128, 100, "anim_passable_maze2.gif")


