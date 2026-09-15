# Reference

## Havok (generic)
- [HKX packfile format spec](havok/hkx_format.md) — standard Havok packfile (FO4 2014, `__classnames__`/`__types__`/`__data__`)

## Bethesda
- [Bethesda Archive, Model & Texture Formats](bethesda/bethesda-formats.md) — BSA/BA2, NIF (Skyrim/FO4/FO76/Switch), BCx textures, audio, saves, cell data, tools
- [Fallout 4 Havok animation guide](bethesda/fallout4-havok-guide.md) — FO4 behavior/animation workflow
- [Skyrim Havok animation reference](bethesda/skyrim-havok-guide.md) — Skyrim LE/SE/AE .hkx pipeline, hkxcmd/KF round-trip
- [Oblivion Remastered reference](bethesda/oblivion-remastered-guide.md) — UE5 + Gamebryo hybrid, pak assets, animation workflow
- [Starfield modding & RE reference](bethesda/starfield-guide.md) — CE2, SFSE, Creation Kit status, open questions

## Watch Dogs (Disrupt engine)
### Cross-Game
- [Engine Lineage](disrupt/engine-lineage.md) — Dunia 2 fork, why Far Cry tools work on Disrupt
- [Tool Gotchas](disrupt/tool-gotchas.md) — Gibbed >4GB, UnpackWD2 wrong for WDL, FCBastard WD1-only
- [Blender Addon](disrupt/blender-addon.md) — import/export/inject XBG/GLM, HKX, MAB, .material.bin
- [GLM Format](disrupt/glm-format.md) — Havok skeletal mesh text format
- [XBGMIP Format](disrupt/xbgmip-format.mdx) — streamed LOD0 companion (PIMG header + vertex/index blocks)
- [Depload Format](disrupt/depload-format.md) — resource dependency/preload tables
- [Compressed Mesh](disrupt/compressed_mesh_format.md)
- [Hashing](disrupt/watch_dogs/hashing.md) — CRC32, FNV32, FNV64, CRC64_WD2
- [Material Descriptors](disrupt/watch_dogs/materialdescriptors/index.md) — 44 shader types

### Watch Dogs 1
- [FAT/DAT Archive Format](disrupt/watch_dogs/fat-archive-format.md)
- [XBG Format (GeomParser)](disrupt/watch_dogs/xbg-format-geparser.md) — compiler decompile
- [MAB Animation](disrupt/watch_dogs/mab-format.md) — custom Disrupt animation
- [HKX Collision](disrupt/watch_dogs/hkx-format.md)
- [BFD Sprite Format](disrupt/watch_dogs/bfd-format.md)
- [SPK Sound Format](disrupt/watch_dogs/spk-format.md)
- [Material Bin Format](disrupt/watch_dogs/material-bin-format.md)
- [Injection Internals](disrupt/watch_dogs/injection-internals.md)
- [Archive Priorities](disrupt/watch_dogs/archive-priorities.md)
- [Lighting Parameters](disrupt/watch_dogs/lighting-parameters.md)
- [Shader Editing](disrupt/watch_dogs/shader-editing-workflow.md)
- [Engine Modules](disrupt/watch_dogs/engine-modules.md) — internal library codenames (Pilot pathfinding, Storm networking, Gears serializer, DARE audio, depload, APS\* progression, CDebugRecorder); cut-content `ToBeDeleted_` prefix
- [Wii U Debug Symbols](disrupt/watch_dogs/wii-u-debug-symbols.md)

### Watch Dogs 2
- [Community Resources](disrupt/watch_dogs_2/community-resources.md) — OpenDogs, Volfin importer
- [Model Format](disrupt/watch_dogs_2/model-format.md) — `.model` XBG+material binding
- [Model Hash Mappings](disrupt/watch_dogs_2/model-hash-mappings.md)
- [RebuildFileLists Schema](disrupt/watch_dogs_2/rebuildfilelists-schema.md)
- [Vehicle Handling](disrupt/watch_dogs_2/vehicle-handling-modding.md)
- [TAG0 Collision](disrupt/watch_dogs_2/wd2-tag0-collision-format.md)

### Watch Dogs: Legion
- [Engine Reference](disrupt/watch_dogs_legion/wdl_engine_reference.md)
- [Modding Workflow](disrupt/watch_dogs_legion/modding-workflow.md)
- [HKX (Havok Physics) Dunia Wrapper Format](disrupt/watch_dogs_legion/HKX_FORMAT_REVERSE.md)
- [Vehicle Adding Process](disrupt/watch_dogs_legion/vehicle-add-process.md)
- [PreparePlatformData](disrupt/watch_dogs_legion/prepareplatformdata-pipeline.md)

