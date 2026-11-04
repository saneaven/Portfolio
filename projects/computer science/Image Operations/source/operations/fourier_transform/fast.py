from PIL import Image
import numpy as np
import tkinter as tk
from tkinter import ttk
import time

from ...tk_components.custom_image_frame import CustomImageFrame

from ..helpers.helper import colorPalette

from .._OperationFactory import register_page

def bitReverse(n, bit_length):
    result = 0
    for _ in range(bit_length):
        result = result << 1
        if n & 1:
            result += 1
        n = n >> 1
    return result

def applyCentering(arr):
    # Apply centering by multiplying with (-1)^(x+y)
    H, W = arr.shape[:2]
    y, x = np.indices((H, W))
    mask = ((-1) ** (x + y))[:, :, np.newaxis]
    return arr * mask

def transform2D(arr, core_func):
    # Apply 1D transform along each axis
    arr = np.transpose(arr, (0, 2, 1))
    arr = core_func(arr)
    arr = np.transpose(arr, (1, 2, 0))
    arr = core_func(arr)
    arr = np.transpose(arr, (2, 1, 0))
    return arr

def fastFourier1D(fourier_arr):
    # Input may not be in (N, Length) form but (H, 3, W) etc., so reshape accordingly
    original_shape = fourier_arr.shape
    N = original_shape[-1]
    
    # Flatten 2D array into (Lines, N) form
    flat_arr = fourier_arr.reshape(-1, N)
    num_lines = flat_arr.shape[0]
    
    bit_len = (N - 1).bit_length()

    for i in range(num_lines):
        line = flat_arr[i]
        
        # Perform bit-reversal permutation
        for j in range(N):
            k = bitReverse(j, bit_len)
            if j < k:
                line[j], line[k] = line[k], line[j]

        # Butterfly operation
        step = 1
        while step < N:
            # Precompute W values
            k_vals = np.arange(step)
            angles = -1j * np.pi * k_vals / step
            W_vals = np.exp(angles)

            for start_idx in range(0, N, step * 2):
                for k in range(step):
                    W = W_vals[k]
                    
                    idx_even = start_idx + k
                    idx_odd = start_idx + k + step
                    
                    even_val = line[idx_even]
                    odd_val = line[idx_odd]
                    
                    T = W * odd_val
                    
                    line[idx_even] = even_val + T
                    line[idx_odd] = even_val - T
            
            step *= 2
        
    return flat_arr.reshape(original_shape)

def fastFourierTransform(PILImage: Image.Image, mode:str="log", bins:None|int=None, compression_range:None|tuple[int, int]=None):
    img_rgb = PILImage.convert("RGB")
    image_arr = np.asarray(img_rgb).astype(np.complex64)
    image_arr = applyCentering(image_arr)
    
    H, W, C = image_arr.shape
    new_H = 1 << (H - 1).bit_length()
    new_W = 1 << (W - 1).bit_length()
    
    padded_arr = np.zeros((new_H, new_W, C), dtype=image_arr.dtype)
    padded_arr[:H, :W, :] = image_arr
    
    fourier_arr = transform2D(padded_arr, fastFourier1D)

    if compression_range is not None:
        min_freq, max_freq = compression_range
        H, W = fourier_arr.shape[:2]
        
        # Get center coordinates
        center_u = H // 2
        center_v = W // 2
        
        # Calculate distance from center
        u_indices, v_indices = np.indices((H, W))
        freq_radius = np.sqrt((u_indices - center_u)**2 + (v_indices - center_v)**2)
        
        # Create mask for frequencies to keep
        mask = (freq_radius >= min_freq) & (freq_radius <= max_freq)
        mask = mask[:, :, np.newaxis] 
        
        fourier_arr = fourier_arr * mask
    
    magnitude_arr = np.abs(fourier_arr)

    if mode == "color":
        if bins is None:
            raise ValueError("bins must be provided for color mode")
        
        magnitude_mean = np.mean(magnitude_arr, axis=2)

        colors = colorPalette(n=bins + 1)
        min_magnitude = magnitude_mean.min()
        max_magnitude = magnitude_mean.max()
        bin_edges = np.logspace(np.log10(min_magnitude), np.log10(max_magnitude), colors.shape[0])
        result_arr = np.zeros((new_H, new_W, 3), dtype=np.uint8)
        for i in range(bins):
            mask = (magnitude_mean >= bin_edges[i]) & (magnitude_mean < bin_edges[i + 1])
            result_arr[mask] = colors[i + 1]  # Assign color based on bin
    else:
        result_arr = np.log(1 + magnitude_arr)
        
        min_val = result_arr.min()
        max_val = result_arr.max()
        
        if max_val - min_val > 1e-6:
            result_arr = 255 * (result_arr - min_val) / (max_val - min_val)
        
    return Image.fromarray(result_arr.astype(np.uint8), "RGB"), fourier_arr

