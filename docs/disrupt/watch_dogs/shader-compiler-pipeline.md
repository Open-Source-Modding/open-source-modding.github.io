# Watch Dogs 1 / Legion — Shader Compiler Pipeline & Shader Registry

> **Sources**: Ubisoft leak (`leak/ubisoft/`), Disrupt-Shader-Compiler RE, WDL `obj_editor/` analysis (2026-09-12, updated 2026-09-28).
> Documents how Ubisoft's ShaderCompiler2 actually produces `shadersobj.dat`, the
> `FastInitData` shader registry, and the compiled shader format — the ground truth
> for recompiling/modding Disrupt engine shaders.

## Correction chain: what a shipped shader file starts with

Container-format claims changed three times. The history, so it stays auditable:

1. **Early claim (original doc)**: Ubisoft shaders have a custom container, an extra
   header before the `DXBC` magic.
2. **2026-09-12**: "Extra header claim WRONG, both samples start with plain `DXBC`
   at offset 0." The sample was wrong: one file was our own DXC compiler output
   (`pixel_05439780.pso`), the other a leak `obj_editor` file
   (`pixel_4271c18a7feae380.pso`). Neither came from a retail `shadersobj` archive.
3. **2026-09-27 (current, retail-verified)**: shipped `.pso`/`.vso`/`.cso` begin with
   a short **`.header` stub**, then the `DXBC` magic (observed at offset 4 or 12). A
   300-file random sample of retail unpacked `shadersobj` contained **zero** files
   with `DXBC` at offset 0.

The stub is reproducible: per-file sibling `<name>.pso.header` stubs ship with the
reconstruction, and the build script prepends them. **Without the prefix the engine
will not load the shader.** The `.crc` file (2 bytes) is a separate verifier, not
part of the stub. Open question: the leak's WDL `obj_editor` files do start with
plain `DXBC` at offset 0, so whether WDL retail archives carry the stub is untested.

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

- **`.header` stub + DXBC**: files begin with a short stub, `DXBC` follows at
  offset 4 or 12 (never offset 0 in the 300-file retail sample, 2026-09-27; see
  the correction chain above). 64-bit ID in WDL filename: `pixel_<16hex>.pso`,
  32-bit in WD1: `pixel_<8hex>.pso`.
- **Bucket rule**: shaders are grouped into `obj/h00..h7f` where
 **`bucket = shaderID_low_byte & 0x7F`** (verified 100% on 2388 WDL shaders).
 Bit 7 of the low byte = free flag (shader-type or variant bit, ~50% set).
- **`.dep` file** (`dependencies_<id>.dep`): XML `<ShaderFileChecksums>` listing
 every `#include`d source file + a **uint64 content Checksum**.
 - **The Checksum = plain FNV-1 64 of the raw (CRLF) file bytes** (VERIFIED:
 `fnv164(DepthShadow.inc.fx raw)` = `0xb8d6781ddfedf051` = 13318965017701118033
 exactly matches the .dep). NOT FNV-1a, NOT masked, NOT a path hash.
- **`.crc` file**: 2-byte shader verifier, separate from the `.header` stub. The
  4-byte `engine/shaders/shaders.crc` is a different file: the engine reads it from
  two mounts and compares the values, so it is a cross-mount consistency check, not
  a content hash of the shader set. Retail and the working community mod TFOWC2
  ship byte-identical values, so a recompile does not need to regenerate it.
- **`.header` stub** (WD1 DSC): first u32 = content version (observed 0-9), then
  version-dependent fields; reconstruction stubs measure 4, 8, 12, 16, 20, 24, or
  40 bytes (4 and 12 dominate), while the 300-file retail sample only showed `DXBC`
  at offset 4 or 12 (open question: whether the larger stub sizes ship in retail).
  NOT the shader ID (u32[1] ≠ filename id). These are
  prepended to compiled output — see below.

## FastInitData_editor.bin — the REQUIRED shader registry

`leak/ubisoft/data_win64/engine/shaders/FastInitData_editor.bin` (3.5MB, nbCF v3) is
the AUTHORITATIVE shader registry — required for shader modding.

```
[nbCF v3 header] RootId 0x67974467
[family base-ID block] 143 u64s (file 0x30..0x4A8): 0x03,0x05,0x06,0x09,0x0B..0xE6
 each 0xNN_00000000000000 = a shader FAMILY base ID
[shader member block] 444k u64s (from 0x4A8): 0x54_0000..., 0x58_...01...
 each family's member shader IDs (low bits = defines + type flag)
```

- **shader top byte (`sid>>56`) = family ID** (240 distinct among 2385 shipped WDL shaders).
- To MOD a shader: its ID must fall in the family's range AND be registered in
 FastInitData's membership, or the engine won't enumerate/load it. A mod coordinates:
 recompiled `.pso` in `shadersobj.dat` + shader IDs + FastInitData membership.

