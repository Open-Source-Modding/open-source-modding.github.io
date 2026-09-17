# FC6 XBG Format — Reverse-Engineered Specification

## Technology
- Engine: Dunia (Far Cry 6, codename "tetra")
- Binary format: HSEM chunk-based (same lineage as FC2-FC5)
- Version: `0x00130049` (u32 at file offset 4)
- Verified against: 70,837/94,589 unpacked XBG files from real FC6 install

## File Header (32 bytes)

```
+ 0: u32  HSEM magic (0x4853454D)
+ 4: u32  version (0x00130049)
+ 8: u32  hash1 (zero observed)
+12: u32  hash2 (zero observed)
+16: u32  unknown (zero observed)
+20: u32  filesize_minus_12
+24: u32  reserved (zero)
+28: u32  n_chunks (UNRELIABLE — walk by fourcc+size instead)
+32: first chunk
```

## Chunk Structure

Each chunk: `4B name + 4B version + 4B ck_size + 4B ck_dsize + 4B reserved` (20B header).
`ck_size` includes the 20B header. Payload = ck_size - 20.

**Chunk order** (17 chunks observed):
```
LTMR → LEKS → EDON → MB2O → KSRP → DIKS → DNKS → ITOM → SDOL
→ DHRM → ZNRM → XOBB → HPSB → FIKS → SDPD → [PMCP] → [PMCU]
```

PMCP/PMCU may appear as trailing chunks after SDPD (not inline in the chain).

## Chunk Contents

### LTMR — Material References
```
u32 n_materials
u32 unknown
per material:
  u32 length, string path (e.g. "graphics\_materials\X.material.bin")
  u32 length, string name (e.g. "Vaas_head2")
```

### EDON — Node/Bone Hierarchy
```
u32 n_bones
per bone:
  i32 parent_id (-1 = root)
  f32[3] translation
  f32[4] quaternion (xyzw)
  u32 bone_id
  u32 length, string name
```
Uses same layout as FC5 `_read_edon()`.

### DNKS — Skin Cluster Data
Contains SULC sub-chunk(s) for per-vertex palette indices, plus optional
face-based weight clusters (buffer flag 0x8000) for vehicles.

### SDOL — Mesh Geometry (bulk of file)

**Identical format to FC2-FC5** (verified: Noesis FC2/3 plugin, FC4 Blender
importer by Szkaradek123, FC6 binary analysis).

```
u32 lod_count
per LOD:
  f32 lod_distance
  u32 vb_count
  per VB (16B):
    u32 flags        — vertex format bitmask (D3D9-style, see below)
    u32 stride       — bytes per vertex
    u32 vcount       — number of vertices
    u32 offset       — byte offset within LOD vertex data
  u32 sm_count       — submesh count
  per submesh (28B):
    u32[7] — vb_index, material_index, sub_entry_index,
             idx_start, last_vert, vert_byte_off, reserved
  u32 vb_size        — total vertex data bytes
  align(16)
  vertex_data        — vcount × stride bytes per VB, concatenated
  align(16)
  u32 index_count
  align(16)
  index_data          — index_count × u16 indices
```

### PMCP — Position Compression
```
f32 unknown
f32 pos_scale     — typically 1/16384 (baked into vertex decode)
```

### PMCU — UV Compression
```
f32 uv_offset
f32 uv_scale
```

## Vertex Format Flags (bitmask in VB descriptor `flags` field)

Same flag system as FC2-FC5 (Gibbed Dunia Tools / Avatar module):

```
Bit   Hex      Name        Size  Description
0     0x0001   POS_FLOAT   12B   Position: float32 × 3
1     0x0002   POS_INT16    8B   Position: int16 × 4 (÷16383.5, most common)
2     0x0004   POS_HALF     8B   Position: float16 × 4
3     0x0008   UV0          4B   UV channel 0: int16 × 2
4     0x0010   BONE_WTS1    8B   Bone weights: u8 × 4 + u8 × 4 (indices)
5     0x0020   BONE_WTS2    8B   Bone weights set 2 (rare, >4 influences)
6     0x0040   NORMAL       4B   Normal: R10G10B10A2 (FC5/FC6) or D3DCOLOR
7     0x0080   COLOR        4B   Vertex color: D3DCOLOR (BGRA)
8     0x0100   TANGENT      4B   Tangent: R10G10B10A2 or D3DCOLOR
9     0x0200   BINORMAL     4B   Binormal: R10G10B10A2 or D3DCOLOR
10    0x0400   UNK_400      4B   Unknown (sometimes stores tangent)
11    0x0800   UV1          4B   UV channel 1: int16 × 2
12    0x1000   UV2          4B   UV channel 2: int16 × 2
```

