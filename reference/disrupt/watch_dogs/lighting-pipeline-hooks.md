# Disrupt WD1 Lighting Pipeline — Renderer Internals & Hook Points

> **Source**: Shadow Engine source (`Open-Source-Modding/Shadow-Engine`, cloned
> `/tmp/shadow_engine_src/`), shipped WD1 shader source (`shaders_unpack/`),
> `Disrupt_b64.c` Hex-Rays decompile, Wii U debug symbols. Assimilated
> 2026-08-28 for the SSRTGI-on-real-lights project.

## The goal

SSRTGI (screen-space ray-traced GI) currently gathers flat screen color from
the pre-UI scene color buffer. It looks flat because it has no knowledge of
actual light sources. This doc maps the engine's **real illumination data
path** so we can hook it and feed actual lights (position/color/attenuation)
into the SSRTGI pass.

## Engine light data flow (verified)

```text
world + vehicle lights
  → CSceneLightInstance / CSceneLightInstancePrivateData (CRendererLighting)
  → light candidate records in the renderer queue (0x24C0 bytes each)
  → CMultiPassDeferredLighting::JobSetup (deferred lighting pass)
  → per-light LightData constant buffer (LightPosition/LightFrontColor/...)
  → GPU shaders (lighting.inc.fx SSunLight/SOmniLight/SSpotLight)
```

The renderer queue record stride is `0x24C0` bytes; face/light-object slots at
`entry + 0x7D8`, stride `0x820` (each slot holds a light object pointer; the
object has a cost/type u32 at `object + 0x20`). `RENDER_QUEUE_COUNT_OFFSET`
holds the queue entry count.

## Shadow Engine RVAs (vanilla WD1 retail) — ⚠️ +0x1000000 offset correction

These are the *supported* (base/vanilla) RVAs from Shadow Engine's
`src/modules/00_shared_config_state.inc`. `resolve_engine_rva()` returns them
verbatim when no protected profile is selected.

**⚠️ CORRECTION (verified 2026-08-28 against retail bytes + Ghidra decomp):**
Shadow Engine's published values are **+0x1000000 (16MB) below the true file
RVA / Ghidra absolute address**. The retail DLL image base is `0x180000000`
(PE header, confirmed). Ground truth: at file RVA `0x306E60` the bytes are
mid-instruction (`48 8b d0 e8...`, no prologue); the real function prologue
(`48 89 5c 24 08 48 89 6c 24 10...`) is at RVA `0x1306E60`, which Ghidra names
`FUN_181306e60`. So: **ShadowEngine value + 0x1000000 = true RVA =
Ghidra/IDA absolute address `0x180000000 + that`.** Always add the offset.

| Purpose | ShadowEngine value | True RVA | Ghidra symbol |
|---|---|---|---|
| Renderer queue processor (shadow/light dispatch) | `0x00306E60` | `0x1306E60` | `FUN_181306e60` |
| Frame builder | `0x003DC780` | `0x13DC780` | `FUN_1813dc780` |
| Frame builder end | `0x003DF833` | `0x13DF833` | — |
| Execute frame graph dispatch | `0x003BA460` | `0x13BA460` | — |
| Render record acquire/release/ctor | `0x003E0F80`/`0x003C1350`/`0x003E4420` | `0x13E0F80`/... | — |
| Post-call task ctor/execute | `0x003E69D0`/`0x003E4DF0` | `0x13E69D0`/... | — |
| Post-call object renderers | `0x003CA980` | `0x13CA980` | `LAB_1813ca980` |
| Shadow manager | `0x002E9B60` | `0x12E9B60` | — |
| Scheduler init/floor/upper | `0x002E8E89`/`0x002E9C21`/`0x002E9C3B` | +0x1000000 | — |
| Face cost reader | `0x002C0490` | `0x12C0490` | — |
| Resource submit/wrapper | `0x00427710`/`0x00427860` | +0x1000000 | — |
| Shared texture lookup | `0x00430A80` | `0x1430A80` | — |
| Pass index lookup/store | `0x003073E2`/`0x00308441` | +0x1000000 | — |
| Queue count init | `0x002E2236` | `0x12E2236` | — |

