# Oblivion Remastered (2025) — Engine, Containers & Animation Reference

> **Cross-reference**: Legacy Gamebryo formats (BSA v104/v105, NIF, IMG/COL/CFA/DFA, EGM morphs, cell data, save files) → [XeNTaX Bethesda knowledge §1,3,6–8,10–11](xentax-bethesda-knowledge.md)

Oblivion Remastered (released 2025-04-22) is **not** a Havok title: it is a
dual-engine hybrid. **Unreal Engine 5.3** renders the game (models, textures,
UI, audio, animation), while the **original Gamebryo engine**
still runs core game logic (quests, AI, scripting, physics, combat). The
Fallout 4/Skyrim `.hkx` behavior-animation ecosystem documented elsewhere in
this hub does **not** apply here — this page collects verified facts about the
engine split, the UE5 Io Store containers, the legacy Gamebryo data layer, the
UE5-native animation pipeline, and the community tooling, and explicitly marks
what is not yet confirmed.

## 1. Overview: A Hybrid Engine, Not a Havok Title

- **Release**: 2025-04-22 (PS5 / Windows / Xbox Series X|S); Switch 2 port on
  2026-08-11. Development began 2021; co-developed by **Virtuos (Paris)** and
  **Bethesda Game Studios**. [HIGH — Wikipedia]
- **Renderer**: Unreal Engine 5.3 (5.3.2 per community tooling; FModel's preset
  is `GAME_UE5_3`; animation modding requires compiling UE **5.3.2** source).
  Uses UE5's Lumen dynamic lighting and ray tracing. [HIGH — multiple sources]
- **Game logic**: the original Gamebryo engine is still used to handle core
  game logic — quests, AI, scripting, physics, combat. [HIGH — UESP]
- **Consequence for modders**: there are **two asset pipelines** — UE5 `.pak`
  assets for presentation, and the legacy Gamebryo Data/plugin layer for logic.
  Original game data (ESM/BSA/NIF) was re-authored as UE5 assets for rendering;
  BSA/NIF are ignored at runtime.
- **Why the FO4 guide does not apply**: 2006 Oblivion used Gamebryo `.kf`/`.nif`
  animation (`.hkx` behavior files only arrived with Skyrim/FO4). The remaster
  ignores NIF entirely; the original animation files are not loadable. There is
  **no Havok hkx anywhere** on the UE5 side. [HIGH]
- The Gamebryo core is reported as updated to 64-bit stable. [MEDIUM — secondary
  source]

## 2. Content Containers & File Structure

### UE5 side — Io Store paks

- Install layout: `\Steam\steamapps\common\Oblivion Remastered\OblivionRemastered\Content\Paks`
  holds all `.pak` files. [HIGH]
- Main package: `OblivionRemastered-Windows.pak` — all new sounds, code, and most
  content. [HIGH]
- Container format: UE5 **Io Store** — each archive is a triple of `.pak` +
  `.utoc` + `.ucas`; mod managers must list/delete all three, and repacking
  `.utoc` requires the `retoc` tool. Assets inside are `.uasset`/`.uexp`.
  [HIGH]
- Browsing paks in FModel needs a **usmap** mappings file and the `GAME_UE5_3`
  preset. No entry exists in TheNaeem/Unreal-Mappings-Archive; the community
  standard is huwiee's usmap. [HIGH]
- Compression is **not explicitly confirmed** as Oodle; UE 5.3 Io Store defaults
  to Oodle — verify from the `.pak`/`.utoc` header or FModel. [MEDIUM]

### Legacy Gamebryo side — Data folder & plugins

- Legacy Data lives at `Content\OblivionRemastered\Content\Dev\ObvData\Data`
  (contains the ESMs/ESPs and `Plugins.txt`; the Generate Plugins.txt Loader
  regenerates `Plugins.txt` there). [MEDIUM — gist, corroborated by forum posts]
- The original ESM/ESP plugins are **byte-identical to the 2006 release**; the
  only exception is `DLCHorseArmor.esp` (one dirty message). Remaster changes
  ship via **3 new ESPs**: `AltarESPMain.esp`, `AltarDeluxe.esp`,
  `AltarESPLocal.esp`. [HIGH — UESP]
- Localization: every ESM/ESP record's Full Name was replaced by a unique **UE5
  string-table key**; unkeyed mod records show a `[NL]` prefix, unresolved keys
  show `<MISSING STRING TABLE ENTRY>`. [HIGH]
- Console: the UE5 console replaces the classic console; legacy commands need
  the `obvConsole` prefix (e.g. `obvConsole tgm`), and some Gamebryo commands
  (e.g. `tcl`) are inert. [HIGH]
