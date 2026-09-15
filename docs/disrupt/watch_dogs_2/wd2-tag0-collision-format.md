# Watch Dogs 2 TAG0 Collision Format (Havok 2015.1 — Ubisoft-modified / Disrupt)

> Reverse-engineered from WD2 binaries + FrankMK04's `HavokDisruptWD2` parser
> (verified against `galilei_glass_breakable02.hkx` and `roadres_*.phys`).
> **Cross-reference**: generic TAG0/TCM0 chunk format, TCRF compendium system, WDL
> (Havok 2017.2) type catalog → [`../watch_dogs_legion/HKX_FORMAT_REVERSE.md`](../watch_dogs_legion/HKX_FORMAT_REVERSE.md);
> wrapper/header overview → [`../watch_dogs/hkx-format.md`](../watch_dogs/hkx-format.md);
> old WD1 packfile (Havok 2012) → [`../watch_dogs/packfile-format.md`](../watch_dogs/packfile-format.md).
WD2 collision lives in two container kinds, both carrying the same **TAG0**
compendium-style body but with **different type compendiums** (see below):

| File | Magic | Notes |
|------|-------|-------|
| `.hkx` | 16-byte wrapper, TAG0 at +0x10 | Convex polytope shapes (33/147 roots) |
| `.phys` | `SYHP` magic, TAG0 at +0x40 | `hknpCompoundShape` (type 35 root) |

`.phys` parse recipe: `b'\x00'*0x10 + buf[0x40:]` (the 16 zero bytes are
required — feeding `buf[0x40:]` alone yields 0 items).

## Chunk walk (TAG0 body)

Chunks start at offset 0x10 (after the wrapper). Each chunk:

```
+0x00  u32  sizeAndFlags   (size = value & 0x3FFFFFFF, big-endian word)
+0x04  u32  fourcc         (little-endian)
+0x08  byte[size] payload
```

| FourCC | Constant (LE) | Meaning |
|--------|---------------|---------|
| TAG0 | 0x30474154 | Header marker (empty payload, skip) |
| SDKV | 0x564B4453 | ASCII version string, e.g. `20150100` |
| DATA | 0x41544144 | Serialized object data (items live here) |
| TCRF | 0x46524354 | Reference chunk |
| INDX | 0x58444E49 | Index data |
| ITEM | 0x4D455449 | Item table (see below) |
| PTCH | 0x48435450 | Patch/fixup table (see below) |
| TYPE | — | Type compendium (see below) |

## ITEM table

`numItems = (chunkSize - 8) / 12`. Each entry is 12 bytes:

```
+0x00  u32  raw     (type = raw & 0xFFFFFF, flags = raw >> 28)
+0x04  u32  offset  (dataOffset into DATA)
+0x08  u32  count
```

Item payload = `DATA[offset : next_item_offset]` (items are contiguous;
type-0 items are empty placeholders with count 0).

## PTCH (fixup) table

Sequence of fixup groups:

```
+0x00  u32  ptrType
+0x04  u32  count
then count × { u32 location }
```

Each `location` is an offset into DATA. **The fixup target is the u32 read at
that location, and that value is an ITEM INDEX (not a byte offset).**
Resolve: `target = u32le(DATA, location)` → item `items[target]`.

Verified on `roadres_aac25fa5ac1b95dd.phys`: 1770 fixups, all resolve —
every item is reachable, no dangling targets.

## Shape graph (fixup walk)

### Quantized variant (hkx type 33 root)

```
type 33  hknpConvexPolytopeShape (root)
   └─fixup→ type 45 shapeData        AABB min @ +0x20, max @ +0x30  ← GLOBAL bbox
             ├─fixup→ type 73 vertexHeader   (96 B, per-region sub-bboxes)
             ├─fixup→ type 75 indexArray     (quad indices, count = idx_len/4)
             └─fixup→ type 37 vertexArray    (u32 4B quantized, biggest = vert array)
```

### Full-precision variant (hkx type 147 root)

```
type 147 root
   └─fixup→ type 45 shapeData
             ├─fixup→ type 78 planes     (16 B each)
             ├─fixup→ type 87 vertices   (16 B each, hkVector4)
             ├─fixup→ type 13 indices    (1 B each, count = total indices)
             └─fixup→ type 130/131       (aux / connectivity)
```

