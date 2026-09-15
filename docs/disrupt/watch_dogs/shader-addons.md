# Watch Dogs 1 Shader System — Parallellines' Addon Configuration

> **Source**: Discord conversation between Selene (PP2) and Parallellines (2026-07-29 to 2026-09-27).
> Documents WD1 shader architecture, community context, and technical approach.

## Key Facts

- **WD1 ships full shader source** in `shaders.dat` (Ubisoft included the entire shader code)
- **No BC6H support** — WD1 uses DX11 and doesn't support HDR texture compression
- **HDR workaround**: RGBM encoding in textures + shader decode (instead of BC6H)
- **Parallellines' mods are WD1-only** — shader work cannot be ported to WD2 or WDL
- Parallellines plans to open-source shaders eventually ("all mods should be open source")
- Selene has a Blender XBG plugin fork with integrated material editor

## Community Context

- **Mod thief concern**: Strong suspicion that zachimpact/Zuhair is an alt of a banned mod thief
- Parallellines stepped down as moderator (was doing it as a favor,不喜欢 the role)
- Community trust issues around mod attribution and theft
- Epic stream man took over moderation after Parallellines stepped down

## Shader Addon Configuration

Parallellines' `addonconfig.h` — controls which shader addons are enabled during compilation:

### Vanilla-Compatible Addons (no adjustments needed)

```c
// Rain
#define ADDON_FIXRAINALPHA             1 // Fix rain transparency.
#define ADDON_FIXMETHODZERORAIN        1 // Fix NaN rain with auto exposure computation method 0. Requires FIXUNLITEXPOSURE.

// Water
#define ADDON_FIXWATERLIGHTING         1 // Disable fake water shadow attenuation, use proper ambient lighting and fix double waterflow fresnel.
#define ADDON_WATERDEPTHEXP            1 // Smooth water transparency.
#define ADDON_FIXWATERREFLECTION       1 // Fix black spots in water reflection, disable the distant fade hack and disable SSR on the Loop lake.
#define ADDON_WATERPARABOLOIDSSR       1 // Combine paraboloid and screen space reflections for water.

// Post Processing
#define ADDON_FIXNEGATIVECOLORS        1 // Clamp broken colors before tone mapping.
#define ADDON_DITHERING                1 // Fix color banding.
#define ADDON_DEBANDMOTIONBLUR         1 // Jitter motion blur to reduce banding.
#define ADDON_DISABLEBOKEHBOOST        1 // Disable DOF bokeh boost.
#define ADDON_UPDATEFXAA               1 // Use less blurry settings for FXAA.
#define ADDON_UPDATESMAA               1 // Update SMAA to latest version, fix the edge detection pass and use more sensible settings.

// Lighting
#define ADDON_FIXFOGCOLOR              1 // * Fix several color-related issues with fog.
#define ADDON_FIXGLASSLIGHTING         1 // * Fix several issues with glass lighting.
#define ADDON_FIXBLACKTREEREFLECTION   1 // Fix black trees in reflections.
#define ADDON_FIXOCCLUSIONTERM         1 // * Fix several issues with ambient occlusion.
#define ADDON_HORIZONFADE              0 // Use IBL horizon fade for deferred lighting. Same implementation as Legion. May cause edge artifacts.
#define ADDON_REFLECTIONLIGHTINGTWEAK  1 // Use "highcontrast" hack for reflection lighting to make up for the lack of GI.
#define ADDON_AIDENSKINTWEAK           1 // Use Aiden's skin tweak for every character and hair as well.
#define ADDON_ENABLECORNEAAMBIENT      1 // Enable ambient lighting for eye cornea.
#define ADDON_DISABLEFAKEDARKENING     1 // Disable fake car paint darkening.
#define ADDON_DISABLENORMALMASK        1 // Disable masking specular by normal, which fixes dark spots on cars and glass.
#define ADDON_UNLITTWEAKS              1 // Force unlit AffectedByExposure = 1 and AffectedByAutoExposure = 1, and disable FadeOutParams.
#define ADDON_DISABLERIMLIGHT          1 // Disable fake character and cloth rim lighting. Too much of a hassle to disable from materials.
#define ADDON_DISABLEWETREFLECTANCE    1 // Disable WetReflectance parameter. Too much of a hassle to disable from materials.
#define ADDON_FIXGLASSFRESNEL          1 // * Always use 4% reflectance for glass. Too much of a hassle to fix in materials.
#define ADDON_OVERRIDELODFRESNEL       1 // * Allow MaskBlueChannelMode to override LOD building reflectance.
#define ADDON_DISABLEHAIRNOISE         1 // Remove hair noise.
#define ADDON_CUBICLIGHTPROBES         1 // Use cubic filtering for light probes. May cause light leaking.
#define ADDON_FIXLIGHTCOOKIES          1 // Do not sample mipmaps for projected light textures. Fixes aliased edges around lit objects.

// Specular Mask Toggles
#define ADDON_DISABLECARMASK           1 // * Disable specular masking on cars.
#define ADDON_DISABLEGLASSDUSTMASK     1 // Disable specular masking on glass by dust.
#define ADDON_CUSTOMVEGETATIONMASK     1 // Add a bit of specular to vegetation.
#define ADDON_CUSTOMREALTREEMASK       1 // Reduce realtree specular intensity.

// SSAO
#define ADDON_FIXAONANS                1 // Fix NaN values on e.g. fences with MHBAO.
#define ADDON_FIXAONORMALS             1 // Use more accurate reconstructed normals, fixes black dots and overocclusion on vegetation.
#define ADDON_FIXAODOWNSAMPLE          1 // Use nearest depth and normal downsampling.
#define ADDON_XEGTAO                   1 // Replace MHBAO with XeGTAO. Requires FIXAONORMALS and FIXAODOWNSAMPLE.

// Miscellaneous
#define ADDON_FIXLUMCONSTANTS          1 // Use BT.709 luminance constants everywhere.
#define ADDON_FIXSSNEXUSLINES          1 // Fix invisible nexuslines when supersampling is enabled.
```

