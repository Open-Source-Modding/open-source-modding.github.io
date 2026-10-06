# TDU2 Car Sound Mods
Source: TDU2 modding workspace notes.

A car sound pack is a WAV directory plus two files that describe how the game plays those WAVs: `CarVSTConfig.json` (the editable source) and `CarVSTConfig.xmb` (the compiled binary the game loads). Export and import between the two with TDU2Emploder.

## Pack layout

| File | Role |
|------|------|
| `CarVSTConfig.json` | Editable sound config. Uses typed wrappers. |
| `CarVSTConfig.xmb` | Compiled config loaded by the game. |
| `*.wav` | Sample files referenced indirectly through the target BNK's WAV table. |

## Typed wrappers

Every value in `CarVSTConfig.json` is wrapped in a typed object. Do not flatten or remove the wrapper, the game parser requires it.

```json
{
  "_PrimitiveType": "TYPE",
  "Value": 0
}
```

## Key fields

| Field | Meaning |
|-------|---------|
| `nWaveIndex` | Index into the target `.bnk`'s own internal WAV table, not the filesystem. |
| `nCarID` | Target vehicle slot. Sound mods are replacements by default. |
| `Links[].Physic` | Driving channel: 0 = RPM, 1 = speed, 3 = throttle. |
| `Links[].Audio` | Output: 0 = interior, 1 = exterior. |
| `Links[].CurveSet` | Interleaved `[x, y]` physics-to-gain points. |

### nWaveIndex resolves through the BNK

`nWaveIndex` does not point at a file path. It indexes the target `.bnk`'s own internal WAV table, which is visible with a hex editor. Each BNK has its own table order, and that order differs between BNKs and from filesystem order. The same WAV set reordered into a different BNK will not play the same.

`sSampleName` labels in the JSON are informational only. The game does not reference them.

### nCarID and slot injection

`nCarID` is the vehicle slot the pack replaces. All sound mods are replacements by default. Add-on cars that occupy a new slot need slot injection, which is done with external tools (the PP2 framework).

### Curves and channels

`Links[].CurveSet` is an interleaved list of `[x, y]` points mapping a physics channel value to an output gain. `Links[].Physic` selects the input channel (0 = RPM, 1 = speed, 3 = throttle) and `Links[].Audio` selects the output (0 = interior, 1 = exterior).

## Engine sample layers (pitch chain)

Engine samples are layered by pitch, stepping down 25% per layer:

```
onhigh → onmid (-25%) → onlow (-25%) → onverylow (-25%, often empty) → onidle (separate sample)
```

Sample naming follows `EngineLoad_N` / `EngineUnload_N`, where N is the layer:

| N | Layer |
|---|-------|
| 0 | idle |
| 1 | verylow |
| 2 | low |
| 3 | mid |
| 4 | high |

## Conventions and traps

> **The SLK55 trap.** A Mercedes SL500 mod broke because the modder copied SLK55 filenames 1:1. The SL500 is a convertible and needed different internal names to work. Never assume another car's naming convention maps directly, verify per target. (Discovered by Nikki [MEOW].)

- Sound files are often cannibalized from other games (NFS Carbon and others) and keep misleading original filenames. The names are not authoritative.
- **Do not rename WAV files.** The filename is the only human-readable semantic hint, but the actual `nWaveIndex` → WAV mapping comes from the BNK's internal WAV table order, which is not filesystem order and is not consistent between BNKs.