## The Disrupt-Shader-Compiler project (WD1 recompile)

`Disrupt-Shader-Compiler/` (git repo) reconstructs the WD1 shader DB:
- `Shader_Compile_Command_Sorted.txt` (39,131 lines): 39,128 retail fxc command
  lines, one per permutation (the exact set Ubisoft compiled; 23357 ps_5_0 pixel,
  15649 vs_5_0 vertex, 101 cs_5_0, 21 gs_5_0), plus 3 research-only RT test lines
  (`rt_probes.fx`, `rt_probes_radiance.fx`, `rt_shadow_ao.fx`).
- `CompileShaders.py` — original (Windows fxc). **Includes a `prepend_header()` step**
 that prepends the shipped `.header` stub onto each compiled output — the engine-compat
 bridge. **`compile_shaders_linux.py` — the Linux port (DXC)**; must ALSO prepend headers.
 Which compiler backend actually produces output the game accepts is settled in the
 recompilation section below.
- The `.pso.header` stubs in `obj/` = exactly the shipped set (39,128 stubs, 1:1 with commands).

### Recompilation: what actually works (verified 2026-09-28)

- **All 39,131 command lines compile** (100%) using the shipped fxc command lines
  against Microsoft `d3dcompiler` DLLs through a D3DCompile wrapper. The command
  list plus d3dcompiler is sufficient; no Ubisoft tool is needed to compile.
- **DXC (modern) fails**: it promotes `ps_5_0`/`vs_5_0` to shader model 6 / DXIL
  regardless of flags, raises semantic errors on the reconstruction's struct I/O,
  and the D3D11 runtime rejects the output. Only fxc-era d3dcompiler produces
  valid retail-style DXBC.
- **Semantic strictness is uniform across era compilers** (d3dcompiler_43 / _46 /
  _47): a bare struct member in an entry-function parameter is rejected with
  `error X3502: 'MainPS': input parameter 'x' missing semantics`. Non-standard but
  explicit semantics compile: `float2 uv : uv;` works, and the compiler preserves
  the name verbatim instead of renaming it to TEXCOORD.
- Version fingerprints embedded in the DXBC (readable with `strings`): retail
  = `HLSL Shader Compiler 9.29.952.3111` (fxc 2010), working community mod
  TFOWC2 = 10.1 (`d3dcompiler_47`). The game accepts both. Modern DXC output is
  rejected.

### Semantic system (the reconstruction gap, dated)

The DSC source tree is a **reconstruction**. As found (2026-09-12), many struct
members and bare entry returns lacked semantics, and era compilers reject both
cases: `E5004: missing a semantic` for returns, `X3502: 'MainPS': input parameter
'x' missing semantics` for bare struct members (see the strictness notes above).
Examples from that state:
- `bbox.fx:23` returned `float4(1,1,1,1)` with no return semantic (the file in the
  tree now carries `: SV_Target`).
- Shipped bbox shader ground truth: PS ISGN=`SV_Position`, OSGN=`SV_Target`.

As of 2026-09-28 the tree compiles 100% of command lines against d3dcompiler, so
the missing semantics have been filled in. Compiling is solved; matching retail's
signatures is the remaining problem (next sections).

The semantic layer uses **custom semantic names + macros**:
- `GlobalSemantics.inc.fx`: `ATTR0=position, ATTR1=normal, ATTR2=color0, ATTR3=color1,
 ATTR4=blendweight, ATTR5=blendindices, ATTR6-15=texcoord0-9`.
- `CustomSemantics.inc.fx`: `CS_Position=ATTR0`, etc.
- `Profile.inc.fx` (PC): `SEMANTIC_VAR(var) var : var`, `SEMANTIC_OUTPUT(semantic) : semantic`,
 `VS_TARGET=vs_3_0`, `PS_TARGET=ps_3_0`, cbuffer gating for SHADERMODEL>=40.
- These custom names map to real DXBC semantics at compile time via the shader
 signature tables (`SMeshVertex`, `SVertexToPixel`).

### SEMANTIC_VAR and what shipped builds actually used

`Profile.inc.fx` defines the macro twice:

```
#ifdef INTERPOLATOR_PACKING
    #define SEMANTIC_VAR(var) var
    #define SEMANTIC_OUTPUT(semantic)
#else
    #define SEMANTIC_VAR(var) var : var
    #define SEMANTIC_OUTPUT(semantic) : semantic
#endif
```

- `INTERPOLATOR_PACKING` appears only as that `#ifdef`. Zero occurrences across all
  39,131 compile command lines, so no build ever defined it: shipped compiles used
  `var : var` (variable-name semantics).
