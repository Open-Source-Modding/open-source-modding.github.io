# Mirror Cubemap Mod (Watch_Dogs 1)

> **Recipe proven by**: Stethri's Nexus mod *"Nikki's Bathroom Mirror Fix"*
> (verified byte-level, then independently reproduced with the official
> converter). Shader analysis guidance: Parallellines.
>
> **Cross-reference**: full format spec →
> [Material Binary Format](material-bin-format.md); installing loose mod files
> → [installpackage-over-patch](../installpackage-patch.md); texture repacking
> → [XBT Texture Format](xbt-texture-format.md)

---

## Background: How DriverGeneric Does Reflections

Most world geometry in WD1 uses the `DriverGeneric` mesh shader. Its
reflection path is fully data-driven from the material:

- The material descriptor
  (`engine/shaders/materialdescriptors/drivergeneric.xml`) declares:

  ```xml
  <parameter name="ReflectionTexture" type="samplerCUBE" platform="PC,DURANGO,ORBIS" />
  ```

- `mesh_drivergeneric.fx` binds it as a cubemap:

  ```hlsl
  PROVIDER_TEXTURE_DECLARE( DECLARE_TEXCUBE, MaterialDriverGeneric, _ReflectionTexture );
  ```

  and the `CUSTOM_REFLECTION` variant samples it with a glossiness-scaled LOD
  plus a Fresnel term derived from the material's `Reflectance`:

  ```hlsl
  float3 reflectionVector = reflect( cameraToVertexWS, normal );
  reflectionTexture = texCUBElod( ReflectionTexture,
      float4( reflectionVector, glossiness * -MaxStaticReflectionMipIndex + MaxStaticReflectionMipIndex ) );
  ```

- In the G-buffer pass, `CUSTOM_REFLECTION` (and `MATCAP`) set
  `gbuffer.isReflectionDynamic = false` and
  `gbuffer.isDeferredReflectionOn = false`, then hand the cubemap straight to
  the lighting pass — i.e. the engine skips its dynamic probe machinery
  entirely and uses **your static cubemap**.

So: give the material a `ReflectionType` that enables the static-cubemap path
and a `ReflectionTexture` pointing at any cubemap XBT, and the surface becomes
reflective.

## ReflectionType Values Observed

| Value | Behavior | Status |
|-------|----------|--------|
| 0     | No custom reflection (engine probes / none) | retail default |
| 1     | Not cleanly tested on DriverGeneric | unverified |
| 2     | Static cubemap reflection from `ReflectionTexture` | ✅ verified |

**Caveat**: semantics are per-shader. Unlit-shader materials ship
`ReflectionType=1` + `ReflectionTexture=citycubemap.xbt` (e.g. the
`sdore2-m-*` road materials) and work fine — there's no evidence value 1 is
inherently bad; it simply isn't part of the verified recipe here. Stick to
value 2 on `DriverGeneric`.

## The Recipe — Exactly Two Changes

Example material:
`graphics/_materials/adolbec-m-2013070356157858.material.bin`
(name `NH_Bathroom_Mirror`, shader `DriverGeneric`, 48 params retail).

1. **`ReflectionType`: 0 → 2**
2. **Insert a new string param `ReflectionTexture`** (type `8`) pointing at a
   cubemap, e.g. `graphics\_textures\cubemap\nh_bathroom_01_d.xbt` —
   **immediately after the `ReflectionType` elem**.

That's it. Resulting XML fragment:

```xml
<Elem>
    <type>5</type>
    <unk1>0</unk1>
    <name>ReflectionType</name>
    <Value>2</Value>
</Elem>
<Elem>
    <type>8</type>
    <unk1>0</unk1>
    <name>ReflectionTexture</name>
    <Value>graphics\_textures\cubemap\nh_bathroom_01_d.xbt</Value>
</Elem>
```

### Leave untouched

- `DiffuseTexture1`
- `Reflectance`
- `WetReflectance`

