# No Man's Sky (Hello Games Engine) — Archives, MBIN, Models & Shaders

> **Source**: XeNTaX forum knowledge extraction (2016–2023).
> Custom Hello Games engine (not Unity/Unreal). PC uses PSAR variant; PS4/Switch use HGPAK. Core format: `.mbin` (binary) → `.exml` (XML).

---

## 1. Engine & Archive Overview

| Aspect | Detail |
|--------|--------|
| Engine | Hello Games custom |
| Archive (PC) | `.pak` — PSAR variant |
| Archive (PS4/Switch) | `.pak` — **HGPAK** (different header, zstd/Oodle compression) |
| Key PAKs | `NMSARC.553AF401.pak` (engine settings), `NMSARC.EEAC04FA.pak` (shaders) |

---

## 2. PAK Format

### PC (PSAR variant)

| Tool/Script | Notes |
|-------------|-------|
| **aluigi's brink.bms** | aluigi.org/bms/brink.bms — works for NMS PAK |
| QuickBMS batch | `quickbms -d -F "*.pak" "brink.bms" "PCBANKS" "OUTPUT"` |
| **PSARC-Decompile-Tool** | nomansskymods.com/mods/psarc-decompile-tool/ |
| **NMS-Extract (HugoPeters)** | github.com/HugoPeters/NMS-Tools — CLI extractor |

### Structure
- Magic: PSAR variant
- Files zlib compressed in **64 KiB chunks**
- brink.bms fix: change `log NAME OFFSET SIZE` → `clog NAME OFFSET SIZE SIZE`

### Loose File Loading (No Repack Needed)
- Game loads loose files from `GAMEDATA/PCBANKS/` if present
- Reddit guide by emoose (2016) — enables shader/texture mods without repacking
- Use cases: Chromatic aberration disable, vignette removal, shader edits

### PS4/Switch — HGPAK
- **Magic**: `HGPAK` (different from PC PSAR)
- **Compression**: zstd (possibly Oodle)
- **Tool**: **HGPAKtool** (monkeyman192) — github.com/monkeyman192/HGPAKtool
- BMS script: Partial (uncompressed only); zstd/Oodle support needed

---

## 3. MBIN Format (Core Data Format)

### Structure
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

### Known Types
| Type | Description |
|------|-------------|
| `cTkLocalisationTable` | Language/localization data |
| `GcWidgetData` | UI widgets |
| `GcSceneData` / `GcModelData` | Scene/model descriptors |
| `GcSkeletonData` | Skeleton/bone hierarchy |

### Tools
| Tool | Purpose |
|------|---------|
| **MBINCompiler** (Atvaark / swuforce) | 010 Editor template + compiler/decompiler |
| **NMS-View** (HugoPeters) | Model viewer — github.com/HugoPeters/NMS-Tools |
| **MBIN to EXML** | Community tools (swuforce texttool, MBINCompiler) |
| **NMS-Modding-Tools** | Various community forks |

### Localization (`.mbin` → `.exml`)
- 010 Editor Template by Atvaark: parses `cTkLocalisationTable`
- Structure: `Header → LocalisationTableHeader → LocalisationTableEntry[key[32], lines[]]`
- Workflow: `.mbin` → extract → `.exml` (XML) → edit → compile → `.mbin`

### Repacking Issues
- **PC**: PSARC-Decompile-Tool requires manual file listing for repack
- **PS4/Switch**: HGPAKtool supports extraction; repacking WIP
- **Loose files preferred**: Avoid repack entirely when possible

---

## 4. Model / Skeleton Format

### Model Files
- Format: Embedded in `.mbin` (type `GcModelData` / `GcSceneData`)
- Geometry: Standard vertex buffers (positions, normals, UVs)
- Textures: `.dds` (DXT/BC compressed) — directly editable
- Shaders: Uncompiled GLSL in `NMSARC.EEAC04FA.pak`

### Skeleton / Bone Hierarchy
- Descriptor: Separate `.mbin` with bone hierarchy + transforms
- Fields: `transmat` = 9 values (3×3 rotation + translation? or TRS)
- Coordinate system: OpenGL (right-handed, Y-up)
- **Rotation order issue**: **XZY** works for some models, **XYZ** for others
- Progress: gregkwaste (2016) — partial success, some models still misaligned

### Model Tools
| Tool | Notes |
|------|-------|
| **NMS-View** | HugoPeters' viewer (models + skeletons) |
| **Noesis** | Community scripts WIP |
| **Blender** | Via export → OBJ/FBX → import |

---

## 5. Shaders

### Format
- **Location**: `NMSARC.EEAC04FA.pak`
- **Type**: Uncompiled GLSL (OpenGL)
- **Editable**: Yes — direct text edit after extraction

### Community Mods
- Chromatic aberration disable (Reddit 2016)
- Vignette removal
- Shader injection via loose files

---

## 6. Audio

| Format | Detail |
|--------|--------|
| Container | Wwise `.bnk` / `.pck` (standard) |
| Tools | Wwise Authoring, bnkextr, vgmstream |
| Note | Standard Wwise pipeline — not extensively discussed in XeNTaX threads |

---

## 7. Key Repositories & Tools

| Repo / Tool | Author | Purpose |
|-------------|--------|---------|
| **NMS-Tools** | HugoPeters | NMS-Extract, NMS-View (model viewer) |
| **MBINCompiler** | Atvaark / swuforce | MBIN ↔ EXML (010 Editor templates) |
| **HGPAKtool** | monkeyman192 | PS4/Switch HGPAK extraction |
| **brink.bms** | aluigi | QuickBMS script for PC PAK |
| **PSARC-Decompile-Tool** | NoMansSkyMods | PAK extract/repack |
| **SCSSIITool** | Ekey | SCS decryption (mentioned in NMS context?) |

---

## 8. Gaps / Unresolved
- **Skeleton alignment**: Rotation order (XZY vs XYZ) inconsistent per model
- **MBIN full spec**: Only localization table documented; other types unknown
- **HGPAK repacking**: PS4/Switch modding blocked
- **Oodle compression**: PS4/Switch PAK may use Oodle, not zstd
- **Procedural generation data**: Planet/ship assembly logic in MBIN — not reverse engineered
- **Animation format**: Not documented in threads

---

## 9. Cross-References
- **PSAR format**: Used by Brink, other PS3/PS4 games — see aluigi's scripts
- **Wwise audio**: Standard middleware
- **010 Editor templates**: Reusable pattern for binary formats
- **QuickBMS**: Universal extractor — scripts portable across games