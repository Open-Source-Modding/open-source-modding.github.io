# WDL .phys Working Notes (2026-08-19) — SORT LATER

> Dump of excess context during WDL (Legion) collision decode session. Raw
> findings, not yet organized. Everything here was verified against
> `roadres_a0f6d274e1bb32f3.phys` unless marked otherwise.

## Core discovery

**WDL `.phys` (2017.2) = hknpCompressedMeshShape + hkcdStaticMeshTree family**
— the SAME format family as WD2 `.hkx` (which is fully decoded), NOT the
compound shape format used by WD2 `.phys`. So the WD2-hkx decode knowledge
applies to WDL `.phys`; the WD2-`.phys` compound mapping does NOT.

### ✅ VERTEX DECODE VERIFIED (2026-08-19 18:07)

**type 138 = Section** (96B/child):
- AABB min @+16 (3 floats), AABB max @+32 (3 floats)
- codecParms (quant scales) @+60/+64/+68 — e.g. 0.0019, 0.0316, 0.0023
- Stored scales EXACTLY match computed: (amax-amin)/2047 for x,y; /1023 for z
  (3.86/2047=0.0019 ✓, 64.77/2047=0.0316 ✓, 2.39/1023=0.0023 ✓)

**type 140 = shared vertices** (11:11:10 quant, count×4B, u32)
- vert = amin + ((u&0x7FF)*sx, ((u>>11)&0x7FF)*sy, ((u>>22)&0x3FF)*sz)

**VERIFIED OUTPUT (child 0)**: 32 verts → x 17.01-20.44 (3.4m road width),
y -29.33-9.17 (38.5m length), z 4.20-4.54 (flat road surface). REAL ROAD.

**type 160 = primitive data runs**: 4B each = value(u16), index(u8), count(u8);
count field = number of runs (63/75 × 4B = 252→256B / 300→304B, 16B-padded ✓)

**type 116 = per-child wrapper** (256B): 6 fixup slots → 98, 11, 124, 138, 140, 153
**type 19 = 12×16B spheres** (x,y,z,0.5) — RED HERRING for quant scale (was
tried first, gave 1×1×1 patch; 138 is the true Section). Role: maybe child
bounds/transforms — revisit later.
**type 36 = VelocityInfo/transform** (identity + 0.5s), owns 12 fixups → children
**type 98 = 13 items** (560-1904B) = section tree (hkcdStaticTree::DefaultTree5?)
with ±FLT_MAX AABB sentinels; type 108 = 12×144B tree nodes; type 124 = 16B
(first=60?, count=32? — primitives ref?); type 153 = 16B zeros; type 11 = 16B zeros

### UNFINISHED: face/primitive index decode (state as of 18:13)
- **Data runs (160) are a BINARY-SEARCH index** (2018 SDK getPrimitiveData):
  for localIndex, find run where `localIndex - run.m_index` in `[0, run.m_count)`;
  run.m_value = the triangle data (u16). Binary search, then linear for ≤4 runs.
- **Primitive struct** (m_primitives array): 4×u8 vertex indices; triangle if
  idx[2]==idx[3] else quad. decodeVertex: localIndex >= firstSharedIndex →
  shared else packed.
- **Section layout (96B) deduced**: tree base (+16 AABB min, +32 AABB max),
  +48 codecParms[6] = {min.x, min.y, min.z, scale.x, scale.y, scale.z} ✓
  (16.985,-29.334,4.197 + 0.0019,0.0316,0.0023), +76 firstSharedVertex=40?,
  +80 numPackedVertices=32 (matches type140 count!), +84 leafIndex=1?,
  +88 firstDataRunIndex=40?
- **Type 124 (16B) = packed (count<<24)|first refs** per child:
  0x2000003C(32,60), 0x26000002(38,2), 0x01000002(1,2), 0x28000002(40,2),
  0x1000003C(16,60), 0x13000002(19,2), 0x15000002(21,2) — primitive/shared refs?
- **Type 11 large items (96-192B) = s16 delta values** (0xFFD0=-48, 0xFCF4=-780)
  = Codec3Axis5 tree codec data (node deltas), NOT primitive indices
- **Type 98 (13 items) = tree node AABBs** (DefaultTree5): ±FLT_MAX sentinels
  (0x7F7FFFEE/0xFF7F7FFF) + real bounds (2.82/2.59/4.99, 17.73/20.84/20.89,
  -94.2/-93.7/-29.3, 35.44/36.13...) = node storage array
- ⚠️ CAVEAT: my run parse used t160[0] (FIRST item) — must use the fixup-resolved
  per-child 160 (138→160 fixup). Run index must be monotonic per child.
- **NEXT**: (1) resolve per-child 160 via 138→160 fixup, verify run indices
  monotonic; (2) locate m_primitives array (4×u8 idx) — candidates: type 124
  refs into a global primitive array, or inside type 98; (3) decode faces,
  write WDL decoder → OBJ; (4) compare WDL hkx (33758.hkc compendium)
- SectionDecoder source (authoritative):
  `havok/sdk/havok-content-tools-2018/Havok 2018/hk2018_1_0_r1/Source/Geometry/Internal/DataStructures/StaticMeshTree/hkcdStaticMeshTreeDecoder.inl`
  + `hkcdStaticMeshTree.h` (Section/Primitive/PrimitiveDataRun structs)
