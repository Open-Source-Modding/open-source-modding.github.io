# installpackage-over-patch DLL Hex Edit

> **Cross-reference**: Patch archive behavior (patch.fat/dat overrides content) → [XeNTaX Watch Dogs knowledge §1.1](../../unsorted/xentax-watchdogs-knowledge.md)

The Disrupt engine loads archives in priority order. By default, `patch` has
highest priority and `installpackage` is unused. A hex edit to the game DLL
reorders this so `installpackage` loads first — letting you ship loose mod files
without repacking the ~16GB `patch` archive.

## The Patches

Two byte patterns in every Disrupt DLL (WD1/WD2/WDL):

```
search:  48 b8 17 00 00 00 39 00 00 00    replace 39 → 2b
search:  c7 86 00 0d 00 00 88 13 00 00    replace 88 13 → 00 00
```

**Applies to all Disrupt DLLs:**
- WD1: `Disrupt_b64.dll`
- WD2: `Disrupt_64.dll`
- WDL: `DuniaDemo_clang_64_dx11.dll`, `DuniaDemo_clang_64_dx12.dll`

Same bytes since forever (jason098 / Cobra, 7/8/21).

## How It Works

The DLL hardcodes archive load priority. In WDL:

| Priority (high→low) | Archive |
|---------------------|---------|
| 1 | `patch` |
| 2 | `common` |
| 3 | `worlds\london\london` |

The hex edit changes the priority constants so `installpackage` loads first:

| Priority (high→low) | Archive |
|---------------------|---------|
| 1 | `installpackage` |
| 2 | `patch` |
| 3 | `common` |
| 4 | `worlds\london\london` |

Since `installpackage` is unused by default, you install mods there and never
repack `patch` again. It's a **permanent** fix (Pesky Fly, 7/26/25).

## Caveats

- **BattlEye**: editing the DLL trips BattlEye's modified-file check. Launch
  with `-BattlEyeLauncher` in launch arguments to bypass. (BattlEye was removed
  in the latest WDL version, so this no longer applies there.)
- **`PreparePlatformData.ini`**: has an `installpackage` param, which is why the
  load path exists (Troplo noted this).

## Workflow

1. Patch the DLL with the hex edit (or use `installpackage_patcher.py`)
2. Create `installpackage/` folder next to the game EXE
3. Place mod files there (same folder structure as the game)
4. Launch — the game loads from `installpackage` first, falling back to `patch`

## Tool

`installpackage_patcher.py` — Python script that applies both patches,
backs up the original as `.bak`, and supports `--restore`.

```bash
python3 installpackage_patcher.py "path/to/DuniaDemo_clang_64_dx12.dll"
python3 installpackage_patcher.py --restore "path/to/DuniaDemo_clang_64_dx12.dll.bak"
```
