# Watch Dogs HKX Collision Format

The `.hkx` files alongside `.xbg` models contain Havok physics collision data. Ubisoft modified the Havok SDK (2015.1) to generate its own collision meshes, which is why standard Havok tools couldn't parse them.

## File Structure

An HKX file contains:
- Header (size, CRC, SDK version)
- Data section (raw Havok serialized data)
- TCRF section (type information)
- Item table (objects with type IDs, offsets, flags)
- Fixup table (pointer references between items)

## Key Types

### HkxFile (container)
```
HeaderField0   uint32
HeaderCrc      uint32
TagfileTotalSize uint32
SdkVersion     string
DataSection    byte[]
TcrfSection    byte[]
Items          List<HkxItem>
Fixups         List<HkxFixup>
RootItemIndex  int32
ConvexShapes   List<HkxConvexShape>
ChildTransforms Dictionary<int, HkxChildTransform>
```

### HkxItem (each object in the file)
```
Index      int32      item index
TypeId     uint32     Havok type identifier
Flags      byte       item flags
DataOffset uint32     offset to item data in DataSection
Count      uint32     element count
Bytes      byte[]     raw item bytes
```

### HkxFixup (pointer references)
```
PointerType     uint32   type of pointer
PointerLocation uint32   location in data where pointer is stored
TargetItemIndex int32    index of target item
```

### HkxConvexShape (collision shape)
```
Variant           HkxShapeVariant   Quantized or FullPrecision
ShapeItemIndex    int32
VertexHeaderItemIndex int32
AabbMin           Vector3           bounding box min
AabbMax           Vector3           bounding box max
ConvexRadius      float32
QuantScale        Vector3
Vertices          List<Vector3>     vertex positions
QuadIndices       List<int32>       quad indices
```

### HkxChildTransform (bone/node transform)
```
ChildItemIndex int32       index of child bone
Transform      Matrix4x4   4x4 transformation matrix
Scale          Vector3     scale vector
```

## Parsing Pipeline

1. `Parse(path)` / `ParseBytes(buf)` — read file, parse header
2. `ReadItems(buf, off, len, file)` — read item table
3. `ReadPatches(buf, off, len, file)` — read fixup/patch table
4. `SliceItemBytes(file)` — slice item data from data section
5. `ResolveFixups(file)` — resolve pointer references
6. `FindRoot(file)` — find root object
7. `ExtractAllConvexShapes(file)` — extract collision shapes
8. `TryDecodeQuantized(shape, byIdx, fixupsByOwner)` — decode quantized shapes
9. `DecodeQuantized(shape, shapeData, hdr, verts, idx)` — decode quantized data
10. `TryDecodeFullPrecisionShape(shape, byIdx, fixupsByOwner)` — decode full precision
11. `GetShapeTransform(file, shape)` — get transform matrix
12. `ToTransformedTriangleMesh(file, shape, positions, indices)` — convert to triangles
13. `ExportObj(file, outPath, applyTransforms, reverseWinding)` — export to OBJ

## Injection (Reverse Direction)

The tool supports injecting OBJ meshes back into HKX files:
- `LoadObj(path)` — load OBJ file
- `InjectObjIntoShape(file, shapeIndex, obj, originalBytes)` — inject into shape
- `InjectFullPrecisionShape(file, shapeIndex, obj, originalBytes)` — inject full precision
- `AutoFitAndInject(file, shapeIndex, obj, originalBytes)` — auto-fit and inject
- `AutoFitAllShapes(file, obj, originalBytes)` — auto-fit all shapes
- `ComputeConvexHull(points, hullVertices, hullTriangles)` — compute convex hull

## Notes

- Ubisoft modified Havok SDK 2015.1, removing several classes for their collision mesh generation
- Complex models (vehicles) and non-convex meshes have known bugs in the viewer
- The tool is WPF-based (Windows only), but the parser logic is pure C#/.NET
- PDB file available for source-level debugging

## References

- [FrankMK04's HKX Format Viewer](https://reshax.com/topic/1265-watch-dogs-1-pc/) — original tool and format analysis
- [HavokDisrupt source](https://github.com/Open-Source-Modding/blender-io-xbg/blob/main/AGENTS.md) — format notes in AGENTS.md
