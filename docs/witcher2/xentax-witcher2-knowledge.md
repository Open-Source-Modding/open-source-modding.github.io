# The Witcher 2 — XeNTaX Forum Knowledge

Extracted and organized from XeNTaX forum discussions (2011–2022). Covers the
REDEngine 2 file formats of The Witcher 2: Assassins of Kings — the `.w2ent`
entity containers, `.w2mesh` meshes, `.xbm` textures, `.w2speech` dialogue
audio, `.usm` video, and `.fsb` music — plus the community tools used to unpack
and import them (Gibbed's RED Tool, chrrox's QuickBMS scripts, Szkaradek123's
Blender 2.49 importer, REDkit).

---

## 1. Archive Unpacking (pack0.dzip)

The game's data is packed in **`.dzip`** archives (e.g. `pack0.dzip`). The
community unpacked them with either:

- **Gibbed's RED Tool** (WitcherNexus file id 52) — unpacks `pack0.dzip`.
  *(RobeN, 2011-06-03, t=6660)*
- **chrrox's QuickBMS script** for unpacking `pack0.dzip` (`w2_dzip_unpack.bms`).
  *(Szkaradek123, 2011-06-03; srcs34k, 2018-04-17, t=6660/t=13838)*

When importing into Blender, select the **entire unpacked game directory**
(`pack0` root), **not** the texture folder — the importer needs to read all the
textures across the tree. *(Szkaradek123, 2011-05-30; odrin, 2011-06-07, t=6660)*

## 2. Models: `.w2ent` vs `.w2mesh`

The Witcher 2 stores its models in **`.w2ent`** (entity) files and standalone
**`.w2mesh`** files.

- `.w2ent` files **contain the mesh data** — they are not just references.
  *(odrin, 2011-06-10, t=6660)*
- Not all `.w2mesh` data lives inside `.w2ent` files; when a `.w2ent` contains
  `.w2mesh` data, the importer opens the package and lets you select them.
  *(CMihai, 2011-09-22, t=6660)*
- `.w2ent` files usually contain data other than `CMesh` objects too (camera
  config, entity settings). *(Rick, 2011-06-10; SkacikPL, 2015-06-09, t=6660/t=12869)*
- Model location example: `pack0.dzip\game\npc\riszon.w2ent` holds
  `richon__body1_b1.w2mesh` (Letho). *(MichaelDarkAngel, 2012-06-01, t=6660)*

### Standalone vs embedded mesh data differ

shakotay2 compared the same mesh (`scoiatael_woman__body1_b1`) inside a
5.22 MB `.w2ent` vs a standalone 873 kB `.w2mesh` — **the data structures are
different**, so a script that loads one does not trivially load the other.
*(shakotay2, 2013-07-31, t=6660)*

```
# inside q102_elf_woman_hanger.w2ent   (embedded)
# 1.   e0d2d:
v 0.178874 -0.028297 1.424668
# 00 01 02 03  92 4F 19 04  86 00 80 FF  FF FF FF FF
vt 0.348128 0.709508

# standalone scoiatael_woman__body1_b1.w2mesh
# 1.     759:
v 0.178874 -0.028297 1.424668
# 00 01 02 03  0.574310 0.310812 0.098458 0.016420 0.055559 -0.998417 0.008791
# FF FF FF FF
```

## 3. Textures: `.xbm` → DDS

`.xbm` texture files are essentially **DDS files without the DDS header** —
usually **DXT1 or DXT5**. *(chrrox, 2011-05-20, t=6649)*

- To convert: throw a DDS header on them and you're done. *(chrrox,
  2011-05-20, t=6649)*
- The community produced a **witcher2_texture_converter** (QuickBMS + cmd
  scripts) for `.xbm` ⇄ `.tga`/`.dds` conversion, based on chrrox's initial
  work. *(hhrhhr, 2015; srcs34k, 2018-04-17, t=13838)*
- The converter runs with `quickbms.exe` placed next to the unpacked files,
  producing `filename_DXT(1,5).bin` (binary info) and `filename_DXT(1,5).tga`
  (the texture). *(hhrhhr via ellegirl01, 2016-01-17, t=13838)*

