# Research: Starfield (Creation Engine 2, 2023) — engine, animation, file formats, modding

Date: 2026-08-18
Status: research notes for a reference guide at `reference/bethesda/`
Confidence: HIGH for facts sourced from primary code (CommonLibSF, xEdit/BSArch) and
official sites (SFSE, Steam); MEDIUM where flagged.

> Scope: Starfield (2023) keeps the Bethesda **Havok-based `.hkx` animation lineage** from
> Fallout 4 (unlike Oblivion Remastered, which is a UE5 hybrid). CE2 drives animation
> through the **Animation Graph (AnimGraph)** framework — the same wrapper around Havok
> behavior that Skyrim SE / FO4 use. It is **64-bit** throughout.

---

## 1. Engine — Creation Engine 2

| Fact | Detail | Source (URL) | Confidence |
|------|--------|--------------|------------|
| Game | Starfield, released 2023-09-05 (Steam AppID 1716740); expansion *Shattered Space* 2024-09-30 (AppID 2721670) | Steam Store AppDetails API | HIGH |
| Engine | Creation Engine 2 (CE2), successor to the Creation Engine (FO4/Skyrim). 64-bit (x64). | CommonLibSF (x64, `SF_RTTI_VTABLE` etc.) | HIGH |
| Official CK | Starfield Creation Kit released 2024-06-09 (Steam AppID 2722710, free) | Steam Store AppDetails | HIGH |
| Script extender | SFSE (Starfield Script Extender) by ianpatt/behippo; Steam only (Game Pass NOT supported); build 0.2.21 / game 1.16.244 | sfse.silverlock.org; github.com/ianpatt/sfse; nexusmods.com/starfield/mods/106 | HIGH |

## 2. Animation — Havok `.hkx` + AnimGraph (CONFIRMED lineage)

The single most important finding: **Starfield retains the Havok behavior/animation `.hkx`
lineage**, exposed through the **Animation Graph (AnimGraph)** framework.

Primary-source evidence from CommonLibSF (`include/RE/`):
- `BSAnimationGraph` — base class (derives `BSIntrusiveRefCounted` + several `BSTEventSource`s). Subclass `AnimationManager : public BSAnimationGraph`.
- `BSAnimationGraphManager` (`sizeof == 0x80`) — the graph manager object.
- `BGSAnimationGraphComponent` (`sizeof == 0x40`) — Form component carrying the graph
  project/behavior names as `BSFixedString` members (`unk10..unk28`) + a smart-ptr.
- `IAnimationGraphManagerHolder` — interface with `NotifyAnimationGraphImpl`,
  `GetAnimationGraphManagerImpl`, `GetGraphVariableImpl{Float,Int,Bool}`, and
  `Pre/PostUpdateAnimationGraphManager`. This is **identical in role** to the Skyrim SE
  `BGSAnimationGraphManager` / `IAnimationGraphManagerHolder` AnimGraph system.
- **No `hkb*` or `hkx*` Havok types are exposed** in CommonLibSF — Havok behavior is
  handled opaquely behind the AnimGraph interface (same as Skyrim SE).

Implication (MEDIUM-HIGH confidence, inference from architecture):
- Starfield `.hkx` files (Havok packfile, same magic `0x57e0e057 0x10c0c010`) still carry
  behavior graphs (`hkbBehaviorGraph`), skeletons (`hkaSkeleton`), and animation bindings
  (`hkaAnimationBinding`), organized into **project / character / behavior file sets**.
- The AnimGraph framework (graph variables, `NotifyAnimationGraph` events) is the
  high-level API over those `.hkx` files — the same design Bethesda used since Skyrim SE.
- Behavior project authoring tooling for CE2 (official Havok Behavior Tool or a community
  equivalent) is **not confirmed** — see Open Questions.

| Fact | Detail | Source | Confidence |
|------|--------|--------|------------|
| Animation API | CE2 exposes `BSAnimationGraph`/`BSAnimationGraphManager`/`BGSAnimationGraphComponent`/`IAnimationGraphManagerHolder` | CommonLibSF include/RE (fetched 2026-08-18) | HIGH (primary code) |
| Behavior events | `NotifyAnimationGraph(const BSFixedString&)`, graph variables (float/int/bool) via `GetGraphVariable*` | CommonLibSF IAnimationGraphManagerHolder.h | HIGH |
| `.hkx` files | `.hkx` is a first-class **`meshes` asset-folder** extension in BSArch (see §3) | xEdit wbBSArchive.pas (`atMesh` ext list) | HIGH |
| Havok version string | **NOT confirmed** (e.g. does CE2 use `hk_2019.2`/`hk_2023.x`? no public contents-version fact found) | — | LOW / gap |
| `.hkx` class layout vs FO4 | Whether `hkb*`/`hka*` class layouts changed vs FO4's 2014 SDK — **unverified**; FO4-era tools (hkxcmd/hkxpack) may not round-trip | — | gap |