This is a **minimal-diff principle**, not a safety claim: the proven-working
mod changes only `ReflectionType` + adds `ReflectionTexture`, so those are
the only changes validated end-to-end. A mirror-approach **freeze** we hit
earlier turned out to be a symptom of an *invalid material file* (hand-built
binary with wrong encoding) — not of any particular parameter value. If the
material parses, parameter tweaks like reflectance are fair game; the recipe
above just stays minimal because it's what's proven.

### Order matters

The new `ReflectionTexture` elem goes **right after `ReflectionType`**, not
appended at the end of the command list. Parameter order is part of the file
contract (see [Material Binary Format](material-bin-format.md)).

## Step by Step with ConvertMaterials.exe

Prereqs: [`ConvertMaterials.exe` + `SDL2.dll` + `res/`](material-bin-format.md)
(the Disrupt Editor standalone converter), wine installed, the target
`.material.bin` extracted from the archives, and a cubemap `.xbt`.

```bash
cd /path/to/convert_materials                 # run from the exe directory!

# 1. unpack side: convert retail BIN → XML
cp /extracted/adolbec-m-….material.bin work.material.bin
wine ConvertMaterials.exe work.material.bin   # writes work.material.bin.xml

# 2. edit work.material.bin.xml:
#    ReflectionType Value 0 → 2, insert ReflectionTexture elem after it (above)

# 3. pack side: convert edited XML → BIN
wine ConvertMaterials.exe work.material.bin.xml   # rewrites work.material.bin

# sanity check
ls -la work.material.bin    # ~1056 bytes for this material (1008 + 48)
```

Sanity-check expectations for this material: output grows by exactly one
string param (+48 bytes: header/type/hash/length/path), round-trip of the
*unedited* file differs only in 4 header bytes.

## Packaging the Mod

Loose-file layout (works with the installpackage trick or a mod loader's
workspace mount):

```
workspace/
├── graphics/
│   ├── _materials/
│   │   └── adolbec-m-2013070356157858.material.bin   # edited
│   └── _textures/
│       └── cubemap/
│           └── nh_bathroom_01_d.xbt                  # your cubemap asset
```

Mirror the same relative paths under whichever install route you use — see
[installpackage-over-patch](../installpackage-patch.md) for the DLL hex edit
that makes the game load loose `installpackage/` files without repacking the
patch archive.

## Cubemap Assets You Can Point At

- **WD1 global fallback**: `graphics/_textures/cubemap/citycubemap.xbt` —
  already shipped with every WD1 install (used by the Unlit road materials),
  so it needs no extra asset file.
- **WDL parallax probes**: Watch Dogs: Legion ships per-location probes as
  `worlds/london/localcubemaps/{GUID}_{color|depth|normal}.xbt`.
- Any cubemap XBT works — bake or convert your own via the usual XBT tooling.
  A bathroom-appropriate cubemap (like Stethri's `nh_bathroom_01_d.xbt`) sells
  the effect far better than a generic city sky.

## Applying World-Wide (Batch Notes)

To fix *every* mirror instead of one:

1. Enumerate all `.material.bin` files from the unpacked archives.
2. For each: BIN → XML, then detect candidates — has a `ReflectionType` elem
   (value 0), low/unset reflection params, and mirror-ish names/materials.
3. Apply the two-change recipe; XML → BIN; stage into the mod workspace
   preserving archive paths.
4. **Game-test each batch before mass deployment.** The freeze failure mode is
   silent at build time — only loading the area proves the file is good.

Keep the batch script driving `ConvertMaterials.exe` (not a hand-rolled
serializer) — see the warning in
[Material Binary Format](material-bin-format.md).

## Credits

- **Stethri** — Nexus mod that proved the approach end-to-end.
- **Parallellines** — shader guidance.
- Disrupt Editor converter (`ConvertMaterials.exe`) by j301scott.
