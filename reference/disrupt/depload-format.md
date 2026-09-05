# Depload Format (Dependency Preload Manifest)

> **Cross-reference**: Hash algorithms used throughout → [Watch Dogs Hashing](watch_dogs/hashing.md);
> loose-file install route → [installpackage-over-patch DLL Hex Edit](installpackage-patch.md)

`.depload.dat` files are the Disrupt engine's dependency-load manifests: for every
resource container (model, animation, entity payload…) they list the hashed paths of
the resources it needs, so the streaming system can preload them ahead of demand.
They pair with the entitylibrary — elib defines *what exists*, depload caches *what
it loads*.

## File locations

| Game | Files |
|------|-------|
| WD1 | `worlds/windy_city/generated/windy_city_depload.dat` |
| WD2 | `worlds/san_francisco/generated/san_francisco_depload.dat` |
| WDL | `worlds/london/london/worlds/london/generated/london_depload.dat` + `common/generated/common_depload.dat` |

WDL additionally ships a vehicle side-mount depload
(`lib_graphic_vehicle_depload`, seen as a 33 MB converted XML).

## Three variants

Two format generations exist. All integers are little-endian.

### WD1 — 32-bit era

Starts directly with the u32 count (no version byte). Parse consumes the file
exactly.

```
u32 containerCount                        (WD1: 96,927)
count × 12-byte record:
    u32 startIndex      → index into the dep-hash/dep-type arrays
    u32 depCount        → number of consecutive deps starting there
    u32 fileHash        → 32-bit TRUNCATED FNV1a64 of the container path
u32 depHashCount (= max startIndex + 1)
depHashCount × u32                          ← dep hashes (truncated FNV1a64)
u32 depTypeCount
depTypeCount × u8                           ← parallel to dep hashes
u32 typeStringCount
typeStringCount × u32                       ← CRC32s of element names
```

Notes:

- The 32-bit hash is `(uint)FNV1a64(lowercased-path)` — i.e. the low 32 bits of
  the same 64-bit hash family WD2/WDL use (see [hashing](watch_dogs/hashing.md):
  WD1 "FNV32" = WD2 FNV64 & 0xFFFFFFFF). One reserved value is remapped on
  decode: `0xFFFF0000 → 0xFFFEFFFF`.
- Unlike the 64-bit format, both dep arrays carry explicit u32 counts.

### WD2 / WDL — 64-bit era (version byte 0x01)

First byte is `0x01` (WD1 files never start with 0x01 — their first byte is the
low byte of a large count). Otherwise the layout is close to WD1's, widened:

```
u8  version = 0x01
u32 containerCount                        (raw; last 1-2 records may be filler)
<header gap>                              COPY VERBATIM — purpose unknown
    observed: 9 bytes in both games
    WD2: 1a 91 ae 05 00 01 00 00 00
    WDL: 08 08 26 01 00 01 00 00 00
count × 17-byte record:
    u64 hash          masked CRC64_WD2 of the RESOURCE PATH (see below)
    u8  type          index into the footer type-string table
    u32 startIndex    index into the dep arrays
    u32 count         number of deps
    ⚠ last 1-2 records are malformed FILLER (type out of range) — stop parsing
      at the first invalid record
dep hashes: u64 × (maxValidStartIndex + 1)  ← count NOT stored; implied!
dep types:  u8  × same length               (may overrun the implied count by a
                                             few bytes of boundary fuzz — parsers
                                             should tolerate this)
u32 typeStringCount
typeStringCount × u32 CRC32                 ← to EXACT EOF
```

Verified stats (our dumps, `depload_tool.py dump`):

| File | Size | Valid records | Deps | Type strings |
|------|-----:|--------------:|-----:|-------------:|
| WD1 `windy_city_depload.dat` | 4,722,345 B | 96,927 | 711,813 | 35 |
| WD2 `san_francisco_depload.dat` | 14,011,414 B | 198,928 (+1 filler) | 1,181,047 | 43 |
| WDL `london_depload.dat` | 15,076,905 B | 176,579 (+1 filler) | 1,341,646 | 49 |

Example records (WDL `london_depload.dat`):

```
rec[0] hash=0xa00010d43ebf27e3 type=3 startIndex=364711 count=2
rec[1] hash=0xa0002818a08be5bc type=7 startIndex=130830 count=3
rec[2] hash=0xa0003be3920b4fda type=2 startIndex=986709 count=2
```

## Hash scheme — masked CRC64_WD2

Record and dep hashes are **masked CRC64_WD2 of the resource path**:

```
masked = (CRC64_WD2_raw & 0x03FFFFFFFFFFFFFF) | 0xA000000000000000
```

