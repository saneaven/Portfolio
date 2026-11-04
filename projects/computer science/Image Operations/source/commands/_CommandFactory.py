from __future__ import annotations
from typing import Callable, Dict

CommandFactory = Callable[[dict], tuple[bool, str]]

command_registry: Dict[str, CommandFactory] = {}

## @ decorator for module
def register_command(name: str) -> Callable[[CommandFactory], (CommandFactory)]:

    def deco(factory: CommandFactory) -> CommandFactory:
        if name in command_registry:
            raise ValueError(f"Duplicated command: {name}")
        command_registry[name] = factory
        return factory

    return deco
