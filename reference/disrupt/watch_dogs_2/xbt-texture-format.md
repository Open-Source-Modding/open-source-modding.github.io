# XBT Texture Format — Watch Dogs 2 (Disrupt Engine)

> **Applies to**: **Watch Dogs 2** (Disrupt engine). The XBT format is identical
> to Watch Dogs 1 — see the shared Disrupt reference
> [`disrupt/xbt-format.md`](../xbt-format.md) for the full header/DDS/DX10 layout,
> data-size math, and tool list. Per-game docs: Watch Dogs 1 →
> [`watch_dogs/xbt-texture-format.md`](../watch_dogs/xbt-texture-format.md),
> Watch Dogs Legion → [`watch_dogs_legion/xbt-format.md`](../watch_dogs_legion/xbt-format.md).

## Format

Same `TBX\x00` magic + DDS wrapper as WD1/WDL. Header size 0x34 (no string table)
or larger (string table with mip-level filepaths for streaming).

## Game-specific notes

- Identical format to WD1 — `xbt2dds` works.
- `_high` textures are separate files (lowest mip level, highest res for close
  viewing), referenced by filepath in the non-high XBT header.

## Tools

See the shared [Disrupt XBT doc](../xbt-format.md#tools) for the full list
(`xbt2dds`, `hV_WD1ModdingKit.exe`, Gibbed.Dunia, `xbt_decode.py`).
