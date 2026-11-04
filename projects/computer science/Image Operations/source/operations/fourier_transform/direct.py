from PIL import Image
import numpy as np
import tkinter as tk
from tkinter import ttk
import time

from .fast import inverseFastFourierTransform
from ..helpers.helper import colorPalette
from ...tk_components.custom_image_frame import CustomImageFrame

from .._OperationFactory import register_page


def getFourierResultImage(fourier_arr: np.ndarray, mode: str, bins: int | None):
    H, W, _ = fourier_arr.shape
    magnitude = np.abs(fourier_arr)

    if mode == "color":
        if bins is None:
            raise ValueError("bins must be provided for color mode")
        
        magnitude_mean = np.mean(magnitude, axis=2)

        colors = colorPalette(n=bins + 1)
        min_magnitude = magnitude_mean.min()
        max_magnitude = magnitude_mean.max()
        
        bin_edges = np.logspace(np.log10(min_magnitude), np.log10(max_magnitude), colors.shape[0])
        result_arr = np.zeros((H, W, 3), dtype=np.uint8)
        for i in range(bins):
            mask = (magnitude_mean >= bin_edges[i]) & (magnitude_mean < bin_edges[i + 1])
            result_arr[mask] = colors[i + 1]  # Assign color based on bin
    else:

        result_arr = np.log(1 + magnitude)
        
        min_val = result_arr.min()
        max_val = result_arr.max()
        
        if max_val - min_val > 1e-6:
            result_arr = 255 * (result_arr - min_val) / (max_val - min_val)

    return result_arr


def directFourierTransform(PILImage: Image.Image, mode: str, bins: int | None = None):
    image_arr = np.asarray(PILImage.convert("RGB")).astype(np.float32)
    fourier_arr = np.zeros_like(image_arr, dtype=np.complex64)
    H, W, _ = image_arr.shape

    for x in range(H):
        for y in range(W):
                image_arr[x, y] *= (-1)**(x + y)  # Centering

    for u in range(H):
            for v in range(W):
                
                sum_val = 0
                
                for x in range(H):
                    for y in range(W):
                        # exp( -j * 2pi * (ux/H + vy/W) )
                        exponent = -2j * np.pi * ((u * x) / H + (v * y) / W)
                        W_uv = np.exp(exponent)
                        
                        sum_val += image_arr[x, y] * W_uv
                
                fourier_arr[u, v] = sum_val

    print("Direct transform done")

    result_arr = getFourierResultImage(fourier_arr, mode, bins)
        
    return Image.fromarray(result_arr.astype(np.uint8), "RGB"), fourier_arr

def separableFourierTransform(PILImage: Image.Image, mode: str, bins: int | None = None):
    image_arr = np.asarray(PILImage.convert("RGB")).astype(np.float32)
    fourier_arr = np.zeros_like(image_arr, dtype=np.complex64)
    H, W, _ = image_arr.shape

    for x in range(H):
        for y in range(W):
                image_arr[x, y] *= (-1)**(x + y)  # Centering

    # Row-wise transform
    row_transform = np.zeros_like(image_arr, dtype=np.complex64)
    for u in range(H):
        for y in range(W):
            sum_val = 0
            for x in range(H):
                exponent = -2j * np.pi * (u * x) / H
                W_ux = np.exp(exponent)
                sum_val += image_arr[x, y] * W_ux
            row_transform[u, y] = sum_val
    print("Row transform done")

    # Column-wise transform
    for u in range(H):
        for v in range(W):
            sum_val = 0
            for y in range(W):
                exponent = -2j * np.pi * (v * y) / W
                W_vy = np.exp(exponent)
                sum_val += row_transform[u, y] * W_vy
            fourier_arr[u, v] = sum_val
    print("Column transform done")

    result_arr = getFourierResultImage(fourier_arr, mode, bins)
        
    return Image.fromarray(result_arr.astype(np.uint8), "RGB"), fourier_arr


@register_page("Direct Fourier Transform")
def DirectfourierFrame(parent: tk.Misc):
    directFourierFrame = ttk.Frame(parent)
    setting_frame = ttk.Frame(directFourierFrame)
    setting_frame.pack(side=tk.LEFT, padx=0, pady=(20, 0))

    # Two options: Direct and Separable
    option_val = tk.StringVar(value="Direct")
    direct_radio = ttk.Radiobutton(
        setting_frame,
        text="Direct",
        variable=option_val,
        value="Direct",
    )
    separable_radio = ttk.Radiobutton(
        setting_frame,
        text="Separable",
        variable=option_val,
        value="Separable",
    )
    direct_radio.pack(side=tk.TOP, padx=0, pady=(10, 0))
    separable_radio.pack(side=tk.TOP, padx=0, pady=(10, 0))

    # Info Frame
    info_frame = ttk.Frame(setting_frame)
    info_frame.pack(side=tk.TOP, padx=0, pady=(20, 0))

    # Show time taken label
    time_label = ttk.Label(info_frame, text="Time taken: N/A" \
    "")
    time_label.pack(side=tk.TOP, padx=0, pady=(10, 0))

    mode_var = tk.StringVar(value="log")

    bins_frame = ttk.Frame(setting_frame)
    ttk.Label(bins_frame, text="Slicing Bins:").pack(side=tk.LEFT)
    bins_entry = ttk.Entry(bins_frame, width=5)
    bins_entry.insert(0, "10")
    bins_entry.pack(side=tk.LEFT, padx=5)

    def toggle_bins():
        if mode_var.get() == "color":
            bins_frame.pack(side=tk.TOP, pady=5, anchor="w")
        else:
            bins_frame.pack_forget()

    ttk.Label(setting_frame, text="Display Mode:").pack(side=tk.TOP, anchor="w")
    
    rb_log = ttk.Radiobutton(
        setting_frame, 
        text="Grayscale (Log)", 
        variable=mode_var, 
        value="log",
        command=toggle_bins
    )
    rb_log.pack(side=tk.TOP, anchor="w", pady=2)

    rb_color = ttk.Radiobutton(
        setting_frame, 
        text="Color (Slicing)", 
        variable=mode_var, 
        value="color",
        command=toggle_bins
    )
    rb_color.pack(side=tk.TOP, anchor="w", pady=2)

    toggle_bins()

    reconstructedImageFrame = CustomImageFrame(directFourierFrame)
    reconstructedImageFrame.pack(fill="both", expand=True, side=tk.RIGHT, padx=5)

    def onApply(image: Image.Image, maxVal: int):
        # Calculate time taken
        start = time.perf_counter()

        if option_val.get() == "Direct":
            result, fourier_arr = directFourierTransform(image, mode=mode_var.get(), bins=int(bins_entry.get()) if mode_var.get()=="color" else None)
        else:
            result, fourier_arr = separableFourierTransform(image, mode=mode_var.get(), bins=int(bins_entry.get()) if mode_var.get()=="color" else None)
        
        end = time.perf_counter()
        time_label.config(text=f"Time taken: {end - start:.4f} seconds")

        # I don't want to spend another long time here, so reuse the fast inverse
        inversed = inverseFastFourierTransform(fourier_arr, original_shape=np.asarray(image.convert("RGB")).shape)
        reconstructedImageFrame.setCopyOfImage(inversed, maxVal)

        return result
        
    return (directFourierFrame, onApply)