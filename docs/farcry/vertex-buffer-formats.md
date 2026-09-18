---
title: Vertex Buffer Formats
---

Observed vertex buffer formats in Far Cry (Dunia engine). The format field is a bitset indicating which elements are present.

## Format 138

```
138 <0000000010001010>: 16 bytes
Unknown layout
```

## Format 3018

```
3018 <0000101111001010>: 32 bytes
```

```cpp
D3DVERTEXELEMENT9[] =
{
 {0, 0x00, D3DDECLTYPE_SHORT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
 {0, 0x08, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
 {0, 0x0C, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
 {0, 0x10, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
 {0, 0x14, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
 {0, 0x18, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_FOG, 0},
 {0, 0x1C, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE, 0},
};
```

## Format 3034

```
3034 <0000101111011010>: 40 bytes
```

```cpp
D3DVERTEXELEMENT9[] =
{
 {0, 0x00, D3DDECLTYPE_SHORT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
 {0, 0x08, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
 {0, 0x0C, D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
 {0, 0x10, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0},
 {0, 0x14, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
 {0, 0x18, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
 {0, 0x1C, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
 {0, 0x20, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_FOG, 0},
 {0, 0x24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE, 0},
};
```

## Source

Gibbed's Dunia Tools

---

## FC6 Vertex Element Types (from FC6 engine binary string dump)

FC6 uses DX12-style vertex element descriptions with named types and semantics.

**Format types:**
| Type | Size | Description |
|------|------|-------------|
| `UInt1` | 4B | Unsigned integer |
| `Float1`–`Float4` | 4–16B | Float vectors |
| `UByte4` / `UByte4N` | 4B | Unsigned bytes (raw / normalized [0,1]) |
| `Short2` / `Short4` | 4–8B | Signed 16-bit integers |
| `Short2N` / `Short4N` | 4–8B | Signed 16-bit, normalized [-1,1] |
| `UShort2N` / `UShort4N` | 4–8B | Unsigned 16-bit, normalized [0,1] |
| `Float_16_2` / `Float_16_4` | 4–8B | Half-float |
| `UNorm_10_10_10_2` | 4B | R10G10B10A2 unsigned normalized |

**Semantic names** (CS_ prefix in binary, S_ prefix in shadersobj.dat XML):
`CS_Position`, `CS_Normal`, `CS_Tangent`, `CS_Binormal`, `CS_TangentSign`,
`CS_TexCoord`, `CS_Color`, `CS_BlendWeights`, `CS_BlendIndices`

**Vertex element structure** (from shadersobj.dat):
`S_Position Float3 Index 0 Stream 0 InstanceStepRate`

The FC6 `S_` names in shadersobj.dat correspond to `CS_` names in the binary — both
are DX12-style vertex input layout descriptions.
