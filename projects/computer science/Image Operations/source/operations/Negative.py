from PIL import Image
import numpy as np
import tkinter as tk
from tkinter import ttk

from ._OperationFactory import register_page


def negative(PILImage: Image.Image, maxVal: int):

    image_arr = np.asarray(PILImage.convert("RGB"))

    negative_arr = np.full_like(image_arr, maxVal) - image_arr

    return Image.fromarray(negative_arr.astype(np.int8), "RGB")


@register_page("Negative")
def negativeFrame(parent: tk.Misc):
    emptyFrame = ttk.Frame(parent)

    label = ttk.Label(emptyFrame, text="Nothing to config")
    label.pack(padx=0, pady=(10, 0))

    def onApply(image: Image.Image, maxVal: int):
        return negative(image, maxVal)

    return (emptyFrame, onApply)
