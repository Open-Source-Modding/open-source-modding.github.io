# Starfield Modding & Reverse Engineering Reference

> **Cross-reference**: BA2 archives (BTDX/GNRL/DX10), NIF changes (BSTriShape, FP16), texture formats (BC5 for roughness/metalness), tools (BAE, F4 Tools, Noesis) → [XeNTaX Bethesda knowledge §2–4](../unsorted/xentax-bethesda-knowledge.md)

Starfield (Creation Engine 2, 2023) continues Bethesda's long-running Havok-based
animation lineage — the `.hkx` behavior/animation ecosystem documented for Fallout 4 in
[`fallout4-havok-guide.md`](fallout4-havok-guide.md). CE2 drives animation through the
**Animation Graph (AnimGraph)** framework, the same wrapper around Havok behavior that
Skyrim SE / Fallout 4 use. This page collects verified facts about the engine, the
file formats, the script extender, the official Creation Kit, and the community
tooling, and explicitly marks what is not yet documented rather than guessing.

## Engine Context: Creation Engine 2

- **Game**: Starfield, released 2023-09-05 (Steam AppID 1716740). Expansion
  *Shattered Space* released 2024-09-30 (AppID 2721670).
- **Engine**: Creation Engine 2 (CE2) — the successor to the Creation Engine used by
  Fallout 4 and Skyrim. Build environment is x64 (64-bit throughout).

### Animation system — Havok `.hkx` + AnimGraph (confirmed lineage)

The single most important finding: **Starfield retains the Havok behavior/animation
`.hkx` lineage**, exposed through the **Animation Graph (AnimGraph)** framework.
Primary-source evidence from CommonLibSF (`include/RE/`):

- `BSAnimationGraph` — base class (derives `BSIntrusiveRefCounted` plus several
  `BSTEventSource`s); subclass `AnimationManager : public BSAnimationGraph`.
- `BSAnimationGraphManager` (`sizeof == 0x80`) — the graph manager object.
- `BGSAnimationGraphComponent` (`sizeof == 0x40`) — the Form component that carries
  the graph project/behavior names as `BSFixedString` members plus a smart pointer.
- `IAnimationGraphManagerHolder` — the interface with `NotifyAnimationGraphImpl`,
  `GetAnimationGraphManagerImpl`, `GetGraphVariableImpl{Float,Int,Bool}`, and
  `Pre/PostUpdateAnimationGraphManager`. This is **identical in role** to the Skyrim SE
  AnimGraph system.
- **No `hkb*` or `hkx*` Havok types are exposed** in CommonLibSF — Havok behavior is
  handled opaquely behind the AnimGraph interface (same as Skyrim SE).

Behavior events flow through `NotifyAnimationGraph(const BSFixedString&)` and graph
variables (float/int/bool) via the `GetGraphVariable*` accessors. The `.hkx` files
themselves still carry behavior graphs (`hkbBehaviorGraph`), skeletons
(`hkaSkeleton`), and animation bindings (`hkaAnimationBinding`), organized into
**project / character / behavior file sets** — but **not** in the FO4 packfile
layout.

> **MAJOR FINDING (2026-08-18):** Starfield ships Havok **2019.02** — the
> `sdk_version` string is `"20190200"` — and uses the **new chunked packfile
> format** (FourCC chunks `DATA`/`SDKV`/`TYPE`/`INDX`, `ChunkType` records, `PTCH`
> patch sections, indexed blocks), **not** the pre-2015 FO4-style packfile
> (magic `0x57e0e057 0x10c0c010`, version 11). This answers the long-open
> question of which Havok SDK CE2 uses (see
> [`hkx_format.md`](../havok/hkx_format.md) — its "new format" scope note is
> exactly this container). The packfile-level differences mean FO4-era tooling
> (hkxcmd, hkxpack) cannot read Starfield `.hkx` without a chunked-format port.
> This is HIGH confidence — verified directly from the file headers.

## File Formats: BA2 Archives & Assets

Authoritative from `TES5Edit/TES5Edit` `Core/wbBSArchive.pas` (dev-4.1.6) — the same
BSArch engine that packs and unpacks the game archives. High confidence.

### BA2 / BSA header versions (4-byte magic)

| Magic | Meaning |
|-------|---------|
| `BSA\0` | Classic `.bsa` (TES4 / FO3 / SSE) |
| `BTDX` | `.ba2` (Fallout 4 / Starfield) |
| `GNRL` | BA2 subtype — general archive |
| `DX10` | BA2 subtype — texture/DDS archive |

