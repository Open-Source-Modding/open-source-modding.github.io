# No Man's Sky (Hello Games Engine) — XeNTaX Forum Knowledge Extraction

## 1. Engine & Archive Overview

### 1.1 Engine
- **Engine**: Hello Games custom engine (not Unity/Unreal)
- **Archive format**: `.pak` files — variant of **PSAR** (PlayStation Archive) / **HGPAK**
- **Platform differences**: PC uses standard PSAR; PS4/Switch use **HGPAK** (different header, compression)

### 1.2 Archive Pipeline
```
Game data → *.pak (PSAR/HGPAK) → extract → *.mbin / *.exml / *.dds / shaders
Key PAK: NMSARC.553AF401.pak (engine settings), NMSARC.EEAC04FA.pak (shaders)
```

---

## 2. PAK Format (PC — PSAR variant)

### 2.1 Extraction
| Tool/Script | Notes |
|-------------|-------|
| **aluigi's brink.bms** | http://aluigi.org/bms/brink.bms — works for NMS PAK |
| **QuickBMS batch** | `quickbms -d -F "*.pak" "brink.bms" "PCBANKS" "OUTPUT"` |
| **PSARC-Decompile-Tool** | http://nomansskymods.com/mods/psarc-decompile-tool/ |
| **NMS-Extract (HugoPeters)** | https://github.com/HugoPeters/NMS-Tools — CLI extractor |

### 2.2 Structure (from brink.bms / community analysis)
- Magic: PSAR variant
- Files zlib compressed in **64 KiB chunks**
- brink.bms fix: change `log NAME OFFSET SIZE` → `clog NAME OFFSET SIZE SIZE`

### 2.3 Loose File Loading (No Repack Needed)
- **Method**: Game loads loose files from `GAMEDATA/PCBANKS/` if present
- **Source**: Reddit guide by emoose (2016) — enables shader/texture mods without repacking
- **Use case**: Chromatic aberration disable, vignette removal, shader edits

### 2.4 PS4/Switch — HGPAK Format
- **Magic**: `HGPAK` (different from PC PSAR)
- **Compression**: zstd (possibly Oodle)
- **Tool**: **HGPAKtool** by monkeyman192 — https://github.com/monkeyman192/HGPAKtool
- **BMS script**: Partial (uncompressed only); zstd/Oodle support needed

---

## 3. MBIN Format (Core Data Format)

### 3.1 Structure
```
Header:
  int32  cc / magic
  int32  version
  int32  padding[2]
  int64  hash
  char   type[64]       # e.g., "cTkLocalisationTable", "GcWidgetData"
  int64  padding

Type-dependent data follows...
```

### 3.2 Known Types
| Type | Description |
|------|-------------|
| `cTkLocalisationTable` | Language/localization data |
| `GcWidgetData` | UI widgets |
| `GcSceneData` / `GcModelData` | Scene/model descriptors |
| `GcSkeletonData` | Skeleton/bone hierarchy |

### 3.3 Tools
| Tool | Purpose |
|------|---------|
| **MBINCompiler** (Atvaark / swuforce) | 010 Editor template + compiler/decompiler |
| **NMS-View** (HugoPeters) | Model viewer — https://github.com/HugoPeters/NMS-Tools |
| **MBIN to EXML** | Community tools (swuforce texttool, MBINCompiler) |
| **NMS-Modding-Tools** | Various community forks |

### 3.4 Localization (.mbin → .exml)
- **010 Editor Template** by Atvaark: parses `cTkLocalisationTable`
- Structure: `Header → LocalisationTableHeader → LocalisationTableEntry[key[32], lines[]]`
- **Workflow**: `.mbin` → extract → `.exml` (XML) → edit → compile → `.mbin`

### 3.5 Repacking Issues
- **PC**: PSARC-Decompile-Tool requires manual file listing for repack
- **PS4/Switch**: HGPAKtool supports extraction; repacking WIP
- **Loose files preferred**: Avoid repack entirely when possible

---

## 4. Model / Skeleton Format

### 4.1 Model Files
- **Format**: Embedded in `.mbin` (type `GcModelData` / `GcSceneData`)
- **Geometry**: Standard vertex buffers (positions, normals, UVs)
- **Textures**: `.dds` (DXT/BC compressed) — directly editable
- **Shaders**: Uncompiled GLSL in `NMSARC.EEAC04FA.pak`

### 4.2 Skeleton / Bone Hierarchy
- **Descriptor**: Separate `.mbin` with bone hierarchy + transforms
- **Fields**: `transmat` = 9 values (3x3 rotation + translation? or TRS)
- **Coordinate system**: OpenGL (right-handed, Y-up)
- **Rotation order issue**: **XZY** works for some models, **XYZ** for others
- **Progress**: gregkwaste (2016) — partial success, some models still misaligned

### 4.3 Tools for Models
| Tool | Notes |
|------|-------|
| **NMS-View** | HugoPeters' viewer (models + skeletons) |
| **Noesis** | Community scripts WIP |
| **Blender** | Via export → OBJ/FBX → import |

---

## 5. Shaders

### 5.1 Format
- **Location**: `NMSARC.EEAC04FA.pak`
- **Type**: Uncompiled GLSL (OpenGL)
- **Editable**: Yes — direct text edit after extraction
- **Examples**: Chromatic aberration, vignette, post-process

### 5.2 Community Mods
- Chromatic aberration disable (Reddit 2016)
- Vignette removal
- Shader injection via loose files

---

## 6. Audio

### 6.1 Format
- **Container**: Wwise `.bnk` / `.pck` (standard)
- **Tools**: Wwise Authoring, bnkextr, vgmstream
- **Note**: Standard Wwise pipeline — not extensively discussed in XeNTaX threads

---

## 7. Key Repositories & Tools Summary

| Repo / Tool | Author | Purpose |
|-------------|--------|---------|
| **NMS-Tools** | HugoPeters | NMS-Extract, NMS-View (model viewer) |
| **MBINCompiler** | Atvaark / swuforce | MBIN ↔ EXML (010 Editor templates) |
| **HGPAKtool** | monkeyman192 | PS4/Switch HGPAK extraction |
| **brink.bms** | aluigi | QuickBMS script for PC PAK |
| **PSARC-Decompile-Tool** | NoMansSkyMods | PAK extract/repack |
| **SCSSIITool** | Ekey | SCS decryption (mentioned in NMS context?) |

---

## 8. Gaps / Unresolved (from threads)
- **Skeleton alignment**: Rotation order (XZY vs XYZ) inconsistent per model
- **MBIN full spec**: Only localization table documented; other types unknown
- **HGPAK repacking**: PS4/Switch modding blocked
- **Oodle compression**: PS4/Switch PAK may use Oodle, not zstd
- **Procedural generation data**: Planet/ship assembly logic in MBIN — not reverse engineered
- **Animation format**: Not documented in threads

---

## 9. Cross-References
- **PSAR format**: Used by Brink, other PS3/PS4 games — see aluigi's scripts
- **Wwise audio**: Standard middleware — see generic audio docs
- **010 Editor templates**: Reusable pattern for binary formats
- **QuickBMS**: Universal extractor — scripts portable across games