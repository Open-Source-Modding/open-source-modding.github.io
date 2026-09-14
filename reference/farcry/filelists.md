# Far Cry file lists (hash → name recovery)

How FC file lists are generated and where they live. Verified against the real
FC6 retail install (Sep 2026).

## Data flow

```
concatenated/ (raw accumulation, RE repo /home/selene/Documents/Code/re/Ubisoft/Dunia/concatenated/)
        |  339 snapshots, 7.35M lines; per-archive families like common (1)-(8).filelist
        |  from FC5Hook/FC6Hook filename loggers + community file lists
        v  [Gibbed.Dunia RebuildFileLists]
FarCry*-File-Lists submodules (OSM org)
        |  configs/Far Cry X/files/pc/<archive>.filelist
        |  status.txt = named/total per archive
        v  [Unpack / FCBConverter]
named files extracted
```

## FAT hashing

- FC FAT entries store **64-bit CRC64** of the path (case-sensitive, backslash
  separators). Verified 12/12 against the Joseph-Seed Discord bot's `!crc` outputs.
- FC_Plugins' FNV1a32 is **not** the archive hash.
- FAT v11 decode: `offset = ((compressedSize>>29 | unresolvedOffset<<3)<<4)`,
  `compressedSize &= 0x1FFFFFFF`, `uncompressedSize >>= 2`, `flag = uncompressedSize&3`.
  (Full struct: see xentax-farcry-knowledge.md §1.) **The FAT stores the raw CRC64 —
  NOT halves-swapped** (see the correction under "CRC64 algorithm" below).

## File-list repos (OSM)

| Game | Repo | Notes |
|------|------|-------|
| FC2 | FarCry2-File-Lists | 32-bit hashes (uint) |
| FC3 | FarCry3-File-Lists | created Sep 2026 (was missing upstream) |
| FC4 | FarCry4-File-Lists | created Sep 2026 (was missing upstream) |
| FC5 | FarCry5-File-Lists | |
| FC6 | FarCry6-File-Lists | 64% of install entries named |
| New Dawn | FarCryNewDawn-File-Lists | |
| Primal | FarCryPrimal-File-Lists | |

## RebuildFileLists usage

Install path is a **positional** argument (not `--install-path=`):

```
dotnet run --project Gibbed.FarCry6.RebuildFileLists -- <data_final/>
```

- Reads every `*.fat` under the install dir (plus `.fat.bak`), loads all
  `*.filelist` from the game's files/ dir (submodule), resolves each entry hash,
  writes `<archive>.filelist` + `status.txt` + `failure.txt`.
- `failure.txt` receives genuine hash collisions (filtered from output) —
  case variants are deduped via `HashList.Add()` + lowercase modifier.
- Merge rule: unions with existing lists, never overwrites/removes names.

## CRC64 algorithm (the table gotcha)

The FAT path hash is **CRC-64/XZ (ECMA-182)**, reflected, with Gibbed's exact
table (`Gibbed.Dunia.FileFormats/Hashing/CRC64.cs`, first entries
`0x0000000000000000, 0x01B0000000000000, 0x0360000000000000, 0x02D0000000000000`):

```
hash = 0
for each byte b (lowercased path):
    hash = Table[(hash & 0xFF) ^ b] ^ (hash >> 8)
```

- Lowercase the path first (`ToLowerInvariant`); separators are backslashes.
- A non-reflected table (e.g. poly 0x42F0E1EBA9EA3693 fed into a shifted
  generator) produces **wrong hashes** — use the table verbatim from CRC64.cs.

> **⚠️ Correction (Sep 2026): the FAT stores the RAW CRC64, NOT halves-swapped.**
> An earlier version of this doc claimed the on-disk FAT entry was the hash with
> its 32-bit halves swapped (so `AA32FC2B699F7B7F` = "stored form" of
> `699F7B7FAA32FC2B`). That was **wrong**. Verified: 200 random known
> `xbt/bik/xbg` entries → all 200 matched the fat by **raw** crc64, 0 by
> swapped. A FAT entry's 64-bit value (assembled `(a<<32)|b` from two LE u32s)
> **equals** `CRC64(path)` directly — no swap. The swap was a red herring that
> hid ~5,499 recoverable names. Always test membership with `CRC64(path)`
> itself, and add `.png/.dds/.tga/.tif/.jpeg/.jpg/.bmp/.psd` → `.xbt` rewrites:
> image *source* paths inside compiled FCBs resolve to their packed `.xbt`
> archive name (same path, `.xbt` extension).

## Recovering unresolved (`__UNKNOWN`) entries

