# Watch Dogs HKX Collision Format (WD1/WD2 — Ubisoft's Havok fork)

> **Cross-reference**: WDL (2017.2 Dunia wrapper) leak-vs-retail bytes, build tags,
> TAG0/TCM0 chunking, TCRF/TCID compendium, PTCH/ITEM, type catalog →
> [`../watch_dogs_legion/HKX_FORMAT_REVERSE.md`](../watch_dogs_legion/HKX_FORMAT_REVERSE.md)
> **Cross-reference**: Standard Havok packfile format (FO4 uses regular Havok SDK) →
> [`reference/havok/hkx_format.md`](../../../havok/hkx_format.md)

The `.hkx` files alongside `.xbg` models contain Havok physics collision data for
**Watch Dogs 1 and Watch Dogs 2**. Ubisoft used a **custom fork** of Havok to
generate custom collision meshes — **standard Havok tools cannot parse them**
(confirmed by FrankMK04: "the commercial SDK doesn't work for rendering them").
He reverse-engineered the format from the game's graphics engine DLL.

> **FrankMK04** (Discord, 8/18/26): "It's interesting to know that Ubisoft modified
> the SDK and removed several classes just to generate its own collision meshes,
> which is why other tools couldn't understand or analyze its structure. But now
> 'HKX has been hacked.' For now, it has a few bugs with complex models like
> vehicles (easy to fix, but it takes time) and with models that don't use convex
> meshes (first the complicated stuff, then the easy stuff) because basic colliders
> are easier to find."

- **WD1** (2014): Uses Havok 2012 SDK, modified by Ubisoft (old packfile format) — see [`packfile-format.md`](packfile-format.md)
- **WD2** (2016): Uses Havok 2015.1 SDK, modified by Ubisoft (TAG0 format) — see [`../watch_dogs_2/wd2-tag0-collision-format.md`](../watch_dogs_2/wd2-tag0-collision-format.md)
- **WDL** (2020): Uses Havok 2017.2 SDK with Dunia wrapper — see [`../watch_dogs_legion/HKX_FORMAT_REVERSE.md`](../watch_dogs_legion/HKX_FORMAT_REVERSE.md)

## File Roles

| Extension | Role |
|-----------|------|
| `.hkx` | Havok collision mesh (physics body) |
| `.hkr` | Havok ragdoll (skeleton constraints) |
| `.ci` | Collision index — maps collision meshes to entity slots |

**HKX + CI handshake**: The `.hkx` file contains the actual collision geometry. The
`.ci` (collision index) file maps which `.hkx` to use for each entity slot. They must
match — a mismatched CI/HKX pair causes missing or broken collision.

## File Counts

| Game | .hkx Files | Notes |
|------|-----------|-------|
| WD1 | 23,278 | Buildings, vehicles, characters |
| WD2 | 8,195 | Locations, vehicles, characters |

## Binary Structure

### Header (16 bytes)

```
+0x00  u32     HeaderField0 (typically 0x89)
+0x04  u32     HeaderCrc
+0x08  u32     TagfileTotalSize (entire file size)
+0x0C  u32     Reserved/unknown
```

### Section Loop (starts at offset 0x10)

Each section is an `hkChunk` (8 bytes header + payload):
```
+0x00  u32     sizeAndFlags (24-bit size + flags, little-endian)
+0x04  u32     tag (FourCC, little-endian)
+0x08  byte[]  payload (size bytes)
```

The `sizeAndFlags` field: `size = value & 0xffffff`, `isSubChunk = (value & 0x40000000) != 0`.

### Section Tags (FourCC)

| Tag | Constant | Description |
|-----|----------|-------------|
| TAG0 | `CompileFourCC("TAG0")` | Header marker (809976148) |
| SDKV | `CompileFourCC("SDKV")` | Havok SDK version string |
| DATA | `CompileFourCC("DATA")` | Serialized object data |
| INDX | `CompileFourCC("INDX")` | Index data |
| TYPE | `CompileFourCC("TYPE")` | Type definitions |
| TSTR/TST1 | `CompileFourCC("TSTR")` | Type name strings |
| FSTR/FST1 | `CompileFourCC("FSTR")` | Field name strings |
| SCFG | — | Ubisoft-specific section |

### DATA Section

Contains serialized Havok objects:
- Type count (u32) + type entries (class_id + byte_count + data)
- Item count (u32) + item entries (type_idx + flags + offset + count)

## HavokLib vs HavokDisrupt

### HavokLib (PredatorCZ)
- C++ library, GPL-3.0
- Supports Havok 5.0.0 - 2017
- Handles hkaSkeleton, hkaAnimation, hkxEnvironment
- Python wrapper available
- Standard packfile format

### HavokDisrupt (FrankMK04)
- C#/.NET tool (WPF)
- Watch Dogs 2 specific
- Handles HkxConvexShape (quantized/full-precision)
- Supports OBJ injection (HKX ↔ OBJ)
- Custom collision mesh format

## Key Types (from HavokDisrupt)

### HkxConvexShape
```
Variant         HkxShapeVariant   Quantized or FullPrecision
ShapeItemIndex  int32
VertexHeaderItemIndex int32
AabbMin         Vector3
AabbMax         Vector3
ConvexRadius    float32
QuantScale      Vector3
Vertices        List<Vector3>
QuadIndices     List<int32>
```

### HkxChildTransform
```
ChildItemIndex  int32
Transform       Matrix4x4
Scale           Vector3
```

## Ubisoft Modifications

1. Removed several Havok classes for custom collision mesh generation
2. Added `HkxConvexShape` with Quantized/FullPrecision variants
3. Used `QuantScale` + `AabbMin/AabbMax` for quantized vertex compression
4. The `TypeId` field maps to modified Havok class IDs

## References

- [HavokLib](https://github.com/PredatorCZ/HavokLib) — C++ packfile library
- [HavokDisruptWD2](https://github.com/FranciscoManzanilla/HavokDisruptWD2) — FrankMK04's HKX resolver for WD2 (reverse-engineered from game DLL, beta; bugs with complex vehicles and non-convex meshes)
- [Havok 2013 SDK](https://github.com/sigmaco/havok-2013-v1.0r1) — original headers
- `format_new.cpp` in HavokLib — packfile parsing reference
