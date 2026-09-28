# Far Cry shader system — lineage (FC1 CryENGINE1 → FC6)

How the shader pipeline evolved from Far Cry 1's runtime-preprocessed text
scripts to Far Cry 6's offline-compiled DXBC + `<technique>` XML, traced
through the actual FC1 source tree (`game-tools/Ubisoft/Dunia/FarCry-1-CryENGINE1`,
leaked FC 1.34, buildable VS2022 x64).

## Engine line

```
CryENGINE1 (FC1) → Dunia (FC2) → Dunia 2 (FC3) → Disrupt/Dunia (WD1/2/L + FC4/5/6 + Primal)
```

FC1 = ancestor. When a Dunia/Disrupt shader module looks alien, cross-reference
the FC1 code first (`RenderDll/Common/Shaders/`).

## FC1: runtime-preprocessed TEXT shader scripts

FC1 shaders are **not** precompiled. At load the engine reads a shader script
from the `.pak` archive and preprocesses it at runtime, then compiles via
NVIDIA Cg (`cgCompileProgram`/`cgCreateProgram`) or `D3DXAssembleShader`.

Pipeline (`RenderDll/Common/Shaders/ShaderScript.cpp`):

```
mfScriptForFileName
 → reads script via iSystem->GetIPak()->FOpen (PAK archive)
 → prepends generation-mask #defines
 sprintf("#define %s 0x%I64x", m_ParamName, m_Mask) // SShaderGen bitmask
 → mfScriptPreprocessor
 RemoveCR → RemoveComments → mfPreprCheckIncludes → mfPreprCheckConditions
 → mfPreprCheckMacros
```

Key methods: `mfReloadShaderScript`, `mfRescanScript`, `mfScanScript`,
`mfLoadSubdir`, `mfScriptPreprocessorMask` (uint64 `nMaskGen`).

### Generation masks = ancestor of TShaderID define-variation

`SShaderGen` (`ShaderParse.cpp` `mfCompileShaderGen`) holds a bitmask of
feature flags; each enabled bit emits a `#define <ParamName> 0x<mask>`
prepended to the script. This is the runtime-text ancestor of Disrupt's
**TShaderID<uint64>** scheme (high bits = family, low bits = define-variation).

### Render-state text tokens = ancestor of the `<technique>` XML

`mfCompileRendState` parses these text tokens — which map 1:1 to the
`<AlphaBlendEnable>`, `<ZEnable>`, `<ZFunc>`, `<Stencil>`, `<CullMode>`,
`<DepthMask>`, `<RenderTargetFormat>` elements of FC6's `shadersobj.dat`:

- State: `Blend`, `DepthFunc`, `AlphaFunc`, `NoDepthTest`, `DepthMask`,
 `DepthWrite`, `PolyLine`, `NoColorMask`, `ColorMaskOnlyAlpha/RGB/Color`
- Blend modes: `BlendDiffuseAlpha`, `Modulate`, `Modulate2X/4X`,
 `MultiplyAdd`, `AddSigned`, `SUBTRACT`, `DOTPRODUCT3`, `BumpEnvMap`,
 `MODULATEALPHA_ADDCOLOR`, `MODULATECOLOR_ADDALPHA`,
 `MODULATEINVALPHA_ADDCOLOR`, `MODULATEINVCOLOR_ADDALPHA`, `TFactor`
- Source/dest: `Texture`, `Specular`, `Previous`, `Current`, `Constant`,
 `SelectArg1/2`, `Replace`, `Disable`, `Decal`
- Funcs: `GT0`, `LT128`, `GE128`, `LEqual`, `Equal`, `NoSet`
- Stencil (`mfCompileStencil`): `Func`, `ALWAYS`, `EQUAL`, `GEQUAL`,
 `GREATER`, `LEQUAL`, `LESS`, `NEVER`, `NOTEQUAL`, `Op`

### No binary shader cache in FC1

`CV_r_shaderssave > 2` writes only the *preprocessed text script* to a `.csf`
debug file — there is no compiled-DXBC-on-disk cache. FC1 compiles everything
at runtime.

