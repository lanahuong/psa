import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


def normalize(m, dimensions, shape):
    norm = np.sqrt(np.sum(m * m))
    mapSizeX, mapSizeY = dimensions
    nbMeshX, nbMeshY = shape
    dx = mapSizeX * 2 / nbMeshX
    dy = mapSizeY * 2 / nbMeshY
    ds = np.sqrt(dx * dy)
    return m / (norm * ds)


def createMapGaussian(wave, dimensions, shape):
    coordX, coordY = wave["Coord"]
    width = wave["Width"]
    kx, ky = wave["Speed"]
    mapSizeX, mapSizeY = dimensions
    nbMeshX, nbMeshY = shape
    x, y = np.meshgrid(
        np.linspace(-mapSizeX, mapSizeX, nbMeshX),
        np.linspace(-mapSizeY, mapSizeY, nbMeshY),
    )
    g = np.exp(-(((x - coordX) ** 2 + (y - coordY) ** 2) / (width ** 2)))
    return normalize(g, dimensions, shape) * np.exp(1j * (kx * x + ky * y))


# Obsolete
def afficheInit(field, init):
    X, Y = field["dimensions"]
    nbMesh = field["nbSegments"]
    x = np.linspace(-X, X, nbMesh)
    y = np.linspace(-Y, Y, nbMesh)

    hf = plt.figure()
    ha = hf.add_subplot(111, projection="3d")

    X, Y = np.meshgrid(x, y)
    ha.plot_surface(X, Y, init)

    plt.show()
