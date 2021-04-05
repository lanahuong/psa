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


