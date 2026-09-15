# The Witcher 3: Wild Hunt (REDEngine v2) — Archive, Texture, String & Save Formats

> **Source**: XeNTaX forum knowledge extraction (2015–2020).
> Covers `.bundle`/`.cache` archives, `.xbm` textures, `.w3strings` localization, `.w3speech` voices, `.sav` saves, meshes/skeletons, and tools.

---

## 1. Archives: `.bundle` and `.cache`

Witcher 3 packs data in **`.bundle`** and **`.cache`** files.

### Key Points
| Format | Description | Extraction |
|--------|-------------|------------|
| `.bundle` | Main game archives | QuickBMS `witcher3.bms` (aluigi) |
| `.cache` | Bulk textures (`texture.cache`) | **Not** handled by `witcher3.bms`; use Lua-utils-for-Witcher-3 |
| `.map` | Archive content map (v1.0.2) | Shared by Haoose |

### Texture Cache Detail
- `.xbm` files contain only **small preview images**
- Real textures live in `content?\texture.cache`
- Initial `.bundle` repacking was **not supported** (requested feature)

---

## 2. Textures: `.xbm` → DDS

`.xbm` files are **CR2W** containers (REDEngine format) wrapping **DDS texture data without a DDS header**.

### XBM Structure (CR2W)
```
CR2W magic → version → string table → CBitmapTexture fields:
  - width (Uint32)
  - height (Uint32)
  - compression (ETextureCompression = TCM_DXTNoAlpha)
  - textureGroup (CName, e.g., CharacterDiffuse)
  - residentMipIndex (Uint8)
  - textureCacheKey (Uint32)
```

### Conversion
- **Trick**: Load DXT1 data without header (DDS without header), or strip `.xbm` header and add DDS one
- Texture data is DDS-without-header; length/offset logic not fully documented

### Extraction Tools

#### Lua-utils-for-Witcher-3 (hhrhhr)
```
lua unpack_textures.lua texture.cache output_dir
```
- Requires **Lua 5.3** + **lua-zlib**
- 64-bit fix: use `"<I4"` instead of `"<L"` for 4-byte LE ints
- Extracted files need DDS headers added via `mod_dds_header.lua`

#### witcher2_texture_converter (reused for W3)
- `w2_xbm2dds.bms` — `.xbm` → `.dds`
- `w2_dds2xbm.bms` — `.dds` → `.xbm`
- Includes nvcompress/nvdecompress

---

## 3. Localization: `.w3strings`

W3 localization string files (new format vs W2 `.w2strings`).

### Format
- **Not simply XOR'd** — contains "almost clean text (UTF16 with some html tags)"
- Looks like XOR with a long byte sequence; fixed-key vs increment-key undetermined
- **Haoose released unpacker/repacker** (zenhax t=964)

---

## 4. Voices: `.w3speech`

### Key Differences from W2
- Voice audio is **NOT in `.cache` files** — lives in `enpc.w3speech` files (per-language, e.g., `enpc` = English)
- `.w3speech` format **differs entirely** from W2 `.w2speech`
- W3 `.w3speech` tools are **not** the same as W2 `.w2speech` tools

---

## 5. Save Files (`.sav`)

REDEngine save format.

### Structure
- **Single file split into 1,048,576-byte (0x100000) parts**
- Each part **LZ4-compressed** (LZ4 r131, default settings)
- Header "checksum" = **uncompressed file length in bytes**
- Decompressed payload resembles **binary XML**

### Tools
- **W3SavegameEditor** (github.com/Atvaark/W3SavegameEditor) — unpacking support
- W2 save references: Gibbed.RED, 13xforever's Witcher-2 SaveFormat
- Editing breaks game easily

---

## 6. Meshes, Bones & Skeletons

### CR2W Mesh Files
- W3 meshes are **CR2W** files (like W2), lineage: `.w2mesh` → `.w2mesh`
- W3 `.w2mesh`/`.w2cutscene`/`.w2scene` **cannot** be loaded by W2 REDkit — "Version 162 not supported" (max supported 115)
- Faking version (`0xA2` → `0x59`/`0x73`) crashes REDkit

### Bone Data (48 bytes/bone)
- ~48 bytes per bone: 16 bytes each for position/rotation/scale (32-bit floats)
- Float values between -1 and 1 (tiny values like `2.0e-12`) → need extra processing
- Vertex data stored in **half-floats** needing division by 65535 + scale/offset
- `CSkeleton` exposes bone names + parent indices; scale/offset (cookedData) on `CMesh`

### Model Conversion Tools
| Tool | Notes |
|------|-------|
| **Wolven-kit** | CDPR modding toolkit; github.com/Traderain/Wolven-kit; W3 + CP2077 |
| **RedTools (JLouis-B)** | Converts W3 + CP2077 models, source available |
| **Witcher 3D Model Converter** | Common tool but **no skeleton export** |
| **fmt_TW3_to_XPS.py** | Old Noesis plugin (links dead) |
| **AMD GPU PerfStudio / texture rippers** | Texture ripping; foliage "smudging" = alpha bleed-prevention |

---

## 7. Official Modkit

CD Projekt RED released **Witcher 3 Modkit** (mod support, announced 2015-08-14).

---

## 8. Tools Reference

| Tool | Purpose | Notes |
|------|---------|-------|
| QuickBMS + `witcher3.bms` | Extract `.bundle` files | aluigi; no `.cache` support |
| Lua-utils-for-Witcher-3 | Extract `texture.cache` | hhrhhr; Lua 5.3 + lua-zlib |
| witcher2_texture_converter | `.xbm` ⇄ `.dds`/`.tga` | `w2_xbm2dds.bms`, `w2_dds2xbm.bms` |
| Haoose `.w3strings` unpacker | W3 localization | zenhax t=964 |
| W3SavegameEditor | Save unpack/edit | github.com/Atvaark/W3SavegameEditor |
| Wolven-kit | CDPR modding toolkit | W3 + CP2077 |
| RedTools (JLouis-B) | Model conversion | github.com/JLouis-B/RedTools |
| Witcher 3 Modkit | Official modding tools | CDPR, 2015-08-14 |
| Witcher 3D Model Converter | Model conversion | No skeleton export |

---

## Key Facts
- Archives: `.bundle` (extractable) + `.cache` (textures, need Lua tools)
- Textures: `.xbm` = CR2W wrapper around DDS-without-header
- Localization: `.w3strings` (UTF16 + obfuscation, not simple XOR)
- Voices: `.w3speech` per-language (not in .cache, different from W2)
- Saves: `.sav` = LZ4 1MB chunks → binary XML
- Meshes: CR2W, version 162 (W2 REDkit max 115), bone parenting manual
- Modding: Wolven-kit (modern), REDkit (legacy), Modkit (official)