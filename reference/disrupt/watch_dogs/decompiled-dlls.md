# Decompiled DLLs (Ubisoft Leak)

IDA Pro 9.3 Hex-Rays decompiled output from the Ubisoft leak binaries.
All are x64 Visual C++ with RTTI intact.

## Files

| DLL | Size | Purpose |
|-----|------|---------|
| `GeomParser_r64.dll.c` | 4.5 MB | **XBG parser/compiler** — reads GLM/Collada/GamEx/GLx, outputs `.xbg` game mesh. Also handles `.hkr` (Havok ragdoll) |
| `gxasset_release_x64.dll.c` | 1.7 MB | Asset system (GXAsset) — serialization, node graph, buffer management |
| `DuniaDemo_r64.dll.c` | 780 KB | Main game DLL (DX11) — entity system, rendering, physics, Lua |
| `DuniaDemo_r64_dx12.dll.c` | 775 KB | Main game DLL (DX12) — same as above, different rendering backend |
| `Bloomberg.dll.c` | 1.8 MB | Bloomberg financial data integration (debug/telemetry?) |
| `helix.online_capture_v140.dll.c` | 1.3 MB | Helix online services — capture/replay, telemetry |
| `Comet_2_Release_x64.dll.c` | 80 KB | Comet networking — multiplayer, session management |
| `prim_4_10252363098235285984.hkx.c` | 312 B | Havok primitive (minimal) |

## GeomParser — XBG Pipeline

`GeomParser_r64.dll` is the XBG parser/compiler. It reads multiple source formats and outputs the game's runtime mesh format (`.xbg`).

**Input formats:**
- `.glm` — Havok skeletal mesh (Ubisoft's GLM format, MOEG versioned)
- `.glx` — text-based mesh description
- `.dae` — COLLADA (import/export)
- GamEx — internal asset format (via `CGamExParser`)

**Output:**
- `.xbg` — compiled game mesh (runtime format, loaded by the engine)
- `.hkr` — Havok ragdoll (via `CHavokParser`)

**Key classes:**
- `CGLMParser` — reads/writes `.glm` binary (Ubisoft's Havok-based mesh container)
- `CGLxParser` — reads `.glx` text format (human-readable mesh description)
- `CColladaParser` — reads COLLADA `.dae` files
- `CGamExParser` — reads GameX asset format (internal)
- `CTriMesh` — triangle mesh with clusters, hardware vertices, edge links
- `GLMMesh` — Havok-style mesh with vertices, faces, normals, UVs, skinning
- `CCluster` — LOD/rendering cluster

**Pipeline:**
```
.glm / .glx / .dae / gamex → GeomParser → .xbg (game runtime)
```

**Skinning/Animation:**
- `IGeomSkin` — skinning interface
- `CSkeleton`, `CBone` — skeleton hierarchy
- `CBlendShapeMesh` — morph targets
- `SSecondaryMotionUnitDesc` — cloth/hair physics

**Cloth:**
- `SClothWrinkleControlPatchBundle` — wrinkle map system
- `SSMLimitCollectionDesc` — secondary motion limits

## Key Classes (DuniaDemo)

The main game DLL reveals runtime systems:

- Entity component system (same as XML entity prototypes)
- Rendering pipeline (DX11/DX12 backends)
- Physics integration (Havok)
- Lua scripting engine
- Audio (Wwise integration)

## Multiplayer Backend (PRUDP)

WD1's multiplayer uses Ubisoft's PRUDP (Persistent Reliable UDP) protocol for
rendezvous server communication. Debug traces from hook development reveal:

**Rendezvous Backend Login:**
- `RdvBackendLogin_Detour` — detour hook function in Disrupt_b64.dll for
  intercepting the backend login flow
- Sandbox alias: `main_dlc`
- Sandbox access key: `p4BkcPBh`
- PRUDP endpoint: `prudp:/address=1b-rdv-as-prodel.ubisoft.com;port=21601`

**Hook Offsets (Disrupt_b64.dll, retail):**

| Offset | Purpose |
|--------|---------|
| `0xB965A0` | Matchmaking hook (RdvBackendLogin_Detour entry) |
| `0x319CD50` | Secondary matchmaking hook |

The `getSandboxUrl` function resolves the PRUDP endpoint for the multiplayer
sandbox. The sandbox system allows isolated multiplayer environments (e.g.
for DLC content testing).

Source: Debug trace from hook development (`wd1_matchmaking.txt`, community).

## Usage

These are reference decompilations — not meant to be compiled. Use for:
- Understanding function signatures and data structures
- Cross-referencing with Ghidra analysis
- Finding format constants and magic numbers
- Tracing API calls between DLLs
