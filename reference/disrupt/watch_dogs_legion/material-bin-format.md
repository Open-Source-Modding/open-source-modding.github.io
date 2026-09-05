# Disrupt material.bin Format Specification

**Status**: Decoder verified across WD1 (11,384 bins), WDL (70 bins), and leak compiled (12,959 bins). 0 decode errors, 0 shader mismatches against leak XMLs.

**Authoritative source**: 010 Editor template `material.bin.bt` (`ConvertMaterials/res/`).

## Format Overview (little-endian)

### Header (17 × u32 = 68 bytes)

| Offset | Field | Notes |
|--------|-------|-------|
| 0x00 | magic | `0x004D4154` ("TAM\0") |
| 0x04 | version | **7** = WD1, **15** = WDL/Leak |
| 0x08–0x40 | header block | Version-dependent size/offset fields |

### Name

```
u32   nameSize
char  name[nameSize]
byte  pad[(4 - nameSize % 4) % 4]
```

### ShaderSetting

```
u32   shaderNameSize
char  shader[shaderNameSize]
byte  mod = (4 - shaderNameSize % 4) % 4
      if mod >= 2: origMod = mod, mod -= 2  // InitSettings includes extra 2 bytes
byte  pad[mod]

// InitSettings
u16   unk0
byte  unk_extra[2]     // ONLY if origMod >= 2
u8    unk2
u8    unk3
u32   unk4
i32   unk5
i32   unk6
i32   unk7
```

### Parameters

```
u8    unk74
u8    unk75
u16   parameterCount

per parameter:
  // Alignment (CRITICAL: type byte sits at CURRENT position, may be misaligned)
  byte  pad = 4 - (currentOffset % 4) - 1
  u8    type                          // read at current (misaligned) position
  if currentOffset % 4 == 0:
      skip 4                         // skip alignment bytes
  skip pad

  if (type - 1) <= 10:
      u32 nameID                      // = zlib CRC32(parameter name)

  // Values by type
  type 1:  u32
  type 2:  vec2 (2 × u32)
  type 3:  vec3 (3 × u32)
  type 4:  vec4 (4 × u32)
  type 5:  i32
  type 6:  u8  (bool: 0 or 1)
  type 7:  u32
  type 8:  u32 size, char string[size]
  type 9:  u32 size, char string[size]
  type 10: u32 size, char string[size]
  type 11: u32
```

### Gradient Block

```
// Align to 4-byte boundary first
u32   bUseGradient
if bUseGradient == 1:
    i32   grad_vec                    // count of gradient vectors
    vec4  vecs[grad_vec]              // 4 × u32 each
    u32   grad_id
    u32   unk1
    u8    unk2
```

### EOF

```
u32   eof                             // always 0
byte  trailing[]                      // variable: 0–16 bytes of zero padding
```

## Key Findings

### nameID = zlib CRC32

Standard zlib CRC32 of the UTF-8 parameter name. **Verified 6/6**:

| Parameter | CRC32 |
|-----------|-------|
| DiffuseColor | `0x9015351F` |
| Opacity | `0x8C4054FA` |
| AmbientColor | `0xDC05433D` |
| HeightScaleColor | `0xAFD746E7` |
| IgnoreMapEffect | `0x62E3EAB4` |
| DiffuseTexture0State | `0xA94DD2BF` |

Enum/state **values** are also CRC32-hashed: `"ColorWrap"` → `0x13371135`

### Type System

| Type | Name | Stored As | Notes |
|------|------|-----------|-------|
| 1 | u32 | Raw u32 bits | Float as bit pattern |
| 2 | vec2 | 2 × u32 | Float pair as bit patterns |
| 3 | vec3 | 3 × u32 | Float triple as bit patterns |
| 4 | vec4 | 4 × u32 | Float quad as bit patterns |
| 5 | i32 | Signed int | — |
| 6 | bool | Single byte | 0 or 1 |
| 7 | u32 | Raw u32 bits | Enum as CRC32 hash |
| 8 | string | u32 len + bytes | Texture paths, etc. |
| 9 | string | u32 len + bytes | — |
| 10 | string | u32 len + bytes | — |
| 11 | u32 | Raw u32 bits | — |

### Critical Gotcha: Misaligned Type Byte

