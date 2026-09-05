# Disrupt Engine — Tool Gotchas

Cross-game tool compatibility issues and known problems.

## Gibbed.Disrupt

- **Breaks at archives >4GB** (The Pesky Fly confirmed). Large WDL patch
  archives may exceed this limit.
- **`Gibbed.Disrupt.ConvertBinaryObject.exe`** (WD2 version) works for WDL
  binary objects. Use the WD2 version, not the WDL-specific one.

## Unpack / Pack Tools

| Tool | Works For | Notes |
|------|-----------|-------|
| **UnpackLegion** | WDL | Correct tool for Watch Dogs: Legion archives |
| **UnpackWD2.exe** | WD2 only | **Wrong for WDL** (qstlijku confirmed) — produces corrupt output |
| **PackLegion** | — | **Outdated** — do not use |
| **ManageLegion** | — | **Outdated** — do not use |

## DisruptManager (rootCBR)

- Does **NOT** implement the installpackage trick — only packs into `patch*`
  archives.
- Archive priorities for WDL: `patch` > `common` > `worlds\london\london`.

## FCBastard

- Latest version works for **WD1 only**.
- **Vector3 buffer overflow** on `colorColor` crashes WD2 and WDL.

## WD2 XBG Binary Import

- The Blender addon's XBG binary import (`import_wd2_xbg.py`) works for
  **static props only** (buildings, vehicles, props).
- **Character models** (pers09, wrench, etc.) have a different binary format
  that needs further reverse-engineering. Use `.glm` text import for character
  models.

## Cross-References

- [Engine Lineage](engine-lineage.md) — why tools share conventions across games
- [installpackage Patch](installpackage-patch.md) — archive priority hex edit
- [WDL Modding Workflow](watch_dogs_legion/modding-workflow.md) — step-by-step WDL modding