## Evolution to Disrupt / FC5 / FC6

| | FC1 (CryENGINE1) | Disrupt / FC5 / FC6 |
|---|---|---|
| Shader source | text `.sht`/script in `.pak` | offline HLSL/`.fx` (WD1 shipped it; FC6 does not) |
| Preprocessing | runtime (`mfScriptPreprocessor`) | offline ShaderGenerator2 |
| Compile | runtime Cg / `D3DXAssembleShader` | offline d3dcompiler_47 → DXBC |
| Cache | none (`.csf` text debug only) | `shadersobj.dat` (compiled DXBC + `<technique>` XML) |
| ID scheme | runtime gen-mask `#define` | `TShaderID<uint64>` family/define-variation |
| Render state | text tokens (`Blend`/`DepthFunc`/`Stencil`/…) | `<technique>` XML elements |

The FC6 `<technique>` render-state XML is the **compiled form** of the exact
state keywords FC1's `mfCompileRendState`/`mfCompileStencil` parse.

## FC6 FastInitData (`d3d12/fastinitdata.bin`)

The engine's shader registry — what `shadersobj.dat` must contain (family +
define-variation IDs), decoded 2026-09-14. **Big-endian, string-keyed**
(unlike WDL's FastInitData which is little-endian + u64 base-IDs).

### Format (fully resolved Sep 2026, updated)

```
[header]          u32BE @0 = 12 (version/type)
                  u32BE @4 = 119329 (total TShaderID count)
[ID block]        119,329 × u64BE TShaderIDs @0x08 .. 0xe9108 (sorted ascending)
[6-byte gap]      @0xe9108 .. 0xe910e
[string table]    3 null-terminated ASCII strings @0xe910e .. end
                  ("aaedgedetect" × 2 + binary data)
[per-family]      237 structs, variable sizes (62–2387 bytes, avg 1140)
                  All end with: 0xffffffff 0x000000ff (VCO/VPFO)
```

- **ID block**: sparse (not dense): `1,2,3,6,7,11,12,13,14,15,16,17,25,27,31,32,33,35,40,48...`
  then large values. These are the actual TShaderIDs, sorted.
- **String table**: Only 3 entries (not 10804 as initially thought). The actual
  shader family names and define names are embedded in the per-family structs.
- **Per-family structs**: Variable-size (62–2387 bytes). Layout:
  - Bytes 0-114: header/padding (mostly zeros)
  - Bytes 115-140: family name (u8 length + ASCII, e.g., `0x0b "blendshapes"`)
  - Bytes 140-670: define table + sampler data (scattered entries)
  - Bytes 670+: define names (u8 length + ASCII, e.g., `0x18 "UPDATE_PREVIOUS_POSITION"`)
  - Footer: `0xffffffff 0x000000ff` (ValidCommonOptions/ValidPerFamilyOptions)
  - Define names appear twice (forward + reverse order) in the struct body
- **Define table entries**: Scattered in struct (not at regular intervals), format:
  `(u16 BE bit_position, u16 BE type, u16 BE count, u16 BE padding)`.
  type=4 → affects TShaderID; type=0 → doesn't.
- **Role**: family + define registry. To mod FC6 shaders, a shader's ID must be
  registered here or the engine won't enumerate/load it (same as WDL).

### TShaderID = Bitmask (NOT a hash)

**Critical insight** (resolved Sep 2026): TShaderIDs are **bitmasks**, not hashes.
Each option contributes `1 << BitShift` to the TShaderID. The TShaderID for a
variation is the OR of all active option bits.

```
TShaderID = OR of (1 << BitShift) for each active option
```

**Each game has its own bit→define mapping** — TShaderIDs are NOT portable
between games. FC6 and WD1 use different bit positions for the same defines:

| Define | FC6 bit | WD1 bit |
|--------|---------|---------|
| SHADOW | 0 | 8 |
| DEPTH | 3 | 12 |
| PICKING | 20 | 21 |

### FC6 Common Defines (bits 0-23)

From `commonoptions.xml`, mapped to bit positions by analyzing TShaderID
distribution across all 119,329 entries:

```
bit  0: SHADOW                         (83.8% of IDs)
bit  1: RECEIVE_SPOT_SHADOWS           (82.1%)
bit  2: PROJECTED_TEXTURE              (81.0%)
bit  3: DEPTH                          (82.8%)
bit  4: DEPTH_BACKFACE                 (81.8%)
bit  5: NEARZ                          (16.7%)
bit  6: INSTANCING                     (17.9%)
bit  7: INDIRECT_INSTANCING            (76.6%)
bit  8: VERTEX_CST_INSTANCING          (1.3%)
bit  9: SKINNING                       (3.9%)
bit 10: SKINNING_8BONES                (3.0%)
bit 11: SKINNING_1BONE                 (1.5%)
bit 12: SUN_SHADOW                     (0.1%)
bit 13: LOCAL_LIGHTS                   (44.9%)
bit 14: OUTPUT_GBUFFER                 (2.7%)
bit 15: OUTPUT_MOTION_VECTORS          (2.7%)
bit 16: OUTPUT_TERRAIN_GEOMETRY        (5.5%)
bit 17: OUTPUT_OPACITY                 (10.3%)
bit 18: SOLID_WIREFRAME                (2.1%)
bit 19: WIREFRAME                      (1.0%)
bit 20: PICKING                        (33.6%)
bit 21: OUTPUT_GI_SURFELS              (35.5%)
bit 22: HAS_MATERIAL_MODIFIER          (8.4%)
bit 23: OUTPUT_OFFSCREEN               (5.6%)
```

Bits 24+ are per-family defines (specific to each shader family).

### WDL vs FC6 FastInitData

| Aspect | WDL | FC6 (d3d12) |
|--------|-----|-------------|
| Endianness | little | **big** |
| Header | `nbCF` magic v3 | u32BE=12 + u32BE=count |
| Family keying | `0xNN_00000000000000` base IDs | sparse u64 IDs + name strings |
| Names | none in file | null-terminated ASCII strings |
| Entry count | ~444k members | 119,329 |
| TShaderID scheme | bit-shift based | bit-shift based (different bit positions) |
| Per-family struct | N/A | 522/298/262/258/206/180/178/109/108/90 bytes |
| Per-family options start | bit 32 | bit 24 |
| Common options | bits 8-21 | bits 0-23 |

### TShaderID Structure (detailed)

**TShaderIDs are bitmasks** — `TShaderID = OR of (1 << BitShift)` for each active option.

Two distinct regions:
- **Common options** (bits 0-23): shared across ALL families, same bit positions
- **Per-family options** (bits 24-63): different options in different families, same bit positions but different meanings

**WD1 Common Options (bits 8-21):**

| Bit | Define |
|-----|--------|
| 8 | SHADOW |
| 9 | SHADOW_NOFSM |
| 10 | SHADOW_PARABOLOID |
| 11 | SAMPLE_SHADOW |
| 12 | DEPTH |
| 13 | DITHERING |
| 14 | INSTANCING |
| 15 | INSTANCING_POS_ROT_Z_TRANSFORM |
| 16 | INSTANCING_BUILDINGFACADEANGLES |
| 17 | INSTANCING_MISCDATA |
| 18 | SKINNING |
| 19 | SKINNING_EXTRA |
| 20 | ALPHA_TEST |
| 21 | PICKING |

**WD1 Extended Common (bits 22-31):**

| Bit | Define |
|-----|--------|
| 22 | PARABOLOID_REFLECTION |
| 23 | GBUFFER_WITH_POSTFXMASK |
| 24 | GBUFFER_VELOCITY |
| 25 | GRIDSHADING |
| 26 | AMBIENT |
| 27 | SUN |
| 28 | OMNI |
| 29 | DIRECTIONAL |
| 30 | SPOT |
| 31 | PROJECTED_TEXTURE |

**FC6 Common Options (bits 0-23):**

| Bit | Define | WD1 Bit |
|-----|--------|---------|
| 0 | SHADOW | 8 |
| 1 | RECEIVE_SPOT_SHADOWS | — |
| 2 | PROJECTED_TEXTURE | 31 |
| 3 | DEPTH | 12 |
| 4 | DEPTH_BACKFACE | — |
| 5 | NEARZ | — |
| 6 | INSTANCING | 14 |
| 7 | INDIRECT_INSTANCING | — |
| 8 | VERTEX_CST_INSTANCING | — |
| 9 | SKINNING | 18 |
| 10 | SKINNING_8BONES | — |
| 11 | SKINNING_1BONE | — |
| 12 | SUN_SHADOW | — |
| 13 | LOCAL_LIGHTS | — |
| 14 | OUTPUT_GBUFFER | — |
| 15 | OUTPUT_MOTION_VECTORS | — |
| 16 | OUTPUT_TERRAIN_GEOMETRY | — |
| 17 | OUTPUT_OPACITY | — |
| 18 | SOLID_WIREFRAME | — |
| 19 | WIREFRAME | — |
| 20 | PICKING | 21 |
| 21 | OUTPUT_GI_SURFELS | — |
| 22 | HAS_MATERIAL_MODIFIER | — |
| 23 | OUTPUT_OFFSCREEN | — |

**Per-family options (bits 24-63):**
Each family defines its own options at bits 24+. Bit 32 is the most common (137/178 WD1 families).
Per-family options have different names in different families — bit 32 means STENCILMASK in Clear but DEPTH_SECONDPASS in BlendedCompositing.

**ValidCommonOptions**: u64 mask of bits valid across all families. WD1=0xFFFFFFFFFFF0003F (bits 0-5, 52-63).
Each family has its own ValidCommonOptions mask that extends this with family-specific bits.

**To map a TShaderID to its options:**
1. Identify which family the TShaderID belongs to (from fastinitdata struct ordering)
2. Look up that family's option set (bit→define mapping)
3. Extract active bits from the TShaderID and resolve to define names
4. Use defines to find the corresponding HLSL source files

### Family → HLSL Source Mapping

**shaders.filelist** (774 entries) = actual HLSL source file paths.
**shadersobj.filelist** (53,375 entries) = compiled shader objects (PSO/CSO/VSO/RS).

321/774 (41.5%) of source files match WDL counterparts. The "99% coverage" refers to
compiled shader objects (53,375/53,613 = 99%), not source files.

The HLSL source is from the Disrupt engine — two sources available:

- **WDL (Watch Dogs: Legion, 2020 dev build)** — `ubi_shader_compiler2/shaders/` (271 .inc.fx + 942 .fx + 231 .meta.xml = 1345 files)
  From the leak (`leak/ubisoft/data/engine/shaders/`). More relevant to FC6 (2021).
- **WD1 (Watch Dogs 1, 2014)** — `Disrupt-Shader-Compiler/engine/shaders/` (184 .inc.fx files)
  Older shader compiler version, fewer includes.
- 71 files shared between WD1 and WDL. 131 unique to WDL. 113 unique to WD1.

Key mappings (FC6 family → WD1 HLSL source):

| FC6 Family | HLSL Source |
|------------|-------------|
| BlendShapes | BlendShapes.inc.fx |
| BoxParticle | Particle.inc.fx |
| BrunetonAtmosphereCompute | Atmosphere.inc.fx |
| Clear | ClusteredClear.fx |
| Clouds\VolumetricClouds | VolumetricClouds.fx |
| ConvertColorSpace | Colors.inc.fx |
| Debug\DebugLine | DebugLine.fx |
| Debug\DebugText | DebugText.fx |
| DeferredLighting\* | DeferredLighting.inc.fx |
| DepthCopy | Depth.inc.fx |
| FX_Decal | Decal.inc.fx |
| GBufferClassify | GBuffer.inc.fx |
| GPUParticle | Particle.inc.fx |
| GlobalImpostor | Impostor.inc.fx |
| HDRToSDR | PostEffect_HDRToSDROutput.fx |
| Mesh\* | Mesh.inc.fx (40+ variants) |
| Particle | Particle.inc.fx |
| PostEffect\* | Post.inc.fx / Depth.inc.fx |
| Primitive | PrimitiveProjection.inc.fx |
| Renormalization | Renormalization.inc.fx |
| ResolveGBuffer | GBuffer.inc.fx |
| Shadows\* | Shadow.inc.fx / DeferredShadows.inc.fx |
| Sky\* | Sky.inc.fx / CelestialBody.fx |
| Terrain\* | Terrain.inc.fx |
| TreeSimulation | TreeSimulation.fx |
| VegetationBender | Vegetation.inc.fx |
| VolumetricFog | VolumetricFog.inc.fx |
| Water\* | Water.inc.fx |
| WireframeRenderer | Wireframe.fx |

**Source locations:**
- HLSL source (WDL): `ubi_shader_compiler2/shaders/` (271 .inc.fx + 942 .fx + 231 .meta.xml)
- HLSL source (WD1): `Disrupt-Shader-Compiler/engine/shaders/` (184 .inc.fx)
- FC6 meta: `FC6/unpacked_new/engine/shaders/meta/` (237 meta files)
- FC6 fastinit: `FC6/unpacked_new/engine/shaders/d3d12/fastinitdata.bin`
- WD1 debug XML: `Disrupt-Shader-Compiler/engine/shaders/fastinitdata.bin.debug.xml`

## FC6 DXBC Decompilation Pipeline

**WORKS** — two-step process using Windows binaries via Wine:

```bash
# Step 1: Extract DXBC from sarb container (offset 16)
# Step 2: DXBC → SPIR-V → HLSL
wine ~/Tools/HLSLDecompiler-YYadorigi/dxil-spirv.exe input.dxbc --output output.spv
wine ~/Tools/HLSLDecompiler-YYadorigi/spirv-cross.exe output.spv --hlsl --shader-model 60 > output.hlsl
```

**Tools:** `~/Tools/HLSLDecompiler-YYadorigi/` (dxil-spirv.exe, spirv-cross.exe)
**Performance:** ~4.2 files/min (~10s/file including Wine startup)
**Output:** Valid SM6.0 HLSL with cbuffers, Texture2D, RWBuffer, SamplerState
**Limitation:** Variable names mangled but structure intact

### index.pso Format

- Header: 28 bytes (magic DAEH at offset 4)
- Entries: u64 LE TShaderID + u32 LE filelist_index (12 bytes each, 135,210 total)
- 111,750 TShaderIDs map to valid filelist paths (82.6%)
- 23,460 have out-of-range indices (≥53,375)

## FC6-specific details

- **`sarb` wrapper — RESOLVED Sep 2026**: FC6 `shadersobj.dat` records are
 LZ4-compressed (FAT flag=2) DXBC-variant containers. Decompressed layout:
 `sarb` magic + u32 ver(=1) + u32 size1 + u32 size2 + `DXBC` + **16-byte
 checksum** (this broke naive DXBC parsing) + DXBC header (ver=1, count=8)
 + chunk offsets + 8 chunks: `SFI0`(shader flags) `ISG1`(input sig v1)
 `OSG1`(output sig v1) `PSV0`(pipe state) `RTS0`(resource table)
 `ILDN`(il debug) `HASH` **`DXIL`**(SM6.0+ bytecode). The `sarb` string is
 an LZ4 literal-run echo of the header, not a framing layer.
 Decompilable via dxil-spirv→spirv-cross (WDL route). Full layout in
 [xbg-format.md](xbg-format.md#fc6-shader-blobs-lz4-compressed-dxbc-variant-containers-resolved-sep-2026).
- FC6 ships **no shader source** (unlike WD1's leaked `shaders.dat`).
 Only `d3d12/shadersobj.dat` + `.fat` exist; the binary string dump
 references `%backend%shadersobj.dat` + `patchshadersobj.dat` only.
- **FC6 `<technique>` XML**: 5404 `technique` records in `shadersobj.dat`
 (AlphaBlendEnable, ZEnable/ZFunc, Stencil, CullMode CCW,
 RenderTargetFormat, DepthStencilFormat, VertexElement input layout).
 The XML uses a compressed-string scheme; first shader input layout shows
 `S_Position Float3 Index 0 Stream 0 InstanceStepRate`.