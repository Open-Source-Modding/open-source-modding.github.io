# Watch Dogs: Legion — Game Engine & Modding Reference

Technical reference for WDL's engine internals, derived from DLL RE, shader
analysis, and format documentation.

---

# 1. dinput8 Proxy DLL — Game Entrypoint

The game loads `dinput8.dll` from its own directory (standard DirectInput8 DLL
search order). The modding community replaces this with a proxy that hooks into
the game's DLL loading at process attach.

## DLL_PROCESS_ATTACH

1. Opens a console window (title: "EncryptedStudios Dev Console - Watch Dogs Legion")
2. Redirects stdout/stderr/stdin to the console
3. Opens a timestamped log file (`EncryptedStudios_boot.log`) next to the DLL
4. Loads the real system `dinput8.dll` from `System32` via `GetSystemDirectoryW`
5. Resolves and stores function pointers for all 5 DirectInput8 exports
6. Spawns a background thread to search for and load payload DLLs from the game directory

## Export Forwarding

Every proxy export (`DirectInput8Create`, `DllCanUnloadNow`, `DllGetClassObject`,
`DllRegisterServer`, `DllUnregisterServer`) calls the real system DLL's
implementation through resolved function pointers. The proxy is transparent to
the game — it adds mod-loading behavior without altering DirectInput8 semantics.

## DLL_PROCESS_DETACH

Closes the log file, then `FreeLibrary` on the payload and real `dinput8.dll` handles.

## Key Takeaway

This exploits the Windows DLL search order: placing a custom `dinput8.dll` in the
game directory causes Windows to load it instead of the System32 version. The
proxy forwards all DirectInput8 calls unchanged while injecting mod payloads in
a background thread.

---

# 2. Shader System

## 2.1 Shader Object Layout

```
engine/shaders/
├── obj/               # DX11-style compiled shader objects (.cso / .vso)
├── obj_d3d12/         # D3D12 variant (primary modding target)
├── obj_vulkan_dxc/    # Vulkan via DXC (vestigial — PC only ships DX11/D3D12)
├── fastinitdata.bin           # Precompiled shader blob (DX11)
├── fastinitdata_d3d12.bin     # Precompiled D3D12 blob
├── fastinitdata_vulkan_dxc.bin
└── shaders.crc                # 4-byte CRC checksum
```

Each `obj*/` has 128 hash buckets (`h00`–`h7f`). File naming:
`<stage>_<hash64>.<ext>` (e.g. `compute_132c5d0cc6f1be00.cso`).

Base counts: **54,821** D3D12 shaders. Patch override: **43,065** (zero hash
overlap with base — all new shaders).

## 2.2 DXIL Format (D3D12/Vulkan)

Both `obj_d3d12/` and `obj_vulkan_dxc/` files have a **0x38-byte wrapper**
before the standard DXBC container. Inside the DXBC is **DXIL** (Shader Model
6.0+). The DXBC checksum is invalid — the game bypasses validation.

Strip the header:

```bash
dd if=input.cso bs=1 skip=56 of=stripped.cso
```

Key properties:
- All cbuffer reads use `cbufferLoadLegacy` (opcode 59), zero scalar
  `cbufferLoad` (opcode 58) — early SM 6.0 scalar-CBV layout
- `float[N]` arrays have `ArrayStride 4` (tightly packed, not vec4-aligned)
- HLSL cannot express stride-4 arrays → requires flattened cbuffer fallback

## 2.3 Shader Decompilation Pipeline

```
input.cso → strip 0x38 header → dxil-spirv → patched spirv-cross → .hlsl
```

| Tool | Purpose |
|------|---------|
| `dd bs=1 skip=56` | Strip D3D12/Vulkan wrapper |
| `dxil-spirv` (YYadorigi v0.6) | DXIL → SPIR-V |
| `spirv-cross` (patched) | SPIR-V → HLSL SM 6.0, with cbuffer stride-4 fix |
| `dxc` (v1.9) | DXIL disassembly / HLSL recompilation |

**Result**: 53,545/53,551 base D3D12 shaders decompiled (99.99%). The 6
failures are non-shader files (`index.*`, `a8e8da7e1e9c3506`).

