# Cities: Skylines — XeNTaX Forum Knowledge

Extracted from XeNTaX forum discussions (2015–2022). Covers localization
(`.locale` files) and the `.crp` save/asset container format for maps and assets.

---

## 1. Localization: `.locale` Files

### Overview

Cities: Skylines stores localized text in **`.locale` files** under
`Cities Skylines\Files\Locale` (one per language, e.g. the English file).
The format is a proprietary/compiled binary; it is **not** plain text.
*(SkyF1, 2015-03-14; Fodosan, 2015-04-11)*

### Editing approach

- A working method for editing `.locale` files was posted to the Steam
  Community forums for app 255710 (Cities: Skylines) in March 2015.
  *(SkyF1, 2015-03-15; confirmed by WRS, 2015-04-11)*
- No in-thread format spec is provided — the practical answer points to the
  Steam Community guide/discussion rather than a standalone tool.

### Fonts

- Localization also needs the game **font** (which may not cover all scripts).
  The font file location was an open question; no definitive answer is given
  in this dump. *(Fodosan, 2015-04-11)*

---

## 2. `.crp` Files: Maps, Assets & Extraction

### Overview

A saved **map** in Cities: Skylines is a single **`.crp`** file — the same
container format used when you create any **asset** in-game. All information
about building/object **coordinates and placement** is stored in the `.crp`.
*(eyewee, 2018-08-12)*

### What's in a `.crp`

- A `.crp` contains both the **mesh/data** and a **`.bin`** portion with the
  placement/coordinate information.
- A GitHub tool exists that extracts **map preview textures** from a `.crp`,
  but the rest (coordinates in the `.bin`) is **not** extracted by that tool.
  *(eyewee, 2018-08-12)*
- Third-party mesh extraction: the in-game **modTools** can extract 3D meshes
  placed on a map, but does **not** save coordinates needed to recreate the
  map layout in an external 3D application. *(eyewee, 2018-08-12)*

### Recreating a map externally

As of the 2018 thread, there was **no known way** to fully recreate a
Cities: Skylines map (meshes + coordinates) in a third-party application —
the coordinates live in the `.bin` and were not extracted.

### Format stability

As of 2022, a user asked for an unpacker compatible with the **latest patch
and latest `.crp` files**, or a format description; a sample request was made
but no public spec/tool was provided in the thread. *(jfwfreo, ikskoks,
2022-02-12)*

---

## 3. Open Questions

- `.locale` file binary format spec — not documented in the dump (solution
  points to a Steam Community guide).
- Game font file location for non-Latin localizations — unresolved.
- `.crp` `.bin` placement/coordinate format — not publicly documented.
- `.crp` format description / up-to-date unpacker for modern patches — open.

---

## 4. Tools Mentioned

| Tool | Purpose |
|------|---------|
| GitHub `.crp` preview tool | Extracts map preview textures from `.crp` (not full data) |
| modTools (in-game) | Extracts 3D meshes placed on a map (no coordinates) |

---

*Source: XeNTaX forum threads t=12681, t=12761 (localization), t=18672
(Cities: Skylines Maps), t=25039 (Cities: Skylines CRP files).*
