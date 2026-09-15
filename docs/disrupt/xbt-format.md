# XBT Texture Format — Disrupt Engine (shared)

> **Applies to**: Disrupt engine (Watch Dogs 1, 2, Legion; Dunia 2 base — identical
> in Far Cry 3+). This is the **shared/cross-game** reference. Per-game notes:
> Watch Dogs 1 → [`watch_dogs/xbt-texture-format.md`](watch_dogs/xbt-texture-format.md),
> Watch Dogs 2 → [`watch_dogs_2/xbt-texture-format.md`](watch_dogs_2/xbt-texture-format.md),
> Watch Dogs Legion → [`watch_dogs_legion/xbt-format.md`](watch_dogs_legion/xbt-format.md).

XBT is Ubisoft's runtime texture wrapper format used by the Disrupt engine. It
wraps a standard DDS file with a compact metadata header containing texture
quality settings, streaming paths, and checksums. The name is not a known
acronym — community references simply call it "XBT".

## File Layout

```
+0x00  TBX\0 magic (4 bytes)
+0x04  Version u16 (0x0092)
+0x06  Padding u16
+0x08  Header size u32 (total XBT header, before DDS)
+0x0C  Reserved u32 (0)
+0x10  Format/density code u32
+0x14  Bytes-per-pixel/block code u32
+0x18  Quality bytes u32 ([byte0][quality class][byte2][0xFF])
+0x1C  Source file CRC32 (CSourceCrcEvaluator Hash)
+0x20  Texture profile ID (ProfileId)
+0x24  Texture profile CRC32 (ProfileCrc)
+0x28  Source .meta CRC (CSourceMetaCrcEvaluator Hash, usually 0)
+0x2C  Constant u32 (0x7F7F7F7F)
+0x30  String count u8, String table version u8
+0x32  String table (variable length, null-terminated strings)
       [DDS header starts at offset = header_size]
       [Pixel data follows immediately after DDS header]
```

## Header Fields

| Offset | Size | Description |
|--------|------|-------------|
| +0x00 | 4 | Magic `TBX\x00` |
| +0x04 | 2 | Version (0x0092 = CTexCompiler Version; **platform-specific** — see below) |
| +0x06 | 2 | Reserved |
| +0x08 | 4 | Header size (0x34 = 52 if no strings, else larger) |
| +0x0C | 4 | Reserved (0) |
| +0x10 | 4 | Format/density code (see below) |
| +0x14 | 4 | Bytes-per-pixel/block-density code (see below) |
| +0x18 | 4 | Quality bytes: `[byte0≈01][byte1=quality/res class][byte2≈01][byte3=0xFF]` |
| +0x1C | 4 | Source file CRC32 (CSourceCrcEvaluator Hash) |
| +0x20 | 4 | Texture profile ID (ProfileId) |
| +0x24 | 4 | Texture profile CRC32 (ProfileCrc) |
| +0x28 | 4 | Source .meta CRC (usually 0) |
| +0x2C | 4 | Constant 0x7F7F7F7F |
| +0x30 | 1 | String count |
| +0x31 | 1 | String table version |
| +0x32 | var | String table |

### Platform-specific version byte (+0x04)

The version byte at +0x04 differs between platforms and builds. Known values:
- `0x0092` — PC (CTexCompiler version)
- Xbox 360 and PS3 builds use different version values

**Critical**: If the version byte doesn't match the target platform, the game
rejects the file entirely. When porting textures between platforms (e.g. 360→PC),
the version byte must be updated to match the target. The DDS payload itself may
also need endian swapping for 360→PC (normal maps are particularly affected).

Source: scuba (Discord, 2023-07-23) — "there is a difference in the version byte
in the xbt header between builds or platforms. if you don't change the version
byte to the one that matches your platform the game will just reject it."

### Field identification (via .lmetadata sidecars)

The `.xbt.lmetadata` XML sidecars are the TexCompiler build logs and
**definitively identify** several header fields (correlated across 7
engine/shaders files: grid, iblbrdf_ref_256, preintegratedskindiffuse,
preintegratedskinindirect, waterdetailnormalmap, wavebase, blue_noise_array):

- **+0x1C** = Source file CRC32 = `CSourceCrcEvaluator Hash` (e.g. grid = 0x28d92b4e = 685321038)
- **+0x20** = Texture profile ID = `ProfileId` (e.g. grid=5, iblbrdf=36, preintegratedskin=68, water=50)
- **+0x24** = Texture profile CRC32 = `ProfileCrc` (e.g. grid = 0xec506e3c = 3964694076)
- **+0x28** = Source `.meta` CRC = `CSourceMetaCrcEvaluator Hash` (usually 0)
- **String table** entries = `CCompiledFileExistsEvaluator` / `CTextureMipResource`
  filepaths (e.g. `grid_med`/`grid_high`), matching the `.dep` file.
- **Version 0x0092** = `CTexCompiler Version`.

The source texture files were removed after compilation, so the CRC32 cannot be
independently recomputed; field identity rests on `.lmetadata` correlation. Of
the 7 files checked, 6 (grid, iblbrdf_ref_256, preintegratedskindiffuse,
preintegratedskinindirect, waterdetailnormalmap, wavebase) validate **all three**
fields (+0x1C, +0x20, +0x24); blue_noise_array validates only +0x1C (its
`.lmetadata` has no profile fields). HIGH confidence.

### Format / density codes (+0x10, +0x14)

`+0x10` low byte ≈ `2×(bytes-per-pixel) − 1`; pairs `(+0x10 low, +0x14 low)`
observed:

