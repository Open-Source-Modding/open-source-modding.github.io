# Cyberpunk 2077 — XeNTaX Forum Knowledge

Extracted and organized from XeNTaX forum discussions (2020–2023). Covers the
**REDEngine 4** (REDengine v4) file formats of Cyberpunk 2077 — the `.archive`
container format, Oodle compression, the `.mesh`/`.buffer`/`.rig`/`.morphtarget`
model stack, `.xbm` textures, Wwise `.wem`/`.opuspak`/`.bnk` audio — plus the
tools (QuickBMS `cyberpunk_2077.bms`, CP77Tools, Wolven-kit, the Noesis
`fmt_CP77mesh` plugin). CP2077 shares **REDEngine** with The Witcher 3 (see the
Witcher 3 knowledge doc); it runs on **REDengine 4**. *(bertonberton, 2020-11-24,
t=23052)*

---

## 1. Container Format: `.archive` (RDAR + KARK)

The game's data is packed in **`.archive`** files, identified by the **`RDAR`**
magic. *(Haoose, 2020-12-07, t=23052)*

### Header & filesystem layout (IceReaper's 010 template)

```
struct Header {
    char RDAR[4];
    int version;               // 12
    int64 fileSystemOffset;
    int64 fileSystemSize;
    int64 unk2;                // 0
    int64 fileSize;
    int unk3[33];              // 0
};

struct FileSystem {
    int unk1;                  // 8
    int chunkSize;
    int64 unk2;
    int numDataChunkGroups;
    int numDataChunks;
    int numUnk3;
    DataChunkGroup dataChunkGroup[numDataChunkGroups];
    DataChunk dataChunk[numDataChunks];
    int64 unk3[numUnk3];
};
```

*(IceReaper, 2020-12-07, t=23052)*

### Oodle compression & the QuickBMS script

Compressed entries begin with the **`KARK`** magic and are **Oodle-compressed**.
eprilx's QuickBMS script (`cyberpunk_2077.bms`, official by aluigi) decodes
them with `comtype oodle` (Oodle). *(eprilx, 2020-12-08; bertonberton,
2020-11-24, t=23052)*

```
idstring "RDAR"
get Version long
get offset_table longlong
goto offset_table
getdstring unk 16
get numChunk1 long
get numChunk2 long
get unk long
savepos OffsetChunk1
...
    if zsize == size
        log name offset zsize          # stored raw
    else
        goto offset
        idstring "KARK"
        GET size long
        math zsize - 8
        savepos offset_compressed
        comtype oodle
        clog name offset_compressed zsize size
    endif
```

- Oodle decompression requires the game's **`oo2core_<version>_win64.dll`** (or
  `oo2ext_7_win64.dll`); call its decompress/compress functions, e.g. via
  **OodleSharp** (github.com/Crauzer/OodleSharp). *(eprilx, rfuzzo, 2020-12-08,
  t=23052)*
- rfuzzo published a **KAITAI (.ksy)** spec for `.archives`.
  *(rfuzzo, 2020-12-08, t=23052)*

### Filenames are hashed

Filenames inside `.archives` are stored as **hashes** (like Watch Dogs, Hitman,
RE engine), so tools must hook filenames or ship a hash→name dictionary.
*(eprilx, 2020-12-08, t=23052)*

- eprilx's filename hook: of ~600,000 known filenames, ~50,000 were still
  missing. *(eprilx, 2020-12-11, t=23052)*
- Ekey's multi-language project list mapped 1,307,466 hashes across archives
  (e.g. `basegame_3_nightcity.archive` → 122,248 of 122,502 = 99%;
  `lang_en_text.archive` → 3,222 of 3,222 = 100%). *(Ekey, 2020-12-19, t=23052)*
