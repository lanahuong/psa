import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


def normalize(map, field):
    norm = np.sqrt(np.sum(map*map))
    mapSizeX , mapSizeY = field["dimensions"]
    nbMesh = field["nbSegments"]
    dx = mapSizeX * 2 / nbMesh
    dy = mapSizeY * 2 / nbMesh
    ds = np.sqrt(dx*dy)
    return map/ (norm*ds)

def createMapGaussian(wave , field):
    coordX , coordY = wave["Coord"]
    width = wave["Width"]
    mapSizeX , mapSizeY = field["dimensions"]
    nbMesh = field["nbSegments"]
    x , y = np.meshgrid(np.linspace(-mapSizeX,mapSizeX,nbMesh), np.linspace(-mapSizeY,mapSizeY,nbMesh))
    g = np.exp(-( ((x - coordX)**2 + (y - coordY)**2) / ( width**2 ) ) )
    return normalize(g, field)



def afficheInit(field, init):
    X , Y = field["dimensions"]
    nbMesh = field["nbSegments"]
    x = np.linspace(-X,X,nbMesh)
    y = np.linspace(-Y,Y,nbMesh)

    hf = plt.figure()
    ha = hf.add_subplot(111, projection='3d')

    X, Y = np.meshgrid(x, y)  # `plot_surface` expects `x` and `y` data to be 2D
    ha.plot_surface(X, Y, init)

    plt.show()