# Watch Dogs 1 — SSRTGI ASI (screen-space ray-traced GI) project notes

> **Source**: `~/Documents/Code/re/Ubisoft/Disrupt/Watch_Dogs/ssrtgi/ssrtgi.cpp`
> (D3D11 hook ASI, C++11, mingw cross). Companion to
> [lighting-pipeline-hooks.md](lighting-pipeline-hooks.md) which maps the
> engine's real illumination data path. Assimilated 2026-08-28.

## What it is

A D3D11 hook plugin that adds screen-space ray-traced global illumination to
Watch Dogs 1 (Disrupt engine, retail `Disrupt_b64.dll`, image base
`0x180000000`, CompleteEdition build). Screen-gather SSRTGI **works**; real-light
driven GI is in progress.

- Build: `make debug && make install-debug` (from `ssrtgi/`)
- Install: `.../Watch_Dogs/bin/ssrtgi_debug.asi`
- Log: `D:\ssrtgi_debug.log` (append; grep `=== SSRTGI ASI ===` for session starts)
- Repo: `github.com/Open-Source-Modding/WD_SSRTGI.git`
- IP/clean-room policy: shader gather techniques must be independently
  documented (80.lv UNIGINE SSRTGI article, r/nvidia thread, ZN_DAMP_RT_RF.fx,
  HTMA2024/RTGI-Unity, kiselgra/rtgi — all verified independent). The engine
  light-capture + camera hooks are pure own RE from `Disrupt_b64.c`.

## Architecture

```
DllMain
 ├─ IAT-hook D3D11CreateDevice
 │   └─ vtable-hook CreateTexture2D (idx 5) → capture depth textures/SRVs
 ├─ Dummy-device Present hook (MinHook inline, global)
 │   ├─ Track device/context/backbuffer; resize → recreate resources
 │   ├─ ExecuteCommandLists hook (idx 25) → depth after deferred replay
 │   ├─ OMSetRenderTargets hook (idx 33) → track scene depth + scene-color RTV
 │   ├─ render_ssrtgi(): pick depth SRV → upload lights → half-res ray march
 │   │   → ping-pong temporal → composite (HDR pre-tonemap, see below)
 └─ Engine hooks (MinHook inline)
     ├─ UpdateCamera (RVA 0x170ED50, CompleteEdition) → camera view matrix
     └─ (light-capture hook — see status)
```

## D3D11 vtable indices (verified vs mingw `d3d11.h` `ID3D11DeviceContextVtbl`)

OMSetRenderTargets=33, ExecuteCommandLists=25, CopySubresourceRegion=46,
CopyResource=47, UpdateSubresource=48, Map=14, Unmap=15, VSSetConstantBuffers=7,
PSSetConstantBuffers=16, Draw=13.

## Engine hook points

### UpdateCamera (camera view transform)
Direct 2-arg function `UpdateCamera(CCameraManager, TargetView)`. RVA depends on
GameVersion (read `Imagebase+0x100C`):
1=Ubisoft `0x7935F0`, 2=Steam `0x2A1151` → `0x2A1150`, 3=Asia `0x2D2170`,
4=CompleteEdition **`0x170ED50`** (ours). Prologue `48 8b c4 48 89 70 10 57`.
CCameraManager camera data: `+84/88/92` = world pos, `+96/100/104` = rotation
(yaw/pitch/roll), `+108` = FOV. Reference: Sad-Gamers/Camera-Tools-For-Watch_Dogs.

SSRTGI builds a world→view matrix (YXZ rotation, row-major float4x3) into
`g_view_matrix`/`g_cam_pos`, bound as `g_cb_view` (b1):
`cbuffer View : register(b1) { float4x3 ViewMatrix; float4 CameraPos; };`
Light term: `mul(float4(lightPos,1), ViewMatrix)` (valid HLSL overload returning
float3; a `(float4x4)` cast is a compile error X3017). This hook was installed
but did NOT fire in the last test — verify during actual gameplay and confirm
the YXZ rotation convention from real `CAMERA:` pos/rot log values.

### Per-light deferred job (light capture — status: BROKEN, needs pivot)
`FUN_181e5ad30` (RVA `0x1E5AD30`, Ghidra decomp ~5374699) walks the active
light-candidate linked list each frame:
- head = `**(void***)(p7+0x10)`; node->next = `*node`; window `[p12, p13)`.
- Node layout: `[0]`=next, `[1]`=cfg (type@`+8`, flags@`+0x40`
  bit0x84000000=disabled, color@`+0xa8/0xac/0xb0`), `[2]`=pos(`+8/+0xc/+0x10`),
  range@`node+0x1c`.