The patched `spirv-cross` falls back to `uniform float4 _name[size]` for
non-packoffset-compatible arrays (SPIRV-Cross issue #2658 is wontfix upstream).

## 2.4 Cbuffer Member-Name Recovery

DXIL binaries retain **original HLSL cbuffer member names** as a string table at
the end of the module. Pairing with `dxc -dumpbin` struct types
(`%dx.alignment.legacy.*`) reconstructs: member name → byte offset → float4
register slot.

This is the highest-fidelity source of shader variable names — the game strips
reflection names, but the string table + struct types survive.

Tools:
- `cbuf_names.py` — extracts `<name, reg, comp, byte_offset>` per cbuffer member
- `rewrite_cbuffers.py` — rewrites flattened `float4 Viewport_c[854]` → named struct

```
python3 rewrite_cbuffers.py <shader.cso> <decompiled/*.hlsl> <output.hlsl>
```

Full batch: **53,545** base shaders rewritten → `decompiled_cb/`. 100/100 random
sample compiles via `dxc -T [vs|ps|cs]_6_0 -E main`.

## 2.5 Patch Shader Set

The patch `.dat`/`.fat` pair overrides base archives at runtime:

- **Effective runtime set** = base stage files + patch index (superset) + patch fastinitdata blobs
- `obj_vulkan_dxc/` absent from patch — base retained entirely
- Patch index: 271,744 entries (vs 235,801 base) — ~65K new hashes
- Patch `fastinitdata_d3d12.bin`: 2,382,067 bytes (vs 2,092,388 base)

Content changes:
- New cbuffer layouts: **540-float4** lookup buffer in ~67% of patch compute shaders
- Skinning matrices **3066→3075**, new 268/1024-size buffers
- RT reflection shaders reworked: `PerInstanceData`/`PerInstanceAddData` → single `PerMaterialData` buffer
- Cloth closest-hit **+1,157 lines** fp math, water **+616**, skin/hair **+284**

Total decompiled: **95,777** .hlsl (53,545 base + 42,232 patch).

## 2.6 RenderDoc Capture

The best way to get readable HLSL from live shaders:

```
renderdoccmd capture ~/.local/share/Steam/steamapps/common/WatchDogs_Legion/bin/WatchDogsLegion.exe
```

Captures shaders used in a given scene (not all 170K), but是最practical approach
for modding-specific shader investigation.

---

# 3. GLM Model Format

`SportBoat_01.glm` example — 31 meshes, each with 4 LOD levels:

| Mesh | LOD0 verts | LOD0 faces | LOD3 verts | LOD3 faces |
|------|-----------|-----------|-----------|-----------|
| Chassis_LOD | 3,531 | 599 | 85 | 550 |
| SteeringWheels_LOD | 320 | 641 | 72 | 79 |
| EngineBoat_81_LOD | 711 | 1,163 | 103 | 119 |
| TopRack_Boat_LOD | 134 | 249 | 39 | 28 |
| SportRack_LOD | 974 | 1,736 | 240 | 172 |
| CoverTop_LOD | 69 | 101 | 8 | 0 |
| Inboard_LOD | 70 | 141 | 4 | 16 |
| Bayliner_Glass_01_LOD | 41 | 32 | 26 | 14 |
| Bayliner_Glass_02_LOD | 41 | 32 | 26 | 14 |

LOD naming: `MeshName_LOD0` (highest detail) through `MeshName_LOD3` (lowest).
Glass parts have only 2 LODs. Export applies -90° X-axis rotation for Blender
compatibility.

---

# 4. TAM Material Library

Materials are stored in `.material.bin` packs (TAM v15). Each material declares:
- Shader binding (e.g. `KODOCarGeneric`, `KODOCarPaint`, `KODOGlass`)
- Texture parameters with FNV-1 hashes
- 56 declared parameters per material (typical)

## Shader Types

| Shader | Role |
|--------|------|
| `KODOCarGeneric` | Standard vehicle surfaces (body, rims, panels) |
| `KODOCarPaint` | Painted surfaces with clearcoat |
| `KODOGlass` | Transparent surfaces (interior + exterior) |

## Texture Bindings (KODOCarGeneric example)

| Slot | Hash | Purpose |
|------|------|---------|
| `colour` | `0xF95881AF` | Albedo/diffuse |
| `specular` | `0x260A3CFE` | Specular/roughness |
| `normal` | `0x045829E2` | Normal map |
| `damage_specular` | `0x146C808B` | Damage overlay |
| `emissive` | `0x4E0D5E9` | Emissive glow |

Texture paths follow: `graphics\vehicles_nexus\land_textures\wd2\glob*`

Material archive paths reference hashed names:
`paulmaidens-m-9223372094509408844.material.bin`

---

# 5. Vehicle Handling Format

`.handling.bin` files encode vehicle physics as indexed float/uint parameters.
Parsed by the game's drive handling system.

## Parameter Format

Each parameter: `<index> <type> <value> <name>`

| Index | Type | Name | Example |
|-------|------|------|---------|
| 3 | float | Length | 0.3 |
| 14 | float | Pitch | -20.0 (on ground) |
| 15 | float | Pitch | 9643.5 |
| 17 | float | Roll | 3500.0 |
| 19 | float | Max Spring Force Front | -1.0 |
| 25 | float | Front Offset | 0.0 |
| 26 | float | Rear Offset | 0.8 |
| 27 | float | Camber Effect | 0.0 |
| 35 | float | Underground Length | 0.25 |
| 50 | float | Stiffness | 2500.0 |
| 110 | uint | Enable low rider | 0 |
| 115 | float | Suspension Speed | 0.3 |
| 140 | float | On Ground | -20.0 |
| 141 | float | When Airborne | -20.0 |
| 505 | float | Stiffness - Rear Scale | 0.85 |
| 587 | float | Progressivity | 0.6 |
| 761 | float | Raycast Extrapolation | 0.0 |
| 764 | float | Wall Riding Scalar | 0.1 |

XML roundtrip: `Bin → XML → edit → XML → Bin` via the WDL Studio tools.

---

# 6. Wwise Audio (WEM Format)

Sound files use Wwise (WEM) format. The game's sound directory contains WEM
files indexed by numeric IDs (e.g. `1028811276.wem`).

| Property | Value |
|----------|-------|
| Format | Wwise (0xFFFF header) |
| Channels | 2 (stereo) |
| Sample rate | 48,000 Hz |
| Typical size | 1–3 MB per file |

Tools: `vgmstream` (bundled with WDL Studio), `wav2wem` for re-encoding.
Swap workflow: drop WAV/MP3/FLAC/OGG/WEM onto highlighted clip → encode & swap.

Sound library index files map IDs to WEM files within `soundbinary/`.

---

# 7. Engine Path Reference

Key paths inside the game's virtual filesystem:

| Path | Contents |
|------|----------|
| `engine/scripts/` | Lua scripts (domino engine) |
| `engine/scripts/MissionTools.lua` | Mission scripting API |
| `engine/settings/` | XML config (engine, network, threading, launch) |
| `engine/settings/defaultrenderconfig.xml` | Render settings (RT, resolution, quality) |
| `engine/shaders/` | Shader objects (see §2) |
| `engine/shaders/Textures/SkyResource.bin` | Sky LUT |
| `engine/shaders/Textures/PuddleHeight.bin` | Puddle height map |
| `engine/gamemodes/` | GameModesConfig.xml |
| `data_win64/` | Archive root (`.dat`/`.fat` pairs) |
| `data_win64/shadersobj.dat` | Shader archive |
| `data_win64/commonengine.dat` | Common engine data |

Archives are split by category. `filelist.txt` maps paths to archives.

---

# 8. Lua Scripting

The game uses Lua 5.1.5 with a custom "domino" visual scripting layer. Scripts
are compiled to Lua bytecode but decompile cleanly (no variable/function names
lost — the visual scripting tool generates standard Lua).

## Globals

`globals.lua` contains a live dump of the `_G` table: **1,181** function names
(e.g. `SetItem`, `RemoveImmunity`, `SetNetworkVariableBool`). Of these, **766**
are string-locatable in the DLL (the rest are runtime-registered or packed).

## LoaderWDL Integration

LoaderWDL hooks `lua_pcall` and `luaL_loadbuffer` at known RVAs (see
`pdb_build/build_pdb_yaml.py` lines 75–82 for the dx12 offsets). The hook
captures `lua_state` and enables injecting Lua scripts at runtime.

Console output pattern:
```
[UI/menu] DirectX 12 detected
[UI/d3d12] Starting d3d12 hook
Captured lua_state from pcall!
[Lua] running script...
```

## WLU Files

WLU (compiled Lua) files require exactly 2 null bytes appended after the FCB
portion or the game crashes.

---

# 9. Archive Extraction / Repacking

| Tool | Use |
|------|-----|
| `BE_LegionExtract.exe` / `BE_LegionPack.exe` | GUI pack/unpack (Wine) |
| `Gibbed.WatchDogsLegion.Unpack.exe` / `Pack.exe` | Command-line alternative |

Archives: `~/.local/share/Steam/steamapps/common/WatchDogs_Legion/data_win64/`

`info.xml` with `UseCombineMode="true"` enables merge installation (overlay
without full archive replacement).

---

# 10. Disrupt Engine Toolchain (from source leak)

*Source: `ubisoft_leak_contents.txt` — 2020 Ubisoft source code leak directory listing.*

The leak contains the full Disrupt engine internal toolchain used to build WDL.

## Core Tools

| Tool | Purpose | Key DLLs |
|------|---------|----------|
| **Wolfskin** | Main Disrupt engine editor (WPF) | `Wolfskin.dll`, `WolfskinStandalone.exe`, `Disrupt.Core.dll`, `Disrupt.Controls.dll`, `Disrupt.Materials.Editor.dll`, `Disrupt.SourceControl.dll` |
| **Wolfskin Max/Maya** | 3ds Max + Maya DCC plugins | `WolfskinMax2015.dll`, `WolfskinMax2017.dll`, `WolfskinMaya2015.nll.dll` |
| **VEdit** | Vehicle handling editor | See §10.1 |
| **WatchBox** | Data inspector | `WatchBox.dll`, `WatchBase.dll`, `WatchDomino.dll` |
| **Painkiller** | Tool (WPF-based) | `Painkiller.Core.dll`, `Painkiller.DisruptModule.dll` |
| **Rulesmith** | Visual scripting editor (93MB) | `RulesmithEditor.exe`, uses D3D + OpenGL ES |
| **BlackBox** | Game launcher | Contains Painkiller modules |

## Shader Pipeline

| Tool | Platforms | Purpose |
|------|-----------|---------|
| `bin/DXC/PC/` | PC | DXC — DXIL compiler (SM 6.0+) |
| `bin/DXC/XboxOne/` | Xbox One | DXC for Xbox |
| `bin/DXC/Scarlett/` | Xbox Series | DXC for Scarlett |
| `bin/DXC/SPIRV/` | Vulkan | DXC → SPIRV cross-compilation |
| `bin/FXC/PC/` | PC | FXC — FX compiler (SM 5.0 and below) |
| `bin/FXC/XboxOne/` | Xbox One | FXC for Xbox |

## Asset Pipeline

| Tool | Purpose |
|------|---------|
| **OasisExporter** | Asset export pipeline (Win32 + Win64 libs) |
| **PakViewer** | Archive (.dat/.fat) inspection |
| **ArchiveDiffer** | Diff two archive versions |
| **MaterialEditorStandalone** | Material editing (standalone) |
| **StringIDTool** | String hash/ID mapping |
| **StreamingPNG** | Streaming texture pipeline |
| **VideoCompiler** | Cutscene video compilation |
| **VideoEncoder** | ffmpeg/ffprobe for video encoding |

## Audio Pipeline

| Tool | Purpose |
|------|---------|
| **WwiseAuthoring** | Wwise audio authoring (full project with plugins, skins, data) |
| **WwiseBinarizer** | Wwise → binary conversion for runtime |
| **AudioTool** | Audio inspection/editing |

## Other Tools

| Tool | Purpose |
|------|---------|
| **Havok/StandaloneFilterManager** | Physics filter configuration |
| **lua/** | Lua tooling |
| **FSMExport** | Finite state machine export |
| **Memory/** | MemoryHUD, MemGraphViewer, MemoryService, EvilCorp, SecretService, Spy |
| **MapExplorer** | PDB/map exploration |
| **ExcelImporter** | Data import from spreadsheets |
| **FileRenamer** | Batch file renaming (uses `Disrupt.SourceControl.dll`) |
| **Mosaic** | Tile/asset mosaicking |
| **DemographicPaint** | NPC population painting |
| **toolframework/** | Shared tool framework plugins |

## Disrupt DLLs (from `bin/`)

| DLL | Purpose |
|-----|---------|
| `Disrupt.Core.dll` | Core engine runtime |
| `Disrupt.Controls.dll` | Editor UI controls |
| `Disrupt.Materials.Editor.dll` | Material editor |
| `Disrupt.SourceControl.dll` | Perforce integration |
| `Disrupt.AssetsHelpers.dll` | Asset pipeline helpers |
| `Disrupt.ApplicationModel.dll` | Application framework |
| `Disrupt.Services.Metadata.dll` | Metadata services |
| `RulesmithServices_r64.dll` | Rulesmith backend (dx11) |
| `RulesmithServices_r64_dx12.dll` | Rulesmith backend (dx12) |

---

## 10.1 VEdit — Vehicle Handling Editor

VEdit is the Disrupt engine's vehicle dynamics editor. It contains
documentation, parameter definitions, and UI layouts for car and helicopter
handling.

### Data Structure

```
VEdit/
├── Data/
│   ├── Contexts/Car/        # Chassis, Engine, Transmission contexts
│   ├── Controls/Car/        # Car UI controls
│   ├── Controls/Helicopter/ # Helicopter UI controls
│   ├── Controls/DesignSettings/
│   ├── Graphs/              # Handling curves/graphs
│   ├── Layouts/             # UI layouts
│   ├── Parameters/Car/      # Parameter definitions
│   ├── Simulation/          # Physics simulation config
│   │   ├── DefaultHandlings/
│   │   ├── Measurement/
│   │   ├── ParameterDefinitions/
│   │   └── TelemetryDefinitions/
│   └── Windows/Car/         # Editor windows
│       └── TestBezier/
├── Documentation/
│   └── html/CarHandling/    # Full handling documentation
│       ├── brakes/          + diagrams
│       ├── cameras/         + diagrams
│       ├── chassis/         + diagrams
│       ├── drag/            + diagrams
│       ├── drivetrain/      + diagrams
│       ├── engine/          + diagrams
│       ├── gearbox/         + diagrams
│       ├── graphics/
│       ├── loadtransfer/    + diagrams
│       ├── steering/        + diagrams
│       ├── surface/         + diagrams
│       ├── suspension/      + diagrams
│       ├── tutorial/
│       └── tyres/           + diagrams
└── Tool/
    └── locales/             # Multi-language support
```

### Handling Parameters (from `handling.bin`)

Vehicle physics are stored as indexed float/uint parameters in `.handling.bin`
files. Categories from VEdit documentation:

| Category | Key Parameters |
|----------|---------------|
| **Chassis** | Mass, centre of gravity, inertia tensor, dimensions |
| **Suspension** | Length, stiffness, damping, ride height, travel, progressivity |
| **Steering** | Lock angle, speed sensitivity, assist curve |
| **Engine** | Torque curve, RPM limits, power, flywheel inertia |
| **Gearbox** | Gear ratios, shift times, final drive |
| **Drivetrain** | FWD/RWD/AWD, differential locking, torque distribution |
| **Brakes** | Max torque, bias front/rear, ABS threshold |
| **Tyres** | Grip curve (slip angle vs friction), load sensitivity, rolling resistance |
| **Drag** | Aerodynamic drag coefficient, downforce |
| **Surface** | Terrain grip multipliers (asphalt, dirt, grass, etc.) |
| **Load Transfer** | Weight transfer during braking/acceleration/cornering |
| **Cameras** | Chase cam position, FOV, spring damping |

VEdit's `Documentation/html/CarHandling/` contains detailed HTML docs with
diagrams for each category — the authoritative reference for `handling.bin`
parameter semantics.