Version values following the magic:

- `0x67` TES4 (Oblivion), `0x68` FO3/FNV/TES5, `0x69` SSE
- `0x01` FO4, `0x07`/`0x08` FO4 NG/AE
- **`0x02` Starfield v2, `0x03` Starfield v3** (SF)

### Starfield BA2 header layout (after `BTDX` magic)

1. `Version` — `0x02` (SF v2) or `0x03` (SF v3)
2. `Magic2` — `GNRL` (general) or `DX10` (textures/DDS); DX10 → DDS archive
3. `FileCount` (uint32)
4. `FileTableOffset` (int64)
5. **SF v2+**: `ReadUInt64` — "always set to 1, immediately discarded on load"
   (padding/hash placeholder)
6. **SF v3+**: `CompressionMethod` (uint32):
   - `COMPRESSION_METHOD_ZLIB = 0` (default)
   - `COMPRESSION_METHOD_LZ4 = 3`

### Compression support (per archive type; first entry = default)

| Archive type | Supported | Default |
|--------------|-----------|---------|
| `baSF` (Starfield general) | ZLib, LZ4 | **ZLib** |
| `baSFdds` (Starfield DDS/texture) | LZ4, ZLib | **LZ4** |

The official Archive2 tool creates **v3 (`0x03`) archives for lz4 compression only** —
the v3 header + `CompressionMethod` field is used specifically when LZ4 is chosen.

### Asset folders (root + extensions)

| Root | Extensions | Notes |
|------|-----------|-------|
| `meshes` | `.nif .kf .kfm .egm .egt .tri .psa .hkt .hkx .ssf .btr .bto .btt .dtl` | **`.hkx` animation files live here**; NIF `.nif` still a mesh extension (FO4-era) |
| `geometries` | `.mesh` | Starfield's **new geometry format** |
| `textures` | `.dds .tga .png` | |
| `materials` | `.bgsm .bgem` | |
| `scripts` / `source\scripts` | `.pex .psc` | |
| `strings` | `.strings .ilstrings .dlstrings` | |

### `.nif` vs `.mesh`

Starfield ships geometry as the new **`.mesh`** format under `geometries/`, while the
archive system still understands NIF `.nif` + `.hkx` in `meshes/` (legacy/FO4-era).
Whether the CE2 *runtime* reads `.nif` directly is not fully confirmed; the xEdit
model treats `.mesh` as the current geometry and `.nif` as legacy — MEDIUM confidence.

### Plugin (.esm/.esp) format

Confirmed from xEdit whatsnew (dev-4.1.6):

