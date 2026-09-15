# Bethesda Archive, Model & Texture Formats

> **Source**: XeNTaX forum (2010–2020). Complements Havok-focused guides in `reference/bethesda/`.
> **Cross-reference**: FO4 Havok animation/behavior → [`fallout4-havok-guide.md`](fallout4-havok-guide.md); Skyrim Havok pipeline → [`skyrim-havok-guide.md`](skyrim-havok-guide.md); Starfield CE2 → [`starfield-guide.md`](starfield-guide.md); Oblivion Remastered UE5 → [`oblivion-remastered-guide.md`](oblivion-remastered-guide.md); Generic HKX packfile → [`../havok/hkx_format.md`](../havok/hkx_format.md)

---

## 1. BSA Archive Format (TES3–TES5, FO3/NV)

### Structure

BSA archives are the primary container format for Elder Scrolls and early Fallout games.

**Supported games** (CynicRus, 2014):
- TES3: Morrowind
- TES4: Oblivion
- TES5: Skyrim
- Fallout 3
- Fallout: New Vegas

### BSA Header Format (Zero Diamond, 2010)

```c
struct BSAHeader {
    uint32 magic;        // "BSA\0"
    uint32 version;      // 104 (Skyrim LE), 105 (Skyrim SE)
    uint32 offset;       // Offset to filename buffer
};
```

### Version Notes

| Version | Game | Compression |
|---------|------|-------------|
| v104 (0x68) | Skyrim LE | zlib |
| v105 (0x69) | Skyrim SE | LZ4 |

*(UESP "Skyrim Mod:Archive File Format")*

### Key Observations

- BSA format changed between Morrowind and Oblivion (TaylorMouse, 2011: "They don't seem to be the same bsa files as Morrowind")
- Game File Explorer can open BSA files and view IMG/### files with COL palettes (Zero Diamond, 2010)
- BSA files can contain multiple data types: meshes, textures, animations, scripts, strings

### Extraction Tools

| Tool | Source | Games |
|------|--------|-------|
| BSArch (official) | Bethesda | TES5, FO4 |
| F4 Tools / BAE | Nexus Mods | FO4, FO3/NV |
| Fallout Mod Manager (FOMM) | SourceForge | TES5 |
| BSAExtractor (Delphi) | https://github.com/CynicRus/BSAExtractor | TES3–5, FO3/NV |

---

## 2. BA2 Archive Format (Fallout 4 / Fallout 76)

### Overview

Fallout 4 introduced BA2 (magic `BTDX`). Two sub-formats:

1. **GNRL** — General format (meshes, scripts, audio, etc.)
2. **DX10** — Texture format (DDS textures with DX10 headers)

### BA2 GNRL Header (Ekey, 2015)

```c
idstring "BTDX"          // Magic: "BTDX"
get VERSION long         // Version number
idstring "GNRL"          // Sub-format identifier
get FILES long           // Number of files
get NAMES_TABLE_OFFSET long
get NULL long
```

**File entry structure:**
```c
get DUMMY long           // Unknown
getdstring EXT 4         // File extension (4 chars)
get DUMMY long
get DUMMY long
get OFFSET long          // Offset in archive
get NULL long            // Padding
get ZSIZE long           // Compressed size
get SIZE long            // Decompressed size
get DUMMY long
```

**Extraction logic:**
```
if ZSIZE == SIZE:
    log NAME OFFSET ZSIZE      # Uncompressed
else:
    clog NAME OFFSET ZSIZE SIZE  # Compressed (zlib)
```

### BA2 DX10 Texture Format (MrAdults, 2015; Loomy, 2016; GRiNDERKILLER, 2016)

Texture archives use DX10 DDS headers. The archive stores the DXGI format directly.

**Key details:**
- Textures stored with full DX10 DDS headers
- DXGI format relayed directly from archive
- BC5 (ATI2) used for normal maps AND roughness/metalness (not just normals)
- BC5 also used for single-channel spec maps where both channels are identical (should be BC4)
- Noesis v4.171+ supports FO4 archives/textures/NIFs

