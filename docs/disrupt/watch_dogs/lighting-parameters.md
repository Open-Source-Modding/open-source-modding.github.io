# Watch Dogs 1 — Environment Lighting Parameters

Reference for `environmentlighting.lib` parameters. These control time-of-day lighting, exposure, GI probes, and reflections.

## Sun/Moon Lighting

| Parameter | Description |
|-----------|-------------|
| `gradSunLightColor` | Projected sunlight color (gradient over ToD) |
| `gradMoonLightColor` | Ambient moonlight color |
| `curveSunLightIntensity` | Projected sunlight intensity (curve over ToD) |
| `curveMoonLightIntensity` | Ambient moonlight intensity |

## Shadows & Reflections

| Parameter | Description |
|-----------|-------------|
| `curveWorldShadowContribution` | Toggles shadows; supports in-between values (not just on/off) |
| `curveStaticReflectionIntensityDay` | Static cubemap reflection intensity (daytime) |
| `curveStaticReflectionIntensityNight` | Static cubemap reflection intensity (nighttime) |
| `curveDynamicReflectionGIInfluence` | How much GI affects dynamic reflections |

## Global Lights

| Parameter | Description |
|-----------|-------------|
| `curveGlobalLightsIntensity0` | On/off switch for light source type 0 |
| `curveGlobalLightsIntensity1` | On/off switch for light source type 1 |
| `curveGlobalLightsIntensity2` | On/off switch for light source type 2 |
| `curveGlobalLightsIntensity3` | On/off switch for light source type 3 |

## GI Probe Parameters

| Parameter | Description |
|-----------|-------------|
| `curveProbeScale` | Global GI probe scale multiplier |
| `curveProbeSkyBounceMultiplier` | Sky bounce light multiplier |
| `curveProbeSkyDirectMultiplier` | Sky direct light multiplier |
| `curveProbeSunBounceMultiplier` | Sun bounce light multiplier |
| `curveProbeSkySaturation` | Sky color saturation in GI |
| `curveProbeLocalLightsMultiplier` | Local artificial light multiplier (exterior) |
| `curveProbeLocalLightsMultiplierInterior` | Local artificial light multiplier (interior) |
| `curveProbeBackgroundAmbientStrength` | Background ambient contribution to probes |

## Exposure (Auto-Exposure)

| Parameter | Description |
|-----------|-------------|
| `curveExposureValue` | Base exposure value |
| `selLuminanceComputationMethod` | Method for measuring scene luminance |
| `curvePercentile` | Luminance percentile threshold |
| `curveTargetLuminance` | Target average luminance |
| `curveSpeed` | Exposure adaptation speed |
| `curveSpeedMultiplier` | Exposure adaptation speed multiplier |
| `fScreenPercentX` | Screen sample region X |
| `fScreenPercentY` | Screen sample region Y |
| `curveMinimumExposureValue` | Minimum exposure clamp |
| `curveMaximumExposureValue` | Maximum exposure clamp |

### Luminance Computation Methods

| Value | Method | Notes |
|-------|--------|-------|
| 0 | Histogram threshold | Used in 2013 beta builds |
| 1 | Histogram average | Used in 360/PS3 retail |
| 2 | *(unused)* | — |
| 3 | GPU average | Next-gen (PC/PS4/XB1) |

## Dunia Legacy Parameters (Unused in WD1)

These carried over from Far Cry 3 (Dunia engine) but are not actively used:

- `gradPaintedGIColor`, `curvePaintedGIIntensity`
- `curveAmbientDirectionalContrast`
- `gradAmbientSkyColor`, `curveAmbientSkyIntensity`
- `gradAmbientGroundColor`, `curveAmbientGroundIntensity`
- `gradAmbientSunFrontColor`, `curveAmbientSunFrontIntensity`
- `gradAmbientSunBackColor`, `curveAmbientSunBackIntensity`
- `curveRimLightIntensity`
- `AmbientOcclusion`

## Cross-References

- [Lighting Pipeline Hooks](lighting-pipeline-hooks.md) — renderer queue RVAs, GPU light structs
- [Shader Addons](shader-addons.md) — Parallellines' lighting fixes