def inverseFastFourierTransform(fourier_arr: np.ndarray, original_shape=None):
    # Get conjugate for inverse transform
    input_conj = np.conj(fourier_arr)

    # Reuse forward transform on conjugated input
    transformed_conj = transform2D(input_conj, fastFourier1D)
    
    N = fourier_arr.shape[0] * fourier_arr.shape[1]
    recovered_arr = np.conj(transformed_conj) / N
    
    recovered_image = np.real(recovered_arr)
    recovered_image = applyCentering(recovered_image)
    
    if original_shape:
        org_H, org_W = original_shape[:2]
        recovered_image = recovered_image[:org_H, :org_W, :]
        
    recovered_image = np.clip(recovered_image, 0, 255).astype(np.uint8)
    
    return Image.fromarray(recovered_image, "RGB")

@register_page("Fast Fourier Transform")
def FastFourierFrame(parent: tk.Misc):
    fastFourierFrame = ttk.Frame(parent)
    
    setting_frame = ttk.Frame(fastFourierFrame)
    setting_frame.pack(side=tk.LEFT, fill=tk.Y, padx=10, pady=10)

    time_label = ttk.Label(setting_frame, text="Time taken: N/A")
    time_label.pack(side=tk.TOP, pady=(0, 10))

    mode_var = tk.StringVar(value="log")

    bins_frame = ttk.Frame(setting_frame)
    ttk.Label(bins_frame, text="Slicing Bins:").pack(side=tk.LEFT)
    bins_entry = ttk.Entry(bins_frame, width=5)
    bins_entry.insert(0, "10")
    bins_entry.pack(side=tk.LEFT, padx=5)

    def update_ui_visibility():
        mode = mode_var.get()

        bins_frame.pack_forget()
        comp_frame.pack_forget()
        
        if mode == "color":
            bins_frame.pack(side=tk.TOP, pady=5, anchor="w")
        elif mode == "compression":
            comp_frame.pack(side=tk.TOP, pady=5, fill=tk.X)

    ttk.Label(setting_frame, text="Display Mode:").pack(side=tk.TOP, anchor="w")
    
    rb_log = ttk.Radiobutton(
        setting_frame, 
        text="Grayscale (Log)", 
        variable=mode_var, 
        value="log",
        command=update_ui_visibility
    )
    rb_log.pack(side=tk.TOP, anchor="w", pady=2)

    rb_color = ttk.Radiobutton(
        setting_frame, 
        text="Color (Slicing)", 
        variable=mode_var, 
        value="color",
        command=update_ui_visibility
    )
    rb_color.pack(side=tk.TOP, anchor="w", pady=2)

    rb_comp = ttk.Radiobutton(
        setting_frame, 
        text="Compression", 
        variable=mode_var, 
        value="compression",
        command=update_ui_visibility
    )
    rb_comp.pack(side=tk.TOP, anchor="w", pady=2)

    comp_frame = ttk.Frame(setting_frame)
    
    ttk.Label(comp_frame, text="Min Radius (Low Cut):").pack(side=tk.TOP, anchor="w", pady=(5, 0))
    low_cut_var = tk.IntVar(value=0)
    scale_low = tk.Scale(comp_frame, from_=0, to=200, orient=tk.HORIZONTAL, variable=low_cut_var)
    scale_low.pack(side=tk.TOP, fill=tk.X)

    ttk.Label(comp_frame, text="Max Radius (High Cut):").pack(side=tk.TOP, anchor="w", pady=(5, 0))
    high_cut_var = tk.IntVar(value=200)
    scale_high = tk.Scale(comp_frame, from_=0, to=400, orient=tk.HORIZONTAL, variable=high_cut_var)
    scale_high.pack(side=tk.TOP, fill=tk.X)

    update_ui_visibility()


    reconstructedImageFrame = CustomImageFrame(fastFourierFrame)
    reconstructedImageFrame.pack(fill="both", expand=True, side=tk.RIGHT, padx=5)


    def onApply(image: Image.Image, maxVal: int):
        start = time.perf_counter()
        result_image, fourier_arr = fastFourierTransform(image, 
                                                         mode=mode_var.get(), 
                                                         bins=int(bins_entry.get()) if mode_var.get()=="color" else None,
                                                         compression_range=(low_cut_var.get(), high_cut_var.get()) if mode_var.get()=="compression" else None)
        end = time.perf_counter()
        time_label.config(text=f"Time taken: {end - start:.4f} seconds")
        reconstructed_image = inverseFastFourierTransform(fourier_arr, original_shape=np.asarray(image.convert("RGB")).shape)
        
        reconstructedImageFrame.setCopyOfImage(reconstructed_image, maxVal)
        
        return result_image
        
    return (fastFourierFrame, onApply)