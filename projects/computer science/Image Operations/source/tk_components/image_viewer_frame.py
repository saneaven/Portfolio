import tkinter as tk
from tkinter import ttk

from .custom_image_frame import CustomImageFrame
from .image_selector import addImageSelector



class ImageViewerFrame(ttk.Frame):
    def __init__(
        self,
        master: tk.Misc,
        **kw,
    ):
        super().__init__(master, **kw)
            # Create the image and browser frames inside the view frame
        master.rowconfigure(0, weight=2)
        master.columnconfigure(0, weight=2)
        master.columnconfigure(1, weight=1)
        self.originalFrame = ttk.Frame(master)
        self.edittingFrame = ttk.Frame(master)
        self.originalFrame.grid(row=0, column=0, sticky="nsew")
        self.edittingFrame.grid(row=0, column=1, sticky="nsew", padx=4)

        self.is_fresh = True


        self.originalImagePlace = addImageSelector(self.originalFrame, treeSelectEvent=self.onImageSelected)

        saveButton = ttk.Button(self.edittingFrame, text="save", command=self._onSave)
        self.edittingImagePlace = CustomImageFrame(self.edittingFrame)
        saveButton.pack(side=tk.TOP)
        self.edittingImagePlace.pack(side=tk.BOTTOM, fill="both", expand=True)

    # When SaveButtonClicked
    def _onSave(self):
        self.edittingImagePlace.edittedImage.save("./Outputs/Output.ppm", format="ppm")

    def onInterpolationChanged(self, interpolation_mode):
        self.originalImagePlace.changeResizeMode(interpolation_mode)
        self.edittingImagePlace.changeResizeMode(interpolation_mode)

    def onImageSelected(self, _imageLocation):
        self.is_fresh = True

    def onApplyClicked(self, image_operations, selectOperation):
        # Clear previous operation to prevent it from running during image setup
        self.edittingImagePlace.editOperation = None
        
        name = selectOperation.get()
        if self.is_fresh:
            self.edittingImagePlace.setImage(self.originalImagePlace.fileLocation)
            self.is_fresh = False
        else:
            print("Swapping Images")
            self.originalImagePlace.setCopyOfImage(self.edittingImagePlace.edittedImage, self.edittingImagePlace.maxVal)
            self.edittingImagePlace.setCopyOfImage(self.originalImagePlace.originalImage, self.originalImagePlace.maxVal)

        self.edittingImagePlace.applyEffect(image_operations[name])