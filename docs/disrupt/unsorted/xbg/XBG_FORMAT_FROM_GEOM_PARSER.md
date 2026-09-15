# XBG Format Specification (from GeomParser_r64.dll Decompile)

**Source**: Hex-Rays decompile of `GeomParser_r64.dll` (128,018 lines) — the actual XBG compiler ("DuniaGeometryDeserializer" = XBG WRITER, name inverted from editor perspective).

**Reference Importer**: `import_wdl_xbg.py` (823 lines) — READ ONLY, works on real files.

---

## 1. Header Layout

| Offset | Size | Type | Meaning | Source |
|--------|------|------|---------|--------|
| 0x00 | 4 | char[4] | Magic "MOEG" | Importer line 112 |
| 0x04 | 2 | u16 | Version major (0x95 for WDL) | Importer line 113, 394 |
| 0x06 | 2 | u16 | Version minor (0x46 for WDL) | Importer line 113, 395 |
| 0x08 | 16 | — | Unknown/reserved | Importer line 403 |
| 0x18 | 4 | u32 | unk_count | Importer line 404 |
| 0x1C | 4 | u32 | odd_flag | Importer line 405 |
| 0x20 | var | — | Odd tables (if odd_flag > 0) | Importer lines 118-122 |
| — | 76 | — | Unknown/reserved | Importer line 410 |
| — | 4 | u32 | lod_count | Importer line 412 |
| — | 8×lod | u32[2] | LOD data pairs | Importer lines 414-416 |

**Note**: The compiler's `ParseFileBuffer` (sub_1800B3960, ~line 99372) deserializes the asset via `GXAsset::Asset::Deserialize` (line 99515) but does NOT write the MOEG header — that is likely written by `EditorExporter_r64.dll` which consumes GeomParser.

---

## 2. Hierarchy Structure (ParseGeometryAssetHierarchy)

**Function**: `DuniaGeometryDeserializer::ParseGeometryAssetHierarchy` (sub_1800B4B90, ~line 100095)

The hierarchy walk:
1. Reads node `type` (line 100218), `export_tag` (line 100238), `export_name` (line 100258)
2. Filters node types: BREAKABLE, FACADE, BASIC_ANIMATED, DYNAMIC_ANIMATED, LOFTSHAPE (lines 100298-100317)
3. Checks for LOD/OCM suffixes (lines 100374-100379, 100378-100379)
4. For geometry nodes: calls `sub_18000650A` (line 100614) → `DeserializePolyMesh`
5. For skeleton nodes: calls `SkeletonNodeToMeshSkeleton` (line 113732)
6. For breakable: calls `BreakableNodeHierarchyToBoneHierarchy` (line 47886)

**Mesh list written per-LOD** via `GXData::WriteBuffer` (lines 100425-100426, 100514-100515):
- `WriteBuffer` vtable offset 16 = GetData pointer
- `WriteBuffer` vtable offset 24 = GetSize

---

## 3. PolyMesh Write Order (DeserializePolyMesh)

**Function**: `DuniaGeometryDeserializer::DeserializePolyMesh` (sub_180076890, ~line 63022)

### Field-by-field emission order (from decompile trace):