**DDS header template** (010 Editor):
```c
typedef struct {
    CHAR    fType[4];      // "DDS "
    uint32  size;           // 124
    ubyte   flags[4];
    uint32  height;
    uint32  width;
    uint32  sizeorpitch;
    uint32  depth;
    uint32  mipmapcount;
    uint32  alphabitdepth;
    uint32  reserved[10];
} DDSHEADER;

typedef struct {
    uint32  size;
    ubyte   flags[4];
    uint32  fourcc;
    uint32  rgbbitcount;
    uint32  rbitmask;
    uint32  gbitmask;
    uint32  bbitmask;
    uint32  alphabitmask;
} PIXELFORMAT;
```

### QuickBMS Extraction Script (Ekey, 2015)

**GNRL archives:**
```bms
# Fallout 4 (BA2 format) — GNRL
idstring "BTDX"
get VERSION long
idstring "GNRL"
get FILES long
get NAMES_TABLE_OFFSET long
get NULL long
savepos TEMP

goto NAMES_TABLE_OFFSET
for i = 0 < FILES
  get NSIZE short
  getdstring NAME NSIZE
  putarray 0 i NAME
next i

goto TEMP
for i = 0 < FILES
    get DUMMY long
    getdstring EXT 4
    get DUMMY long
    get DUMMY long
    get OFFSET long
    get NULL long
    get ZSIZE long
    get SIZE long
    get DUMMY long
    getarray NAME 0 i
    if ZSIZE == SIZE
       log NAME OFFSET ZSIZE
    else
       clog NAME OFFSET ZSIZE SIZE
    endif
next i
```

**For texture archives**, replace `idstring "GNRL"` with `get MAGIC long` (Ekey, 2015).

### Fallout 76 BA2 Changes (Notex, 2018)

- BA2 archives still use BTDX header
- Pre-launch files encrypted (starting with `->` magic); retail files use standard BA2
- BAE v0.9.0.15 handles FO76 archives correctly

### Extraction Tools

| Tool | Source | Games |
|------|--------|-------|
| BAE (Bethesda Archive Extractor) | https://www.nexusmods.com/fallout4/mods/78 | FO4, FO76 |
| F4 Tools | michalss (2015) | FO4 (GNRL repacking; textures v3+) |
| BSArch (official) | Bethesda | TES5, FO4 |
| Noesis v4.171+ | MrAdults | FO4, FO76 (archives + textures + NIF auto-load) |

---

## 3. NIF Model Format (Skyrim / Fallout 4 / Fallout 76 / Switch)

### Skyrim NIF Format

Standard Gamebryo NIF format. NifTools/NifSkope supports it.

**Key tools:** NifSkope, 3DS Max NIF importer/exporter (MajinCry, 2015), Noesis (MrAdults, 2015)

### Fallout 4 NIF Changes (MrAdults, 2015)

**Major changes from Skyrim NIF:**
1. **BSTriShape** — New shape type with embedded geometry (no data link to separate NiTriShapeData)
2. **Flattened skeleton** — Many character models use flattened hierarchy; reference `skeleton.nif` for proper hierarchy
3. **FP16 vertex positions** — Bind pose centered at 0 for maximum FP16 precision
4. **New bone reference/inverse pose data** — Minimal changes
5. **PBR material system** — Core values and texture sets mostly same as Skyrim despite PBR addition

**Noesis NIF loading commands:**
```
-nifloadskel <filename>    # Load external skeleton for hierarchy
-nifnotransform            # Don't transform geometry (for exported pieces)
-ddsati2nonorm             # Disable ATI2 normalization (safe for all FO4)
-fbxnewexport              # Export current FBX format (not legacy 6100)
```

**Vertex format flags** (jonwd7, 2015):
- 17 unique vertex flag combinations across all FO4 NIF files
- Flags determine vertex buffer layout (position, normal, tangent, UV, vertex colors, skin weights)
- Can be decoded via XOR between known buffer configurations
- `gpuBufferSize` field must be correct or PS4/XB1 will break (MrAdults, 2016)

**Bind pose technique** (MrAdults, 2015):
- FO4 uses traditional Gamebryo "EVERYTHING HAS A TRANSFORM" approach
- Bind pose centered at 0 to maximize FP16 precision
- Node space pose = bind pose after conversion
- Recalculate on export: `newInvBindPose * fittedPose` where fittedPose quantizes to FP16

**Texture path handling** (throttlekitty, 2015):
- Texture paths sometimes start with `textures\\` and sometimes don't
- Extract all texture archives to a common directory for auto-loading

### Fallout 76 NIF Compatibility (silverm9, 2018; qs12, 2020)

