from tkinter import ttk
from PIL import Image
import numpy as np
import tkinter as tk
import cv2

from ..tk_components.custom_image_frame import CustomImageFrame

from ._OperationFactory import register_page

from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

def getDistribution(image_arr: np.ndarray, maxVal: int):
    histogram, _ = np.histogram(image_arr.flatten(), bins=maxVal + 1, range=(0, maxVal + 1))
    return histogram

def getDistributionOfImage(image: Image.Image, maxVal: int):
    image_arr = np.asarray(image.convert("RGB"), dtype=np.uint8)
    return getDistribution(image_arr, maxVal)

def histogramEqualization(PILImage: Image.Image, maxVal: int):
    image_arr = np.asarray(PILImage.convert("RGB"), dtype=np.uint8)
    EPSILON = 0.0001
    histogram = getDistribution(image_arr, maxVal)
    cdf = histogram.cumsum()
    p_k = (maxVal - 1) / image_arr.size
    s_k = p_k * cdf.astype(np.float32)
    
    s_k = np.rint(s_k).astype(np.uint8)
    transformed_arr = s_k[image_arr]

    result = np.clip(transformed_arr, 0, maxVal)

    return Image.fromarray(result.astype(np.int8), "RGB")

def opencvHistogramEqualization(PILImage: Image.Image, maxVal: int):
    image_arr = np.array(PILImage)
    equalized_arr_r = cv2.equalizeHist(image_arr[:, :, 0])
    equalized_arr_g = cv2.equalizeHist(image_arr[:, :, 1])
    equalized_arr_b = cv2.equalizeHist(image_arr[:, :, 2])

    merged = cv2.merge([equalized_arr_r, equalized_arr_g, equalized_arr_b])

    result = np.clip(merged, 0, maxVal)

    return Image.fromarray(result.astype(np.uint8), "RGB")

def setupPlotAxes(ax, title: str):
    ax.set_title(title, fontsize=4)
    ax.tick_params(axis='x', labelbottom=False)
    ax.tick_params(axis='y', labelleft=False)

@register_page("Histogram Equalization")
def histogramEqualizationFrame(parent: tk.Misc):
    histogramFrame = ttk.Frame(parent)

    histogramFrame.grid_rowconfigure(0, weight=1)
    histogramFrame.grid_columnconfigure(0, weight=1)
    histogramFrame.grid_columnconfigure(1, weight=3)

    openCVFrame = ttk.Frame(histogramFrame)
    openCVFrame.grid(row=0, column=0, sticky="nsew")
    openCVLabelFrame = ttk.Label(openCVFrame, text="OpenCV Histogram Equalization Result")
    openCVLabelFrame.pack(fill="x", side=tk.TOP)
    openCVImageFrame = CustomImageFrame(openCVFrame)
    openCVImageFrame.pack(fill="both", expand=True, side=tk.BOTTOM, padx=5)


    fig = Figure(figsize=(6, 2), dpi=100)

    (ax1, ax2, ax3) = fig.subplots(nrows=1, ncols=3, sharex=True)
    setupPlotAxes(ax1, "Original Image Histogram")
    setupPlotAxes(ax2, "Equalized Image by OpenCV Histogram")
    setupPlotAxes(ax3, "Equalized Image by Me Histogram")

    fig.subplots_adjust(hspace=0.3)
    canvas = FigureCanvasTkAgg(fig, master=histogramFrame)
    canvas_widget = canvas.get_tk_widget()
    canvas_widget.grid(row=0, column=1, sticky="nsew")

    def onApply(image: Image.Image, maxVal: int):
        converted_image = histogramEqualization(image, maxVal)
        opencv_converted_image = opencvHistogramEqualization(image, maxVal)

        ax1.clear()
        ax2.clear()
        ax3.clear()

        original_hist = getDistributionOfImage(image, maxVal)
        equalized_hist = getDistributionOfImage(converted_image, maxVal)
        opencv_equalized_hist = getDistributionOfImage(opencv_converted_image, maxVal)

        ax1.bar(range(256), original_hist, color="gray", alpha=0.5)
        ax2.bar(range(256), opencv_equalized_hist, color="blue", alpha=0.5)
        ax3.bar(range(256), equalized_hist, color="red", alpha=0.5)

        setupPlotAxes(ax1, "Original Image")
        setupPlotAxes(ax2, "Equalized Image by OpenCV")
        setupPlotAxes(ax3, "Equalized Image by Me")

        canvas.draw()
        openCVImageFrame.setCopyOfImage(opencv_converted_image, maxVal)
        return histogramEqualization(image, maxVal)

    return (histogramFrame, onApply)
