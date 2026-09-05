# Watch Dogs 1 — Shader Editing Workflow

> **Sources**: Discord logs from みる97 (Soaring), Vector4782, Parallellines, VP (2021–2026).
> Documents two practical approaches to editing Disrupt engine shaders.

## Overview

WD1 ships compiled DX11 shaders in `shadersobj.dat` (thousands of `.pso`/`.vso` files). Each shader is a compiled DXBC bytecode blob named by a 32-bit CRC hash (e.g., `pixel_add5cf1b.pso`). You cannot add new shader resources — only modify existing ones.

**Key constraint**: Ubisoft shaders have an extra header before the DXBC magic. CRC verification in the compiled bytecode means hex-editing DXBC directly invalidates the shader.

## Approach A — Direct shadersobj Replacement (みる97)

Simpler workflow, no external tools beyond a hex editor.

### Steps

1. **Unpack** `shadersobj.dat` with `Gibbed.WatchDogs.Unpack` (or Gibbed.Disrupt tools)
2. **Find target shader** by name/hash — the "maze" step, thousands of files
3. **Hex edit**: delete everything before the `DXBC` magic → save as `.dxbc`
4. **Decompile**: `cmd_Decompiler.exe -d file.dxbc` → `.asm`
5. **Edit assembly** — can only modify existing buffers/textures, cannot add new ones
6. **Recompile**: `cmd_Decompiler.exe -a --copy-reflection file.dxbc file.asm` → `.shdr`
7. **Replace** original file (from `DXBC` onward) with new `.shdr` content
8. **Repack** `shadersobj.dat`

### Tool

- **3Dmigoto cmd_Decompiler** 1.3.16 — cross-platform via Wine on Linux
- Works on any DX11 compiled shader (not WD1-specific)

### Pros/Cons

- ✅ No runtime injection needed
- ✅ Simple file replacement
- ❌ Finding the right hash among thousands is tedious
- ❌ No resource copying/creation — only modify what exists

## Approach B — 3Dmigoto Injection (Parallellines)

Most flexible approach. Parallellines uses this for TFOWC2.

### What is 3Dmigoto?

3Dmigoto is a **general-purpose d3d11 shader injector and decompiler** — not game-specific. It hooks the Direct3D 11 API and can intercept, override, copy, and replace shader resources at runtime. Originally developed for SSAO/ssao depth buffer fixes, it evolved into a full shader modding framework.

### How it works

1. Install 3Dmigoto as a d3d11 wrapper (`d3d11.dll` in game directory)
2. Configure shader overrides in `ShaderOverride` sections of `d3dx.ini`
3. At runtime, 3Dmigoto intercepts draw calls and applies your overrides
4. Can copy resources between draw calls, replace shaders, inject textures

### Pros/Cons

- ✅ Works across all WD1 versions (Ubisoft never updated shadersobj — hashes are stable)
- ✅ Can copy resources and create new ones (more flexible than direct replacement)
- ✅ No repacking archives — runtime injection
- ✅ Parallellines uses this for TFOWC2
- ❌ Requires d3d11 wrapper installation
- ❌ More complex configuration

## Hash Format Differences

| Game | Hash Size | Example | Source |
|------|-----------|---------|--------|
| WD1 | 32-bit CRC | `pixel_add5cf1b.pso` | `shadersobj.dat` |
| WD2 | 32-bit CRC | Same format | `shadersobj.dat` |
| WDL | 64-bit CRC | `pixel_xxxxxxxxxxxxxxxx.pso` | `shadersobj.dat` |

## Legion Leak Shader Compiler

The Ubisoft leak included a shader compiler. It **works for WDL** (Legion) shader modding but **not directly for WD1**:

- **Hash mismatch**: WD1 uses 32-bit hash names, WDL uses 64-bit
- **CRC verification**: Compiled DX11 bytecode has integrity checks (`shaders.crc` = 4 bytes) — hex-editing DXBC codes invalidates the shader
- **Extra header**: Ubisoft shaders have a custom header before DXBC magic
- **Needs `fastinitdata.bin`**: With correct format (game-specific)
- **Ubi shaders are "a bit different than normal ones"** — みる97

