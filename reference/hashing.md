# Watch Dogs Hashing

Watch Dogs games (and the Disrupt engine generally) use several hash
functions to name files, objects, and resources. Two families:

- **FNV** (FNV-1a) — a non-cryptographic hash. **Case-insensitive.**
- **CRC32** — a cyclic redundancy check. **Case-sensitive.**

Both are computed over the same byte string, in either endianness.

## Supported functions

| Function | Endianness | Case |
|----------|-----------|------|
| CRC32 | Little Endian | sensitive |
| CRC32 | Big Endian | sensitive |
| FNV32 | Little Endian | insensitive |
| FNV32 | Big Endian | insensitive |
| FNV64 | Little Endian | insensitive |
| FNV64 | Big Endian | insensitive |

> **Note:** FNV32 and FNV64 are **case insensitive** — input is
> lower-cased before hashing — while CRC32 is **case sensitive**. Always
> lowercase (or normalize) input before an FNV lookup; preserve case for
> CRC.

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
