from PIL import Image
import numpy as np
import tkinter as tk
from tkinter import ttk

from ._OperationFactory import register_page


def createGaussianKernel(kernel_size: int, std_dev: float):
    """Create a Gaussian kernel."""
    if kernel_size % 2 == 0:
        raise ValueError("kernel_size must be odd.")

    # Create a coordinate system with the center at (0,0)
    k = kernel_size // 2
    x, y = np.meshgrid(np.arange(-k, k + 1), np.arange(-k, k + 1))

    # 2D Gaussian function G(x, y) calculation
    # G(x, y) = (1 / (2 * pi * sigma^2)) * exp(-(x^2 + y^2) / (2 * sigma^2))
    exponent = -(x**2 + y**2) / (2 * std_dev**2)
    kernel = np.exp(exponent)

    # Normalize so that the sum of all elements is 1
    kernel = kernel / np.sum(kernel)
    
    return kernel

def gaussianSmoothing(PILImage: Image.Image, maxVal: int, kernel_size: int, std_dev: float):
    image_arr = np.asarray(PILImage.convert("RGB")).astype(np.float32)
    kernel = createGaussianKernel(kernel_size, std_dev)
    kernel = kernel[:, :, np.newaxis]  # Expand dims for broadcasting
    padding_size = kernel_size // 2
    padded_image = np.pad(image_arr, ((padding_size, padding_size), (padding_size, padding_size), (0, 0)), mode='reflect')
    smoothed_image = np.zeros_like(image_arr)

    for i in range(image_arr.shape[0]):
        for j in range(image_arr.shape[1]):
            smoothed_image[i, j, :] = np.sum(kernel * padded_image[i:i + kernel_size, j:j + kernel_size], axis=(0, 1))

    return Image.fromarray(smoothed_image.astype(np.uint8), "RGB")


@register_page("Gaussian Smoothing")
def gaussianSmoothingFrame(parent: tk.Misc):
    gaussianFrame = ttk.Frame(parent)


    kernel_size_val = tk.IntVar(value=3)
    std_dev_val = tk.DoubleVar(value=10.0)

    kernel_size_val_label = ttk.Label(gaussianFrame, text="Kernel Size")
    std_dev_val_label = ttk.Label(gaussianFrame, text="Standard Deviation")

    kernel_size_slider = tk.Scale(
        gaussianFrame,
        from_=1,
        to=99,
        orient="horizontal",
        resolution=2,
        variable=kernel_size_val,
    )
    std_dev_slider = tk.Scale(
        gaussianFrame,
        from_=1.0,
        to=100.0,
        orient="horizontal",
        resolution=0.1,
        variable=std_dev_val,
    )
    

    kernel_size_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    kernel_size_slider.pack(side=tk.TOP, expand=True, fill="both")
    std_dev_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    std_dev_slider.pack(side=tk.TOP, expand=True, fill="both")


    def onApply(image: Image.Image, maxVal: int):
        return gaussianSmoothing(image, maxVal, kernel_size_val.get(), std_dev_val.get())

    return (gaussianFrame, onApply)
