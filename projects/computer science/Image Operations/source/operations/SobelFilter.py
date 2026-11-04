from PIL import Image
import numpy as np
import tkinter as tk
from tkinter import ttk

from ._OperationFactory import register_page


def createSobelKernel(mode="x"):
    # Sobel kernels for edge detection
    sobel_x = np.array([[-1, 0, 1],
                        [-2, 0, 2],
                        [-1, 0, 1]])

    sobel_y = np.array([[1, 2, 1],
                        [0, 0, 0],
                        [-1, -2, -1]])
    if mode == "x":
        return sobel_x
    else:
        return sobel_y

def SobelFilter(PILImage: Image.Image, maxVal: int, scale_factor: float, mode="x"):
    image_arr = np.asarray(PILImage.convert("RGB")).astype(np.float32)
    kernel = createSobelKernel(mode)
    kernel = kernel[:, :, np.newaxis]  # Expand dims for broadcasting
    padding_size = 1
    padded_image = np.pad(image_arr, ((padding_size, padding_size), (padding_size, padding_size), (0, 0)), mode='reflect')
    filtered_image = np.zeros_like(image_arr)

    for i in range(image_arr.shape[0]):
        for j in range(image_arr.shape[1]):
            filtered_image[i, j, :] = np.sum(kernel * padded_image[i : i + 3, j : j + 3], axis=(0, 1)) # Padded image slicing

    scaled_image = filtered_image * scale_factor

    scaled_image += maxVal / 2  # Center the values around mid-gray

    scaled_image = np.clip(scaled_image, 0, maxVal)

    return Image.fromarray(scaled_image.astype(np.uint8), "RGB")

def sobelFilterBoth(PILImage: Image.Image, maxVal: int, scale_factor: float):
    image_arr = np.asarray(PILImage.convert("RGB")).astype(np.float32)
    kernel_x = createSobelKernel("x")
    kernel_y = createSobelKernel("y")
    kernel_x = kernel_x[:, :, np.newaxis]  # Expand dims for broadcasting
    kernel_y = kernel_y[:, :, np.newaxis]  # Expand dims for broadcasting
    padding_size = 1
    padded_image = np.pad(image_arr, ((padding_size, padding_size), (padding_size, padding_size), (0, 0)), mode='reflect')
    filtered_image_x = np.zeros_like(image_arr)
    filtered_image_y = np.zeros_like(image_arr)

    for i in range(image_arr.shape[0]):
        for j in range(image_arr.shape[1]):
            filtered_image_x[i, j, :] = np.sum(kernel_x * padded_image[i : i + 3, j : j + 3], axis=(0, 1)) # Padded image slicing
            filtered_image_y[i, j, :] = np.sum(kernel_y * padded_image[i : i + 3, j : j + 3], axis=(0, 1)) # Padded image slicing

    magnitude_image = np.sqrt(filtered_image_x**2 + filtered_image_y**2) # Compute the gradient magnitude

    # filtered_image_x = filtered_image_x + maxVal / 2  # Center the values around mid-gray
    # filtered_image_y = filtered_image_y + maxVal / 2  # Center the values around mid-gray
    # magnitude_image = filtered_image_x + filtered_image_y

    scaled_image = magnitude_image * scale_factor

    scaled_image = np.clip(scaled_image, 0, maxVal)

    return Image.fromarray(scaled_image.astype(np.uint8), "RGB")


@register_page("Sobel Filter")
def sobelFilterFrame(parent: tk.Misc):
    sobelFrame = ttk.Frame(parent)
    
    mode_selection_frame = ttk.Frame(sobelFrame)
    mode_selection_frame.pack(side=tk.TOP, expand=True, fill="both", pady=(10,0))
    mode_selected_label = ttk.Label(mode_selection_frame, text="Select Mode")
    mode_selected_label.pack(side=tk.LEFT, padx=(0, 10))
    selected_mode = tk.StringVar(value="x")
    mode_x_radio = ttk.Radiobutton(mode_selection_frame, text="X Direction", variable=selected_mode, value="x")
    mode_y_radio = ttk.Radiobutton(mode_selection_frame, text="Y Direction", variable=selected_mode, value="y")
    mode_both_radio = ttk.Radiobutton(mode_selection_frame, text="Both Directions", variable=selected_mode, value="both")
    mode_x_radio.pack(side=tk.LEFT)
    mode_y_radio.pack(side=tk.LEFT)
    mode_both_radio.pack(side=tk.LEFT)

    scale_factor_val = tk.DoubleVar(value=1.0)


    scale_factor_val_label = ttk.Label(sobelFrame, text="Scaling Factor")
    scale_factor_slider = tk.Scale(
        sobelFrame,
        from_=1.0,
        to=5.0,
        orient="horizontal",
        resolution=0.1,
        variable=scale_factor_val,
    )
    
    scale_factor_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    scale_factor_slider.pack(side=tk.TOP, expand=True, fill="both")




    def onApply(image: Image.Image, maxVal: int):
        if selected_mode.get() == "both":
            return sobelFilterBoth(image, maxVal, scale_factor_val.get())
        else:
            return SobelFilter(image, maxVal, scale_factor_val.get(), selected_mode.get())

    return (sobelFrame, onApply)