## Quantized vertex decode (11:11:10)

```
u32 q:
  xq =  q        & 0x7FF      (11 bits)
  yq = (q >> 11) & 0x7FF      (11 bits)
  zq = (q >> 22) & 0x3FF      (10 bits)

scale = (aabbMax - aabbMin) / (2047, 2047, 1023)
vert  = aabbMin + (xq, yq, zq) * scale
```

Global AABB comes from type-45 shapeData `+0x20` (min) / `+0x30` (max) —
**not** from the type-73 header (that holds per-region sub-bboxes).

Faces: quad indices (type 75), `faceCount = idx_len / 4`.

## Type compendiums — hkx vs .phys (CRITICAL)

The two container kinds use **different class compendiums**. FrankMK04's type
IDs are for hkx only.

### hkx compendium (verified matches FrankMK04)

| Type | Class |
|------|-------|
| 33 | hknpConvexPolytopeShape (quantized root) |
| 147 | full-precision root |
| 45 | shapeData (AABB) |
| 73 | vertex header |
| 37 | quantized vertices |
| 75 | quad indices |
| 87 | full-precision vertices |
| 78 | full-precision planes |
| 13 | full-precision indices |

### .phys compendium (verified on roadres)

| Type | Count | Notes |
|------|-------|-------|
| 35 | 1 | **hknpCompoundShape root** (count 174 children) |
| 66 | 174 | hknpShapeInstance (child instances) |
| 72 | 174 | child shape objects (256 B each; AABB@+0x20/+0x30, 6 slots) |
| 62 | 1 | 32 B × 349 |
| 13 | 348 | **vertices** (2 per child: u32 11:11:10 quant, count×4B) |
| 22, 28 | 9 | per-region data |
| 57 | 1 | 64 B |
| 82, 99, 101, 104, 116 | 174 | per-child arrays (101 = u8 face indices ALL bytes; 85 = 224 B count 2; 116 = secondary verts) |

`.phys` = `hknpCompoundShape` of 174 child instances; the child shapes are
**compressed mesh** variants (not convex polytopes). Type IDs do NOT map
1:1 to the hkx table above — resolve via the file's own TYPE compendium.

## TYPE compendium chunk

The TYPE chunk contains nested sub-chunks (same 8-byte header format).
**Sub-chunk names differ from WDL**: WD2 uses TNAM/TBOD where the WDL (Havok
2017.2) self-contained files use TNA1/TBDY — same roles, renamed.

| Sub-chunk | Payload | Contents |
|-----------|---------|----------|
| TPTR | 952 B | 119 × u64 pointers |
| TSTR | 1812 B | Null-terminated class name strings |
| TNAM | 416 B | Type→name index (encoding TBD) |
| FSTR | 1020 B | Field name strings |
| TBOD | 1464 B | Varint-encoded type bodies |
| THSH | 148 B | 37 × u32 hashes |
| TPAD | 0 B | Padding |

Verified class names in TSTR: `hknpCompoundShape`, `hknpCompoundShapeBase`,
`hknpCompositeShape`, `hkFreeListArray`, `hkAabb`, `hknpShapeSignals`,
`hknpShape`, `hknpSparseCompactMap`, `hknpShapeInstance`, `hkArray`,
`hkVector4`, `hkTransform`, `hkFlags`, `hkEnum`, ...

TNAM first byte 0x77 = 119 (TPTR entry count); encoding of the index not yet
solved — values look like interleaved (offset, typeIndex) pairs or varint
deltas. **TBD.**

## .phys child shape layout (shape72, 256 B) — VERIFIED

Child shape object layout (from `roadres_aac25fa5ac1b95dd.phys`, item 179):

```
+0x00  16 B  zeros
+0x10  u32   fixup slot → item type 104 (16 B, zeros)
+0x20  f32×3 AABB min
+0x30  f32×3 AABB max          (same offset convention as hkx type-45!)
+0x40  u32×4 counts            (50, 6, 51, 255)
+0x50  fixup slot → item type 99  (96 B: AABB + u32 ref to item 533)
+0x60  fixup slot → item type 101 (112 B, count 26)
+0x80  fixup slot → item type 13  (16 B, count 2, zeros)
+0xa0  fixup slot → item type 82  (16 B)
+0xb8  fixup slot → item type 85  (224 B, count 2)
```

