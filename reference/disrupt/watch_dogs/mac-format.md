# Watch Dogs MAC (Mesh Animation Clip) Binary Format

> **Source**: Decompiled `AnimationMarkupTool.exe` (ilspycmd 11.0.0.9375) + hex analysis of 43,253 MAC files from the Ubisoft leak.
> **Confidence**: HIGH — derived from official tool source, verified against leak binaries.
> **Python port**: `blender-io-disrupt/modules/Watch_Dogs/animation_parser.py` (767 lines, MAC + markup parser + CLI).

---

## Overview

MAC is the **source/authoring** animation format used by the Disrupt engine's AnimationMarkupTool. It stores uncompressed animation curves, skeleton hierarchy, flags, events, and sub-animation parts. MAC files are the input to the compilation pipeline that produces runtime MAB files.

### Position in the Pipeline

```
animation.fbx          (FBX source — DCC tool export)
    ↓  FBX import via FbxLoaderManaged.dll (C++/CLI, native FBX SDK)
animation.mac          (binary source — curves, skeleton, flags, events, parts)
animation.markup       (XML source — time-stamped game events)
    ↓  CMarkupCompiler (AnimationMarkupTool internal)
animation.markup.bin   (compiled markup header — 10 bytes, version=3)
animation.mab          (compiled animation markup — compressed per-bone data)
```

- **MAC** = source format (uncompressed, full skeleton, FBX path)
- **MAB** = compiled runtime format (Oodle compressed, quantized u8 curves, per-clip)
- **Markup XML** = event definitions referencing the MAC by name

### File Counts (Ubisoft Leak)

| Type | Count | Location |
|------|-------|----------|
| `.mac` | 43,253 | `data/animations/` |
| `.markup` | 40,691 | `data/animations/` |
| `.mab` | 38,419 | `data_win64/animations/` |
| `.markup.bin` | 37,971 | `data_win64/animations/scr/` |

### Size Relationship

MAB is heavily compressed vs MAC — typically **0.01x to 0.1x** the MAC size. For example:
- `idle_spidertank.mac`: 139KB → `idle_spidertank.mab`: 3.4KB (0.024x)
- `attack01_spidertank.mac`: 46KB → `attack01_spidertank.mab`: 2.6KB (0.057x)
- `ghost-mode_mplyr.mac`: 1.2MB → `ghost-mode_mplyr.mab`: 7.7KB (0.006x)

Compression uses **Oodle** (`oo2core_4_win64.dll` present in AnimationMarkupTool).

---

## MAC Binary Layout

Source: `AnimationFile.LoadFromStream(BinaryReader stream)` in `AnimationMarkupTool.exe`.

### Header

| Offset | Type | Field | Notes |
|--------|------|-------|-------|
| 0x00 | f32 | `version` | Version indicator. **8.305** = legacy format, **9.0** = new format |
| 0x04 | (conditional) | | If version == 9.0: u16 `file_version` + u8 `exporter_major` + u8 `exporter_minor` |

The version field determines encoding behavior throughout the file. Legacy (v8.305) and new (v9.0) differ primarily in how `_Source` is encoded after `_FrameRate`.

### Post-Header

| Offset | Type | Field | Notes |
|--------|------|-------|-------|
| +0 | StringID | `_Type` | Animation type name (e.g. `"07_SpiderTank"`) |
| +N | f32 | `_FrameRate` | Frame rate (e.g. 30.0) |
| +N | StringID | `_Source` | FBX source path. **Only present if** version > 7 and version < 2.139e9 |

### Arrays

All arrays are preceded by a u32 count. Read in this order:

| Order | Type | Description |
|-------|------|-------------|
| 1 | AnimFlag[] | Animation flags (`n_flags` items) |
| 2 | AnimFileEvent[] | Embedded animation events (`n_events` items) |
| 3 | AnimPart[] | Animation parts/sub-animations (`n_parts` items) — **RECURSIVE** |