In the Ghidra decomp (`Disrupt_b64.c`), `FUN_181306e60` is the renderer queue
processor. It calls `FUN_1813057b0` (the per-record dispatcher / light-member→
shader-param setter).

Queue/record offsets (native 16-map layout; Shadow Engine expands to 30 maps
for its own patch — **the vanilla game uses 16**):

| Field | Offset |
|---|---|
| First mapping array | `0x270D0` |
| Second mapping array | `0x27114` |
| Queue count | `0x27158` |
| Queue tail | `0x27160` |
| Queue result | `0x27170` |
| Record entry stride | `0x24C0` |
| Face slot base in record | `+0x7D8`, stride `0x820` |

`SHADOW_PASS_CATEGORY = 0x0C`, pass 16 = `LongRangeShadow`, native 16 local maps,
17 physical queue entries (extra entry = long-range job).

## GPU light structures (from shipped shaders)

`shaders_unpack/engine/shaders/lighting.inc.fx`:

- `SOmniLight` (132 B): `position float3, attenuation float3, rcpRadius,
  rcpShadowFadeRange, backColor float3, frontColor float3, capsuleDivLength
  float4, capsuleMulLength float3, shadowMapSizes float4, shadowFactor float2,
  halfLambert bool, receiveShadow bool, hiResShadowFilter bool,
  specularIntensity float, depthTransform float2`
- `SSpotLight` (180 B): position, direction, attenuation, backColor, frontColor,
  coneFactors float2, specularIntensity, shadowProjections float4x4, ...
- `SSunLight` (160 B): direction, shadowProjection float4x4, backColor,
  frontColor, halfLambert, receiveShadow, shadowFactor float2, ...

`parameters/lightdata.fx` (per-light CB): `LightPosition`, `LightFrontColor`,
`LightDirection`, `LightAttenuation`, `LightSpecularIntensity`,
`LightSpotConeFactors`, `LightSpotNearClipFactor`, `LightShadowMapSize`,
`LightVolumeTransform`, cascaded shadow params.

Ambient / GI systems:
- `parameters/ambientdata.fx`: `AmbientLightDirectionWS`, `SkyColor`,
  `GroundColor` (sky/ground ambient)
- `ambient.inc.fx`: `WorldAmbientColorTexture` viewport-sized world ambient
- `meta/lightmap/lightprobesupdate.fx`: `SRadianceTransferProbeCompute` (32 B)
  — SH radiance transfer + static irradiance + sky visibility, relit by a
  compute shader. The engine's native GI probe volume.
- `meta/lightmap/lightprobes.fx`: `CLightProbeRenderer` generates a
  viewport-sized GI image.

## Verified CPU-side LightData CB layout (from Ghidra decomp)

The engine builds one **unified "LightData" CB** per light. Its parameter
registration is `FUN_181e08ba0` (Disrupt_b64.c ~5330698), which registers every
member with its CB offset via `FUN_181ef81e0(&DAT_183be29d0, idx, name, &off, ...)`.
Offsets (`local_18`) are literal CB byte offsets (corrected for the running
`_DAT_183be29f8` base). Decompiled offsets match the shipped `lighting.inc.fx`
GPU structs and `lightdata.fx` CB. Full member map:

