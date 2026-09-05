# Cities: Skylines — Localization & CRP Formats

> **Source**: XeNTaX forum knowledge extraction (2015–2022).
> Covers `.locale` localization files and `.crp` save/asset container format.

---

## 1. Localization: `.locale` Files

### Overview
Cities: Skylines stores localized text in **`.locale` files** under:
```
Cities Skylines\Files\Locale
```
(One per language, e.g., English file). The format is a **proprietary/compiled binary** — NOT plain text.

### Editing Approach
- Working method for editing `.locale` files posted to Steam Community forums for app 255710 (Cities: Skylines) in March 2015
- No in-thread format spec provided — practical answer points to Steam Community guide/discussion rather than standalone tool

### Fonts
- Localization also needs the game **font** (may not cover all scripts)
- Font file location was an open question; no definitive answer in dump

---

## 2. `.crp` Files: Maps, Assets & Extraction

### Overview
A saved **map** in Cities: Skylines is a single **`.crp`** file — same container format used when creating any **asset** in-game. All building/object **coordinates and placement** stored in the `.crp`.

### What's in a `.crp`
- Contains both the **mesh/data** and a **`.bin`** portion with placement/coordinate information
- GitHub tool extracts **map preview textures** from `.crp`, but rest (coordinates in `.bin`) NOT extracted by that tool
- In-game **modTools** can extract 3D meshes placed on a map, but does **not** save coordinates needed to recreate map layout externally

### Recreating a Map Externally
As of 2018 thread: **no known way** to fully recreate a Cities: Skylines map (meshes + coordinates) in third-party application — coordinates live in `.bin` and were not extracted.

### Format Stability
As of 2022: user asked for unpacker compatible with latest patch and latest `.crp` files, or format description; sample request made but no public spec/tool provided in thread.

---

## 3. Open Questions
- `.locale` file binary format spec — not documented (solution points to Steam Community guide)
- Game font file location for non-Latin localizations — unresolved
- `.crp` `.bin` placement/coordinate format — not publicly documented
- `.crp` format description / up-to-date unpacker for modern patches — open

---

## 4. Tools

| Tool | Purpose |
|------|---------|
| GitHub `.crp` preview tool | Extracts map preview textures from `.crp` (not full data) |
| modTools (in-game) | Extracts 3D meshes placed on a map (no coordinates) |

---

## Key Facts
- `.locale` = proprietary binary (not plaintext), editing via Steam Community guide
- `.crp` = unified map/asset container (mesh + `.bin` coordinates)
- No public `.crp` coordinate extraction — modTools extracts meshes only, no coordinates
- Full map recreation in external 3D app = not possible (coordinates unavailable)
- Format spec for modern `.crp` = open (as of 2022)