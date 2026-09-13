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
  `compressedSize &= 0x1FFFFFFF`, `uncompressedSize >>= 2`, `flag = uncompressedSize&3`;
  u64 hash halves byte-swapped on read. (Full struct: see xentax-farcry-knowledge.md §1.)

## File-list repos (OSM)

| Game | Repo | Notes |
|------|------|-------|
| FC2 | FarCry2-File-Lists | 32-bit hashes (uint) |
| FC3 | FarCry3-File-Lists | created Sep 2026 (was missing upstream) |
| FC4 | FarCry4-File-Lists | created Sep 2026 (was missing upstream) |
| FC5 | FarCry5-File-Lists | |
| FC6 | FarCry6-File-Lists | 63% of install entries named |
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
