# Watch Dogs Animation Markup XML Format

> **Source**: Decompiled `AnimationMarkupTool.exe` (ilspycmd 11.0.0.9375) + hex analysis of 40,691 markup files from the Ubisoft leak.
> **Confidence**: HIGH — derived from official tool source, verified against leak binaries.
> **Python port**: `blender-io-disrupt/modules/Watch_Dogs/animation_parser.py` (767 lines, MAC + markup parser + CLI).

---

## Overview

Markup files are XML event definitions that reference `.mac` animation files by name. They contain time-stamped game events — footsteps, sounds, attacks, camera shakes, weapon events, etc. The engine loads these at runtime to trigger gameplay events during animation playback.

### Position in the Pipeline

```
animation.fbx          (FBX source — DCC tool export)
    ↓  FBX import via FbxLoaderManaged.dll (C++/CLI, native FBX SDK)
animation.mac          (binary source — curves, skeleton, flags, events, parts)
animation.markup       (XML source — time-stamped game events)   ← THIS FILE
    ↓  CMarkupCompiler (AnimationMarkupTool internal)
animation.markup.bin   (compiled markup header — 10 bytes, version=3)
animation.mab          (compiled animation markup — compressed per-bone data)
```

- **Markup XML** = source event definitions (human-readable, editable)
- **Markup BIN** = compiled header (tiny, just version=3)
- **MAB** = compiled runtime format that embeds the actual event data

### File Counts (Ubisoft Leak)

| Type | Count | Location |
|------|-------|----------|
| `.markup` | 40,691 | `data/animations/` |
| `.markup.bin` | 37,971 | `data_win64/animations/scr/` |
| `.mac` | 43,253 | `data/animations/` |
| `.mab` | 38,419 | `data_win64/animations/` |

### Naming Convention

Markup files share the same basename as their corresponding MAC file:
```
data/animations/locomotion/walk_forward.markup
data/animations/locomotion/walk_forward.mac
```

---

## XML Structure

```xml
<?xml version="1.0" encoding="utf-8"?>
<markup>
  <EditionHelper>
    <ClipParams>
      <ClipParam ResNodeID="mainroot\blendtrees\walk_forward" ClipStart="0" ClipEnd="0.966667" />
    </ClipParams>
  </EditionHelper>
  <events>
    <event time="0.39375" name="EventName" Track="AI1">
      <CSoundEvent hidEventName="snd_footstep_left" hidTargetEntityId="18446744073709551615" bAlwaysTrigger="false" />
    </event>
    <event time="0.7875" name="EventName2" Track="Audio1">
      <CFootStepEvent boneBoneName="LeftFoot" sndSound="event:/char/footstep_run_concrete_left" />
    </event>
  </events>
</markup>
```

### Root Element

`<markup>` — top-level container for clip parameters and events.

### EditionHelper Section

Contains `<ClipParams>` with one or more `<ClipParam>` elements defining the animation clip boundaries:

| Attribute | Type | Description |
|-----------|------|-------------|
| `ResNodeID` | string | Resource path to the MAC animation (e.g. `"mainroot\blendtrees\walk_forward"`) |
| `ClipStart` | float | Start time in seconds (usually `0`) |
| `ClipEnd` | float | End time in seconds (animation duration) |

### Events Section

Contains `<event>` elements placed on the animation timeline:

| Attribute | Type | Description |
|-----------|------|-------------|
| `time` | float | Position in seconds on the animation timeline |
| `name` | string | Display name (usually matches event type without `C` prefix) |
| `Track` | string | Track name for the event (e.g. `"AI1"`, `"Audio1"`, `"FX1"`, `"Weapon1"`) |

### Event Child Elements

Each `<event>` contains exactly one child element whose **tag name is the event ClassId** (e.g. `CSoundEvent`, `CFootStepEvent`, `CWeaponEvent`). Common parameters across most events:

| Parameter | Type | Description |
|-----------|------|-------------|
| `hidEventName` | CStringID | Event display name / identifier |
| `hidTargetEntityId` | CUniqueID | Target entity ID (`18446744073709551615` = `UINT64_MAX` = "any") |
| `bAlwaysTrigger` | bool | Trigger even if conditions not met |

---

## Event Definitions

**Source**: `leak/ubisoft/data/animations/EventDefinitions/AnimationMarkupEvents.defs` — 184 event types.

### Top Event Types