| Offset | Member | Shader type |
|---|---|---|
| `0x20` | LightVolumeTransform | matrix (0x110) |
| `0x70` | LightCapsuleMulLength | float3 (0x103) |
| `0x90` | LightCapsuleDivLength | float4 (0x104) |
| `0x90` | RainLightViewpointLPS_DistVPToEdgesLPS | float4 (0x104) |
| `0xb0` | LightCapsuleParams | float2 (0x102) |
| **`0xd0`** | **LightPosition** | float3 (0x103) |
| `0xf0` | LightDirection | float3 (0x103) |
| **`0x110`** | **LightFrontColor** | float3 (0x103) |
| `0x130` | LightAttenuation | float3 (0x103) |
| `0x150` | LightShadowFactor | float4 (0x104) |
| `0x170` | LightShadowMapSize | float4 (0x104) |
| `0x190` | LightShadowTexture | tex (0x40001) |
| `0x1a0` | LightShadowDepthTransform | float2 (0x102) |
| `0x1c0` | LightSpotConeFactors | float2 (0x102) |
| `0x1e0` | LightSpotNearClipFactor | float1 (0x101) |
| `0x200` | LightSpotShadowProjections | matrix (0x110) |
| `0x370` | LightSpecularIntensity | float1 (0x101) |
| `0x390` | LightProjectedTexture | tex (0x30001) |
| `0x3a0` | LightProjectedTextureState | tex (0x70001) |
| `0x3b0` | LightProjectedVideo | tex (0x30001) |
| `0x3c0` | LightProjectedVideoUnpack | (0x804) |
| `0x450` | LightFadingClipPlanes | (0x804) |
| `0x4e0` | FacettedShadowReceiveParams | float4 (0x104) |
| `0x500` | FacettedShadowNoiseTexture | tex (0x60001) |
| `0x250` | CascadedShadowRanges | float4 (0x104) |
| `0x270` | CascadedShadowScale | float4 (0x104) |
| `0x290` | CascadedShadowSliceScaleOffsets | float3x4 (0x304) |
| `0x2d0` | CascadedShadowSliceScaleOffsetsBiased | float3x4 (0x304) |
| `0x310` | CascadedShadowSliceDepthScales | float4 (0x104) |
| `0x330` | CascadedShadowSliceDepthOffsets | float4 (0x104) |
| `0x350` | CascadedShadowDepthRanges | float4 (0x104) |
| `0x510` | LongRangeShadowVolumePosScaleBias | float4 (0x104) |
| `0x530` | LongRangeShadowVolumeUvScaleBias | float4 (0x104) |
| `0x550` | LongRangeShadowVolumeTexture | tex (0x60001) |

**For SSRTGI the SSRTGI-relevant fields are `LightPosition` (0xd0), `LightFrontColor`
(0x110), `LightAttenuation` (0x130), `LightDirection` (0xf0)** — read these from
the CB (or the light record) to feed real-light tracing.

## Verified light-record layout (from Ghidra decomp)

The renderer queue processor `FUN_181306e60` → `FUN_1813057b0` (Disrupt_b64.c
~3416281) is a **light-member → shader-param setter**: `switch(member_idx)`
maps each index to an offset in the light record `param_4`, then calls
`FUN_180d08ad0(cb, cb_member_idx, &value, 1)`.

Light object member indexing (case 2 in `FUN_1813057b0`):
`addr = light_obj + face_idx*0x218 + 0x448 + slot_idx*0xb0` — i.e. a per-face
record stride of **`0x218`**, with a per-face slot sub-array at **`+0x448`**
stride **`0xb0`**. (0x448 > 0x218, so the slot sub-array is a separate region
within the light object, not inside each face record.)

| Record offset | Meaning |
|---|---|
| `0xd0`/`0xd4`/`0xdc`/`0xe0`/`0xe4`/`0xe8` | position / extent components |
| `0x44`/`0x48`/`0x4c` | color-ish (front color group) |
| `0x54`–`0x88` | per-member params (0x54,0x58,0x5c,0x60,0x64,0x68,0x6c,0x70,0x74,0x78,0x7c,0x80,0x84,0x88) |
| `0x8c`–`0xb8` | params (0x8c,0x90,0x94,0x98,0x9c,0xa0,0xa4,0xa8,0xac,0xb0,0xb4,0xb8,0xbc,0xc0) |
| `0x180`/`0x184`/`0x188` | params |
| `0x364`/`0x368`/`0x3ac` | params (near end of record) |
| `+0x448` sub-array | stride `0xb0` (repeated light data, e.g. per-face) |

`FUN_1813057b0` case 0/1 route through `FUN_18116b340`/`FUN_18116b3c0`; case 2
reads `record[face*0x218 + 0x448 + slot*0xb0]`. Cases 7/8/9 read
`0xe0/0xe4/0xe8` with a ×2/×3 multiplier applied when `FUN_18128b660` /
`FUN_1812ff270` fire (feature toggles).

## CMultiPassDeferredLighting::JobSetup

`FUN_181e61610` (Disrupt_b64.c ~5378687) creates the deferred lighting job; the
job callback is `FUN_181e5ad30` (passed as the job fn). The name string
`"CMultiPassDeferredLighting::JobSetup"` is emitted at 5378721 via
`FUN_181e4cc60` (job debug naming). Related CBs registered in the same region:
`DeferredLightingCommon` (`DAT_183be20c0`, ~5368895), `DeferredLightingBase`
(`DAT_183be2070`, ~5369015); `DeferredLighting` shader passes 1/2 registered at
~5365973.

