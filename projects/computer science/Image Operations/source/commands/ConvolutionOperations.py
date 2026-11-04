from pathlib import Path
from ._CommandFactory import register_command
from PIL import Image

from ..operations.HistogramEqualization import histogramEqualization
from ..operations.GausianSmoothing import gaussianSmoothing
from ..operations.SobelFilter import sobelFilterBoth
from ..ppm.Parsing import readMaxval

from ._OperationHelpers import getTargetLocation
from . import _OperationHelpers as OpHelpers



@register_command("histeq")
def histEqImage(command: dict):
    path_input = Path(command["i"][0])

    image_path = getTargetLocation(path_input)

    if not image_path.is_file():
        return (False, f"Image not found: {image_path}")

    image = Image.open(image_path)
    max_val = readMaxval(image_path)

    Output = histogramEqualization(image, max_val)

    if OpHelpers.setEdittedImage is None:
        return (False, "UI is not ready yet")

    OpHelpers.setEdittedImage(Output)
    path_output = Path(command["o"][0])

    save_path = getTargetLocation(path_output)

    Output.save(save_path, format="ppm")

    return (True, f"Histogram Equalization done and saved in {save_path}")

@register_command("gblur")
def gaussianBlurImage(command: dict):
    path_input = Path(command["i"][0])

    n_value = int(command.get("n", [3])[0])  # Default kernel size 3
    sigma_value = float(command.get("sigma", [10.0])[0])  # Default sigma 10.0

    print(f"Gaussian Blur parameters: n={n_value}, sigma={sigma_value}")
    image_path = getTargetLocation(path_input)

    if not image_path.is_file():
        return (False, f"Image not found: {image_path}")

    image = Image.open(image_path)
    max_val = readMaxval(image_path)

    Output = gaussianSmoothing(image, max_val, n_value, sigma_value)

    if OpHelpers.setEdittedImage is None:
        return (False, "UI is not ready yet")

    OpHelpers.setEdittedImage(Output)
    path_output = Path(command["o"][0])

    save_path = getTargetLocation(path_output)

    Output.save(save_path, format="ppm")

    return (True, f"Gaussian Blur done and saved in {save_path}")

@register_command("sobel")
def sobelEdgeDetection(command: dict):
    path_input = Path(command["i"][0])

    image_path = getTargetLocation(path_input)

    if not image_path.is_file():
        return (False, f"Image not found: {image_path}")

    image = Image.open(image_path)
    max_val = readMaxval(image_path)

    Output = sobelFilterBoth(image, max_val, 1)

    if OpHelpers.setEdittedImage is None:
        return (False, "UI is not ready yet")

    OpHelpers.setEdittedImage(Output)
    path_output = Path(command["o"][0])

    save_path = getTargetLocation(path_output)

    Output.save(save_path, format="ppm")

    return (True, f"Sobel Edge Detection done and saved in {save_path}")