- The hash→name list was merged into **WolvenKit/CP77Tools** (issue #27).
  *(Ekey, 2020-12-19, t=23052)*

## 2. Extraction Tools

| Tool | Purpose | Notes |
|------|---------|-------|
| QuickBMS `cyberpunk_2077.bms` | Extract `.archives` → `.dat` files | Official aluigi script; Oodle decode |
| **CP77Tools** (rfuzzo) | Extract with correct filenames/paths; decompress; rebuild | github.com/rfuzzo/CP77Tools; needs `oo2ext_7_win64.dll`; `uncook`/`unbundle`/`rebuild` |
| Wolven-kit (Traderain) | CDPR modding toolkit | github.com/Traderain/Wolven-kit; CP2077 support added |
| CR2WTools / cr2w.dll | CR2W file viewer | W3 `cr2w.dll` in Wolven-kit; classes out-of-date for CP2077 |

### CP77Tools: `uncook` vs `unbundle`

There are two ways to extract with CP77Tools (alphaZ, 2021-02-12, t=23162):

- **Unbundle** — gives you the `.mesh` files themselves, but the actual mesh
  data stays hidden inside **Oodle-compressed blocks** *within* those `.mesh`
  files. Noesis cannot preview these without the data.
- **Uncook** — decompresses that data into separate **`.buffer`** files, which
  Noesis needs. Uncooking requires the game's Oodle DLL.

```
uncook -p "D:\GOG\Cyberpunk2077\...\basegame_4_gamedata.archive" -w *mesh*
```

- Some meshes may need **`--forcebuffers`** when uncooking to get buffers.
  *(nightwolf1982, 2021-09-03, t=23162)*
- Later CP77Tools versions (v1.0) **stopped producing buffer files**, breaking
  older Noesis scripts that required them. *(lndrx, 2021-01-16, t=23162)*
- Use **CP77Tools 0.2.0.1** (an early version) if you need the buffer files for
  the older `fmt_mesh` Noesis script. *(lndrx, Piranjak, 2021-01-16, t=23162)*

## 3. Models: `.mesh`, `.buffer`, `.rig`, `.morphtarget`

### File stack

- **`.mesh`** — the CR2W mesh definition (referenced external data + external
  file links).
- **`.buffer`** / **`.mesh.N.buffer`** — the Oodle-decompressed vertex/index
  data blocks. Noesis loads the matching `.buffer` automatically if it's in the
  same folder with the same base name as the `.mesh`.
  *(alphaZ, 2021-02-12, t=23162)*
- **`.rig`** — skeleton/bone hierarchy data.
  *(Joschka, 2020-12-17, t=23162)*
- **`.morphtarget`** — facial morph targets (character customization), stored
  under `basegame_4_gamedata\base\characters\head\player_base_heads\`.
  *(sydie, 2020-12-22, t=23162)*
- **`.mi`** — material instance; **`.mt`/`.ml*`** — material/layer files.
  *(Loomy, 2021-01-11, t=23162)*
- The **`.dat`** files from QuickBMS need decryption/decompression (see §1/§2);
  CP77Tools handles them. *(Sharppy, 2020-12-10, t=23052)*

### The Noesis plugin (fmt_CP77mesh)

Joschka (with akderebur) made the first Noesis script; **alphaZ** took over and
expanded it into **`fmt_CP77mesh.py`** (v1.4+). *(Joschka, 2020-12-13; alphaZ,
2021-02-05, t=23162)*

**Supported import (v1.1–1.2.x):** positions (vertices scaled correctly), UVs,
normals, bone indices, bone weights, bones and bone names, and (1.2) full
skeleton support via `.rig` files. *(Joschka, 2020-12-13..12-17, t=23162)*

**alphaZ's fmt_CP77mesh adds (v1.4x):**

- **Textures:** preview `.xbm` and `.mi` textures (incl. multi-image files);
  rename embedded textures to `.cp77tex` to preview; beta texture export
  encoding as `.buffer` files.
- **Model import:** upright rotation + user scale (default 100×), corrected
  bone maps via `.rig`, correctly rotated normals, `.morphtarget` support,
  double-sided hair mesh splitting, vertex colors, tangents, damage meshes,
  garment meshes.
- **Model export:** FBX → `.mesh`, writing new data to a new `.mesh.buffer`;
  modify bone positions only (`-bones`); modify bones + model data
  (`-meshbones`).
- **Get a mesh mod in-game:**
  ```
  rebuild -p "MODDED FOLDER PATH" -b -t --keep --unsaferaw
  ```
  *(alphaZ, 2021-02-05 & 2021-02-12, t=23162)*

**Key options** (top of the .py): `meshScale = 100`, `bHighestLODOnly`,
`bLoadRigFile` / `bAutoDetectRig`, `bParentToRootIfNoParent`,
`bReadTangents`, `bImportMorphtargets`, `bVertexColors`, `bFlipImage`, etc.
*(alphaZ, 2021-02-05, t=23162)*

### Troubleshooting the Noesis plugin

- **"Wrong buffer file" / can't preview / can't export** → you haven't
  **uncooked**; unbundle alone leaves data hidden in Oodle blocks. Uncook (see
  §2) and keep the `.buffer` next to the `.mesh`. *(alphaZ, 2021-02-12, t=23162)*
- **Rig file rejected as invalid** → some meshes use **several `.rig` files**
  (a mesh's bones can be split across rigs). Use `bLoadSeveralRigFiles` to feed
  multiple rigs. *(Joschka, 2020-12-17, t=23162)*
- **Textures share the same name** → some meshes' textures all come out with
  the same filename; users patched the script to load
  `<meshname>_d01.dds` (and `*_n01` for normals) by convention. *(Loomy,
  2021-01-04, t=23162)*
- **UVs dead/messed up** → an open issue; weapons/vehicles/hair mods worked but
  clothes/characters often had issues. *(CharlieV, 2020-12-25; alphaZ,
  2021-02-05, t=23162)*
- **Normal maps look wrong / lost on FBX round-trip** → the engine interprets
  normal maps with a flipped green channel; try inverting/flipping the green
  channel. *(devilsnake88, 2021-03-07, t=23162)*
- **Bone weights off by one** → a reported issue on some models (iguana).
  *(pox911, 2020-12-30, t=23162)*
- **Morphtarget FBX export** → blend-shape export was **broken/disabled** in the
  script (annotated); workarounds were documented on the Cyberpunk modding
  Discord. *(uncalquera, dimis9138, 2021-07-04, t=23162)*

### Assembling characters / finding meshes

- Characters are **assembled from many meshes scattered across archives**; they
  don't always have one individual mesh. You must dig through the
  **appearance / cookedappearance** files to find which meshes and where.
  *(nightwolf1982, 2021-09-03, t=23162)*
- The **3D map** data lives at `base\entities\cameras\3dmap\3dmap_*` (gamedata
  archive). *(Loomy, 2021-01-11, t=23162)*

## 4. Textures: `.xbm` / `.mi` / atlas masks

- Textures are `.xbm` files (CR2W-wrapped DDS). Extracted via CP77Tools you get
  `.png`/`.dds` named e.g. `*_d01` (diffuse), `*_n01` (normal), `*_rm01` (rough/
  metal), `*_maskset` (channel masks: specular/AO/metallic packed per RGB).
  *(JKerman, 2021-02-05, t=23162)*
- Normal maps often **have no blue channel** (only RG) — this is expected, not
  a bug. *(uncalquera, 2021-07-03..04, t=23162)*
- **Texture atlases with tile-masks:** clothes/weapons use **mask-packed texture
  atlases**, not direct diffuse textures. Black 16×16 tiles are removed to save
  space and must be **reconstructed** from a tiles buffer using the CR2W
  definition JSON (dumped via CP77Tools) + atlas buffer + tiles buffer. This was
  unsolved on the forum. *(Hitmanhimself, 2020-12-23, t=23211)*
- Texture repacking into the game was originally unsupported; CP77Tools added a
  **pack/rebuild** feature, but repacking without uncooking crashed the game
  before the start screen. *(Felldude, 2021-01-26, t=23052)*
- CDPR later released the **official modding toolkit** for unpacking/packing
  textures. *(DENver666, 2021-01-27, t=23052)*

## 5. Audio: Wwise `.wem`, `.opuspak`, `.bnk`

CP2077 audio is **Wwise**. *(LinkOFF, 2020-12-08, t=23052)*

- Audio banks live in `.archive` files (`audio_2_soundbanks.archive`, etc.);
  extracted as `.bin`/`.wem` files.
- **`.wem`** — Wwise WEM (RIFF). Three variants found in
  `audio_2_soundbanks.archive` (v1.04, k1tteh, 2020-12-17, t=23052):
  - `wFormatTag 0x0001` = normal **PCM** → convert with **WEMConverter** /
    vgmstream (rename `.bin`→`.wem`).
  - `0xFFFE` = **WAVE_EXTENSIBLE** PCM → WEMConverter.
  - `0xFFFF` = **Ogg Vorbis** (non-standard) → convert with **ww2ogg**.
- Some files have a RIFF/wav header + PCM format tag but actually contain
  **Opus** data — detect by hex `4F 67 67 53` (`"OggS"`) at offset `0x2C` and
  strip everything before it to make a valid `.opus`. *(k1tteh, 2020-12-23,
  t=23052)*
- A Python script was shared to split soundbanks into named `.opus` files.
  *(k1tteh, 2020-12-29, t=23052)*
- **`.opuspak`** — Opus sound archives in soundbanks; **ninearts' OpusUnpack**
  (.NET tool) extracts them to playable `.wem` (playable via foobar2000 +
  vgmstream plugin). *(ninearts, 2021-04-23, t=23782)*
- **`.bnk`** — Wwise sound bank containers for car sounds; these use a **new
  Wwise variation** that Wwise Extractor, `bnkextr`/`ww2ogg`/`revorb`, and
  vgmstream could **not** decode (only garbled engine noise via IMA codecs).
  *(CiccioCiolla, DKDave, 2021-08-10..12, t=24342)*
- Video `.bk2` (Bink) plays with **Rad Video Tools**. *(LinkOFF, 2020-12-08,
  t=23052)*

## 6. Game Data / Localization

- Game text is in **`lang_*_text.archive`** (e.g. `lang_en_text.archive` →
  3,222 of 3,222 = 100% named). *(Ekey, 2020-12-19, t=23052)*
- `basegame_4_gamedata.archive` holds gameplay/gamedata files (76536 of 120287
  named = 63% at the time). *(Ekey, 2020-12-19, t=23052)*
- The game uses **DRM-free** distribution on GOG (GOG preload was used for
  early RE). *(bertonberton, Haoose, 2020-11-24..12-08, t=23052)*
- CDPR confirmed **official modding tools** would come after release.
  *(Ekey, 2020-12-02, t=23052)*

## 7. Tools Reference

| Tool | Purpose | Notes |
|------|---------|-------|
| QuickBMS `cyberpunk_2077.bms` | Extract `.archives` | aluigi; Oodle decode via `comtype oodle` |
| CP77Tools | Extract/decompress/rebuild `.archives` | rfuzzo; needs `oo2ext_7_win64.dll`; uncook/unbundle/rebuild |
| Wolven-kit | CDPR modding toolkit | Traderain; merged hash→name list |
| fmt_CP77mesh.py (Noesis) | Import/export `.mesh`/`.rig`/`.morphtarget` | alphaZ; needs uncooked `.buffer` files |
| OodleSharp | Oodle decompress wrapper | github.com/Crauzer/OodleSharp |
| ww2ogg | Convert Vorbis WEM → OGG | hcs64 |
| WEMConverter / vgmstream | PCM WEM decode/play | vgmstream also plays `.opuspak` output |
| OpusUnpack (ninearts) | Extract `.opuspak` → `.wem` | .NET console tool |
| Rad Video Tools | Play `.bk2` video | Bink |
| ww2ogg / revorb / bnkextr / Wwise Extractor | `.bnk` decoding | **fail** on CP2077's new Wwise variation |

---

*Source: XeNTaX forum threads t=23052 (Cyberpunk 2077 extraction), t=23162
(Noesis mesh script), t=23201 (Wwise audio), t=23211 (texture atlases), t=23782
(.opuspak), t=23815 (assembling NPCs), t=24342 (.bnk car sounds).*
