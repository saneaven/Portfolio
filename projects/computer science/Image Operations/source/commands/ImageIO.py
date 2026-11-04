from pathlib import Path
from typing import Callable
from ._CommandFactory import register_command
from PIL import Image

setOriginalImage: Callable[[Path], None] | None = None
getEdittedImage: Callable[[], Image.Image] | None = None


def registerSetOriginalImage(func: Callable[[Path], None]):
    global setOriginalImage
    setOriginalImage = func


def registerGetEdittedImage(func: Callable[[], Image.Image]):
    global getEdittedImage
    getEdittedImage = func


@register_command("load")
def loadImage(command: dict):
    global setImageFrame
    from .Browser import current_location

    path_input = Path(command["variable"][0])
    target_location = (current_location / path_input).resolve()

    if not target_location.is_file():
        return (False, f"Error: File not found: {path_input}")

    if setOriginalImage is None:
        return (False, "UI is not ready yet")

    setOriginalImage(target_location)

    return (True, f"Image successfully Loaded: {target_location}")


@register_command("save")
def saveImage(command: dict):
    global getEdittedImage
    from .Browser import current_location

    path_input = Path(command["variable"][0])
    target_location = (current_location / path_input).resolve()

    if getEdittedImage is None:
        return (False, "UI is not ready yet")

    image: Image.Image = getEdittedImage()

    image.save(target_location, format="ppm")

    return (True, f"Image successfully saved: {target_location}")
