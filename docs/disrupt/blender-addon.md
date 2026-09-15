# blender-io-disrupt — Blender Addon

Blender 5.0 add-on for importing, editing and re-exporting 3D models from
Disrupt engine games: **Watch Dogs 1**, **Watch Dogs 2**, and **Watch Dogs:
Legion**.

Source: `~/Documents/Code/game-tools/Ubisoft/Disrupt/blender-io-disrupt/`

## What it does

### Import

| Game | Format | What comes in |
|------|--------|---------------|
| **WD1** | `.xbg` | Skeleton, vertex groups (weights), UVs, normals, tangents, binormals, vertex colors, per-LOD meshes. LOD selector (pick one or all). |
| **WD2** | `.glm` (text) + `.xbg` (binary fallback) | Skeleton, vertex groups, UVs. Same binary path as WD1 when `.glm` header fails. |
| **WDL** | `.xbg` (MOEG binary) + `.skel` | Skeleton, vertex groups, UVs, normals. Packfile header parsing for section offsets. |

### Inject / Re-export

| Game | What you can do |
|------|----------------|
| **WD1** | Full rebuild: add/delete geometry, delete submeshes, join foreign meshes, re-skin from vertex groups. Writes to a new copy of the `.xbg`. Auto-updates companion `.high.xbgmip` for streamed-LOD vehicles. |
| **WD2** | Displacement-only: edit vertex positions, UVs, vertex colors, weights. Preserves materials/skeleton/physics blocks byte-for-byte. Outputs `.glm` text (ready for GLM2XBG converter) or injects into `.xbg`. |
| **WDL** | Displacement-only (vertex positions, UVs, normals, vertex colors, weights) or full fresh `.xbg` export from Blender (static props — no physics/procedural). |

### HKX Collision

| Game | Import | Inject |
|------|--------|--------|
| **WD1** | `hkx_collision_wd1` operator — parses `hkcdCompressedMeshShape` (v3), builds convex hull + box meshes | Yes — vertex displacement injected back byte-identical for unedited shapes |
| **WD2** | `hkx_collision_wd2` — parses TAG0 wrapper with `hclClothData` / `hclObject*.hkx` | Yes — same displacement-only injection |
| **WDL** | `hkx_collision_wdl` — HKX 2017.2 tagfile | Import only |

### Animation

- **WD1 + WDL**: `.mab` skeletal animation import (Oodle-compressed, quantized u8 curves). Resampling, helper-bone emulation, twist baking options.
- **WD1 only**: `.mac` animation clip import/export (binary curve format). `.markup` XML event import (timeline markers for footsteps, sounds, attacks).

### Materials

- **All games**: `.material.bin` import/export (TAM v7/v15). Maps PBR parameters (Base Color, Roughness, Metalness, Emissive, Normal) to Principled BSDF. Non-PBR params stored as custom properties for round-trip.
- **TFOWC2**: Extended material system (specular channel masks, metalness workflow) auto-detected and preserved.
- **Standalone `.skeleton`** import for WD1 and WDL.

### Other

- **FaceFX phoneme import** (WD1): `.txt` phoneme timeline → viseme shape key animation on the mesh.
- **XBT↔DDS utility**: `xbt_wd.py` converts compiled textures. Not wired into a Blender material pipeline — standalone script.
- **LOD peek** (WD1): check LOD count before importing.

## Module structure

```
modules/
  Core/           shared builder, logging, preferences
  Havok/          HKX collision parser, decompressor, injector
  Watch_Dogs/     WD1 import, export, inject, collision, materials, animation
  Watch_Dogs_2/   WD2 GLM/xbg import, export, collision, materials
  Watch_Dogs_Legion/  WDL import, export, inject, animation, collision
  UI/             game picker + per-game panels
```

Games share `Core/` and `Havok/`. WDL reuses WD1's MAB animation parser.
Game-specific import/export stays in each game's directory.

## Recent fixes (Sep 2026)

- **INT_MAX overflow** in WD1 XBG export: vertex indices exceeding 32K caused struct pack failures. Fix: `_safe_int()` clamps to `INT_MAX` in `disrupt_common.py`, used by `operators_wd.py`'s `_execute_export`.
- **Sharp edges / broken normals** in WD1 XBG export: the old exporter built a flat vertex list and used a shared normal array indexed by flat vertex index — wrong for per-face normals on hard edges. Fix: per-loop vertex buffer (`vtx_buf`) in `export_wd1.py` so each face vertex carries its own normal.
- **Material name mismatch**: WD1 materials were named `"material"` (hardcoded). Fix: `material_editor_wd.py` now uses the basename of the `.material.bin` file as the material name. Also handles packed materials (extracts filename from the hash-to-name map).
- **GLM parser skeleton bug** (WD2): `_parse_skeletons()` returns 0 bones despite 423 in file. Root cause: depth-tracking issue in `_skip_children()` — the parser loses sync when encountering `}` tokens. Under investigation.

## Known issues (under investigation)

- **WD2 binary XBG import crashes on character models**: The addon has a complete WD2 MOEG parser (`import_wd2_xbg.py`, 789 lines, ported from Volfin's io_scene_WD2) handling versions 0x89/0x46 (WD2) and 0x95/0x46 (WDL). However, character models like `pers09.xbg` (519KB) crash with `"unpack requires a buffer of 4 bytes"`. Root cause: the `_read_skip_mess()` function (ReflexSystem blob) has undocumented `entry_type` variants for character models with procedural animation nodes. The sentinel scan for geometry region offset may also fail on character files. **Fix needed**: add entry_type logging, debug the ReflexSystem blob structure for character models, and/or add fallback parsing paths. Spec: `reference/disrupt/wd2-moeg-xbg-format.md`.
- **WD1 `.xbgm` (XBGMIP)**: high-detail streamed LOD for vehicles. Parser exists but has no Blender export integration — parsed data is discarded. Format: 512-byte header + 2× packed vertex blocks + optional normals block. Doc: `reference/disrupt/xbgmip-format.md`.
- **GLM skeleton parsing**: see above — depth tracking in `_skip_children()` needs fixing.

## Limitations

- WDL inject is displacement-only — can't add/delete geometry or change vertex counts.
- WD2 doesn't support `.mab` animation import or standalone `.hkx` collision import from Blender.
- No texture baking pipeline — XBT conversion is a standalone utility, not wired into Blender materials.
- No scene/camera/anchor import — only meshes, skeletons, animation, and collision.
- LOD selection and control is WD1-only.