**Relevant QuickBMS scripts** (from the witcher2_texture_converter pack,
srcs34k, 2018-04-17, t=13838):
- `w2_xbm2dds.bms` — `.xbm` → `.dds`
- `w2_dds2xbm.bms` — `.dds` → `.xbm`
- `w2_dzip_unpack.bms` — unpack `.dzip`

## 4. Blender Importer (Szkaradek123)

**Szkaradek123's Blender 2.49 importer** was the primary tool for importing
Witcher 2 models (geometry, weights, UVs, textures, bones). *(Modman69,
2011-05-30, t=6649)*

### Requirements & usage

- **Must use Blender 2.49** with **Python 2.6** (not 2.5x / Python 3.x) — the
  scripts are not compatible with Blender 2.5+. *(Szkaradek123, 2011-06-01;
  Tosyk, 2011-06-07, t=6660)*
- Run from the **Scripts window** (open the `.blend`, right-click the script
  window, Execute). *(Tosyk, 2011-06-07, t=6660)*
- Select the **unpacked game dir** (the whole `pack0`), load a `.w2ent` model.
  *(Szkaradek123, 2011-06-03, t=6660)*
- The importer reads all needed textures and converts them to `.dds`; if it
  can't, the script breaks. *(Szkaradek123, 2011-05-30, t=6660)*
- Some models error out because the Witcher 2 file format was **not 100% known**.
  *(Szkaradek123, 2011-05-30, t=6660)*

### Version history (feature additions)

| Version date | Added |
|--------------|-------|
| 2011-06-05 | materials, bones (no joints) |
| 2011-06-23 | 'magic' button, better mesh names, texture import, static meshes, bone parenting |

*(Szkaradek123, 2011-06-05 & 2011-06-15, t=6660)*

The **'magic' button** fixes `.w2ent`/`.w2mesh` files that fail to import
correctly — mainly body meshes. *(soulslayerzx, 2013-08-13, t=6660)*

### Common troubleshooting

- **Textures don't apply** → load them manually from the character folder
  (search `.dds`), or use the UV/Image editor; a "single user" material
  workaround helps when the hair texture covers the body and vice versa.
  *(jcarl904, CMihai, Modman69, 2011-06-03..06-05, t=6660)*
- **No face** → faces are stored in
  `\templates\characters\appearances\main npc`. *(CMihai, 2011-06-05, t=6660)*
- **Bones import but aren't connected** → parent them manually
  (select armature → TAB → shift+RMB child+parent → ctrl+P → Keep Offset), then
  remove doubles and disable deform on roll joints. *(Szkaradek123, CMihai,
  2011-06-11..06-17, t=6660)*

### Armature / rigging notes

- The bones are all present but **not connected**; parenting them is manual.
  *(Ladoo, 2011-06-11, t=6660)*
- Most characters share a **common bone structure** — one saved armature works
  for many models (Geralt, Iorweth). *(CMihai, 2011-06-17, t=6660)*
- Roll joints (elbows, forearms, hands) need manual re-parenting to bend
  correctly. A documented arm hierarchy: `Bicep2 → Bicep → ShoulderRoll →
  Forearm/Elbowroll → Forearmroll1/2 → Handroll → Hand`. *(CMihai, 2011-06-17,
  t=6660)*
- Exporting to 3ds Max: use the **Blender 2.49 DAE (or FBX) exporter**, or the
  **md5mesh exporter** for the best Blender↔Max cross-import, then Max→FBX→UDK.
  *(junk angel, shakotay2, zardalu, 2012-01..2013-08, t=6660)*

## 5. Dialogue Audio: `.w2speech`

`.w2speech` files contain **localized dialogue**. *(tomatofarmer, 2013-01-25,
t=10079)*

- The audio is thought to be **MPEG layer 2 @ 48000 Hz** (decodable as such),
  with at least 21,568 individual samples. *(tomatofarmer, 2013-01-25, t=10079)*
- A voice-over is referenced as `VO_<1>_<2>_<3>`:
  - `<1>` = voice tag id (e.g. `TRIS` for Triss)
  - `<2>` = scene/set number (all VOs in a scene usually share it, e.g. `300202`)
  - `<3>` = line number (e.g. `0343`)
  - Example: `VO_TRIS_300202_0343` (from `q104_triss_interaction_oneliner.w2scene`)
  *(tomatofarmer, 2013-01-25, t=10079)*