**Broken**: it's a job callback — the dispatcher passes a job-context pointer,
not the full 28-arg signature (p12 arrives as a pointer ~0x29028500, p13=0,
head=0). Job framework `FUN_181e4cc60` (~5367561) stores callback at job-struct
slot[3], args slot[6]=param_7, slot[0xb]=param_12, slot[0xc]=param_13 — the
job-context fallback also didn't match. **Pivot options** (from Disrupt_b64.c):
1. **`FUN_181306e60`** (RVA `0x1306E60`, renderer queue processor,
   Shadow-Engine-verified RENDER_QUEUE_PROCESS, 4-arg) — walks the same
   light-candidate list; calls light-member setter `FUN_1813057b0`.
2. **`FUN_1813057b0`** (light-member setter, param_4 = light record pos@0xd0) —
   fires per member; hook when cb==LightData & idx==LightPosition/FrontColor/Attenuation.
3. Trace JobSetup (`FUN_181e61610`) arg → find light-list global.

### LightData CB (target data)
Registration fn `FUN_181e08ba0` (~5330698). Offsets: LightPosition@0xD0,
LightDirection@0xF0, LightFrontColor@0x110, LightAttenuation@0x130,
LightSpotConeFactors@0x1C0, LightSpecularIntensity@0x370.

### Lights GPU buffer (current, works)
Typed `R32G32B32A32_FLOAT` buffer (a StructuredBuffer SRV fails
`E_INVALIDARG 0x80070057` on this Wine/driver). SRV `NumElements = MAX_LIGHTS*2`
(2 float4 per light). Shader `Buffer<float4> Lights : register(t3)`:
`Lights[i*2]` = data0 (pos.xyz | range), `Lights[i*2+1]` = data1
(type | color.yzw). CPU `LightRecord` is 32 bytes.

## HDR pre-tonemap composite (Feature A)

Additively blends the indirect-light SRV into the HDR **scene-color buffer**
(`g_scene_color_tex`) BEFORE the engine tonemaps, so HDR values don't clamp.

- Trigger: in `hook_OMSetRenderTargets`, when `g_scene_color_tex` transitions
  bound→not-bound, the HDR scene pass finished → `composite_into_scene_color()`.
  Fallback in `hook_ExecuteCommandLists` (`g_scene_color_was_bound_ecl`).
- FULL D3D11 state save/restore (RTV/DSV, blend, VS/PS, PS SRV[0], PS sampler[0],
  PS CB b0+b1, viewport) — runs mid-frame so every touched state must be restored.
- Typed RTV for typeless textures; RTV invalidated on texture change.
- `g_comp_scene_color` gates the old Present LDR composite (skip if already
  composited), reset each Present.
- Runtime status: **unconfirmed**.

## Known limitations / gotchas

- **EyePos/pixP assumes identity camera view** (camera +Z) — the light term is
  correct only near an axis-aligned camera. Full fix = reconstruct pixP via the
  real view matrix (larger refactor).
- **Wall-bleed through walls** = expected screen-space GI artifact; tune
  `depthFade`/`distFade`/`max_ray_dist` knobs.
- **Backbuffer SRV**: swap-chain backbuffer is `R8G8B8A8_UNORM_SRGB` (fmt=29)
  without `BIND_SHADER_RESOURCE` → must CopyResource to a bind-flagged copy.
- **UpdateSubresource hook (idx 48)** never fires (engine uses Map/Unmap) — kept
  harmless. VSSetConstantBuffers GPU-CB hunting abandoned (wrong buffers:
  0x3C0 color, 0x800 color, 0xD00 garbage) → replaced by the UpdateCamera hook.
- Log grows fast ("Active depth" spam); grep session boundaries.

## Status (2026-08-28)

- Screen-gather SSRTGI **works**. Light capture broken (job-callback ABI → pivot
  to `FUN_181306e60`). Camera hook installed but didn't fire (verify in gameplay,
  correct rotation convention). Feature A composite runtime unconfirmed.
- See `.opencode/context.md` in the `ssrtgi/` project dir for full session state.