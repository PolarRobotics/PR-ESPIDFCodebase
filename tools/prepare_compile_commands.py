#!/usr/bin/env python3
"""Prepare compile_commands.json for clang-tidy / clang tooling.

clangd applies .clangd CompileFlags.Remove and --query-driver itself.
clang-tidy does neither, so this script:
  1. Strips GCC-only flags that clang rejects (mirrors .clangd Remove)
  2. Injects the cross-compiler system include paths (mirrors --query-driver)
"""

from __future__ import annotations

import json
import re
import subprocess
import sys
from functools import lru_cache
from pathlib import Path

# Keep in sync with .clangd CompileFlags.Remove (+ pyclang extras).
REMOVE_FLAGS = {
    "-fno-tree-switch-conversion",
    "-fstrict-volatile-bitfields",
    "-mlongcalls",
    "-fno-shrink-wrap",
    "-fno-test-coverage",
}

MARKER = "-DPR_CLANG_TOOLING_PREPARED=1"


def _split_command(command: str) -> list[str]:
    # compile_commands entries from CMake/Ninja are space-separated without
    # embedded spaces in paths for this toolchain layout.
    return command.split()


def _join_command(args: list[str]) -> str:
    return " ".join(args)


@lru_cache(maxsize=8)
def _system_includes(compiler: str, language: str) -> tuple[str, ...]:
    """Return -isystem paths from the real cross-compiler (query-driver equivalent)."""
    try:
        proc = subprocess.run(
            [compiler, "-E", f"-x{language}", "-", "-v"],
            input=b"",
            stdout=subprocess.DEVNULL,
            stderr=subprocess.PIPE,
            check=False,
        )
    except OSError:
        return ()

    text = proc.stderr.decode(errors="replace")
    match = re.search(
        r"#include <\.\.\.> search starts here:\n(.*?)\nEnd of search list\.",
        text,
        re.S,
    )
    if not match:
        return ()

    includes: list[str] = []
    for line in match.group(1).splitlines():
        path = line.strip()
        if path and not path.startswith("("):
            includes.extend(["-isystem", path])
    return tuple(includes)


def _language_of(args: list[str], source: str) -> str:
    if source.endswith((".cpp", ".cc", ".cxx", ".C")) or "++" in Path(args[0]).name:
        return "c++"
    return "c"


def _prepare_args(args: list[str], source: str) -> list[str]:
    if not args:
        return args

    cleaned = [a for a in args if a not in REMOVE_FLAGS]
    if MARKER in cleaned:
        return cleaned

    includes = list(_system_includes(cleaned[0], _language_of(cleaned, source)))
    # Insert after the compiler binary so later -I flags still win for project headers.
    return [cleaned[0], MARKER, *includes, *cleaned[1:]]


def prepare(path: Path) -> int:
    db = json.loads(path.read_text())
    changed = 0
    for entry in db:
        source = entry.get("file", "")
        if "arguments" in entry:
            new_args = _prepare_args(list(entry["arguments"]), source)
            if new_args != entry["arguments"]:
                entry["arguments"] = new_args
                changed += 1
        if "command" in entry:
            new_cmd = _join_command(_prepare_args(_split_command(entry["command"]), source))
            if new_cmd != entry["command"]:
                entry["command"] = new_cmd
                changed += 1

    path.write_text(json.dumps(db, indent=2) + "\n")
    print(f"prepare_compile_commands: updated {changed} entries in {path}")
    return 0


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print(f"usage: {argv[0]} <compile_commands.json>", file=sys.stderr)
        return 2
    return prepare(Path(argv[1]))


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
