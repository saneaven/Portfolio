from PIL import Image
import numpy as np

from .helpers.helper import linearInterpolation
from ..util_types import Vec2Int, Vec3Int, Vec2


def resize_bilinear(PILImage: Image.Image, width, height) -> Image.Image:
    original_width = PILImage.width
    original_height = PILImage.height

    img = PILImage.convert("RGB")
    original_arr = np.asarray(img, dtype=np.float32)

    # Generate Indices
    y_indices = np.arange(height)
    x_indices = np.arange(width)
    y_grid, x_grid = np.meshgrid(y_indices, x_indices, indexing="ij")

    # Get a scaled position of each pixel
    scaled_y = y_grid / height * original_height
    scaled_x = x_grid / width * original_width

    # indices for each scaled
    under_y = np.floor(scaled_y).astype(int)
    under_x = np.floor(scaled_x).astype(int)
    over_y = np.minimum(under_y + 1, original_height - 1)
    over_x = np.minimum(under_x + 1, original_width - 1)

    # Get positions for interpolation

    p00 = original_arr[under_y, under_x]
    p01 = original_arr[under_y, over_x]
    p10 = original_arr[over_y, under_x]
    p11 = original_arr[over_y, over_x]

    # UV position
    uv_y = scaled_y - under_y
    uv_x = scaled_x - under_x

    # Add dimention for match with p_nn
    uv_y = uv_y[..., np.newaxis]
    uv_x = uv_x[..., np.newaxis]

    # bilinear interpolation
    top = (1 - uv_x) * p00 + uv_x * p01
    bottom = (1 - uv_x) * p10 + uv_x * p11
    result = (1 - uv_y) * top + uv_y * bottom

    return Image.fromarray(result.astype(np.uint8), "RGB")


def resize_nearest(PILImage: Image.Image, width, height) -> Image.Image:
    original_width = PILImage.width
    original_height = PILImage.height

    img = PILImage.convert("RGB")
    original_arr = np.asarray(img, dtype=np.float32)

    # Generate Indices
    y_indices = np.arange(height)
    x_indices = np.arange(width)
    y_grid, x_grid = np.meshgrid(y_indices, x_indices, indexing="ij")

    # Get a scaled position of each pixel
    scaled_y = y_grid / height * original_height
    scaled_x = x_grid / width * original_width

    # indices for each scaled
    nearest_y = np.clip(np.round(scaled_y).astype(int), 0, original_height - 1)
    nearest_x = np.clip(np.round(scaled_x).astype(int), 0, original_width - 1)

    result = original_arr[nearest_y, nearest_x]

    return Image.fromarray(result.astype(np.uint8), "RGB")