- FO76 NIF files do NOT work with the FO4 Noesis script
- Some imports only load armature; others give "list index out of range"
- NifSkope 2.0 Dev can open and export FO76 models as OBJ

### Nintendo Switch Textures (Skyrim Switch) (Acewell, 2019)

**Switch texture format:**
- Uses Nvidia Tegra X1 (XTX) format
- Width at offset 0x40 (int)
- Height at offset 0x44 (int)
- Format ID at offset 0x50 (int):
  - 0x42 = DXT1 (BC1)
  - 0x44 = DXT5 (BC3)
- Mipmap count at offset 0x54 (int)
- Total data size at offset 0x58 (int)
- Mip offset table starts at 0x5C
- Image data starts at 0xD0

**Tools:**
- XTX-Extractor: https://github.com/aboood40091/XTX-Extractor
- RawTex tool (enter correct settings, change extension from .dds)

### Early Bethesda Image Formats (Arena / Terminator) (Zero Diamond, 2010)

| Format | Description |
|--------|-------------|
| **COL** | Palette files (256 entries, 3-byte RGB triplets, red in low byte) |
| **CFA** | RLE compressed + muxed animations (variable frames) |
| **DFA** | RLE compressed, partial animations following base frame |
| **IMG** | Three variants — no-header, uncompressed with header, compressed with unknown algorithm |
| **SET** | 64x64 blocks of IMG data |
| **###** | Mixed image/animation container (Future Shock, SKYNet) |

**Source code reference**: WinArena includes `ParseBSA.cpp` with full format documentation.

---

## 4. Texture Formats

### DDS / BCx Compression (Fallout 4) (volfin, 2016)

| Format | Use Case | Compression |
|--------|----------|-------------|
| BC1 (DXT1) | Color + 1-bit alpha | 4:1 |
| BC3 (DXT5) | Color + interpolated alpha | 4:1 |
| BC4 (ATI1) | Single-channel (grayscale) | 2:1 |
| BC5 (ATI2) | Dual-channel (normals, roughness/metalness) | 2:1 |
| BC6H | HDR | — |
| BC7 | High-quality RGBA | — |

**Noesis ATI2 handling** (MrAdults, 2015):
- Noesis ATI2 = BC5
- BC5 stores z derived + renormalization (correct for normal maps)
- FO4 uses BC5 for roughness/metalness (no z derivation needed)
- **Use `-ddsati2nonorm` flag for all FO4 textures**

**Intel DDS Photoshop plugin**: https://software.intel.com/en-us/articles/landing-page/01000-dds-plugins-reference-guide

---

## 5. Audio Formats

### xWMA Format (Skyrim) (Axsis, 2011)

- xWMA = WMA frames in a RIFF container
- Can be converted to PCM using `xWMAEncode` from X360 SDK
- May be convertible to standard WMA without re-encoding (unconfirmed)

### PS3 XWM/MSFC Format (Skyrim PS3) (NotDukeNukem, 2012; AlphaTwentyThree)

- Header tag: `MSFC` (not the same as standard MSF format)
- Conversion script: `msf2at3` by AlphaTwentyThree (works with Atrac codec)
- Install Atrac codec for audio editor support

---

## 6. Save File Formats (Oblivion) (jonwil, 2015)

**References:**
- Oblivion save file format: http://www.uesp.net/wiki/Tes4Mod:Save_File_Format
- Oblivion mod (ESM/ESP) format: http://www.uesp.net/wiki/Tes4Mod:Mod_File_Format
- Comprehensive parser code: http://forums.bethsoft.com/topic/148761-oblivion-resource-files-available/
- Parser covers most Oblivion data formats figured out at the time

---

## 7. EGM Morph Format (Oblivion) (falconcool, 2010)

- Controls face morph data for facial expressions (speaking, blinking, etc.)
- Each vertex in the head mesh has an index number
- 010 Editor script for modifying vertex data: http://www.tesnexus.com/downloads/file.php?id=18566
- Limitation: Cannot add new vertices, only modify existing ones
- Adding new head meshes requires new morph data for all facial expressions

---

## 8. Cell/Landscape Data

### CellToNif Conversion (Yacoby)

**Tool**: CellToNif — converts Morrowind cell mesh data to 3D NIF format
- Source: https://github.com/Yacoby/CellToNif
- Nexus: http://www.nexusmods.com/morrowind/mods/3290

