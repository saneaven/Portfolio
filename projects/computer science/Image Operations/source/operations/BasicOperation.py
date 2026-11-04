from pathlib import Path
from tkinter import ttk
import tkinter as tk
from PIL import Image
import numpy as np

from .Resize import resize_bilinear as resize
from ._OperationFactory import register_page
from ..tk_components.image_selector import addImageSelector


def basicMathmetics(
    PILImage1: Image.Image, PILImage2: Image.Image, maxVal: int, mode: str
):
    MAX_WIDTH = max(PILImage1.width, PILImage2.width)
    MAX_HEIGHT = max(PILImage1.height, PILImage2.height)

    resized_arr1 = np.asarray(
        resize(PILImage1, MAX_WIDTH, MAX_HEIGHT).convert("RGB"), dtype=np.float32
    )
    resized_arr2 = np.asarray(
        resize(PILImage2, MAX_WIDTH, MAX_HEIGHT).convert("RGB"), dtype=np.float32
    )

    calculated_arr = None

    match mode:
        case "add":
            calculated_arr = resized_arr1 + resized_arr2
        case "sub":
            calculated_arr = resized_arr1 - resized_arr2
        case "mul":
            calculated_arr = resized_arr1 * resized_arr2
        case _:
            raise ValueError("Not Basic Operation")

    result = np.clip(calculated_arr, 0, maxVal)

    return Image.fromarray(result.astype(np.uint8), "RGB")


@register_page("Basic Mathmetics")
def BasicMathmeticsFrame(parent: tk.Misc):
    basicMathMeticsFrame = ttk.Frame(parent)
    basicMathMeticsFrame.pack(side=tk.LEFT, fill="both", expand=True)

    modeFrame = ttk.Frame(basicMathMeticsFrame)
    selectSubImageFrame = ttk.Frame(basicMathMeticsFrame)
    modeFrame.pack(side=tk.LEFT)
    selectSubImageFrame.pack(side=tk.RIGHT, fill="both", expand=True)

    ## Inside mode frame
    methmetics_mode = tk.StringVar(value="add")
    ttk.Radiobutton(
        modeFrame, text="Addition", value="add", variable=methmetics_mode
    ).pack(side=tk.TOP)
    ttk.Radiobutton(
        modeFrame,
        text="Subtraction",
        value="sub",
        variable=methmetics_mode,
    ).pack(side=tk.TOP)
    ttk.Radiobutton(
        modeFrame, text="Product", value="mul", variable=methmetics_mode
    ).pack(side=tk.TOP)

    ## inside Select Sub Image Frame
    subImageView = addImageSelector(selectSubImageFrame)

    def OnApply(image: Image.Image, maxVal: int):
        edditedImage = basicMathmetics(
            image, subImageView.originalImage, maxVal=maxVal, mode=methmetics_mode.get()
        )
        return edditedImage

    return (basicMathMeticsFrame, OnApply)
