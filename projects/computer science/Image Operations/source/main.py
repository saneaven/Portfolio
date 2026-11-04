import gc
import tkinter as tk
from tkinter import ttk

from .tk_components.image_viewer_frame import ImageViewerFrame

from .tk_components.custom_image_frame import CustomImageFrame

from .tk_components.image_selector import addImageSelector

from .operations import page_registry
from .commands import command_registry, parseCommand
from .commands.Browser import current_location
from .commands.ImageIO import registerGetEdittedImage, registerSetOriginalImage
from .commands._OperationHelpers import registerSetEdittedImage

if __name__ == "__main__":
    # Event handlers for the command line box
    def onFocusIn(event):

        if commandLine.get() == "Type your command here":
            commandLine.delete(0, tk.END)

    def onFocusOut(event):
        if not commandLine.get():
            commandLine.insert(0, "Type your command here")

    def submitCommand(event):
        command = commandLine.get()
        command_parsed = parseCommand(command)
        print(f"Command entered: {command}")
        (result, message) = command_registry[command_parsed["command"]](command_parsed)

        commandInfo.config(text=message)
        from .commands.Browser import current_location

        commandPath.config(text=f"{current_location}")

        commandLine.delete(0, tk.END)

    ##############################################



    ## Code
    root = tk.Tk()
    root.geometry("786x680")

    root.title("CS 370 Assignment 2")
    viewFrame = ttk.Frame(root)
    operationFrame = ttk.Frame(root)

    viewFrame.pack(
        side=tk.TOP,
        fill="both",
        expand=True,
    )  # The top frame contains the image and browser frames
    operationFrame.pack(
        side=tk.BOTTOM, fill="both", expand=True, padx=10, pady=10
    )  # The bottom frame contains the command line

    imageViewerFrame = ImageViewerFrame(viewFrame)

    # When Apply Button Clicked
    def onApplyClicked():
        imageViewerFrame.onApplyClicked(image_operations, selectOperation)

    registerSetOriginalImage(lambda path: imageViewerFrame.originalImagePlace.setImage(path))
    registerGetEdittedImage(lambda: imageViewerFrame.edittingImagePlace.edittedImage)
    registerSetEdittedImage(lambda image: imageViewerFrame.edittingImagePlace.forceEdittedImage(image))

    operationGUIFrame = ttk.Frame(operationFrame)
    operationGUIFrame.pack(side=tk.TOP, fill="both", expand=True)

    ## Childs of operationGUIFrame
    operationWorkFrame = ttk.Frame(operationGUIFrame)
    operationConfig = ttk.Frame(operationGUIFrame, padding=10)
    operationWorkFrame.pack(side=tk.TOP, fill="x")
    operationConfig.pack(side=tk.BOTTOM, fill="both", expand=True)

    def onInterpolationChanged():
        imageViewerFrame.onInterpolationChanged(interpolation_mode.get())

    ## Childs of operationWorkFrame
    mode = tk.StringVar()
    selectOperation = ttk.Combobox(
        operationWorkFrame, textvariable=mode, state="readonly"
    )
    applyButton = ttk.Button(operationWorkFrame, text="apply", command=onApplyClicked)

    operationWorkFrame.rowconfigure(0, weight=1)
    operationWorkFrame.columnconfigure(0, weight=2)
    operationWorkFrame.columnconfigure(1, weight=1)
    operationWorkFrame.columnconfigure(2, weight=1)
    operationWorkFrame.columnconfigure(3, weight=1)
    operationWorkFrame.columnconfigure(4, weight=2)

    selectOperation.grid(row=0, column=0, sticky="w")
    applyButton.grid(row=0, column=4, sticky="e")
    interpolation_mode = tk.StringVar(value="bilinear")
    ttk.Label(operationWorkFrame, text="Interpolation:").grid(row=0, column=1)
    ttk.Radiobutton(
        operationWorkFrame,
        text="Nearest Neighbor",
        value="nearest",
        variable=interpolation_mode,
        command=onInterpolationChanged,
    ).grid(row=0, column=2, sticky="we")
    ttk.Radiobutton(
        operationWorkFrame,
        text="Bilinear",
        value="bilinear",
        variable=interpolation_mode,
        command=onInterpolationChanged,
    ).grid(row=0, column=3, sticky="we")

    def showConfig(event):
        pages[mode.get()].lift()

    pages = {}
    image_operations = {}
    operation_names = list(page_registry.keys())
    print(operation_names)
    selectOperation.configure(values=operation_names)
    selectOperation.set(operation_names[0] if operation_names else "")
    selectOperation.bind("<<ComboboxSelected>>", showConfig)

    ## Childs of operationConfig
    for name in operation_names:
        factory = page_registry[name]
        frame, onApply = factory(operationConfig)

        frame.grid(row=0, column=0, sticky="nsew")  # Duplicate Positions
        operationConfig.rowconfigure(0, weight=1)
        operationConfig.columnconfigure(0, weight=1)
        pages[name] = frame
        image_operations[name] = onApply

    pages[mode.get()].lift()

    # Add a command line box to the bottom frame
    commandFrame = ttk.Frame(operationFrame)
    commandFrame.pack(side=tk.BOTTOM)
    commandInfo = ttk.Label(commandFrame, text="Command Line")
    commandInfo.pack(padx=0, pady=(10, 0))
    commandPath = ttk.Label(commandFrame, text=f"{current_location}")
    commandPath.pack(padx=0, pady=(10, 0))

    commandLine = tk.Entry(commandFrame, width=50)
    commandLine.insert(0, "Type your command here")
    commandLine.pack(padx=0, pady=(5, 10))

    commandLine.bind("<FocusIn>", onFocusIn)
    commandLine.bind("<FocusOut>", onFocusOut)
    commandLine.bind("<Return>", submitCommand)

    root.mainloop()
    gc.collect()
