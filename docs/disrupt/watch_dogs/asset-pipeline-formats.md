# Disrupt Engine — Asset Pipeline Format Mapping

> **Source**: OCR of WDL dev pipeline reference (`wdl_format_info.txt`), cross-referenced with community findings.

The Disrupt engine converts authoring formats into runtime binary formats during the build process (`PreparePlatformData64.exe`). Here is the mapping from source/authoring format to compiled/runtime format.

## Format Conversions

| Authoring Format | Runtime Format | Description |
|------------------|----------------|-------------|
| `material.xml` | `.material.bin` | Material descriptors (TAM v7 binary, magic `0x004D4154`) |
| `.mac` + `.markup` | `.mab` + `.markup.bin` | Animation clips — MAC (source binary) + markup (XML events) compiled via CMarkupCompiler to MAB (Oodle-compressed runtime) + markup.bin (10-byte header) |
| `.seq` | `.cseq` | Cutscene sequences (QES binary format) |
| `_move.xml` | `_move.bin` | Movement/locomotion data |
| `_markup` | `_markup.bin` | UI markup data |
| `.1ft` | `.xIf` | Physics fixture data |
| `.tree_xml` / `.impostor.xml` | `.bin` | Vegetation tree + impostor data |
| `.bik` | `.bik` / `.biklight` | Bink video (light variant for pre-rendered sequences) |
| `.fba` / `.animset` | `.dpax` | Animation blend trees / animation sets |
| `-dpds` | `_dpdx` | Depload dependency data (compiled) |

## Resource Class Registry (WD2 editor strings)

Extracted from WD2 memory dumps (noah Gooder, Dec 2018). These define the
engine's internal resource type system — which source extensions map to which
compiled runtime extensions:

| Resource Class | Source Extensions | Target Extension | Notes |
|----------------|-------------------|------------------|-------|
| `CActorDialogPackResource` | *(none)* | `.stimuli.dsc.pack` | Dialog stimulus packs |
| `CAnimationTrackCollectionResource` | `.animtrackcol.xml` | `.animtrackcol.rml` | Animation track collections |
| `CBinkResource` | `.bik` | `.bik` | Bink video (passthrough) |
| `CFuzzyLogicTableResource` | `.fzl.xml` | `.fzl` | Fuzzy logic tables |
| `CGeometryResource` | `.glm` `.dae` `.gamex` | `.xbg` | **3D models** — GLM (text), DAE (Collada), GameX (binary) are all geometry source |
| `CMaterialResource` | `.material.xml` | `.material.bin` | Material descriptors |
| `CModelResource` | `.model` | `.model` | Model descriptors (bind XBG + material) |
| `CMoveResource` | `.move.xml` | `.move.bin` | Movement/locomotion data |
| `CNomadResource` | `.impostor.xml` | `.impostor.bin` | Vegetation impostors |
| `CNomadResource` | `.tree.xml` | `.tree.bin` | Vegetation trees |
| `CPhysResource` | `.hkr` | `.hkx` | Physics (Havok) — optional `.gamex` source |
| `CPhysResource` | `.tree.hkr` | `.tree.hkx` | Vegetation physics |
| `CSequenceResource` | `.seq` | `.cseq` | Cutscene sequences (deps: `.naseq` `.liseq` `.auseq` `.fxseq` `.prseq`) |
| `CSkeletonResource` | `.skel.xml` | `.skeleton` | Skeleton definitions |
| `CSoundResource` | *(none)* | `.bnk` | Sound banks |
| `CStateMachineResource` | `.gosm.xml` | `.gosm.xml` | Game object state machines (passthrough) |
| `CTextureResource` | `.png` `.dds` `.hdr` | `.xbt` | Textures |
| `CWorldLoadingUnitResource` | `.data.fcb` | `.data.fcb` | World loading units (passthrough) |

### Editor Indexer Types

