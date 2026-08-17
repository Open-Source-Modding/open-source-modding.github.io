# Watch Dogs HKX Collision Format

The `.hkx` files alongside `.xbg` models contain Havok physics collision data. Ubisoft modified the Havok SDK (2015.1) to generate custom collision meshes, which is why standard Havok tools couldn't parse them.

## File Counts

| Game | .hkx Files | Notes |
|------|-----------|-------|
| WD1 | 23,278 | Buildings, vehicles, characters |
| WD2 | 8,195 | Locations, vehicles, characters |
| WDL | 11,043 | World geometry, batched meshes |

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
- [HavokDisrupt](https://reshax.com/topic/1265-watch-dogs-1-pc/) — FrankMK04's tool
- [Havok 2013 SDK](https://github.com/sigmaco/havok-2013-v1.0r1) — original headers
- `format_new.cpp` in HavokLib — packfile parsing reference
