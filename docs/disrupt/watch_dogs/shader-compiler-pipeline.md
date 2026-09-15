# Watch Dogs 1 / Legion — Shader Compiler Pipeline & Shader Registry

> **Sources**: Ubisoft leak (`leak/ubisoft/`), Disrupt-Shader-Compiler RE, WDL `obj_editor/` analysis (2026-09-12).
> Documents how Ubisoft's ShaderCompiler2 actually produces `shadersobj.dat`, the
> `FastInitData` shader registry, and the compiled shader format — the ground truth
> for recompiling/modding Disrupt engine shaders.

## Correction to earlier doc

`shader-editing-workflow.md` says "Ubisoft shaders have an extra header before the
DXBC magic." **Verified 2026-09-12: that is WRONG.** Both WD1 compiled output
(`pixel_05439780.pso`, DXC) and WDL shipped shaders (`pixel_4271c18a7feae380.pso`)
start with plain `DXBC` magic immediately. The `DXBC` header is standard: magic(4) +
checksum(16) + version + length + chunk count. The `.crc` file (2 bytes) is a separate
shader-verifier, not a header.

## The compiler pipeline (ShaderCompiler2)

Ubisoft's toolchain (from the leak `bin/`, decompiled `ubi_shader_compiler2/`):

```
ShaderGenerator2 --GenerateAllVariations()
    -> ComputeShaderID(name, defines[]) -> TShaderID<uint64>
    -> CShaderCache -> CPlatformShaderCompiler
    -> CBackEndD3D11 / D3D12 -> ByteCodeCompiler (d3dcompiler_47.dll)
    -> obj/pixel_<64bitid>.pso
```

- **TShaderID** = uint64: HIGH bits = shader family, LOW bits = define-variation.
- **ComputeShaderID** mangled signature (recovered from ShaderGenerator2 import table):
  `?ComputeShaderID@@YA?AU?$TShaderID@_K@@AEAVCShaderHandlerManager@@AEAV?$ndStringBase@DUndStringAllocator@@@@AEBV?$ndVector@USDefi...`
  = `ComputeShaderID(CShaderHandlerManager&, ndStringBase<char,ndStringAllocator>&, ndVector<SDefine>&) -> TShaderID<u64>`.
  Imported from `ShaderCompilerUtils_r64.dll`.
- **Build flow (PC)** (`comp_shadersids.bat`): `ShaderGenerator2 operation=Generate
  shaderidsfile=shaderids.txt` → `prepareplatformdata64.exe -shaders` → collect
  `obj/*.pso` → `FileArchiver archive=shadersobj.dat`.
- **ShaderCompilerUtils_r64.dll is obfuscated**: export dir base=1, nfuncs=526,
  nnames=0 (ordinal-only), and `AddressOfFunctions RVA = 0x20e` (bogus/corrupted).
  Cannot resolve ComputeShaderID's code address via the PE export dir. The base
  string hash is FNV-1 64 (`0xcbf29ce484222325` offset + `0x100000001b3` prime,
  `NomadDefaultHashFunctor<ndStringBase>`).

## Compiled shader format (.pso/.vso/.cso/.gso)

- **Plain DXBC** (no extra header). 64-bit ID in WDL filename: `pixel_<16hex>.pso`,
  32-bit in WD1: `pixel_<8hex>.pso`.
- **Bucket rule**: shaders are grouped into `obj/h00..h7f` where
  **`bucket = shaderID_low_byte & 0x7F`** (verified 100% on 2388 WDL shaders).
  Bit 7 of the low byte = free flag (shader-type or variant bit, ~50% set).
- **`.dep` file** (`dependencies_<id>.dep`): XML `<ShaderFileChecksums>` listing
  every `#include`d source file + a **uint64 content Checksum**.
  - **The Checksum = plain FNV-1 64 of the raw (CRLF) file bytes** (VERIFIED:
    `fnv164(DepthShadow.inc.fx raw)` = `0xb8d6781ddfedf051` = 13318965017701118033
    exactly matches the .dep). NOT FNV-1a, NOT masked, NOT a path hash.
- **`.crc` file**: 2-byte shader verifier.
- **`.header` stub** (WD1 DSC): 12 bytes `02 000000 + hash + hash` (version + two
  u32 hashes); some are longer. NOT the shader ID (u32[1] ≠ filename id). Some are
  just `00000000` (4 bytes). These are prepended to compiled output — see below.

## FastInitData_editor.bin — the REQUIRED shader registry

`leak/ubisoft/data_win64/engine/shaders/FastInitData_editor.bin` (3.5MB, nbCF v3) is
the AUTHORITATIVE shader registry — required for shader modding.

```
[nbCF v3 header]       RootId 0x67974467
[family base-ID block] 143 u64s (file 0x30..0x4A8): 0x03,0x05,0x06,0x09,0x0B..0xE6
                       each 0xNN_00000000000000 = a shader FAMILY base ID
[shader member block]  444k u64s (from 0x4A8): 0x54_0000..., 0x58_...01...
                       each family's member shader IDs (low bits = defines + type flag)
```