| Order | Field | Channel Name | Type/Precision | Decompile Lines | Notes |
|-------|-------|--------------|----------------|-----------------|-------|
| 1 | Vertex Count | — | u32 (from `NumElements`) | 64187, 64191 | Position channel element count |
| 2 | Positions | "Positions" | 3×i16 (quantized) | 64196-64242 | Transformed by node hierarchy matrix; quantized: `i16 / 32768 * bbox_ext + bbox_min` |
| 3 | Face Indices | "PolyFacesIndices" | u32 (tri-list) | 64580-64777 | 3 indices per face; written per-face with vertex links |
| 4 | Normals | "Triangulation"/"TriangulationIndex" | 3×f32 (computed) | 64595-64770 | Computed from face vertices via cross product; normalized |
| 5 | Tangents | "Triangulation" (channel 0) | 4×f32 | 64595-64770 | Computed per-face, stored in vertex link struct (+64 to +75) |
| 6 | UV Channels (0-6) | "TexCoords" | 2×f32 per channel | 65033-65371 | Up to 7 UV channels; loop j=0..6 (line 64906); channel index = `v252 - 1` |
| 7 | Vertex Colors | "VertexColors" | 4×u8 (normalized) | 65448-65589 | RGBA u8 → f32 / 255.0 |
| 8 | Vertex Illumination | "VertexIllumination" | 3×f32 | 65372-65466 | Only if valid alongside VertexColors |
| 9 | Bone Weights | "BoneWeights" | 4×f32 (normalized) | 64252-64568 | Max 8 bones/vertex; sorted by weight descending; weights sum to 1.0 |
| 10 | Bone Indices | "BoneIDs" | 4×u16 | 64252-64568 | Indices into skeleton bone array |
| 11 | Smoothing Groups | "SmoothingGroups" | u32 | 64024 | From off_180118080[0] |
| 12 | Material IDs | "MaterialIDs" | u32 | 64024 | From off_180118080[1] |
| 13 | Custom Faces | "CustomFaces" | — | 64024 | From off_180118080[2] |
| 14 | Vertex Links | — | struct (312 bytes) | 64806-64899 | Per-vertex face adjacency (max 64 faces/vert) |
| 15 | Sub-parts | — | — | 114242 (SplitMeshSubParts) | Called after main mesh |
| 16 | Transform | — | 4×4 matrix | 64077-64185 | Node world transform + parent hierarchy |

### Key struct offsets in GamExMesh (v61, allocated at line 63832, size 0xC0=192):
| Offset | Field | Size |
|--------|-------|------|
| +0x08 | Position buffer ptr | 8 |
| +0x10 | Position count | 4 |
| +0x14 | Position stride | 4 |
| +0x18 | Normal/Tangent buffer | 8 |
| +0x24 | Face index buffer | 8 |
| +0x2C | Face count | 4 |
| +0x30 | UV buffers (7×) | 8×7 |
| +0x38 | UV counts | 4×7 |
| +0x56 | Vertex color buffer | 8 |
| +0x5E | Skin data (weights/indices) | inline in vertex links |
| +0x72 | Vertex link buffer | 8 |
| +0x90 | Sub-mesh parts | 8 |
| +0x98 | Material refs | 8 |
| +0xB0 | Name string | 64 |
| +0xF0 | LOD index | 4 |

---

## 4. Discrepancy Analysis: Compiler Write vs Importer Read

| # | Aspect | Compiler (GeomParser) | Importer (import_wdl_xbg.py) | Status |
|---|--------|----------------------|------------------------------|--------|
| 1 | Header magic | NOT written by GeomParser | Expects "MOEG" at offset 0 | **Gap** — EditorExporter writes header |
| 2 | Vertex positions | Quantized i16 in local space | Reads quantized i16, applies bbox | **Match** (importer lines 585-588) |
| 3 | Face indices | Tri-list, 3×u32 per face | Reads via quad-split index buffer decode | **Partial** — importer has `_try_decode_faces` (line 594) |
| 4 | Normals | Computed per-face, stored in vertex links | NOT directly read — reconstructed from faces | **Gap** — importer doesn't read explicit normals |
| 5 | Tangents | Computed per-face (4×f32) | NOT read | **Gap** |
| 6 | UV channels | Up to 7, 2×f32 each | Reads UVFlag (params[3]) but decode unclear | **Partial** — importer reads UV data but mapping unclear |
| 7 | Bone weights/indices | 4×f32 + 4×u16, sorted by weight | NOT read — .skel file used instead | **Gap** — importer uses companion .skel |
| 8 | Vertex colors | 4×u8 RGBA | NOT read | **Gap** |
| 9 | Vertex illumination | 3×f32 | NOT read | **Gap** |
| 10 | Smoothing groups | u32 per face | NOT read | **Gap** |
| 11 | Material IDs | u32 per face | Read via mesh descriptor `matID` (params[2]) | **Partial match** |
| 12 | Sub-parts / LOD | SplitMeshSubParts called | LOD handled via lod_count + mesh lists | **Different approach** |
| 13 | Skeleton | SkeletonNodeToMeshSkeleton | Reads separate .skel file | **Different file** |
| 14 | Transform hierarchy | Applied to positions during compile | NOT applied — uses bbox for position decode | **Gap** |

