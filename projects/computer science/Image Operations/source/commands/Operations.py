from pathlib import Path
from ._CommandFactory import register_command
from PIL import Image

from ..operations.BasicOperation import basicMathmetics
from ..operations.Negative import negative
from ..operations.Transforms import logTransform, gammaTransform
from ..ppm.Parsing import readMaxval

from ._OperationHelpers import getTargetLocation
from . import _OperationHelpers as OpHelpers


@register_command("add")
def addImage(command: dict):

    path_input1 = Path(command["i"][0])
    path_input2 = Path(command["i"][1])

    image_path1 = getTargetLocation(path_input1)
    image_path2 = getTargetLocation(path_input2)

    if not image_path1.is_file():
        return (False, f"Image not found: {image_path1}")
    if not image_path2.is_file():
        return (False, f"Image not found: {image_path2}")

    image1 = Image.open(image_path1)
    image2 = Image.open(image_path2)
    max_val = readMaxval(image_path1)

    Output = basicMathmetics(image1, image2, max_val, "add")

    if OpHelpers.setEdittedImage is None:
        return (False, "UI is not ready yet")

    OpHelpers.setEdittedImage(Output)
    path_output = Path(command["o"][0])

    save_path = getTargetLocation(path_output)

    Output.save(save_path, format="ppm")

    return (True, f"Add Image done and saved in {save_path}")


@register_command("sub")
def subImage(command: dict):
    global setEdittedImage
    path_input1 = Path(command["i"][0])
    path_input2 = Path(command["i"][1])

    image_path1 = getTargetLocation(path_input1)
    image_path2 = getTargetLocation(path_input2)

    if not image_path1.is_file():
        return (False, f"Image not found: {image_path1}")
    if not image_path2.is_file():
        return (False, f"Image not found: {image_path2}")

    image1 = Image.open(image_path1)
    image2 = Image.open(image_path2)
    max_val = readMaxval(image_path1)

    Output = basicMathmetics(image1, image2, max_val, "sub")

    if OpHelpers.setEdittedImage is None:
        return (False, "UI is not ready yet")

    OpHelpers.setEdittedImage(Output)
    path_output = Path(command["o"][0])

    save_path = getTargetLocation(path_output)

    Output.save(save_path, format="ppm")

    return (True, f"Subtract Image done and saved in {save_path}")


@register_command("mul")
def mulImage(command: dict):
    path_input1 = Path(command["i"][0])
    path_input2 = Path(command["i"][1])

    image_path1 = getTargetLocation(path_input1)
    image_path2 = getTargetLocation(path_input2)

    if not image_path1.is_file():
        return (False, f"Image not found: {image_path1}")
    if not image_path2.is_file():
        return (False, f"Image not found: {image_path2}")

    image1 = Image.open(image_path1)
    image2 = Image.open(image_path2)
    max_val = readMaxval(image_path1)

    Output = basicMathmetics(image1, image2, max_val, "mul")

    if OpHelpers.setEdittedImage is None:
        return (False, "UI is not ready yet")

    OpHelpers.setEdittedImage(Output)
    path_output = Path(command["o"][0])

    save_path = getTargetLocation(path_output)

    Output.save(save_path, format="ppm")

    return (True, f"Multiply Image done and saved in {save_path}")


@register_command("inv")
def invImage(command: dict):
    path_input = Path(command["i"][0])

    image_path = getTargetLocation(path_input)

    if not image_path.is_file():
        return (False, f"Image not found: {image_path}")

    image = Image.open(image_path)
    max_val = readMaxval(image_path)

    Output = negative(image, max_val)

    if OpHelpers.setEdittedImage is None:
        return (False, "UI is not ready yet")

    OpHelpers.setEdittedImage(Output)
    path_output = Path(command["o"][0])

    save_path = getTargetLocation(path_output)

    Output.save(save_path, format="ppm")

    return (True, f"Inverse Image done and saved in {save_path}")


@register_command("log")
def logImage(command: dict):
    path_input = Path(command["i"][0])

    image_path = getTargetLocation(path_input)

    if not image_path.is_file():
        return (False, f"Image not found: {image_path}")

    image = Image.open(image_path)
    max_val = readMaxval(image_path)

    c_val = float(command["c"][0])
    b_val = float(command["b"][0])
    Output = logTransform(image, max_val, c_val, b_val)

    if OpHelpers.setEdittedImage is None:
        return (False, "UI is not ready yet")

    OpHelpers.setEdittedImage(Output)
    path_output = Path(command["o"][0])

    save_path = getTargetLocation(path_output)

    Output.save(save_path, format="ppm")

    return (True, f"Log transform done and saved in {save_path}")


@register_command("pow")
def powImage(command: dict):
    path_input = Path(command["i"][0])

    image_path = getTargetLocation(path_input)

    if not image_path.is_file():
        return (False, f"Image not found: {image_path}")

    image = Image.open(image_path)
    max_val = readMaxval(image_path)

    c_val = float(command["c"][0])
    gamma = float(command["gamma"][0])
    Output = gammaTransform(image, max_val, c_val, gamma)

    if OpHelpers.setEdittedImage is None:
        return (False, "UI is not ready yet")

    OpHelpers.setEdittedImage(Output)
    path_output = Path(command["o"][0])

    save_path = getTargetLocation(path_output)

    Output.save(save_path, format="ppm")

    return (True, f"Gamma transform done and saved in {save_path}")
