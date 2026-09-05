# Skyrim Havok Animation Reference

> **Cross-reference**: BSA v104/v105, NIF format, BSA tools → [XeNTaX Bethesda knowledge §1–3](../unsorted/xentax-bethesda-knowledge.md)

Skyrim (LE/SE/AE) drives its animation system with Havok packfile (`.hkx`)
files built against the **Havok 2010.2.0 SDK**, the same generation of the
engine used by Fallout 3/New Vegas. This document distills the animation
file types, the pack/unpack pipeline, and the community tooling — most of it
from the `hkxcmd` README and the Havok packfile format notes in
`reference/havok/hkx_format.md`. It is written from the formats and the tools,
not from any single implementation.

## Havok Version & File Types

Skyrim's `.hkx` files are Havok *packfile* binaries (magic `0x57e0e057
0x10c0c010`). The packfile version and pointer width differ between the
releases: **LE ships 32-bit packfiles (4-byte pointers)**, while **SE/AE
ship 64-bit packfiles (8-byte pointers), packfile version 8** — version 11
and 64-bit pointers apply to Fallout 4, not Skyrim. Always parse pointer
width from the header (`bytes_in_pointer`), not from the game title.

| Extension | Contents | Notes |
|-----------|----------|-------|
| `.hkx` | Havok packfile binary | Animation clips, skeletons, behavior graphs — same container, different payloads |
| `skeleton.nif` | NetImmerse/Gamebryo rig | The mesh-side bone hierarchy; what NifSkope and the 3ds Max importer read |
| `skeleton.hkx` | `hkaSkeleton` packfile | Havok-side skeleton with reference pose, parent indices, bone names |
| `animation\*.hkx` | `hkaAnimation` packfile | Spline/interleaved compressed animation data |
| `behavior\*.hkx` | `hkbBehaviorGraph` project data | State machines, variables, events, modifiers; **not readable by SDK tooling** |
| `.kf` | Gamebryo keyframe | Interchange format for 3ds Max round-trips |
| `.bsa` | Bethesda archive | Bundled `.nif`/`.hkx`/`.kf` — must be unpacked before tooling |

## Where Animation Files Live

Animation data ships inside `.bsa` archives; **you must unpack the BSAs
first** (BSA Browser, Archive2, or similar) before any tool can touch them.
The archive format itself differs by release: **LE ships BSA v104 (0x68)
with zlib compression, SE ships BSA v105 (0x69) with LZ4 compression**
(UESP "Skyrim Mod:Archive File Format"). Within the extracted meshes tree
the layout is consistent:

```
meshes\actors\<actor>\character assets\skeleton.hkx   (Havok skeleton)
meshes\actors\<actor>\character assets\skeleton.nif   (rig)
meshes\actors\<actor>\animation\*.hkx                 (animation clips)
```

Concrete example from the `hkxcmd` README (the chicken tutorial):

```
meshes\actors\ambient\chicken\character assets\skeleton.hkx
meshes\actors\ambient\chicken\animation\getup.hkx
```

Behavior graph files sit in parallel `behavior` folders (e.g. the human
`0_master.hkx` master graph); the exact per-actor layout varies, but all of
them are Havok Behavior payloads.

## Animation Pipeline

The standard pipeline converts packed HKX → XML HKX (for Havok tools) and
HKX → Gamebryo KF (for 3ds Max), then back. All commands take an input folder
(or file) and recurse into subfolders; without an output folder, output goes
next to the input with an `-out` suffix.

| Step | Command | Purpose |
|------|---------|---------|
| 1 | `hkxcmd convert <in> [out]` | Packed HKX → XML HKX (loadable in Havok tools) |
| 2 | `hkxcmd exportkf <in> [out]` | HKX → Gamebryo `.kf` for 3ds Max / NifSkope |
| 3 | `hkxcmd convertkf <skeleton.hkx> <anim.kf> <out.hkx>` | KF → HKX using a skeleton's bone layout and bind pose; Win32 packed binary by default, `-v:XML` for XML output |
| 4 | `hkxcmd dumptext <skeleton.hkx>` | Dump bone names to text (useful for the Havok export pipeline) |
| 5 | `hkxcmd retarget <...>` | Re-map animation to another skeleton's bone layout — **does not work correctly** |

End-to-end workflow:

1. **Unpack** the BSAs so `.hkx` files are loose on disk.
2. **Convert** `hkxcmd convert "Skyrim - Animations"` → XML HKX.
3. **Preview** in Havok Preview Tool (see below).
4. **Export KF** `hkxcmd exportkf` → `.kf` files.
5. **Import into 3ds Max** via the niftools importer (`Netimmerse/Gamebryo
   (*.nif, *.kf)`). Import the `skeleton.nif` explicitly *before* the mesh
   if animations come in corrupt.
6. **Edit**, add the `start`/`end` note tracks, export as **"Single KF w/o
   NIF"**.
7. **Convert back** `hkxcmd convertkf skeleton.hkx export.kf export.hkx`.
8. **Validate** the result in NifSkope.

## Note-Track Convention

KF duration is carried by note tracks on the root node (`NPC Root [Root]`),
and `exportkf` writes them automatically. When exporting from 3ds Max by
hand you must replicate them:

- Add a **Note Track** to the root node.
- Add a **note at time 0** with text `start`.
- Add a **note on the final frame** with text `end`.

`exportkf` (v1.3+) computes frames as `frame_increment = duration / (nframe - 1)`;
`convertkf` rounds frames back, so the note positions define the clip's
timeline on both sides of the round-trip.

## Havok Preview Tool Workflow

The Preview Tool (from the Havok Content Tools 2010.2.0 install) can display
Skyrim animations, but only the **converted XML** forms — not the packed
originals:

1. Convert `skeleton.hkx` and the animation `.hkx` with `hkxcmd convert`.
2. `File | Open...` → the converted skeleton XML.
3. `File | Add...` → the converted animation XML.

> **"Wrong platform for packfile"** means you selected the **original packed
> skeleton**, not the converted one. Use the `-out` folder's files.

The Preview Tool also chokes on very large XML dumps — `dragon.hkx`, for
instance, fails to load even after conversion.

## Known Limitations

The community tooling is partial, and the gaps are structural, not bugs:

| Limitation | Detail |
|------------|--------|
| Behavior files unreadable | The Havok SDK only ships **Physics and Animation** support; Bethesda's `hkbBehaviorGraph` project data (state machines, variables) cannot be read — "ignore any errors about files it cannot convert" |
| Bone mapping assumed 1:1 | `hkxcmd` assumes a 1:1 skeleton-to-animation-track mapping because the mapping info is not present in the files; this is "obviously not always a correct assumption" |
| Non-uniform scale lost in KF | Havok supports non-uniform scale; Gamebryo does not, so `.kf` export drops that information |
| No visibility/camera tracks | `.kf` export has no handling for visibility or camera tracks, which Skyrim uses |
| `retarget` broken | The retarget command is documented as not working correctly |
| Round-trip is lossy | `exportkf` + `convertkf` does not reproduce the original `.hkx` (hkxcmd issue #1) |
| SDK lock-in | hkxcmd targets the **2010.2.0** SDK; the question of a 2012.2 SDK build is still open (issue #2), and `skeleton.hkx` replacement workflows remain unsolved (issues #4, #5) |

## Packfile Format Notes

For binary-level work, `reference/havok/hkx_format.md` documents the packfile
container (header, section table, fixups). Key Skyrim deltas from that doc:

- Packfile **version 8** (SE/AE) vs 11 (FO4); LE is 32-bit → **4-byte
  pointers**, SE/AE are 64-bit → **8-byte pointers**.
- Contents version string is the `hk_2010.2.0-r1` era for Skyrim, versus
  `hk_2014.1.0-r1` for FO4.
- `hkaSkeleton` in the Skyrim era predates the HK2012+ `partitions` member
  seen in FO4 files.

## Tools

| Tool | Source | Role |
|------|--------|------|
| hkxcmd 1.4 | [figment/hkxcmd](https://github.com/figment/hkxcmd) | Convert packed↔XML HKX, export/convert KF, dump bones |
| NifSkope | [niftools/nifskope](https://github.com/niftools/nifskope) | NetImmerse `.nif`/`.kf` editor & validation |
| HKXPack | [Dexesttp/hkxpack](https://github.com/Dexesttp/hkxpack) | Pack/unpack HKX↔TagXML (the FO4 guide's editor of record); targets 2014.1, 2010.2 noted |
| Havok Content Tools 2010.2 | Intel (free, registration required) | Preview Tool + SDK for the 2010.2 format |
| 3ds Max + niftools exporter | [niftools.org](http://www.niftools.org) | Import/export `.nif`/`.kf` for editing |
| Havok Behavior Tool | [Creation Kit wiki](https://www.creationkit.com/index.php?title=Havok_Behavior_Tool) | Behavior graph editing — **wiki is down for backend maintenance** (last modified 2024-02-07) |

## Sources

- [figment/hkxcmd — Havok Command Line Tools for Skyrim (README)](https://github.com/figment/hkxcmd)
- [hkxcmd README.txt](https://raw.githubusercontent.com/figment/hkxcmd/master/README.txt)
- [hkxcmd issues](https://github.com/figment/hkxcmd/issues)
- [niftools/nifskope](https://github.com/niftools/nifskope)
- [Havok Behavior Tool — Creation Kit wiki](https://www.creationkit.com/index.php?title=Havok_Behavior_Tool)
- [Havok Content Tools (Intel)](http://software.intel.com/sites/havok/en/)
- [UESP — Skyrim Mod:Archive File Format](https://en.uesp.net/w/index.php?title=Skyrim_Mod:Archive_File_Format&action=raw)
- [Dexesttp/hkxpack](https://github.com/Dexesttp/hkxpack)
- [reference/havok/hkx_format.md](../havok/hkx_format.md) — packfile container format (FO4/Skyrim SE header facts)