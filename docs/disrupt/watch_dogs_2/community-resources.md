# Watch Dogs 2 — Community Tools & References

Curated community findings and modding references. Sources: Discord logs, AGENTS.md research notes.

## Gun Sounds (Mel Breezy, Pesky Fly)

Gun sounds are NOT in patch, patch2, common, or sound.dat. They live in `san_francisco` archive in bnk format.

**Mod workflow**: Open `weapons.lib` → find weapon sound IDs → load bnk with FusionTools → export hidrc → find sound IDs (wem/bnk).

## Custom Music Radio (Nevon)

Custom radio stations are possible but need native audio integration. Nevon's approach:
1. Swap OverrideBus to native one with FusionTools
2. Import 6 hierarchies from donor `.bnk` (4 attenuation, 2 settings props, actor mixer)
3. Now has proper ducking with dialogues
4. Switch & Layer containers, at least 1 more SoundSFX, 4 FXCustom and FXShareSet

Tutorial: https://steamcommunity.com/sharedfiles/filedetails/?id=3448173007 (uses Wwise 2015.1.9 — higher versions cause silence).

## Police Mod (Yorpie)

Nearly complete FBI faction overhaul. Key findings:
- **DRL vs flasher conflict**: Setting lights as flashers stops them working as DRLs
- **Pursuit difficulty**: Only easy and medium used in free roam
- **Elite/Mayhem**: Max 4 enforcers spawn, unlimited in pursuits/gangs/ops

## Unused Vehicles (Pesky Fly)

Unused or barely used in traffic:
- `Agile.04.04_Reward.ForCarOnDemand`
- `Agile.04.Mint`
- `Air.Helicopter_01` (civilian)
- `Bike.01.01_AuntiShyBoys`
- `Bicycle.Bicycle_02` (electric — added to university area)
- Convoy (completely unused before Yorpie added it)
- Magnate, Blume/Proviblue armored trucks

## Model Scaling (Pesky Fly)

`vectorNonUniformScale` on `graphickit_parts`: `(height, width_side, width_front)`
- `5, 1, 1` — super tall
- `1, 5, 1` — super wide from side (back to front)
- `1, 1, 5` — super wide from front (left to right)

## Reticle Fix

`uihudelementcontextsdbobject.lib` controls reticle visibility. Delete from patch3 to get reticle back when unarmed.

## .rml Files

Have an additional header before the NBCF format — must be removed before ConvertBinaryObject can open them. Content is often gibberish anyway.

## Cut Content

- Blume Security truck model is **WD1, not WD2** (commonly confused)
- "The Future Is Blume" mission has two guard models (normal + fat) — possibly removed
- Loot truck event spawn pool can't be randomized between Proviblue/Greystrom/Blume
- **S01_M2_CA_Park_n_Play_Intro_TS**: Cut intro dialogue — extended Wrench/Sitara intro
- **S01_M2_CE_MP1_HackerSpotted_TS**: Unused dialogue
- **S01_M2_NewClothes**: Cutscene with Sitara commenting on Marcus's new outfit
- **Post-white-room cutscene**: Josh telling a joke, was to play after tutorials
- **Prime Eight Raid** (`TeamSpeakPrime8`): Cut UI section with Prime Eight channel controls
- **s03** = Cyberdriver DLC, **s05** = not in first-release base files, **s06** = DedSec XXX (not in leak)
- First patch.dat/patch.fat preserved by SlyCooperReloadCoded (119 MB) — Horatio dialogue changes visible

## WatchBox (Cut Scene Authoring System)

In the leak at `data/WatchBox/`. Cutscene/sequence authoring and remote playback system — NOT a level editor.
- 284 scene files for `san_francisco` (every mission cutscene s01–s18, drone/street/yacht races, E3 demos, turntables)
- 59 in `gym_mabelleau` (greenscreens, PGV/SOI/WS pre-renders, foreshadowing)
- Connects to running game via TCP (localhost:7913, PS4, X1, Xbox 360)
- Plans orchestrate event chains: SetEnvironment → SetScriptedTimeOfDay → TeleportPlayer → LoadMissionLayer → DominoStartSequence → WaitForQualitiesLoaded
- Video capture pipeline: screenshots → ffmpeg (libx264, 1280×720, CRF 20) → .mp4
- 123 thumbnail JPGs, 34 tags (S01–S18, Cinematics, E3, CUT, FPScenes, GreenScreens, Stolen Footage, The-Wrench, Turntable), 21 markers, 46 events

## Album Art & UI Texture Paths (Cali Sav)

- Album art: `dlc_ultra_textures\common\ui\fire\sources\textures\ingame\apps\mediaplayer\albumart` — 140×140 DXT5 `.xbt`
- Car on demand decals: `common\ui\fire\sources\textures\ingame\apps\carondemand\sourcepattern`
- Weapon paintjobs: `common\ui\fire\sources\textures\ingame\shops\menu_weapons\weapon_paintjobs`
- 3D Printer weapon icons: `common\ui\fire\sources\textures\ingame\shops\menu_weapons`
- RC drone paintjobs: `...menu_weapons\rc_drone_paintjobs`
- RC jumper paintjobs: `...menu_weapons\rc_jumper_paintjobs`
- Map icons: `common\ui\textures\icons`
- Weapon decal texture maps: `installpackage\graphics\weapons\weapon_decal`

## Radio Station Hashes (exp0t/yotta_imoto)

Full JSON at [exp0t/wd2-radio-hashes](https://github.com/exp0t/wd2-radio-hashes) (77 songs, 5 stations). Schema: `{artist_id, artist_name, song_id, song_title, song_hash}` (decimal FNV1a32).

| Station | Songs | Genre |
|---------|-------|-------|
| Bay City Pop KBY-FM | 15 | Pop |
| Rock The Bridge K66-FM | 18 | Rock |
| DedSec Pirate Radio | 17 | Electronic/Hip-Hop |
| Radio Bay Nation KBNT | 19 | Country |
| Las Ondas De La Ria KODR | 8 | Latin |

## Spawn Distance

Spawned archetypes despawn when player is ~250 meters away.

## WD2 Audio File Conversion

WD2 audio packages contain `.Unknown` files that are actually WEM (Ogg Vorbis) audio.

### Conversion Pipeline

1. **Unpack** audio package: `WD2Extract.exe` on `sound.dat`/`sound.fat`
2. **Rename**: `.Unknown` → `.wem` (they are WEM files with wrong extension)
3. **Convert**: Drag `.wem` files into `UnktoWem.bat` → `.wem` files with correct headers
4. **Play/Extract**: Use wem→ogg conversion tool to get playable `.ogg` audio

### Notes

- The `.Unknown` extension is a quirk of the WD2 extraction tools — the underlying format is standard WEM (Ogg Vorbis).
- Multiple audio packages may exist (`sound.dat`, `sound_english.dat`, etc.).

## Related

- [blender-io-disrupt addon](../blender-addon.md) — Blender 5.0 add-on: import/export/inject XBG/GLM models, HKX collision, .material.bin
- [Archive Load Priority](../watch_dogs/archive-priorities.md) — per-game archive order
- [Hashing (CRC32 / CRC64_WD2)](../watch_dogs/hashing.md) — hash algorithms
- [Vehicle Handling](vehicle-handling-modding.md) — DriveConverter, .handling.bin ↔ XML
- [Depload Format](../depload-format.md) — resource dependency/preload tables
