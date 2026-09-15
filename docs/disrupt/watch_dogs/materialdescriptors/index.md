# Material Descriptors

85 XML files covering material formats across Watch Dogs 1, 2, and Legion.

[Browse files on GitHub](https://github.com/open-source-modding/open-source-modding.github.io/tree/main/reference/watch_dogs/materialdescriptors)

## Material System Overview

The material descriptor XMLs define **44 shader types** across Watch Dogs 1, 2,
and Legion. Each shader type specifies which textures, colors, and float
parameters it expects. The compiled binary format is **TAM v7** (magic
`0x004D4154`), with a full reader/writer in
[blender-io-disrupt](https://github.com/Open-Source-Modding/blender-io-disrupt)'s
`material_bin.py`.

A `materialNames.txt` file (916 entries) maps CRC32 hashes to human-readable
parameter names.

### Key Car Shaders

#### DriverCarGeneric

| Parameter | Type | Notes |
|-----------|------|-------|
| DiffuseTexture1 / DiffuseTexture2 | texture | Base diffuse maps |
| NormalTexture1 | texture | Normal map |
| SpecularTexture1 | texture | Specular map |
| AlphaTexture1 | texture | Transparency mask |
| EmissiveTexture | texture | Emissive map |
| SpecularPower | float4 | `z / 8192` = specular exponent |
| Reflectance | float | Default 0.04 |
| ReflectionType | enum | 0=WorldStatic, 1=WorldDynamic, 2=Custom, 3=Matcap |
| EmissiveMeshLights | bool | Enables MeshLights table lookup |

#### DriverCarPaint

| Parameter | Type | Notes |
|-----------|------|-------|
| DamageTexture1 | texture | Damage/detail map |
| DiffuseColor1 / DiffuseColor2 | color | Base paint colors |
| DualCarpaint | bool | Two-tone paint |

**Damage mask** (DamageTexture1 RGBA channels):

| Channel | Effect |
|---------|--------|
| R | DefectsIntensity |
| G | DefectsNormalOffsetX |
| B | DustIntensity |
| A | DefectsNormalOffsetY |

#### DriverGlass

| Parameter | Type | Notes |
|-----------|------|-------|
| TintColor | color | Glass tint |
| Opacity | float | Transparency |
| ReflectionAffectedByDaylight | bool | Day/night reflection variation |

**Mask** (texture RGBA channels):

| Channel | Effect |
|---------|--------|
| R | SpecularGloss |
| G | DustIntensity |
| B | Reflectance |
| A | SpecularOcclusion |

### Cross-References

- [XeNTaX Watch Dogs knowledge — Materials](xentax-watchdogs-knowledge.md) — community format notes
- [WDL material.bin Format](../watch_dogs_legion/material-bin-format.md) — binary format details
- [Asset Pipeline Formats](asset-pipeline-formats.md) — material.xml → material.bin compilation
