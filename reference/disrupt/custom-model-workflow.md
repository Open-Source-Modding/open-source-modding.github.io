# Custom Model Workflow (Watch Dogs 1/2)

> **Source**: Discord modding community — FrankMK04 (WD2 custom model injection), Mr. Bubsy (ZModeler WD1 workflow), community findings (2024-2026).
> **Cross-reference**: XBG format → [`xbg-format-geparser.md`](watch_dogs/xbg-format-geparser.md); Material hex editing → [`material-hex-editing.md`](watch_dogs/material-hex-editing.md); blender-io-disrupt addon → [`blender-addon.md`](blender-addon.md)

---

## Overview

Community members have successfully injected custom 3D models into Watch Dogs 1 and 2. This document consolidates the known workflows for creating and injecting custom XBG models, covering material creation, mesh export, and the XBGMIP workaround.

---

## WD2 Custom Model Process (FrankMK04)

### Material Creation

1. Start with an existing material (`.material.bin`)
2. Convert to XML using the material converter tool (Disrupt Editor Material Tool or similar)
3. Change texture paths to reference your new textures
4. Save with a different name — do not overwrite the original
5. Place the new material in the patch file alongside other modded materials

### Texture Placement

- Textures (`.xbt`) go in `graphics_geometries\locations\[yourpath]`
- Need diffuse + normal + other material layers as required by the shader
- Hash of custom material: use the `bot_spam` function on the Discord server, or `hash_tool.py` (`python3 hash_tool.py "string"`)

### XBG Injection

1. Create your 3D model in Blender or ZModeler
2. Export as XBG using the blender-io-disrupt addon or ZModeler
3. Replace the material hash in the XBG file (hex edit: find the original hash, replace with your material's hash)
4. Place the XBG in the patch file structure

### XBGMIP Hack (No High-Res Needed)

Some XBG files reference high-quality `.xbgmip` versions. FrankMK04 discovered a workaround that avoids needing to understand the XBGMIP format:

1. Delete the xbgmip hash from the XBG file (hex edit)
2. Replace `high.xbgmip` with your modified XBG (renamed to `.high.xbgmip`)
3. The game cannot find the original high-res, so it uses your modified XBG as LOD0
4. Original collision detection is retained

This means custom models can be created without reverse-engineering the XBGMIP format.

### Patch File Structure

Custom assets go in the patch archive (`patch.fat`/`patch.dat`), not in the game's base archives. Use `Gibbed.Disrupt.Pack.exe` or the `DisruptManager` tool to build patch bundles. Do not repack the game's original ~16GB patch archive — use the `installpackage` trick instead (see [`installpackage-patch.md`](installpackage-patch.md)).

---

## WD1 Custom Model Process (ZModeler)

### Prerequisites

- **ZModeler 3** (commercial, the only software that properly handles WD1 XBG import/export with materials)
- Unpacked `windy_city` archive
- Original model for weight copying

### Workflow

1. Import XBG into ZModeler with Root/Mod folder paths set correctly
2. Select LOD level (L0 = highest detail)
3. Edit the model in ZModeler, or export as `.DAE` for editing in Blender
4. Re-import the edited `.DAE` into ZModeler
5. Copy weights from the retail model to your model (Rigging > Copy Weights)
6. Attach your model to the retail model (Modify > Attach)
7. Separate by materials, delete the retail parts
8. Assign game materials via the Properties panel
9. Export as XBG

### Known Issues

- **Material path truncation**: ZModeler sometimes cuts the first character of material group names (`"raphics\materials"` instead of `"graphics\materials"`). Check the Materials tab before export.
- **Double rigging**: Custom models need the weight-copying process done twice — import retail, rig, attach, delete retail, import retail again, rig again.
- **Merge Models**: Some models (especially weapons) need "Merge Models" turned off.
- **Weight copy time**: Depends on vertex and bone count. Large models may take significant time.
- **Crash on fast actions**: ZModeler can crash on rapid undo operations. Save frequently.

---

## GLM Format (Beta Source)

GLM files are a beta model source format — the "structure for how XBGs will be created." They define mesh, materials, and skeleton in a binary format before compilation to XBG.

GLM files from the leak:

- `avat_ma_tor_bombersweater01.glm` (4.94 MB)
- `avatar01_tor_bombercoat01.glm` (5.13 MB)

GLM can potentially be converted to XBG for importing beta models into the retail game. The format is distinct from XBG and requires separate parsing (see GLM format investigation in `.opencode/todo.md` M15).

---

## Tools

| Tool | Purpose | Notes |
|------|---------|-------|
| blender-io-disrupt addon | XBG import/export/inject for Blender 5.0 | Our primary tool; handles WD1/WD2/WDL |
| ZModeler 3 | XBG import/export with material support | Commercial; only tool that fully handles WD1 XBG |
| Material Converter | XML to/from .material.bin conversion | Disrupt Editor fan tool; works across WD1/WD2/WDL |
| hash_tool.py | CRC32 + CRC64_WD2 hash calculator | `python3 hash_tool.py "string"` |
| Gibbed.Disrupt | .dat/.fat archive unpack/pack | dotnet 8.0; see `~/Documents/Code/game-tools/Ubisoft/Disrupt/Gibbed.Disrupt/` |
| Razor Data Conversion Utility | String to CRC32 to BinHex conversion | For manual material hex editing |
| DisruptEditor | Open-source C++ model editor (MIT license) | `~/Documents/Code/re/Ubisoft/Disrupt/DisruptEditor/` — patched working version; handles material conversion, XBG import/export. Source: [GitHub](https://github.com/Open-Source-Modding/DisruptEditor) |

---

## Cross-References

- [XBG Format (GeomParser Decompile)](watch_dogs/xbg-format-geparser.md) — binary format reference
- [Material Hex Editing](watch_dogs/material-hex-editing.md) — manual material parameter editing
- [Material Bin Format](watch_dogs/material-bin-format.md) — .material.bin structure
- [blender-io-disrupt Addon](blender-addon.md) — Blender addon documentation
- [Weapon Adding Guide](weapon-adding-guide.md) — adding new weapons (entitylibrary, items, depload)
- [Installpackage Patch](installpackage-patch.md) — permanent mod installation without repacking
- [Archive Load Priority](watch_dogs/archive-priorities.md) — per-game archive loading order
