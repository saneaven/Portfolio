from tkinter import ttk
from PIL import Image
import numpy as np
import tkinter as tk

from ._OperationFactory import register_page


def logTransform(PILImage: Image.Image, maxVal: int, c: float, under: float):
    image_arr = np.asarray(PILImage.convert("RGB"), dtype=np.float32)
    EPSILON = 0.0001
    transformed_arr = np.log(1.0 + image_arr) / np.log(under) * c

    result = np.clip(transformed_arr, 0, maxVal)

    return Image.fromarray(result.astype(np.int8), "RGB")


def gammaTransform(PILImage: Image.Image, maxVal: int, c: float, gamma: float):
    EPSILON = 0.0001
    image_arr = np.asarray(PILImage.convert("RGB"), dtype=np.float32)
    transformed_arr = np.power(image_arr + EPSILON, gamma) * c

    result = np.clip(transformed_arr, 0, maxVal)

    return Image.fromarray(result.astype(np.int8), "RGB")


@register_page("Log Transform")
def logTransformFrame(parent: tk.Misc):
    logFrame = ttk.Frame(parent)

    c_val = tk.DoubleVar(value=1.0)
    under_val = tk.DoubleVar(value=10.0)

    c_val_label = ttk.Label(logFrame, text="Constant Value")

    under_val_label = ttk.Label(logFrame, text="Log Value")

    c_val_slider = tk.Scale(
        logFrame,
        from_=0,
        to=100,
        orient="horizontal",
        resolution=0.01,
        variable=c_val,
    )
    under_val_slider = tk.Scale(
        logFrame,
        from_=0,
        to=100,
        orient="horizontal",
        resolution=0.1,
        variable=under_val,
    )

    c_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    c_val_slider.pack(side=tk.TOP, expand=True, fill="both")
    under_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    under_val_slider.pack(side=tk.TOP, expand=True, fill="both")

    def onApply(image: Image.Image, maxVal: int):
        return logTransform(image, maxVal, c_val.get(), under_val.get())

    return (logFrame, onApply)


@register_page("Gamma Transform")
def gammaTransformFrame(parent: tk.Misc):
    gammaFrame = ttk.Frame(parent)

    c_val = tk.DoubleVar(value=1.0)
    gamma_val = tk.DoubleVar(value=1.0)

    c_val_label = ttk.Label(gammaFrame, text="Constant Value")

    gamma_val_label = ttk.Label(gammaFrame, text="Gamma Value")

    c_val_slider = tk.Scale(
        gammaFrame,
        from_=0,
        to=10,
        orient="horizontal",
        resolution=0.1,
        variable=c_val,
    )
    gamma_val_slider = tk.Scale(
        gammaFrame,
        from_=0,
        to=3,
        orient="horizontal",
        resolution=0.01,
        variable=gamma_val,
    )

    c_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    c_val_slider.pack(side=tk.TOP, expand=True, fill="both")
    gamma_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    gamma_val_slider.pack(side=tk.TOP, expand=True, fill="both")

    def onApply(image: Image.Image, maxVal: int):
        return gammaTransform(image, maxVal, c_val.get(), gamma_val.get())

    return (gammaFrame, onApply)
