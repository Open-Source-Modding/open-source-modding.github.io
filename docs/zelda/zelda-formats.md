# Zelda (Wii U / 3DS / Switch) — Archives, Textures, Audio & Models

> **Source**: XeNTaX forum knowledge extraction (2015–2023).
> Covers Breath of the Wild, Tears of the Kingdom, Twilight Princess HD, Skyward Sword, Ocarina of Time 3D, Zelda 3DS general, Sonic Lost World Zelda Zone DLC, Four Swords Anniversary.

---

## 1. Breath of the Wild (Wii U / Switch) — Yaz0 / SARC / sblarc

### Archive Pipeline
```
Game data → .pack (SARC) → .sblarc (Yaz0 compressed SARC) → content files
Path: content/Pack/Bootup.pack/Layout/Common.sblarc
```

### Yaz0 Compression
- **Magic**: `Yaz0` (4 bytes)
- **Endianness**: Big-endian (Nintendo standard)
- **Original spec**: http://www.amnoid.de/gc/yaz0.txt (2005, GameCube era)
- BotW uses Yaz0 wrapping SARC archives (`.sblarc` = Yaz0 + SARC)

### Tools
| Tool | Purpose | Notes |
|------|---------|-------|
| **Syroot.NintenTools.Yaz0** | C# decode/encode | github.com/Syroot/NintenTools.Yaz0 — NuGet |
| **Yaz0.exe** (XeNTaX) | CLI decode/encode | Fake compression (level 0) on encode → larger but valid |
| **Uwizard / yaz0enc** | Community tools | Repacking issues (game crashes even with untouched content) |
| **Switch-Toolbox** | Multi-format | github.com/KillzXGaming/Switch-Toolbox — Yaz0, SARC, BWAV, GTX |

### SARC Archives
- **Magic**: `SARC`
- **Tool**: JayK's unpacker (2015) — works for BotW, Mario 3D World, Captain Toad
- **Repacking**: No confirmed working repacker in thread

### Key Issues
- **Repacking crashes**: Game requires maximum compression; fake compression (level 0) produces valid Yaz0 but game rejects it
- **Compression ratio**: Original 3.77 MB → re-encoded larger; no tool matched original ratio
- **Structure**: `content/Pack/*.pack` (SARC) → contains `Layout/*.sblarc` (Yaz0)

---

## 2. Tears of the Kingdom (Switch) — Audio

### BWAV Format
- **Extension**: `.bwav`
- **Container**: `.bars.zs` (zstd compressed) → extract with zstd → `.bars` → contains `.bwav`
- **Playback**: ~90% playable in Foobar2000 + vgmstream; ~10% fail (different encoding?)

### Tools
| Resource | Link |
|----------|------|
| GameBanana post | gamebanana.com/posts/10308634 |
| Switch-Toolbox | github.com/KillzXGaming/Switch-Toolbox |
| vgmstream | Foobar2000 component for BWAV |

---

## 3. Twilight Princess HD (Wii U) — GTX Textures

### GTX Format
- **Extension**: `.gtx`
- **Difference from standard Wii U GTX**: Stores **multiple textures per file** (newer format)
- **Standard tools** (texconv2, Noesis) only extract first texture
- **Community work**: RandomTBush developing GTX splitter (2016)

### Archive
- **Container**: `.pack.gz` (gzipped SARC)
- **Extraction**: QuickBMS script → `geo/` and `tex/` folders

---

## 4. Skyward Sword (Wii) — LZ / ARC / Enemy Data

### Formats
- **Compression**: LZ
- **Archive**: `.arc`
- **Enemy files**: `.endat`, `.entxt` (spawn points/events?), `.zev.dat` (enemy spawn data?)
- **Status**: Partially documented; spawn/event formats not fully understood

---

## 5. Ocarina of Time 3D (3DS) — CSAB / CMB

### Formats
- **`.csab`**: Animation data (not readable by N3dsCmb viewer)
- **`.cmb`**: Model container
- **`.zar`**: Archive containing `.cmb` + `.csab`

### Tools
| Tool | Notes |
|------|-------|
| **N3dsCmb Viewer** | Loads `.cmb`, fails on `.csab` animations |
| **hex2obj** | Manual mesh extraction (shakotay2) |
| **OoT3D-Importer** | github.com/MeltyPlayer/OoT3D-Importer — Blender importer |
| **GAR - ZAR Unpack** | Extracts models/animations from ZAR |

