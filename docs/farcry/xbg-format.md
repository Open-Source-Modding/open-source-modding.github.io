# Far Cry XBG Format Notes (FC5 vs FC6)

Cross-engine status: Far Cry's XBG shares the Dunia mesh lineage with Disrupt
(Watch Dogs). Disrupt = Dunia 2 (FC3) fork, so the Disrupt XBG reference applies
as the working hypothesis — **verified against real FC5/FC6 binaries here.**

## Key lineage difference: magic

| Engine/game | XBG magic | Reversed = |
|-------------|-----------|-----------|
| Disrupt WDL | `MOEG` | `GEOM` |
| **Far Cry 5** | `HSEMG` | `GMESH` |
| **Far Cry 6** | `HSEMI` | `IMESH` |

FC wraps geometry as `GMESH` (FC5) / `IMESH` (FC6), not Disrupt's `GEOM`. Same
Dunia lineage, different wrapper magic per title.

## Chunk chain (both FC5 & FC6 — reverse-order 4CCs)

```
HSEMG (FC5) / HSEMI (FC6) header
LTMR material ref (graphics_materials\<X>.material.bin)
LEKS = SKEL skeleton
EDON = NODE
KSRP submesh/bounding region
DIKS = SKID
DNKS = SKND
SULC = CLUS cluster / bone refs
ITOM = MOTI
SDOL = LODS bulk mesh data (majority of file)
```

- 4CC chunks are **markers, not length-prefixed** (u32 after each 4CC is `0x1`).
- Chunk names stored reversed (`SDOL` = "LODS").

## LODS (SDOL) header

```
u32[0] 0x4c4f4453 "SDOL"
u32[1] 0x1
u32[2] payload size
u32[3] payload size + 0x14
u32[4] 0
u32[5] 0x1
u32[6] FC5: 0x1 / FC6: 0x0
u32[7] float bounding (FC5: 288.0 / FC6: 50.0)
...
```

Parsed with `blender-io-dunia/modules/Avatar/sdol_avatar.py` `parse_sdol()`:
header `unk_0, unk_1, lod_count`; per-LOD `lod_dist, vb_count`, vb_info
`{flags, stride, unk, offset}`=16B, submeshes 28B, then 16B-aligned vert + index sections.

### FC5 (`PANEL_SIGN_22.xbg`)
```
unk_0=0x4082c unk_1=0 lod_count=1
[extra=1] lod_dist=288.0 vb_count=1
 vb[0]: flags=0xc7a stride=40 unk=5735 offset=0
sm_count=1: {vb_idx=0 lod_grp=0 sub_idx=0 idx_off=0 vert_mark=5734 ...}
```
→ **FC5 vertex stride = 40 bytes** (pos 12 + uv 8 + normal + color …).

### FC6 (`new_fc6_model.xbg`)
```
unk_0=0x24835 unk_1=0 lod_count=1
[extra=0] lod_dist=50.0 vb_count=1
 vb[0]: (FC5 field order) flags=0 vertsize=0xb340(45888) buffsize=0x1c020(114720) offset=0x1c020(114720)
 more descriptor words: 0,0,0,0x2b(43),0x1c020(114720),0x04050607,0x5c010203
```
→ FC6 SDOL descriptor **RESOLVED**: the "extra" fields decode onto the XeNTaX SDOL
20-byte struct (John76, t=24572): `dword lod | dword unk1(null) | float somefloat |
dword unk2 | dword unk3(null) | dword vertsize | dword buffsize(=vertsize+normals+uv)
| dword buffsize1`. Observed: **45888 = vertsize** (position bytes → 45888/6 = **7648
verts**, packed i16 XZY); **114720 = buffsize = vertsize + 68832 (normals+uv)**;
**114720 − 16 = 114736 = r1 region size**; **43 (0x2b) = unk2**; second **114720 =
buffsize1**. FC1's `MESH_CHUNK_DESC` stores the same geometry as **counts**
(`nVerts/nTVerts/nFaces`); FC6 stores **byte-sizes** (`vertsize/buffsize`).

## Confirmed FC5-vs-FC6 structural divergence

| | FC5 | FC6 |
|---|-----|-----|
| LODS offset | 0x646 | 0x6ed |
| submesh countdown `07..01` | 1 hit (header) | **2** hits (header + 0x1c07c) |
| data regions | 1 | **2**: r1=vertex (0x4c..0x1c07c=114736B), r2=sequential u16 index buffer |
| record packing | interspersed with state/header bytes | densely packed 8-byte records |

FC6's `0x1c020` (114720) = vertex-section size, matching region r1 (114736 =
114720 + 16B align). FC6 region r2 = sequential u16 index buffer
(`00 00 01 00 02 00 03 00 ...`).

## Vertex data is quantized / packed — NOT float32

