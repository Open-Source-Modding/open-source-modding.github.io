# Project Context

## Environment
- Repo A (this): /home/selene/Documents/Code/web/open-source-modding.github.io — static GitHub Pages RE hub, Jekyll theme cayman, no build/CI. README.md = homepage; reference/index.md + watch_dogs/index.md = hubs; .opencode/ untracked.
- Repo B (related): /home/selene/Documents/Code/game-tools/blender-io-xbg → renamed blender-io-disrupt, remote git@github.com:Open-Source-Modding/blender-io-disrupt.git. Blender 5.0+ addon importing/editing/re-exporting Ubisoft Disrupt engine models (WD1 .xbg, WD2 .glm, WDL .xbg), HKX collision support, .mab animations. Has own AGENTS.md + .opencode/ (archive/docs/plugins/context.md/status.md/todo.md/work-log.md). RE/ notes moved to open-source-modding.github.io/reference/watch_dogs/. Its modules/Havok/ has hkx_format.py (OldPackfileParser + Tag0Parser with TAG0/SDKV/DATA/TYPE/INDX/ITEM/PTCH chunks!), decompress_compressed_mesh.py (hkcdStaticMeshTree decoder, Havok 2018 getType: TRIANGLE b≠d,c==d / QUAD / INVALID 0xdeaddead skip), injectors.

## Mission (complete): Skyrim, Oblivion Remastered, Starfield guides in reference/bethesda/
- **STATUS: COMPLETE.** Todo 9/9 [x] (this repo), sync-issues clean, all Reviewer-verified, zero invented facts (traced to .opencode/docs/*.md).

## Current Status
### Deliverables (all done, uncommitted)
- reference/bethesda/skyrim-havok-guide.md (~161 lines): Havok 2010.2.0 SDK, hkxcmd pipeline, packfile v8 SE/v11 FO4, LE 32/SE 64-bit
- reference/bethesda/oblivion-remastered-guide.md (~164 lines): UE5.3+Gamebryo hybrid, Io Store pak/utoc/ucas, Altar ESPs, obvConsole, UE string-table loc, Wwise .wem, _P rule, tools, confidence markers
- reference/bethesda/starfield-guide.md (253 lines): AnimGraph-over-Havok confirmed lineage, **MAJOR FINDING 20190200**, BA2 spec, .mesh vs .nif, master limits, SFSE, mesh/collision authoring (3ds Max 2021 + BGS ArtTools + BSToolSettings.ini + Havok Content Tools rigid bodies + CollisionGroup + MetalHollow/WoodHeavy materials + F4 preview), StarfieldMeshConverter (Blender 3.5/3.6, Nexus 4360)
- Hub sync: reference/index.md 4 Bethesda entries; README "## [Bethesda Engine References](reference/index.md)" + stale hashing link fixed
- AGENTS.md (this repo): structure section updated — lists all 4 bethesda/ guides + records Starfield Havok 2019.02 chunked-packfile fact
- reference/havok/hkx_format.md: scope note now has "Confirmed users of chunked format: Starfield (CE2) Havok 2019.02 sdk_version 20190200, ChunkType/PTCH/indexed blocks" + TODO for chunked layout section

### MAJOR FINDING (user, 2026-08-18) — recorded across 3 files
Starfield = Havok **2019.02** (sdk_version "20190200"), NEW chunked packfile format (FourCC DATA/SDKV/TYPE/INDX, ChunkType, PTCH patches, indexed blocks) — NOT FO4-style packfile (0x57e0e057, v11). Answers the open "biggest gap". FO4-era tools (hkxcmd/hkxpack) can't read it as-is. starfield-guide.md animation section corrected (removed wrong "same magic as FO4" claim), Open Questions updated (SDK version struck ANSWERED; remaining: chunk table/PTCH semantics/indexed-block addressing, 2019-era class drift).

### User's new question (PENDING ANSWER)
User ran `ls .opencode/` in blender-io-disrupt repo B and asked "shouldn't some of this be here?" (with stray "\addpart" keystroke). Disrupt repo has its own .opencode/docs/ (Havok software wiki, havok-2013-v10r1, zenhax/xentax forum pages) + docs/compressed_mesh_format.md + AGENTS.md detailing Havok module (Tag0Parser already handles TAG0/SDKV/DATA/TYPE/INDX/ITEM/PTCH chunks — WD1 uses TAG0 format!). Question likely = should the Starfield 2019.02 chunked-packfile finding / website RE notes live in the Disrupt repo too, OR should Disrupt's Havok knowledge (TAG0 chunk parser) be cross-referenced in the website's hkx_format.md? NOTE: Disrupt WD1 .hkx uses TAG0 format (2012-era), Starfield uses 2019.02 chunked — related but distinct. Need to answer user.

## Pending Tasks
1. Answer user's "shouldn't some of this be here?" — clarify relationship: website = RE reference hub (cross-game, incl. Starfield 2019.02 finding — already recorded there); blender-io-disrupt = WD-specific tooling with own .opencode/. Optionally: cross-link Disrupt's Tag0Parser knowledge into website hkx_format.md (it documents TAG0 chunks which overlap chunked format), and/or note Starfield finding in Disrupt repo context. ASK/confirm user intent before moving content.
2. Optional: commit website changes (user hasn't asked).

## Key Files (this repo)
- reference/bethesda/{skyrim-havok-guide,oblivion-remastered-guide,starfield-guide}.md (done)
- reference/havok/hkx_format.md (chunked-format note added)
- AGENTS.md (structure updated)
- .opencode/todo.md 9/9 [x]; .opencode/sync-issues.md clean
- reference/index.md, README.md (hubs synced)
- .opencode/docs/starfield.md etc. — research sources of truth

## Notes
- Working tree also has user's own prior changes (reference/watch_dogs/* new docs, reference/havok/, reference/hashing.md deleted) — untouched
- All changes uncommitted; LSP has EACCES env issue (N/A for markdown)
- Keep outputs terse; anomaly system fires on long outputs regardless of correctness