The engine scans for assets via `ToIndex` entries in the build pipeline:

| Indexer Type | Extensions | FolderPath | Notes |
|-------------|-----------|------------|-------|
| `GLMToIndex` | `.glm` | `graphics` | Geometry source files |
| `HkrToIndex` | `.hkr` | `graphics` | Havok physics |
| `TextureToIndex` | `.png` | `graphics;ui;editor` | Texture sources (SmallMax=16K, BigMax=4M) |
| `NomadToIndex` | `.tree.xml` | `graphics` | Vegetation |
| `EntityToIndex` | `.xml` | `worlds;missions;entitylayers` | World entities |
| `ResourceFileToIndex` | `.dae` | `graphics;editor\scenes` | Collada geometry (XmlInvokeDep) |
| `ResourceFileToIndex` | `.gamex` | `graphics` | GameX binary geometry |
| `ResourceFileToIndex` | `.lua` | `domino` | Lua scripts |
| `ResourceFileToIndex` | `.markup` | `graphics;animations` | Animation markup (TriggerKey=MarkupToAnim) |
| `ResourceFileToIndex` | `.dds` | `graphics;ui;editor` | DDS textures |
| `ResourceFileToIndex` | `.seq` `.naseq` `.anseq` `.liseq` `.fxseq` | `sequences` | Cutscene sequences |
| `ResourceFileToIndex` | `.skel.xml` | `graphics` | Skeleton XML |
| `ResourceFileToIndex` | `.animtrackcol.xml` | `animations` | Animation tracks |
| `ResourceFileToIndex` | `.stimuli.dsc` | `dialog` | Dialog stimuli |
| `DominoToIndex` | `.domino` | `domino` | Domino scripts |
| `StateMachineBlobToIndex` | `.xml` | `engine\scripts\game\objects\pawn\stateMachine` | Pawn state machines |
| `XmlDepToIndex` | `.xml` | `graphics;editor\scenes` | XML dependencies (FileExtensions: `glm;hkr;gamex;dae`) |

Key insight: **`.dae` and `.gamex` are alternative geometry source formats** —
same `CGeometryResource` class as `.glm`. The GLM text parser should handle
all three with format detection (GLM = text tabs, DAE = Collada XML, GameX = binary).

## Other Asset Categories

| Category | Notes |
|----------|-------|
| Road/spline | Geometry + loft (graphics/physics) |
| Sound banks | `.bnk` / `.spk` audio containers |
| Character customization | Graphickit parts, clothing definitions |
| Lua scripts | Compiled to bytecode (not source — game reads compiled) |
| World/terrain | Terrain chunks, navmesh, world generation |
| Specialized Havok variants | Physics data (collision, ragdoll, vehicle) |
| AI/world systems | Behavior trees, spawn tables, traffic patterns |

## Key Notes

- **`.fx` shader source is NOT compiled** — the game reads compiled shaders from `shadersobj.dat`, not `.fx` source from `shaders.dat`
- **`.material.bin` uses TAM v7** — magic bytes `54 41 4D 00` (little-endian). Beta builds used v5 big-endian (`00 4D 41 54`)
- **`.cseq` cutscenes** are QES (Quake Engine Sequence) binary format — 307 files in WD1 unpacked data
- **Lua scripts** are compiled to bytecode at build time — decompilation required for editing
- **Depload** (`_dpds` → `_dpdx`) is the dependency preload manifest — see [Depload Format](../depload-format.md)

## Related

- [Material Binary Format](material-bin-format.md) — TAM v7 structure
- [MAC Binary Format](mac-format.md) — animation source format (curves, skeleton, events)
- [MAB Animation Format](mab-format.md) — compiled runtime animation format
- [Markup XML Format](markup-format.md) — event definitions for animation clips
- [Depload Format](../depload-format.md) — dependency preload tables
- [Shader Editing Workflow](shader-editing-workflow.md) — why .fx source isn't used
