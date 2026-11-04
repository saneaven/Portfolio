from PIL import Image
import numpy as np
from ...util_types import Vec2, Vec3
import matplotlib.colors as mcolors


def linearInterpolation(
    uv: Vec2, points
):  # uv: [y, x] points: (0,0), (0,1), (1,0), (1,1) ([y, x])
    x_y1 = (1 - uv[1]) * points[0] + uv[1] * points[1]
    x_y2 = (1 - uv[1]) * points[2] + uv[1] * points[3]

    x_y = (1 - uv[0]) * x_y1 + uv[0] * x_y2

    return x_y


class RootFind:
    def __init__(self):
        self.parent = [0]  # Empty value for match index

    def expand(self):
        self.parent.append(len(self.parent))

    def findRoot(self, x):
        if self.parent[x] != x:
            self.parent[x] = self.findRoot(self.parent[x])

        return self.parent[x]

    def union(self, x, y):
        rootX = self.findRoot(x)
        rootY = self.findRoot(y)

        if rootX != rootY:
            if rootX < rootY:
                self.parent[rootY] = rootX
            else:
                self.parent[rootX] = rootY


def colorPalette(n, s=0.70, v=0.92, v_osc=0.18, s_osc=0.10):

    phi = 137.50776405 / 360.0  # golden angle in turns
    colors = []
    for k in range(n):
        h = np.fmod(0.5 + k * phi, 1.0)
        v_k = np.clip(
            v + v_osc * ((-1) ** k) * 0.6 + v_osc * np.sin(2 * np.pi * k / 7.0) * 0.4,
            0.15,
            0.98,
        )
        s_k = np.clip(
            s
            + s_osc * ((-1) ** (k // 2)) * 0.7
            + s_osc * np.sin(2 * np.pi * k / 9.0) * 0.3,
            0.25,
            1.0,
        )
        rgb = mcolors.hsv_to_rgb([h, s_k, v_k])

        colors.append((rgb * 255).round().astype(int))

    return np.array(colors)


def isIndexIn2dArray(idx, array: np.ndarray):
    return (
        -1 < idx[0]
        and idx[0] < array.shape[0]
        and -1 < idx[1]
        and idx[1] < array.shape[1]
    )