### Root Motion & Skeleton

After the arrays, the file contains:

| Order | Type | Description |
|-------|------|-------------|
| 4 | AnimDirNode | Root motion node (direction/speed curves) |
| 5 | AnimMomentum | Momentum data (direction + speed curves) |
| 6 | AnimSkeleton | Full skeleton with per-bone animation curves |

### Trailing Data

```
i32       trailing_string_len
char[]    trailing_string    (optional, length-prefixed)
```

### Complete Layout (Visual)

```
[0x00] f32 version (8.305 or 9.0)
[0x04] (if v9.0: u16 file_version + u8 exporter_major + u8 exporter_minor)
[+0]   StringID _Type
[+N]   f32 _FrameRate
[+N]   StringID _Source (if version > 7 and < 2.139e9)
[+N]   u32 n_flags + AnimFlag[n_flags]
[+N]   u32 n_events + AnimFileEvent[n_events]
[+N]   u32 n_parts + AnimPart[n_parts]  (recursive!)
[+N]   AnimDirNode _DirNode (root motion)
[+N]   AnimMomentum _Momentum
[+N]   AnimSkeleton
[+N]   i32 trailing_string_len + string
```

---

## StringID Structure

Used throughout the MAC format. Recurring structure:

```
u32  type_id      (hash/type identifier)
i32  string_len   (length of string in chars, NOT bytes)
char[] string     (string_len chars)
```

The `type_id` is a hash of the string content. The string is length-prefixed with a signed 32-bit integer indicating character count (not byte count — relevant for Unicode, though most strings are ASCII).

---

## AnimFlag

```
StringID  _Name      (e.g. "MoveMgr_Poses", "GameEvents")
i32       _FlagType  (enum FlagType)
```

### FlagType Enum

| Value | Name | Description |
|-------|------|-------------|
| 0 | Global | Global animation flag |
| 1 | Curve | Curve-based flag |
| 2 | Step | Step trigger flag |
| 3 | Spike | Spike trigger flag |
| 4 | Frame | Frame-based flag |
| 5 | Unknown | Unknown/unmapped |

---

## AnimFileEvent

```
i32       _Type      (enum AnimFlagName)
f32       _Time      (time in seconds)
u32       _NumParams
AnimFileParameter[_NumParams]  (each: StringID _Type + StringID _Name)
```

### AnimFlagName Enum

| Value | Name | Description |
|-------|------|-------------|
| 0 | Unknown | Unidentified event type |
| 1 | MoveMgr_Poses | Movement manager pose triggers |
| 2 | GameEvents | General game events |
| 3 | DurationEvent | Duration-based events |
| 4 | Max | Sentinel |
| 5 | Force32Bit | Force 32-bit alignment |

---

## AnimPart (Recursive Sub-Animation)

AnimPart contains a nested `AnimationFile`, making the MAC format recursive. This is how complex animations compose sub-animations (e.g., an idle animation with separate upper-body and lower-body parts).

```
i32       version_tag_1      (always 987654321)
i32       version_tag_2      (unknown value)
StringID  _Name
f64       _Time
i32       _PartEventType     (enum PartEventType)
StringID  _ParentId
StringID  _Handle
i32       unk1
i32       unk2
AnimationFile  _File          (RECURSIVE — nested MAC!)
i32       handle_name_len
char[]    _HandleName         (length-prefixed)
i32       ik_bone_name_len
char[]    _IKBoneName         (length-prefixed)
```

### PartEventType Enum

| Value | Name | Description |
|-------|------|-------------|
| 0 | Unused0 | Unused |
| 1 | InPossession | In possession state |
| 2 | Unused1 | Unused |
| 3 | Unused2 | Unused |
| 4 | Unused3 | Unused |
| 5 | Unused4 | Unused |
| 6 | IKPath | Inverse kinematics path |
| 7 | Anchor | Anchor point |
| 8 | SubPartInPossession | Sub-part in possession state |
| 9 | Unused5 | Unused |
| 10 | NumberOf | Sentinel/count |

