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
  → reads script via iSystem->GetIPak()->FOpen  (PAK archive)
  → prepends generation-mask #defines
      sprintf("#define %s 0x%I64x", m_ParamName, m_Mask)   // SShaderGen bitmask
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

## Open (FC6-specific, not in FC1)

- FC6 `sarb` wrapper around `DXBC` in `shadersobj.dat` — the blob does not
  validate as standard DXBC (garbage header). FC1 has no equivalent; this is
  a Disrupt/FC6-era packaging detail.
- FC6 ships **no shader source** (unlike WD1's leaked `shaders.dat`).
  Only `d3d12/shadersobj.dat` + `.fat` exist; the binary string dump
  references `%backend%shadersobj.dat` + `patchshadersobj.dat` only.