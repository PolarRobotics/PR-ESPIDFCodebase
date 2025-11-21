#!/usr/bin/env python3
"""Apply a deterministic patch to Arduino's esp32-hal-adc files.

The esp32-hal-adc sources shipped with arduino-esp32 <=3.3.0 declare a
user-facing struct named `adc_continuous_data_t`, which now conflicts
with the newer ESP-IDF headers that introduce an identically named type.

This script renames the Arduino-only struct to
`arduino_adc_continuous_data_t` in both the header and implementation.
It is idempotent and can run on every configure step to keep the
managed component patched even after it is re-downloaded by the
component manager.
"""
from __future__ import annotations

import argparse
import sys
from pathlib import Path
from typing import Iterable, Tuple

Replacement = Tuple[str, str]

HEADER_REPLACEMENTS: Iterable[Replacement] = (
    ("} adc_continuous_data_t;", "} arduino_adc_continuous_data_t;"),
    (
        "bool analogContinuousRead(adc_continuous_data_t **buffer, uint32_t timeout_ms);",
        "bool analogContinuousRead(arduino_adc_continuous_data_t **buffer, uint32_t timeout_ms);",
    ),
)

SOURCE_REPLACEMENTS: Iterable[Replacement] = (
    (
        "adc_continuous_data_t *adc_result = NULL;",
        "arduino_adc_continuous_data_t *adc_result = NULL;",
    ),
    (
        "adc_result = malloc(pins_count * sizeof(adc_continuous_data_t));",
        "adc_result = malloc(pins_count * sizeof(arduino_adc_continuous_data_t));",
    ),
    (
        "bool analogContinuousRead(adc_continuous_data_t **buffer, uint32_t timeout_ms) {",
        "bool analogContinuousRead(arduino_adc_continuous_data_t **buffer, uint32_t timeout_ms) {",
    ),
)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--component-dir",
        required=True,
        type=Path,
        help="Path to the managed arduino-esp32 component directory.",
    )
    return parser.parse_args()


def apply_replacements(path: Path, replacements: Iterable[Replacement]) -> bool:
    if not path.exists():
        raise FileNotFoundError(f"Cannot patch missing file: {path}")

    text = path.read_text()

    # If the new identifier is already present, we assume the patch was applied.
    if "arduino_adc_continuous_data_t" in text:
        return False

    updated = text
    for old, new in replacements:
        if old not in updated:
            raise RuntimeError(
                f"Expected snippet not found in {path}: {old!r}. Has the upstream file changed?"
            )
        updated = updated.replace(old, new, 1)

    if updated == text:
        return False

    path.write_text(updated)
    return True


def main() -> int:
    args = parse_args()
    component_dir: Path = args.component_dir.resolve()

    header = component_dir / "cores" / "esp32" / "esp32-hal-adc.h"
    source = component_dir / "cores" / "esp32" / "esp32-hal-adc.c"

    any_changes = False
    any_changes |= apply_replacements(header, HEADER_REPLACEMENTS)
    any_changes |= apply_replacements(source, SOURCE_REPLACEMENTS)

    if any_changes:
        print("Applied Arduino ADC compatibility patch.")
    else:
        print("Arduino ADC compatibility patch already present; no changes made.")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