- Audio: **Wwise**; sounds ship as `.wem` under `Content\WwiseAudio\Media`,
  referenced via `MediaPathName` (e.g. `Media/94211525.wem`); new sounds live
  under `OblivionRemastered/Content/WwiseAudio/Interface/...`. [HIGH]

## 3. Animation & Physics

### UE5-native animation — not Havok behavior graphs

- Animation is **UE5 native** (Animation Sequences / Skeletal Meshes). The
  workflow is: import **FBX** into a UE 5.3.2 project → asset becomes a
  `.uasset` → package into a `.pak` with Io Store. **No hkx anywhere.** [HIGH]
- Prerequisites (per the official-style animation article): ~200 GB free disk,
  Blender 4.2 LTS, UE 5.3.2 source, Visual Studio 2022 (C++ workload), the
  Oblivion Remastered sample project, and FModel. [HIGH — Nexus article 141]
- Naming/paths example: `A_Humanoid_OneHand_Sprint.fbx` lands in
  `Content/Art/Animation/Humanoid/ThirdPerson/OneHanded/Locomotion/Normal`.
  [HIGH]
- Skeleton: `SKEL_HumanoidSkeleton` — its **virtual bones are IK leg targets**;
  the humanoid rig FBX is `SK_HumanoidFull.fbx`. [HIGH]
- Lip-sync is an **entirely new system**. [HIGH]

### Physics — legacy Gamebryo era

- Physics are the **exact same as the original Oblivion**: gravity is slightly
  heavier with less ragdolling, and physics-enabled objects settle immediately
  on cell load. [HIGH — Steam discussion + UESP]
- There is no Havok on the UE5 side; the Gamebryo side retains the legacy
  (Havok-era) physics stack — but Oblivion physics were never the hkx behavior
  system anyway. [MEDIUM — inference]

## 4. Modding Tools

| Tool | Role | Source |
|------|------|--------|
| FModel | Browse/extract UE assets from paks; needs huwiee usmap + `GAME_UE5_3` | https://fmodel.app/ ; https://www.nexusmods.com/oblivionremastered/mods/47 |
| UnrealPak (FluffyQuak) | Pack/unpack `.pak` (drag folder onto `UnrealPak-With-Compression.bat` / `UnrealUnpak.bat`) | https://github.com/Dmgvol/UE_Modding/blob/main/BasicModding/UnrealPak.md |
| UE4SS for OblivionRemastered | Lua scripting loader/injector for the UE side | https://www.nexusmods.com/oblivionremastered/mods/32 |
| UE4SS TesSyncMapInjector | Syncs `.esp` form IDs with the UE runtime (no pak needed); Smart Mapper variant | https://www.nexusmods.com/oblivionremastered/mods/1272 ; /2784 |
| OBSE64 | Oblivion Remastered Script Extender (engine-level) | https://raw.githubusercontent.com/KelpyCode/oblivion-remastered-modding-wiki/main/create.md |
| xEdit → `TES4REdit.exe` | Plugin editor; rename the xEdit exe to load remaster plugins | https://forums.nexusmods.com/topic/13515989 |
| Construction Set v1.2 | Legacy plugin editing still works for the data layer | https://cs.uesp.net/wiki/The_Elder_Scrolls_Construction_Set |
| MagicLoader + MagicPatcher | New interior cells; generates `.json` config | https://www.nexusmods.com/oblivionremastered/mods/1966 ; /2906 |
| nathtest/UProjOblivionRemastered | UE 5.3.2 project derived from a UE4SS dump; blueprint-modding base (needs `ENGINE_API` fix in `InputDeviceProperties.h`) | https://github.com/nathtest/UProjOblivionRemastered |
| Blueprint Mod Loader / Simple Mod Loader | Load blueprint `.pak` mods | https://www.nexusmods.com/oblivionremastered/mods/2610 |
| retoc + uAssetGUI | Repack `.utoc`; edit `.uasset`; mods load if named alphabetically before Oblivion paks (e.g. `000_Mod.pak`) | https://www.nexusmods.com/oblivionremastered/mods/3918 |
| Plugins and Paks Manager | Plugin + pak management GUI | https://www.nexusmods.com/oblivionremastered/mods/446 |
| Generate Plugins.txt Loader | PowerShell `Plugins.txt` generator | https://www.nexusmods.com/oblivionremastered/mods/283 |
| Wwise | Convert `.wav` → `.wem` for sound mods | https://www.audiokinetic.com/en/ |
| MO2 dev build | Mod Organizer 2 build with remaster support (Discord) | https://discord.gg/ewUVAqyrQX |
| NIF→FBX converter | DaggerfallTeam (Patreon, not free) | https://www.patreon.com/DaggerfallTeam/posts/nif-to-fbx-for-128457165 |

