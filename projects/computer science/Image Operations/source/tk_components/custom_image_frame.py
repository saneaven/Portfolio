from tkinter import ttk
from typing import Callable, Optional
from PIL import Image, ImageTk

from ..ppm.Parsing import readMaxval
from ..util_types.constants import BASE_FOLDER, IMAGE_FOLDER


class CustomImageFrame(ttk.Frame):
    def __init__(
        self,
        master,
        fileLocation=None,
        editOperation: Optional[Callable[[Image.Image, int], Image.Image]] = None,
        **kw,
    ):
        super().__init__(master, **kw)
        if fileLocation is None:
            fileLocation = BASE_FOLDER / IMAGE_FOLDER / "_empty.ppm"

        self.fileLocation = fileLocation
        self.editOperation: Optional[Callable[[Image.Image, int], Image.Image]] = (
            editOperation
        )

        self.label = ttk.Label(self)
        self.label.pack(fill="both", expand=True)
        self.resize_mode = "bilinear"

        self.setImage(fileLocation)
        self.bind("<Configure>", self._on_configure)
        self.pack_propagate(False)

        self._last_size = (1, 1)

    def _update_current(self):
        self._update(self.winfo_width(), self.winfo_height())

    def _update(self, width: int, height: int):
        from ..operations import resize_bilinear, resize_nearest

        if self.editOperation is not None:
            self.edittedImage = self.editOperation(self.originalImage.copy(), self.maxVal)
        else:
            self.edittedImage = self.originalImage

        match self.resize_mode:
            case "bilinear":
                self.resizedImage = resize_bilinear(self.edittedImage, width, height)
            case "nearest":
                self.resizedImage = resize_nearest(self.edittedImage, width, height)
            case _:
                raise ValueError("Not Resize Mode")
        
        # print(f"Resized Image Size: {self.resizedImage.size}")

        self.tkImage = ImageTk.PhotoImage(self.resizedImage)
        self.label.configure(image=self.tkImage)

    def _on_configure(self, event):
        self._update(event.width, event.height)
        self._last_size = (event.width, event.height)

    def setImage(self, fileLocation):
        self.fileLocation = fileLocation
        self.originalImage = Image.open(fileLocation)
        self.edittedImage = self.originalImage
        self.resizedImage = self.originalImage

        # Get max value for
        self.maxVal = readMaxval(fileLocation)

        self._update_current()

    def setCopyOfImage(self, image: Image.Image, maxVal: int):
        self.originalImage = image.copy()
        self.maxVal = maxVal
        self._update_current()

    def applyEffect(self, imageEffect: Callable[[Image.Image, int], Image.Image]):
        self.editOperation = imageEffect
        self._update_current()

    def changeResizeMode(self, mode: str):
        self.resize_mode = mode
        self._update_current()

    def forceEdittedImage(self, image):
        from ..operations import resize_bilinear, resize_nearest

        self.edittedImage = image

        match self.resize_mode:
            case "bilinear":
                self.resizedImage = resize_bilinear(
                    self.edittedImage, self.winfo_width(), self.winfo_height()
                )
            case "nearest":
                self.resizedImage = resize_nearest(
                    self.edittedImage, self.winfo_width(), self.winfo_height()
                )
            case _:
                raise ValueError("Not Resize Mode")

        self.tkImage = ImageTk.PhotoImage(self.resizedImage)
        self.label.configure(image=self.tkImage)