The type byte is read at the **current stream position**, which may NOT be 4-byte aligned (after bool params = 1 byte, or string params = variable length). The alignment calculation `pad = 4 - (off % 4) - 1` accounts for this, but the type byte itself sits in the potentially misaligned position.

### Gradient Block

When `bUseGradient == 1`, the gradient block follows immediately after the last parameter (with 4-byte alignment). It contains a count, that many 16-byte gradient vectors, and 3 trailing fields. This was the root cause of earlier 12–20 byte gaps in WD1 files.

### Trailing Padding

Files may have 0–16 bytes of zero padding after the EOF field. The decoder tolerates any all-zero trailing bytes.

## Version Differences

| Field | WD1 (v7) | WDL/Leak (v15) |
|-------|----------|-----------------|
| Header size | 68 bytes | 68 bytes |
| Init settings | 6 × u32 (cmd_count in high 16 bits of last) | u16 + conditional + u8×2 + u32 + i32×3 |
| Param layout | u8 type, u8 sub_type, align, name_hash, data | pad, type byte (misaligned), align skip, pad skip, nameID, data |
| Gradient | Present if bUseGradient=1 | Same |
| Shader count | Multiple shader families | Multiple shader families |

## Validation Results

### Leak Compiled vs Dev XML (12,959 files)
- **Decode**: 12,959/12,959 OK (0 errors)
- **Shader**: 0 mismatches
- **Params**: 1,308,621 total; mismatches only on enum strings (CRC32 vs "ColorWrap") and texture paths (.xbt vs .png)

### Leak Compiled vs Retail WDL (69 matched files)
- **Identical**: 44/69 byte-for-byte identical
- **Diffs**: 25/69 differ in header offset fields (16B padding) and parameter value tuning (artist changes, not format changes)

### WDL Retail vs Leak XML (66 matched files)
- 58 numeric differences = retail tuning (EmissiveIntensity, AlphaTestValue normalized to [0,1], face blend params)

### WD1 (11,384 files)
- 11,384/11,384 decode OK
- 479,136 params total, 314 unknown (0.07%) = 35 WD1-specific hashes
- 20 shader types (DriverGeneric, DriverCloth, Unlit, DriverCarPaint, Character, etc.)

## Shader Families

Known shaders across all games:
`DriverGeneric`, `DriverCloth`, `Unlit`, `DriverCarPaint`, `Character`, `DriverBuilding`, `DriverGlass`, `DriverHair`, `NeonSign`, `WindowLight`, `DriverCarGeneric`, `DriverVegetation`, `DriverMinimap`, `RealTreeDriverLeaf`, `RealTreeDriverTrunk`, `DriverEye`, `DriverNexusmon`, `WaterFlow`, `DriverWater`, `DriverWaterDecal`, `WD2Generic`, `WD2Emissive`, `WD2Minimap`

## Data Sources

| Source | Path | Count |
|--------|------|-------|
| WD1 bins | `~/Documents/Code/re/Disrupt/Watch_Dogs/WD_materials_BIN/_materials/` | 11,384 |
| WDL bins (retail) | `~/Documents/Modding/WDL/unpacked/common/graphics/_materials/` | 70 |
| Leak compiled | `~/Documents/Code/re/Disrupt/leak/ubisoft/data_win64/graphics/_materials/` | 13,048 |
| Leak dev XMLs | `~/Documents/Code/re/Disrupt/leak/ubisoft/data/graphics/_materials/` | 24,082 |
| Name dictionary | `~/Documents/Code/re/Disrupt/ConvertMaterials/res/materialNames.txt` | 916 |

## Community Notes

- Disrupt Editor material converter (fan-made) has **bugs**: wrong file length on roundtrip, float precision loss.
- Texture paths use backslash: `graphics\_textures\_plastic\plasticpanel_01_d.xbt`
- `textureHistory` field lists previously assigned textures.
- `selMaterialOverridesId` = enum from material "Name" binhex → unsigned long (Intel byte order).
- Leak .NET editor (`Disrupt.Materials.Editor.dll`) is **XML-only** — no binary read/write. Binary format is handled by native C++ engine.
- Leak `TexCompiler_r64.dll` strings confirm XBT = header + DDS, NVTT compression, per-platform mip writers.