- The header format was **not fully documented**; individual samples were hard
  to find. *(micTronic, 2021-12-06, t=10079)*
- **REDkit extracts all speech automatically** after install, and you can run
  the extraction manually (see §7). The extractor only outputs the **already
  extracted** samples — it can't convert other-language `.w2speech` that isn't
  in the same format. *(micTronic, 2021-12-07; nikich340, 2021-12-09, t=10079)*

## 6. Video: `.usm` (Xbox 360)

The Witcher 2 Xbox 360 version uses **`.usm`** files for cutscenes.
*(MiLØ, 2012-04-18, t=8789)*

- Demux to **M2V + ADX** with **VGMToolbox**; convert ADX → WAV, then mux into
  MKV with **mkvmerge**. *(MiLØ, 2012-04-18, t=8789)*
- Some `.usm`-derived `.m2v` streams play in VLC but are **not recognized by
  mkvmerge** (end-of-file / unknown type) — possibly a slightly different MPEG
  encode inside those particular files. *(MiLØ, 2012-04-18, t=8789)*
- VGMToolbox was the only widely known USM demuxer at the time. *(MiLØ,
  2012-04-18, t=8789)*

## 7. REDkit (official modding tool)

**REDkit** (RedKit, CD Projekt RED) is the official Witcher 2 editor, released
May 2013. *(Modman69, 2013-05-17, t=6660)*

- As of 2013 it had **no import/export for skeletal meshes**; it could only
  export static meshes as `.3ds`, `.obj`, and `.fbx` (no characters/NPCs).
  *(Modman69, 2013-05-17, t=6660)*
- It ships a **3DS Max 2009/2013 plugin** that exports only the `.re` static
  mesh format. *(Modman69, 2013-05-17, t=6660)*
- REDkit also **extracts all speech** (VO) — run the editor, or use:
  ```
  "<SteamLibrary>\steamapps\common\the witcher 2\bin\editor.exe" exportstrings exportvo
  ```
  This extracts the currently-selected Steam language. *(micTronic, 2022-03-17,
  t=10079)*

## 8. Music: `.fsb` (Unknown codec)

Early on the Witcher 2 music `.fsb` files used an **unknown codec** that
`fsbext` couldn't identify and `towav`/other FSB tools wouldn't decode.
*(OrangeC, 2011-05-16, t=6625)*

- A coder later released a **Witcher 2 music FSB unpacker** that worked great.
  *(OrangeC, 2011-05-23, t=6625)*

## 9. Tools Reference

| Tool | Purpose | Notes |
|------|---------|-------|
| Gibbed's RED Tool | Unpack `pack0.dzip` | WitcherNexus file id 52; used for W2, also supports W1/W3 later |
| chrrox QuickBMS scripts | Unpack `pack0.dzip` (`w2_dzip_unpack.bms`) | Referenced by Szkaradek123 |
| witcher2_texture_converter | `.xbm` ⇄ `.tga`/`.dds` | QuickBMS + cmd scripts; `w2_xbm2dds.bms`, `w2_dds2xbm.bms` |
| Szkaradek123 Blender 2.49 importer | Import `.w2ent`/`.w2mesh` | Geometry, weights, UVs, textures, bones; not 100% complete |
| VGMToolbox | Demux `.usm` (M2V + ADX) | For Xbox 360 cutscenes |
| mkvmerge | Mux M2V+ADX → MKV | Some M2V streams unrecognized |
| REDkit | Official W2 editor | Static mesh export only (`.3ds`/`.obj`/`.fbx`/`.re`); extracts VO |
| Blender 2.49 DAE/FBX exporter | Export to Max | Use md5mesh for best cross-import |

---

*Source: XeNTaX forum threads t=6625 (Unknown fsb format), t=6649 (.xbm/.w2mesh),
t=6660 (The Witcher 2 Models — Blender importer), t=8789 ([Xbox360] .USM),
t=10079 (w2speech file), t=12869 (W3 .w3strings entity config note), t=13838
(Unpacking XBM files? W2/W3).*