Fixup graph: compound35 → 174× instance66 → each → 1× shape72 + 1× type13.
Type 99 (shape data) contains an indirect ref (u32 → item 533, type 116,
count 51, 208 B).

**Array contents observed:**

| Type | Size/count | Contents |
|------|-----------|----------|
| 101 | 112 B, count 26 | **8-bit face indices** (verified: `23 24 21 22...`, all < 32) |
| 85  | 224 B, count 2 | `ee ff 7f 7f` repeating = 0x7F7FFFEE — NOT plain u32 quant (would be floats 3.4e38 = NaN-ish); likely hkPackedVector3 / half16 / 7-bit-packed |
| 99  | 96 B, count 1 | AABB + item ref (indirect) |
| 104 | 16 B, count 1 | zeros |
| 82  | 16 B, count 1 | sparse (`2a 80 00 1a...`) |
| 13  | 16 B, count 2 | zeros (per-instance) |

## NEXT STEPS (open)

- Reviewer verification of the .phys decode (face index bounds, vertex sanity)
- TNAM varint encoding (for authoritative class names) — not needed for decode
- Cross-check decoded verts against WDL `decompress_compressed_mesh` output
- Batch-decode more .phys files (roadres_*, building cols) to confirm generality

## Implementation notes

- Script: `/tmp/opencode/wd2_decode.py` — end-to-end decoder (parse → fixup
  walk → AABB → 11:11:10 quant → faces; type-ID overrides for .phys).
- Fixup target resolution must use **item index** semantics — offset-based
  lookup only ever matched type 35 by accident.
- SDKV in WD2 retail = `20150100` (Havok 2015.1.0 as source — Ubisoft modified the SDK, removed several classes, and added custom collision mesh generation). The leaked Havok 2015 SDK can be used as a baseline to diff against Ubisoft's changes. The 2013 SDK binary
  tagfile format (magic 0xCAB00D1E) is a DIFFERENT, older format — not a
  reference for TCM0/TAG0.
- ES (EncryptedStudios) confirmed this compendium/TYPE-section approach is
  what he used to crack WDL collision; he has not written his logic down.

## Status (2026-08-19)

- ✅ **WD2 hkx decode VERIFIED end-to-end** — `/tmp/opencode/wd2_decode.py` on
  `galilei_glass_breakable02.hkx` → 2 quantized shapes (8 verts/6 quads each,
  closed boxes, valid indices 0-7, plausible window-pane AABBs). FrankMK04
  algorithm confirmed working.
- ✅ **WD2 .phys decode COMPLETE (ground truth)** — `/tmp/opencode/wd2_phys_decode.py`
  on `roadres_aac25fa5ac1b95dd.phys` → **all 174 child shapes decoded to OBJ
  (4247 verts, 3050 quads, 0 out-of-range refs, 0 degenerate quads)**. Road
  surface flat at Z≈2.704, spanning a real road patch. Shape 179: 52 verts.
- ✅ **.phys vertex encoding SOLVED (correct mapping, all combos tested)**:
  - **type 13** = 11:11:10 quantized vertices (u32 4 B each; count = vert
    count, EXACT size match: shape179 count 52 × 4 = 208 B ✓; 8×4=32 ✓)
  - **type 101** = u8 face indices — **count = QUAD count, indices = count×4
    bytes** (item payload padded to 16 B: 26 quads = 104 B stored as 112 B)
  - **shape72** = per-child wrapper (AABB @+0x20 min / +0x30 max, 6 fixup
    slots: +0x10→104, +0x50→99, +0x60→101, +0x80→13, +0xa0→82, +0xb8→85)
  - type 116 = secondary vertex array (count field unreliable — 51 vs 52
    words; not used by the index array; role open)
  - Small shapes (1 vert, 0 quads) are legitimate markers/degenerate children
- ⏳ Remaining: Reviewer verification (S2.4.2/S2.4.3), TNAM varint encoding
  still unsolved (not needed for decode).
- ℹ️ File organization within this repo: initial attempt made; further
  organization/linking deferred (noted in repo AGENTS.md).