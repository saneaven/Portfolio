from __future__ import annotations
import tkinter as tk
from tkinter import ttk
from typing import Callable


from PIL import Image

ApplyFuncs = Callable[[Image.Image, int], Image.Image]
PageFactory = Callable[[tk.Misc], tuple[ttk.Frame, ApplyFuncs]]

page_registry: dict[str, PageFactory] = {}


## @ decorator for module
def register_page(name: str) -> Callable[[PageFactory], (PageFactory)]:

    def deco(factory: PageFactory) -> PageFactory:
        if name in page_registry:
            raise ValueError(f"Duplicated page name: {name}")
        page_registry[name] = factory
        return factory

    return deco
