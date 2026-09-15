# XBT Texture Format — Watch Dogs 1 (Disrupt Engine)

> **Applies to**: **Watch Dogs 1** (Disrupt engine, Dunia 2 base; identical base
> format in Far Cry 3+). The XBT layout (TBX header, DDS wrapper, DX10, string
> table, data-size math, format list) is shared across the Disrupt games — see
> [`disrupt/xbt-format.md`](../xbt-format.md). Per-game docs: Watch Dogs 2 →
> [`watch_dogs_2/xbt-texture-format.md`](../watch_dogs_2/xbt-texture-format.md),
> Watch Dogs Legion → [`watch_dogs_legion/xbt-format.md`](../watch_dogs_legion/xbt-format.md).
> **Source**: XeNTaX forum (cra0, 2014), guru3D forums, Discord community (2020-2026).

---

## Tools (WD1)

| Tool | Purpose | Source |
|------|---------|--------|
| `xbt2dds` | XBT → DDS conversion | cra0 (github.com/cra0kalo/xbt2dds) |
| `DDS2XBT.bat` | DDS → XBT (prepends dummy header) | Community |
| `XBT.dummy` | 44-byte dummy header | Community |
| `hV_WD1ModdingKit.exe` | Drag XBT → get DDS + header; edit DDS → drag back | Gibbed.Dunia tools |
| Gibbed.Dunia | Full Dunia2 toolset | git@github.com:gibbed/Gibbed.Dunia.git |

## Conversion Workflow (WD1)

### XBT → DDS (Extract)
```bash
# Option 1: xbt2dds
xbt2dds texture.xbt

# Option 2: hV_WD1ModdingKit.exe
# Drag .xbt onto exe → outputs .dds + header file
```

### DDS → XBT (Repack)
```bash
# Option 1: DDS2XBT.bat (prepends XBT.dummy)
copy /B XBT.dummy + texture.dds texture.xbt

# Option 2: hV_WD1ModdingKit.exe (PRESERVES HEADER)
# Drag edited .dds back onto exe → rebuilds .xbt with original header
```

**CRITICAL**: Always use `hV_WD1ModdingKit.exe` for round-trip editing. The dummy
header in `DDS2XBT.bat` causes brightness/gamma issues.

---

## Common Issues & Fixes (WD1)

### Textures Too Dark / Bright in Game

**Cause**: Wrong XBT header (brightness/gamma parameters)

**Fix** (The Silver, 2022-07-16):
1. Use `hV_WD1ModdingKit.exe` to extract DDS + header
2. Edit DDS in image editor (make darker if too bright in-game)
3. Drag edited DDS back to `hV_WD1ModdingKit.exe` — preserves original header
4. Test in-game

### White/Black Guns (Reflective)

**Cause**: XBT filename mismatch — header references `_high` texture that doesn't exist

**Fix** (rich off slaves, 2025-04-24):
- Rename `.xbt` files to match existing game weapon names
- Example: `sg511_x.xbt` → `ocp11_x.xbt` (match existing weapon)
- **Must also update material.bin** to reference new texture name

### _High Textures (The Silver, 2022-07-16)

