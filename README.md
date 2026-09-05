---
layout: default
title: Open Source Modding
permalink: /
---

# Open Source Modding

Reverse engineering and modding research for game engines, primarily the **Disrupt engine** (Watch Dogs 1/2/Legion).

---

## Disrupt Engine (Cross-Game)

- [Engine Lineage](reference/disrupt/engine-lineage.md) — Dunia 2 fork, why Far Cry tools work on Disrupt
- [Tool Gotchas](reference/disrupt/tool-gotchas.md) — Gibbed >4GB breakage, UnpackWD2 wrong for WDL, FCBastard WD1-only
- [Blender Addon](reference/disrupt/blender-addon.md) — import/export/inject XBG/GLM, HKX, MAB, .material.bin
- [GLM Format](reference/disrupt/glm-format.md) — Havok skeletal mesh text format (skeleton, Reflex, SecondaryMotion, geometry)
- [XBGMIP Format](reference/disrupt/xbgmip-format.md) — streamed LOD0 companion (PIMG header + vertex/index blocks)
- [XBT Texture Format](reference/disrupt/xbt-format.md) — DDS wrapper
- [Depload Format](reference/disrupt/depload-format.md) — resource dependency/preload tables
- [Compressed Mesh Format](reference/disrupt/compressed_mesh_format.md)
- [Custom Model Workflow](reference/disrupt/custom-model-workflow.md)
- [Weapon Adding Guide](reference/disrupt/weapon-adding-guide.md)
- [WLU Format](reference/disrupt/wlu-format.md)
- [installpackage Patch](reference/disrupt/installpackage-patch.md) — DLL hex edit for mod loading
- [Havok Repos](reference/disrupt/havok-repos.md)
- [Disrupt Editor](reference/disrupt/disrupt-editor.md)

## Watch Dogs 1

- [Format Reference Index](reference/disrupt/watch_dogs/index.md) — all WD1 format docs
- [FAT/DAT Archive Format](reference/disrupt/watch_dogs/fat-archive-format.md) — packfile structure
- [XBG Format (GeomParser)](reference/disrupt/watch_dogs/xbg-format-geparser.md) — compiler decompile reference
- [MAB Animation Format](reference/disrupt/watch_dogs/mab-format.md) — custom Disrupt animation (magic `aNi`, bitstream decoder)
- [MAC Animation Format](reference/disrupt/watch_dogs/mac-format.md)
- [HKX Collision Format](reference/disrupt/watch_dogs/hkx-format.md) — Disrupt's modified Havok collision
- [BFD Sprite Format](reference/disrupt/watch_dogs/bfd-format.md) — UI sprite atlas definitions
- [SPK Sound Format](reference/disrupt/watch_dogs/spk-format.md) — sound package header structure
- [FEU Format](reference/disrupt/watch_dogs/feu-format.md)
- [SLID Format](reference/disrupt/watch_dogs/slid-format.md)
- [Markup Format](reference/disrupt/watch_dogs/markup-format.md)
- [Material Bin Format](reference/disrupt/watch_dogs/material-bin-format.md)
- [Material Hex Editing](reference/disrupt/watch_dogs/material-hex-editing.md)
- [Material Descriptors](reference/disrupt/watch_dogs/materialdescriptors/) — 44 shader types, car shaders
- [Lighting Parameters](reference/disrupt/watch_dogs/lighting-parameters.md)
- [Lighting Pipeline Hooks](reference/disrupt/watch_dogs/lighting-pipeline-hooks.md)
- [Shader Addons](reference/disrupt/watch_dogs/shader-addons.md)
- [Shader Editing Workflow](reference/disrupt/watch_dogs/shader-editing-workflow.md)
- [SMAA Notes](reference/disrupt/watch_dogs/smaa-notes.md)
- [Mirror Cubemap Mod](reference/disrupt/watch_dogs/mirror-cubemap-mod.md)
- [Player Watch Component](reference/disrupt/watch_dogs/player-watch-component.md)
- [SSRTGI Project](reference/disrupt/watch_dogs/ssrtgi-project.md)
- [Injection Internals](reference/disrupt/watch_dogs/injection-internals.md)
- [Archive Priorities](reference/disrupt/watch_dogs/archive-priorities.md)
- [Wii U Debug Symbols](reference/disrupt/watch_dogs/wii-u-debug-symbols.md)
- [Wii U RPX Symbols](reference/disrupt/watch_dogs/wii-u-rpx-to-elf.md)
- [Hashing](reference/disrupt/watch_dogs/hashing.md) — CRC32, FNV32, FNV64, CRC64_WD2

## Watch Dogs 2

- [Community Resources & Tools](reference/disrupt/watch_dogs_2/community-resources.md) — OpenDogs, Volfin importer, ZModeler
- [Model Format](reference/disrupt/watch_dogs_2/model-format.md) — `.model` XBG+material binding
- [Model Hash Mappings](reference/disrupt/watch_dogs_2/model-hash-mappings.md) — vegetation/character prop CRC32 hashes
- [RebuildFileLists Schema](reference/disrupt/watch_dogs_2/rebuildfilelists-schema.md) — all 16 ToIndex types
- [Vehicle Handling Modding](reference/disrupt/watch_dogs_2/vehicle-handling-modding.md)
- [TAG0 Collision Format](reference/disrupt/watch_dogs_2/wd2-tag0-collision-format.md) — Havok 2015.1 compendium
- [XBT Texture Format (WD2)](reference/disrupt/watch_dogs_2/xbt-texture-format.md)
- [Item Clothing Tutorial](reference/disrupt/watch_dogs_2/wd2-item-clothing-tutorial.md)

## Watch Dogs: Legion

- [Engine Reference](reference/disrupt/watch_dogs_legion/wdl_engine_reference.md) — Denuvo RE, synthetic PDB, RTTI extraction
- [Modding Workflow](reference/disrupt/watch_dogs_legion/modding-workflow.md) — UnpackLegion → edit → DisruptManager
- [Denuvo Analysis](reference/disrupt/watch_dogs_legion/denuvo_analysis.md)
- [HKX Format Reverse](reference/disrupt/watch_dogs_legion/HKX_FORMAT_REVERSE.md) — Havok 2017.2 chunked format
- [Vehicle Adding Process](reference/disrupt/watch_dogs_legion/vehicle-add-process.md)
- [Material Bin Format (WDL)](reference/disrupt/watch_dogs_legion/material-bin-format.md)
- [Primitive Files](reference/disrupt/watch_dogs_legion/legion-primitive-files.md)
- [XBT Format (WDL)](reference/disrupt/watch_dogs_legion/xbt-format.md)
- [PreparePlatformData Pipeline](reference/disrupt/watch_dogs_legion/prepareplatformdata-pipeline.md) — world/shader compilation
- [Physics Working Notes](reference/disrupt/watch_dogs_legion/wdl-phys-working-notes-2026-08-19.md)

## Reference (All Games)

- [Reference Index](reference/index.md) — Bethesda, NFS, L4D, Sims 3, Ghost Recon, Mirror's Edge, Zelda, Cyberpunk
- [Community Tools](reference/disrupt/watch_dogs/community-resources.md)
