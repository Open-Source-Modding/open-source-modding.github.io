# Watch Dogs Reference

## Formats
- [HKX Collision Format](hkx-format.md) — WD1/WD2 (Ubisoft's Havok fork) general overview
- [WD2 TAG0 Collision Format](../watch_dogs_2/wd2-tag0-collision-format.md) — Havok 2015.1 compendium: item/fixup tables, shape graph, 11:11:10 quant decode, hkx vs .phys type tables
- [Packfile Format](packfile-format.md) — old packfile (Havok 2012) + TAG0 chunk format, fixup resolution
- [FAT/DAT Archive Format](fat-archive-format.md) — FAT2/FAT3/FAT5 variants, entry layouts, hash differences, .nfo pairing
- [Compressed Mesh Format](compressed-mesh-format.md) — hkpBvCompressedMeshTree structure, section/primitive/dataRun layout
- [Injection Internals](injection-internals.md) — WD1/WD2 HKX injection pipelines, format differences
- [MAC Binary Format](mac-format.md) — source animation clip format (curves, skeleton, events, recursive parts)
- [MAB Animation Format](mab-format.md) — compiled runtime animation (Oodle compressed, quantized u8 curves)
- [Markup XML Format](markup-format.md) — event definitions referencing MAC files (footsteps, sounds, attacks)
- [XBG Model Format (GeomParser Decompile)](xbg-format-geparser.md) — header, hierarchy, mesh write order, bone mapping, channel refs
- [Material Hex Editing](material-hex-editing.md) — Razor tool, CRC32 lookup, hex workflow, converter bugs
- [Material Binary Format](material-bin-format.md) — TAM\0 v7/v15 structure, param command types, ConvertMaterials.exe XML editing workflow
- [XBT Texture Format](xbt-texture-format.md) — TBX header structure, _high texture system, hex analysis, hV_WD1ModdingKit workflow
- [BFD Sprite Atlas Format](bfd-format.md) — UI icon mappings, paired with XBT textures, percentage-based coordinates
- [SPK Sound Package Format](spk-format.md) — Wwise audio wrapper, header fields, bus IDs, music library integration
- [Hashing (CRC32 / CRC64_WD2)](hashing.md)
- [FEU Format (Flash UI)](feu-format.md) — SWF with UEF header, JPEXS editing, AS2 UI class hierarchy, Linux conversion
- [Entity XML Structure](entity-xml-structure.md) — prototype format, hash field conventions, components
- [Material Descriptors](material-descriptors.md) — shader parameter schema XMLs (94 in leak, 46 in WD1)

## Modding
- [blender-io-disrupt addon](../blender-addon.md) — Blender 5.0 add-on: import/export/inject XBG/GLM models, HKX collision, MAB animation, .material.bin
- [installpackage-over-patch](installpackage-patch.md) — DLL hex edit to avoid repacking patch
- [Archive Load Priority](archive-priorities.md) — per-game archive order, DisruptManager RE
- [WD2 Custom Item/Clothing Tutorial](wd2-item-clothing-tutorial.md) — graphickit_models/parts, items.lib, shop setup
- [WD2 Vehicle Handling](../watch_dogs_2/vehicle-handling-modding.md) — DriveConverter, .handling.bin ↔ XML, patch3 workflow
- [Mirror Cubemap Mod](mirror-cubemap-mod.md) — make mirrors reflective: ReflectionType=2 + ReflectionTexture via official converter
- [Shader Addons](shader-addons.md) — Parallellines' addonconfig.h, PBR extension, lighting fixes, community context
- [Shader Editing Workflow](shader-editing-workflow.md) — direct shadersobj replacement vs 3Dmigoto injection, Legion leak compiler, gridshading
- [Asset Pipeline Formats](asset-pipeline-formats.md) — dev pipeline format mapping (authoring → runtime conversions)
- [Community tools & references](community-resources.md)
- [SMAA Notes](smaa-notes.md) — edge detection settings, community research

## Research
- [Lighting Pipeline Hooks](lighting-pipeline-hooks.md) — renderer queue RVAs, GPU light structs (SSunLight/SOmniLight/SSpotLight), LightData CB, GI probes
- [Environment Lighting Parameters](lighting-parameters.md) — environmentlighting.lib reference: sun/moon, exposure, GI probes, Dunia legacy
- [SSRTGI Project](ssrtgi-project.md) — D3D11 screen-space GI ASI: UpdateCamera RVA (0x170ED50), vtable indices, HDR pre-tonemap composite, light-capture hook status
- [Wii U Debug Symbols](wii-u-debug-symbols.md) — Full unstripped symbols from USA retail disc (317K functions), Cafe OS ELF format, decompression method
- [Wii U RPX → ELF Conversion](wii-u-rpx-to-elf.md) — all sections zlib-compressed, rpx_to_elf.py converter, 292K-name IDC script for IDA
- [SLID Format](slid-format.md) — sound event → .spk bank lookup table (13,581 entries, no magic)
- [Player Watch Component](player-watch-component.md) — cut early-alpha wristwatch component (Feb 2012), fixed-speed seconds hand
- [Depload Format](../depload-format.md) — cross-game dependency preload tables (WD1 32-bit / WD2+WDL 64-bit masked CRC64)
- [Havok Repos](../havok-repos.md) — GitHub/GitLab repos with HKX parsing (hkparse, HKX2, BlenderAddon, etc.)
- [Legion Primitive Files](../watch_dogs_legion/legion-primitive-files.md) — Havok 2017.2 class definitions from WDL leak
- [Decompiled DLLs](decompiled-dlls.md) — IDA Hex-Rays output from Ubisoft leak (GeomParser, DuniaDemo, gxasset, etc.)

## Watch Dogs 2
- [Community Resources](../watch_dogs_2/community-resources.md) — gun sounds, radio, police mod, cut content, WatchBox

## Data
- [Material descriptors](materialdescriptors/)
