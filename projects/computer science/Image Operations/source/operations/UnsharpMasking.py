from PIL import Image
import numpy as np
import tkinter as tk
from tkinter import ttk

from ._OperationFactory import register_page
from .GausianSmoothing import gaussianSmoothing



def unsharpMasking(PILImage: Image.Image, maxVal: int, kernel_size: int, std_dev: float, scaling_factor: float):
    # Create Gaussian kernel
    image_arr = np.asarray(PILImage.convert("RGB")).astype(np.float32)
    blurred_image = gaussianSmoothing(PILImage, maxVal, kernel_size, std_dev)
    blurred_arr = np.asarray(blurred_image.convert("RGB")).astype(np.float32)

    unsharp_image = image_arr + ((image_arr - blurred_arr) * scaling_factor)

    unsharp_image = np.clip(unsharp_image, 0, maxVal)

    return Image.fromarray(unsharp_image.astype(np.uint8), "RGB")


@register_page("Unsharp Masking")
def unsharpMaskingFrame(parent: tk.Misc):
    unsharpFrame = ttk.Frame(parent)


    kernel_size_val = tk.IntVar(value=3)
    std_dev_val = tk.DoubleVar(value=10.0)
    scaling_factor_val = tk.DoubleVar(value=1.0)

    kernel_size_val_label = ttk.Label(unsharpFrame, text="Kernel Size")
    std_dev_val_label = ttk.Label(unsharpFrame, text="Standard Deviation")
    scaling_factor_val_label = ttk.Label(unsharpFrame, text="Scaling Factor K")

    kernel_size_slider = tk.Scale(
        unsharpFrame,
        from_=1,
        to=99,
        orient="horizontal",
        resolution=2,
        variable=kernel_size_val,
    )
    std_dev_slider = tk.Scale(
        unsharpFrame,
        from_=1.0,
        to=100.0,
        orient="horizontal",
        resolution=0.1,
        variable=std_dev_val,
    )
    scaling_factor_slider = tk.Scale(
        unsharpFrame,
        from_=0.1,
        to=10.0,
        orient="horizontal",
        resolution=0.1,
        variable=scaling_factor_val,
    )

    kernel_size_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    kernel_size_slider.pack(side=tk.TOP, expand=True, fill="both")
    std_dev_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    std_dev_slider.pack(side=tk.TOP, expand=True, fill="both")
    scaling_factor_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    scaling_factor_slider.pack(side=tk.TOP, expand=True, fill="both")



    def onApply(image: Image.Image, maxVal: int):
        return unsharpMasking(image, maxVal, kernel_size_val.get(), std_dev_val.get(), scaling_factor_val.get())

    return (unsharpFrame, onApply)
