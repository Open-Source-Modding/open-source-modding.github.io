# Magic: The Gathering — MTG 1997, Duels of the Planeswalkers, MTG Arena

> **Source**: XeNTaX forum knowledge extraction (2007–2023).
> Covers MicroProse MTG (1997), DotP 2009–2014 (`.cat` archives), MTG Arena (Unity AssetBundles).

---

## 1. Games & Engines

| Game | Year | Engine / Platform | Archive Format |
|------|------|-------------------|----------------|
| **Magic: The Gathering** (MicroProse / Shandalar) | 1997 | Custom (DOS/Win95) | `.cat` |
| **Duels of the Planeswalkers** (DotP 2009/2012/2013/2014) | 2009+ | Custom (Xbox 360/PS3/PC) | `.cat` |
| **Magic: The Gathering Arena** | 2017+ | Unity (C#) | `.bundle` / Unity AssetBundles |

---

## 2. Classic MTG / Duels of the Planeswalkers — `.cat` Archives

### Archive Format (`.cat`)
- **Games**: MicroProse MTG (1997), DotP 2009–2014
- **Structure**: Simple container, no compression (or custom)
- **File naming**: Internal names end with `.tif` (TIFF) but **headerless**
- **Magic/Header**: Files start with `0001 0000 9C00 0000` (version=1, offset=0x9C=156)
- **Filename**: Null-terminated string at offset 8, ends with `.tif`

### Extracted File Format (Card Art)
- **Format**: **Headerless TIFF** (missing TIFF/IFF header)
- **Common header bytes**:
```
0001 0000 9C00 0000 4602 0000 D301 0000 2001 0000 E800 0000 0900 0000 0100 0000 0000 0000
```
  - `0001` = version
  - `9C00` = 156 = header size
  - `4602` = 582 = width?
  - `D301` = 467 = height?
  - `2001` = 288 = ?
  - `E800` = 232 = ?
  - `0900` = 9 = bits per sample?
  - `0100` = 1 = samples per pixel?
- **Data marker**: `0080` precedes pixel data
- **Compression**: Unknown (possibly custom or uncompressed)

### Tools & Extraction
| Tool | Notes |
|------|-------|
| **GameExtractor** | Extracts `.cat` — output files unusable |
| **MultiEx-Commander** | Extracts `.cat` — same issue |
| **HexWorkshop** | Manual analysis — can see `.tif` names in hex |
| **Custom fixer** | Add TIFF header (II* or MM*) → viewable in TIFF viewers |

### Header Reconstruction (Silver / Matsy, 2007)
- Confirmed headerless TIFF
- **Fix**: Prepend standard TIFF header:
  - Little-endian: `49 49 2A 00` (II*) + IFD offset
  - Big-endian: `4D 4D 00 2A` (MM*) + IFD offset
- **IFD**: Must describe width, height, compression, photometric, etc. from header bytes

### Archives Mentioned
- `medart.cat` — medium card art
- `smallart.cat` — small card art
- Extracted files named `00001.tif`, `0508c.tif`, etc.

---

## 3. Magic: The Gathering Arena (Unity)

### Engine
- **Engine**: **Unity** (2017+)
- **Platform**: Windows, macOS, mobile
- **Archive**: Unity **AssetBundles** (`.bundle`, `.assets`, `.resS`)

### Audio Format
- **Likely**: **Wwise** (`.bnk`, `.pck`) or **FMOD** — standard Unity middleware
- **Tools**:
  - **AssetStudio** — extract AudioClips from AssetBundles
  - **Wwise CLI** / **bnkextr** — decode `.bnk`/`.pck`
  - **vgmstream** — playback

### General Unity Modding Tools
| Tool | Purpose |
|------|---------|
| **AssetStudio** | github.com/Perfare/AssetStudio — extract models, textures, audio, scripts |
| **UABE** | Unity Assets Bundle Extractor — edit/replace assets |
| **Il2CppDumper** | Dump C# metadata (if IL2CPP) |
| **UnityPy** | Python library for AssetBundle parsing |

---

## 4. Key Repositories & Tools

| Tool | Game | Purpose |
|------|------|---------|
| **GameExtractor / MultiEx** | MTG 1997, DotP | Extract `.cat` archives |
| **HexWorkshop / 010 Editor** | MTG 1997, DotP | Manual header reconstruction |
| **AssetStudio** | MTG Arena | Extract Unity AssetBundles |
| **UABE** | MTG Arena | Edit/replace Unity assets |
| **Wwise / FMOD tools** | MTG Arena | Audio extraction |
| **Il2CppDumper** | MTG Arena | Code analysis |

---

## 5. Gaps / Unresolved

### Classic MTG / DotP
- **TIFF header reconstruction**: No automated tool — manual per file
- **Compression**: Unknown if pixel data compressed (LZW? custom?)
- **Palette/color space**: Not documented (RGB? CMYK? indexed?)
- **Multiple images per file**: `0001` suggests single; `0508c` suggests multiple?
- **Card data**: Card stats, text, mechanics — not in `.cat` (separate format?)

### MTG Arena
- **AssetBundle encryption**: None reported (standard Unity)
- **Audio middleware**: Confirmed Wwise? FMOD? Both?
- **Card art/models**: High-res extraction via AssetStudio — works
- **Game logic**: C# (IL2CPP on mobile) — dumpable

---

## 6. Cross-References
- **TIFF format**: Standard — libtiff, ImageMagick
- **Unity AssetBundles**: Generic — Unity modding docs
- **Wwise/FMOD**: Standard middleware — audio RE docs
- **QuickBMS**: May have `.cat` script — check aluigi.org
- **010 Editor templates**: Reusable for headerless TIFF

---

## 7. Recommended Next Steps
1. **Write TIFF header fixer (Python)**: Read `.cat` extract → parse header bytes → prepend valid TIFF header → save
2. **Test AssetStudio on MTG Arena** — should extract card art, models, audio directly
3. **Check QuickBMS for `.cat` script** — may automate extraction
4. **Document `.cat` structure fully** — build QuickBMS script if missing