**Mod naming rule**: pak mods go in a folder named
`OblivionRemastered-Windows_<mod>_P` and the `.pak` file must end in `_P`
for MO2/Vortex to load it. [HIGH]

## 5. Community Resources

| Resource | URL | Notes |
|----------|-----|-------|
| Nexus Mods game page | https://www.nexusmods.com/oblivionremastered | Official mod hosting |
| Oblivion Remastered Modding Wiki | https://github.com/KelpyCode/oblivion-remastered-modding-wiki (site: oblivion.wiki) | Tooling (create.md); FAQ page currently empty |
| Modding Discord | https://discord.gg/4FJEEsmDSy ; https://oblivion.wiki/discord | Guides, tech support |
| UESP | https://en.uesp.net/wiki/Oblivion:Oblivion_Remastered ; Remastered Changes ; Remastered Combat | Canonical gameplay/technical change list |
| Steam store page | https://store.steampowered.com/app/2623190/ | App ID 2623190; active discussions |
| Nexus forums tools/tutorials list | https://forums.nexusmods.com/topic/13515989 | Curated tool + tutorial index |
| Full modding guide (video) | https://www.youtube.com/watch?v=Kv-iOPHuOiE | Blender + UE5 mesh/texture pipeline |

## 6. Open Questions & Unconfirmed Details

- **Pak compression**: Oodle is likely (UE 5.3 Io Store default) but was not
  confirmed from the `.pak`/`.utoc` header — verify with FModel. [MEDIUM]
- **usmap completeness**: the community usmap (huwiee, Nexus mods/47) works with
  FModel, but no mapping exists in TheNaeem/Unreal-Mappings-Archive; coverage of
  all classes is unverified.
- **Gamebryo 64-bit update** rests on a single secondary source. [MEDIUM]
- **"No Havok on the UE5 side"** is an inference from the animation pipeline and
  container contents, not a Bethesda statement. [MEDIUM]
- The Wakui tecblog UE 5.3.2 animation-replacement guide
  (https://wakui-tecblog.chimpoy.click/modding-oblivion-remastered-in-ue-5-3-2-a-guide-to-replacing-animations/)
  could not be fetched at research time (connection error).

## 7. Sources

- [Wikipedia — The Elder Scrolls IV: Oblivion Remastered](https://en.wikipedia.org/wiki/The_Elder_Scrolls_IV:_Oblivion_Remastered)
- [UESP — Oblivion:Oblivion Remastered](https://en.uesp.net/wiki/Oblivion:Oblivion_Remastered)
- [UESP — Oblivion:Remastered Changes](https://en.uesp.net/wiki/Oblivion:Remastered_Changes) (wikitext via UESP API)
- [Nexus forums — unpacking/packing pak files and sound replacement guide](https://forums.nexusmods.com/topic/13514000-unpacking-packing-pak-files-and-sound-replacement-guide-for-oblivion-remastered/)
- [Nexus article 141 — Animating for Oblivion Remastered](https://www.nexusmods.com/oblivionremastered/articles/141)
- [Nexus mods/3989 — Humanoid Rig](https://www.nexusmods.com/oblivionremastered/mods/3989) (SK_HumanoidFull.fbx)
- [Nexus mods/880 — sample project](https://www.nexusmods.com/oblivionremastered/mods/880)
- [Steam discussion — physics comparison](https://steamcommunity.com/app/2623190/discussions/0/604154245960183490/)
- [KelpyCode/oblivion-remastered-modding-wiki](https://github.com/KelpyCode/oblivion-remastered-modding-wiki) (create.md, home.html)
- [nathtest/UProjOblivionRemastered](https://github.com/nathtest/UProjOblivionRemastered)
- [gist jonathanrbowman — Data folder layout](https://gist.github.com/jonathanrbowman/28c53c0c17cf86e95bccccc3f089671e)
- [FModel](https://fmodel.app/)
- [Dmgvol/UE_Modding — UnrealPak.md](https://github.com/Dmgvol/UE_Modding/blob/main/BasicModding/UnrealPak.md)
- [UESP Construction Set wiki](https://cs.uesp.net/wiki/The_Elder_Scrolls_Construction_Set)
- [TheNaeem/Unreal-Mappings-Archive](https://github.com/TheNaeem/Unreal-Mappings-Archive)
- [Audiokinetic Wwise](https://www.audiokinetic.com/en/)