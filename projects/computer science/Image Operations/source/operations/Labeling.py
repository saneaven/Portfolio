from tkinter import ttk
from PIL import Image
import numpy as np
import tkinter as tk
from collections import deque

from ._OperationFactory import register_page
from .helpers.helper import RootFind, isIndexIn2dArray, colorPalette


def mPointLabeling(PILImage: Image.Image, thredshold: float, mode):
    image_arr = np.asarray(PILImage.convert("RGB"), dtype=np.int16)
    label_arr = np.full(image_arr.shape[:2], 0)

    luminance_arr = image_arr.mean(axis=-1)
    activated_arr = luminance_arr > thredshold

    visited_arr = np.full(image_arr.shape[:2], False)
    print(label_arr.shape)

    def getNextHitLabelIndex():
        mask_arr = (~visited_arr) & activated_arr

        if not mask_arr.any():
            return None

        idx_flat = mask_arr.ravel().argmax()
        return np.array(np.unravel_index(idx_flat, mask_arr.shape))

    def check4Neighbor(idx):
        diagonal_mask.fill(True)
        for i in range(4):  # nswe
            check = nswe_checklist[i]
            check_idx = idx + check
            if not isIndexIn2dArray(check_idx, label_arr):
                continue

            if not visited_arr[tuple(check_idx)] and activated_arr[tuple(check_idx)]:
                visited_arr[tuple(check_idx)] = True
                queue.append(check_idx)

    def check8Neighbor(idx):
        diagonal_mask.fill(True)
        for i in range(4):  # nswe
            check = nswe_checklist[i]
            check_idx = idx + check
            if not isIndexIn2dArray(check_idx, label_arr):
                continue

            if not visited_arr[tuple(check_idx)] and activated_arr[tuple(check_idx)]:
                visited_arr[tuple(check_idx)] = True
                queue.append(check_idx)

        for i in range(4):  # Diagonal
            check = diagonal_checklist[i]
            check_idx = idx + check

            if not isIndexIn2dArray(check_idx, label_arr):
                continue

            if not visited_arr[tuple(check_idx)] and activated_arr[tuple(check_idx)]:
                visited_arr[tuple(check_idx)] = True
                queue.append(check_idx)

    def checkMNeighbor(idx):
        diagonal_mask.fill(True)
        for i in range(4):  # nswe
            check = nswe_checklist[i]
            check_idx = idx + check
            if not isIndexIn2dArray(check_idx, label_arr):
                continue

            if not visited_arr[tuple(check_idx)] and activated_arr[tuple(check_idx)]:
                visited_arr[tuple(check_idx)] = True
                queue.append(check_idx)
                diagonal_mask[nswe_diagonal_neighbors[i]] = False

        for i in range(4):  # Diagonal
            if diagonal_mask[i]:
                check = diagonal_checklist[i]
                check_idx = idx + check

                if not isIndexIn2dArray(check_idx, label_arr):
                    continue

                if (
                    not visited_arr[tuple(check_idx)]
                    and activated_arr[tuple(check_idx)]
                ):
                    visited_arr[tuple(check_idx)] = True
                    queue.append(check_idx)

    nswe_checklist = np.array([[0, 1], [0, -1], [1, 0], [-1, 0]])
    diagonal_checklist = np.array([[1, 1], [-1, -1], [1, -1], [-1, 1]])
    nswe_diagonal_neighbors = np.array(
        [
            [0, 3],  # right - UR, DR
            [1, 2],  # Left - UL, DL
            [0, 2],  # Down - DR, DL
            [1, 3],  # Up - UR, UL
        ]
    )
    diagonal_mask = np.full(4, True)
    mergeTree = RootFind()

    current_label = 0
    queue = deque()
    current_pixel_idx = getNextHitLabelIndex()
    label_num = 0

    # Pass 1 ------------------------------
    while current_pixel_idx is not None:
        queue.append(current_pixel_idx)
        visited_arr[tuple(current_pixel_idx)] = True
        current_label += 1
        mergeTree.expand()
        print(f"Start mapping Label {current_label}")
        label_num = 0

        while queue:
            now = queue.popleft()
            label_arr[tuple(now)] = current_label
            label_num += 1

            match mode:
                case "4p":
                    check4Neighbor(now)
                case "8p":
                    check8Neighbor(now)
                case "mp":
                    checkMNeighbor(now)

        current_pixel_idx = getNextHitLabelIndex()

        print(f"Done mapping Label {current_label}: {label_num} pixels")

    # Pass 2 ---------------------------------
    for i in range(len(mergeTree.parent)):
        root = mergeTree.findRoot(i)
        if i != root:
            print(f"Merging {i} to {root}...")
            label_arr[label_arr == i] = root

    result = np.zeros_like(image_arr, dtype=np.uint8)
    colors = colorPalette(n=current_label + 1)

    colors[0] = np.array([0, 0, 0])

    for i, color in enumerate(colors):
        result[label_arr == i] = color

    return Image.fromarray(result.astype(np.uint8), "RGB")


@register_page("m-point labeling")
def mPointLabelingFrame(parent: tk.Misc):
    logFrame = ttk.Frame(parent)

    modeFrame = ttk.Frame(logFrame)
    labeling_mode = tk.StringVar(value="4p")
    ttk.Radiobutton(modeFrame, text="4-point", value="4p", variable=labeling_mode).pack(
        side=tk.LEFT
    )
    ttk.Radiobutton(
        modeFrame,
        text="8-point",
        value="8p",
        variable=labeling_mode,
    ).pack(side=tk.LEFT)
    ttk.Radiobutton(modeFrame, text="m-way", value="mp", variable=labeling_mode).pack(
        side=tk.LEFT
    )

    thredshold_val = tk.DoubleVar(value=127.0)
    thredshold_val_label = ttk.Label(logFrame, text="Constant Value")

    thredshold_val_slider = tk.Scale(
        logFrame,
        from_=1,
        to=255,
        orient="horizontal",
        resolution=0.1,
        variable=thredshold_val,
    )

    modeFrame.pack(side=tk.TOP, padx=0, pady=(10, 0))
    thredshold_val_label.pack(side=tk.TOP, padx=0, pady=(10, 0))
    thredshold_val_slider.pack(side=tk.TOP, expand=True, fill="both")

    def onApply(image: Image.Image, _maxVal: int):
        return mPointLabeling(image, thredshold_val.get(), labeling_mode.get())

    return (logFrame, onApply)
