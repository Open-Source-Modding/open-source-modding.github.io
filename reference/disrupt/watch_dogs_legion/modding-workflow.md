# Watch Dogs: Legion — Modding Workflow

Step-by-step workflow for modding Watch Dogs: Legion.

## Prerequisites

- **UnpackLegion** — the correct tool for WDL archive extraction
- **DisruptManager** (rootCBR) — for packing and installing mods

## Workflow

### 1. Unpack the game data

Use **UnpackLegion** to extract `patch.dat`/`patch.fat`:

```
UnpackLegion.exe patch.dat
```

> **⚠ Do NOT use** Gibbed's Unpack tool or PackLegion — they are outdated and
> produce incorrect results for WDL archives.

### 2. Edit files

Modify extracted files as needed — binary objects, textures, materials, etc.

For binary object editing, `Gibbed.Disrupt.ConvertBinaryObject.exe` (the WD2
version) works for WDL binary objects.

### 3. Pack and install mods

Use **DisruptManager** (rootCBR) to pack modified files into `patch*` archives
and install them.

> **⚠ DisruptManager does NOT implement the installpackage trick.** It only
> packs into `patch*` archives. See below for the installpackage workaround.

## Archive Priority

WDL loads archives in this order (highest priority first):

1. `patch` (and `patch0`, `patch1`, etc.)
2. `common`
3. `worlds\london\london`

## installpackage Workaround

The `installpackage` archive is unused by default. A hex edit to the game DLL
(`DuniaDemo_clang_64_dx11.dll` / `DuniaDemo_clang_64_dx12.dll`) reorders
priority so `installpackage/` loads before `patch`:

```
search:  48 b8 17 00 00 00 39 00 00 00    replace 39 → 2b
search:  c7 86 00 0d 00 00 88 13 00 00    replace 88 13 → 00 00
```

**Once patched, it's permanent** — install mods into `installpackage/` and
never repack the ~16GB `patch` archive again.

> **Cross-reference**: Full hex edit details → [installpackage Patch](../installpackage-patch.md)

> **⚠ Editing the DLL trips BattlEye's modified-file check.** Bypass with
> `-BattlEyeLauncher` launch arg (BattlEye removed in latest WDL version).

## Cross-References

- [Tool Gotchas](../tool-gotchas.md) — tool compatibility issues across games
- [Engine Lineage](../engine-lineage.md) — why Disrupt tools share conventions with Far Cry
- [PreparePlatformData Pipeline](prepareplatformdata-pipeline.md) — official build pipeline
