# Material BIN — Community Knowledge (Discord)

Signal extracted from raw Discord dumps (WD1 / WD2 / WDL channels).
Cross-references: [`material-bin-format.md`](material-bin-format.md) (the
010-template spec), [`material-hex-editing.md`](material-hex-editing.md).

---

## WD1 — selMaterialOverridesId and material lookup (2022-12-07)

Problem: a model part (e.g. jacket) has a `graphickitpartPart` BinHex name
(`BA710830`) but the material line needs a `selMaterialOverridesId` Enum value
(`1132120660`).

Workflow (The Silver):
1. In the model part's material list, take the `Name` field's **binhex** value.
2. Convert it from Byte Order **Intel** to **unsigned long** using BConv32.exe
 (57.55 KB, attachment) — binhex in `graphickit_parts`, enum in
 `graphickit_models`.
3. Use the result as `selMaterialOverridesId`.

BConv32.exe: hex-value converter (binhex ↔ enum ↔ decimal).

## WD1 — editing material.bin directly (2022-12-08)

- The material filename for a part comes from field `9301DFBD`; find it in
 `graphics/_materials` after unpacking `windy_city`.
- Editing options: hex edit (painful), ZModeler (export problems), or convert
 to XML and edit (round-trip not guaranteed). Best bet: convert to XML via
 the Disrupt Editor material tool.
- Materials control texture choice and coloring (per-part: jacket color,
 inner-shirt color, etc.).

## ZModeler import/export gotchas (WD1, 2025-04)

- **0 kb file**: export failed — one or more mesh parts (in any LOD) lack a
 proper material and use "Default Material".
- **Out of Memory**: importing an already-exported (broken) model. Avoid
 importing a model you exported as xbg in ZModeler.
- **Model deformed in viewer**: enter polygon-editing mode (unknown why it
 fixes it, but it does).
- **Missing physics**: when importing, check the model's original path and
 find its unknown files A–C. Not all models have them, but they must be in
 your export path.
- **Materials not loading**: import settings off, or missing materials in
 `windy_city_unpack`. ZModeler has two paths: **Root** = directory of your
 `windy_city_unpack` (e.g. `C:\Modding\bin\windy_city_unpack`), **Mod**
 folder = the folder `windy_city_unpack` resides in (e.g. `C:\Modding\bin`).

## Weapon reskins — custom addon weapons (2025-04-24)

- Renaming a weapon's material.bin (e.g. m468 → assault_hk416) works, but
 you **must also rename the .xbt files** to names that exist in your patch —
 otherwise you get reflective black guns.
- Reason: Reborn 2's custom addon weapons work because their depload/weapon
 entries ship in the patch. Without those entries, replace with an
 already-existing in-game gun.
- Note: text in the XML photo shows the referenced filenames.

## WDL — tools for recolors (2021-12-12)

- ZModeler does **not** work with WDL.
- For recolors you need a fan-made **Disrupt Editor** (originally made for
 WD1; no public link; unofficial — not the 300 GB official editor, which is
 a MEGA link, likely down; original torrent magnet exists, ~561 GB, too
 heavy for a laptop).
- The fan-made editor has a **material converter that works for all three WD
 games**. It can view the WD1 map, export layers to XML and import (WD1
 only), audio (SPK) conversion, material editor, and xbg2xml (WD1 only,
 and reverse).
- Cobra modified the source version to make layer import work for WD2
 (experimental support existed in DE; source no longer public).
- You must find the material of the clothing you want to edit — that alone
 is the painful part.

## Converted-material instability (2020-05-31)

- Editing only 2 materials caused crashes (sometimes on first load, sometimes
 on reload). Suspected cause: incomplete tools, not the game.
- Round-trip (material → XML → material) of a **vanilla** unedited material
 produces a different file — a few bytes differ at the end (same for
 zcancer). The Disrupt material editor does not pack identically; using it
 to edit certain materials results in crashes.
- ZModeler can remove settings on export — not to be trusted for this.