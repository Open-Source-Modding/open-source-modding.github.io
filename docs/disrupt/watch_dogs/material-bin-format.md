# Watch Dogs .material.bin Format

> **Source**: Reverse-engineered from our Python parser (`blender-io-disrupt`
> `modules/Watch_Dogs/material_bin.py`, validated by decode + round-trip over
> 11,384 WD1 retail and 12,959 Ubisoft-leak material files) and cross-checked
> against the **official** Disrupt Editor standalone converter
> `ConvertMaterials.exe` (2026-08). Complements the older community notes in
> [`../unsorted/MaterialBin/material-bin-format.md`](../unsorted/MaterialBin/material-bin-format.md).
>
> **Cross-reference**: manual hex workflow + historical converter bugs →
> [Material Hex Editing](material-hex-editing.md); CRC32 hashing →
> [Hashing](hashing.md); per-shader parameter definitions →
> [`materialdescriptors/`](materialdescriptors/)

---

## Overview

A `.material.bin` is a compiled material descriptor: it binds a shader
(*shader setting*, e.g. `DriverGeneric`) to a flat list of named parameters
(floats, vectors, texture paths, sampler states). Materials live under
`graphics/_materials/*.material.bin` inside the game archives.

All integers are **little-endian**. Three format versions are known:

| Version | Endianness | Game / Build |
|---------|-----------|--------------|
| 5       | big-endian | Early 2013 beta builds (July 28 2013 Aiden mats) — magic bytes `00 4D 41 54` |
| 7       | little-endian | Watch_Dogs 1 (retail) — magic bytes `54 41 4D 00` |
| 15      | little-endian | Watch Dogs: Legion era / Ubisoft leak builds |

v5 and v7 are **structurally identical** — same header layout, same param
encoding, only byte order + version differ. `material_bin.py` auto-detects
endianness from the magic bytes (0x004D4154 = "TAM\0" written in the file's
own byte order). `ConvertMaterials.exe` **asserts version==7 and cannot read
v5** (fails `materialFile.cpp:21-22`); use `material_bin.py` for v5 files.

---

## Binary Layout

```
┌───────────────────────────────────────────────┐
│ header   (68 bytes, 17 × u32)                 │
│ name string                                   │
│ shaderName string                             │
│ initSettings (~20 bytes)                      │
│ command count + N command records (aligned)   │
│ gradient block                                │
│ eof u32 (0) + zero padding                    │
└───────────────────────────────────────────────┘
```

### Header

```
offset  size  field
0x00    4     magic   — 54 41 4D 00 ("TAM\0") = LE u32 5062996 (0x004D4154)
0x04    4     version — 7 (WD1) or 15 (WDL/leak)
0x08    60    opaque bookkeeping words (see below)
```

The 15 remaining u32s are internal bookkeeping (sizes/offsets/hashes), not
fully mapped:

- Words 2–7 (file offsets `0x08`–`0x1C`) are exposed verbatim by the official
  converter's XML as `<unk2>`…`<unk7>` and are preserved unchanged on save.
  Words 2–4 are a 12-byte GUID that changes per material/build.
- **Size words** (offsets `0x20` word 8, `0x24` word 9, `0x30` word 12, `0x38`
  word 14) — the engine reads these to locate the end of the param data:
  - `size` / `size3` / `size4` (words 8, 12, 14) = gradient-field offset + 4 − 32
  - `size2` (word 9) = total file size − 32
- **Crash bug**: older `ConvertMaterials.exe` writes WRONG values for these
  size words on XML→BIN, causing random in-game crashes (Parallellines,
  2026-01-07 Discord; qstlijku fixed in later builds). Workaround script in the
  converter's `res/material_sizefixer.1sc`. `material_bin.py` recomputes them
  from the real gradient offset on write — safe for modified materials, and
  byte-identical for unmodified round-trips.

### Strings

Both the material `name` (e.g. `NH_Bathroom_Mirror`) and `shaderName`
(e.g. `DriverGeneric`) serialize as:

```
u32 byteLength, raw bytes, zero-pad to the next 4-byte boundary
```

**Quirk**: if the shaderName padding computes to ≥ 2 bytes, only *pad−2* bytes
are written — the two "stolen" bytes belong to the following initSettings blob
instead. Reader and writer must agree on this or everything after shifts.

