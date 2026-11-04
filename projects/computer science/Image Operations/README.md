# Image Operations

*A from-scratch image-processing toolkit with a Tkinter GUI, a command line, and a hand-written Fourier transform suite.*

## Overview

Image Operations is a desktop image-processing application built in Python. It loads `.ppm` images and applies a range of classic image operations, with each algorithm implemented by hand (point operations, spatial convolution, histogram equalization, and the 2D Fourier transform) rather than delegating to a library. OpenCV is used only as a reference to compare results against.

The application has two ways to drive it from a single window:

- **A GUI panel** where you pick an operation from a dropdown, tune its parameters with sliders/radio buttons, and hit *Apply* to see the original and the edited image side by side.
- **A command line box** at the bottom of the window that accepts text commands (e.g. `add`, `log`, `histeq`, `gblur`, `sobel`) with flags for inputs, outputs, and parameters, plus a small file browser (`cd`, `load`, `save`) for navigating the image folder.

This is a solo project: I wrote the whole application (the GUI shell, the command parser, the I/O layer, every operation, and the Fourier transform implementations and their performance harness).

## Highlights

- **Hand-written 2D Fourier transform in three forms**, all built without an FFT library:
  - a **direct DFT** that evaluates the definition with four nested loops (every output point sums over the whole image),
  - a **separable DFT** computed as two passes of 1D transforms (rows, then columns) for a large speedup, and
  - a **decimation-in-time FFT** with a bit-reversal permutation and in-place butterfly stages, applied along each axis with power-of-two zero-padding.
- **Spectrum centering and display**: the input is multiplied by `(-1)^(x+y)` before transforming so the spectrum is centered, and magnitudes can be shown either as a log-scaled grayscale image or as a **color "intensity slicing"** view that buckets magnitudes into a user-set number of bins on a log scale.
- **Inverse transform reuse**: the inverse is computed by conjugating the input, running the same forward FFT, and conjugating again: the reconstructed image is recovered without a separate inverse implementation.
- **Frequency-domain compression**: a min/max radius pass zeroes out frequency components outside a chosen ring before the inverse transform, letting you see how much of the spectrum can be discarded before the image degrades.
- **Performance analysis harness** (`fourier_test/`) that scales each image down through a sequence of square sizes, times all three transform methods (averaged over repeated runs), writes a CSV, and plots time vs. image size.
- **A full set of spatial operations**, all implemented directly on NumPy arrays:
  - point operations: add / subtract / multiply two images, negative, log transform, gamma (power) transform;
  - histogram equalization (computed from the CDF, with the OpenCV result and histograms shown alongside for comparison);
  - Gaussian smoothing with a generated Gaussian kernel, unsharp masking built on top of it, and Sobel edge detection (X, Y, or gradient magnitude);
  - connected-component labeling with 4-, 8-, and m-connectivity using a union-find merge tree;
  - bilinear and nearest-neighbor resizing.
- **A custom PPM header parser** that reads the maxval directly from `P3`/`P6` files (handling comments and whitespace), independent of the image library.

## How it works

The app is a Python package under `source/`. `source/main.py` builds the Tkinter window: a viewer frame on top (original image, edited image, and a browser/command path) and an operation frame below (the operation dropdown, parameter widgets, and the command line).

Two registry patterns keep the operations decoupled and pluggable:

- **`page_registry`**: each GUI operation registers itself with a `@register_page("Name")` decorator that returns a parameter frame plus an `onApply(image, maxVal)` callback. `main.py` discovers every registered page at startup, builds the dropdown from their names, and stacks their config frames.
- **`command_registry`**: each command-line verb registers itself with `@register_command("name")`. `parseCommand` tokenizes the input (via `shlex`) into a command, positional variables, and `-flag` options; the matching handler runs and returns a `(success, message)` pair shown back in the UI.

