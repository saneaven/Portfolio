from pathlib import Path
from typing import Callable
from PIL import Image

setEdittedImage: Callable[[Image.Image], None] | None = None


def getTargetLocation(path: Path):
    from .Browser import current_location

    target_path = (current_location / path).resolve()

    return target_path


def registerSetEdittedImage(func: Callable[[Image.Image], None]):
    ### Assign Set editted image for allow commandline to show image
    ### This one might be moved to helpers
    global setEdittedImage
    setEdittedImage = func