# Watch Dogs 1 — Additive Shader Modding Spec

> **Status**: Draft v0.1 (2026-10-04). Part 1 and Part 4 are verified; Part 2 mechanism is
> under active reverse engineering; Part 3 design is based on decompiled donor code but not
> yet proven in-game. Parts marked *unverified* must not be treated as stable API.
>
> **Purpose**: let shader mods coexist. Today TFOWC2 and Dramatic Realism Realized (DRR)
> each replace the entire `shadersobj` archive, so they are mutually exclusive by
> construction. This spec defines how a mod adds shaders *without* replacing the archive.

## Problem statement

`shadersobj.dat` holds ~39,131 compiled shader permutations. Existing shader mods ship the
whole archive as one NexusTools pack: enabling one pack shadows every file beneath it.
Two mods that both rewrite `shadersobj` cannot coexist — not because their features
conflict, but because their delivery mechanism is all-or-nothing.

Meanwhile non-shader mods already coexist fine: Particle System Overhaul (PSO) ships a
single `windy_city_deploymentparticles.rml` through the NexusTools `workspace/` overlay and
never touches `shadersobj`. The community also has a material-level contract (TFOWC2's PBR
materials) that shader mods already share. The missing pieces are: a documented material
contract, a way to *add* shader families instead of replacing them, and delivery rules.

## Part 1 — Material contract (existing, verified)

TFOWC2 established the de facto material spec by adding PBR materials. Any shader mod that
reads these channels the same way can consume the same material files.

### DriverGeneric specular texture RGBA

| Channel | Meaning | Gate flag |
|---|---|---|
| Red | Glossiness | `MaskRedChannelMode=0` → red unused; `SpecularPower`/`Wetness` first value used instead |
| Green | Colorize mask | `UseColorizeDiffuse=0` → green unused |
| Blue | Reflectance | `MaskBlueChannelMode=0` → blue unused; `Reflectance` first value used instead |
| Alpha | Specular occlusion | `MaskAlphaChannelMode=1` → alpha becomes wetness mask instead |

Flags that rewire the channels: `SwapSpecularGlossAndOcclusion` (swap red ↔ alpha first),
`ColorizeDiffuseMode` (use green specular for colorization), `InvertMaskForColorize`.

### Metalness recipe (TFOWC2 extension)

- `ReflectionType = 1` (dynamic reflection; matcap elements erased)
- `MaskAlphaChannelMode` disabled, `SwapSpecularGlossAndOcclusion` disabled
- `SpecularPower.y = 8190` — entire material metallic
- `SpecularPower.y = 8191` — specular texture alpha channel becomes a binary metalness
  mask (white = metal, black = not metal; no grey values)
- Reference materials: deagle/tommy gun
- Non-metal reflectance baseline: `0.04` (F0 for n≈1.5)

### Vanilla-compatible fallback

For materials that must also work under stock shaders:

- `ReflectionType = 0` (static reflection — stock has no bottom paraboloid hemisphere,
  dynamic path renders black)
- Diffuse completely black on metallic surfaces
- Reflectance set to the metal's F0 colour: steel ≈ 0.67, aluminium ≈ 0.92,
  silver ≈ 0.97 (greys only — coloured metals are not expressible this way)
