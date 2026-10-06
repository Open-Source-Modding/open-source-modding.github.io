# Starfield INI Performance Presets

> Source: the Starfield custom INI preset pack (README and the five preset INIs:
> Ultra, High, Medium, Low, VeryLow).

**Smarter quality, not just less quality.** These presets cut settings you cannot
see and keep the ones you can. The stated goal is that every tier looks as good as
or better than its vanilla counterpart while running faster.

## Philosophy

Vanilla Ultra maxes out everything, including settings where the visual gain from
High to Ultra is imperceptible while the GPU cost is real. These presets target
those diminishing returns. The five tiers are:

| Tier | Target |
|------|--------|
| Ultra | Native-adjacent quality with targeted cuts to imperceptible settings; stated target about 10 to 15 percent GPU savings vs vanilla Ultra, with invisible quality loss |
| High | Balanced performance and quality; stated target about 20 to 25 percent GPU savings vs vanilla Ultra |
| Medium | Aggressive render resolution cut with CAS recovery; stated target is vanilla Medium visuals at about 15 to 20 percent faster |
| Low | Maximum performance while staying visually coherent; stated target is vanilla Low visuals at about 20 to 30 percent faster |
| VeryLow | Absolute minimum GPU cost while still looking like Starfield; for hardware that cannot handle Low, or for maximum FPS; stated target about 40 to 50 percent faster than vanilla Low |

## Free wins (applied to all tiers)

| Feature | What it does | GPU savings | Visual impact |
|---------|--------------|-------------|---------------|
| VRS (Variable Rate Shading) | Reduces shading precision in low-variance areas | 5 to 15% | None: hardware-accelerated on RDNA4/Ada |
| Half-res fog blur | Renders fog blur at half resolution | 2 to 3% | None: indistinguishable |
| Stochastic tiling | Eliminates visible texture repetition | 0% | Better: no more tiling patterns |
| CAS sharpening | Recovers perceived detail from lower render resolution | 0% | Better: sharper than native at lower res |

## Render resolution and CAS strategy

Render resolution is described as the biggest performance lever: lower resolution
means less GPU work, and CAS (Contrast Adaptive Sharpening) recovers perceived
detail so the image looks sharper than the raw resolution suggests.

| Preset | Render scale | CAS | Strategy |
|--------|--------------|-----|----------|
| Ultra | 0.90 | 0.5 | Near-native, light sharpening |
| High | 0.80 | 0.7 | Moderate cut, sweet spot recovery |
| Medium | 0.55 | 0.85 | Aggressive cut, strong recovery |
| Low | 0.60 | 0.85 | Maximum useful CAS before artifacts |
| VeryLow | 0.40 | 0.9 | Minimum resolution, maximum sharpening |

CAS has diminishing returns. Past about 0.85, visible halos start appearing around
high-contrast edges. At Low's render scale CAS can only do so much, and the image
will look softer regardless.

## Installation

1. Download the preset files (`Ultra.ini`, `High.ini`, `Medium.ini`, `Low.ini`,
   `VeryLow.ini`).
2. Place them in the Starfield game root directory, the same folder that contains
   `Starfield.exe`.
3. Set the desired quality tier in `StarfieldPrefs.ini`:

```ini
[Quality]
uGlobalRendererQuality=4   ; Ultra
uGlobalRendererQuality=3   ; High
uGlobalRendererQuality=2   ; Medium
uGlobalRendererQuality=0   ; Low (and VeryLow)
```

VeryLow shares the `0` value with Low and replaces Low for minimum hardware.

## Do not include

The preset files deliberately omit `bDepthOfFieldEnable`, `uMotionBlurQuality` and
`fFilmGrainIntensity`. These are controlled by the player's personal
`StarfieldPrefs.ini`, and including them in a root INI would override that choice.

## Tuning in this order

If more performance is needed, cut settings in this order (biggest impact first):

| Order | Setting | Stated savings |
|-------|---------|----------------|
| 1 | Render resolution | 10 to 25% per step |
| 2 | Volumetric lighting quality | 10 to 15% |
| 3 | Shadow map resolution | 10 to 20% |
| 4 | Global illumination | 10 to 15% |
| 5 | Reflections (SSR) | 5 to 10% |
| 6 | Shadow filtering | 5 to 10% |
| 7 | Contact shadows | 3 to 5% |

## GPU cost ranking

### Tier 1: biggest levers (10 to 25% each)

- Render resolution: linear scaling, the biggest single setting
- Volumetric lighting quality: expensive multi-pass rendering
- Shadow map resolution: 4096 to 1024 is a large bandwidth saving
- Global illumination: indirect lighting computation

### Tier 2: moderate impact (5 to 15% each)

- Reflections (SSR): screen-space reflections
- Shadow filtering quality: soft shadows require multiple samples
- Contact shadows: full-screen per-pixel pass
- VRS: hardware-accelerated, variable savings

## Caveats

These are one author's presets, not official Bethesda settings. Each setting
section can be adjusted independently in game, render resolution can be tweaked in
game without editing an INI, and running Steam Verify will restore the vanilla INIs,
so experimenting is safe.