## Far Cry (Dunia engine)
- [XBG Format Notes (FC5 vs FC6)](farcry/xbg-format.md) — GMESH/IMESH magic vs Disrupt GEOM, SDOL/LODS chunk layout, FC5 stride 40 vs FC6 richer descriptor table, packed/quantized vertices
- [Shader system lineage (FC1 → FC6)](farcry/shader-system-lineage.md) — FC1 runtime-preprocessed text scripts (Cg/D3DX, gen-mask defines) → Disrupt/FC6 offline ShaderCompiler2 DXBC + `<technique>` XML; FC1 render-state tokens map 1:1 to FC6 shadersobj.dat elements
- [`.cgf` chunk → `.xbg` lineage](farcry/cgf-chunk-lineage.md) — FC1 CryChunkedFile ChunkTypes enum + CHUNK_HEADER{Type,Version,Offset,ID} is the ancestor of the FC5/FC6 XBG chunk chain; FC1 Mesh/Node/Mtl/BoneNameList/BoneAnim chunks map to SULC/EDON/LTMR/DNKS/ITOM; ResourceCompiler origins; .pak (not .dat/.fat) archive in FC1
- [Raw XeNTaX extraction](farcry/xentax-farcry-knowledge.md) — source threads (2018–2023): FAT v10→v11 change, hook/filelist filename recovery, FC6 XBG short-signed XZY verts per-LOD, SDOL chunk struct, FC6Model.exe + .bwsk/.dpdx facial-bone workflow
- [File lists (hash → name)](farcry/filelists.md) — concatenated/ raw accumulation → OSM FarCry*-File-Lists submodules via RebuildFileLists; CRC64 path hashing, FAT v11 decode, positional install-path usage

- [MAB animation format (FC3, buu342 RE)](farcry/mab-animation-format.md) — MAB version bytes per game (0x4C FC2 … 0xB0 FC5+ND), 9-section offset table at +200, 48-bit compressed quaternions (sqrt-W reconstruction), mystery-byte = quaternion-count bitmask, frame-array interpolation (FC4+); source knockout.chat/thread/55079 + github.com/buu342/PY-DuniaAnimationExtractor
- [Far Cry modding (community docs)](farcry/modding/index.md) — FCModInstaller workflows (package/replace/option/template from the .docx) + community guides (FC4 materials/suppressors, FC5 outfit BWSK, FC6 Wwise music, FOV hex editing, StimTypes)

## XeNTaX Forum Knowledge (all games)
Extracted from the XeNTaX forum dump (2004–2023). Each game's knowledge doc lives alongside its formats reference:

## Watch Dogs
- [Watch Dogs knowledge](disrupt/watch_dogs/xentax-watchdogs-knowledge.md) — FAT/DAT, XBG, XBT, localization, audio, materials

## Havok
- [Havok knowledge](havok/xentax-havok-knowledge.md) — HKX versions, animation formats, tools, version compat

## Bethesda
- [Bethesda knowledge](bethesda/xentax-bethesda-knowledge.md) — BSA, BA2, NIF, XWM, save formats

## Need for Speed
- [NFS Archive, Model & Texture Formats](nfs/nfs-formats.md) — VIV/BIG, BIN/BUN, TPK, BFF (Shift), BML/BMT, SGB tracks, audio (MUS/SPS/GSTR/FSB), XTX textures, tools
- [Raw XeNTaX extraction](nfs/xentax-nfs-knowledge.md) — source threads (2005–2010)

## Left 4 Dead
- [L4D Archive & Texture Formats](left4dead/left4dead-formats.md) — VPK (PC), VTFX (X360), 360.zip, Source SDK encryption
- [Raw XeNTaX extraction](left4dead/xentax-left4dead-knowledge.md) — source threads (2008–2017)

## The Sims 3
- [Sims 3 Package & Audio Formats](sims3/sims3-formats.md) — DBPF v2 packages, PC vs X360 endianness, EAlayer3/GC_ADPCM audio, scripts.package
- [Raw XeNTaX extraction](sims3/xentax-sims3-knowledge.md) — source threads (2009–2012)