### Addons Requiring Minor Adjustments

```c
// Post processing
#define ADDON_NEWBLOOM                 1 // * Complete bloom reimplementation. Recommended to use iDownsamplePower = 1 in bloom.lib for better quality.

// Lighting
#define ADDON_BOTTOMREFLECTION         1 // * Enable bottom paraboloid reflection. Requires enabling ParaboloidReflectionRenderDownward in defaultrenderconfig.xml.

// Shadows
#define ADDON_CHSS                     1 // Contact hardening shadows (aka PCSS) and improved filtering for perspective lights.
#define ADDON_SSS                      1 // Screen space shadows.
```

### Addons Requiring Significant Adjustments

```c
// Rain
#define ADDON_NEWRAINLIGHTING          1 // Custom rain lighting using GI.

// Post processing
#define ADDON_HDRLUT                   1 // Use PQ (ST2084) encoded LUTs. Requires FIXNEGATIVECOLORS.

// Lighting
#define ADDON_PBREXTENSION             1 // * Overhaul BRDFs and add metalness support. Requires FIXGLASSLIGHTING and FIXOCCLUSIONTERM.
#define ADDON_BURLEYDIFFUSE            0 // Implement Burley diffuse BRDF (Frostbite version). Requires PBREXTENSION, do not use with ORENNAYARDIFFUSE.
#define ADDON_ORENNAYARDIFFUSE         1 // Implement Oren-Nayar diffuse BRDF (EON version). Requires PBREXTENSION, do not use with BURLEYDIFFUSE.
#define ADDON_DISABLEGLOSSMASK         1 // Disable global reflection masking by gloss. Requires BOTTOMREFLECTION.
#define ADDON_FIXWHITEPOINT            0 // * Fix white point setting affecting the brightness of several materials and effects.

// Specular Mask Toggles
#define ADDON_DISABLEGENERICMASK       1 // * Disable specular masking on most generic objects by default.

// Sky
#define ADDON_HEMISPHERE               1 // Turn cloud ring into a hemisphere.
#define ADDON_ALIGNSKYWITHSUN          1 // Align center of cloud texture with the sun. Requires HEMISPHERE.
#define ADDON_HDRSKY                   1 // Use RGBM encoded cloud textures. Requires HEMISPHERE.
#define ADDON_CUSTOMCLOUDSSS           1 // * Custom subsurface scattering for clouds.
```

### Experimental Addons

```c
// Lighting
#define ADDON_INVERSESQUAREFALLOFF     0 // Use alternative inverse square light falloff function. Looks good in gameplay, but ruins all cutscene spotlights, so they'd need to be reauthored.
```

## Technical Notes

### PBREXTENSION
- Adds GGX specular
- Completely overhauls water BRDF
- Ensures energy conservation between diffuse and specular when possible
- Hair IBL is always disabled (doesn't look good without anisotropy)
- Half lambert is removed from characters and skin, alternative function used for lights
- Metalness has 5 bits of precision and may not work correctly with decals
- No pregenerated mips for filtered importance sampling — gaussian prefilter used instead

### HDR Sky Workflow
- WD doesn't support BC6H (HDR texture compression)
- Parallellines encodes textures with RGBM and decodes in shaders
- HDR sky requires HEMISPHERE addon to be enabled first

### CUSTOMCLOUDSSS
- Made specifically for "The Fall of Windy City 2"
- Won't work elsewhere without code adjustments

## Related Links

- Parallellines' Maniapark profile: https://maniapark.com/usershow/2054
- WD1 shader source: `shaders.dat` (included with game)
- Selene's Blender XBG plugin fork with material editor (in development)
