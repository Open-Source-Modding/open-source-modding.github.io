# The Witcher 2 (REDengine 2) — Archive, Model, Texture & Audio Formats

> **Source**: XeNTaX forum knowledge extraction (2011–2022).
> Covers `.dzip` archives, `.w2ent`/`.w2mesh` models, `.xbm` textures, `.w2speech` dialogue, `.usm` video, `.fsb` music, and tools.

---

## 1. Archive Unpacking (`pack0.dzip`)

Game data packed in **`.dzip`** archives (e.g., `pack0.dzip`).

### Tools
| Tool | Purpose | Notes |
|------|---------|-------|
| Gibbed's RED Tool | Unpack `pack0.dzip` | WitcherNexus file id 52; used for W2, also supports W1/W3 later |
| chrrox QuickBMS `w2_dzip_unpack.bms` | Unpack `pack0.dzip` | Referenced by Szkaradek123 |

### Important
When importing into Blender, select the **entire unpacked game directory** (`pack0` root), **not** the texture folder — the importer needs to read all textures across the tree.

---

## 2. Models: `.w2ent` vs `.w2mesh`

The Witcher 2 stores models in **`.w2ent`** (entity) files and standalone **`.w2mesh`** files.

### Key Distinctions
- `.w2ent` files **contain the mesh data** — they are not just references
- Not all `.w2mesh` data lives inside `.w2ent` files; when a `.w2ent` contains `.w2mesh` data, the importer opens the package and lets you select them
- `.w2ent` files usually contain data other than `CMesh` objects too (camera config, entity settings)
- **Standalone vs embedded mesh data differ** — data structures are different, so a script that loads one does not trivially load the other

### Example
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

Model location example: `pack0.dzip\game\npc\riszon.w2ent` holds `richon__body1_b1.w2mesh` (Letho).

---

## 3. Textures: `.xbm` → DDS

`.xbm` texture files are essentially **DDS files without the DDS header** — usually **DXT1 or DXT5**.

### Conversion
- Throw a DDS header on them and you're done
- Community produced **witcher2_texture_converter** (QuickBMS + cmd scripts) for `.xbm` ⇄ `.tga`/`.dds` conversion

### QuickBMS Scripts (from witcher2_texture_converter)
| Script | Purpose |
|--------|---------|
| `w2_xbm2dds.bms` | `.xbm` → `.dds` |
| `w2_dds2xbm.bms` | `.dds` → `.xbm` |
| `w2_dzip_unpack.bms` | unpack `.dzip` |

Converter runs with `quickbms.exe` placed next to unpacked files, producing `filename_DXT(1,5).bin` (binary info) and `filename_DXT(1,5).tga` (texture).

---

## 4. Blender Importer (Szkaradek123)

**Szkaradek123's Blender 2.49 importer** was the primary tool for importing Witcher 2 models (geometry, weights, UVs, textures, bones).

### Requirements
- **Must use Blender 2.49** with **Python 2.6** (not 2.5x / Python 3.x) — scripts not compatible with Blender 2.5+
- Run from **Scripts window** (open `.blend`, right-click script window, Execute)
- Select **unpacked game dir** (whole `pack0`), load a `.w2ent` model
- Importer reads all needed textures and converts them to `.dds`; if it can't, script breaks
- Some models error out because file format was **not 100% known**

### Version History
| Date | Added |
|------|-------|
| 2011-06-05 | materials, bones (no joints) |
| 2011-06-23 | 'magic' button, better mesh names, texture import, static meshes, bone parenting |

### 'Magic' Button
Fixes `.w2ent`/`.w2mesh` files that fail to import correctly — mainly body meshes.

### Common Troubleshooting
- **Textures don't apply** → load manually from character folder (search `.dds`), or use UV/Image editor; "single user" material workaround helps when hair texture covers body and vice versa
- **No face** → faces stored in `\templates\characters\appearances\main npc`
- **Bones import but aren't connected** → parent manually (select armature → TAB → shift+RMB child+parent → ctrl+P → Keep Offset), then remove doubles and disable deform on roll joints

