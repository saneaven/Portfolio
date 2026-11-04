from pathlib import Path


def readMaxval(path: str | Path) -> int:
    # magic, width, height, maxval
    with open(path, "rb") as f:

        def _nextToken():
            tok = b""
            while True:
                c = f.read(1)
                if not c:
                    break
                if c == b"#":
                    f.readline()
                    continue
                if c.isspace():
                    if tok:
                        break
                    else:
                        continue
                tok += c
            return tok.decode()

        magic = _nextToken()
        if magic not in ("P3", "P6"):
            raise ValueError("Not PPM(P3/P6)")
        _w = int(_nextToken())
        _h = int(_nextToken())
        maxval = int(_nextToken())
        return maxval