- **shader top byte (`sid>>56`) = family ID** (240 distinct among 2385 shipped WDL shaders).
- To MOD a shader: its ID must fall in the family's range AND be registered in
  FastInitData's membership, or the engine won't enumerate/load it. A mod coordinates:
  recompiled `.pso` in `shadersobj.dat` + shader IDs + FastInitData membership.

## The Disrupt-Shader-Compiler project (WD1 recompile)

`Disrupt-Shader-Compiler/` (git repo) reconstructs the WD1 shader DB:
- `Shader_Compile_Command_Sorted.txt` — 39,128 fxc command lines, one per permutation
  (the exact set Ubisoft compiled; 23357 ps_5_0 pixel, 15649 vs_5_0 vertex,
  101 cs_5_0, 21 gs_5_0).
- `CompileShaders.py` — original (Windows fxc). **Includes a `prepend_header()` step**
  that prepends the shipped `.header` stub onto each compiled output — the engine-compat
  bridge. **`compile_shaders_linux.py` — the Linux port (DXC)**; must ALSO prepend headers.
- The `.pso.header` stubs in `obj/` = exactly the shipped set (39,128 stubs, 1:1 with commands).

### Semantic system (the reconstruction gap)

The DSC source tree is a **reconstruction** and is **semantic-incomplete**: many struct
members and bare entry returns lack semantics, and **neither fxc nor DXC accepts them**
(both give `E5004: missing a semantic` / "Semantic must be defined"). Verified:
- `bbox.fx:23` `float4 MainPS(...) { return float4(1,1,1,1); }` (no return semantic).
- Shipped bbox shader ground truth: PS ISGN=`SV_Position`, OSGN=`SV_Target`.

The semantic layer uses **custom semantic names + macros**:
- `GlobalSemantics.inc.fx`: `ATTR0=position, ATTR1=normal, ATTR2=color0, ATTR3=color1,
  ATTR4=blendweight, ATTR5=blendindices, ATTR6-15=texcoord0-9`.
- `CustomSemantics.inc.fx`: `CS_Position=ATTR0`, etc.
- `Profile.inc.fx` (PC): `SEMANTIC_VAR(var) var : var`, `SEMANTIC_OUTPUT(semantic) : semantic`,
  `VS_TARGET=vs_3_0`, `PS_TARGET=ps_3_0`, cbuffer gating for SHADERMODEL>=40.
- These custom names map to real DXBC semantics at compile time via the shader
  signature tables (`SMeshVertex`, `SVertexToPixel`).

**Implication**: to recompile the reconstructed source, you must either (a) restore
the missing semantics (167 files, guessing-risk), (b) use fxc (same strictness — no
help), or (c) **derive semantics from the shipped `.pso` DXBC ISGN/OSGN** (ground truth,
correct — the recommended path). The shipped `.pso` encode the exact I/O signature per
permutation.

## WDL vs WD1 shader differences

| Aspect | WD1 | WDL (Legion) |
|--------|-----|-------------|
| Filename | `pixel_<8hex>.pso` | `pixel_<16hex>.pso` (full 64-bit TShaderID) |
| Bucket | `obj/h00-h7f` | same `id_low_byte & 0x7F` |
| Source | `shaders.dat` (full source shipped) | leak `data/engine/shaders/` (942 fx) |
| Registry | FastInitData (nbCF) | same |
| Extra header | none (plain DXBC) | none (plain DXBC) |
| .dep checksum | — | FNV-1 64 of raw CRLF bytes |

## Key tooling locations

- Leak compiler binaries: `leak/ubisoft/bin/` (`ShaderCompilerUtils_r64.dll`,
  `ShaderGenerator2_r64.dll` in `toolframework/plugins/`, `ToolLauncher_r64.exe`,
  `PreparePlatformData64.exe`, `ShaderCompiler2.exe`)
- Leak d3dcompiler: `leak/ubisoft/bin/FXC/PC/d3dcompiler_47.dll` (4.4MB, exact Ubi build)
- WDL source: `leak/ubisoft/data/engine/shaders/` (942 fx)
- WDL compiled: `leak/ubisoft/data_win64/engine/shaders/obj_editor/` (2388 shaders)
- WDL FastInitData: `leak/ubisoft/data_win64/engine/shaders/FastInitData_editor.bin`
- WD1 reconstruction: `Disrupt-Shader-Compiler/` (git repo)
- WDL shader dictionary: `WDL/wdl_shader_dictionary.md` + `/tmp/wdl_shader_dict.json`
- Unpacked WD1 shaders: `/tmp/shadersobj_unpack/engine/shaders/obj/` (23,359 .pso)

## Open items

- [ ] Build the semantic-derivation tool (option 3): map each shader ID → shipped
      .pso DXBC ISGN/OSGN → auto-inject into the reconstructed source.
- [ ] Decode the FastInitData shader-member low-56-bit encoding (define-variation +
      bit-7 type flag) to generate valid member IDs.
- [ ] Map the 143 FastInitData family base-IDs → family NAMES (correlate top-byte
      with .dep meta/<family>.fx in canonical order).
- [ ] ComputeShaderID full formula remains uncracked (obfuscated exports); the
      shipped .pso + FastInitData registry sidestep it.