## Ghost Recon
- [Ghost Recon Archive & Audio Formats](ghostrecon/ghostrecon-formats.md) — GR2 Unreal, GRAW .bundle/PS2 PKG/IMG, X360 xbadpcm, GRFS Yeti/GEAR BIG (CRC32 names, multi-part)
- [Raw XeNTaX extraction](ghostrecon/xentax-ghostrecon-knowledge.md) — source threads (2005–2018)

## Mirror's Edge
- [Mirror's Edge / Catalyst Frostbite Pipeline](mirrors-edge/mirrors-edge-formats.md) — Frostbite 3 bundle/chunk/EBX, EALayer3 audio, EBX numDict field types, original ME texture ripping
- [Raw XeNTaX extraction](mirrors-edge/xentax-mirrors-edge-knowledge.md) — source threads (2008–2017)

## SimCity
- [SimCity Audio Format](simcity/simcity-formats.md) — RIFF Vorbis `.wav`, ww2ogg/revorb pipeline
- [Raw XeNTaX extraction](simcity/xentax-simcity-knowledge.md) — source thread (2014)

## Cities: Skylines
- [Cities: Skylines Formats](citiesskylines/citiesskylines-formats.md) — .locale binary, .crp map/asset container
- [Raw XeNTaX extraction](citiesskylines/xentax-cities-skylines-knowledge.md) — source threads (2015–2022)

## Trackmania / Maniaplanet
- [Trackmania Formats](trackmania/trackmania-formats.md) — GBX format, NadeoPak (Blowfish encrypted), Noesis plugin, model/audio extraction
- [Raw XeNTaX extraction](trackmania/xentax-trackmania-knowledge.md) — source threads (2013–2023)

## The Witcher 2 (REDengine v1)
- [Witcher 2 Formats](witcher2/witcher2-formats.md) — .dzip archives, .w2ent/.w2mesh models, .xbm textures, .w2speech dialogue, .usm video, .fsb music, tools
- [Raw XeNTaX extraction](witcher2/xentax-witcher2-knowledge.md) — source threads (2011–2022)

## The Witcher 3 (REDengine v2)
- [Witcher 3 Formats](witcher3/witcher3-formats.md) — .bundle/.cache archives, .xbm (CR2W) textures, .w3strings localization, .w3speech voices, .sav saves (LZ4 1MB chunks), meshes/bones (48B/bone), tools
- [Raw XeNTaX extraction](witcher3/xentax-witcher3-knowledge.md) — source threads (2015–2020)

## Cyberpunk 2077 (REDengine 4)
- [Cyberpunk 2077 Formats](cyberpunk2077/cyberpunk2077-formats.md) — .archive (RDAR+KARK), Oodle compression, .mesh/.buffer/.rig/.morphtarget models, .xbm/.mi textures, Wwise .wem/.opuspak/.bnk audio, tools (CP77Tools, Wolven-kit, Noesis fmt_CP77mesh)
- [Raw XeNTaX extraction](cyberpunk2077/xentax-cyberpunk-knowledge.md) — source threads (2020–2023)

## Zelda (Wii U / 3DS / Switch)
- [Zelda Formats](zelda/zelda-formats.md) — Yaz0/SARC (BotW), BWAV (TotK), GTX (TPHD), CMB/CSAB (OoT3D), HKX 2012 (Sonic Lost World), LZ/ARC (Skyward Sword)
- [Raw XeNTaX extraction](zelda/xentax-zelda-knowledge.md) — source threads (2013–2023)

## No Man's Sky (Hello Games Engine)
- [No Man's Sky Formats](nms/nms-formats.md) — PAK/PSAR (PC), HGPAK (PS4/Switch), MBIN/EXML data, models/skeletons, GLSL shaders, Wwise audio
- [Raw XeNTaX extraction](nms/xentax-nms-knowledge.md) — source threads (2016–2023)

## Euro Truck Simulator 2 / ATS (Prism3D)
- [ETS2/ATS Formats](ets2/ets2-formats.md) — SCS# archives, 3nK encryption, PMG/PMD models, SII config, textures, mod system
- [Raw XeNTaX extraction](ets2/xentax-ets2-knowledge.md) — source threads (2012–2023)

## Magic: The Gathering
- [MTG Formats](mtg/mtg-formats.md) — Classic .cat archives (headerless TIFF card art), MTG Arena (Unity AssetBundles, Wwise/FMOD audio)
- [Raw XeNTaX extraction](mtg/xentax-mtg-knowledge.md) — source threads (2007–2023)
