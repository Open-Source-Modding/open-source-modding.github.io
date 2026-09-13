# Far Cry geometry format lineage — FC1 `.cgf` chunked file → FC6 `.xbg`

Traced from the actual FC1 source tree (`re/Ubisoft/Dunia/FarCry-1-CryENGINE1`,
leaked FC 1.34, buildable VS2022 x64). FC1 stores 3D geometry in a **chunked
`.cgf`** format whose architecture is the direct ancestor of the FC5/FC6 `.xbg`
chunk chain.

## Engine line

```
CryENGINE1 (FC1, .cgf chunks) → Dunia (FC2) → Dunia 2 (FC3) → Disrupt/Dunia (WD + FC5/6, .xbg chunks)
```

## FC1 chunked geometry format

Defined in `CryCommon/CryHeaders.h` + `ResourceCompiler/CryChunkedFile.{cpp,h}`
and `ResourceCompiler/ChunkFileReader.{cpp,h}`.

### Chunk type enum

```cpp
enum ChunkTypes {
    ChunkType_ANY = 0,
    ChunkType_Mesh = 0xCCCC0000,
    ChunkType_Helper,
    ChunkType_VertAnim,
    ChunkType_BoneAnim,
    ChunkType_GeomNameList,      // obsolete
    ChunkType_BoneNameList,
    ChunkType_MtlList,           // obsolete
    ChunkType_MRM,               // obsolete
    ChunkType_SceneProps,
    ChunkType_Light,
    ChunkType_PatchMesh,         // not implemented
    ChunkType_Node,
    ChunkType_Mtl,
    ChunkType_Controller,
    ChunkType_Timing,
    ChunkType_BoneMesh,
    ChunkType_BoneLightBinding,
    ChunkType_MeshMorphTarget,
    ChunkType_BoneInitialPos,
    ChunkType_SourceInfo
};
```

### Chunk header (versioned)

```cpp
struct CHUNK_HEADER_0623 {           // VERSION = 0x0623
    ChunkTypes  ChunkType;
    int         ChunkVersion;
    int         FileOffset;
};
struct CHUNK_HEADER_0744 {           // VERSION = 0x0744, adds ChunkID
    ChunkTypes  ChunkType;
    int         ChunkVersion;
    int         FileOffset;
    int         ChunkID;
};
typedef CHUNK_HEADER_0744 CHUNK_HEADER;
```

Each chunk's desc struct is independently version-tagged, e.g.
`TIMING_CHUNK_DESC_0918 { enum {VERSION = 0x0918}; CHUNK_HEADER chdr; ... }`.

### Chunk read dispatch

`CryChunkedFile::addChunk*` methods (`addChunkTiming/Node/Light/Mesh/BoneMesh/
Material/BoneNameList/BoneAnim/BoneInitialPos/MeshMorphTarget/BoneLightBinding/
SceneProps`) dispatch on `CHUNK_HEADER.ChunkType`, and the reader throws
`CryChunkedFile::Error` on truncated/unknown chunks.

## Resource Compiler (the offline toolchain ancestor)

- `ResourceCompiler/` — `CgfConvertor`-adjacent plumbing: `CryChunkedFile`,
  `ChunkFileReader`, `CryBoneDesc`, `CryBoneHierarchyLoader`, `CgfUtils`,
  `FileMapping`, `CfgFile`, `ExtensionManager`.
- `ResourceCompilerPC/` — the x64 converter: `CgfConvertor.cpp`,
  `CrySkinBase`/`Builder`/`BasisBuilder`/`Full`/`Morph` (plus
  `CrySkinAMD64.asm`), `Controller`, `CryAnimationInfo`, `BoneLightBindInfo`.
- `ResourceCompilerImage/` — `ImageCompiler`, `ImageObject`, `dds.h`,
  `neuquant` (DDS/quantized texture compression).

This is the origin of Disrupt's `ShaderCompiler2`/`ResourceCompiler`
architecture — the offline compile pipeline whose outputs the FC6
`shadersobj.dat` + `.xbg` files carry.

## Archive system: `.pak` in FC1 (not `.dat`/`.fat`)

FC1's VFS is **CryPak** (`CrySystem/CryPak.cpp`, `ICryPak.h`) reading `.pak`
directory-file archives. There is **no `.dat`/`.fat` hashed BigFile** in FC1 —
that store appears later in the Dunia line (FC2+). So the archive layout in
`AGENTS.md` (`FAT2`/BigFile v11) is a Dunia/Disrupt-era design, not FC1-origin.
The `.pak` directory-file model is the conceptual ancestor of the `.dat`/`.fat`
split (manifest + data), but the hashed key scheme is newer.

## Mapping FC1 chunks → FC6 XBG chunks

| FC1 `.cgf` chunk | FC6 XBG chunk (4CC) | Role |
|---|---|---|
| `ChunkType_Mesh` | `SULC` (CLUS) | cluster / submesh geometry |
| `ChunkType_Node` | `EDON` (NODE) | node hierarchy |
| `ChunkType_Mtl` | `LTMR` (material ref) | material list |
| `ChunkType_BoneNameList` | `DNKS` (SKND) | bone names / skin data |
| `ChunkType_BoneAnim` | `ITOM` (MOTI) | animation |
| `ChunkType_BoneMesh` / `MeshMorphTarget` | `KSRP` / `DIKS` (SKID) | skinning / morph |
| `ChunkType_SourceInfo` | header `HSEMI`/`HSEMG` | file metadata |

Same architecture — type-tagged, versioned chunk headers with IDs, read via a
chunk-file reader with per-type dispatch. Only the magic changed: FC1 uses an
int `ChunkType` enum in `.cgf`; FC6 uses 4CC markers in `.xbg`.

## Open items

- FC6 `.xbg` SDOL descriptor extra-field semantics (the field-level decode the
  repo still tracks) — FC1's `MESH_CHUNK_DESC`/`NODE_CHUNK_DESC` layouts are a
  candidate reference for what those fields mean, worth comparing field-by-field.
- FC6 `sarb` shader wrapper — no FC1/`.cgf` equivalent; a Disrupt/FC6-era
  packaging detail.