# Archive Load Priority (Disrupt Engine)

The Disrupt engine loads game data from `.dat`/`.fat` archive pairs in a
specific priority order. Higher-priority archives override lower ones for
identical file paths.

## Per-Game Priorities

Decompiled from `DisruptManager.Model.GameInfos.*` → `ArchivePriorities`:

| Game | Priority 1 (highest) | 2 | 3 | 4 | 5 (lowest) |
|------|---------------------|---|---|---|------------|
| **WD1** | `patch1` | `patch` | `common` | `worlds\windy_city\windy_city` | — |
| **WD2** | `patch2` | `patch1` | `patch` | `common` | `worlds\san_francisco\san_francisco` |
| **WDL** | `patch` | `common` | `worlds\london\london` | — | — |

## Key Points

- **No `patch2` in WDL** — only `patch` exists. Repacking `patch` with
  everything produces ~30GB result.
- **`installpackage`** is unused by default but can be given highest priority
  via the DLL hex edit (see [installpackage-patch.md](installpackage-patch.md)).
- **Archive format**: WD1/WD2 use `Depload` format; WDL uses `FatFile`
  (BigFile) format.
- **Source**: `CBR.WatchDogsLegion.dll` (WDL), `CBR.WatchDogs*.dll` (WD1/WD2),
  `CBR.Disrupt.dll` (shared packing engine).

## Tool: DisruptManager

rootCBR's DisruptManager (`github.com/rootCBR/DisruptManager`) is a .NET WPF
app that manages mod installation. Key classes:

- `GameInfo.ArchivePriorities` — hardcoded load order per game
- `CBR.WatchDogsLegion.Formats.BigFile` — WDL `.fat` format
- `CBR.WatchDogs*.Formats.Depload` — WD1/WD2 format
- `CBR.Disrupt.Packing.*` — Gibbed archive engine (pack/unpack)

**Does NOT implement** the installpackage-over-patch trick — only packs/installs
mods into `patch*` archives. Planned to be open-sourced (issue #30).

## Launch Parameters

| Game | EXE | Launch Params |
|------|-----|---------------|
| WD1 | `Watch_Dogs` | *(none)* |
| WD2 | `WatchDogs2` | `-eac_launcher` |
| WDL | `WatchDogsLegion` | *(none)* |