## Per-light job `FUN_181e5ad30` (the real light enumeration — prime hook)

A ~1470-line function (Disrupt_b64.c 5374699–5376172) that iterates **every
active light candidate** and builds the CPU-side `LightData` struct (`uVar27`,
allocated 0x560 via `FUN_181e08960` constructor) per light, then enqueues it
for GPU upload.

### Light candidate linked list (THE enumeration source)

```
head = **(param_7 + 0x10)        // 5375511 — list head
puVar33 = *puVar33               // 5375516/5376158 — next pointer
loop: param_12 .. param_13       // 5375519,5376160 — [start, count) window
```

Each `puVar33` node (light candidate):

| Offset | Meaning |
|---|---|
| `[0]` | next pointer |
| `[1]` | light config/type struct |
| `[1]+8` | **type**: 1, 2, 3, 4 (spot/omni/etc) |
| `[1]+0x40` | flags (bit0x84000000 = disabled) |
| `[1]+0x3c` | range/radius (0 = use default) |
| `[1]+0x60` | specular intensity |
| `[2]` | position/transform struct — **position at +8/+0xc/+0x10** |
| `+0x1c` | range (world units) |
| `[4]` | cone half-angle |
| `+0x24` | inner cone angle |
| `+0x44`, `[9]`, `+0x4c` | shadow params |
| `[0xa]` | spot data (cone factors at +0x28/+0x2c) |
| `[0xb]` | shadow map data |
| `[0xe]` | misc |

### CPU-side LightData struct (`uVar27`) — mirrors the CB layout

Constructed by `FUN_181e08960` (0x560 bytes). Written per type, then enqueued
(`puVar29[count+5] = uVar27`). Mirrors the LightData CB offsets exactly:
position `0xd0` (filled from node `[2]+8/0xc/0x10` at `0xe0/0xe4/0xe8`),
direction `0xf0`, front color `0x110`, attenuation `0x130`, shadow `0x150`,
spot cone `0x1c0`, specular `0x370`.

Type handling:
- **type 3** (spot/capsule): position from `[2]+8/0xc/0x10` (5375878-5882);
  direction normalized from `[2]+0x14/0x18/0x1c` (5375880-5892); cone factors
  from `[0xa]+0x28/0x2c` (5375897-98); front/back color from `[1]+0xa8/0xac`.
- **type 4**: `FUN_181c67cd0(node[2], node[1], &param_14, &pos, &range, &shadow, &out)`
  extracts color+position+range (5376002); position written 5376008-10;
  range `0xc0` from `[1]`+0x1c; capsule params from `+0x44/0x4c`.

### For SSRTGI capture (recommended)

Hook the candidate-list walk in `FUN_181e5ad30`: iterate
`head=**(param_7+0x10)` from `param_12`→`param_13`, and for each node read:
`type=[1]+8`, `pos=[2]+8/0xc/0x10`, `range=+0x1c`, `color=[1]+0xa8/0xac` (spot)
or via `FUN_181c67cd0` (omni, type 4). Build a light structured buffer
{pos, color, range, type} and feed the SSRTGI shader. Because the job runs on
a job-queue thread, capture into a lock-free ring / atomic-swap buffer and read
it in the Present-hook composite pass.

## Generated header for Ghidra/IDA

`Watch_Dogs/disrupt_shaders_gpu.h` — 203 constant-buffer structs + light/probe/
GBuffer structs extracted from the shaders by `Watch_Dogs/shaders_to_header.py`.
Parse in Ghidra via File → Parse C Source (self-contained, no SDK includes).
Pairs with the 291K-symbol synthetic PDB (`WDL/pdb/Disrupt_b64_wd1.pdb`):
PDB = function names/RVAs, header = GPU struct layouts.

## Community context

black^0 (Discord 2026-08-28) hooked headlight fog via an `enableraincone` flag
— crude engine-flag toggle, looked bad, disabled. Nobody has done a real
lighting-pass hook for SSRTGI on WD1; our LightData/queue hook is novel.