### CSAB Animation Structure (shakotay2)
- Bone weights in `VatrChunk.BoneWeights`
- Animation data at `sepd.BoneWeightArrayOffset`
- Frame data: 16 bytes/frame (offset diff = count × 16)
- Skinning weights possibly in `.dae` export (unconfirmed)

---

## 6. Zelda 3DS Models (General) — CMB / CSAB

### Formats
- **`.cmb`**: Model format (Nintendo 3DS)
- **`.csab`**: Skeletal animation
- **`.zar`**: Archive (GAR tool extracts)

### Working Pipeline
1. **GAR - ZAR Unpack** → extract `.cmb` + `.csab`
2. **OoT3D-Importer** (MeltyPlayer) → import to Blender
3. **Save as .blend** → open in Blender 2.83+ LTS (fixes GFX issues)

---

## 7. Zelda Zone DLC (Sonic Lost World Wii U) — Hedgehog Engine

### Archive Pipeline
```
.cpk (CriWare) → quickbms cpk script → .pac files
.pac → pacpack-WiiU (libgens-sonicglvl WiiU branch) → game files
```

### File Types
| Extension | Content |
|-----------|---------|
| `.model` | Mesh (convertible to FBX via modelfbx_2012) |
| `.shadow-model` | Shadow mesh |
| `.skl.hkx` | Havok skeleton (HKX 2012) |
| `.anm.hkx` | Havok animation |
| `.dds` | Textures (Gfx2 header = Wii U GX2) |
| `.material` | Material definitions |

### Tools
| Tool | Purpose |
|------|---------|
| **modelfbx_2012** | `.model` → FBX (no bones) |
| **HKXConverter-2012-With-Metadata** | `.skl.hkx` + `.anm.hkx` → FBX with bones |
| **Switch-Toolbox / texconv2** | `.dds` (Gfx2) conversion |
| **SonicGLvl** | Level editor/viewer |

### Special Cases
- `zdlc03_obj_fairy.model` → converts to 5 planes (texture-driven geometry)
- 4th material slot expects "water" string; other values trigger error

---

## 8. Four Swords Anniversary (DSi) — Binary Formats

### Formats
- **`.bin`**, **`.blz`** (compressed textures)
- Community requested help for translation (2021)
- No documented tools in thread

---

## 9. Cross-Game Nintendo Formats Summary

| Format | Games | Compression | Tools |
|--------|-------|-------------|-------|
| **Yaz0** | BotW, TPHD, Mario 3D World, many Wii U | LZSS variant | Syroot.NintenTools, Yaz0.exe, Switch-Toolbox |
| **SARC** | BotW, Mario 3D World, Captain Toad, many | None (container) | JayK unpacker, Switch-Toolbox |
| **GTX** | TPHD, BotW, most Wii U | Block compression | texconv2 (partial), Switch-Toolbox, custom splitter needed |
| **BWAV** | TotK, BotW (Switch) | ADPCM/PCM in BAR | vgmstream, Switch-Toolbox |
| **BAR/ZS** | TotK | zstd (`.zs`) | zstd + Switch-Toolbox |
| **CMB/CSAB** | OoT3D, MM3D, Zelda 3DS | Custom | OoT3D-Importer, GAR, hex2obj |
| **HKX (2012)** | Sonic Lost World (Wii U) | Havok packfile | HKXConverter-2012, havok2fbx |
| **LZ/ARC** | Skyward Sword (Wii) | LZ | Custom tools |

---

## 10. Key Repositories

| Repo | Description |
|------|-------------|
| Syroot/NintenTools.Yaz0 | C# Yaz0 library |
| KillzXGaming/Switch-Toolbox | All-in-one Switch/Wii U tool |
| MeltyPlayer/OoT3D-Importer | Blender importer for 3DS Zelda |
| slashiee/libgens-sonicglvl (WiiU branch) | Sonic Wii U PAC/PACK tools |
| RandomTBush (GTX splitter) | WIP GTX multi-texture splitter |

---

## 11. Gaps / Unresolved
- **Yaz0 maximum compression**: No public tool matches Nintendo's ratio
- **SARC repacking**: No confirmed working repacker for Wii U
- **GTX multi-texture splitter**: Never released (RandomTBush 2016)
- **CSAB animation playback**: Not implemented in N3dsCmb viewer
- **Skyward Sword `.endat`/`.entxt`/`.zev.dat`**: Spawn/event format undocumented
- **Four Swords DSi `.blz`**: No decompression tool documented