# Watch Dogs Hashing

Watch Dogs games (and the Disrupt engine generally) use several hash
functions to name files, objects, and resources. Two families:

- **FNV** (FNV-1a) — a non-cryptographic hash. **Case-insensitive.**
- **CRC32** — a cyclic redundancy check. **Case-sensitive.**

Both are computed over the same byte string, in either endianness.

> **Cross-reference**: Full WD2 FNV1a x64 algorithm with C code from XeNTaX forum (Ekey, 2016) → [XeNTaX Watch Dogs knowledge §1.3](xentax-watchdogs-knowledge.md)

> **Cross-reference**: WD2 FAT entry bitfield structure (Hash/CompressedSize/Offset/CompressionMethod/UncompressedSize), WD2 FAT first-8-bytes signature analysis, LZ4 dec32_table modification, LZMA constant 0xE6C → [XeNTaX Watch Dogs knowledge §1.3–1.4, §5–7](xentax-watchdogs-knowledge.md)

## Supported functions

| Function | Endianness | Case |
|----------|-----------|------|
| CRC32 | Little Endian | sensitive |
| CRC32 | Big Endian | sensitive |
| FNV32 | Little Endian | insensitive |
| FNV32 | Big Endian | insensitive |
| FNV64 | Little Endian | insensitive |
| FNV64 | Big Endian | insensitive |
| CRC64 (WD2) | Little Endian | insensitive |

> **Note:** FNV32 and FNV64 are **case insensitive** — input is
> lower-cased before hashing — while CRC32 is **case sensitive**. Always
> lowercase (or normalize) input before an FNV lookup; preserve case for
> CRC.

## CRC64 (WD2)

Watch Dogs 2 (and later Disrupt titles) additionally uses a 64-bit hash
colloquially called "CRC64" (in tools like CRC64_WD2), but it is **FNV-1 64
under the hood**, not a CRC — same offset/prime as FNV64, with two tweaks:
path normalization and a top-bit type tag.

Reference implementation (C#):

```csharp
string text = value.Replace("/", "\\").ToLower();
ulong num = 14695981039346656037uL;              // FNV-1 64 offset basis
foreach (char c in text)
{
    num *= 1099511628211L;                        // FNV-1 64 prime
    num ^= c;
}
return (num & 0x1FFFFFFFFFFFFFFFuL) | 0xA000000000000000uL;
```

Decoding the pieces:

- **Input normalization**: `/` → `\` and `.ToLower()` — case-insensitive and
  slash-normalized, so `Meshes/foo` and `MESHES\FOO` hash identically.
  **Case-sensitive variant exists**: tooling also exposes a *raw input* hash
  that skips the `.ToLower()` (case-preserving) — same FNV-1 64 core, same
  mask/tag; only the normalization differs.
- **Core loop**: exactly FNV-1 64 (multiply-then-xor per byte, same prime
  `1099511628211` and offset `14695981039346656037` as the FNV64 row above).
  **Note**: This is FNV-**1** (multiply before XOR), not FNV-1a. The CBR.Disrupt.dll
  class is misnamed `FNV1a64`.
- **61-bit fold**: `num & 0x1FFFFFFFFFFFFFFF` keeps only the low 61 bits
  (top 3 bits cleared).
- **Type tag**: `| 0xA000000000000000` forces the top 3 bits to `101`
  (`0xA` = `1010` in the top nibble). This tags the hash's namespace/type,
  distinct from the raw FNV64 and from the 30-bit fold (`bIs30bit`) noted
  above. Only the low 61 bits carry hash entropy.

**Verified against live tool output** (WD modding ModBot, 2026-08-18) — all
values reproduced exactly:

| Input | WD2 FNV64 (big-endian) | WD1 "FNV32" (big-endian) |
|-------|------------------------|--------------------------|
| `graphics\models\canals_entrances.model` | `A9E52BCAA0661429` | `A0661429` |
| `graphics\models\canals_railings.model` | `A2799BB1325E10A5` | `325E10A5` |
| `{07916e31-9b26-11f1-89ff-45766b9249cc}` | `A3B0C18010419F50` | `10419F50` |

**WD1 "FNV32" is the low 32 bits of the 64-bit hash**, not an independent
FNV-1a 32: in every sample, `WD1 FNV32 == WD2 FNV64 & 0xFFFFFFFF`. A separate
standard FNV-1a 32 (prime `16777619`, offset `2166136261`) does **not**
reproduce the WD1 values — treat the engine's "FNV32" as a truncation of the
64-bit hash, not a standalone 32-bit FNV.

Implications for lookups: to reproduce a WD2 name hash, normalize the path
(`\`, optionally lowercase), FNV-1a 64 it, then apply the 61-bit mask and the
`0xA` tag — do not use a plain CRC64 polynomial or the bare FNV64. For a WD1
name hash, use the low 32 bits of the same computation.

## Implementation detail (Disrupt/WDFusionTools)

The engine's FNV is the **FNV-1a** variant:
- **FNV-1a order**: `hash *= prime; hash ^= byte[i]` (multiply-then-xor).
- **FNV64**: prime `1099511628211`, offset `14695981039346656037`.
- **FNV32**: prime `16777619`, offset `2166136261`.
- **`ReverseBytes()`** is applied to produce the on-disk (byte-flipped)
  "big endian" form. A `bIs30bit` flag additionally folds to 30 bits via
  `(h >> 30) ^ (h & 0x3FFFFFFF)`.
- **CRC32**: standard poly `0xEDB88320`, final XOR inversion, over the
  ASCII bytes of the input.

## Notes

- WD2/WDL use **FNV64** (64-bit) — fewer collisions than WD1's 32-bit
  hashes. Each hash "bucket" offers ~4 billion namespace options.
- The `.lib`/`.fcb` binary-object naming, model/material paths, and the
  Rulesmith hash tables all key off these functions.
- The 7z archive's CRC / pack-stream integrity uses the standard zlib
  CRC32 (little-endian), unrelated to the in-game hashing above.