### Armature / Rigging Notes
- Bones all present but **not connected**; parenting is manual
- Most characters share **common bone structure** — one saved armature works for many models (Geralt, Iorweth)
- Roll joints (elbows, forearms, hands) need manual re-parenting to bend correctly
- Documented arm hierarchy: `Bicep2 → Bicep → ShoulderRoll → Forearm/Elbowroll → Forearmroll1/2 → Handroll → Hand`
- Exporting to 3ds Max: use **Blender 2.49 DAE (or FBX) exporter**, or **md5mesh exporter** for best Blender↔Max cross-import, then Max→FBX→UDK

---

## 5. Dialogue Audio: `.w2speech`

`.w2speech` files contain **localized dialogue**.

### Format
- Audio thought to be **MPEG layer 2 @ 48000 Hz** (decodable as such), with at least 21,568 individual samples
- Voice-over referenced as `VO_<1>_<2>_<3>`:
  - `<1>` = voice tag id (e.g., `TRIS` for Triss)
  - `<2>` = scene/set number (all VOs in a scene usually share it, e.g., `300202`)
  - `<3>` = line number (e.g., `0343`)
  - Example: `VO_TRIS_300202_0343` (from `q104_triss_interaction_oneliner.w2scene`)
- Header format **not fully documented**; individual samples hard to find

### REDkit Extraction
REDkit extracts all speech automatically after install:
```
"<SteamLibrary>\steamapps\common\the witcher 2\bin\editor.exe" exportstrings exportvo
```
Extracts currently-selected Steam language. Only outputs already-extracted samples — can't convert other-language `.w2speech` not in same format.

---

## 6. Video: `.usm` (Xbox 360)

Xbox 360 version uses **`.usm`** files for cutscenes.

### Demux Pipeline
```
.usm → VGMToolbox → M2V + ADX → ADX → WAV → mkvmerge → MKV
```

### Issues
- Some `.usm`-derived `.m2v` streams play in VLC but **not recognized by mkvmerge** (end-of-file / unknown type) — possibly slightly different MPEG encode
- VGMToolbox was the only widely known USM demuxer at the time

---

## 7. REDkit (Official Modding Tool)

**REDkit** (RedKit, CD Projekt RED) — official Witcher 2 editor, released May 2013.

### Capabilities
- **No import/export for skeletal meshes** as of 2013; only static meshes as `.3ds`, `.obj`, `.fbx` (no characters/NPCs)
- Ships **3DS Max 2009/2013 plugin** exporting only `.re` static mesh format
- **Extracts all speech (VO)** — run editor or use CLI command above

---

## 8. Music: `.fsb` (Unknown Codec)

Early Witcher 2 music `.fsb` files used an **unknown codec** that `fsbext` couldn't identify and `towav`/other FSB tools wouldn't decode.

- A coder later released a **Witcher 2 music FSB unpacker** that worked great

---

## 9. Tools Reference

| Tool | Purpose | Notes |
|------|---------|-------|
| Gibbed's RED Tool | Unpack `pack0.dzip` | WitcherNexus file id 52; supports W1/W3 later |
| chrrox QuickBMS scripts | Unpack `pack0.dzip` | `w2_dzip_unpack.bms` |
| witcher2_texture_converter | `.xbm` ⇄ `.tga`/`.dds` | `w2_xbm2dds.bms`, `w2_dds2xbm.bms` |
| Szkaradek123 Blender 2.49 importer | Import `.w2ent`/`.w2mesh` | Geometry, weights, UVs, textures, bones |
| VGMToolbox | Demux `.usm` (M2V + ADX) | Xbox 360 cutscenes |
| mkvmerge | Mux M2V+ADX → MKV | Some M2V streams unrecognized |
| REDkit | Official W2 editor | Static mesh export only; extracts VO |
| Blender 2.49 DAE/FBX exporter | Export to Max | Use md5mesh for best cross-import |

---

## Key Facts
- Archive: `.dzip` (pack0.dzip)
- Models: `.w2ent` (entity, contains mesh) + `.w2mesh` (standalone) — **different structures**
- Textures: `.xbm` = DDS without header (DXT1/DXT5)
- Dialogue: `.w2speech` = MPEG layer 2 @ 48kHz, VO naming: `VO_<tag>_<scene>_<line>`
- Video: `.usm` (X360) → M2V + ADX via VGMToolbox
- Music: `.fsb` (custom codec, later unpacked)
- Blender importer: **Blender 2.49 + Python 2.6 only**
- REDkit: Static mesh export only, VO extraction