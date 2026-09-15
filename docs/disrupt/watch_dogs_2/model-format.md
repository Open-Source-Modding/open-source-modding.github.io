# WD2 Model Descriptor Format (`.model`)

Small binary descriptors that bind a **model (XBG)** and optionally a
**material** to a spawnable object in Watch_Dogs 2. Official Ubisoft format
(confirmed by Pesky Fly) — retail ships 258 of them at
`graphics/models/0x8000XXXXXXXXXXXX.model`, UID-named in the same high-bit
namespace as entity-library UIDs.

> Tooling: read/verify with `hash_tool.py --crc64wd2`
> ([Hashing](../watch_dogs/hashing.md)). Unpack archives with
> Gibbed.WatchDogs2.Unpack (see § Unpacking).

## Layout

```
u32  version = 1
u32  0x46
char[4] fourCC 'nbCF'
u32  3
u32  N1                  # entry-related count (semantics TBD)
u32  N2                  # resource count (semantics TBD)
byte[10] const blob      # 01 b5 ba d0 14 02 75 72 78 b1 — identical in every file
--- repeated per resource entry ---
<ascii path>\0           # full archive-relative path, backslashes
u32  tag                 # field id: XBG = 0x430A34BB, MAT = 0x83475FEE
u8   type = 0x08
u64  hash                # crc64_wd2(path) — LE bytes
--- trailing block ---
shared prefix 01 61 0b a6 0f 00 ... then per-file data,
including a label string (e.g. 'hoodmoprh') and padding/sentinels
```

Verified against **all 344 path/hash entries** across the 258 retail files:
every single `hash == crc64_wd2(entry path)`.

- Single-resource models exist (ingamemap roads → one XBG entry only).
- 64 of 258 carry a material entry; e.g. `pantscargo01.model` →
  `graphics\_materials\lsauvage-m-9223372052129531776.material.bin`.

## The dupe recipe (custom accessories/props)

A working community example (Pesky Fly's hooded faceless accessory for a
custom character model) is a **three-file trio**, all custom — none of the
paths exist in retail:

1. `<name>.xbg` — mesh (custom or cloned)
2. `<name>.material.bin` — material definition
   ([format](../watch_dogs/material-bin-format.md)); WD2 files are version
   **12** (`TAM\0`; v7=WD1, v15=WDL). Derived materials follow
   `<base>_Derivation_<source>_0x<id>` naming. Textures referenced as
   archive paths (`motifs\moti_heather01_c.xbt`, own `_m4.xbt` etc.)
3. `<name>.model` — descriptor binding the two, built per the layout above

Rules that make dupes work:

- **Recompute both hashes** after editing any path:
  `hash = crc64_wd2(path)`. A real-world file shipped with a stale material
  hash (string renamed without recompute) and *still worked* — the engine
  binds via the path strings — but don't rely on it.
- Entry tags are constant: use `0x430A34BB` before an XBG path and
  `0x83475FEE` before a material path, each followed by `08`.
- Keep the const blob and header fields byte-identical to a working retail
  file; change only entries + label.

Known pitfall: repacking edited `.model`s back into an archive has crashed
the game for at least one modder — prefer loose-file/per-mod mounting when
available.

## Unpacking (Gibbed.WatchDogs2)

The widely-distributed prebuilt `Gibbed.WatchDogs2.Unpack.dll` is stale and
throws `NotImplementedException: DecompressLZ4LW`. Rebuild from source
(repo also contains the LZ4LW implementation):

```sh
cd Gibbed.Disrupt
dotnet build projects/Gibbed.WatchDogs2.Unpack -c Release   # output lands in ../bin/
cd ../bin
dotnet Gibbed.WatchDogs2.Unpack.dll <archive.dat> <outdir> [--filter=REGEX] [--nf -v]
```

Notes: `--filter` is a **regex**, not a glob; `--nf -v` lists entry names
without extracting. WD2 map/world content lives inside
`common.dat`/`patch*.dat` (there is no separate `san_francisco.dat`;
in-game-map assets appear under `graphics\ingamemap\san_francisco\...`).

Credit: Pesky Fly (working trio + format confirmation),
Slick (dupe method), Yorpie (failing-dupe report that motivated the hash
rule).