---

## AnimDirNode (Root Motion)

Root motion node containing directional animation curves.

```
i32       unk1
i32       unk2
u32       _NumCurves
AnimDiscreteCurve[_NumCurves]  _Curves
```

---

## AnimMomentum

Momentum data with direction and speed curves.

```
i32       unk1
i32       unk2
AnimCurve  _DirectionCurve    (reads i32)
AnimCurve  _SpeedCurve        (reads i32)
```

---

## AnimSkeleton

Full skeleton hierarchy with per-bone animation data.

```
i32       unk1
i32       unk2
u32       _NumBones
AnimBone[_NumBones]  _Bones
```

---

## AnimBone (extends AnimDirNode)

Each bone extends AnimDirNode, adding identity and parent information.

```
StringID  _Name
u32       _Id
i32       _ParentId
[AnimDirNode curves: u32 count + AnimDiscreteCurve[]]
```

- `_Name`: Bone name (e.g. `"Hip"`, `"Spine1"`, `"LeftHand"`)
- `_Id`: Unique bone identifier
- `_ParentId`: Parent bone ID (-1 for root)

---

## AnimDiscreteCurve

Individual animation curve data. The core building block of MAC animation data.

```
u32       _NumValues
i32       _CurveDataType    (enum CurveDataType)
i32       unk
f32[_NumValues]  _Values    (only present for Trans/Quat/Rot types)
```

### CurveDataType Enum

| Value | Name | Description |
|-------|------|-------------|
| 0 | RotX | Rotation X axis |
| 1 | RotY | Rotation Y axis |
| 2 | RotZ | Rotation Z axis |
| 3 | TransX | Translation X axis |
| 4 | TransY | Translation Y axis |
| 5 | TransZ | Translation Z axis |
| 6 | ScaleX | Scale X axis |
| 7 | ScaleY | Scale Y axis |
| 8 | ScaleZ | Scale Z axis |
| 9 | QuatX | Quaternion X component |
| 10 | QuatY | Quaternion Y component |
| 11 | QuatZ | Quaternion Z component |
| 12 | QuatW | Quaternion W component |
| 13 | Radius | Radius (cylindrical) |
| 14 | Height | Height (cylindrical) |
| 15 | Unknown | Unknown/unmapped |

### Value Storage

- Rotation, Translation, and Quaternion curves store `f32[_NumValues]` — one float per keyframe.
- Scale, Radius, Height, and Unknown types do **not** store float arrays (the curve is implicit).
- `_NumValues` is typically equal to the number of keyframes in the animation.

---

## Key Insights

### Recursive Structure
The MAC format's recursive AnimPart → AnimationFile nesting allows complex animation compositions. A single `.mac` file can contain multiple sub-animations, each with their own skeleton, curves, and events. This is how blend trees and layered animations are represented at the source level.

### Version Detection
The version float at offset 0x00 is the primary discriminator:
- **8.305** = legacy format (most common in the leak)
- **9.0** = new format (adds file_version + exporter version fields)

The `_Source` field (FBX path) is only present when `version > 7 AND version < 2.139e9`, which covers both known versions.

### MAC vs MAB
- MAC stores **f32 keyframe arrays** per bone per curve — full precision, uncompressed
- MAB stores **quantized u8 values** decoded to [-1.0, +1.0] — heavily compressed
- MAB does NOT use the same data layout as MAC — it reorganizes data for runtime efficiency
- Direct byte-for-byte comparison between MAC re-encoded values and MAB shows zero matches

---

## Related

- [MAB Animation Format](mab-format.md) — compiled runtime format
- [Markup Format](markup-format.md) — event definitions referencing MAC files
- [Asset Pipeline Formats](asset-pipeline-formats.md) — full pipeline mapping
