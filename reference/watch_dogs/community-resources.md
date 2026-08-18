# Watch Dogs — Community Tools & References

A curated list of third-party tools, forum knowledge and modding workflows that
inform the Disrupt-engine reference work. Some are outdated or non-authoritative
— they are kept here for the *format confirmations* and *gameplay semantics*
they provide, clearly marked where applicable.

## HavokLib / HavokMax (PredatorCZ)

- **HavokLib** — independent C++ library (GPL-3.0) for Havok **packfile**
  serialize + XML, v5.0.0–2017, both endians, x86/x64, all compilers.
  Handles `hkaSkeleton`, `hkaAnimation*`, `hkRootLevelContainer`,
  `hkxEnvironment`; binary export for v5–2014.
- **HavokMax** — 3ds Max importer/exporter plugin built on HavokLib
  (archived 2022; development moved to HavokLib's `toolset/`).
- **Relevance:** the authoritative cross-reference for the **WD1 native Havok
  packfile** (`import_hkx_wd.py`). It does **not** support tagfiles ("never
  will"), so it does *not* apply to WD2/WDL Disrupt `.hkx` (tagfile wrapper) —
  HavokDisrupt's port remains authoritative there.

## OpenDogs tools (Nexus, WD2)

Watch Dogs 2 UWP tools that bundle an embedded Blender 2.79 + Volfin's
`import_WD2.py`:

- **OpenDogs FileSystem** — 119 **Gibbed.Disrupt filelists** for WD2 (~604k
  paths, 60% coverage) and WDL. Format: `;` progress header, a GUID line, then
  one path per line (`\`-separated). Useful as a hash→path database companion
  to the FNV-1a 64-bit `hash_wd2.py` utility (practical value mostly on the
  Fat/hash side, since `.xbg` materials store paths as plaintext Wstr).
- **OpenDogs NewUI / 3D View** — WinUI 3 viewers. Bundle `xbgtools/` with
  Volfin's `import_WD2.py` (already ported into the addon) and
  `convert_xbg_to_obj.py` (a thin Blender CLI wrapper).
- **Volfin importer confirmations** (from the bundled `import_WD2.py`):
  vertex strides 40/36/32/28/24/20; 16-bit half-float UVs; MOEG 0x89/0x46
  version gate; `.skel` (nbCF v3) reading.

## ZModeler 3 forum posts (Oleg — **outdated, not authoritative**)

These predate full file-format RE and treat parts of the format as immutable,
but document **gameplay/modding semantics** that still hold:

### Materials (`*.material.bin` + `engine\shaders\materialdescriptors`)

- Material name matters: e.g. a material named `VehicleName_CarPaint_01` gets
  the game's *variative paint color scheme* automatically (settings ignored).
- **`Damage` user property** — comma-separated case-sensitive object names,
  where the *position* is the damage-part index (`,,,TireFront_L` → L=#4,
  R=#5). Indices are **coded in the collision `.hkx`** — retain original
  indices or in-game damage (windshield cracks, tire deflate) will map to the
  wrong parts.
- Shader-parameter overrides can force any materialdescriptor parameter by name.

### Scene setup, hierarchy, LODs

- Export roots on a dummy node named like the output file; name suffixes
  toggle export scenarios: **`.rigid`** (rigid skinning — vehicles, whole
  volume split into parts), **`.skel`+`.mesh`** (blend skinning — characters,
  up to 8 bones/vertex in the tool, 6 in WD1), no suffix = regular objects
  (weapons/scenery).
- Blend skinning models carry "unknown fragments" reused from the original —
  interactive physics (movement/wind) is lost without them.
- **LODs**: UH/VH/H/M/L/VL scene states; `LODs` property on the root dummy is
  comma-separated switch distances (`10, 20, 30, 45, 90, 250`).
- Hierarchy dummies in square brackets (`[DoorFront_R_Hinge]`) are export-time
  only — the **`.skeleton` file** is required to retain them (vehicle hinge
  pivots for animations).

### Mod workflow (packing)

- `Gibbed.Disrupt.Pack.exe patch.fat <folder>` → single `patch.fat`/`patch.dat`
  pair dropped in the game root (WD1 honors one patch bundle).
- XBT = repacked DDS; header refinement is hex-edit work (a garbled but
  consistent sample: TBX magic, version byte 0x7B/0x8F family, 0x2C header
  size for DXT5).