- Brushed/rough metals render dim under stock specular
- Fresnel values: physicallybased.info; SpecularPower curves: community Desmos graphs
  (see #wd1_resources, Parallellines)

**Status**: DRR implements this contract (PBR extension addons) → materials are already
interchangeable between TFOWC2 and DRR. Material compatibility is *not* the blocker.

## Part 2 — Engine-side additive shader families (DX11) — UNVERIFIED, under RE

Goal: a mod registers **new** shader families that the engine loads, permutates, and
dispatches like native ones, without shipping the existing 39,131 files.

- `fastinitdata.bin` is the family registry (143 family base-IDs in the top byte of the
  shader ID, plus per-family permutation membership). The engine only loads families it
  finds there.
- New family IDs ⇒ new shader filenames (`obj/hXX/pixel_*.pso`) ⇒ **zero filename overlap`
  with any full-replacement pack. TFOWC2/DRR contents are untouched underneath.
- The engine's per-family C++ bindings decide which SRVs/UAVs a family's shaders get.
  That machinery is free for lighting/material-style work but is a *hard constraint* for
  ray-tracing shaders, which need UAV bindings the engine never sets up. RT additions
  belong to Part 3.
- Known existing evidence: `LightProbesUpdate` permutations are fixed in the C++
  `CLightProbeRenderer` constructor ("IF YOU ADD NEW VARIATIONS HERE, PLEASE PRE-FETCH
  THEM IN THE CLightProbeRenderer CONSTRUCTOR") — families with engine-side prefetch have
  registration constraints beyond the registry file.

**Open question (active RE)**: the runtime path that parses `fastinitdata.bin` and creates
family entries — whether it can be extended from an ASI at load time, or whether the
registry file itself must be edited. Findings will be added here when verified.

## Part 3 — Hook-owned loader (DX12/RT) — design, not yet proven in-game

Goal: ray-tracing shaders live entirely outside the DX11 `shadersobj` world.

- Delivery: an ASI (loaded by NexusTools' ASI loader) hooks the renderer's frame-graph
  setup seam — `CSceneRendererFrameGraph::SetupFrameGraph` / `PrepareSetupFrameGraph` —
  the same hook surface ShadowEnginePatch already uses in retail WD1.
- The hook owns its shader pipeline: loads its own compiled images (DXIL/SM6) from its own
  archive or loose directory, owns permutation lookup, owns dispatch, and owns UAV/SRV
  binding. This is the part that makes RT possible at all, since RT compute needs UAV
  bindings the DX11 family machinery never exposes.
- Because it never enters `shadersobj.dat`, it cannot collide with TFOWC2, DRR, or any
  other DX11 shader mod — incompatibility is structurally impossible, not just avoided.
- The donor frame graph (WDL-class renderer) uses packed job keys (`EFrameJobType`:
  low bits = job, high bits = slot) that match the WD1 pass-table scheme, so grafting
  donor passes maps onto existing WD1 pass IDs rather than requiring a new system.

## Part 4 — Delivery conventions (verified against current mod ecosystem)

1. **Additive mods declare themselves.** `modconfig.json` lists what the mod adds and any
   `incompatibleMods`. A mod that adds shader families lists the IDs it claims.
2. **Never overwrite existing family IDs or existing archive files.** Additive means
   additive: a spec-conforming mod ships only files it owns.
3. **Data mods use the `workspace/` overlay**, not archive replacement
   (`mods/<name>/workspace/worlds/...` — the PSO pattern).
4. **Load order is NexusTools order.** Mods must behave deterministically under any order;
   when order matters it goes in `incompatibleMods`, not folklore.
5. **Loose resources ship declared.** Community practice already includes
   `Modders_Resource.xml` bundles with credit requirements (e.g. Backfire mod); the spec
   formalizes "declare bundled resources + attribution" in the README.
6. **Full-replacement mods remain legal.** TFOWC2/DRR-style packs keep working; they just
   declare themselves incompatible with each other exactly as today.

## Why this is believed to work (evidence, not yet proof)

- PSO coexists with TFOWC2 today via workspace overlay — non-overlapping delivery already
  produces compatibility in practice.
- TFOWC2 and DRR share the Part 1 material contract and are meant to be interchangeable
  shader-for-shader; their only conflict is the delivery mechanism this spec replaces.
- ShadowEnginePatch proves the frame-graph hook surface is live and stable in retail WD1.
- DX12/DXIL RT shaders and DX11 fxc shaders occupy disjoint namespaces by format — a
  pack can contain one without the other.

## Status / next steps

| Part | Status |
|---|---|
| 1 — Material contract | Verified; DRR conforms |
| 2 — Engine-side families | Registry understood; runtime extension path under RE |
| 3 — Hook-owned loader | Design fixed; seam decompiled; not yet in-game |
| 4 — Delivery conventions | Verified against PSO/Backfire/NexusTools practice |

Nothing in this document should ship as a "spec 1.0" until Part 2 and Part 3 each have one
end-to-end in-game demonstration.
