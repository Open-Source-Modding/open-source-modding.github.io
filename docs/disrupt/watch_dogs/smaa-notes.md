# Watch Dogs — SMAA Notes

> Source: Community research OCR files (`wd_smaa_info*.txt`).

## SMAA Edge Detection

WD1 uses SMAA (Subpixel Morphological Anti-Aliasing) as its primary AA solution. Key technical details:

- **Non-sRGB input**: SMAA edge detection operates on linear (non-sRGB) color data — this is critical for correct edge detection
- **Color/luma deltas**: Edge detection computes max delta across RGB channels in the direct neighborhood, then compares left-left and top-top neighbors to classify edge orientation
- **Parallellines addon**: `ADDON_UPDATESMAA` updates SMAA to latest version, fixes the edge detection pass and uses more sensible settings
- **Combined with FXAA**: `ADDON_UPDATEFXAA` uses less blurry FXAA settings as a fallback

## Shader Source

The SMAA implementation lives in `Engine/Shaders/Private/SMAA/SMAA.ush`. The engine samples neighboring pixels and computes color deltas to detect edges:

```hlsl
// Edge detection: max delta across RGB
float3 Cbottom = SMAASamplePointMMV(colorTex, offset[0].xy, ArrayIndex).rgb;
t = abs(C - Cbottom);
delta.w = max(max(t.r, t.g), t.b);

// Left-left and top-top deltas for double-resolution comparison
float3 Cleftleft = SMAASamplePointMMV(colorTex, offset[2].xy, ArrayIndex).rgb;
t = abs(Cleftleft - C);
delta.z = max(max(t.r, t.g), t.b);

float3 Ctoptop = SMAASamplePointMMV(colorTex, offset[2].zw, ArrayIndex).rgb;
t = abs(Ctoptop - C);
delta.w = max(max(t.r, t.g), t.b);
```

This double-sample approach (comparing at 2× the base offset) helps catch edges that single-sample detection would miss.

## Shader Addons

See [Shader Addons](shader-addons.md) for the full addon reference, including `ADDON_UPDATESMAA` and `ADDON_UPDATEFXAA`.

## Related

- [Shader Addons](shader-addons.md) — Parallellines' addonconfig.h, PBR extension, lighting fixes
- [Shader Editing Workflow](shader-editing-workflow.md) — direct shadersobj replacement vs 3Dmigoto injection
- [Lighting Pipeline Hooks](lighting-pipeline-hooks.md) — renderer queue, GPU structs, deferred lighting
