# Research: Skyrim Havok / Animation Reference

Date: 2026-08-18
Purpose: fact base for a Skyrim Havok/animation reference guide at
`reference/bethesda/` (alongside `fallout4-havok-guide.md` and
`reference/havok/hkx_format.md`).
Scope: animation/Havok focus; BSA/BA2, .pex, and plugin formats covered briefly.

---

## 1. Havok version(s) — LE vs SE vs AE

### Skyrim Legendary Edition (LE, 2011)
- Uses the **Havok 2010.2.0 SDK**; contents version string `hk_2010.2.0-r1`.
- Source: figment/hkxcmd README (only converts "native HKX files built with
  Havok 2010.2.0 SDK") — https://github.com/figment/hkxcmd
- Source: Dexesttp/hkxpack README ("support for hk_2010.2.0-r1 or other hkx
  formats may be added"; target format is hk_2014.1.0-r1 = FO4) —
  https://github.com/Dexesttp/hkxpack
- LE packfiles are **32-bit** (x86), little-endian.
- Tooling for LE (hkxcmd) required the Intel "Havok PcXs Content Tools
  2010.2.0" / Preview Tool.

### Skyrim Special Edition (SE, 2016) and Anniversary Edition (AE, 2021)
- SE packfile **version 8** (per existing repo doc
  `reference/havok/hkx_format.md`, line 33: `version = 11 for FO4, 8 for
  Skyrim SE`), **64-bit** pointers, little-endian. This is the pre-2015 "old"
  packfile layout shared with FO4 (which uses version 11), NOT the newer
  FourCC-chunked container.
- SE/AE behavior/animation `.hkx` are 64-bit; LE ones are 32-bit — a key
  LE→SE porting difference. (Implied by hkx_format.md header pointer-size note:
  `bytes_in_pointer = 8` for 64-bit, `4` for x86 packfiles.)
- SE introduced **BSA v105** (LZ4 compression) vs LE's **BSA v104** (zlib) —
  see section 3.
- AE is the 2021 re-release (update 1.6+, current 1.6.1170); it keeps SE's
  Havok/packfile layout. ESL plugins introduced in SE update 1.5 (see §3).

### Comparison: Fallout 4 (for reference)
- FO4 uses **Havok 2014.1.0**, contents `hk_2014.1.0-r1`, packfile **version
  11**, 64-bit — see `reference/havok/hkx_format.md`.
- So Skyrim (both LE and SE) predates FO4's Havok version; class layouts and
  packfile version differ (8 vs 11).

### Gotcha from hkxcmd README
- The standard Havok SDK (2010.2) ships **Physics + Animation** support only.
  Bethesda also uses **Havok Behavior** classes, which the stock SDK cannot
  read — those files must be handled by the Behavior Tool / community
  reflection tools, not the plain SDK converters.

---

## 2. Animation files & workflow

### File types (all are Havok `.hkx` packfiles unless noted)
- **Skeleton**: `meshes\actors\<race>\character assets\skeleton.nif`
  (NetImmerse) plus a matching **skeleton.hkx** (hkaSkeleton). The .nif holds
  the bone nodes; the .hkx specifies which nodes are bones and the bind pose.
- **Animation**: individual `.hkx` (hkaAnimationBinding +
  hkaInterleavedUncompressedAnimation / spline-compressed variants).
- **Behavior**: behavior-project files (hkbBehaviorGraph /
  hkbBehaviorProjectData), one per actor/race in `meshes\actors\<race>\`
  (e.g. `0_master.hkx`).
- **Project file**: the top-level behavior project `.hkx` referencing the
  skeleton, behavior file, and animations (hkbBehaviorProjectData) — the FO4
  guide documents the same structure (project/behavior/skeleton/animation).

### Behavior project workflow (Havok Content Tools / Behavior Tool)
- Authored in the **Havok Behavior Tool** (Intel "Havok Content Tools 2010.2"),
  the same tool family as FO4's (which uses Havok 2014's Behavior Tool).
- Workflow (shared with FO4, per fallout4-havok-guide.md): create a project
  file that references a rig (skeleton, `rigName` → `.hkt` tagfile), a behavior
  file (`behaviorFilename` → `.hkx`), and animation folders; pack loose files
  into `.hkx`.
- The FO4 guide's structure (GenericBehaviors / Behaviors / CharacterAssets
  folders; `.hkt` intermediate tags; `#NN` object references) mirrors Skyrim's.
- Pack/unpack via **hkxcmd** (convert HKX↔XML, KF↔HKX) and the **Havok Preview
  Tool** for previewing animations on a skeleton.

### hkxcmd workflow (LE; from figment/hkxcmd README)
- `hkxcmd convert <dir>` — packed HKX → XML HKX (loadable in Preview Tool).
- `hkxcmd exportkf <dir>` — HKX → Gamebryo `.kf` (for 3ds Max via niftools
  importer).
- `hkxcmd convertkf <skeleton.hkx> <anim.kf> <out.hkx>` — KF → HKX
  (`-v:XML` for XML output).
- `hkxcmd DumpText` — dumps skeleton bone list.
- Known limits: no non-uniform scale in KF; no visibility/camera tracks;
  1-to-1 bone→track assumption.

### Ragdoll
- Ragdolls/physics (hkbRagdollDriver, constraint setups) live in the behavior
  project; physics packfiles use Havok's Physics module (2010.2-era classes).
  (Note: ragdoll specifics not deeply verified this pass — needs a dedicated
  research round if the guide covers ragdoll internals.)

---

## 3. File containers (brief)

### BSA / BA2
- **BSA** is Skyrim's archive (BSA, ESM/ESP, and ESS are IFF variants).
- LE **v104 (0x68)**, compression **zlib**; SE **v105 (0x69)**, compression
  **LZ4**. Source: UESP "Skyrim Mod:Archive File Format".
- Header: `"BSA\0"`, version, offset, archiveFlags (bit 1 include dir names,
  bit 2 include file names, bit 3 compressed archive, bit 7 Xbox big-endian,
  bit 9 embed file names...), folderCount, fileCount, total name lengths,
  fileFlags (mesh/texture/.../misc type bits), padding.
- Folder record: nameHash, count, offset (+v105-only padding fields).
- File record: nameHash, size (bit 30 = per-file compression toggle), offset.
- Names sorted by 64-bit hash.
- Tools: **BSAOpt**, **B.A.E (Bethesda Archive Extractor)** (SE Nexus mod 974,
  also handles BA2/Starfield), FO3 Archive Utility, FOMM/OBMM unpackers.
- BA2 is FO4's format (not Skyrim); SE uses BSA.

### .pex (compiled Papyrus scripts)
- Magic **0xFA57C0DE**, **big-endian** regardless of platform, versions
  **3.0–3.1** (Dawnguard/Hearthfire/Dragonborn scripts = 3.1).
- Header: magic, majorVersion, minorVersion, gameId, compilationTime,
  sourceFileName, username, machinename, then string table, debug info,
  user flags, objects.
- Objects contain variables/properties/states/functions; bytecode opcodes
  (00 nop … 23 array_rfindelement). Source: UESP "Skyrim Mod:Compiled Script
  File Format".
- .psc (source) is compiled to .pex by the Papyrus compiler (in the Creation
  Kit); decompilers exist (Caprica/Champollion, TESVTranslator, xTranslator).

### Plugin format (ESM / ESP / ESL)
- Plugins are record/group/field files: a TES4 header record + top-level
  **GRUP** groups containing records (form IDs), each record has fields.
- **ESM** master (pinned top of load order), **ESP** plugin, **ESL** light
  master (up to **4096** plugins in load order; up to **4096** form-ID records
  — 2048 before SE patch **1.6.1130**, which reserved 0x0000–0x07FF).
- ESL introduced with Creation Club/"Creations" in **SE update 1.5**; works in
  **AE**, not in LE/pre-1.5 SE.
- Plain ESP references are treated as permanent (all loaded, count toward the
  reference cap) — prefer ESL/ESM.
- Group types 0–9 (top/type, world children, interior/exterior cell blocks,
  cell children, topic children, persistent/temporary children).
- Source: UESP "Skyrim Mod:Mod File Format".

---

## 4. Modding tools

| Tool | What it does | URL |
|------|--------------|-----|
| **hkxcmd** (figment) | Havok CLI: HKX↔XML, HKX↔KF, skeleton dump; LE 2010.2 | https://github.com/figment/hkxcmd |
| **NifSkope** (niftools) | .nif (NetImmerse) viewer/editor; Skyrim/FO | https://github.com/niftools/nifskope |
| **hkxpack** (Dexesttp) | pack/unpack HKX↔TagXML (FO4 2014.1 target; 2010.2 noted) | https://github.com/Dexesttp/hkxpack |
| **hkxanim** (Dexesttp) | FBX→HKX animation creator (FO4 focus) | https://github.com/Dexesttp/hkxanim |
| **Havok Content Tools / Behavior Tool / Preview Tool** (Intel 2010.2) | author/preview behaviors & animations | (Intel Havok site; 2010.2.0 build) |
| **FNIS** (Fore) | "Fores New Idles" animation/idle patcher | Skyrim Nexus mod 11811 |
| **Nemesis** (Project New Reign) | SE/AE behavior/animation engine patcher | SE Nexus mod 60033 |
| **SKSE** (Skyrim Script Extender) | DLL script extender; needed by most mods | https://skse.silverlock.org/ |
| **B.A.E** (Bethesda Archive Extractor) | unpack BSA/BA2 | SE Nexus mod 974 |
| **TES5Edit / xEdit** | plugin editing/cleaning | Nexus / github (zilav) |
| **Wrye Bash** | mod install/management, bashed patch | Skyrim Nexus mod 1840 |
| **PyNifly** (BadDogSkyrim) | Blender addon: Skyrim/FO models+animations | https://github.com/BadDogSkyrim/PyNifly |
| **hkxparse** (exyorha) | C++ Havok packfile/tagfile parser; **2010 only** | https://github.com/exyorha/hkxparse |

SKSE builds (from skse.silverlock.org, verified 2026-08-14):
- Classic (LE): 1.7.3
- SE: 2.0.20 (game 1.5.97)
- **AE: 2.2.6 (game 1.6.1170)**; GOG AE 2.2.6 (1.6.1179)
- VR: 2.0.12 (1.4.15)
- No support for Windows Store/Game Pass or Epic releases.

---

## 5. Community documentation URLs (collected)

Verified fetched (content confirmed):
- UESP Skyrim Mod:Archive File Format
  https://en.uesp.net/w/index.php?title=Skyrim_Mod:Archive_File_Format&action=raw
- UESP Skyrim Mod:Mod File Format
  https://en.uesp.net/w/index.php?title=Skyrim_Mod:Mod_File_Format&action=raw
- UESP Skyrim Mod:Compiled Script File Format
  https://en.uesp.net/w/index.php?title=Skyrim_Mod:Compiled_Script_File_Format&action=raw
- UESP Skyrim Mod:Modding (notes: "much ... is very outdated")
  https://en.uesp.net/w/index.php?title=Skyrim_Mod:Modding&action=raw
- UESP Skyrim Mod:File Formats (index)
  https://en.uesp.net/w/index.php?title=Skyrim_Mod:File_Formats&action=raw
- figment/hkxcmd README: https://github.com/figment/hkxcmd (raw README.txt)
- figment/hkxcmd issues: https://github.com/figment/hkxcmd/issues
- niftools/nifskope: https://github.com/niftools/nifskope
- Dexesttp/hkxpack: https://github.com/Dexesttp/hkxpack
- Dexesttp/hkxanim: https://github.com/Dexesttp/hkxanim
- SKSE: https://skse.silverlock.org/
- Nemesis: https://www.nexusmods.com/skyrimspecialedition/mods/60033
- FNIS: https://www.nexusmods.com/skyrim/mods/11811
- B.A.E: https://www.nexusmods.com/skyrimspecialedition/mods/974
- exyorha/hkxparse: https://github.com/exyorha/hkxparse (from existing havok-repos.md)

Could NOT access (blocked/empty — user may want to paste content):
- **ck.uesp.net** (Creation Kit wiki, UESP-hosted) — **HTTP 403 Forbidden** on
  multiple pages (Animation_Files, Havok_Behavior_Tool, BSA, Mod_File_Format).
  This is the main lost source; it has the Skyrim behavior-project/animation
  docs.
- **www.creationkit.com** — currently "down for backend maintenance" (the
  Havok_Behavior_Tool page returns a maintenance page). Try again later or use
  a web-archive snapshot:
  https://web.archive.org/web/*/www.creationkit.com/index.php?title=Havok_Behavior_Tool
- UESP raw pages return empty body via the normal `.wiki/` URL (worked via
  `/w/index.php?title=...&action=raw`). Some guessed page names 404
  (Animation_Files, Animation_File_Format, Papyrus, Plugin_File_Format,
  BSA_File_Format).

---

## Notes / confidence

- HIGH (verified official/primary): SKSE builds; hkxcmd README (Havok 2010.2.0
  for LE); hkxpack/hkxanim; niftools/nifskope; UESP archive/plugin/pex formats;
  Nemesis/FNIS/B.A.E URLs; existing repo hkx_format.md (SE packfile version 8).
- MEDIUM (inference from adjacent sources): SE 64-bit vs LE 32-bit behavior
  .hkx; AE keeping SE's packfile layout (implied by SE→AE continuity, not a
  single authoritative statement).
- The Skyrim behavior-project internals (hkbBehaviorProjectData structure,
  Behavior Tool authoring specifics) are NOT confirmed this pass — ck.uesp.net
  was blocked and creationkit.com is down. Recommend a follow-up fetch of a
  web-archive snapshot of ck.uesp.net/wiki/Havok_Behavior_Tool and
  ck.uesp.net/wiki/Animation_Files before writing that section of the guide.
