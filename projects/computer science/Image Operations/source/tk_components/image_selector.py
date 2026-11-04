from pathlib import Path
from tkinter import ttk
import tkinter as tk

from ..util_types.constants import BASE_FOLDER, IMAGE_FOLDER

from .custom_image_frame import CustomImageFrame


def addImageSelector(parent, mode="horizontal", treeSelectEvent=None) -> CustomImageFrame:
    browserFrame = ttk.Frame(parent)
    imageFrame = ttk.Frame(parent)

    match mode:
        case "horizontal":
            browserFrame.pack(side=tk.LEFT)
            imageFrame.pack(side=tk.RIGHT, fill="both", expand=True)
        case "vertical":
            browserFrame.pack(side=tk.TOP, fill="y")
            imageFrame.pack(side=tk.BOTTOM, fill="both", expand=True)

    treeView = ttk.Treeview(browserFrame, selectmode="browse")

    imagePlace = CustomImageFrame(imageFrame)
    imagePlace.pack(fill="both", expand=True, padx=10, pady=10)

    rootIID = treeView.insert("", -1, text="Image List")

    folder = Path(BASE_FOLDER / IMAGE_FOLDER).resolve()
    files = [p.name for p in folder.glob("*.ppm") if p.is_file()]

    for file in files:
        treeView.insert(rootIID, -1, text=file)

    def onTreeSelect(event):
        selectedItem = treeView.focus()
        itemDetails = treeView.item(selectedItem)
        print(itemDetails)
        if Path(itemDetails["text"]).suffix.casefold() != ".ppm":
            return
        imageLocation = BASE_FOLDER / IMAGE_FOLDER / itemDetails["text"]
        print(imageLocation)

        if treeSelectEvent is not None:
            treeSelectEvent(imageLocation)

        imagePlace.setImage(imageLocation)

    treeView.pack(padx=10, pady=10)
    treeView.bind("<<TreeviewSelect>>", onTreeSelect)

    return imagePlace