- Starfield `.esp` editing enabled with **small/medium master** support.
- **Master limits**: up to **252 full / 4095 light / 254 medium** masters per file
  (vs FO4's 254 full / 4095 light).
- **Blueprint master** concept: modules with a blueprint master can't be saved;
  blueprints can't be masters.
- An `.esp` may be loaded as a master for editing but cannot be saved as a master;
  small/medium flags cannot be set on `.esps`.
- Adding a master also adds its required up-chain masters (the engine requires the
  full master chain).
- xEdit ships `Core/Hardcoded/Starfield.esm` + `Core/wbDefinitionsSF1.pas`
  (Starfield record definitions, ~19k lines) and edits Starfield plugins in
  **"SF1" mode**.

## Starfield Script Extender (SFSE)

SFSE is the community script extender for Starfield, maintained by ianpatt and
behippo (the same team behind F4SE/SKSE).

- **Current build**: 0.2.21, targeting game version 1.16.244.
- **Availability**: Steam only. The MS Store/Game Pass release is explicitly
  **not supported**.
- **Install**: Nexus Mods page (mod ID 106); do not link directly to the download
  files. Use 7-Zip (or the installer) to extract — avoid Windows Store archivers.
- **Source**: `github.com/ianpatt/sfse`. Build with CMake:

  ```
  git clone https://github.com/ianpatt/sfse
  cmake -B sfse/build -S sfse
  cmake --build sfse/build --config Release
  ```

  This generates the solution at `sfse/build/umbrella.sln`. SFSE supports only the
  latest Starfield version on Steam — the Game Pass files are not loadable.

## Official Creation Kit

- Bethesda released the Starfield Creation Kit on 2024-06-09 (Steam AppID 2722710,
  free download).
- The official documentation wiki (`creationkit.com/starfield/`) is **down for
  backend maintenance** — and has been since **2024-02-07** (the maintenance page's
  own "last modified" date). The site is an XWiki install that returns a static
  maintenance page. Any status news is posted in the official Bethesda Game Studios
  Discord, `#modding-news` channel.
- `starfieldwiki.net` (community wiki) blocks scraping (HTTP 403 / Cloudflare), but
  its content is reachable via the MediaWiki API or direct page paste; key
  mesh/collision authoring guides are summarized in the next section.

## Mesh & Collision Authoring (3ds Max 2021)

Official BGS tooling for Starfield meshes ships in the game install:
`Starfield\Tools\ArtTools\BGS_Starfield_ArtTools_a.b.c.d.exe` — a 3ds Max 2021
plugin (Autodesk license required for the Max version). Workflow summarized from
the *Starfield Wiki* guides:

1. **Plugin setup**: create `%LOCALAPPDATA%\Autodesk\3dsMax\2021 - 64bit\ENU\en-US\plugcfg\BSToolSettings.ini`:

   ```ini
   [Config]
   ConfigFilesDir=C:\Users\<USERNAME>\AppData\Local\Autodesk\3dsMax\2021 - 64bit\ENU\en-US\plugcfg\BGS\Configs

   [Materials]
   LocalSourcePath=<PATH TO STARFIELD>\Source
   LocalDataPath=<PATH TO STARFIELD>\Data
   ```

   Skipping this makes collision materials fail to load in Max.
2. **Toolbar**: Customize → Customize User Interface → Toolbars tab, Group *Main UI*,
   Category *Bethesda* → drag **Open Bethesda FBX Export Tool** onto the toolbar.
3. **Utilities**: Utilities panel → Configure Button Sets → add **CollisionGroup** under
   *BGS Utility*.
4. **Author the mesh** with the FBX export-config naming convention (root node = export
   config name, e.g. `TestExport`).
5. **Collision** (Havok Content Tools → Physics): primitives (Sphere → Capsule →
   Cylinder/Box → Convex Hull → Mesh, in escalating cost) named `c_<name>`, linked as
   siblings under the mesh, then **Create Rigid Body with Proxy(s)** — the mesh gets a
   *Rigid Body* modifier, each collision a *Shape* modifier (set Shape Type, e.g. Box).
   Avoid converting primitives to Editable Poly/Mesh (breaks transform/scale).
6. **Collision materials**: with the CollisionGroup utility, set *Default Materials*
   (e.g. `MetalHollow`, `WoodHeavy`) and *Object Type* (Static) per collision mesh,
   then Apply To Selected. Materials define bullet reactions and footstep sounds.
7. **Export**: Open Bethesda FBX export → Export Checked; in Creation Kit place the
   static, select it and press **F4** to preview the collision overlay (red).

## Community Resources

| Resource | What it is | Notes |
|----------|-----------|-------|
| [SFSE on Nexus](https://www.nexusmods.com/starfield/mods/106) | SFSE download page | Build 0.2.21 / game 1.16.244 |
| [ianpatt/sfse](https://github.com/ianpatt/sfse) | SFSE source | CMake build, Steam only |
| [CommonLibSF](https://github.com/libxse/commonlibsf) | Collaborative reverse-engineered C++ library for Starfield | New home (libxse); the old [Starfield-Reverse-Engineering repo](https://github.com/Starfield-Reverse-Engineering/CommonLibSF) is unmaintained and redirects here |
| [Address Library for SFSE Plugins](https://www.nexusmods.com/starfield/mods/3256) | SFSE plugin address database | Required by many plugins |
| [CLibSFPluginTemplate](https://github.com/Starfield-Reverse-Engineering/CLibSFPluginTemplate) | SFSE/CommonLibSF plugin template | CMake / XMake variants |
| [TES5Edit (xEdit)](https://github.com/TES5Edit/TES5Edit) | Plugin editor | Starfield `.esp` editing in dev-4.1.6 ("SF1" mode; blueprints can't be saved as masters) |
| BSArch / Archive2 | BA2/BSA packer-unpacker | Source of the format facts above (bundled in TES5Edit repo) |
| [NifSkope](https://github.com/niftools/nifskope) | NIF editor | NIF still relevant for meshes; `.mesh` is not a NifSkope format |
| [hkxcmd](https://github.com/figment/hkxcmd) | Havok command-line tools | Skyrim/FO4-era; CE2 compatibility unverified |
| [hkxpack](https://github.com/Dexesttp/hkxpack) | HKX ↔ XML (TagXML) | hk_2014.1.0-r1 target (FO4); CE2 class changes unverified |
| [StarfieldMeshConverter](https://github.com/SesamePaste233/StarfieldMeshConverter) | Blender 3.5/3.6 plugin | Exports models, morph, geometry, and physics data in one click; incompatible with Blender 4.0+; [Nexus mod 4360](https://www.nexusmods.com/starfield/mods/4360) |

## What's Not Yet Documented (Open Questions)

These are honest gaps — no confirmed facts exist yet for the public record:

- **~~Exact Havok SDK version~~** — **ANSWERED (2026-08-18)**: Havok **2019.02**
  (`sdk_version` `"20190200"`), new chunked packfile format (see the finding in the
  Animation section). What remains open is the chunked-format layout details
  (chunk table, `PTCH` patch semantics, indexed-block addressing) and how the
  `hkb*`/`hka*` class serialization differs from the 2014-era SDK.
- **`.hkx` class layout vs FO4**: whether `hkbBehaviorGraph` / `hkaSkeleton` /
  `hkaAnimationBinding` class layouts changed vs FO4's 2014-era SDK — unverified;
  the 2019.02 SDK sits between FO4 (2014) and the 2020+ SDKs, so mid-generation
  class drift is expected.
- **FO4-era tool round-trip**: FO4-era tools (hkxpack, hkxcmd) **cannot** read
  Starfield `.hkx` as-is — the container changed (chunked vs classic packfile);
  a chunked-format port is required. Whether the class *payloads* would then be
  compatible is unverified.
- **Behavior project tooling**: whether the official Havok Behavior Tool or any
  community equivalent can author Starfield behavior projects — not confirmed.
- **`.nif` runtime use**: whether the CE2 runtime reads `.nif` directly or `.mesh`
  only — MEDIUM confidence.

## Sources

- [SFSE official site](https://sfse.silverlock.org/) — build 0.2.21, game 1.16.244,
  Steam-only compatibility.
- [ianpatt/sfse GitHub](https://github.com/ianpatt/sfse) — build instructions,
  runtime support statement.
- [SFSE on Nexus Mods](https://www.nexusmods.com/starfield/mods/106) — download page.
- [Address Library for SFSE Plugins](https://www.nexusmods.com/starfield/mods/3256) —
  required by many plugins.
- [CommonLibSF (new home)](https://github.com/libxse/commonlibsf) — reverse-engineered
  library for Starfield; the unmaintained
  [Starfield-Reverse-Engineering repo](https://github.com/Starfield-Reverse-Engineering/CommonLibSF)
  redirects here.
- [Creation Kit — Starfield](https://www.creationkit.com/starfield/) — **down for
  backend maintenance since 2024-02-07**; status posted in Bethesda Game Studios
  Discord `#modding-news`.
- [Starfield Wiki](https://starfieldwiki.net/wiki/Starfield:Starfield) — MediaWiki
  install (blocks bots with HTTP 403 / Cloudflare); guides "Setting up 3ds Max 2021
  for Starfield" and "Adding Collision with 3ds Max 2021" (last edited 2025-02-10)
  provided via direct page paste — summarized in the Mesh & Collision Authoring
  section.
- [StarfieldMeshConverter](https://github.com/SesamePaste233/StarfieldMeshConverter) —
  Blender 3.5/3.6 exporter for models/morph/geometry/physics (Nexus mod 4360).
- [TES5Edit whatsnew (dev-4.1.6)](https://github.com/TES5Edit/TES5Edit/blob/dev-4.1.6/whatsnew.md) —
  Starfield `.esp` editing support, `.mesh` geometries root, master limits.
- [TES5Edit `Core/wbBSArchive.pas` (dev-4.1.6)](https://github.com/TES5Edit/TES5Edit) —
  BSArch/BA2 source: `BTDX` magic, v2/v3 header versions, `CompressionMethod`
  (ZLIB/LZ4), compression defaults, asset-folder extensions.
- Steam Store AppDetails — Starfield (1716740), Starfield: Creation Kit (2722710),
  Shattered Space (2721670).
