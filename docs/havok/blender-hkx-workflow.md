# Blender HKX Workflow Without the Havok SDK

Goal: import and export Havok data in Blender without obtaining the proprietary SDK. This page records which parts of that pipeline are already solved, which parts still need SDK binaries, and how much of the result is actually proven.

The SDK exists to do two things that HKX tooling cannot skip: serialize class objects according to a version-specific layout, and compress animation data with Havok's spline codec. Everything below works around one or both.

## What needs the SDK

| Task | SDK needed | Why |
|---|---|---|
| Read a packfile (HKX to XML or JSON) | No | The class layout is recoverable from the file's own type data or from the game's shipped class definitions |
| Write a packfile (XML back to HKX) | Sometimes | Needs a serializer that matches the target version and its class layout |
| Reverse the tick/spline compression | No | `HavokLib` ships a compressor and decompressor in C++ (GPL, read it, do not copy it) |
| Convert XML to binary with `AssetCc` | Yes | `AssetCc` is an SDK tool, and `AssetCc2` from hk2013 only handles `hk_2013.2.0-r1`, not `hk_2013.1.0-r1` |
| Convert binary to XML in bulk | Yes | Same tools |

## Blender addons in play

**blender-io-disrupt** covers HKX collision for the Watch Dogs games. It imports collision, lets you edit it, and re-injects it. Its test suite asserts byte-identical re-injection for Watch Dogs 1 and Watch Dogs 2, which is the strongest correctness evidence available for any HKX writer outside the SDK. Legion import is read-only, and the addon's own notes say export targets displacement rather than a full rebuild.

**blender-hkx** (Jonas Gernandt, MIT) is the other Blender addon and handles animation instead of collision. It ships as two pieces: a Python addon and a separate C++ converter. The C++ half must be compiled against the Havok 2010.2-r1 SDK and pugixml on Visual Studio 2019, so the addon is not usable as downloaded. It writes both 32-bit and 64-bit packfiles and covers regular, paired and additive animation plus annotations and float tracks. A port to Fallout 4's 2014.1.0 format is documented as a plan in `FO4_PORT.md`, with no round-trip results behind it.

Character meshes are out of scope for both addons. The Disrupt Blender addon's `.xbg` and `.glm` paths handle models, not `.hkx`.

## SDK-free tools by task

**Packfile pack and unpack.** `hkxpack` and `hkxpack-plus` (Java) convert HKX to TagXML and back with no SDK. Their input is the game's own classXML definitions, so `hkxpack-plus`, which bundles 910 class definitions, is the better starting point when you do not want to extract them. TagXML is not interchangeable with the plain XML that `hkxcmd` and the Noesis `damnhavok` plugin read.

**Packfile reading.** `HavokLib` (PredatorCZ, GPL v3) reads packfiles from 5.0.0 through 2017 in both endiannesses, animation and skeleton classes only. It will not read tag files by design. `hkxparse` uses a JSON layout description, so a 2017.2 layout generated from a game's shipped data would extend it to newer titles.

**Packfile writing.** The `HKX2 Enhanced Library` (C#) serializes the Skyrim SE packfile to XML natively, then shells out to `hkxcmd` for the reverse direction. That hand-off is where the SDK re-enters, because `hkxcmd` builds against the 2010.2-r1 SDK. For Watch Dogs collision, `blender-io-disrupt` avoids the problem entirely with its own writer.

**Tag files.** `tagtools` converts tag files up to 2012 into 2016 binary form without the SDK, which matters for titles whose tag data predates the 2015 chunked container.

**3ds Max route.** `hkxImport` ships one `HavokMax.dlu` per SDK year from `x64_2010` to `x64_2023`. Those DLLs link against the matching Havok runtime, which makes them a source of class layouts for versions where no header set is available. The `HKX_Import_Script` package drives the same DLLs from a MAXScript.

## Starting points per game

| Game | Havok version | SDK-free route | Known limit |
|---|---|---|---|
| Watch Dogs 1 and 2 | Disrupt-era packfiles | `blender-io-disrupt` collision, byte-identical on export | Mesh HKX still needs its own parser |
| Watch Dogs: Legion | 2017.2 retail, 2015.1 leak, chunked | `StarfieldMeshConverter`'s parser handles the 2017.2 family | Import only in the Blender addon |
| Fallout 4, Dark Souls 2 and 3 | 2014.1.0-r1, packfile 11 | `hkxpack` with game classXML, `havok2fbx` for conversion | Instance and mesh exports are not covered by either |
| Skyrim, Dark Souls 1 | 2010.2.0, packfile 8 to 9 | `hkxpack`, `hkxcmd` | Animation writing requires the SDK build of `blender-hkx` |
| Starfield | 2019.02, chunked | `StarfieldMeshConverter` (export only) | No importer |

Version and packfile numbers per title are in the compatibility table on [HKX format](hkx_format.md).

## The evidence gap

No public record shows a modified HKX file loading inside a shipping game. Every claim in this pipeline stops at one of three weaker levels: a byte-identical round trip, a successful parse, or an import that produced plausible geometry in Blender. The Disrupt prototype notes say so directly, and the byte-identical tests in `blender-io-disrupt` are a round-trip property, not proof that the game accepts the file.

Treat this as the open question to answer before documenting any Blender HKX workflow as complete. Loading a re-injected collision file in Watch Dogs and confirming collision changes in game would settle it.

## Header reference

Version-specific class layouts for the 2012.2.0-r1, 2013.1.0-r1, 2014.1.0-r1 and 2018.1.0-r1 SDK releases can be read straight out of the headers that ship with each release (`Common/Base/Config/hkConfigVersion.h` identifies a tree, and the class declarations live beside their implementation). Deduplicated reference trees from those releases, with build files and binaries removed so nothing compiles, are useful as a reading aid only, and are not published here.