### initSettings

~20 bytes of small flags/counts (values like `128` observed):

```
u16 unk0, [u16 extra — only when the shader pad was ≥2], u8 unk2, u8 unk3,
u32 unk4, i32 unk5, i32 unk6, i32 unk7
```

Exposed in the converter XML as eight `<unk1>`…`<unk8>` elements. Treat it as
an opaque blob: copy it verbatim when converting.

### Command Array (Parameters)

```
u8  unk74
u8  unk75
u16 paramCount
paramCount × command record
```

Each command record:

```
[alignment filler — see quirk below]
u8  type                       type code (table below)
[u32 gap]                      ONLY when the type byte ends on a 4-byte boundary
[pad]                          remainder padding → name hash is 4-aligned
u32 nameHash                   CRC32 of the parameter NAME
value payload                  size depends on type
```

Notes:

- There is **no inline name string** in the binary — only the CRC32 of the
  name (`zlib.crc32(name)`, **case-sensitive**, exact CamelCase spelling as in
  the parameter dictionaries). Full names exist only in the XML interchange
  form and in `materialNames.txt` (~916 known names shipped with the editor
  converter).
- Parameter **order is part of the file contract**. Preserve shipped order;
  when adding a parameter, place it adjacent to its related parameter (see the
  worked example in [Mirror Cubemap Mod](mirror-cubemap-mod.md)).

#### Alignment Quirk

Padding is computed from the stream position *before* the type byte:
`pad = 4 − (pos % 4) − 1`. Read the type byte; if the position now lands
exactly on a multiple of 4, skip an **additional 4-byte word**; then skip
`pad`. Net effect: every name hash starts 4-byte aligned, and when the type
byte occupies the last byte of an aligned group, a whole empty u32 follows it.
Any writer must reproduce this dance exactly or the engine misparses the rest
of the file.

#### Type Codes

| Code | Editor meaning        | Binary payload      | Examples |
|------|-----------------------|---------------------|----------|
| 1    | float                 | 4 B (IEEE-754 bits as raw u32) | `ReliefDepth` |
| 2    | vec2                  | 8 B (2× f32)        | `GrungeTiling` |
| 3    | vec3                  | 12 B (3× f32)       | `DiffuseColor1`, `Reflectance`, `WetReflectance` |
| 4    | vec4                  | 16 B (4× f32)       | `DiffuseTiling1AndGroup1` |
| 5    | u32 / int             | 4 B                 | `ReflectionType`, `ColorizeDiffuse1Mode` |
| 6    | bool                  | 1 B                 | `UseColorizeDiffuse1` |
| 7    | sampler state (enum)  | 4 B — CRC32 of the enum string (e.g. `ColorWrap` → `0x13371135`) | `DiffuseTexture1State` |
| 8    | string / texture path | u32 length + bytes  | `DiffuseTexture1`, `ReflectionTexture` |
| 9    | string variant        | same encoding as 8  | |
| 10   | string variant        | same encoding as 8  | |
| 11   | dual-value sampler    | 4 B observed (XML carries `Value1` + `Value2`) | `RaindropSplashesTexture` |

Vector payloads are raw little-endian float32s — the reader keeps the raw bits
and reinterprets them as floats on display.

#### Gradient Block

```
align to 4
u32 useGradient                     (0 or 1)
if 1: i32 vecCount, vecCount × 16 B (vec4), u32 id, u32 unk, u8 unk
```

Rare in practice; most materials ship `useGradient = 0`.

#### Trailer

```
u32 eof        — always 0 in every file we have dumped
zero padding   — leftover alignment from the original packer; copy verbatim
```

### Worked Micro-Example

Tail of the retail `adolbec-m-2013070356157858.material.bin`
(`NH_Bathroom_Mirror`, 1008 bytes):

```
00 00 00 00 │ 0b 00 00 00 │ cb ec 8a 16 │ ff ff ff ff │ 00 00 00 00 │ 00 00 00 00 │ 00 …
             type = 11      nameHash      value          gradient = 0  eof = 0     trailing
                            0x168AECCB    0xFFFFFFFF
```

`0x168AECCB = CRC32("RaindropSplashesTexture")`.

---

## XML Interchange Format (ConvertMaterials.exe)