**Potential**: The compiler tool itself could be RE'd to produce a working shader compiler for WD1. The format differences (hash size, header) are solvable — the core compilation pipeline is the same DX11 backend.

## ShaderCompiler2 Architecture (Leak Decompilation)

The leak included the full shader compiler toolchain. Decompilation reveals the architecture:

### Pipeline

```
ShaderGenerator2 (editor plugin)
    → reads shader families from XML
    → GenerateAllVariations()
    → ComputeShaderID() → TShaderID<uint64>
CShaderCache → stores compiled shaders indexed by TShaderID
CPlatformShaderCompiler → dispatches to platform backend:
    CBackEndD3D11 (PC DX11)
    CBackEndD3D12 (PC DX12)
    CBackEndOrbis (PS4)
    CBackEndVulkanDxsc (Vulkan)
    CBackEndDurango (Xbox One)
ByteCodeCompiler::CCompiler → preprocesses + compiles HLSL
    → uses d3dcompiler_47.dll (PC)
    → or sce::Shader::Wave::Psslc (Orbis/PS4)
```

### TShaderID

ShaderIDs are **64-bit hashes**. The high bits encode the shader family, the low bits encode the variation (define combinations). Key functions:
- `ComputeShaderID(CShaderHandlerManager&, ndStringBase<char>&, ndVector<SDefineValue>&)` → TShaderID<uint64>
- `GetShaderFamilyBitMask(TShaderID)` → masks family bits from shader ID
- `GetShaderFamilyName(TShaderID)` → returns family name string
- `GetDefinitions(CPreProcessorDefinitions*, TShaderID)` → platform-specific defines per shader ID

### shaders.dat vs shadersobj.dat

Confirmed by Parallellines: `shaders.dat` = source (HLSL), `shadersobj.dat` = compiled result (DXBC). The batch workflow:

```bash
# Generate shader IDs from families
ShaderGenerator2 platform=%platform% operation=Generate shaderidsfile=shaderids.txt

# Compile shaders
PreparePlatformData64.exe -shaders -platform=%platform%

# Package into archives
FileArchiver platform=%platform% archive=shadersobj.dat addList=shaderfiles.txt
```

### WD1 fastinitdata.bin

WD1's `engine/shaders/fastinitdata.bin` (1,192,728 bytes) is a Nomad resource:
- Magic: `nbCF` (0x4643626e), version 3
- 2,400 shader entries (NumObjects field)
- FCBastard can deserialize but not re-serialize (no filter for RootId 0x67974467)

### WD1 usage.bin

`engine/shaders/usage.bin` (27,752 bytes) = 3,469 u32 pairs — shader usage/dependency tracking data.

### Material Descriptors

94 XML files in the leak, 46 in WD1. Each defines parameters for a shader type:
```xml
<materialdescriptor name="DriverGeneric" id="#(hash_lo, hash_hi)">
  <geometryInputs>normal, tangent, binormal, color, uvChannels</geometryInputs>
  <parameter type="sampler2D" name="DiffuseTexture1" default="graphics\_textures\default.xbt" />
  <parameter type="float4" name="DiffuseTiling1AndGroup1" default="1,1,0,0" />
  ...
</materialdescriptor>
```

WD1-specific descriptors: alonebarrier, character, cloth, decal, dots, drivergeneric, glass, hair, leaf, rain, road, skin, vehicle, water, weapon.

## Vector4782's Highlight System

Post-FX shader approach for NPC/enemy highlighting:

- Single post-FX shader divided into **low/high masks** with color thresholds (2 colors only)
- Enemies = red, NPCs = blue, hackable objects = highlighted
- Fixed distance-based color transitions by editing `entitylibrary`
- Weapons cyan unintentionally — wants gray/white
- "All we need is grid shading and we are set!" for WD2-style hackervision

## GridShading / Hackervision

- Lua function for grid shading exists in WD1, broken in WD2/WDL (compiled Lua bytecode)
- Could toggle via Lua keybind for WD2-style hackervision overlay
- Needs Lua decompilation to find the function

## shadersobj.dat Internals

`shadersobj.dat` is not just a flat archive — it has internal structure:

