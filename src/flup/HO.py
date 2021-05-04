import numpy as np
import math
import sys

def not_implemented():
    sys.exit("Not implemented")

def potentialHO(wave,dimensions,shape):
    mapSizeX, mapSizeY = dimensions
    nbMeshX, nbMeshY = shape
    x, y = np.meshgrid(
        np.linspace(-mapSizeX, mapSizeX, nbMeshX),
        np.linspace(-mapSizeY, mapSizeY, nbMeshY),
    )
    w = wave["Width"]
    m = 1.6749 * 1e-7
    map = 0.5 * m * (w*1e-20)**2 * (x**2+y**2)
    return map

def hermit( n , mesh ):
    H = np.ones([n+1,len(mesh)])
    if (n < 0) :
        return not_implemented()
    if (n == 0):
        return np.ones(len(mesh))
    if ( n == 1):
        return [np.ones(len(mesh)),2 * mesh]

    H[1] = 2 * mesh
    for i in range(2,n+1):
        H[i] = 2 * mesh * H[i-1] - 2 * (i-1) * H[i-2]
    return H

def schro(n , mesh, w):
    m = 1.6749 * 1e-7
    h_bar = 6.62607 * 1e16 / (2*np.pi)

    H = hermit(n,mesh*np.sqrt(m*w/h_bar))
    result = (1/np.sqrt(2**n * math.factorial(n))) * ((m*w)/(np.pih_bar))**0.25 * np.exp(-m*w*mesh*mesh/(2*h_bar)) * H[n]
    return result
    
def createHO(wave,dimensions,shape):
    mapSizeX, mapSizeY = dimensions
    nbMeshX, nbMeshY = shape
    x=np.linspace(-mapSizeX, mapSizeX, nbMeshX)
    y=np.linspace(-mapSizeY, mapSizeY, nbMeshY)
    harmoX , harmoY = wave["Harmo"]
    w = wave["Width"]
    Xval = schro(harmoX,x,w)
    Yval = schro(harmoY,y,w)

    map = np.outer(Xval,Yval)
    return map 