Unpack puts unknown-hash entries under `__UNKNOWN/<type>/<HASH>.<ext>` where
`<type>` is a content-magic guess (e.g. `fonts` for OTF), not the real path.
To recover the real path:

1. Look for the name inside **binary objects** that reference the asset —
   compiled `.fcb` files contain plaintext paths (e.g. `PHXFT` font objects
   embed `UI\Common\fonts\src\BenguiatProITC-BoldCond.otf`).
2. Verify by hashing candidate paths with the correct table + lowercase +
   **raw CRC64** (no swap); the result must equal the stored hash. Also try
   rewriting image-extension paths (`.png/.dds/.tga/...`) to `.xbt`.
3. Add the recovered path to the game's `.filelist` and re-run RebuildFileLists
   so future unpacks resolve it natively.

### Case study: FC6 Benguiat font (Sep 2026)

`__UNKNOWN/fonts/AA32FC2B699F7B7F.otf` was the FC6 HUD/logo font. Its `.fcb`
(Phoenix `PHXFT` font object, `__UNKNOWN/game/DA5801FB83FCC967.fcb`) referenced
`UI\Common\fonts\src\BenguiatProITC-BoldCond.otf`. CRC64 (lowercase)
of `ui\common\fonts\src\benguiatproitc-boldcond.otf` =
`0x699F7B7FAA32FC2B` = stored `AA32FC2B699F7B7F` — exact match (independently
confirmed by the Joseph-Seed bot's `!crc 64`). The path was absent from all
recovered FC6 filelists (only `.ttf` fonts were listed), which is why it landed
in `__UNKNOWN`. Added to `common.filelist` + `worlds/installpkg.filelist`
(entry exists in both `common.fat` and `installpkg.fat`); FC6 RFL status went
956125 → 956126/1502018 (63%).

### Gotcha: RFL "unsupported compression scheme" (LZ4)

The BigFileV2 layout split exposed a latent bug: `SanityCheckEntry` threw
`FormatException("unsupported compression scheme")` on every v11 entry because
the switch handled `None/LZO1x/Zlib/XMemCompress` but **not `LZ4`** — and v11
(FC6) archives are majority-LZ4. Fix: add `LZ4`/`LZ4LW`/`Oodle` cases to the
same zero-size guard as LZO1x/Zlib (BigFileV2.cs `SanityCheckEntry`).

### Case study: 5,499 paths recovered from binary string-scan (Sep 2026)

After the Benguiat font, a full **binary string-crawl** over `common_unpack`
(`.fcb/.spx/.ndb/.ids/.xml/.bin/.material.bin/.xbg`) recovered **5,499
additional paths** (`binary_recovered.filelist` in `FarCry6-File-Lists`):

- Extract every `root\...ext` path string (roots: `ui|graphics|worlds|scripts|
  sound|audio|actionmaps|dictionaries|mission|sectors|configs|engine|wwise|
  shaders|nomad|d3d12|databases`), tolerating trailing framing bytes (e.g. the
  `PHXFT` font object stores `...BoldCond.otfd`).
- Hash each with **raw** CRC64; also hash the `.png→.xbt` / `.dds→.xbt` rewrite.
- Keep those whose hash is in a fat and not already in the filelists.

Breakdown: 3,465 `.material.bin` (`graphics\_materials\<dev>-m-*.material.bin`),
1,411 `.xbg` (world vista_locations, animals, extended content), 577 `.xbt`
(ui\tetra\textures, common\textures\optionpreview), 31 `.spx`, 15 `.bik`.
FC6 RFL status: 956146 → **961645/1502018 (64%)**.
### Case study: 6,079 paths recovered from the installpkg tree (Sep 2026)

A second pass over the **29 GB installpkg_unpack** tree (the full install adds
soundbinary/, animations/, move/, domino/) recovered **6,079 more paths**
(binary_recovered.filelist, now 11,579 lines):

- **5,511 .move.bin** animation state machines: movelendtrees\... and
  move\decisiontrees\... (player 1st/3rd-person weapon reload/shoot trees,
  animal locomotion/action trees, AI stp trees). installpkg.fat holds the whole
  movelendtrees asset class - previously unnamed.
- **328 .xbt** DLC4 animal textures (_textures\<variant>\*_mips.xbt:
  mongoose, jaguar, croc, jellyfish, crow, coyote flayed...).
- **173 .fcb**, **13 .xbg** (world vista_locations), **54 .material.bin**.

FC6 RFL status: 961645 -> **967724/1502018 (64%)**. Commits:
FarCry6-File-Lists 913bfc1, Gibbed.Dunia 9297da8.
Sample: movelendtrees\playerrdperson\weaponseload\gunsiflegripfnfalt_player_3rd_reload_begin_fnfal.move.bin.

