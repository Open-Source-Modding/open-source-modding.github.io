# BFD (Binary Font Data) Format

BFD files define UI sprite atlas mappings in the Disrupt engine. Each BFD maps icon/sprite names to positions and sizes on paired XBT texture sheets.

## File Structure

```
+0x00  u8    magic (0x01)
+0x01  u16   entry_count (little-endian)
+0x03  [entries...] — variable-length
```

Each entry consists of:
1. **Null-terminated string** — sprite/icon name (ASCII)
2. **28 bytes** — position/size data (14× u16 LE)

## Per-Entry Data Layout (28 bytes)

| Byte Offset | u16 Index | Typical Values | Interpretation |
|-------------|-----------|----------------|----------------|
| +0 | [0] | 12–476 | Sprite width (pixels) |
| +2 | [1] | 16–480 | Sprite height (pixels) |
| +4 | [2] | 0–49152 | Source texture coordinate or identifier |
| +6 | [3] | 0–632 | Source texture coordinate or identifier |
| +8 | [4] | 0 | Reserved (always zero) |
| +10 | [5] | 0 | Reserved (always zero) |
| +12 | [6] | 0 | Reserved (always zero) |
| +14 | [7] | 0 | Reserved (always zero) |
| +16 | [8] | 0–60074 | Screen position or offset |
| +18 | [9] | 0–632 | Screen position or offset |
| +20 | [10] | 56316–65516 | Signed: -9220 to -20 (position adjustment?) |
| +22 | [11] | 65535 | Sentinel (0xFFFF) |
| +24 | [12] | 58696–65516 | Signed: -6840 to -20 (position adjustment?) |
| +26 | [13] | 0 or 65535 | Flag (0 or 0xFFFF) |

## Key Observations

- **Percentage-based coordinates** (confirmed by The Silver, 2019) — values represent screen-relative positions
- **0xAAAA / 0x5555 markers** — appear in fields [2] and [8] of some entries; likely tiling/mirroring flags or special encoding
- **0xFFFF sentinel** — field [11] is always 0xFFFF; field [13] is usually 0xFFFF or 0x0000
- **Fields [4]–[7]** — always zero across all examined files (padding or reserved)
- **Single bit changes cause dramatic visual effects** (rotation/scale) — confirmed by community

## BFD ↔ XBT Pairing

BFD files are paired with XBT texture files. Example pairings:
- `lib_icons_color.bfd` + `lib_icons_color.xbt`
- `hud_radialdetection.bfd` + corresponding XBT

The XBT contains the actual texture atlas; the BFD maps named sprites to regions within it.

## File Inventory

- **45 BFD files** across WD1, WD2, and WDL
- Entry counts: 3–186 entries per file
- File sizes: 270 bytes – 20KB
- Found in: `data/UI/SuperTextures/` (and leaked variants at `leak/ubisoft/data/UI/SuperTextures/`)

## Known Limitations

- Exact coordinate interpretation (screen-space vs texture-space) unclear without XBT pairing context
- 0xAAAA/0x5555 pattern purpose unknown
- Fields [2]–[3] and [8]–[9] need more analysis (possibly source texture atlas UV coordinates)

## Tools

No dedicated BFD editor exists. Analysis done via binary parsing (Python `struct` module).

## References

- The Silver (2019): percentage-based values confirmed
- Community: single bit changes → dramatic visual effects
- Source files: `leak/ubisoft/data/UI/SuperTextures/`