---

## 5. Bone/Skeleton Mapping

**Function**: `DuniaGeometryDeserializer::SkeletonNodeToMeshSkeleton` (line 113732)

- Maps skeleton hierarchy nodes to mesh bone indices
- Called from `ParseGeometryAssetHierarchy` for animated nodes (BASIC_ANIMATED, DYNAMIC_ANIMATED)
- Bone IDs from "BoneIDs" channel are indices into this skeleton
- Companion .skel file holds actual bone transforms/names (not in .xbg)

---

## 6. Collision (.col) Generation

**Search results in GeomParser_r64.dll.c**:
- No `CPhys`, `hkpBvCompressedMeshShape`, `.col.hkx` emission found
- Only Collada parser references: `CColladaParser::FindCollisionMeshIDs`, `COLLISION_PRIMITIVE_LIST` (GLM secondary motion)
- `CGamExUtils::gamExSerializeOcc` (line 120249) serializes occlusion mesh (`GXOccMesh`), NOT physics collision

**Conclusion**: GeomParser does **NOT** emit retail-compatible `.col.hkx`. EncryptedStudios confirmed: "the cols it makes is unpacked so wont work for retail — which is weird however im fixing it".

---

## 7. Channel Name Reference (from off_1801180xx arrays)

| Array | Index 0 | Full List |
|-------|---------|-----------|
| off_180118038 | "Positions" | Positions, VertexColors, VertexIllumination, TexCoords, Normals, BoneIDs, BoneWeights, BlendTargets, BlendTarget, SmoothingGroups, MaterialIDs, CustomFaces |
| off_180118020 | "PolyFacesIndices" | PolyFacesIndices, Triangulation, TriangulationIndex, Positions, VertexColors, VertexIllumination, TexCoords, Normals, BoneIDs, BoneWeights, BlendTargets, BlendTarget, SmoothingGroups, MaterialIDs, CustomFaces |
| off_180118028 | "Triangulation" | Triangulation, TriangulationIndex, Positions, VertexColors, VertexIllumination, TexCoords, Normals, BoneIDs, BoneWeights, BlendTargets, BlendTarget, SmoothingGroups, MaterialIDs, CustomFaces |
| off_180118030 | "TriangulationIndex" | TriangulationIndex, Positions, VertexColors, VertexIllumination, TexCoords, Normals, BoneIDs, BoneWeights, BlendTargets, BlendTarget, SmoothingGroups, MaterialIDs, CustomFaces |
| off_180118060 | "BoneIDs" | BoneIDs, BoneWeights, BlendTargets, BlendTarget, SmoothingGroups, MaterialIDs, CustomFaces |
| off_180118068 | "BoneWeights" | BoneWeights, BlendTargets, BlendTarget, SmoothingGroups, MaterialIDs, CustomFaces |
| off_180118080 | "SmoothingGroups" | SmoothingGroups, MaterialIDs, CustomFaces |

---

## Summary

The GeomParser decompile reveals the **full compiler-side mesh data preparation** (quantization, transform application, normal/tangent computation, skin sorting). The actual **XBG binary serialization** (MOEG header, mesh descriptor lists, index buffers, position buffers) is handled downstream — likely in `EditorExporter_r64.dll` which consumes the GeomParser output via `WriteBuffer`.

**Critical gaps for importer enhancement**:
1. Explicit normal/tangent reading (currently reconstructed)
2. UV channel mapping (UVFlag in mesh params needs decoding)
3. Skin data in .xbg vs .skel split
4. Vertex color/illumination support
5. Smoothing groups for hard edges