## 3. File formats — BA2 archives & assets (CONFIRMED from xEdit/BSArch source)

Authoritative from `TES5Edit/TES5Edit` `Core/wbBSArchive.pas` (dev-4.1.6), fetched 2026-08-18.
This is the same BSArch engine that packs/unpacks the game archives.

### BA2 / BSA header versions (magic 4 bytes)
- `MAGIC_BSA = 'BSA\0'` (classic TES4/FO3/SSE .bsa)
- `MAGIC_BTDX = 'BTDX'` (FO4 / Starfield .ba2)
- `MAGIC_GNRL = 'GNRL'`, `MAGIC_DX10` (BA2 subtype: general vs texture/DDS)
- Version values:
  - `0x67` TES4 (Oblivion), `0x68` FO3/FNV/TES5, `0x69` SSE
  - `0x01` FO4, `0x07`/`0x08` FO4 NG/AE
  - **`0x02` Starfield v2, `0x03` Starfield v3** (SF)

### Starfield BA2 header layout (after `BTDX` magic)
1. `Version` — `0x02` (SFv2) or `0x03` (SFv3)
2. `Magic2` — `GNRL` (general) or `DX10` (textures/DDS); DX10 → the archive is a DDS archive
3. `FileCount` (uint32)
4. `FileTableOffset` (int64)
5. **SF v2+**: `ReadUInt64` — "always set to 1, immediately discarded on load" (padding/hash placeholder)
6. **SF v3+**: `CompressionMethod` (uint32):
   - `COMPRESSION_METHOD_ZLIB = 0` (default)
   - `COMPRESSION_METHOD_LZ4 = 3`

### Compression support (per archive type; first = default)
- `baSF` (Starfield general): `[ctZLib, ctLZ4]` → default **ZLib**
- `baSFdds` (Starfield DDS/texture): `[ctLZ4, ctZLib]` → default **LZ4**
- (FO4 general `baFO4`: `[ctZLib]`; SSE `baSSE`: `[ctLZ4F]`)
- Note (line 1608): "official Archive2 tool creates v3 archives for lz4 compression only"
  → Starfield **v3 (0x03)** header + CompressionMethod is used specifically when LZ4 is chosen.

### Asset folders (root + extensions) — from `cBSAssets` table
- `meshes` (atMesh): `.nif .kf .kfm .egm .egt .tri .psa .hkt .hkx .ssf .btr .bto .btt .dtl`
  → **`.hkx` animation files live in `meshes/`**; NIF `.nif` is still a mesh extension (FO4-era).
- `geometries` (atGeometry): **`.mesh`** → Starfield's new geometry format, root `geometries/`
- `textures` (atTexture): `.dds .tga .png`
- `materials` (atMaterial): `.bgsm .bgem`
- `scripts` (atScript): `.pex .psc`; `source\scripts` (atSourceSSE): `.psc`
- `strings` (atStrings): `.strings .ilstrings .dlstrings`
- `sound\voice`, `sound`, `music`, `menus`, `interface`, `seq`, `vis`, `grass`, `trees`, `video` etc.

### Important distinction: `.nif` vs `.mesh`
- xEdit whatsnew (dev-4.1.6): "Add a **Geometries** root asset folder for Starfield **.mesh** files."
- Confirmed in `cBSAssets`: geometry uses `.mesh` under `geometries/`, while the `meshes/` root
  still lists `.nif` (legacy/FO4-era). So **Starfield ships geometry as `.mesh`** but the
  archive system still understands NIF `.nif` + `.hkx` in `meshes/`.
- Whether CE2 *runtime* reads `.nif` directly is not fully confirmed; the xEdit model treats
  `.mesh` as the current geometry and `.nif` as legacy. MEDIUM confidence.

## 4. Plugin (.esm/.esp) format — CONFIRMED from xEdit whatsnew (dev-4.1.6)

- Starfield `.esp` editing enabled with **small/medium master** support.
- **Master limits**: up to **252 full**, **4095 light**, **254 medium** masters in one file
  (vs FO4's 254 full / 4095 light).
- "small or medium masters" + **blueprint master** concept: modules with a blueprint master
  can't be saved; blueprints can't be masters.
- An `.esp` may be loaded as a master for editing but cannot be saved as a master; small/medium
  flags cannot be set on `.esps`.
- Adding a master also adds its required up-chain masters (engine requires the full master chain).
- Starfield record updates: `SCEN` changes (v1.8.83.0), Freelanes, Reflection organization,
  new DLC records; navmesh cover flag addition; wbDOBJ etc. moved to Common.
- xEdit ships `Core/Hardcoded/Starfield.esm` + `Core/wbDefinitionsSF1.pas` (Starfield record
  definitions, ~19k lines).
- `.mesh` geometry handled in xEdit's SNIFF via `Sniff/Proc/ProcAnalyzeMesh.pas` (uses
  zeux/meshoptimizer for vertex-cache/fetch efficiency metrics; also checks NIF `NifVersion >= nfFO4`).