### Known format values
- `0x0BDA` (3018) = 32B static: pos + 2 UVs + normal + color + fog + psize
- `0x0BEA` (3034) = 40B skinned: pos + 2 UVs + blendweight + blendindices + normal + color + fog + psize
- `0x0BCA` = 28B: pos + 2 UVs + normal + color (no fog/psize)
- `0x0c7a` = 40B: pos + 2 UVs + tangent + normal + color + fog + psize

### Vertex component decode (per stride)

| Stride | Layout |
|--------|--------|
| 24 | pos_i16(8) + uv0_i16(4) + normal(4) + color(4) + unk(4) |
| 28 | pos_i16(8) + uv0_i16(4) + uv1_i16(4) + normal(4) + color(4) + unk(4) |
| 32 | pos_i16(8) + uv0_i16(4) + uv1_i16(4) + normal(4) + color(4) + fog(4) + psize(4) |
| 36 | pos_i16(8) + uv0_i16(4) + uv1_i16(4) + unk(4) + normal(4) + color(4) + fog(4) + psize(4) |
| 40 | pos_i16(8) + uv0_i16(4) + uv1_i16(4) + blendw(4) + blendi(4) + normal(4) + color(4) + fog(4) + psize(4) |

### Position decode
```
x = posTrans + i16_x × posScale    (posTrans=0, posScale from PMCP, typically 1/16384)
y = posTrans + i16_y × posScale
z = posTrans + i16_z × posScale
```
XZY order (not XYZ).

### UV decode
```
u = uvTrans + i16_u × uvScale      (uvTrans=0, uvScale from PMCU, typically 1/16384)
v = uvTrans + i16_v × uvScale
```

### Normal decode (FC5/FC6)
R10G10B10A2_UNORM: 3 × 10-bit unsigned, normalized to [-1,1] + 2-bit A.
```python
u = struct.unpack_from('<I', data, offset)[0]
x = (u & 0x3FF) / 1023.0 * 2.0 - 1.0
y = ((u >> 10) & 0x3FF) / 1023.0 * 2.0 - 1.0
z = ((u >> 20) & 0x3FF) / 1023.0 * 2.0 - 1.0
```

### Index buffer
```
index_count × u16, 16-byte aligned.
Face winding: reversed from file (game uses opposite front-face winding).
```

## FC6 Vertex Element Types (DX12-style, from engine binary)

FC6 also supports a DX12-style vertex element system (separate from the
bitmask flags). Used by the shader system:

**Types**: Float1-4, UByte4/UByte4N, Short2/4/Short2N/Short4N,
UShort2N/UShort4N, UInt1, Float_16_2/4, UNorm_10_10_10_2

**Semantics** (CS_ in binary, S_ in shadersobj.dat XML):
CS_Position, CS_Normal, CS_Tangent, CS_Binormal, CS_TangentSign,
CS_TexCoord, CS_Color, CS_BlendWeights, CS_BlendIndices

## Sources
- Noesis plugin: github.com/alrusdi/noesis_farcry (FC2/3)
- FC4 Blender importer: Blender249[FarCry4] by Szkaradek123
- Avatar SDOL parser: blender-io-dunia/modules/Avatar/sdol_avatar.py
- Vertex flags: Gibbed Dunia Tools + blender-io-dunia VertexFlags class
- FC6 types/semantics: FC6 engine binary string dump (`FC6 Strings.csv`)
- XBG format notes: docs/farcry/xbg-format.md
- Vertex buffer formats: docs/farcry/vertex-buffer-formats.md
