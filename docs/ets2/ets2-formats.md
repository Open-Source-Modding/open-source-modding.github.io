# Euro Truck Simulator 2 / American Truck Simulator (Prism3D Engine) — Archives, Models & Config

> **Source**: XeNTaX forum knowledge extraction (2012–2023).
> Engine: **Prism3D** (SCS Software proprietary). Games: ETS2 (2012), ATS (2016), Scania Truck Driving Simulator.

---

## 1. Engine & Archive Overview

| Aspect | Detail |
|--------|--------|
| Engine | Prism3D (SCS Software) |
| Games | ETS2, ATS, Scania Sim |
| Archive (base) | `.scs` (SCS# container) |
| Archive (mods) | `.scs` (ZIP-based) |
| Config | `.sii` (text, encrypted as `.3nk`/`.3nK`) |
| Encryption | `3nK` header on `locale.scs`, `def.scs`, `base_cfg.scs` |

### Pipeline
```
Game data → *.scs (SCS#) → extract → *.sii / *.pmg / *.pmd / *.mat / *.tobj / *.dds
Encrypted: *.3nk / *.3nK → decrypt → *.sii.txt
```

---

## 2. SCS Archive Format (`.scs`)

### Spec
```
Magic: "SCS#" (4 bytes)
Version: uint32
Magic: "CITY" (4 bytes)
File count: uint32
Offset 0x1000: File table begins

Per file (24 bytes):
  uint32 HASH1
  uint32 HASH2
  uint32 OFFSET
  uint32 NULLS
  uint32 FLAG
  uint32 CRC
  uint32 SIZE (decompressed)
  uint32 ZSIZE (compressed)
Filename = "%08X%08X" % (HASH1, HASH2)
If SIZE == ZSIZE: stored; else: zlib compressed
```

### Hash Algorithm
- Custom hash (long MASM32 function by Ekey)
- Recovery: Unpack `def.scs` → search for paths in files (e.g., `model_desc: "/model/sign/..."`)
- **Tool**: **SCSHasher** (Ekey 0.1) — generates hashes from known paths
- Community builds hash dictionaries from collected filenames

### Encryption (`.3nk` / `.3nK`)
- **Header**: `3nK` (little-endian)
- **Files**: `locale.scs`, `def.scs`, `base_cfg.scs` contain encrypted `.sii` files
- **Tool**: **SCSSIITool** (Ekey) — `SCSSIITool <InFILE> <OutFILE>`
  - Input: extracted file with `3nK` header
  - Output: decrypted `.sii` (text) or `.txt`
- **QuickBMS**: aluigi's `scsgames.bms` supports SCS# + ZIP + 3nK decryption
  - aluigi.org/papers/bms/scsgames.bms

### Repacking
| Tool | Status |
|------|--------|
| QuickBMS (aluigi) | Script supports reimport — but encryption reimport fails |
| Ekey tests | "Game don't work after reimport" (2013) — encryption callback issues |
| aluigi fix | QuickBMS 0.5.17c fixed reimport bug — game still rejects reencrypted `locale.scs` |
| **Workaround** | Mod via loose files / overwrite in mod directory (SCS mod system) |

### Mod System
- Native: SCS games load `.scs` mods from `mod/` folder (ZIP-based)
- Priority: Mod archives override base game
- **Best practice**: Package mods as `.scs` (ZIP) — no need to repack base game

---

## 3. SII Format (Text Config)

### Structure
- Custom text format (key-value, nested)
- Extension: `.sii` (binary/encrypted) → `.sii.txt` (decrypted)
- Example:
```
model_desc: "/model/sign/navigation/3hw_exit_far_d.pmd"
model_coll: "/model/sign/navigation/3hw_exit_far_d.pmc"
```

### Editing
- Decrypt with SCSSIITool → edit text → re-encrypt (problematic)
- **Alternative**: Create mod with modified `.sii` in loose file structure

---

## 4. Model Formats (Prism3D)

### File Types
| Extension | Description |
|-----------|-------------|
| `.pmg` | Model geometry (Prism3D Model Geometry) |
| `.pmd` | Model descriptor (references .pmg, .mat, .tobj) |
| `.mat` | Material definition |
| `.tobj` | Texture object (references .dds) |
| `.dds` | Texture (DXT/BC compressed) |
| `.pmc` | Collision mesh |
| `.skl` | Skeleton (rare) |

### PMG Format (Geometry)
- **Versions**: 0x11, 0x13, 0x15 (ETS2/ATS Wyoming+)
- **Structure**: Vertex buffers (pos, normal, uv, tangents), index buffer, submeshes
- **Vertex formats**: Multiple (static, skinned, instanced)

### Tools
| Tool | Author | Notes |
|------|--------|-------|
| **SCS Blender Tools** | SCS Software | Official — modding.scssoft.com/wiki/Documentation/Tools/Download |
| **ConverterPIXWrapper** | simon50keda | github.com/simon50keda/ConverterPIXWrapper — PMG/PIX → Blender |
| **3D Object Converter** | Karpati | v10.001+ supports PMG 0x11/0x13/0x15 — 3doc.i3dconverter.com |
| **Noesis** | Durik256 / Sharppy | Scripts: `fmt_pmg.py`, `fmt_euro_truck_sim_pmg.py` |
| **SCSArchivier** | Community | Extract .scs archives |

### Batch Conversion Pipeline (Noesis)
1. Extract `.scs` → get `.pmg`, `.pmd`, `.mat`, `.tobj`, `.dds`
2. Noesis script reads `.pmd` → finds `.pmg` + textures
3. Batch export → OBJ/FBX/STL
4. **Sharppy's script** (2022): `fmt_euro_truck_sim_pmg.py` — handles material/texture linking

---

## 5. Texture Formats

### DDS
- **Format**: Standard DDS with DXT1/BC1, DXT5/BC3, BC5, BC7
- **Mipmaps**: Yes
- **Tools**: Any DDS viewer, texconv, Paint.NET, GIMP

### TOBJ (Texture Object)
- **Format**: Text (`.tobj`) or binary
- **Content**: Texture path, wrap mode, filter, alpha test, etc.
- **Example**:
```
texture: "/vehicle/truck/man_tgx/texture.dds"
wrap: repeat
filter: linear
```

---

## 6. Audio

| Aspect | Detail |
|--------|--------|
| Engine | FMOD (legacy) / custom |
| Files | `.bank`, `.wem` (Wwise?) — not extensively documented in threads |
| Tools | FMOD Studio, Wwise, vgmstream |

---

## 7. Localization / Language Files

### Structure
- **Encrypted**: `locale.scs` → contains `.3nk` files
- **Decrypted**: `.sii` text files with key-value strings
- **Languages**: Each language = one `.3nk` file in `locale.scs`

### Workflow
1. Extract `locale.scs` (QuickBMS `scsgames.bms`)
2. Decrypt `.3nk` → `.sii.txt` (SCSSIITool)
3. Edit text
4. **Problem**: Re-encryption + repack breaks game
5. **Workaround**: Use SCS mod system with loose `.sii` files

---

## 8. Key Repositories & Tools

| Tool / Repo | Author | Purpose |
|-------------|--------|---------|
| **scsgames.bms** | aluigi | QuickBMS: SCS# + ZIP + 3nK decrypt |
| **SCSSIITool** | Ekey | Decrypt 3nK → SII text |
| **SCSHasher** | Ekey | Generate file hashes from paths |
| **SCS Blender Tools** | SCS Software | Official Blender import/export |
| **ConverterPIXWrapper** | simon50keda | PMG/PIX → Blender (open source) |
| **3D Object Converter** | Karpati | Commercial: PMG 0x11/0x13/0x15 support |
| **Noesis scripts** | Durik256, Sharppy | Batch PMG → OBJ/FBX |

---

## 9. Gaps / Unresolved
- **3nK re-encryption**: No working tool to encrypt edited SII back to 3nK
- **SCS# repacking**: QuickBMS reimport fails on encrypted archives
- **Hash algorithm**: Not publicly documented (Ekey's MASM32 code only)
- **Skeleton/animation**: `.skl` format not documented
- **PMG version 0x15+**: Newer ATS versions may have updates
- **Material system**: `.mat` format not fully documented

---

## 10. Cross-References
- **Prism3D engine**: Shared across ETS2, ATS, Bus Simulator, Scania Sim
- **SCS modding wiki**: modding.scssoft.com — official documentation
- **QuickBMS**: Universal extractor — `scsgames.bms` reusable
- **Blender SCS Tools**: Official pipeline for asset creation
- **Noesis**: Generic model viewer — scripts portable