- That matches the working community mod TFOWC2, which ships variable-name
  semantics (`uv`, `color`, `animColor` from the `SEMANTIC_VAR(var) → var : var`
  expansion) and works in-game. It does not match retail's TEXCOORD numbering.
- **The leak's source is not byte-identical to retail's shipped source.** The leak's
  `SVertexToPixel` declares a bare `float2 pixelCoords`, which no era compiler
  accepts, and its `MainPS` declares a `: VPOS` parameter that retail's shipped
  ISGN lacks for the same permutation.

**Implication**: compiling is solved, signature matching is not. Derive the
engine-bound sides (VS input, PS output) from the shipped `.pso` DXBC ISGN/OSGN,
which is ground truth per permutation, and keep the internal sides consistent
between VS and PS. Which side must do what is the subject of the next section.

## The signature contract (ISGN/OSGN): what actually breaks a recompile

DXBC carries two signatures per shader: an input signature (ISGN) and an output
signature (OSGN). Diffing retail shaders, the working community mod TFOWC2, and our
broken recompiles produced this contract:

- **Vertex-shader INPUT and pixel-shader OUTPUT are engine-bound.** They must match
  retail byte-for-byte because C++ code maps them to vertex declarations and render
  targets. Measured: TFOWC2 matches retail on 15643/15649 vso inputs and
  23355/23357 pso outputs.
- **Vertex-shader OUTPUT ↔ pixel-shader INPUT are internal.** They only need to
  agree with each other. TFOWC2 ships variable-name semantics on this side
  (`uv`, `color`, `animColor`) and works fine in-game.
- **Retail's numbering is per-permutation active-member order**: the first active
  struct member gets TEXCOORD0, the second TEXCOORD1, and so on, produced by
  retail's 2010 compiler (`HLSL Shader Compiler 9.29.952.3111`). No static source
  annotation can reproduce it, because the active member set changes with each
  permutation's `#define`s.
- **A static per-file TEXCOORD renumbering looks correct but breaks linkage.**
  Wrong indices on the engine-bound sides → VS/PS linkage mismatch → black screen
  with audio. Verified by elimination (see triage below).
- **The engine pairs VS with PS via `fastinitdata.bin`**, not via source file.
  Same-source pairing heuristics mislabel even retail (about 19k naive "mismatches"
  that are subset-safe in practice), so same-source pair counts are not a
  broken/working signal.

### Black-screen triage (2026-09-28)

Order that isolated the fault:

1. Full recompile pack → black screen.
2. Added missing retail-only file classes → still black.
3. Retail vertex shaders + recompiled pixel shaders → still black (pixel-side
   signatures implicated).
4. Retail content through the identical pack pipeline → **works**, exonerating the
   `.fat`/`.nfo`/`.dat` format.
5. Working mod's shader set inspected → the signature contract above.

**Current status**: the identified fix is a signature-matching recompile
(variable-name semantics plus the d3dcompiler_47 recipe, mirroring TFOWC2). It is
**not yet verified in-game**.

## WDL vs WD1 shader differences

| Aspect | WD1 | WDL (Legion) |
|--------|-----|-------------|
| Filename | `pixel_<8hex>.pso` | `pixel_<16hex>.pso` (full 64-bit TShaderID) |
| Bucket | `obj/h00-h7f` | same `id_low_byte & 0x7F` |
| Source | `shaders.dat` (full source shipped) | leak `data/engine/shaders/` (942 fx) |
| Registry | FastInitData (nbCF) | same |
| File prefix | `.header` stub (4/12 bytes) before `DXBC`, retail-verified | sampled leak `obj_editor` files start with plain `DXBC`; retail WDL untested (open) |
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
- WDL shader dictionary: `WDL/wdl_shader_dictionary.md`
- Unpacked WD1 shaders: retail `shadersobj` unpack (~23,359 `.pso` under
  `engine/shaders/obj/`)

## Open items

- [ ] Verify the signature-matching recompile in-game (variable-name semantics +
 d3dcompiler_47 recipe, mirroring TFOWC2). Identified fix, not yet tested.
- [ ] Build the signature-derivation tool: map each shader ID → shipped .pso DXBC
 ISGN/OSGN → auto-inject into the reconstructed source. Must reproduce retail
 numbering on the engine-bound sides (VS input, PS output).
- [ ] Decode the FastInitData shader-member low-56-bit encoding (define-variation +
 bit-7 type flag) to generate valid member IDs.
- [ ] Map the 143 FastInitData family base-IDs → family NAMES (correlate top-byte
 with .dep meta/<family>.fx in canonical order).
- [ ] ComputeShaderID full formula remains uncracked (obfuscated exports); the
 shipped .pso + FastInitData registry sidestep it.