**Cell grid sizes:**
- Morrowind: 64x64 cell grid
- FO3/NV, Oblivion, Skyrim, FO4: 32x32 cell grid

**VHGT data** (ReeceMix, 2017):
- Height data stored as signed bytes (e.g., `7F 81` = height value)
- 1024 DWORDs expected for a 32x32 cell (but actual may be fewer)
- Landscape is heightmap-based across all Bethsoft games
- Game-to-game cell data is NOT natively compatible (offsets differ)

**Documentation sources:**
- FO3/NV/4 ESP/ESM: https://github.com/TES5Edit/fopdoc
- Oblivion: http://www.uesp.net/wiki/Tes4Mod:Mod_File_Format
- Skyrim: http://www.uesp.net/wiki/Tes5Mod:Mod_File_Format

---

## 9. Cross-Engine Porting

### Bioware → Bethesda (B2B Project) (adludum, 2018)

**Project**: https://github.com/dhk-room101/B2B
- Automatic porting of Dragon Age Origins content (quests, dialogues) to Skyrim engine
- Based on Figment/Snip for Bethesda side
- Bioware side inspired by Rick (@gibbed) Dragon Age work
- Same process can port KoTOR to Fallout 4

---

## 10. Tool Reference

| Tool | Purpose | Games | Author/Source |
|------|---------|-------|---------------|
| Noesis v4.171+ | Archive extraction, NIF/texture viewing, FBX export | FO4, FO76 | MrAdults |
| NifSkope 2.0 Dev | NIF viewing/editing, OBJ export | FO4, FO76 | jonwd7 |
| BAE | BA2 extraction | FO4, FO76 | Nexus Mods |
| F4 Tools | BA2 extraction + GNRL repacking | FO4 | michalss |
| BSArch | Official archive tool | TES5, FO4 | Bethesda |
| BSAExtractor | BSA extraction (Delphi) | TES3–5, FO3/NV | CynicRus |
| FOMM | BSA extraction | TES5 | SourceForge |
| CellToNif | Cell mesh → NIF | Morrowind | Yacoby |
| TESAnnwyn | Heightmap import | TES3–5, FO3/NV | — |
| XTX-Extractor | Switch texture extraction | Skyrim Switch | aboood40091 |
| RawTex | Switch texture viewing | Skyrim Switch | — |
| Intel DDS Plugin | BCx texture support in Photoshop | FO4 | Intel |
| hkxcmd | HKX ↔ XML | TES5, FO4 | figment |
| havok2fbx | HKX → FBX | FO4 | Highflex |

---

## 11. Key Discoveries & Gotchas

1. **BA2 texture archives need different extraction** — GNRL script doesn't work for DX10; change `idstring "GNRL"` to `get MAGIC long`
2. **FO4 NIF bind pose is centered at 0** — not a bug, intentional FP16 optimization
3. **BC5 used for non-normal data** — FO4 uses BC5 for roughness/metalness, not just normals
4. **Texture paths are inconsistent** — some start with `textures\\`, some don't
5. **FO76 NIFs are incompatible** — FO4 Noesis script fails on FO76 models
6. **Switch textures use XTX format** — not standard DDS; need XTX-Extractor or RawTex
7. **Bodyslide-generated NIFs may break** — `gpuBufferSize` can be incorrect, breaks console rendering
8. **Cell data is not cross-game compatible** — Bethesda alters format between games despite same method
9. **BSA versions differ** — v104 (zlib) vs v105 (LZ4) between Skyrim LE and SE
10. **17 unique vertex flag formats** in FO4 NIFs — must handle all for correct mesh loading

---

## Sources

- Zero Diamond (2010): Bethesda file formats (COL/CFA/DFA/IMG/SET)
- Ekey (2014–2015): FAT/DAT headers, BA2 extraction scripts
- MrAdults (2015): Noesis FO4 support, NIF format analysis, BC5 handling
- jonwd7 (2015): Vertex flag analysis, NIF format debate
- michalss (2015): F4 Tools, BA2 extraction
- Loomy (2016): DDS header template (010 Editor)
- GRiNDERKILLER (2016): Texture header field layout
- Acewell (2019): Switch texture format specs
- shakotay2 (2016–2018): NIF debugging, hex2obj
- throttlekitty (2015): Texture pathing, FBX version
- volfin (2016): BCx format references, Intel plugin
- adludum (2018): B2B cross-engine porting project