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

```
[header] u32BE @0 = 12 (version/type) u32BE @4 = 119329 (count)
[ID block] 119,329 × u64BE shader IDs @0x08 .. 0xe9108 (sorted ascending)
[string block] per-entry @0xe9110..end: 1-byte length + name
```

- **ID block**: sparse (not dense): `1,2,3,6,7,11,12,13,14,15,16,17,25,27,31,32,33,35,40,48...`
 then large hash-like values. These are the actual shader IDs, sorted.
- **String block**: 1-byte length prefix (verified: `0x0c`→`aaedgedetect`,
 `0x0b`→`blendshapes`, `0x18`→`UPDATE_PREVIOUS_POSITION`, `0x09`→`USE_BAKED`,
 `0x0c`→`HAS_BINORMAL`, `0x13`→`IGNORE_BLEND_SHAPES`, `0x0f`→`SKINNING_8BONES`,
 `0x0b`→`BAKING_MODE`). ~4939 ASCII runs; ends with `AllOk`.
- **Name types**: shader/param names (`aaedgedetect`, `blendshapes`,
 `renormalization`, `boxparticle`) + shader compile defines
 (`UPDATE_PREVIOUS_POSITION`, `USE_BAKED`, `SKINNING_8BONES`, `BAKING_MODE`,
 `LIGHT_TYPE_DIRECTIONAL/OMNI/SPOT`, `DEBUG_SKIP_DENOISING`, ...).
- **Role**: family + define registry. To mod FC6 shaders, a shader's ID must be
 registered here or the engine won't enumerate/load it (same as WDL).

**WDL vs FC6 FastInitData:**

| Aspect | WDL | FC6 (d3d12) |
|--------|-----|-------------|
| Endianness | little | **big** |
| Header | `nbCF` magic v3 | u32BE=12 + u32BE=count |
| Family keying | `0xNN_00000000000000` base IDs | sparse u64 IDs + name strings |
| Names | none in file | 1-byte-len-prefixed strings |
| Entry count | ~444k members | 119,329 |

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