## 5. Modding tools (Starfield)

| Tool | Role | URL | Status |
|------|------|-----|--------|
| SFSE | Starfield Script Extender (ianpatt/behippo) | https://sfse.silverlock.org/ ; https://github.com/ianpatt/sfse ; https://www.nexusmods.com/starfield/mods/106 | Active; Steam-only |
| Address Library for SFSE Plugins | SFSE plugin address database | https://www.nexusmods.com/starfield/mods/3256 | Required by many plugins |
| CommonLibSF | Collaborative RE C++ library | https://github.com/Starfield-Reverse-Engineering/CommonLibSF → **moved to https://github.com/libxse/commonlibsf** | Unmaintained repo redirects to libxse fork |
| CLibSFPluginTemplate / commonlibsf-template-xmake | Plugin templates (CMake / XMake) | https://github.com/Starfield-Reverse-Engineering/CLibSFPluginTemplate | — |
| Creation Kit (Starfield) | Official CK, AppID 2722710 | https://www.creationkit.com/starfield/ (wiki **down for maintenance**) | Wiki down |
| xEdit (TES5Edit) | Plugin editor w/ Starfield mode ("SF1") | https://github.com/TES5Edit/TES5Edit (dev-4.1.6+) | Active; `.esp` editing + small/medium master support |
| BSArch / Archive2 | BA2/BSA packer/unpacker (source of format facts above) | bundled in TES5Edit repo (`BSArch/`, `Core/wbBSArchive.pas`) | Active |
| NifSkope | NIF editor | https://github.com/niftools/nifskope | NIF still relevant for meshes; `.mesh` (SF) not a NifSkope format |
| hkxcmd | Havok cmd tools (Skyrim/FO4 era) | https://github.com/figment/hkxcmd | CE2 `.hkx` compat **unverified** |
| hkxpack | HKX ↔ XML (hk_2014.1.0-r1 target) | https://github.com/Dexesttp/hkxpack | CE2 class changes **unverified** |

## 6. Community resources

| Resource | URL | Notes |
|----------|-----|-------|
| SFSE official | https://sfse.silverlock.org/ | build 0.2.21 / game 1.16.244 |
| SFSE Nexus | https://www.nexusmods.com/starfield/mods/106 | download page |
| CommonLibSF (new home) | https://github.com/libxse/commonlibsf | unmaintained repo redirects here |
| Starfield Wiki | https://starfieldwiki.net/ | **HTTP 403 / Cloudflare** — not scrapable |
| UESP Starfield | https://en.uesp.net/wiki/Starfield:Starfield (via MediaWiki API) | UESP hosts Starfield namespace; use API form |
| Creation Kit wiki | https://www.creationkit.com/starfield/ | **down for backend maintenance** |
| Bethesda Game Studios Discord | https://discord.com/invite/BethesdaStudios | official; `#modding-news` channel posts CK status |

## 7. URLs that could NOT be accessed (user may paste content)

1. `https://www.creationkit.com/starfield/` and `.../starfield/index.php?title=Main_Page` — XWiki
   returns "This wiki is currently down for backend maintenance" (as of 2026-08-18).
2. `https://starfieldwiki.net/wiki/...` and its `api.php` — Cloudflare "Just a moment..."
   challenge (HTTP 403).
3. `https://www.nexusmods.com/starfield/mods/106` — page shell fetched (7 lines) but description
   not rendered.
4. DuckDuckGo / Bing HTML search — served anomaly/captcha pages; no organic results usable.
5. No public source found stating the exact **Havok contents-version string** for Starfield
   `.hkx` files (e.g. `hk_2014.1.0-r1` vs a newer SDK). This is the biggest remaining gap.

## 8. Guide-writing pointers (for later)

- Emphasize the **Animation Graph (AnimGraph)** framework as the bridge: CE2 keeps Havok `.hkx`
  behavior/animation but exposes it via `BSAnimationGraph`/`BSAnimationGraphManager`
  (graph variables + `NotifyAnimationGraph` events) — same architecture as Skyrim SE.
- `.hkx` files remain in the `meshes/` asset folder inside `.ba2` archives.
- BA2 format: `BTDX` magic, version `0x02`/`0x03`, `GNRL`/`DX10` subtype, ZLib default
  (LZ4 for DDS, or for v3 archives packed by Archive2). This is a concrete, verifiable spec.
- `.esm/.esp`: 252 full / 4095 light / 254 medium masters; small/medium/blueprint master concepts.
- Geometry moved to **`.mesh`** under `geometries/`; NIF `.nif` retained as legacy mesh.
- Honest gaps: exact Havok SDK version, `.hkx` class layout vs FO4, behavior-authoring tooling.
