# Cyberpunk 2077 Vehicle Sound Mods

> **Source**: "Custom engine sounds pack" (Nexus mod 17888), file `tweak code/engine_sounds_codes.tweak`, cross-checked against the per-car tweaks `r6/tweaks/krnlnik/ford_mustang_boss_302.tweak` (Mustang BOSS 302 Sound) and `r6/tweaks/cornmilf/jeep_grand_cherokee_trackhawk.tweak` (Jeep Grand Cherokee Trackhawk Sound).

Vehicle engine audio in Cyberpunk 2077 is wired at the `.tweak` level: a vehicle record names one Wwise audio resource for the player car and a second one for traffic copies of the same model. This page covers that wiring, the resource naming convention, and the mod folder layout that ships it.

The `.archive` container and the Wwise bank internals are out of scope here; see [Cyberpunk 2077 formats](cyberpunk2077-formats.md) for the RDAR+KARK archive structure and the `.wem` / `.opuspak` / `.bnk` audio section.

## Where vehicle audio is declared

Vehicle audio lives in a `.tweak` file opened with the `Vehicle` package. The likely imports are the standard vehicle set:

```ini
package Vehicle
using RTDB, BaseStats, Driving
```

Inside the package a car block sets two audio resource fields:

```ini
CarName {
    player_audio_resource = "v_car_bmw_m3_gtr";
    traffic_audio_resource = "v_car_bmw_m3_gtr_traffic";
}
```

The sound pack ships exactly this shape: one small block per car, each block carrying only the two audio fields. In a full per-car mod the same two fields sit inside a real vehicle record, alongside the model, handling and UI fields rather than in a standalone stub:

```ini
<car_id> : Vehicle_4w_Default {
    ...
    player_audio_resource = "v_car_mustang_boss_302";
    traffic_audio_resource = "v_car_mustang_boss_302_traffic";
    ...
}
```

The surrounding record holds `entityTemplatePath`, `appearanceName`, `displayName`, `manufacturer`, a `vehicleUIData` block (`productionYear`, `driveLayout`, `horsepower`, `mass`, `info`), a `fk<UIIcon> icon`, the camber fields, `model`, `tags`, `fk<VehicleDataPackage> vehDataPackage`, the destruction block, `vehWheelDimensionsSetup`, `vehDriveModelData`, `vehEngineData`, `tppCameraPresets`, `drivingParamsPanic`, `trafficSuspension` and `savable`. The tweak ends by registering the record:

```ini
vehicle_list {
    list += ["Vehicle.<car_id>"];
}
```

A sound swap on an existing car only needs the two audio lines; the full record is what a mod that adds or redefines the vehicle carries.

## Resource naming convention

The two fields follow a fixed pattern:

- Player-driven vehicle: `v_car_<name>`
- Traffic (NPC) copy: `v_car_<name>_traffic`

`<name>` is derived from the car's internal identity and does not always match the marketing name. Two traps in the pack are worth memorising: the Mercedes-Benz SL 65 AMG uses `v_car_cl_65_amg`, and the Jeep Grand Cherokee SRT8 uses `v_car_cherokee_srt8`. Always read the exact string from a working tweak rather than inferring it from the display name.

## Both resources must be set

The engine reads the two fields as separate streams. The car the player drives resolves `player_audio_resource`; traffic instances of the same model that the game spawns resolve `traffic_audio_resource`. Set both. A mod that edits only `player_audio_resource` leaves every NPC copy of that car on the stock bank, so the new sound is audible only from the driver's seat and only for the player's own vehicle.

## Deployment path for a RedMod-style mod

A per-car sound mod is deployed as loose files in the game's mod tree, mirroring the two halves of the change:

```
base/sound/soundbanks/<car>.bnk        # the Wwise SoundBank asset
r6/tweaks/<author>/<car>.tweak         # the Vehicle package override
```

The `r6/tweaks/<author>/<car>.tweak` path is the same override route used by any other vehicle tweak, where `<author>` is the mod author's folder. The two known examples are `r6/tweaks/krnlnik/ford_mustang_boss_302.tweak` and `r6/tweaks/cornmilf/jeep_grand_cherokee_trackhawk.tweak`. Keeping the `.tweak` under `r6/` and the bank under `base/` means a mod manager or manual install can drop both without repacking an archive.

## Audio assets: the `base/sound` layout

The sound asset itself is a Wwise SoundBank placed under `base/sound/soundbanks/`. The pack uses one bank per car, named after the same `<name>` stem as the resource:

```
base/sound/soundbanks/350z.bnk
base/sound/soundbanks/911_gt2.bnk
base/sound/soundbanks/bmw_m3_gtr.bnk
base/sound/soundbanks/charger_srt8.bnk
base/sound/soundbanks/cherokee_srt8.bnk
base/sound/soundbanks/cl_65_amg.bnk
base/sound/soundbanks/mustang_boss_302.bnk
base/sound/soundbanks/nissan_gt_r_34.bnk
base/sound/soundbanks/subaru_wrx.bnk
```

Each `.bnk` is a little-endian Wwise SoundBank, version 140. The tweak's resource string is what the engine uses to find the bank's contents; the `.bnk` file name is a packaging convention, not part of resource resolution.

## The InfiniteWave archive

The pack also ships `InifiniteWave_for_modders.archive` (note the misspelling, "InifiniteWave"), a packaged form of the sound bank that modders can drop in directly, alongside an extracted `InfiniteWave/archive/pc/mod/#InfiniteWave.sc.archive` tree. The `.archive` is just the CP2077 RDAR+KARK container holding the same audio assets in packed form. For what the container and its compressed entries actually look like, follow the archive section of [Cyberpunk 2077 formats](cyberpunk2077-formats.md); this page stops at the tweak-level wiring that points the game at the assets.

## Nine-car resource table

The sound pack covers nine cars. The player and traffic resource strings are reproduced verbatim from `engine_sounds_codes.tweak`:

| Car | `player_audio_resource` | `traffic_audio_resource` |
|-----|-------------------------|--------------------------|
| BMW M3 GTR | `v_car_bmw_m3_gtr` | `v_car_bmw_m3_gtr_traffic` |
| Dodge Charger SRT8 | `v_car_charger_srt8` | `v_car_charger_srt8_traffic` |
| Ford Mustang Boss 302 | `v_car_mustang_boss_302` | `v_car_mustang_boss_302_traffic` |
| Jeep Grand Cherokee SRT8 | `v_car_cherokee_srt8` | `v_car_cherokee_srt8_traffic` |
| Mercedes-Benz SL 65 AMG | `v_car_cl_65_amg` | `v_car_cl_65_amg_traffic` |
| Nissan 350Z | `v_car_nissan_350z` | `v_car_nissan_350z_traffic` |
| Nissan GT-R (R34) | `v_car_nissan_gt_r_34` | `v_car_nissan_gt_r_34_traffic` |
| Porsche 911 GT2 | `v_car_porsche_911_gt2` | `v_car_porsche_911_gt2_traffic` |
| Subaru Impreza WRX Sti | `v_car_subaru_wrx_sti` | `v_car_subaru_wrx_sti_traffic` |

## See also

- [Cyberpunk 2077 formats](cyberpunk2077-formats.md) for the `.archive` container, Oodle compression and Wwise bank internals.
- [XeNTaX Cyberpunk knowledge](xentax-cyberpunk-knowledge.md) for community-sourced format notes.