| ClassId | Purpose | Key Parameters |
|---------|---------|---------------|
| `CFootStepEvent` | Footstep sounds | `boneBoneName`, `sndSound` |
| `CSoundEvent` | Sound effects | `sndSound`, `bAssignOnEntity`, `bInterruptible` |
| `CDecalEvent` | Decals (blood, bullet holes) | `boneBoneName`, `vectorPositionOffset`, `decalDecal` |
| `CWeaponEvent` | Weapon events (fire, reload) | `sWeaponEvent` |
| `CCameraShakeAndPadRumbleEvent` | Camera effects | `sEffectName`, `fIntensityModifier` |
| `CAnimatedCameraEvent` | Camera animations | `fileCameraAnimation`, `selAnchorMode` |
| `CHideEvent` | Hide/show entities | `bHide`, `fFadeSpeed` |
| `CDeadInAnimEvent` | Death events | `bGotoRagdoll` |
| `CPoseEvent` / `CAnimPoseEvent` | Pose triggers | `poseType` |
| `CAccessoryEvent` | Show/hide accessories | `selEventType` (Phone/Baton/LaptopLight) |
| `CAnimTechEvent` | Tech actions | `selTechType` (Hand in pocket/Face mask/Ear Buds) |
| `CLadderEvent` | Ladder interactions | `bStart`, `bEnd`, `bLeftHand` |
| `CCoverStanceEvent` | Cover system | `hidEventType` |
| `CSpiderTankEventAttack` | SpiderTank attacks | `bStart` |
| `CCLOActionActivityChange` | CLO activity changes | `selActivity` (Sleep/Yoga/Playing...) |
| `CCLOActionLocationChange` | CLO location changes | `selLocation` (Bench/Couch/Ground...) |

### Event Parameter Types

| Type | Description |
|------|-------------|
| `CStringID` | Hashed string identifier (u32 hash + length-prefixed string) |
| `CUniqueID` | 64-bit unique identifier (entity reference) |
| `bool` | Boolean flag |
| `int` | Integer enum value |
| `float` | Floating point value |
| `float3` | Three-component vector |
| `float4` | Four-component vector |

---

## Hash Function

The markup system uses a custom string hash for event name lookup:

```csharp
uint GetHashFromName(string name)
{
    uint hash = 111;
    for (int i = 0; i < name.Length; i++)
    {
        if (name[i] != 0)
            hash = hash * 113 + name[i];
    }
    return hash;
}
```

This is the same hash used for StringID `type_id` values throughout the MAC format.

---

## Compiled Markup (.markup.bin)

The compiled markup binary is minimal — just a version header:

| Offset | Type | Value |
|--------|------|-------|
| 0x00 | u32 | Magic/version (always `3`) |

The actual event data lives in the `.mab` file, not in `.markup.bin`. The `.markup.bin` serves as a resource reference that the engine uses to locate the compiled animation data.

---

## Event Tracks

Events are organized on named tracks for parallel playback:

| Track | Typical Use |
|-------|-------------|
| `AI1` | AI behavior triggers |
| `Audio1` | Sound effects |
| `FX1` | Visual effects (particles, decals) |
| `Weapon1` | Weapon events |
| `Camera1` | Camera events |
| `Anim1` | Animation control events |

Multiple events can share the same track — they play sequentially at their specified time positions.

---

## Practical Usage

### Creating Events

1. Open `.mac` file in AnimationMarkupTool
2. Navigate to the desired time position on the timeline
3. Right-click → Add Event
4. Select event type from the 184 defined types
5. Set parameters (sound file, bone name, intensity, etc.)
6. Save → creates/updates `.markup` file

### Event Timing

Events are placed at specific time positions within the animation. For a 30fps animation with 100 frames:
- Frame 0 = 0.0 seconds
- Frame 30 = 1.0 seconds
- Frame 50 = 1.6667 seconds

The `time` attribute uses float seconds, not frame numbers.

### Common Patterns

**Footstep events** alternate between left and right feet:
```xml
<event time="0.39375" name="footstep_left" Track="Audio1">
    <CFootStepEvent boneBoneName="LeftFoot" sndSound="event:/char/footstep_run_concrete_left" />
</event>
<event time="0.7875" name="footstep_right" Track="Audio1">
    <CFootStepEvent boneBoneName="RightFoot" sndSound="event:/char/footstep_run_concrete_right" />
</event>
```

**Attack events** trigger at the impact frame:
```xml
<event time="0.5" name="attack_hit" Track="Weapon1">
    <CWeaponEvent sWeaponEvent="hit" />
</event>
```

---

## Key Insights

### Separation of Concerns
- MAC stores animation data (curves, skeleton, events)
- Markup stores gameplay events (sounds, effects, AI triggers)
- This separation allows editing events without re-exporting animation from FBX

### Event Count
40,691 markup files vs 43,253 MAC files — not all animations have markup (some are pure motion without gameplay events).

### Track System
The track system allows parallel event streams. A single animation can have simultaneous audio, FX, AI, and weapon events without conflicts.

---

## Related

- [MAC Animation Format](mac-format.md) — source animation binary format (referenced by markup)
- [MAB Animation Format](mab-format.md) — compiled runtime format (contains event data)
- [Asset Pipeline Formats](asset-pipeline-formats.md) — full pipeline mapping
