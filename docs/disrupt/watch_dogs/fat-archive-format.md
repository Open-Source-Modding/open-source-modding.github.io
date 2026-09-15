# FAT/DAT Archive Format (Disrupt Engine)

Disrupt engine stores game data in `.dat`/`.fat` archive pairs. The FAT (File Allocation Table) index maps hashed filenames to compressed data chunks in the DAT file.

## Format Variants

| Format | Magic | Version | Hash Type | Hash Algorithm | Entry Size | Platforms |
|--------|-------|---------|-----------|----------------|------------|-----------|
| BigFileV2 (FAT2) | `0x46415432` | 8 | u64 | CRC64 | 24 bytes | Xbox 360 (Xenon) |
| BigFileV3 (FAT3) | `0x46415433` | 7–8 | u32 | FNV1a (truncated) | 16 bytes | PC, PS3 |
| BigFileV5 (FAT5) | `0x46415435` | 11 | u32 | FNV1a (truncated) | 16 bytes | PC (WD2), PS4 |

### Hash Differences

- **FAT2 (X360)**: Full 64-bit CRC64 hash — `CRC64(path.ToLowerInvariant())`. No practical collisions in u64 space.
- **FAT3 (WD1 PC)**: 32-bit FNV1a hash — truncated from 64-bit. Hash collisions possible; filelists resolve them.
- **FAT5 (WD2/WDL PC)**: 32-bit FNV1a hash with `0xA000000000000000` tag, 61-bit fold. Same truncation issue as FAT3.

## FAT2 Entry Format (X360 — 24 bytes)

```
Offset  Size  Field
+0x00   u64   NameHash (CRC64 of lowercase path)
+0x08   u32   UncompressedSize
+0x0C   u32   Sentinel (always 0x7F7F7F7F)
+0x10   u32   Packed: (compression_scheme[3 bits] << 29) | (compressed_size[29 bits])
+0x14   u32   Offset into .dat file
```

Compression scheme values: 0=None, 1=LZO1x, 2=Zlib, 3=XMemCompress, 5=LZMA, 6=LZ4LW.

## FAT3 Entry Format (WD1/PS3 — 16 bytes)

```
Offset  Size  Field
+0x00   u32   NameHash (FNV1a, truncated from u64)
+0x04   u32   UncompressedSize
+0x08   u32   CompressedSize (low 29 bits) | compression scheme (high 3 bits)
+0x0C   u32   Offset into .dat file
```

## FAT5 Entry Format (WD2/WDL PC — 16 bytes)

Same layout as FAT3 but different version flags. FAT5 adds localization section support and sanity-check filtering of corrupted entries.

## FAT Header (common across all versions)

```
Offset  Size  Field
+0x00   u32   Magic (0x464154XX)
+0x04   i32   Version (7, 8, or 11)
+0x08   u32   Flags:
              - bits 0–7:   Platform (0=Any, 2=Xenon)
              - bits 8–15:  CompressionVersion
              - bits 16–23: NameHashVersion
+0x0C   u32   EntryCount
+0x10   ...   Entries (16 or 24 bytes each)
```

## Localization Section (FAT5/FAT2)

After the entries, FAT5 and FAT2 archives may contain a localization section:
```
u32    localizationCount
For each:
  u32    nameLength
  char[] name (nameLength bytes)
  u64    unknown
```

## .nfo Files

Each FAT archive has a companion `.nfo` file (XML) that maps hash values to human-readable paths:
```xml
<Root>
  <common>
    <File Path="worlds\windy_city\generated\..." Crc="1234567890" FilePosition="..." FileSize="..." FileTime="..." />
  </common>
</Root>
```

The `.nfo` is essential for unpacking — without it, hashes cannot be resolved to paths.

## Tools

| Tool | Purpose | Notes |
|------|---------|-------|
| Gibbed.Disrupt.Unpack | Unpack .dat/.fat pairs | Requires .nfo for path resolution |
| Gibbed.Disrupt.Pack | Pack folder into .fat/.dat | Does not generate .nfo files |
| Gibbed.WatchDogsX360.RebuildFileLists | Rebuild filelists from X360 FAT2 | Uses BigFileV2, CRC64 hashes |

## Cross-References

- [Archive Load Priority](archive-priorities.md) — per-game archive order
- [Depload Format](../depload-format.md) — dependency preload tables (WD1 32-bit / WD2+WDL 64-bit CRC64)
- [Hashing](hashing.md) — CRC32 / CRC64_WD2 hash algorithms
