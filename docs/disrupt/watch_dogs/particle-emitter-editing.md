# Particle and Emitter Editing (WD1)

Source: Watch Dogs 1 particle system overhaul working notes.

This page covers editing WD1 particle systems end to end: pulling the RML databases out of the game archives, converting them to XML, editing emitter attributes and curves, and installing the result. It also records the curve encoding scheme, the texture reference convention, the light attributes that drive global illumination, and the day/night paired emitter pattern, all as they appear in the shipped overhaul. For the environment lighting parameters that sit behind GI, see [Lighting Parameters](lighting-parameters.md).

## The editing pipeline

Particle systems ship as RML databases inside the game archives. The workflow is:

1. **Extract the RML.** Pull `ParticlesEmitters` / `ParticlesSystems` RML databases out of the game archives with Gibbed.Disrupt BigFile.
2. **RML to XML.** Convert each RML with `Gibbed.Disrupt.ConvertXml` in RML format. Use the RML mode, not the binary-object mode (`Gibbed.Disrupt.ConvertBinaryObject`), which uses `.binaryclass.xml` definitions and is for a different asset family.
3. **Edit XML.** Change scalar attributes directly. Leave curve base64 payloads untouched (see below).
4. **XML to RML.** Convert back with the same `Gibbed.Disrupt.ConvertXml` tool.
5. **Install.** Package the mod and install it through NexusTools.

The overhaul illustrates the scale: a single 537K-line XML holding 6,722 emitters.

## Curve encoding

Curves encode animation over a particle's lifetime. Two forms appear in the XML.

### Scalar curves

A scalar attribute carries its keyframes inline:

```xml
<PartSizeKF curve="Count(6) AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" />
```

`Count(N)` is the number of float32 values in the base64 payload, and `N/2` is the number of `(time, value)` keyframes. The payload is a flat sequence of float32 pairs: time first, value second. The payloads shown here are zeroed so the byte counts stay legible; a real curve carries the actual keyframe times and values.

### Colour curves

Colour gradients use two coordinated attributes:

```xml
<PartColorKF times="Count(8) AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA="
             colors="Count(8) AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=" />
```

`times` is `N` float32 values, and `colors` is `N` four-byte RGBA entries (one per time sample). The two counts must match.

### Time normalisation

Times are normalised `0.0` to `1.0` over the particle lifetime, not in seconds. A time of `0.5` is half-way through a particle's life.

### Decoding a curve

The following decodes either form from the attribute string as it appears in the XML.

```python
import base64
import struct

def decode_floats(payload: str) -> list:
    """payload is 'Count(N) BASE64'."""
    count, b64 = payload.split(" ", 1)
    n = int(count[len("Count("):-1])
    raw = base64.b64decode(b64)
    assert len(raw) == n * 4, (len(raw), n)
    return list(struct.unpack(f"<{n}f", raw))

def decode_scalar(payload: str) -> list:
    """Return [(time, value), ...] pairs."""
    vals = decode_floats(payload)
    return list(zip(vals[0::2], vals[1::2]))

def decode_colour(times_payload: str, colors_payload: str) -> list:
    """Return [(time, r, g, b, a), ...] with 0-255 channels."""
    times = decode_floats(times_payload)
    count, b64 = colors_payload.split(" ", 1)
    raw = base64.b64decode(b64)
    assert len(raw) == len(times) * 4
    colours = [tuple(raw[i * 4:i * 4 + 4]) for i in range(len(times))]
    return [(t, *c) for t, c in zip(times, colours)]
```

### Editing rule

When you edit a scalar attribute (say `HDRMul`, `StaticSize`, or `NbInitPart`), leave the base64 in any adjacent curve attribute untouched. The curves and the scalars are independent fields; rewriting a curve by hand means regenerating the float32 payload and re-encoding it, not editing the base64 text character by character.

## Texture references

The XML references textures by a `.png` name, for example:

```xml
<PartEmit DiffuseTexture="graphics\gfx\weapons\gfx_muzzleflashglowuc01_d.png" />
```

The actual files on disk are `.xbt`. The engine maps the `.png` name to the matching `.xbt` at load time, so a new effect needs an XBT whose name matches what the XML references. The overhaul's rifle muzzle flash, for instance, uses a custom `gfx_riflemuzzleflash01_d.xbt` referenced as `gfx_riflemuzzleflash01_d.png`.

The XBT container itself is documented in [XBT Texture Format](xbt-texture-format.md). One fact worth repeating here: byte `0x19` in the TBX header selects the variant class, where `0x08` is self-contained, `0x0A`/`0x0B` is a regular file that references a `_high` file, and `0x01` is the `_high` variant.

## Light attributes

Emitter light behaviour is controlled by a small set of scalar attributes.