- `_High` textures = lowest mip level (highest resolution for close viewing)
- Separate files (WD doesn't use mipmap chain in single file)
- **Correct replacement workflow**:
  1. Export `_high` WITHOUT mipmaps
  2. Halve non-high resolution + export WITH mipmaps
  3. Non-high XBT header references `_high` filepath

### Skin Turns White In-Game (Manu, 2024-12-14)

**Cause**: Using `DDS2XBT.bat` (dummy header) instead of preserving original header

**Fix**: Use `hV_WD1ModdingKit.exe` for round-trip conversion.

---

## XBT Header Structure (Hex Analysis)

### TBX Header Layout (123 bytes, 0x00–0x7A)

```
Offset  Size  Field                       Notes
──────  ────  ─────────────────────────── ──────────────────────────────────────
0x00    4     Magic                       "TBX\0" (0x54425800)
0x04    4     Header block size           Always 123 (0x7B) for weapon textures
0x08    4     DDS offset from file start  0x2C (self-contained) / 0x60–0x68 (regular w/ path)
0x0C    4     Reserved                    Always 0
0x10    2     Format flags A              Varies per texture (see format table below)
0x12    2     Format flags B              Varies per texture
0x14    4     Constant                    Always 1
0x18    1     ???                         0x01 observed
0x19    1     Quality/variant class       KEY FIELD — see below
0x1A    2     Constant                    Always 0xFFFF
0x1C    4     Source CRC32                Hash of source file (varies)
0x20    4     Profile ID                  Texture profile ID (varies)
0x22    4     ???                         Usually 0x02000000
0x24    4     ???                         Usually 0x722A6101
0x28    var   String table                Null-terminated path(s), padded to 16-byte alignment
      ...     [padding]                   Zero bytes to align DDS to 16-byte boundary
DDS     var   DDS data                    Standard DDS header + pixel data
```

### Byte 0x19 — Quality/Variant Class (CRITICAL)

This byte determines how the engine loads the texture:

| Value | Meaning | DDS Offset | String Table |
|-------|---------|------------|--------------|
| 0x01 | _high (self-contained, highest res) | 0x2C | Empty (null at 0x24) |
| 0x08 | Self-contained (no _high variant) | 0x2C | Empty |
| 0x09 | Regular with _high reference (some) | 0x64 | Contains path to _high.xbt |
| 0x0A | Regular with _high reference (pistol) | 0x64 | Contains path to _high.xbt |
| 0x0B | Regular with _high reference (most) | 0x60–0x68 | Contains path to _high.xbt |

**The engine checks byte 0x19 to decide whether to load a _high variant.** If 0x08,
this is the only version used at all distances. If 0x0A/0x0B, the engine follows the
embedded path to load the _high file for close-up viewing.

### Three XBT File Types

#### 1. Self-contained (no _high variant)
```
Byte 0x19 = 0x08
DDS offset = 0x2C
No string table (null at 0x24)
```
Used when the texture doesn't need a high-res variant. The engine uses this single
file at all distances. Example: `gfx_d_bullet_d.xbt`, `gfx_sgforwardtendriluc01_d.xbt`.

#### 2. Regular (references _high)
```
Byte 0x19 = 0x0A or 0x0B
DDS offset = 0x60–0x68 (varies with string length)
String table contains: "graphics\gfx\weapons\<name>_high.xbt"
```
The "low-quality" version. Contains an embedded path to the _high file. The engine
loads this at far distances, and switches to _high when close. Example:
`gfx_handgunmuzzleflash01_d.xbt` → references `gfx_handgunmuzzleflash01_d_high.xbt`.

#### 3. _high (self-contained, highest resolution)
```
Byte 0x19 = 0x01
DDS offset = 0x2C
No string table (null at 0x24)
```
The highest-resolution version. Loaded by the engine when camera is close and texture
quality is set to Ultra. Always starts DDS at 0x2C. Example:
`gfx_handgunmuzzleflash01_d_high.xbt`.

### How the Engine Loads Textures

```
1. Engine reads regular xbt header
2. Checks byte 0x19:
   ├── 0x08 → Use this file at all distances (no _high variant)
   └── 0x0A/0x0B → Read string table → follow path to _high.xbt
       ├── Close + Ultra quality → Load _high version
       └── Far or lower quality → Use this (regular) version
```

**Key insight** (HardVatsuki, 2025-01-28):
> "you don't need to create _high.xbt textures if normal texture is based on _high one"
> "game will use _high texture only if its being referenced in a xbt header of normal one"

### Quick Check: Does an XBT Have a _high Variant?

**Notepad trick** (HardVatsuki, 2025-01-28):
> "pretty fast to check that is just open xbt in notepad and see if there's a
> reference to _high texture"

Open the .xbt in a hex editor or Notepad++. Search for `_high`. If found, the
regular file references a _high variant. If not, it's self-contained.

### Weapon Texture Analysis (WD1)

All weapon textures in `graphics\gfx\weapons\`:

| File | Size | Byte 0x19 | DDS Offset | _high Reference |
|------|------|-----------|------------|-----------------|
| `gfx_handgunmuzzleflash01_d.xbt` | 43 KB | 0x0A | 0x64 | → `_high.xbt` |
| `gfx_handgunmuzzleflash01b_d.xbt` | 43 KB | 0x0A | 0x68 | → `_high.xbt` |
| `gfx_muzzleflash_shotgun_d.xbt` | 350 KB | 0x0B | 0x64 | → `_high.xbt` |
| `gfx_muzzleflashglowuc01_d.xbt` | 87 KB | 0x09 | 0x64 | → `_high.xbt` |
| `gfx_muzzleflashradials01_d.xbt` | 175 KB | 0x0B | 0x64 | → `_high.xbt` |
| `gfx_muzzleflashtendrils01_d.xbt` | 87 KB | 0x0B | 0x68 | → `_high.xbt` |
| `gfx_muzzlesmoke64_d.xbt` | 350 KB | 0x0B | 0x60 | → `_high.xbt` |
| `gfx_muzzlesmoke64b_d.xbt` | 350 KB | 0x0B | 0x60 | → `_high.xbt` |
| `gfx_d_bullet_d.xbt` | 11 KB | 0x08 | 0x2C | (none) |
| `gfx_muzzleflash_1x3_06_d.xbt` | 87 KB | 0x09 | 0x2C | (none, alien queen) |
| `gfx_sgforwardtendriluc01_d.xbt` | 87 KB | 0x08 | 0x2C | (none) |

All _high counterparts use byte 0x19 = 0x01 and DDS at 0x2C.

### Example: Pistol Muzzle Flash Pair

**Regular** (`gfx_handgunmuzzleflash01_d.xbt`, 22,092 bytes):
```
0x00: 54 42 58 00  TBX\0
0x04: 7B 00 00 00  header size = 123
0x08: 64 00 00 00  DDS offset = 0x64 (100)
0x19: 0A           has_path = yes
0x28: "graphics\gfx\weapons\gfx_handgunmuzzleflash01_d_high.xbt\0"
0x60: 00 01 00 00  padding
0x64: 44 44 53 20  DDS header begins
```
DDS: 256×128, DXT1, 9 mipmaps

**_high** (`gfx_handgunmuzzleflash01_d_high.xbt`, 22,036 bytes):
```
0x00: 54 42 58 00  TBX\0
0x04: 7B 00 00 00  header size = 123
0x08: 2C 00 00 00  DDS offset = 0x2C (44)
0x19: 01           self-contained (no path)
0x24: 00 00 00 00  null (no string table)
0x2C: 44 44 53 20  DDS header begins
```
DDS: 256×128, DXT1, 9 mipmaps (same as regular — muzzle flashes are small)

### Creating a _high Pair

If you need to add _high support for a texture:

**Step 1: Create the _high file**
1. Start with your texture DDS (higher resolution if possible)
2. Create TBX header:
   - Magic: `TBX\0`
   - Header block size: 123 (0x7B)
   - DDS offset: 0x2C (44)
   - Byte 0x19: 0x01
   - Bytes 0x24–0x27: 0x00000000 (null string table)
3. Append DDS data after header (at offset 0x2C)

**Step 2: Modify the regular file to reference _high**
1. Change byte 0x19 from 0x08 to 0x0A
2. Insert path string at offset 0x28:
   `graphics\gfx\weapons\<texture_name>_high.xbt\0`
3. Pad with zeros to align DDS to 16-byte boundary
4. Update DDS offset at 0x08 (typically 0x60–0x68 depending on string length)

**Or use hV_WD1ModdingKit.exe** — it handles header preservation automatically.

### _high Texture Best Practices (The Silver, Para, 2022)

- `_high` textures = lowest mip level (highest resolution for viewing up close)
- WD1 doesn't use mipmap chains in a single file — highest mip is a separate file
- **Correct replacement workflow**:
  1. Export `_high` WITHOUT mipmaps (it's the single highest-res level)
  2. Halve non-high resolution + export WITH mipmaps
  3. Non-high XBT header references `_high` filepath
- "sometimes other games simply cap rendering the highest mipmap levels when lowering
  texture quality, WD instead has the highest mipmap as a separate file altogether"
  — The Silver

### FBI Texture Gotcha (F_Aria, 2025-08-15)

> "I did take a look inside those fbi's non high xbt textures related to tctvest02
> with hex edit and there was no address to high xbt in their header so that mean
> those high xbts will not load in game even if those texture being exist in folder"

Some textures (FBI vests) have no _high reference in the regular file (byte 0x19 = 0x08),
so the _high files in the folder are never loaded by the engine. The regular file MUST
have the embedded path for the _high to be used.

---

## Summary (WD1)

| Operation | Tool | Preserves Header? |
|-----------|------|-------------------|
| XBT → DDS | xbt2dds | N/A (header stripped) |
| XBT → DDS + header | hV_WD1ModdingKit.exe | Yes (separate header file) |
| DDS → XBT (dummy) | DDS2XBT.bat | No (uses generic dummy) |
| DDS → XBT (proper) | hV_WD1ModdingKit.exe | **Yes** (reuses extracted header) |

**Golden rule**: Always extract with `hV_WD1ModdingKit.exe`, edit DDS, drag back
to same tool. Never use `DDS2XBT.bat` for final assets.

**_high rule**: You don't need _high files if the regular file is already based on
the _high version. The engine only loads _high if it's referenced in the regular
file's header (byte 0x19 = 0x0A/0x0B with embedded path).
