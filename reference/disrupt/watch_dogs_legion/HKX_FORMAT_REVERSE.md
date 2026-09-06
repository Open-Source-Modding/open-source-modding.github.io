# HKX (Havok Physics) Dunia Wrapper Format

> **Scope:** WDL Dunia-specific wrapper around Havok TAG0 — chunk structure, content hash,
> build-tag routing. For the TAG0 collision format, see
> [`../watch_dogs_2/wd2-tag0-collision-format.md`](../watch_dogs_2/wd2-tag0-collision-format.md).
> For Havok packfile internals, see [`../havok/hkx_format.md`](../havok/hkx_format.md).
> For Havok 2017.2 class definitions, see
> [`legion-primitive-files.md`](legion-primitive-files.md).

WDL wraps every Havok tagfile in a 16-byte Dunia-specific header that controls format
dispatch, content validation, and build-tag routing. The engine validates this header
before stripping it and passing the remaining payload to Havok's `hkSerialize_Load_toVarInplace`.

> **Source:** Watch Dogs: Legion retail files (EncryptedStudios RE). Cross-referenced with Havok 2015.1 class definitions (235 classes) extracted from WD2's Disrupt_64.dll.
> Local tool references: [HKX2-Enhanced-Library](https://github.com/Monitor221hz/HKX2-Enhanced-Library) (Skyrim SE packfile parser), [HavokDisruptWD2](https://github.com/FranciscoManzanilla/HavokDisruptWD2) (HKX resolver for WD2), [StarfieldMeshConverter](https://github.com/SesamePaste233/StarfieldMeshConverter) (HKX mesh converter)
> (HKX/OBJ/XML converter)

## Dunia Wrapper Header (16 bytes)

| Offset | Type | Field | Description |
|--------|------|-------|-------------|
| +0 | u8 | Class byte | `0x9B` = portable, `0x9C` = platform-compiled |
| +1–3 | u8[3] | Zero padding | Must be `00 00 00` |
| +4 | u32 LE | Content hash | Dunia-internal hash (not CRC32/Adler32/FNV/etc.) |
| +8 | u32 LE | Payload size | Equals TAG0 chunk size (= `file_size - 16` standalone) |
| +12 | u16 LE | Format case | Dispatches handler allocation size |
| +14 | u16 LE | Build tag | Selects which `.hkc` compendium to load |

**Header parsing**: The engine validates the first 4 bytes as a LE DWORD matching
`0x9C` (class byte + zero padding). Non-`0x9C` classes and non-zero padding bytes are
rejected. After validation, the engine strips the 16-byte header and passes the remaining
buffer to Havok's tagfile reader.

> **Note:** WD1/WD2 use a different 16-byte header layout (HeaderField0=`0x89`, HeaderCrc,
> TagfileTotalSize, Reserved) — see [`../watch_dogs/hkx-format.md`](../watch_dogs/hkx-format.md).
> The Dunia wrapper is specific to WDL.

> **Source:** Watch Dogs: Legion retail DLL RE (EncryptedStudios, 2026-08).

### Format case + build tag combinations

| FC (+12) | Tag (+14) | TCRF? | Description                          |
|----------|-----------|-------|--------------------------------------|
| 0 (case 0) | `0x3ACA`  | Yes   | Portable (0x9B and 0x9C)             |
| 0 (case 0) | `0x83DE`  | Yes   | Platform-compiled (0x9C only)        |
| 1 (case 1) | `0x83DE`  | Yes   | Custom variant (0x9C only)           |
| 8 (case 8) | `0xFFFF`  | No    | Self-contained TYPE (no compendium)  |

> **Source:** Watch Dogs: Legion retail files (EncryptedStudios RE).

### Class bytes

| Byte | Meaning |
|------|---------|
| `0x9B` | Portable (platform-independent) |
| `0x9C` | Platform-compiled (x64) — only this class is accepted by the engine |

> **Source:** Watch Dogs: Legion retail files (EncryptedStudios RE).

### Content hash

The u32 at offset +4 is a Dunia-internal hash algorithm — not CRC32, Adler32, FNV, or any
standard algorithm. Hash is consistent across 0x9B/0x9C variants of the same asset. Cooked
pipeline produces a zero hash (`0x00000000`). The retail pipeline computes a non-zero value
using an unknown algorithm.

> **Source:** Watch Dogs: Legion retail files (EncryptedStudios RE).

### Payload size

Always equals the TAG0 chunk size (= `file_size - 16` for standalone files). Verified across
test files. The Havok tagfile size is **uncompressed**.

> **Source:** Watch Dogs: Legion retail files (EncryptedStudios RE).
> Cross-reference: TAG0 chunk format → [`../watch_dogs_2/wd2-tag0-collision-format.md`](../watch_dogs_2/wd2-tag0-collision-format.md).

## Havok Tagfile Structure

After the 16-byte Dunia header is stripped, the remaining payload is a standard Havok tagfile:

```
TAG0                          Container (sibling chunks inside)
  SDKV  (8 bytes)             SDK version string
  DATA  (variable)            Serialized object data
  TCRF  (24 bytes)  OR        Type Compendium Reference (sibling chunk inside TAG0)
  TYPE  (variable)            Self-contained type definitions (replaces TCRF)
  INDX                        Index container:
    ITEM  (N*12 bytes)          Object entries: [type_idx:u24+flags:u8][data_off:u32][data_count:u32]
    PTCH  (variable)            Pointer fixup list
```

**TCRF is a sibling chunk inside TAG0** (alongside DATA, INDX, etc.) — NOT a separate
section after TAG0's closing. See
[`../watch_dogs_2/wd2-tag0-collision-format.md`](../watch_dogs_2/wd2-tag0-collision-format.md)
for chunk walk details, ITEM/PTCH format, and TYPE sub-chunks.

> **Note:** The TAG0 chunk structure (sizeAndFlags BE word, FourCC LE, payload) is documented
> in detail in [`../watch_dogs_2/wd2-tag0-collision-format.md`](../watch_dogs_2/wd2-tag0-collision-format.md).
> The overview above is a summary of the WDL-specific layout.

> **Source:** Cross-referenced with [`../watch_dogs_2/wd2-tag0-collision-format.md`](../watch_dogs_2/wd2-tag0-collision-format.md)
> (WD2 TAG0 format) and [`../havok/hkx_format.md`](../havok/hkx_format.md) (standard Havok packfile).

### SDKV string

WDL uses `SDKV "20170200"` — Havok 2017.2. Portable files use `20170200` without `@`.
Platform-compiled files append `@`. See
[`legion-primitive-files.md`](legion-primitive-files.md) for Havok 2017.2 class definitions.

> **Source:** Watch Dogs: Legion retail files.
> Cross-reference: Havok version table → [`../havok/hkx_format.md`](../havok/hkx_format.md).

## Build tags

| Tag (u16 BE) | Decimal | Compendium file | Notes |
|--------------|---------|-----------------|-------|
| `0x83DE` | 33758 | `33758.hkc` | Retail `legionunpacked` |
| `0x3ACA` | 15050 | `15050.hkc` | Portable `lib_graphic_vehicle` |
| `0xFFFF` | 65535 | (none) | Self-contained TYPE, no compendium |

The build tag selects which `.hkc` compendium file to load. The engine resolves
`generated\physics\<tag_decimal>.hkc` via `ArchiveFS_OpenStream`.

> **Source:** Watch Dogs: Legion retail files (EncryptedStudios RE).

## See also

- [`../havok/hkx_format.md`](../havok/hkx_format.md) — Havok old packfile format, license keys
- [`../watch_dogs/hkx-format.md`](../watch_dogs/hkx-format.md) — WD1/WD2 Havok fork, collision shapes
- [`../watch_dogs_2/wd2-tag0-collision-format.md`](../watch_dogs_2/wd2-tag0-collision-format.md) — WD2 TAG0 chunk walk, ITEM/PTCH, shape graph
- [`legion-primitive-files.md`](legion-primitive-files.md) — Havok 2017.2 primitive files and class definitions
- [`../disrupt/havok-repos.md`](../disrupt/havok-repos.md) — GitHub repos for HKX parsing


### Havok 2015.1 Class Definitions (WD2)

Extracted from `Disrupt_64.dll` — 235 classes with sizes, versions, and parent chains.
Member arrays are in the encrypted `.link` section (VMProtect); full layouts require
a runtime dump.

Sample structs (from `Havok2015_WD2.h`):

```c
struct hkAlignSceneToNodeOptions : public hkReferencedObject {
    // size: 40 bytes, version: 7
};

struct hkGeometry : public hkReferencedObject {
    // size: 48 bytes, version: 7
    // 6 members (encrypted .link)
};

struct hkMeshBody : public hkReferencedObject {
    // size: 440 bytes, version: 7
    // 3 members (encrypted .link)
};

struct hkMeshSystem : public hkReferencedObject {
    // size: 240 bytes, version: 7
    // 1138 members (encrypted .link)
};

struct hknpBodyCinfo {
    // size: 256 bytes, version: 7
    // 22 members (encrypted .link)
};
```

> **Source:** Havok 2015.1 class definitions (235 classes) extracted from WD2's Disrupt_64.dll.
> Full header: `WD2/Havok2015_WD2.h`

---

## Credits

- **Havok SDK documentation** — type layouts and class definitions from the Havok 2017.2 Compendium (referenced via `hkx_format.md`).
- **Watch Dogs Modding Discord** — community research on collision shapes, HKX editing workflows, and vehicle physics injection.
