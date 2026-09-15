# Watch Dogs Material Descriptors

> **Source**: Material descriptor XML files from WD1 retail (`shaders_unpack/`) and Ubisoft leak (`data/engine/shaders/materialdescriptors/`).
> **Confidence**: HIGH — these are the actual shader parameter schema files used by the Disrupt engine.

---

## Overview

Material descriptors define the **parameter schema** for each shader type in the Disrupt engine. They tell the engine what textures, colors, floats, and other parameters a material can have, and how they map to the underlying HLSL shader. Each `.material.bin` file provides the actual values; the descriptor defines what values are valid.

### Relationship to .material.bin

```
materialdescriptor.xml    (schema - defines what parameters exist)
    |  CompileMaterials.exe
.material.bin              (runtime - stores actual parameter values)
```

The descriptor is like a class definition; the `.material.bin` is an instance. A material descriptor named `DriverCarGeneric` defines that car paint materials can have `DiffuseTexture1`, `NormalTexture1`, `SpecularPower`, etc. Each car's `.material.bin` stores the actual texture paths and numeric values for those parameters.

### File Locations

| Source | Count | Path |
|--------|-------|------|
| WD1 retail | 46 | `engine/shaders/materialdescriptors/` |
| Ubisoft leak | 94 | `data/engine/shaders/materialdescriptors/` |

24 descriptors are shared by name between WD1 and the leak. WD1 has 22 unique descriptors not in the leak.

---

## XML Structure

The root element is `<materialdescriptor>` with attributes for name, display name, UI visibility, and a unique ID hash. It contains:

- `<geometryInputs>` — vertex data requirements
- `<parameter>` — individual parameter definitions (textures, floats, colors, bools)
- `<parameterprovider>` — maps parameters to HLSL shader files
- `<ui>` — Disrupt Editor material property panel layout

### Root Attributes

| Attribute | Type | Description |
|-----------|------|-------------|
| `name` | string | Shader/material type name (e.g. `DriverCarGeneric`) |
| `displayname` | string | Human-readable display name |
| `showinui` | bool | Whether to show in the Disrupt Editor material UI |
| `id` | string | Two u32 hashes as `#(hash_lo, hash_hi)` — unique material type identifier |

### geometryInputs

Defines what vertex data the shader expects from meshes:

| Attribute | Type | Description |
|-----------|------|-------------|
| `normal` | bool | Requires vertex normals |
| `tangent` | bool | Requires vertex tangents |
| `binormal` | bool | Requires vertex binormals |
| `color` | bool | Requires vertex colors |
| `uvChannels` | int | Number of UV channels required |
| `packedFirstUV` | bool | First UV channel is packed |
| `bonecount` | int | Number of bone weights per vertex |
| `reduceMatrixPerDrawCallBy` | int | Skinning optimization |
| `packedUChannel` | int | Packed UV channel count |

---

## Parameter Types

| Type | Description | Example Default |
|------|-------------|-----------------|
| `sampler2D` | 2D texture (`.xbt` file path) | `graphics/_textures/default.xbt` |
| `samplerCUBE` | Cube map texture | — |
| `samplerState` | Texture sampling mode | `ColorWrap` |
| `float` | Single float | `16` |
| `float2` | Two-component vector | `1,1` |
| `float3` | Three-component vector | `1,1,1` |
| `float4` | Four-component vector | `1,1,1,1` |
| `color3` | RGB color (3 floats) | `1,1,1` |
| `color4` | RGBA color (4 floats) | `0.5,0.5,0.5,0` |
| `int` | Integer enum value | `0` |
| `bool` | Boolean flag | `0` or `1` |

### Texture Parameter Attributes

| Attribute | Description |
|-----------|-------------|
| `tilingName` | Name of the associated tiling parameter |
| `UvName` | Name of the UV channel parameter |
| `uvTilingName` | Name of the UV tiling parameter |
| `normalmap` | Marks this as a normal map texture |
| `optional` | Whether the texture is optional (`0` = required) |
| `maxdisplayname` | Display name in the editor UI |
| `platform` | Platform restriction (e.g. `PC,DURANGO,ORBIS`) |

### Numeric Parameter Attributes

| Attribute | Description |
|-----------|-------------|
| `defaultvalue` | Default value as comma-separated floats |
| `provideronly` | If `1`, computed by the parameter provider (not user-editable) |
| `ignore` | If `1`, ignored by the engine |
| `op_z` | Expression for z-component (e.g. `x/8192`) |
| `op_w` | Expression for w-component (e.g. `y/8192`) |
| `div` | Division factor for packed values |
| `use_if` | Conditional parameter (e.g. `parameter="ReflectionType" value="2"`) |

### samplerState Values

| Value | Description |
|-------|-------------|
| `ColorWrap` | Wrap/repeat sampling |
| `ColorClamp` | Clamp to edge |
| `ColorMirror` | Mirror repeat |
| `ColorWrapAniso` | Anisotropic filtering (expensive) |

---

## Parameter Provider

The `<parameterprovider>` maps descriptor parameters to the HLSL shader file:

- `parametersfilename` = the HLSL shader file that consumes these parameters
- `<alias>` = alternate parameter name for the shader (e.g. point-sampled variant)
- `platform` attribute restricts parameters to specific platforms

## UI Section

The `<ui>` section defines the Disrupt Editor material property panel. Uses `<xi:include>` for shared UI components (DataClasses, ReflectanceGroup, SpecularPowerGroup).

### UI Controls