| Attribute | Purpose |
|-----------|---------|
| `PartLightAffectsEnvironment` | GI toggle (0/1): whether the emitter feeds global illumination |
| `HDRMul` | Light intensity multiplier |
| `AffectedBySunlight` | Sun interaction |
| `TimeOfDay` | 0 = always on, 1 = restricted to a time window |
| `TimeOfDayStartHour` | Window start, decimal hours (`6.5` = 06:30) |
| `TimeOfDayEndHour` | Window end, decimal hours (`17.5` = 17:30) |

`PartLightAffectsEnvironment` is the switch that decides whether a light contributes to GI at all. Setting it to `0` alongside `HDRMul=0` removes an emitter's light from the environment entirely; that is how the overhaul silenced the SO SMG-11 smoke emitters, which were lighting surfaces without actually emitting a muzzle flash.

## Day/night paired emitters

The engine pairs a night light with a daytime clone. A typical vanilla pair:

- `light_m_1`, active at night, `TimeOfDayStartHour=17.5`, `TimeOfDayEndHour=6.5`
- `light_m.day_2`, active by day, `TimeOfDayStartHour=6.5`, `TimeOfDayEndHour=17.5`

The day variant is a clone with the two hours swapped and the same `HDRMul`. This pattern lets a fire or explosion place light at night and a different light (or none) during the day.

The overhaul extended this pattern to daytime fire GI: it created 59 daytime clones at `HDRMul=0.25` covering `TimeOfDayStartHour=6.5` to `TimeOfDayEndHour=17.5`. After those additions the XML held 6,722 emitters total.

## Emission and motion parameters

Beyond the light fields, the overhaul touched the emission and motion parameters that shape how particles leave an emitter.

| Attribute | Purpose | Overhaul value |
|-----------|---------|----------------|
| `EmitDirUseEmitterVelocity` | Inherit the emitting object's velocity (swirl) | `1` enabled |
| `EmitDirAxisSpread` | Angular spread of the emission direction | `45` degrees |
| `EmitVolHalfSize` | Half-size of the emission volume (volume emission) | `0.1` default, `0.15` water, `0.08` dust |
| `EmitVolRadius` | Emission radius (used by some emitters) | varies (e.g. `0.3` to `0.15` on a steam grate) |

`EmitDirUseEmitterVelocity` makes particles inherit the emitter's velocity, which produces swirl; `EmitDirAxisSpread` widens the cone; `EmitVolHalfSize` emits from a volume rather than a point, with the size chosen per particle type. The overhaul made 370 changes across 159 wheelfx emitters this way, using a larger volume for water splash (`0.15`) and a smaller one for dust (`0.08`).

### Impact effect tuning

The overhaul reduced exaggerated impact effects by dropping `HDRMul` and `StaticSize`:

| Effect | Before | After |
|--------|--------|-------|
| Vehicle impact sparks | `HDRMul=20`, `StaticSize=200` | `HDRMul=8`, `StaticSize=90` |
| Bumper detach smoke | `StaticSize=120` | `StaticSize=48` |
| Vehicle window shards | `StaticSize=250` | `StaticSize=100` |
| Glass grit | `StaticSize=100` | `StaticSize=40` |

### Backfire

No backfire particle system exists in the XML. Backfire is not XML-editable: only exhaust fire and NOS systems exist there, and they function. A backfire that fails on some cars is a gameplay or vehicle damage-state trigger issue on those models, not something the particle databases can fix.

## Editor class definitions

The field lists for the particle classes live beside the other binary-object definitions. The particle-relevant ones:

| File | Contents |
|------|----------|
| `ParticlesEmitter.binaryclass.xml` | 249 fields, 44 objects (complete `PartEmit` definition) |
| `ParticlesSystems.binaryclass.xml` | 57 system-level `Mod*` override fields |
| `Knots.binaryclass.xml` | `KnotsParameters` (Position, Color, Value, Info, Type, Gradient) |
| `Curves.binaryclass.xml` | `CurvesParameters` (inherits `KnotsParameters`) |
| `CParticleSequenceTrack.binaryclass.xml` | animation tracks with `Key` objects |

These are the class definitions for `Gibbed.Disrupt.ConvertBinaryObject`, useful as a field reference when an attribute's meaning is unclear. The definitions do not document gameplay intent, only the field shape.

## Diffing a modded particle set against vanilla

The overhaul's changes were audited by diffing the modded database against vanilla, per system:

1. Capture each emitter block from both files.
2. Compare per system (each block is `<library>.<system>.<variant>`), matching system keys.
3. Read the resulting added/removed emitter lists and per-attribute changes.

The property diff for the overhaul reported 51 changed blocks, 112 `PartSys`-level attribute changes, 62 emitters added, 335 emitters removed, and 136 changed binary curves. Per-block emitter churn is often large: one block, `wdfx_pil_spider.bulletlogicalmaterials.water`, lists 247 removed emitters, and muzzle flash systems swapped out radial and night emitters for a projected-fire set. This kind of diff is the only reliable way to see what a particle mod actually changed, since the XML is far too large to review by hand.
