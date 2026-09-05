# Watch Dogs — Community Tools & References

A curated list of third-party tools, forum knowledge and modding workflows that
inform the Disrupt-engine reference work. Some are outdated or non-authoritative
— they are kept here for the *format confirmations* and *gameplay semantics*
they provide, clearly marked where applicable.

## HavokLib / HavokMax (PredatorCZ)

- **HavokLib** — independent C++ library (GPL-3.0) for Havok **packfile**
  serialize + XML, v5.0.0–2017, both endians, x86/x64, all compilers.
  Handles `hkaSkeleton`, `hkaAnimation*`, `hkRootLevelContainer`,
  `hkxEnvironment`; binary export for v5–2014.
- **HavokMax** — 3ds Max importer/exporter plugin built on HavokLib
  (archived 2022; development moved to HavokLib's `toolset/`).
- **Relevance:** the authoritative cross-reference for the **WD1 native Havok
  packfile** (`import_hkx_wd.py`). It does **not** support tagfiles ("never
  will"), so it does *not* apply to WD2/WDL Disrupt `.hkx` (tagfile wrapper) —
  [HavokDisruptWD2](https://github.com/FranciscoManzanilla/HavokDisruptWD2)'s port
  remains authoritative there.

## OpenDogs tools (Nexus, WD2)

Watch Dogs 2 UWP tools that bundle an embedded Blender 2.79 + Volfin's
`import_WD2.py`:

- **OpenDogs FileSystem** — 119 **Gibbed.Disrupt filelists** for WD2 (~604k
  paths, 60% coverage) and WDL. Format: `;` progress header, a GUID line, then
  one path per line (`\`-separated). Useful as a hash→path database companion
  to the FNV-1a 64-bit `hash_wd2.py` utility (practical value mostly on the
  Fat/hash side, since `.xbg` materials store paths as plaintext Wstr).
- **OpenDogs NewUI / 3D View** — WinUI 3 viewers. Bundle `xbgtools/` with
  Volfin's `import_WD2.py` (already ported into the addon) and
  `convert_xbg_to_obj.py` (a thin Blender CLI wrapper).
- **Volfin importer confirmations** (from the bundled `import_WD2.py`):
  vertex strides 40/36/32/28/24/20; 16-bit half-float UVs; MOEG 0x89/0x46
  version gate; `.skel` (nbCF v3) reading.

## FranciscoManzanilla WD2 tooling (GitHub, C# — experimental, source-only)

Small, AI-assisted source dumps (Spanish docs, 1 star, no releases) by
[FranciscoManzanilla](https://github.com/FranciscoManzanilla). Not production
tools, but useful as *implementation cross-references* for format details:

- **[HavokDisruptWD2](https://github.com/FranciscoManzanilla/HavokDisruptWD2)** —
  "hkx resolver for wd2". `HkxParserD.cs` parses WD2
  Disrupt `.hkx`; docs cover injection modes and an autofit-all option. The
  port referenced in the HavokLib section above.
- **[xbgModelModInjector](https://github.com/FranciscoManzanilla/xbgModelModInjector)** —
  WD2 XBG model injector. `XbgParserD.cs`,
  `XbgMeshInjector.cs`, `XbgVertexPatcher.cs`, `XbgBoundsAnalyzer.cs` —
  mesh inject + vertex patching + bounds analysis for `.xbg`.
- **[WD2MSCore](https://github.com/FranciscoManzanilla/WD2MSCore)** —
  "Watch Dogs 2 Model Studio BETA".
- **[OpenII](https://github.com/FranciscoManzanilla/OpenII)** —
  "Watch Dogs 2 — Open Explorer UI".
- **[WLUManager](https://github.com/FranciscoManzanilla/WLUManager)** —
  "WLU Explorer Watch Dogs 2".

## XeNTaX Forum Knowledge (community — **format confirmations**)

**Full knowledge base**: [XeNTaX Watch Dogs knowledge](../../unsorted/xentax-watchdogs-knowledge.md) — extracted from the [XeNTaX forum dump](https://forum.xentax.com) (2014–2023), 14 threads.

The XeNTaX doc contains detailed C structs, compression variants, and tool references not repeated here. Key confirmations cross-referenced below:

> **New from XeNTaX (not in other docs)**: PS3 Deflate header struct, WD2 FAT first-8-bytes analysis, LZ4 `dec32_table` modification (`{0,3,2,3,0,0,0,0}`), LZMA constant `0xE6C`, WD2 character heads (5-6MB, cutscene-specific), ZOLA textures (GPU compute data), WD2 material field hashes & clothing categories, custom clothing tutorial, WD2 archive resolved/unknown counts, `.feu` Flash header fix, vehicle modding (Car on Demand), WD2 launch params, SBAO 55-byte trailing header.

- **FAT/DAT archive format** — WD1: magic `3TAF` (0x33544146), version 8; WD2: magic `5TAF` (0x35544146), version 11. Entry: hash/size/zsize/offset. Compression: PC=XMem, PS3=Deflate, Xbox=XMem. (Ekey, cra0, 2014) → *XeNTaX §1–2*

- **FAT/DAT archive format** — WD1: magic `3TAF` (0x33544146), version 8; WD2: magic `5TAF` (0x35544146), version 11. Entry: hash/size/zsize/offset. Compression: PC=XMem, PS3=Deflate, Xbox=XMem. (Ekey, cra0, 2014) → *XeNTaX §1–2*
- **XCompress headers** — Standard and modified variants; all big-endian on PC. (cra0, killerpepo, 2014) → *XeNTaX §1.2*
- **Patch archive behavior** — Game loads `patch.fat`/`patch.dat` if present, overriding content. Don't repack game archives; use patch bundles. (Rick, 2014) → *XeNTaX §1.1*
- **XBT texture format** — DDS with custom header; TBX magic, version byte 0x7B/0x8F family, 0x2C header for DXT5. (xentax community) → *XeNTaX §4*
- **XBG model format** — WD1 vertex strides 36/40/44; UV scaling at offsets 36/48; 6 bone weights for characters. WD2 version bump breaks WD1 importers. (Wobble, volfin, Oleg, 2014–2017) → *XeNTaX §3*
- **Localization (.loc)** — Magic `53 4C 01 00` ("SL"); fragment-based string storage. (cenkzenk, rengareng, 2014–2019) → *XeNTaX §5*
- **Audio (SBAO)** — 55-byte trailing header; pre-compiled audio; Sox v14.4 + sndfile for conversion. (Paul44, filu23, 2014) → *XeNTaX §6*
- **WD2 hash algorithm** — FNV1a x64 with `0xA000000000000000` tag, 61-bit fold. (Ekey, 2016) → *XeNTaX §1.3*; see also `reference/disrupt/watch_dogs/hashing.md`
- **Material field hashes** — Clothing/category hashes for modding. (disastorm, 2016) → *XeNTaX §8*
- **Modding workflow** — WD1: Gibbed.Disrupt tools; WD2: WD2Extract/WD2Pack + patch.fat/dat. (Rick, Sir Kane, disastorm, 2014–2016) → *XeNTaX §10–11*
- **Archive filelists** — Resolved/unknown counts per FAT. (Ekey, 2016) → *XeNTaX §12*
- **Vehicle/traffic modding** — Car on Demand, trafficpatternselector.lib. (neburas, Paul44, 2016–2017) → *XeNTaX §14–15*

## AI Road Navigation

The AI uses `roadnetwork_lowres.rnf` for pathfinding. Despite the name, it supports embedding hi-res road data — the "lowres" label is misleading. Visual roads and physics are spline lofts separate from the navigation mesh.

## World Loading Units (WLU)

`.wlu` files are World Loading Units — layer/area configuration setups. Missions load layers with scripts via WLU files. Can be modified in a traditional IDE-type environment for mission/layer editing.

## ZModeler 3 forum posts (Oleg — **outdated, not authoritative**)

These predate full file-format RE and treat parts of the format as immutable,
but document **gameplay/modding semantics** that still hold:

### Materials (`*.material.bin` + `engine\shaders\materialdescriptors`)

- Material name matters: e.g. a material named `VehicleName_CarPaint_01` gets
  the game's *variative paint color scheme* automatically (settings ignored).
- **`Damage` user property** — comma-separated case-sensitive object names,
  where the *position* is the damage-part index (`,,,TireFront_L` → L=#4,
  R=#5). Indices are **coded in the collision `.hkx`** — retain original
  indices or in-game damage (windshield cracks, tire deflate) will map to the
  wrong parts.
- Shader-parameter overrides can force any materialdescriptor parameter by name.

### Scene setup, hierarchy, LODs

- Export roots on a dummy node named like the output file; name suffixes
  toggle export scenarios: **`.rigid`** (rigid skinning — vehicles, whole
  volume split into parts), **`.skel`+`.mesh`** (blend skinning — characters,
  up to 8 bones/vertex in the tool, 6 in WD1), no suffix = regular objects
  (weapons/scenery).
- Blend skinning models carry "unknown fragments" reused from the original —
  interactive physics (movement/wind) is lost without them.
- **LODs**: UH/VH/H/M/L/VL scene states; `LODs` property on the root dummy is
  comma-separated switch distances (`10, 20, 30, 45, 90, 250`).
- Hierarchy dummies in square brackets (`[DoorFront_R_Hinge]`) are export-time
  only — the **`.skeleton` file** is required to retain them (vehicle hinge
  pivots for animations).

### Mod workflow (packing)

- `Gibbed.Disrupt.Pack.exe patch.fat <folder>` → single `patch.fat`/`patch.dat`
  pair dropped in the game root (WD1 honors one patch bundle).
- XBT = repacked DDS; header refinement is hex-edit work (a garbled but
  consistent sample: TBX magic, version byte 0x7B/0x8F family, 0x2C header
  size for DXT5).

## Colorgrading Quirk (Parallellines)

WD1 colorgrading is controlled via `windy_city.game.xml` — each weather state references a `.colorgrading` file. **If any value in `colorgrading.lib` is at default (Red=0, Green=0, Blue=0, Saturation=1, Contrast=0), the entire colorgrading is ignored.** Must set dummy values (`±0.001`) for unused channels. Don't make dummy values too small or weather transitions freak out. HDR LUTs are baked into the tone mapping — can't just edit screenshots in Photoshop.

## Beta Material Format (v5 Big-Endian)

Two pre-release Aiden material sets exist:

- `July28AidenMats/` — **v5 big-endian** (magic bytes `00 4D 41 54`)
- `sept2013aidenmats/` — **v7 little-endian** (magic bytes `54 41 4D 00`, retail format)

Same 9 artist materials in each (`lsauvage`, `mkamelaquino`, `slacoste`, `ycloutier` — coat/mask/sweatshirt_E3, head_V2, eye_V2, cornea, lashes, arm_cau, teeth). Same binary structure — **only endianness + version differ** (v5=BE, v7=LE). `material_bin.py` now auto-detects endianness from the magic bytes.

Cross-build param diffs (July28 → Sept13): coat/mask/sweatshirt +1 param, eye_V2 −3, arm_cau −2, lashes renamed `char01_lashes` → `char01_eyelashes`, head/cornea/teeth unchanged. GUID header word[2-4] also differs per build.

`ConvertMaterials.exe` **fails on v5** — asserts `magic==5062996 && version==7` (`materialFile.cpp:21-22`). Use `material_bin.py` for v5 files.

## Sound Filelist (wasd, Discord 2018)

`soundidlinelinks.slid` (binary, 13,581 SoundID→LinkID pairs) has human-readable companions `snd.txt` and `Eng_snd_Fileslist_Loc.csv` — identical 13,579-entry lists of `[StringID] [SoundID] [dialogue text]` (CSV header: `String ID,SPK ID,Dialogue`). Verified: slid Col A (SoundID) == the text list's sound-ID column (13,579/13,581 match; the 2 misses are events with no text). So slid Col A resolves to a voice line, slid Col B = the `.spk` audio-pack hash. The community "SPK ID" label is a misnomer for Col A — the real `.spk` hash is Col B. Contains unused/cut content (e.g. a psychologist-panel interview slated to play before Yolanda's). Sound data lives in `soundbinary/` inside patch archives; note `sound/` vs `sound_english/` folder distinction.

## PS3 Emulation

WD1 PS3 version can be emulated via RPCS3. 1.0 version has unique features removed in later builds (car sounds/music in NPC cars). (Source: SlyCooperReloadCoded, Takudaky)

## Gibbed Tools Definitive Definitions

Updated definitions for Gibbed.Disrupt tools covering all known archive/library formats.

### WD1

- **99.9% of used libs** covered for PC retail
- **X360 versions supported**: Final (common only), July 29, Aug 12, Sep 21, Jan 15 (2013 beta builds)
- Unused definitions have been deleted
- Knots and first 4 lines of each file have their own class, shared across all format classes
- **Filelist additions**: 2013 July 29 X360 build, Living City 2.8, Police and Factions Overhaul, Natural and Realistic Lighting, Classic UI, TFOWC2, 2013 Modpack

### WD2

- All libraries from `common/`, `patch/`, `patch2/` covered
- Some definitions remain unknown

### Warning

High probability of incapability for `.xml` loose files created with old versions of Gibbed Tools. If you encounter parse errors, re-export with the latest tool versions.
