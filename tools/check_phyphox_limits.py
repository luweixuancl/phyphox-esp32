#!/usr/bin/env python3
"""Fail CI if phyphox BLE string limits are exceeded in firmware examples."""

from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCES = [
    ROOT / "firmware" / "examples",
    ROOT / "firmware" / "lib",
]

LIMITS = {
    "setLabel": 41,
    "setLabelX": 20,
    "setLabelY": 20,
    "setUnitX": 5,
    "setUnitY": 5,
    "setUnit": 12,
    "setInfo": 191,
}

CALL = re.compile(
    r'\.(setLabel|setLabelX|setLabelY|setUnitX|setUnitY|setUnit|setInfo)\(\s*"([^"]*)"\s*\)'
)


def main() -> int:
    errors = []
    files: list[Path] = []
    for folder in SOURCES:
        files.extend(folder.rglob("*.cpp"))
        files.extend(folder.rglob("*.h"))
        files.extend(folder.rglob("*.ino"))
    for path in sorted(files):
        text = path.read_text(encoding="utf-8")
        for match in CALL.finditer(text):
            method, value = match.group(1), match.group(2)
            limit = LIMITS[method]
            length = len(value.encode("utf-8"))
            if length > limit:
                errors.append(
                    f"{path.relative_to(ROOT)}: {method}(\"{value}\") "
                    f"is {length} bytes, max {limit}"
                )
    if errors:
        print("phyphox string limit violations:")
        for item in errors:
            print(" ", item)
        return 1
    print("All phyphox UI strings are within library limits.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
