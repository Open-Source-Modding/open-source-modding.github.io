# XBT Texture Format — Watch Dogs Legion (Disrupt Engine)

> **Applies to**: **Watch Dogs Legion** (Disrupt engine). The shared cross-game
> XBT layout (TBX header, DDS wrapper, DX10, string table, format list, tools)
> lives in [`disrupt/xbt-format.md`](../xbt-format.md) — this doc details the
> Legion-specific observations. Per-game docs: Watch Dogs 1 →
> [`watch_dogs/xbt-texture-format.md`](../watch_dogs/xbt-texture-format.md),
> Watch Dogs 2 → [`watch_dogs_2/xbt-texture-format.md`](../watch_dogs_2/xbt-texture-format.md).

## Overview

XBT (Texture Bank) is the runtime texture format used by the Disrupt engine (Watch Dogs Legion). It wraps a standard DDS header with a compact metadata header containing mip-level file paths for streaming.

**Magic**: `TBX\x00`  
**Version**: `0x0092` (all observed WDL files)

## File Layout

```
+0x00  TBX\0 magic (4 bytes)
+0x04  Version u16 (0x0092)
+0x06  Padding u16
+0x08  Header size u32 (total XBT header, before DDS)
+0x0C  Reserved u32 (0)
+0x10  Texture flags u32[4] (format/mip metadata)
+0x20  Value u32 (unknown)
+0x24  Hash u32
+0x28  Hash u32 (often 0)
+0x2C  Constant u32 (0x7F7F7F7F)
+0x30  String count u8, Version u8, Reserved u16
+0x32  String table (variable length, null-terminated strings)
       [DDS header starts at offset = header_size]
       [Pixel data follows immediately after DDS header]
```

## XBT Header Fields

| Offset | Size | Description |
|--------|------|-------------|
| +0x00 | 4 | Magic `TBX\x00` |
| +0x04 | 2 | Version (0x0092) |
| +0x06 | 2 | Reserved |
| +0x08 | 4 | Header size (0x34 if no strings) |
| +0x0C | 4 | Reserved (0) |
| +10 | 16 | Texture metadata (format flags, mip info) |
| +0x20 | 4 | Unknown value |
| +0x24 | 4 | Hash/checksum |
| +0x28 | 4 | Hash/checksum (often 0) |
| +0x2C | 4 | Constant 0x7F7F7F7F |
| +0x30 | 1 | String count |
| +0x31 | 1 | String table version |
| +0x32 | var | String table |

## String Table

Present when `header_size > 0x34`. Contains mip-level file paths for texture streaming.

**Format**:
- Each string: 1-byte type prefix, null-terminated
  - `0x02` = filepath (e.g. `_med` variant path)
  - `0x03` = variant path (e.g. `_high` variant path)
- Padded to align

**Example** (waterdetailnormalmap.xbt):
```
+0x30: count=2, version=1
+0x32: "engine\shaders\waterdetailnormalmap_med.xbt\0"
+0x5E: 0x02 "engine\shaders\waterdetailnormalmap_high.xbt\0"
```

The prefix byte 0x02 indicates a filepath reference. Other prefix values may exist for different string types.

## DDS Header

Standard DDS header starts at offset = `header_size`. Supports all standard DDS pixel formats.

**Pixel format** (at DDS+76):
- `pf_flags=0x04` (FOURCC): Compressed format indicated by fourcc
- `pf_flags=0x41` (RGB|ALPHAPIXELS): Uncompressed BGRA 32-bit

### Supported Formats

| Fourcc | Format | Bytes/pixel | Description |
|--------|--------|-------------|-------------|
| `DXT1` | BC1 | 0.5 | Compressed RGB (4:1) |
| `DXT3` | BC2 | 1.0 | Compressed RGBA (8:1) |
| `DXT5` | BC3 | 1.0 | Compressed RGBA (8:1) |
| `ZOLA` | BC3 | 1.0 | Custom fourcc for BC3 |
| `ATI1` | BC4 | 0.5 | Single-channel compressed |
| `ATI2` | BC5 | 1.0 | Two-channel compressed (normals) |
| `DX10` | Extended | varies | DX10 extended header follows |
| `\x00\x00\x00\x00` | BGRA8 | 4.0 | Uncompressed 32-bit RGBA |
| `q\x00\x00\x00` | Custom | varies | 3D LUT textures |

### DX10 Extended Header

When fourcc = `DX10`, a 20-byte extended header follows the DDS header:

| Offset | Size | Description |
|--------|------|-------------|
| +0 | 4 | DXGI_FORMAT enum |
| +4 | 4 | D3D10_RESOURCE_DIMENSION (0=1D, 1=2D, 2=3D) |
| +8 | 4 | Misc flags |
| +12 | 4 | Array size / depth |
| +16 | 4 | Misc flags 2 |

## UVER/NVTT Metadata

Stored in the DDS reserved1 area (DDS+32 to DDS+76). Contains:
- `UVER\0\0\0\0` marker
- `NVTT\0` + version info (NVIDIA Texture Tools)

This is historical metadata about the original compression format. The actual pixel data may be in a different format than what NVTT reports.

## Data Size Calculation

For uncompressed BGRA (fourcc=0, pf_flags=0x41):
```
data_size = width × height × 4
```

For DXT1/BC1:
```
data_size = max(1, width/4) × max(1, height/4) × 8
```

For DXT5/BC3/ZOLA/ATI2:
```
data_size = max(1, width/4) × max(1, height/4) × 16
```

For mip chains: sum all mip levels (each halved, minimum 1×1).

## Statistics (WDL)

Scanned 103 files from WDL unpacked:
- **77%** have string tables (mip filepaths)
- **99%** have UVER/NVTT metadata
- **Format distribution**: DXT1 (38%), ZOLA/BC3 (23%), ATI2/BC5 (17%), DXT5 (17%), ATI1/BC5 (5%), Other (1%)

## Usage

```python
import struct

with open("texture.xbt", "rb") as f:
    data = f.read()

# Parse XBT header
magic = data[0:4]  # b'TBX\x00'
version = struct.unpack_from('<H', data, 4)[0]  # 0x0092
hdr_size = struct.unpack_from('<I', data, 8)[0]

# DDS header at hdr_size
dds = data[hdr_size:]
width = struct.unpack_from('<I', dds, 16)[0]
height = struct.unpack_from('<I', dds, 12)[0]
mips = struct.unpack_from('<I', dds, 28)[0]
pf_flags = struct.unpack_from('<I', dds, 36+76)[0]
fourcc = dds[40+76:44+76]

# Pixel data starts at hdr_size + 128
data_start = hdr_size + 128
pixel_data = data[data_start:]
```

## Decoder

A working Python decoder implementing this layout is at
`/tmp/opencode/xbt/xbt_decode.py` (untracked, outside the repo). It parses the
TBX header, string table, DDS header, and DX10 extended header, and reports
format/dimensions/mip info. Supports DXT1 (BC1), DXT3 (BC2), DXT5 (BC3), ZOLA
(BC3), BGRA8, and DX10 extended formats.

```
python3 xbt_decode.py <file.xbt>          # print texture info
python3 xbt_decode.py --batch <dir> [--csv out.csv]   # scan a directory
```
