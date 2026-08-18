# Research: Oblivion Remastered (2025) — engine, containers, animation, modding

Date: 2026-08-18
Status: research notes for a future reference guide at `reference/bethesda/`
Confidence: HIGH for engine version / container facts; MEDIUM where flagged.

> Scope note: Oblivion Remastered (Apr 22, 2025) is NOT a Havok/hkx title like FO4.
> It is a **hybrid**: Unreal Engine 5.3 renders the game; the original **Gamebryo**
> engine still runs core game logic (quests, AI, scripting, physics, combat).
> The site's FO4 guide (Havok 2014 hkx) does NOT apply to animation here.

---

## 1. Engine — dual-engine hybrid

| Fact | Detail | Source (URL) | Confidence |
|------|--------|--------------|------------|
| Renderer | Unreal Engine 5.3 (5.3.2 per community) | Wikipedia: https://en.wikipedia.org/wiki/The_Elder_Scrolls_IV:_Oblivion_Remastered ; KelpyCode wiki: https://github.com/KelpyCode/oblivion-remastered-modding-wiki ; nathtest/UProjOblivionRemastered: https://github.com/nathtest/UProjOblivionRemastered | HIGH (5 independent sources) |
| Game logic | Original Gamebryo engine ("original engine is still used to handle core game logic") | UESP Remastered Changes: https://en.uesp.net/wiki/Oblivion:Remastered_Changes | HIGH |
| UE version evidence #2 | FModel preset must be "GAME_UE5_3" | Nexus forums pak guide: https://forums.nexusmods.com/topic/13514000-unpacking-packing-pak-files-and-sound-replacement-guide-for-oblivion-remastered/ | HIGH |
| UE version evidence #3 | Animation modding requires compiling UE **5.3.2** source | Nexus article 141: https://www.nexusmods.com/oblivionremastered/articles/141 | HIGH |
| Rendering tech | UE5 Lumen dynamic lighting; ray tracing | UESP (Graphics section); Wikipedia | HIGH |
| Gamebryo core | Reported as updated to 64-bit stable | https://md-eksperiment.org/en/post/20251211-oblivion-remastered-in-2025-10-key-upgrades-patch-history-and-smart-buyers-checklist | MEDIUM (secondary source) |
| Developers | Virtuos (Paris) + Bethesda Game Studios; dev began 2021; released 2025-04-22 (PS5/Win/XSX), Switch 2 2026-08-11 | Wikipedia | HIGH |

## 2. Content containers / file structure