Positions are **quantized 16-bit** (`i16/32768 * bbox_ext + bbox_min` — see
`disrupt/xbg-format-geparser-raw.md`), which is why naive
float-array scanning finds nothing clean on either FC5 or FC6 files. Compiler
channel emission order (GeomParser): Positions → FaceIndices → Normals →
Tangents → UV channels (up to 7, 2×f32) → VertexColors → VertexIllumination →
BoneWeights → BoneIDs → SmoothingGroups → MaterialIDs.

## FC6 shader blobs: LZ4-compressed DXBC-variant containers (RESOLVED Sep 2026)

**The `sarb` wrapper TODO is resolved.** FC6 `shadersobj.dat` records are
**LZ4-compressed** DXBC-variant containers. Verified end-to-end across 12+
records against the real FC6 install + `shadersobj.fat`:

- **FAT flag=2 = LZ4** (53,568 of 53,613 entries are flag=2; CompressionScheme
 map from FCBConverter: None=0/LZO1x=1/LZ4=2). Each entry's `comp`/`unc` sizes
 drive the decompress: `lz4.block.decompress(dat[off:off+comp], unc)` succeeds
 for every record, output length == `unc`.
- **On-disk record** = raw LZ4 block. The `sarb` string seen at every record
 offset is the LZ4 literal-run echo of the decompressed header, not a separate
 framing layer. (First token `f4 15` = literal-length run copying the `sarb...`
 header verbatim.)
- **Decompressed record layout**: `sarb` magic + u32 ver(=1) + u32 size1 +
 u32 size2 + `DXBC` + **16-byte checksum** + DXBC header (ver=1, total=size1,
 chunk count=8) + 8 chunk offsets (relative to the DXBC magic) + chunks +
 trailing resource-name metadata (length = size2, holds binding names like
 `LinearDepthTexture`, `GeometricEdgeBuffer`, `EdgeDetectParams`).
- **Chunk set (8)**: `SFI0`(shader flags) `ISG1`(input sig v1) `OSG1`(output
 sig v1) `PSV0`(pipeline-state validation, 140B) `RTS0`(render-target state,
 868B) `ILDN`(input-layout name, 44B) `HASH`(20B) `DXIL`(**the SM6 bytecode**).
- **DXIL container validates**: magic `DXIL`, version `0x00010000`, part
 count 16, part-table offset 0xbdc.

**Consequence — the WDL decompile route DOES apply to FC6**: extract the DXIL
chunk → `dxil-spirv` → `spirv-cross` → HLSL. The earlier "not standard DXBC"
conclusion was wrong: it was the **16-byte checksum between the DXBC magic and
the header** that broke naive parsing (it read the checksum as
version/size/count). The `<technique>` XML render-state blocks in
`shadersobj.dat` provide the exact per-shader blend/cull/depth/input-layout
state alongside each DXIL blob.

## Caveats
- `fc5model.txt`/`fc6model.txt` in the Dunia RE repo are Discord-paste-mangled
 hex dumps (UTF-8 corruption: `£F`≈FF, `o0`≈00) — not parseable; the `.xbg`
 binaries are authoritative.
- Open: FC6 shader decompile end-to-end validation (extract DXIL → dxil-spirv
 → spirv-cross) — the container is now confirmed LZ4+DXBC-variant, so this
 should work; FC6 shader input layout (ISGN/ISG1) for true vertex-format
 confirmation (SDOL descriptor now resolved — see above).
## Havok version (FC6) — RESOLVED Sep 2026

FC6 uses **Havok 2017.2.0** (same as Watch Dogs Legion). Verified against the
real FC6 install:

- Havok is statically linked into `FC_m64d3d12.dll` (no standalone Havok DLLs
 in `bin/`). Source paths embedded in the binary: `t:\tetra\main\external\havok\source\...`
 (tetra = FC6 codename) with `Physics2012` module dir + `tagfile2014`
 serializer (`hktagfilereadformat2014.cpp`), plus `hknp` (Havok New Physics,
 2017+) symbols. The `Havok libs built with version [ ]` string is
 blank — version stripped in Ubisoft's custom fork.
- Extracted Havok objects (LZ4-decompressed entries of `worlds/fctworlds.dat`,
 e.g. `hkpWorld` / `hkRootLevelContainer`, 14-16KB) start with the binary
 **`TAG0`** tagfile magic followed by the SDK version string
 **`SDKV20170200`** = SDK V2017.02.00.
- Series evolution: FC5/New Dawn = `hk_2014.1.0-r1` (community-verified, EVE
 11/2021); **FC6 = 2017.2.0**. FC3 = Havok 2010, FC4 = Havok 2012 (timsan99,
 XeNTaX-era threads).
- Note: FC6 Havok objects do NOT start with the XML `<hk` magic — they are the
 binary `TAG0` tagfile format (the XML form only appears in hkxpack-style
 exports). Signature for future extraction: `TAG0` + `SDKV20170200` near
 offset 8.