| Element | Description |
|---------|-------------|
| `<button>` | Texture picker (with `fill="1"` to stretch) |
| `<textbox>` | Numeric input field |
| `<checkbox>` | Boolean toggle |
| `<combobox>` | Dropdown selection |
| `<slider>` | Numeric slider with min/max |
| `<label>` | Static text label |
| `<group>` | Layout container (with `horizontal="1"` for side-by-side) |

### Conditional UI

- `show_if` — show element when condition is true (e.g. `show_if="ReflectionType==2"`)
- `enable_if` — enable element when condition is true (e.g. `enable_if="EmissiveMeshLights"`)
- `hide_if` — hide element when condition is true

---

## WD1-Specific Descriptors

These 22 descriptors exist in WD1 but not in the leak (WD1-only features):

| Name | Purpose | Notable Parameters |
|------|---------|-------------------|
| `AloneBarrier` | Water barrier shader | `WaterColor`, `FlowTexture`, `OverlayTexture`, `LocalWetness` |
| `Character` | Character skin/clothing | `UseSkinSSS`, `SkinSSSColor`, `CharacterProfile` |
| `Cloth` | Cloth simulation | `ClothWrinkleTargetParts`, `ClothWind` |
| `Decal` | Decal overlay | `DecalTexture`, `BlendingType` |
| `Dots` | Dot pattern effect | `DotPattern`, `DotColor` |
| `DriverGeneric` | Vehicle generic | `MaskGreenChannelMode` |
| `DriverNexusMon` | Nexus monitor display | `ScreenTexture` |
| `DriverSpline` | Road spline rendering | `RoadTexture`, `LaneMarkings` |
| `Generic` | General-purpose | Full PBR-like pipeline |
| `Glass` | Glass/transparent | `ReflectionTexture`, `RefractionPower` |
| `Hair` | Hair rendering | `HairStrandTexture`, `AlphaTestEnabled` |
| `Leaf` | Vegetation leaves | `WindAnimation`, `DoubleSided` |
| `Rain` | Rain effect | `RaindropTexture`, `RainIntensity` |
| `RealtreeTrunk` | Tree trunk bark | `BarkTexture`, `NormalIntensity` |
| `Road` | Road surface | `RoadTexture`, `LaneMarkings` |
| `Skin` | Human skin | `UseSkinSSS`, `SkinSSSColor`, `SubsurfaceProfile` |
| `Starsphere` | Sky/stars | `StarsphereTexture` |
| `Template` | Material template | Minimal parameters |
| `UI` | User interface | `UITexture` |
| `Vehicle` | Vehicle body | `CarPaintColor`, `ClearCoat` |
| `VolumeAmbientOcclusion` | AO volume | `AOTexture` |
| `Water/WaterRiver` | Water surfaces | `WaterNormal`, `WaterDepth`, `FoamTexture` |
| `Weapon` | Weapon rendering | `MetalnessTexture`, `WearTexture` |

### WD1-Specific Features

- **`MaskGreenChannelMode`** — selective colorization (DriverGeneric, Vehicle)
- **`UseSkinSSS`** — screen-space subsurface scattering (Character, Skin)
- **`LocalWetness`** system — per-pixel wetness blending (AloneBarrier, Road)
- **`WindAnimation`** — vertex shader wind displacement (Leaf, Vegetation)

---

## Shared Descriptors (WD1 + Leak)

24 descriptors exist in both. Key shared names: Background, Blended, DetectorVolume, DisruptCliff, DriverBuilding, DriverCarGeneric, DriverCarPaint, DriverCloth, DriverEye, DriverGlass, DriverHair, DriverMinimap, DriverVegetation, DriverWater, DriverWaterDecal, DriverWire, Error, FogVolume, Highlight, NeonSign, NetHackingBackground, NetHackingGeneric, NetHackingIntersect, NetHackingVolume, RealTreeDriverLeaf, SkinnedHologram, TerrainLayer, Unlit, VehicleLightHalo, VolumetricClouds, VolumetricCloudsControl_Layers.

## Leaked Descriptors (Not in WD1)

The leak includes 48 additional descriptors for WD2/WDL: WD2AnimalFur, WD2Building, WD2CarGeneric, WD2CarPaint, WD2Character, WD2CharacterEye, WD2CharacterHair, WD2CharacterSkin, WD2Cloth, WD2Glass, WD2Ground, WD2Road, WD2Water, WDLCharacterHair, WDLCharacterSkin, and 33 more.

---

## How Material Descriptors Relate to .material.bin

The descriptor defines the schema; the `.material.bin` stores the values:

1. **Descriptor**: `DriverCarGeneric.xml` defines that car materials have `DiffuseTexture1` (sampler2D), `DiffuseColor1` (color3), `SpecularPower` (float4), etc.
2. **Material .bin**: Each car's `body.material.bin` stores the actual texture path, color values, and specular power.
3. **Engine**: At load time, the engine reads the descriptor to know the parameter layout, then reads the `.material.bin` to get the values, and binds them to the HLSL shader.

### Compile Pipeline

```
material.xml              (source - parameter values + descriptor reference)
materialdescriptor.xml    (schema - parameter definitions)
    |  CompileMaterials.exe
material.bin              (runtime - binary packed parameter values)
```

The compiler reads the descriptor to know parameter types and sizes, then packs the XML values into the binary format. The descriptor's `id` hash is embedded in the `.material.bin` header so the engine can look up the correct shader.

---

## Related

- [Material Binary Format](material-bin-format.md) — `.material.bin` binary structure
- [Material Hex Editing](material-hex-editing.md) — hex workflow for material editing
- [Shader Editing Workflow](shader-editing-workflow.md) — shader compilation pipeline
- [Shader Addons](shader-addons.md) — Parallellines' addon configuration
