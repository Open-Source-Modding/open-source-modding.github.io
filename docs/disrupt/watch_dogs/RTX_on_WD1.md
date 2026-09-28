---
title: Adding raytracing to Watch Dogs 1
---

# Adding raytracing to Watch Dogs 1: feasibility notes

> **Question:** WD1 is DX11. Can you add raytracing with shader code by loading a DX12 DLL like NVIDIA RTX Remix?
>
> **Answer:** No.

## Why the premise is wrong

RTX Remix does not "load a DX12 DLL into the game". What it actually does:

1. You drop a proxy `d3d9.dll` / `d3d11.dll` into the game folder.
2. Remix hooks the existing DX9/DX11 API, intercepts draw calls, and rebuilds the scene in its own graph.
3. The rebuilt scene renders through Remix's own Vulkan-based path tracer (PBRON material system). The game's HLSL gets ignored; only textures and meshes are consumed and reinterpreted as PBR materials.
4. No D3D12 appears anywhere in that chain.

"Adding RT with shader code" therefore does not apply. The RT code lives entirely in Remix, not the game. WD1's DX11 shaders let you map game parameters to Remix materials (content work). They do not let you write ray-traced HLSL.

## Why Remix is a dead end here

- **NVIDIA-only**: Remix needs an RTX GPU (OptiX / hardware RT). This machine has an AMD Radeon RX 9070 XT (RDNA4), so Remix will not run.
- **D3D9-era focus**: Remix targets old DX8/DX9 titles. WD1 is a 2014 D3D11-era Disrupt-engine game whose modern draw-call patterns break Remix's scene-reconstruction assumptions.
- **No dormant RT code**: WD1 has no native D3D12/RT path to enable. Unlike WDL, which ships real D3D12 RT shaders, the 2014 Disrupt engine contains no ray-tracing code at all.

## The options that would work, ranked by effort

1. **Native renderer replacement** (the "Half-Life 2 RTX" approach): reimplement WD1's Disrupt renderer on D3D12/Vulkan with your own RT pipeline. Multi-months-to-years engine project, not a shader edit.
2. **Emulation / forward-port**: run the DX11 path through a custom renderer that converts to Vulkan and injects RT, essentially building your own Remix. Huge, and DXVK-style translation on RDNA4 hits the same VKD3D lockup issues documented in `~/AGENTS.md`.
3. **Engine hooks / dormant RT**: not applicable. The 2014 engine has none.

## Historical note: software raytracing predates GPU RT

Raytracing did not wait for dedicated hardware. In 2007 IBM demonstrated an interactive ray tracer running on a cluster of IBM QS20 Cell blades ([video](https://www.youtube.com/watch?v=zKqZKXwop5E)):

- The demo renders a model of over 300,000 triangles at 60+ fps at 1080p, using 14 Cell processors.
- It progresses from ray casting with Phong shading, through shadows and secondary rays, to reflective/refractive bounces and ambient occlusion (the 80% ambient / 20% direct shader simulating a cloudy day).
- The same scalable tracer runs interactively on a single Linux PlayStation 3 using 6 SPEs.
- The raytracer source was published at `http://www.alphaworks.ibm.com/tech/irt`.

The lesson for WD1: throughput was never the blocker. Raytracing ran at interactive rates on general-purpose CPUs two decades ago. The blockers for WD1 are integration (no RT hooks in the Disrupt renderer) and hardware API access (Remix needs NVIDIA). A CPU tracer proves nothing about either.

## Bottom line

For WD1 on this hardware, RT via Remix or shader-level injection is not practical. The realistic routes are (a) a full custom renderer port, or (b) staying with rasterized shader mods. Compare with WDL, which already ships a native D3D12 RT path you can tune via `engine/settings/defaultrenderconfig.xml` and the DX12 shader objects. That is the tractable RT target.