| Fact | Detail | Source | Confidence |
|------|--------|--------|------------|
| Install layout | `\Steam\steamapps\common\Oblivion Remastered\OblivionRemastered\Content\Paks` holds all .pak (+ .utoc/.ucas) files | Nexus forums pak guide | HIGH |
| Main pak | `OblivionRemastered-Windows.pak` — all new sounds, code, most content | Nexus forums pak guide | HIGH |
| Container format | UE5 Io Store: .pak + .utoc + .ucas (mod managers list/delete all three; retoc repacks utoc; UESP mentions NPC asset folders "in the utoc file") | Nexus mods 446 "Plugins and Paks Manager"; Nexus mods 3918 "retoc and uAssetGUI"; UESP | HIGH |
| NIF/BSA | Model+texture files loose or in BSAs are **ignored**; NIF format not compatible | UESP Remastered Changes (Technical) | HIGH |
| Gamebryo Data folder | Legacy Data lives at `Content\OblivionRemastered\Content\Dev\ObvData\Data` (contains ESMs/ESPs + Plugins.txt; script gens Plugins.txt there) | gist jonathanrbowman: https://gist.github.com/jonathanrbowman/28c53c0c17cf86e95bccccc3f089671e | MEDIUM (single gist, corroborated by forum post: "Plugins.txt in steamapps/common/Oblivion Remastered/...") |
| Plugins | Original ESM/ESP bytecode-identical to 2006 release; only `DLCHorseArmor.esp` differs (1 dirty message); remaster changes via 3 new ESPs: `AltarESPMain.esp`, `AltarDeluxe.esp`, `AltarESPLocal.esp` | UESP Remastered Changes (Technical) | HIGH |
| Localization | UE5 string tables; every ESM/ESP record's Full Name replaced by a unique string key; unkeyed mod records show `[NL]` prefix; unresolved keys show `<MISSING STRING TABLE ENTRY>` | UESP Remastered Changes (Technical) | HIGH |
| Console | UE5 console replaces classic console; legacy commands need `obvConsole` prefix (e.g. `obvConsole tgm`); some Gamebryo commands (tcl) inert | UESP Remastered Changes (Technical) | HIGH |
| Audio | Wwise; sounds as `.wem` under `Content\WwiseAudio\Media` referenced via `MediaPathName` (e.g. `Media/94211525.wem`); new sounds under `OblivionRemastered/Content/WwiseAudio/Interface/...` | Nexus forums pak guide | HIGH |
| Compression | Not explicitly confirmed as Oodle; UE 5.3 Io Store defaults to Oodle — VERIFY from .pak/.utoc header or FModel | (FModel uses game's oodle dll) | MEDIUM |
| usmap | No entry in TheNaeem/Unreal-Mappings-Archive (checked tree 2026-08-18); community uses huwiee's usmap at Nexus mods/47 | https://github.com/TheNaeem/Unreal-Mappings-Archive ; https://www.nexusmods.com/oblivionremastered/mods/47 | HIGH |

## 3. Animation & physics

| Fact | Detail | Source | Confidence |
|------|--------|--------|------------|
| Animation system | **UE5 native animation** (Animation Sequences / Skeletal Meshes). Replaced by importing FBX into a UE 5.3.2 project; no hkx anywhere | Nexus article 141 (Animating for Oblivion Remastered) | HIGH |
| Animation naming/paths | e.g. `A_Humanoid_OneHand_Sprint.fbx` in `Content/Art/Animation/Humanoid/ThirdPerson/OneHanded/Locomotion/Normal`; skeleton `SKEL_HumanoidSkeleton` (virtual bones = IK leg targets); humanoid rig FBX `SK_HumanoidFull.fbx` | Nexus article 141; Nexus mods 3989 (Humanoid Rig) | HIGH |
| Legacy note | 2006 Oblivion used Gamebryo **.kf/.nif** animation (NOT hkx — hkx came with Skyrim/FO4). Remaster ignores NIF; original animation files not loadable | UESP Technical (NIF not compatible); site context | HIGH |
| Lip-sync | "entirely new system" | UESP (Animations) | HIGH |
| Physics | Still original Gamebryo-era physics ("Physics are the exact same as OG oblivion"); UESP: gravity slightly heavier, less ragdolling; physics-enabled objects settle immediately on cell load | Steam discussion: https://steamcommunity.com/app/2623190/discussions/0/604154245960183490/ ; UESP | HIGH |
| Havok? | No Havok on the UE5 side. Gamebryo side retains the legacy (Havok-era) physics stack. Oblivion physics were never the hkx behavior system anyway | inference from above | MEDIUM |

## 4. Modding tools

| Tool | Role | Source URL |
|------|------|------------|
| FModel | browse/extract UE assets from paks; needs huwiee usmap + GAME_UE5_3 setting | https://fmodel.app/ ; https://www.nexusmods.com/oblivionremastered/mods/47 |
| UnrealPak (FluffyQuak) | pack/unpack .pak (drag folder onto UnrealPak-With-Compression.bat / UnrealUnpak.bat) | https://github.com/Dmgvol/UE_Modding/blob/main/BasicModding/UnrealPak.md |
| UE4SS for OblivionRemastered | Lua scripting loader/injector for UE side | https://www.nexusmods.com/oblivionremastered/mods/32 |
| UE4SS TesSyncMapInjector | syncs .esp form IDs with UE runtime (no pak needed); Smart Mapper variant | https://www.nexusmods.com/oblivionremastered/mods/1272 ; /2784 |
| OBSE64 | Oblivion Remastered Script Extender (engine-level) | KelpyCode wiki create.md: https://raw.githubusercontent.com/KelpyCode/oblivion-remastered-modding-wiki/main/create.md |
| xEdit → TES4REdit.exe | plugin editor; rename xEdit exe to load remaster plugins | Nexus forums topic 13515989 |
| Construction Set v1.2 | legacy plugin editing still works for data layer | https://cs.uesp.net/wiki/The_Elder_Scrolls_Construction_Set |
| MagicLoader + MagicPatcher | new interior cells; generates .json config | https://www.nexusmods.com/oblivionremastered/mods/1966 ; /2906 |
| nathtest/UProjOblivionRemastered | UE 5.3.2 project from UE4SS dump; blueprint modding base (needs ENGINE_API fix in InputDeviceProperties.h) | https://github.com/nathtest/UProjOblivionRemastered |
| Blueprint Mod Loader / Simple Mod Loader | load blueprint .pak mods | https://www.nexusmods.com/oblivionremastered/mods/2610 |
| retoc + uAssetGUI | repack utoc; edit uassets; mods load if named alphabetically before Oblivion paks (e.g. 000_Mod.pak) | https://www.nexusmods.com/oblivionremastered/mods/3918 |
| Plugins and Paks Manager | plugin + pak management GUI | https://www.nexusmods.com/oblivionremastered/mods/446 |
| Generate Plugins.txt Loader | PowerShell Plugins.txt generator | https://www.nexusmods.com/oblivionremastered/mods/283 |
| Wwise | convert .wav → .wem for sound mods | https://www.audiokinetic.com/en/ |
| MO2 dev build | supports remaster (Discord) | https://discord.gg/ewUVAqyrQX |
| NIF→FBX converter | DaggerfallTeam (Patreon, not free) | https://www.patreon.com/DaggerfallTeam/posts/nif-to-fbx-for-128457165 |
| Mod naming rule | folder `OblivionRemastered-Windows_<mod>_P`; .pak must end `_P` for MO2/Vortex | Nexus forums pak guide |

## 5. Community resources

| Resource | URL | Notes |
|----------|-----|-------|
| Nexus Mods game page | https://www.nexusmods.com/oblivionremastered | official mod hosting |
| Oblivion Remastered Modding Wiki | https://github.com/KelpyCode/oblivion-remastered-modding-wiki (site: oblivion.kelp.li / oblivion.wiki) | tools (create.md), faq (empty) |
| Oblivion Remastered Modding Discord | https://discord.gg/4FJEEsmDSy ; https://oblivion.wiki/discord | guides, tech support |
| UESP | https://en.uesp.net/wiki/Oblivion:Oblivion_Remastered ; Remastered Changes ; Remastered Combat | canonical gameplay/tech change list |
| Steam | App ID 2623190 — https://store.steampowered.com/app/2623190/ | discussions active |
| Nexus forums tools/tutorials list | https://forums.nexusmods.com/topic/13515989-list-of-tools-and-tutorials-for-making-oblivion-remastered-mods/ | curated tool+tutorial index |
| Full modding guide (video) | https://www.youtube.com/watch?v=Kv-iOPHuOiE | Blender+UE5 mesh/texture pipeline |
| Wakui tecblog animation guide | https://wakui-tecblog.chimpoy.click/modding-oblivion-remastered-in-ue-5-3-2-a-guide-to-replacing-animations/ | UE 5.3.2 animation replacement (could NOT fetch — connection error) |

## 6. URLs that could NOT be accessed (user may want to paste content)

1. https://wakui-tecblog.chimpoy.click/modding-oblivion-remastered-in-ue-5-3-2-a-guide-to-replacing-animations/ — connection error (animation replacement guide; title confirms UE 5.3.2)
2. https://www.nexusmods.com/oblivionremastered/mods/47 — page shell fetched but description/screenshots (FModel settings) not rendered
3. https://modding-on-the-spectrum.com/viewtopic.php?t=282 — content not rendered (JS-driven)
4. https://en.uesp.net/wiki/Oblivion:Remastered_Changes — HTML shell empty; wikitext retrieved via API `https://en.uesp.net/w/api.php?action=parse&page=Oblivion:Remastered_Changes&prop=wikitext&format=json`

## 7. Guide-writing pointers (for later)

- Emphasize the hybrid architecture FIRST — it changes every expectation (two asset pipelines: UE5 paks + legacy Gamebryo Data/plugins).
- "Data conversion": original game data (esm/bsa/meshes/nif) was re-authored as UE5 assets for rendering; the legacy Data folder survives for logic; BSA/NIF ignored at runtime.
- Animation section: UE5 skeletal animation pipeline (FBX → uasset → pak with Io Store), NOT Havok behavior graphs. Contrast with FO4 guide.
- Physics section: legacy Gamebryo/Havok-era physics untouched; ragdoll unchanged in character.
- File formats to document: .pak/.utoc/.ucas (Io Store), .usmap, .uasset/.uexp, .wem (Wwise), .esp/.esm (legacy, byte-identical), Plugins.txt.