- PrimitiveDataRun (2013 patch): value(u16 TYPE_INT), index(TYPE_BYTE), count(TYPE_BYTE)

## Type map (compendium-specific — differs from WD2)

| type | count | notes |
|------|-------|-------|
| 0 | 1 | empty |
| 1 | 1 | 192B, mostly zeros, u32=67895300@+32, 3@+36 |
| 11 | 24 | 16B each |
| 19 | 1 | 192B = **12 × 16B per-child bounding spheres (x,y,z, 0.5)** — quant scale source |
| 36 | 1 | 1344B = VelocityInfo/transform (identity + 0.5s), owns 12 fixups → children |
| 86 | 1 | 112B, zeros, owns 2 fixups |
| 98 | 13 | 560–1904B, starts ±FLT_MAX (0x7F800000/0xFF800000) = tree/AABB sentinels |
| 108 | 12 | 144B, mostly zeros; u32 118030340@+32, 3@+36, 60@+48 |
| 116 | 12 | 256B = **per-child wrapper, 6 slots → 98/11/124/138/140/153** |
| 124 | 12 | 16B |
| 138 | 12 | 96B |
| 140 | 12 | **count 32/32/38, bytes 128/128/160 — count×4 EXACT (38×4=152→160B pad) = candidate u32 quant VERTS** |
| 153 | 12 | 16B |
| 160 | 12 | **count 63/63/75, bytes 256/256/304 (63×4=252→256, 75×4=300→304) = candidate PRIMITIVES/indices** |

## TYPE sub-chunks (2017.2 — renamed vs WD2)

- TPTR @0x69c0 — **ALL ZEROS** (mapping moved to TNA1 in 2017.2)
- TNA1 @0x786c — starts `80a30000 010002 0203 0d04 0805 0206 1007 1108 0009 000a 000b 000c 0106 160d 00` (varint-ish, encoding TBD — same rabbit hole as WD2 TNAM)
- TSTR @0x6ee0 — 145 class names: hknpCompoundShape, hknpShapeInstance, hkAabb, hkVector4, hkTransform, **hknpCompoundShapeData, hknpCompoundShapeCdDynamicTree, hknpCompoundShapeSimdTree, hkcdDynamicTree::DefaultTree32, hkcdSimdTree, hkcdFourAabb, hknpCompressedMeshShape, hkBitField, hknpCompressedMeshShapeData, hknpCompressedMeshShapeTree, hkcdStaticMeshTreeBase(+Connectivity/Section/SharedVertices/Primitives/DataRuns), hkcdStaticTree::DefaultTree4/5, Codec3Axis4/5** etc.
- FSTR @0x7ad8 — field names ("instances\0instanceVelocities\0aab...")
- TBDY @0x7f5c — type body descriptors (varint)
- THSH @0x875c — 37×u32 hashes
- TPAD @0x87ec

## Fixup graph

- Targets by type: {11:24, 19:1, 36:1, 86:1, 98:13, 108:12, 116:12, 124:12, 138:12, 140:12, 153:12, 160:12} — every type reachable
- LOC owners: {1:2, 36:12, 86:2, 108:24, 116:72, 138:12}
- type 116 = per-child wrapper: 6 fixup slots → 98, 11, 124, 138, 140, 153

## Type 19 layout (per-child bounding spheres)

192B = 12 × 16B entries. Each: center (x, y, z) + 0.5:
`(32.26,-123.47,-2.38,0.5), (32.22,-123.47,10.78,0.5), (30.64,-123.40,9.62,0.5), (9.90,-122.61,9.62,0.5), (8.28,-122.55,-2.38,0.5), (8.32,-122.55,10.78,0.5), (37.87,6.10,-2.38,0.5), (37.83,6.10,10.78,0.5), (36.25,6.19,9.62,0.5), (15.52,7.22,9.62,0.5), (13.90,7.29,-2.38,0.5), (13.94,7.29,10.78,0.5)`

12 children, radius constant 0.5. This is the scale/offset source for quant decode (analogous to WD2 hkx type-45 AABB@+0x20/+0x30 and WD2 .phys shape72 AABB@+0x20/+0x30).

## Type 36 (VelocityInfo)

1344B, count=12. Starts: 1.0, 0.0, 0.0, 0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0 — identity matrix + 0.5s. Owns 12 fixups → 12 children.

## Next steps (unfinished)

1. Test type 140 as 11:11:10 quant verts (x[10:0], y[21:11], z[31:22]; scale from type-19 sphere or AABB from type 98)
2. Test type 160 as primitives (quads? u32 index words?)
3. Decode TNA1 to confirm type→class mapping (or skip — class names already known from TSTR order)
4. Confirm AABB source (type 98 tree sentinels? type 19 sphere ±0.5?)
5. Compare with WDL `.hkx` collision and WD2 `.hkx` decode paths (33/45/73/75/37 graph)

## Related

- WD2 ground truth: `watch_dogs/wd2-tag0-collision-format.md` (type 13 = verts, type 101 = quads count×4, 174 shapes/4247 verts/3050 quads/0 bad/0 degenerate)
- WD2 decoders: `/tmp/opencode/wd2_phys_decode.py`, `/tmp/opencode/wd2_decode.py`
- FrankMK04 parser: `~/Documents/Code/re/Disrupt/HavokDisruptWD2/HkxParserD.cs`
- WDL compendium: `~/Documents/Modding/WDL/unpacked/worlds/london/london/generated/physics/33758.hkc`