| +0x10 low | +0x14 low | Typical format |
|-----------|-----------|----------------|
| 1 | 1 | Simple / ZOLA-single / `o` / `q` LUT (15,838) |
| 3 | 2 | lower-density compressed (19,028) |
| 5 | 4 | DXT1 / ZOLA (BC7) / DXT3 / ATI2 (most common, 21,591) |
| 7 | 4 | – (1,765) |
| 3 | 4 | – (1,443) |
| 9 | 8 | high-resolution (1,421) |
| 2 | 0 | – (1,358) |
| 2 | 2 | – (1,106) |
| 6 | 4 | – (481) |
| 1 | 2 | – (399) |
| 4 | 4 | – (390) |

`+0x14` low byte is a bpp/block-density code; over the 66,456-file leak corpus
the low byte is **0** in 1,452 files (ZOLA body-part BRDF/zoneid textures) and
otherwise ∈ {1, 2, 4, 8}. `+0x10` low byte ≈ `2×(bytes-per-pixel) − 1` for most,
but the `(3,4)`, `(2,0)`, `(6,4)`, `(1,2)`, `(4,4)` pairs show the encoding is
not purely a linear bpp function.

### Quality class (+0x18 byte1)

`+0x18` as 4 stored bytes `[byte0≈01][byte1=quality][byte2≈01][byte3=0xFF]`.
`byte1` correlates with mip count **and** dimensions (quality/mip-level class).
Histogram over 66,456 leak files:

| byte1 | count | notes |
|-------|-------|-------|
| 01 | 37,867 | mips=1, large dims (512/1024) |
| 02 | 9,012 | mips=2 |
| 03 | 3,349 | mips=3 |
| 04 | 921 | mips=4 |
| 09 | 1,205 | grid full 7-mip |
| 0a | 2,872 | – |
| 0b | 4,960 | – |
| 0c | 4,244 | – |

e.g. `grid` (full 7 mips) has byte1=09; `_med`/`_high` single-mip have byte1=01.

## String Table

Present when `header_size > 0x34`. Contains mip-level file paths for texture
streaming (e.g. the `_high` variant filepaths).

- Starts at offset +0x32 (after u8 count + u8 version at +0x30)
- Each string: 1-byte type prefix, then null-terminated path
  - `0x02` = filepath (e.g. `_high` variant path)
  - `0x03` = variant path (e.g. `_ultra` variant path)
- Surveyed across 500+ leak XBTs: only prefix bytes 0x02 and 0x03 observed
- Padded to align DDS data to header_size offset

**The _high texture system**: The string table in a "regular" xbt contains the path
to its `_high.xbt` counterpart. Byte 0x19 (quality/variant class) controls whether
the engine follows this path:
- `0x08` = self-contained (no _high variant, string table empty)
- `0x0A/0x0B` = regular with _high reference (string table has path)
- `0x01` = _high file itself (self-contained, DDS at 0x2C)

See [`watch_dogs/xbt-texture-format.md`](watch_dogs/xbt-texture-format.md) for
full _high system documentation, hex analysis, and weapon texture examples.

The header controls how the game utilizes the texture (brightness/gamma,
mipmap loading, addressing, filtering). **If the header is wrong → textures
appear black/reflective/overbright in-game.**

## DDS Header

Standard DDS header starts at offset = `header_size`. Supports all standard DDS
pixel formats.

**Pixel format** (at DDS+76):
- `pf_flags=0x04` (FOURCC): compressed format indicated by fourcc
- `pf_flags=0x41` (RGB|ALPHAPIXELS): uncompressed BGRA 32-bit

### Supported Formats

| Fourcc | Format | Bytes/pixel | Description |
|--------|--------|-------------|-------------|
| `DXT1` | BC1 | 0.5 | Compressed RGB (4:1) |
| `DXT3` | BC2 | 1.0 | Compressed RGBA (8:1) |
| `DXT5` | BC3 | 1.0 | Compressed RGBA (8:1) |
| `ZOLA` | BC7 | 1.0 | Custom fourcc for BC7 (BPTC) |
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

Historical metadata about the original compression format; actual pixel data may
differ from what NVTT reports.

## Data Size Calculation

For uncompressed BGRA (fourcc=0, pf_flags=0x41):
```
data_size = width × height × 4
```
For DXT1/BC1:
```
data_size = max(1, width/4) × max(1, height/4) × 8
```
For DXT5/BC3/ATI2/ZOLA (16-byte blocks, incl. BC7):
```
data_size = max(1, width/4) × max(1, height/4) × 16
```
For mip chains: sum all mip levels (each halved, minimum 1×1).

## Tools

| Tool | Purpose | Source |
|------|---------|--------|
| `xbt2dds` | XBT → DDS conversion | cra0 (github.com/cra0kalo/xbt2dds) |
| `hV_WD1ModdingKit.exe` | Drag XBT → get DDS + header; edit DDS → drag back | Gibbed.Dunia tools |
| Gibbed.Dunia | Full Dunia2 toolset | git@github.com:gibbed/Gibbed.Dunia.git |
| `xbt_decode.py` | Python decoder (parses TBX + DDS + DX10) | community |

## Common Issues

- **Textures too dark/bright in-game**: wrong header (brightness/gamma). Always
  round-trip via a header-preserving tool, never a generic dummy header.
- **White/black reflective surfaces**: XBT filename mismatch — header references
  a `_high` texture that doesn't exist; rename to match existing names and update
  the material.bin reference.
- **Skin turns white**: repacking with a dummy header instead of preserving the
  original header.
