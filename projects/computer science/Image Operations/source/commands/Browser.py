from ._CommandFactory import register_command
from pathlib import Path
from ..util_types.constants import BASE_FOLDER, IMAGE_FOLDER

DEFAULT_FOLDER = BASE_FOLDER / IMAGE_FOLDER

current_location = DEFAULT_FOLDER.resolve()


def findDir(base_path: Path, target_name: str):
    if not base_path.is_dir():
        return None

    for item in base_path.iterdir():
        if item.is_dir() and item.name.lower() == target_name.lower():
            return item

    return None


@register_command("cd")
def browser(command: dict):
    global current_location

    path_input = command["variable"][0]

    if path_input is None:
        return (False, "")

    path = Path(path_input)
    try_path = (current_location / path).resolve()

    if try_path.is_dir():
        current_location = try_path
    else:
        return (False, f"Error: Path not found: {path_input}")

    return (True, f"Path successfully moved to {current_location}")