The official converter exchanges materials as XML. Root element children, in
order: `magic`, `version`, `unk2`…`unk7`, `name`, `shaderName`,
`initSettings` (eight `unkN` children), `commands`, `gradients`.

Each command is an `<Elem>` with:

| Element   | Meaning                                                        |
|-----------|----------------------------------------------------------------|
| `type`    | Same numeric codes as the binary type table above              |
| `unk1`    | Always `0` in observed dumps                                   |
| `name`    | Plain-text parameter name                                      |
| `Value`   | Floats/vectors: comma-separated decimals (`0.01,0.01`, `0,0,0.5,-0`). Textures: plain backslash path (`graphics\_textures\…​.xbt`). Dual-value elems carry `Value1` + `Value2` instead. |

Real fragment (`NH_Bathroom_Mirror`, around the reflection parameters):

```xml
<Elem>
    <type>5</type>
    <unk1>0</unk1>
    <name>ReflectionType</name>
    <Value>0</Value>
</Elem>
<Elem>
    <type>8</type>
    <unk1>0</unk1>
    <name>DiffuseTexture1</name>
    <Value>graphics\_textures\icone\black.xbt</Value>
</Elem>
```

---

## Authoritative Editing Workflow

**Use `ConvertMaterials.exe` for all production edits.** It is the serializer
the game's own editor pipeline used, and it is the only writer we trust for
length-changing changes.

### Tool facts

- PE32+ console tool distilled from Disrupt Editor (by j301scott); ships with
  `SDL2.dll` and a `res/` directory (`materialNames.txt` ≈ 916 parameter
  names — the CRC32 dictionary — plus `types.xml` attribute definitions).
- Runs fine under wine. **Must be launched from the exe's own directory**
  (it resolves `res/` relative to itself).
- Conversion direction is inferred from the file extension:
  - `*.material.bin` → writes `*.material.bin.xml`
  - `*.material.bin.xml` → writes `*.material.bin`
- Internal evidence it is the genuine article: PDB path
  `C:\projects\disrupteditor\build\symbols\ConvertMaterials_Release.pdb`,
  RTTI classes `CBinaryArchiveReader` / `CBinaryArchiveWriter` /
  `IBinaryArchive`, and assertions pinning header fields to stream offsets
  (`header.unkN == SDL_RWtell(fp) - beginOffset`) plus a `version == 7` check.

### Round-trip guarantee

BIN → XML → BIN produces a file of **identical length** differing in exactly
4 header bytes (the recomputed checksum/offset words). Verified functionally
identical — safe baseline before any edit.

### Editing workflow

```bash
cd /path/to/convert_materials                # the exe directory!

cp adolbec-m-….material.bin work.material.bin
wine ConvertMaterials.exe work.material.bin      # → work.material.bin.xml
# …edit the XML (change values, insert/remove <Elem> blocks)…
wine ConvertMaterials.exe work.material.bin.xml  # → work.material.bin
```

See [Mirror Cubemap Mod](mirror-cubemap-mod.md) for a complete worked example
using this workflow.

## Material Hash Resolver (resolve_materials.exe)

`resolve_materials.exe` brute-forces to brute-force resolve unknown material CRC32 hashes by sweeping artist-named paths:

**Input**: 975 unknown material hashes from WDL unpacked data
**Method**: CRC32 prefix sweep across known artist path templates (`graphics\_materials\<artist>-m-<id>.material.bin`)
**Results**: 32 artist presets, 482-777 expected hits, ~10% resolved in initial sweep

| Artist | Expected | Found | Path Template |
|--------|----------|-------|---------------|
| dodnorogova | 0-5 | 5 | `dodnorogova-m-<id>.material.bin` |
| dvoronianskyi | 2-62 | 5 | `dvoronianskyi-m-<id>.material.bin` |
| gdossantos | 7-22 | 30 | `gdossantos-m-<id>.material.bin` |
| jparkd | 101-107 | 35+ | `jparkd-m-<id>.material.bin` |

Speed: ~1.1-1.8×10⁸ hash/s per prefix (24 workers). See `wdl_material_info.txt` for full sweep output.

Source: `resolve_materials.exe` output (`wdl_material_info.txt`).

---

## Validation Results

### CRC32 Verification (nameID = zlib CRC32)

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
