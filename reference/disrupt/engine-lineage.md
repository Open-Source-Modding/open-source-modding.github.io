# Disrupt Engine Lineage

The **Disrupt engine** is a fork of **Dunia 2** (Far Cry 3), sharing the same
folder tree, binary object format (`.dat`/`.fat`), and XML conventions. Open
world city management comes from **AnvilNext** (Assassin's Creed).

This lineage is why Far Cry modding tools (Gibbed.Disrupt, FCBastard) work
across Disrupt games — the archive format, binary object structure, and file
naming conventions are inherited from Dunia 2.

## Games

| Game | Year | Platform | Notes |
|------|------|----------|-------|
| **Watch Dogs 1** | 2014 | PC, consoles | First Disrupt title. 32-bit hash namespace (FNV32 truncation of 64-bit). |
| **Watch Dogs 2** | 2016 | PC, consoles | 64-bit hash namespace (FNV64 + CRC64_WD2). GLM text source format for geometry. |
| **Watch Dogs: Legion** | 2020 | PC, consoles | MOEG binary format for compiled geometry. Havok 2017.2.0 chunked format. |

## Format Inheritance

Because Disrupt descends from Dunia 2:

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