The two layers share the same underlying operation functions, so a transform like Gaussian smoothing has one implementation reachable both from a slider in the GUI and from a `gblur -i ... -o ... -n ... -sigma ...` command. I/O is bridged through small "register a setter/getter" hooks so the operation modules never import the GUI directly. Images are handled as NumPy arrays; results are clipped to the image's maxval and saved back out as PPM.

## Tech stack

- **Python 3** (the bytecode caches in the tree are CPython 3.12)
- **Tkinter / ttk**: GUI, including embedded Matplotlib canvases
- **NumPy**: all array math and the transform implementations
- **Pillow (PIL)**: image loading/saving and array conversion
- **Matplotlib**: histogram plots in the GUI and the timing graph in the analysis harness
- **OpenCV (opencv-python)**: reference histogram equalization for side-by-side comparison
- **pandas**: used by the Fourier performance harness to write timing CSVs (install separately: `pip install pandas`; it is not pinned in `requirements.txt`)

Pinned versions for the core GUI dependencies are listed in `requirements.txt`.

## Build & run

Requires Python 3 with Tkinter available.

```bash
# from the project root
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt

# run the GUI application (run as a module from the project root,
# since source/ is a package that uses relative imports)
python -m source.main
```

The window opens with the operation dropdown, the side-by-side image views, and the command line. Sample `.ppm` images are provided in `Images/`, which is the command line's default working directory.

To reproduce the Fourier transform timing comparison (the harness also needs pandas, which is not in `requirements.txt`):

```bash
pip install pandas
python fourier_test/fourier_transform_tester.py
```

This times the direct, separable, and fast transforms across several square image sizes, writes per-run timings to a CSV, and saves a `result_graph.png` of time vs. image size.

## Project structure

```
Image Operations/
├── source/
│   ├── main.py                     # Tkinter app entry point (run via `python -m source.main`)
│   ├── commands/                   # command-line layer
│   │   ├── Parser.py               # tokenizes "verb -flag value" input
│   │   ├── _CommandFactory.py      # @register_command + command_registry
│   │   ├── Operations.py           # add/sub/mul/inv/log/pow commands
│   │   ├── ConvolutionOperations.py# histeq/gblur/sobel commands
│   │   ├── Browser.py              # cd / current-location file browser
│   │   ├── ImageIO.py              # load / save commands + UI hooks
│   │   └── _OperationHelpers.py    # path resolution + editted-image setter
│   ├── operations/                 # the actual algorithms (each also a GUI page)
│   │   ├── BasicOperation.py       # add / subtract / multiply
│   │   ├── Negative.py
│   │   ├── Transforms.py           # log & gamma transforms
│   │   ├── HistogramEqualization.py# CDF-based + OpenCV reference + plots
│   │   ├── GausianSmoothing.py     # generated Gaussian kernel convolution
│   │   ├── UnsharpMasking.py
│   │   ├── SobelFilter.py          # X / Y / gradient-magnitude edges
│   │   ├── Labeling.py             # connected components (4/8/m-connectivity)
│   │   ├── Resize.py               # bilinear & nearest-neighbor
│   │   ├── _OperationFactory.py    # @register_page + page_registry
│   │   ├── fourier_transform/
│   │   │   ├── direct.py           # direct & separable DFT
│   │   │   └── fast.py             # FFT, inverse, compression
│   │   └── helpers/helper.py       # interpolation, union-find, color palette
│   ├── tk_components/              # reusable image frames & selectors
│   ├── ppm/Parsing.py              # PPM header / maxval reader
│   └── util_types/                 # vectors & path constants
├── fourier_test/
│   ├── fourier_transform_tester.py # times the three transforms, plots results
│   └── images/                     # sample inputs for timing
├── Images/                         # sample .ppm images (default working dir)
├── requirements.txt
└── result_graph.png                # sample timing output
```

## Status

Solo project built at DigiPen in 2025 as a sequence of image-processing assignments, culminating in the Fourier transform implementation and its performance analysis.
