# Cyberpunk 2077 (REDEngine 4 / REDengine v4) — Archives, Models, Textures & Audio

> **Source**: XeNTaX forum knowledge extraction (2020–2023).
> Covers `.archive` (RDAR+KARK), Oodle compression, `.mesh`/`.buffer`/`.rig`/`.morphtarget` model stack, `.xbm`/`.mi` textures, Wwise `.wem`/`.opuspak`/`.bnk` audio, and tools.
> Shares REDEngine lineage with The Witcher 3 (see Witcher 3 doc); CP2077 runs on **REDengine 4**.

---

## 1. Container Format: `.archive` (RDAR + KARK)

Game data packed in **`.archive`** files with **`RDAR`** magic (version 12).

### Header & Filesystem (IceReaper's 010 template)
```c
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

### Oodle Compression
- Compressed entries begin with **`KARK`** magic
- Decompressed via **Oodle** (requires game's `oo2core_<version>_win64.dll` or `oo2ext_7_win64.dll`)
- QuickBMS script (`cyberpunk_2077.bms`, aluigi) uses `comtype oodle`
- OodleSharp wrapper (github.com/Crauzer/OodleSharp) for C# integration
- rfuzzo's KAITAI spec (`.ksy`) available

### Filenames Are Hashed
- Filenames stored as hashes (like Watch Dogs, Hitman, RE Engine)
- Tools must hook filenames or ship hash→name dictionary
- eprilx: ~600k known, ~50k missing
- Ekey: 1.3M hashes mapped across archives (e.g., `basegame_3_nightcity.archive` 99%, `lang_en_text.archive` 100%)
- Merged into WolvenKit/CP77Tools

---

## 2. Extraction Tools

| Tool | Purpose | Notes |
|------|---------|-------|
| QuickBMS `cyberpunk_2077.bms` | Extract `.archives` → `.dat` | aluigi; Oodle decode via `comtype oodle` |
| **CP77Tools** (rfuzzo) | Extract with correct names/paths; decompress; rebuild | github.com/rfuzzo/CP77Tools; needs `oo2ext_7_win64.dll`; `uncook`/`unbundle`/`rebuild` |
| Wolven-kit (Traderain) | CDPR modding toolkit | github.com/Traderain/Wolven-kit; CP2077 support added |
| CR2WTools / cr2w.dll | CR2W file viewer | W3 `cr2w.dll` in Wolven-kit; classes outdated for CP2077 |

### CP77Tools: `uncook` vs `unbundle`
- **Unbundle** — extracts `.mesh` files, but mesh data stays hidden in Oodle-compressed blocks *within* `.mesh`. Noesis cannot preview without data.
- **Uncook** — decompresses data into separate **`.buffer`** files (Noesis needs these). Requires game's Oodle DLL.

```bash
uncook -p "D:\GOG\Cyberpunk2077\...\basegame_4_gamedata.archive" -w *mesh*
```
- Some meshes need `--forcebuffers` to get buffers
- CP77Tools v1.0+ **stopped producing buffer files**, breaking older Noesis scripts
- Use **CP77Tools 0.2.0.1** for buffer files with older `fmt_mesh` Noesis script

---

## 3. Models: `.mesh`, `.buffer`, `.rig`, `.morphtarget`

### File Stack
| Extension | Description |
|-----------|-------------|
| `.mesh` | CR2W mesh definition (external data refs + file links) |
| `.buffer` / `.mesh.N.buffer` | Oodle-decompressed vertex/index data blocks. Noesis auto-loads if same base name in same folder |
| `.rig` | Skeleton/bone hierarchy data |
| `.morphtarget` | Facial morph targets (character customization), under `basegame_4_gamedata\base\characters\head\player_base_heads\` |
| `.mi` | Material instance |
| `.mt`/`.ml*` | Material/layer files |
| `.dat` | Raw QuickBMS output — needs decryption/decompression (CP77Tools handles) |

### Noesis Plugin: `fmt_CP77mesh.py`
- Original: Joschka + akderebur
- Expanded: **alphaZ** (v1.4+)

#### Supported Import (v1.1–1.2.x)
Positions (scaled correctly), UVs, normals, bone indices/weights, bones + names, full skeleton via `.rig`

#### alphaZ Additions (v1.4x)
- **Textures:** preview `.xbm` and `.mi` (incl. multi-image); rename embedded to `.cp77tex`; beta export as `.buffer`
- **Model import:** upright rotation + user scale (default 100×), corrected bone maps via `.rig`, rotated normals, `.morphtarget` support, double-sided hair splitting, vertex colors, tangents, damage/garment meshes
- **Model export:** FBX → `.mesh`, new data to `.mesh.buffer`; `-bones` (bone pos only), `-meshbones` (bones + model)
- **Rebuild for in-game:**
```bash
rebuild -p "MODDED FOLDER PATH" -b -t --keep --unsaferaw
```

#### Key Options (top of `.py`)
`meshScale = 100`, `bHighestLODOnly`, `bLoadRigFile`/`bAutoDetectRig`, `bParentToRootIfNoParent`, `bReadTangents`, `bImportMorphtargets`, `bVertexColors`, `bFlipImage`

### Noesis Troubleshooting
| Symptom | Fix |
|---------|-----|
| "Wrong buffer file" / can't preview/export | **Uncook** (not just unbundle); keep `.buffer` next to `.mesh` |
| Rig rejected as invalid | Mesh uses **multiple `.rig` files**; enable `bLoadSeveralRigFiles` |
| Textures same name | Patch script to load `<meshname>_d01.dds` / `*_n01` |
| UVs dead/messed up | Open issue; weapons/vehicles/hair OK, clothes/characters often broken |
| Normal maps wrong on FBX | Engine flips green channel; invert/flip green channel |
| Bone weights off by one | Reported on some models (iguana) |
| Morphtarget FBX export | Blend-shape export broken/disabled in script; workarounds on Discord |

### Character Assembly
- Characters assembled from **many meshes scattered across archives** — no single mesh
- Dig through `appearance` / `cookedappearance` files to find meshes
- 3D map data: `base\entities\cameras\3dmap\3dmap_*` (gamedata archive)

---

## 4. Textures: `.xbm` / `.mi` / Atlas Masks

- Extracted via CP77Tools → `.png`/`.dds` named: `*_d01` (diffuse), `*_n01` (normal), `*_rm01` (rough/metal), `*_maskset` (channel masks: specular/AO/metallic packed RGB)
- Normal maps often **no blue channel** (only RG) — **expected, not a bug**
- **Texture atlases with tile-masks:** clothes/weapons use mask-packed atlases, not direct diffuse. Black 16×16 tiles removed to save space; must reconstruct from tiles buffer + CR2W definition JSON + atlas buffer (unsolved on forum)
- Repacking originally unsupported; CP77Tools added pack/rebuild but repacking without uncooking crashed game
- CDPR later released official modding toolkit for unpacking/packing textures

---

## 5. Audio: Wwise `.wem`, `.opuspak`, `.bnk`

CP2077 audio is **Wwise**. Banks in `.archive` files (`audio_2_soundbanks.archive`, etc.) → extracted as `.bin`/`.wem`.

### `.wem` Variants (v1.04)
| Format Tag | Type | Decode With |
|------------|------|-------------|
| `0x0001` | PCM | WEMConverter / vgmstream (rename `.bin`→`.wem`) |
| `0xFFFE` | WAVE_EXTENSIBLE PCM | WEMConverter |
| `0xFFFF` | Ogg Vorbis (non-standard) | ww2ogg |

### Opus Detection
- Some files have RIFF/wav header + PCM tag but actually **Opus** data
- Detect: hex `4F 67 67 53` (`"OggS"`) at offset `0x2C`
- Strip everything before to make valid `.opus`
- Python script shared to split soundbanks → named `.opus` files

### `.opuspak` — Opus Sound Archives
- In soundbanks; **OpusUnpack** (ninearts, .NET) extracts to playable `.wem` (foobar2000 + vgmstream)

### `.bnk` — Wwise Sound Banks (Car Sounds)
- **New Wwise variation** — Wwise Extractor, `bnkextr`/`ww2ogg`/`revorb`, vgmstream **fail** (only garbled engine noise via IMA codecs)

### Video
- `.bk2` (Bink) → Rad Video Tools

---

## 6. Game Data / Localization

- Text in `lang_*_text.archive` (e.g., `lang_en_text.archive` = 3,222/3,222 = 100% named)
- `basegame_4_gamedata.archive` = gameplay/gamedata (76,536/120,287 = 63% named at time)
- DRM-free on GOG (GOG preload used for early RE)
- CDPR confirmed official modding tools post-release

---

## 7. Tools Reference

| Tool | Purpose | Notes |
|------|---------|-------|
| QuickBMS `cyberpunk_2077.bms` | Extract `.archives` | aluigi; Oodle via `comtype oodle` |
| CP77Tools | Extract/decompress/rebuild | rfuzzo; needs `oo2ext_7_win64.dll`; uncook/unbundle/rebuild |
| Wolven-kit | CDPR modding toolkit | Traderain; merged hash→name list |
| fmt_CP77mesh.py (Noesis) | Import/export mesh/rig/morph | alphaZ; needs uncooked `.buffer` files |
| OodleSharp | Oodle decompress wrapper | github.com/Crauzer/OodleSharp |
| ww2ogg | Vorbis WEM → OGG | hcs64 |
| WEMConverter / vgmstream | PCM WEM decode/play | vgmstream also plays `.opuspak` output |
| OpusUnpack (ninearts) | `.opuspak` → `.wem` | .NET console tool |
| Rad Video Tools | Play `.bk2` video | Bink |
| ww2ogg/revorb/bnkextr/Wwise Extractor | `.bnk` decoding | **FAIL** on CP2077's new Wwise variation |

---

## Key Facts
- Archives: `.archive` (RDAR magic) + Oodle (KARK) + hashed filenames
- Models: `.mesh` (CR2W) + `.buffer` (vertex data, needs uncook) + `.rig` (skeleton) + `.morphtarget`
- Textures: `.xbm`/`.mi` (CR2W-wrapped DDS), atlases with tile-masks (unsolved), normal maps RG-only
- Audio: Wwise — PCM/WAVE_EXT/Vorbis/Opus `.wem`, `.opuspak`, `.bnk` (new variation, undecoded)
- Modding: CP77Tools (extract/rebuild), Wolven-kit (toolkit), fmt_CP77mesh (Noesis), official toolkit later
- Hash→name dict critical (1.3M+ mapped, merged into tools)