- `CRC64_WD2_raw` is the standard tagged hash (normalize `/`→`\`, lowercase,
  FNV-1 64, fold to 61 bits, `| 0xA000…`). Compute it with
  `hash_tool.py --crc64wd2` in the WDL RE repo.
- The depload mask keeps only the **low 58 entropy bits** and forces the top
  pattern `101000` — so every hash lands in `0xA000000000000000–0xBF…`
  (top byte `0xA0`–`0xBF`). Because the mask keeps a subset of the bits the
  tag already preserved, it doesn't matter whether you start from the raw
  FNV-1 or the tagged CRC64_WD2 — the result is identical.
- Cross-game portability: the low 32 bits still equal the old WD1 truncated
  FNV1a32 for the same path.
- Paths that couldn't be resolved during conversion show up as
  `__unknown\0x<hash>` in exported XMLs.

### Worked example

Container: `graphics\vehicles_nexus\land\special\forklift_01\forklift_01.xbg`
(the forklift PoC mod).

```bash
$ python3 hash_tool.py --crc64wd2 'graphics\vehicles_nexus\land\special\forklift_01\forklift_01.xbg'
  CRC64_WD2   : #4E28FC7F86CD16AD  (BE: AD16CD867FFC284E)
```

⚠ `hash_tool.py` prints the **little-endian byte order** first; the numeric
value is the `(BE:)` column: `0xAD16CD867FFC284E`.

Apply the depload mask:

```
  0xAD16CD867FFC284E
& 0x03FFFFFFFFFFFFFF  = 0x0116CD867FFC284E
| 0xA000000000000000  = 0xA116CD867FFC284E
```

`0xA116CD867FFC284E` is exactly the record appended to
`london_depload.dat` by the forklift PoC — round-trip confirmed by re-parsing
the modified file.

## XML conventions

Two different shapes exist for the human-readable form:

**WD1** (WDDeploy.exe `.converted.xml`) — containers wrap their dependencies:

```xml
<root>
  <CBinaryResourceContainer ID="path\to\model">
    <CGraphicResource ID="path\to\texture"/>
    <CPhysResource ID="path\to\hkx"/>
  </CBinaryResourceContainer>
</root>
```

**WDL** (Encrypted's conversion of the real deploads) — **no**
`CBinaryResourceContainer` (that's a WD1-only concept). Top-level elements ARE
the records; nested children are their dependency ranges:

```xml
<root>
  <CGeometryResource Id="graphics\...\forklift_01.xbg">
    <CMaterialResource Id="graphics\_materials\a.material.bin"/>
    <CMaterialResource Id="graphics\_materials\b.material.bin"/>
  </CGeometryResource>
  <CPhysResource Id="graphics\...\forklift_01.hkx"/>
</root>
```

Each top-level element maps to one 17-byte record:

- record hash = masked CRC64_WD2 of its `Id` **path**
  (validated: Encrypted's exported `CModelResource Id="__unknown\0xA0002818A08BE5BC"`
  equals binary `london_depload.dat` record 1),
- record type = index of the element name in the type-string footer
  (matched by CRC32),
- `startIndex` = current dep-array position, `count` = number of child
  elements.

Observed nesting examples: `CGeometryResource > CMaterialResource ×2`,
`CAnimationResource > CMarkupResource`,
`CFireUIResource > CFireUIResource ×2 + CFireTextureResource`.

The footer holds 35/43/49 (WD1/WD2/WDL) CRC32s of element names such as
`CGraphicResource`, `CPhysResource`, `CSkeletonResource`, `CMaterialResource`
(shared table, per-game ordering). Unknown types render as `BINHEX_<X8>` in
some tools.

## Tooling

| Tool | Games | What it does |
|------|-------|--------------|
| `WDDeploy.exe` (hV_WD_ModdingKit) | WD1 only | Official compile/decompile. **Fails on WD2/WDL** (`EndOfStreamException` — reads u32 where u64 live). `Deploadify` shares the same 32-bit-only code. |
| `depload_tool.py` (WDL RE repo) | all three | `dump <file.dat>` parses/validates any variant; `add <file.dat> <overlay.xml> <out.dat>` appends WDL-convention overlay records (masks hashes, resolves type indices via CRC32, preserves header gap + filler verbatim). |
| Encrypted's converter (private) | WDL | Produced `common_depload.xml` (998 KB), `london_depload.xml` (141 MB), `lib_graphic_vehicle_depload.xml` (33 MB). |

There is **no working depload build exe for WDL** ("this wont work for legion
we dont have a depload exe" — Encrypted), so binary append/conversion tools are
the only route for WD2/WDL.

## Modding rules (learned the hard way)

- **Overlay entries go at the END of the file** (after existing records,
  before the trailing footer/filler — `depload_tool.py add` handles this).
  Wrong order or position of IDs = crash (Yorpie).
- **Materials-only overlays**: if the models/textures are already loaded by
  other means, the overlay only needs the materials registered — "doesn't need
  dupes of the cars since they're already loaded. It doesn't even need the
  textures added. It just needs the materials loaded." (Yorpie)
- Adding unrelated resources can break *other* entities (e.g. motorcycle cops
  broke when an unrelated overlay landed mid-file).
- Back up the retail `.dat` before testing; keep the previous attempt around
  (the forklift PoC's first try used the WD1 container-GUID form and was
  superseded once Encrypted's XML revealed the WDL convention).

## Stability Warnings (WD2)

From Yorpie's experience modding WD2 depload:

- **Extremely fragile ordering**: IDs in wrong order = crash, wrong position = crash
- **Unrelated additions cascade**: Adding entries for one vehicle can crash other vehicles (e.g., adding motorcycle cops broke other traffic)
- **TBT spawn issue**: Tactical Bomb Truck won't spawn even with SUV disabled — known issue with vehicle spawn pools
- **Depload is copy-pasting**: "depload is just copy pasting a few lines... it just needs the **materials, models, and textures** in the right places"
- **New entries go at END**: Keep new depload entries at the END of the list per Yorpie's guidance
- **Crash = placement**: If it crashes, the depload placement is wrong
- **No spawn = missing entry**: If the entity doesn't spawn, it's missing from depload

Source: Discord (Yorpie, Pesky Fly).

## Source

Reverse engineered 2026-08-22 from retail archives of all three games plus
Encrypted's WDL XML exports; full notes in
`re/Ubisoft/Disrupt/WDL/RETAIL_VEHICLE_CATALOG.md` §20–§23, parser/builder in
`re/Ubisoft/Disrupt/WDL/depload_tool.py`.
