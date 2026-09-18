---
title: Wwise SoundBank Format
---

General format specification for `.bnk` SoundBank files used by Audiokinetic Wwise. Little-endian. Signature: `BKHD`.

Source: [XeNTaX Wiki](https://wiki.xentax.com/index.php/Wwise_SoundBank_(*.bnk)) (archived Aug 2023). Additional details from [bnk.ksy](https://github.com/WolvenKit/wwise-audio-tools/blob/master/ksy/bnk.ksy) (Kaitai Struct) and [rewwise](https://github.com/vswarte/rewwise).

## Overview

Wwise SoundBanks contain:

- Individual audio files (`.wem` format, custom Wwise encoding)
- Containers grouping sounds (e.g., random gunshot selector)
- Events playable from game code (e.g., `Play_EmotionalTheme`)

Every Wwise project has an `Init.bnk` with project settings, loaded first. Other SoundBanks hold per-area/level sounds, loaded on demand.

Before 2011, Wwise used `.wav` extension instead of `.wem`. Convert `.wem` to `.ogg` with [ww2ogg](http://hcs64.com/vgm_ripping.html). Some files are streamed (not embedded) — look for `.wem` in the game archive.

## General structure

```
FOR EACH (section) {
    byte[4]   — four-letter identifier (e.g. BKHD, DIDX)
    uint32    — length of section in bytes
    byte[]    — section data
} END FOR
-- END OF FILE --
```

Always seek to the end of each section using the length field. This avoids errors if the parser misses something mid-section.

## BKHD — Bank Header

| Field | Type | Description |
|-------|------|-------------|
| Magic | `42 4B 48 44` | `BKHD` |
| Length | uint32 | Section length |
| Version | uint32 | SoundBank version |
| ID | uint32 | SoundBank ID |
| Reserved | uint32 | Always zero |
| Reserved | uint32 | Always zero |

## DIDX — Data Index

References to `.wem` files embedded in the SoundBank. Each entry is 12 bytes — divide section length by 12 for the count.

| Field | Type | Description |
|-------|------|-------------|
| Magic | `44 49 44 58` | `DIDX` |
| Length | uint32 | Section length |

Per embedded `.wem`:

| Field | Type | Description |
|-------|------|-------------|
| ID | uint32 | `.wem` file ID |
| Offset | uint32 | Offset from start of DATA section |
| Length | uint32 | Length in bytes |

## DATA

Contains the embedded `.wem` files, concatenated. Do not read sequentially — use offsets from DIDX or HIRC.

| Field | Type | Description |
|-------|------|-------------|
| Magic | `44 41 54 41` | `DATA` |
| Length | uint32 | Section length |

Each `.wem` starts with `52 49 46 46` (`RIFF`).

## ENVS — Environments

Not yet analyzed.

## FXPR — Effects Production

Not yet analyzed.

## HIRC — Hierarchy

Contains all Wwise objects: events, containers, sound references, audio buses, music objects.

| Field | Type | Description |
|-------|------|-------------|
| Magic | `48 49 52 43` | `HIRC` |
| Length | uint32 | Section length |
| Count | uint32 | Number of objects |

Per object:

| Field | Type | Description |
|-------|------|-------------|
| Type | byte | Object type (see below) |
| Length | uint32 | Length of this object (includes the 4-byte ID) |
| ID | uint32 | Object ID |
| Data | byte[] | Additional bytes, depends on type |

### Type #1 — Settings

```
01 — identifier
uint32: length
uint32: id
byte: number of settings
FOR EACH (setting):
    byte: setting type (00=Voice Volume, 03=Voice Low-pass Filter)
FOR EACH (setting):
    float: value
```

### Type #2 — Sound SFX / Sound Voice

```
02 — identifier
uint32: length
uint32: id
byte[4]: unknown
uint32: streaming mode
    00 = embedded, not streamed
    01 = streamed
    02 = streamed with Zero Latency (prefetched)
uint32: audio file ID
uint32: source ID
    — if embedded: SoundBank ID (from STID section)
    — if streamed: same as audio file ID (used to find .wem to stream)

IF (embedded):
    uint32: offset to .wem in SoundBank
    uint32: length of .wem in SoundBank
END IF

byte: sound object type (00=Sound SFX, 01=Sound Voice)
byte[]: sound structure (see Sound structure section)
```

### Type #3 — Event Action

```
03 — identifier
uint32: length
uint32: id
byte: scope
    01 = Game object: Switch or Trigger
    02 = Global
    03 = Game object: see referenced object ID
    04 = Game object: State
    05 = All
    09 = All except see referenced object ID
byte: action type
    01 = Stop           08 = Set Voice Pitch
    02 = Pause          09 = Reset Voice Pitch
    03 = Resume         0A = Set Voice Volume
    04 = Play           0B = Reset Voice Volume
    05 = Trigger        0C = Set Bus Volume
    06 = Mute           0D = Reset Bus Volume
    07 = UnMute         0E = Set Voice Low-pass Filter
                       0F = Reset Voice Low-pass Filter
                       10 = Enable State
                       11 = Disable State
                       12 = Set State
                       13 = Set Game Parameter
                       14 = Reset Game Parameter
                       19 = Set Switch
                       1A = Enable/Disable Bypass
                       1B = Reset Bypass Effect
                       1C = Break
                       1E = Seek
uint32: referenced game object ID (or zero)
byte: 00
byte: number of additional parameters
FOR EACH (parameter):
    byte: parameter type
        0E = Delay (uint32, ms)
        0F = Fade in time (uint32, ms)
        10 = Probability (float)
FOR EACH (parameter):
    byte[]: value (format per type above)
byte: 00

IF (action type == 0x12 Set State):
    uint32: State Group ID
    uint32: State ID
ELSE IF (action type == 0x19 Set Switch):
    uint32: Switch Group ID
    uint32: Switch ID
END IF
```

### Type #4 — Event

```
04 — identifier
uint32: length
uint32: id
uint32: number of event actions
FOR EACH (event action):
    uint32: event action ID
```

### Types #5–#7 — Containers and Actor-Mixer

Types 5 (Random/Sequence Container), 6 (Switch Container), 7 (Actor-Mixer) — format not yet documented.

### Type #8 — Audio Bus

```
08 — identifier
uint32: length
uint32: id
uint32: parent bus ID (0 = root/Master Audio Bus)
byte: number of additional parameters
FOR EACH (parameter):
    byte: type (00=Voice Volume, 02=Voice Pitch, 03=Voice Low-pass, 04=Bus Volume)
FOR EACH (parameter):
    float: value
byte: priority equal behavior (00=discard oldest, 01=discard newest)
byte: limit reached behavior (00=kill voice, 01=use virtual voice)
uint16: sound instance limit
boolean: override parent playback limit
uint32: unknown (always 63)
uint32: auto-ducking recover time (ms)
float: auto-ducking max volume
uint32: number of ducked buses
FOR EACH (ducked bus):
    uint32: ducked bus ID
    float: volume
    uint32: fade out (ms)
    uint32: fade in (ms)
    byte: curve shape
        00 = Log3       05 = S-Curve
        01 = Sine In    06 = Exp1.41
        02 = Log1.41    07 = Sine Out
        03 = Inv S      08 = Exp3
        04 = Linear
    byte: target (00=Voice Volume, 04=Bus Volume)
byte: number of effects
IF (effects > 0):
    byte: bypass mask (bit 0-3 = effect 0-3, bit 4 = bypass all)
    FOR EACH (effect):
        byte: index (00–03)
        uint32: effect object ID
        byte[2]: zeros
IF (effects > 0):
uint16: number of RTPCs
FOR EACH (RTPC):
    uint32: game parameter ID (x-axis)
    uint32: y-axis type
        00 = Voice Volume      16 = Output Bus Volume
        03 = Voice Low-Pass    17 = Output Bus LPF
        08 = Priority          18-1C = Bypass Effect 0-3/All
        09 = Sound Limit       1D = Motion Volume Offset
        0F-13 = Aux Sends 0-3  1E = Motion Low Pass
    uint32: unknown ID
    byte: unknown
    byte: number of points
    byte: unknown
    FOR EACH (point):
        float: x
        float: y
        uint32: curve shape (00–09, 09=Constant)
uint32: number of state groups
FOR EACH (state group):
    uint32: state group ID
    byte: change timing (interactive music only, else 0)
        00=Immediate, 01=Next Grid, 02=Next Bar
        03=Next Beat, 04=Next Cue, 05=Custom Cue
        06=Entry Cue, 07=Exit Cue
    uint16: number of states with custom settings
    FOR EACH (state):
        uint32: state ID
        uint32: settings object ID (type #1)
```

### Type #9 — Blend Container

Format not yet documented.

### Type #10 — Music Segment

Similar to Music Track but can contain multiple audio files played simultaneously (e.g., one per instrument).

```
0A — identifier
uint32: length
uint32: id
byte[]: sound structure
uint32: number of child objects
FOR EACH (child):
    uint32: child ID
byte[]: unknown bytes
```

### Type #11 — Music Track

Basic music object. One created per imported audio file. Contained inside a Music Segment.

Format not yet documented.

### Type #12 — Music Switch Container

Uses Switches or States to select which music plays (e.g., `activity` group with `combat`/`stealth`/`exploration` states).

```
0C — identifier
uint32: length
uint32: id
byte[]: sound structure
uint32: number of children
FOR EACH (child):
    uint32: child ID
byte[4]: zeros
float: unknown
byte[8]: zeros
float: tempo (usually 120)
byte: time signature numerator
byte: time signature denominator (usually 4)
byte: 01
byte[4]: zeros
uint32: number of transitions
FOR EACH (transition):
    uint32: source ID (FFFFFFFF=Any, 00000000=Nothing)
    uint32: destination ID (same convention)
    int32: source fade-out time (ms)
    uint32: source fade-out curve shape
    int32: source fade-out offset (ms)
    uint32: exit source at (00=Immediate…07=Exit Cue)
    — newer Wwise may add uint32: custom cue match ID
    byte: 00=Nothing, FF=play post-exit of source
    int32: destination fade-in time (ms)
    uint32: destination fade-in curve shape
    int32: destination fade-in offset (ms)
    uint32: custom cue filter match ID (0=Any)
    uint32: playlist item jump ID (if destination is Playlist Container, else 0)
    uint16: sync to (00=Entry Cue, 01=same time, 02=random cue, 03=random custom cue)
    byte: 00=Nothing, FF=play pre-entry of destination
    boolean: has transition object
    uint32: transition object ID (0=none)
    int32: transition fade-in time (ms)
    uint32: transition fade-in curve shape
    int32: transition fade-in offset (ms)
    int32: transition fade-out time (ms)
    uint32: transition fade-out curve shape
    int32: transition fade-out offset (ms)
    byte: 00=Nothing, FF=play pre-entry of transition
    byte: 00=Nothing, FF=play post-exit of transition
uint32: switch type (00=Switch Group, 01=State Group)
uint32: switch/state group ID
uint32: default switch/state ID (0=None)
boolean: continue on switch change
uint32: number of switches/states
FOR EACH (switch/state):
    uint32: switch/state ID
    uint32: music object ID to play
```

### Type #13 — Music Playlist Container

Contains multiple Segments played in sequence. Transitions must be predefined.

```
0D — identifier
uint32: length
uint32: id
byte[]: sound structure
uint32: number of segments
FOR EACH (segment):
    uint32: segment ID
byte[4]: zeros
float: unknown
byte[8]: zeros
float: tempo (usually 120)
byte: time sig numerator
byte: time sig denominator (usually 4)
byte: 01
byte[4]: zeros
uint32: number of transitions
(FOR EACH transition: same format as Music Switch Container)
```

### Type #14 — Attenuation

Format not yet documented.

### Type #15 — Dialogue Event

Format not yet documented.

### Type #16 — Motion Bus

Format not yet documented.

### Type #17 — Motion FX

Format not yet documented.

### Type #18 — Effect

Format not yet documented.

### Type #20 — Auxiliary Bus

Format not yet documented.

## Sound structure

Shared sub-structure used by types #2, #10, #12, #13. Not yet fully documented.

## STID — Sound Type ID

Lists all SoundBanks referenced in HIRC (including the current one). Maps SoundBank IDs to filenames.

```
53 54 49 44 — STID
uint32: length
uint32: always 01 00 00 00
uint32: number of SoundBanks
FOR EACH (SoundBank):
    uint32: SoundBank ID
    byte: name length
    char[]: name (e.g. "music" → "music.bnk")
```

## STMG — State Management

Only found in `Init.bnk`. Contains project settings, Switch Groups, State Groups, and Game Parameters.

## Curve shapes

Used across multiple sections for fade/automation curves:

| Value | Shape |
|-------|-------|
| 00 | Logarithmic (Base 3) |
| 01 | Sine (Constant Power Fade In) |
| 02 | Logarithmic (Base 1.41) |
| 03 | Inverted S-Curve |
| 04 | Linear |
| 05 | S-Curve |
| 06 | Exponential (Base 1.41) |
| 07 | Sine (Constant Power Fade Out) |
| 08 | Exponential (Base 3) |
| 09 | Constant |

## Tools

- [bnkextr](https://github.com/eXpl0it3r/bnkextr) — extract `.wem` from `.bnk` (C++)
- [ww2ogg](http://hcs64.com/vgm_ripping.html) — convert `.wem` to `.ogg`
- [bnk.ksy](https://github.com/WolvenKit/wwise-audio-tools/blob/master/ksy/bnk.ksy) — Kaitai Struct spec
- [rewwise](https://github.com/vswarte/rewwise) — Rust parser (deku-based)
- [pycozmo audiokinetic.soundbank](https://pycozmo.readthedocs.io/) — Python reader