- **Index files**: Large files listing all shader entries per platform. Some shader IDs are NOT in the index files (unknown/extra shaders exist).
- **Platform entries**: Separate compile scripts per platform: `prepareplatformdata64.exe -shaders -platform=%platform% %blobParam%`
- **Vulkan shaders**: Present in shadersobj for PC alongside DX11 — unknown platform entries. Potential FPS boost target.
- **Shader source checksums**: `<ShaderFileChecksums>` XML inside shadersobj contains checksums for shader source files (e.g., `depthshadow.inc.fx`, `scenegraphicobjectinstance.fx`) plus blend state definitions (SrcBlend, DestBlend, ZEnable, CullMode, etc.)

### Required Files for Shader Swapping

Three files work together (confirmed by みる97 on Xbox 360 beta builds):

| File | Purpose |
|------|---------|
| `shadersobj.dat` | Compiled shader blobs (`.pso`/`.vso`) |
| `engine\shaders\fastinitdata.bin` | FCB file mapping shader IDs → names. Decodes to XML with full ID mapping |
| `engine\shaders\usage.bin` | Engine shader usage table — tells the game which shaders to use. Present in all Dunia engine games (WD1/WD2/WDL, Far Cry, The Crew) |

**Note**: Even playing the entire game won't dump all shader names — there will always be unknowns.

## How shadersobj.dat Is Generated

`PreparePlatformData64.exe` is the official Ubisoft tool for compiling and packaging shaders:

```bash
# Full compile + pack (takes ~6 hours on a beefy machine)
PreparePlatformData64.exe -platform=win64 -shadersobj=all

# Pack only — skips recompilation, packages existing .obj shader files
PreparePlatformData64.exe -platform=win64 -shadersobj=bigfileonly

# Compile only — generates .obj shader files without packing
PreparePlatformData64.exe -platform=win64 -shadersobj=generate
```

**Shader sources**: Individual `.pso`/`.vso` files live in `data_win64/engine/shaders/obj/`.

### Debug Build Shader Modding Workflow

1. Delete `shadersobj.dat` → debug game falls back to individual files in `engine/shaders/obj/`
2. Edit individual `.pso`/`.vso` files in that folder (using cmd_Decompiler)
3. Game also compiles missing shaders on-the-fly when encountered in-game
4. Run `-shadersobj=bigfileonly` to repack into a new archive

**Note**: Only DX11 shaders are compiled this way. DX2 path is unclear.

## Practical Examples

### Bounce Lighting Fix (みる97)

Vanilla character bounce lighting was broken — sun bounce power set to 24. Fix required shader edit, not material descriptor (no material-level control for this parameter).

### isAidenSkin Flag (みる97)

Retail WD1 character shader replaced `isCharacter` with `isAidenSkin` — only Aiden gets the skin shader path. Fix: change encoded flags in the compiled shader to apply skin lighting to all characters. No per-material way to do this.

### .fx Source Files — NOT Used at Runtime

The `.fx` source files in `shaders.dat` are **not used by the engine** (confirmed by みる97). Mrlu pasted .fx content into shadersobj.dat and the game didn't crash — the game reads compiled bytecode from `shadersobj.dat`, not source code from `shaders.dat`. The .fx files serve as reference/documentation only.

### HMI Materials (みる97)

2012 Vespid had HMI (High Metallic Interaction) materials that were metallic — removed in final game. Found buried in game files. Released as `reflective_materials.rar` → install to `graphics\_materials`. 604 car paint materials total across all vehicles; batch regex replacement recommended for bulk edits.

## Entity Highlighting

### bIsAutoHighlighted (BFA86464)

Entitylibrary hash `BFA86464` = `bIsAutoHighlighted`. Controls flashing/highlighting of craftable items, pickups, and interactive objects. Setting `_BFA86464="01"` enables highlighting. Note: some objects (bridges) may still highlight regardless — not all highlighting is controlled by this flag.

## Related

- [Shader Addons](shader-addons.md) — Parallellines' addonconfig.h (what gets compiled into shaders)
- [Material Binary Format](material-bin-format.md) — material parameter editing
- [Material Hex Editing](material-hex-editing.md) — hex workflow for material.bin
- [Material Descriptors](material-descriptors.md) — shader parameter schema XMLs
- [MAC Animation Format](mac-format.md) — source animation binary format
- [Markup Format](markup-format.md) — animation markup event XML format
