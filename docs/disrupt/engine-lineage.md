# Disrupt Engine Lineage

The **Disrupt engine** is a fork of **Dunia 2** (Far Cry 3), sharing the same
folder tree, binary object format (`.dat`/`.fat`), and XML conventions. *(A
prior version claimed "open world city management comes from AnvilNext
(Assassin's Creed)" — this is UNVERIFIED: no occurrence of AnvilNext in the
binaries or sources. Removed pending evidence.)*

**Official primary source** (Ubisoft blog, 2013-07-10, "The Secrets Behind
Watch Dogs' Next Gen Experience", Anne Lewis, interviewing Senior Producer
Dominic Guay + Animation Director Colin Graham): Disrupt is described as
"the product of over four years of dedication," built by Ubisoft Montreal,
"specifically to power Watch Dogs." Four pillars: **dynamism** (simulation —
driving, electricity, water, wind, cloth), **impact** (reputation/NPC
reactions), **connectivity** (seamless online, no loading/matchmaking),
**flexibility** (scales PS3/X360 → PC/PS4/XB1). No mention of AnvilNext or
Assassin's Creed — and none of Dunia either (the Dunia-2 connection is
established via binary-format/tooling compatibility, not this source).
Archived: `web.archive.org/web/20140316064550/http://blog.ubi.com/watch-dogs-disrupt-engine-multiplayer/`

Far Cry/Dunia modding tools (Gibbed.Disrupt, FCBastard) work across Disrupt
games because Gibbed's tools were built FOR Dunia (the Dunia toolset,
[github.com/Open-Source-Modding/Gibbed.Dunia](https://github.com/Open-Source-Modding/Gibbed.Dunia))
and the archive format, binary object structure, and file naming conventions
all come from the shared Dunia 2 lineage. The org's Gibbed.Disrupt
description itself states it is "based on the Dunia engine."

## Games

| Game | Year | Platform | Notes |
|------|------|----------|-------|
| **Watch Dogs 1** | 2014 | PC, consoles | First Disrupt title. 32-bit hash namespace (FNV32 truncation of 64-bit). |
| **Watch Dogs 2** | 2016 | PC, consoles | 64-bit hash namespace (FNV64 + CRC64_WD2). GLM text source format for geometry. |
| **Watch Dogs: Legion** | 2020 | PC, consoles | MOEG binary format for compiled geometry. Havok 2017.2.0 chunked format. |

## Format Inheritance

- **Archive format** (`.dat`/`.fat`) is identical across Far Cry 3–6 and all
 Disrupt titles. Tools like Gibbed.Disrupt handle both.
- **Binary object format** (`.fcb`, compiled XML) uses the same serialization.
 `Gibbed.Disrupt.ConvertBinaryObject.exe` works across games (WD2 version
 handles WDL binary objects).
- **XML conventions** for material descriptors, entity definitions, and world
 data follow Dunia 2 patterns.

## Cross-References

- [Asset Pipeline Formats](watch_dogs/asset-pipeline-formats.md) — source-to-runtime format mapping
- [installpackage Patch](installpackage-patch.md) — archive priority hex edit (same across all Disrupt DLLs)
- [Tool Gotchas](tool-gotchas.md) — cross-game tool compatibility issues
