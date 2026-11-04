import shlex


def parseCommand(s: str) -> dict:
    tokens = shlex.split(s)  # Make Tokens
    result = {"command": "", "variable": []}  # command, variable for command, options
    key = None

    for tok in tokens:
        if tok.startswith("-"):  # Option
            key = tok.lstrip("-").lower()
            result[key] = []
        else:
            if key is None:  # it means command
                result["command"] = tok.lower()
                key = "variable"
                result[key] = []
            else:
                result[key].append(tok.lower())

    return result
