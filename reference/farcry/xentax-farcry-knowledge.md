# Far Cry (Dunia engine) — XeNTaX Forum Knowledge

Extracted and organized from XeNTaX forum threads (`forum.xentax.com`, offline
Nov 2023; scraped MariaDB dump in `/home/selene/Documents/Code/re/xentax/`).
Covers the **Dunia engine** `.fat`/`.dat` Big File archive format (FC2–FC6 +
Primal), the `.xbg`/`.xbt` model/texture formats, and the practical modding
toolchain that emerged from the community. Thread IDs (`t=NNNNN`) are XeNTaX
thread IDs — cross-reference against `attachments/xentax/{cat}/{thread}/{file}`
and `xentax_topic_names.csv`.

Primary source threads:
- **t=17888** "Far Cry 5 .Fat and .Dat Files" (292 posts) — the central FC5 archive thread
- **t=24572** "Far Cry 6 .fat and .dat files" (17 posts) — FC6 v10→v11 FAT change
- **t=9927** "Far Cry 3 .FAT decryption" (39 posts) — FC3 archive, not encrypted
- **t=21425** "Far Cry 5 models modding tool" (198 posts) — XBG modding workflow
- **t=25674** "Far Cry 6 model tool" (10 posts) — daemon1's FC6Model.exe
- **t=25062** "Far Cry 6 (PC) .XBG" (5 posts) — FC6 UV/vertex issues
- **t=11602** "Watch Dogs XBG models" (160 posts) — Dunia-lineage XBG precedent
- **t=2795** ".bin .xbt .xba .xbg farcry file formats" (8 posts) — early XBT analysis

---

## 1. Archive format: `.fat` / `.dat` (Big File)

- The `.fat` is **NOT encrypted** — no decryption needed. *(Rick, 2012-12-04, t=9927)*
- FC3 (Dunia 2) archive tooling: the official **`svn.gib.me/builds/dunia2/`**
  hosted the FC3 unpacker+packer (Gibbed.Dunia2). *(Haoose, 2012-12-04, t=9927)*
- **FC3 FAT version = 10.** The old unpacker (for v10) extracts **null bytes**
  for compressed files against FC6. *(mlleemiles, 2021-10-05, t=24572)*
- **FC6 FAT version = 11** — changed from 10, with the struct layout possibly
  changed as well. *(mlleemiles, 2021-10-05, t=24572)* — **RESOLVED (Sep 2026)**
  via FCBConverter source (`FCBConverter/Program.cs GetFatEntries`, credit
  ミルクティー/miru) + cross-check against the real FC6 install. `.fat` start
  `b'2TAF'` ("FAT2") + u32 version `0x0b`. v11 header (24B): u32 'FAT2', u32
  version=11, u32 unknown=1, u32 subfatTotalEntryCount, u32 subfatCount, u32
  totalFiles. Entries 20B each: u64 NameHash (halves byte-swapped on read →
  stored high-half-first), u32 UncompressedSize (low 2 bits = CompressionScheme
  flag: None=0/LZO1x=1/LZ4=2, >>2 = size), u32 UnresolvedOffset, u32
  CompressedSize. v11 decode: `offset = ((compressedSize>>29 | unresolvedOffset<<3) << 4)`;
  `compressedSize &= 0x1FFFFFFF`; `uncompressedSize >>= 2`; `flag = uncompressedSize&3`.
  After entries: u32 unknown1Count (must be 0), then (v≥7) u32 unknown2Count ×
  16B. SubFats follow. `common_hd.fat` = 24 + 18×20 + 8 = 392 ✓ (18 entries,
  tail `00000000 00000000`). Version map (from FCBConverter): 11=FC6, 10=FC5/FCND,
  9=FC3/FC3BD/FC4, 5=FC2. FCBConverter is the working FC archive tool
  (the bundled Gibbed.Dunia2.FileFormats BigFile.cs only supports v≤9).
- **FAT stores only 64-bit hashes, no filenames.** Filenames must be recovered
  from a file list (see §3). *(multiple, t=17888 / t=24572)*

## 2. Filename recovery — the hook / logger workflow

Because `.fat` stores hashes only, the community recovered names by hooking the
game to log which files it opens:

- **FC5Hook** (Ekey, 2018-03-28, `FC5Hook_0.01_r1.rar`) — hook that logs all
  filenames the game opens to a `FC5.log` in the bin folder. Thx to Haoose for
  tests. Tested with **bypassed EAC**. *(Ekey, t=17888)*
- The map editor loads the whole file list — running the hook through the map
  editor produced a **54,611-filename** list (`FileNames-54611.rar`). *(Ganic3000, t=17888)*
- **FC6Hook** (Ekey, `FC6.Hook_0.01_r2.zip`, 2021-10-09) — same approach for
  FC6. Log file is written **outside** the Bin/Bin_Plus directory; r2 adds a
  date/time-stamped filename so it isn't overwritten. Requires unlocking DLLs on
  Win10. *(Ekey, t=24572)*
- Without a filelist, the unpacker still extracts files but with **random
  names** — you can't tell what anything is. *(multiple, t=17888)*

## 3. Hash → filename mapping (FCBConverter "Projects")

- Unknown-named files can be mapped to real paths: e.g. hash
  `1DC53A544E9EF2D3` == `giveplayerammo_v2.lua`. Adding the supposed path to
  the file list (`FileNames.list`) makes the unpacker name it correctly.
  *(sunbeam906, t=17888)*
- Lua files are stored in an **LUAC container**; strip the LUAC header + 8
  bytes of file size, then decompile with **unluac** (Lua 5.1, `0x51` header).
  *(sunbeam906 / mlleemiles, t=17888)*
- **FCBConverter** is the FC5/FC6 unpack/pack tool and the `Projects` list
  drives name resolution. *(multiple, t=21425 / t=24572 / t=25674)*

## 4. `.xbg` model format — lineage across the series

- **FC3/FC4/Primal** XBG importers existed for Blender; the FC5 importer
  (`io_scene_FarCry5.zip`, Ganic3000) followed. *(Ganic3000, t=17888)*
- **FC5 XBG header magic = `HSEMG`** ("GMESH"). *(Ganic3000, t=17888)* —
  matches independent repo finding; FC6 uses `HSEMI` ("IMESH"), Disrupt WDL uses
  `MOEG` ("GEOM"). Same Dunia lineage, wrapper magic differs per generation.
- **FC5 vs FC4/Primal format drift** (volfin, 2018-06-27, t=17888):
  "They really messed with the format as compared to FarCry4/Primal, the block
  structure is relatively similar, but they removed some blocks and added some
  new ones, and changed how sub-meshes/sub-materials are stored, and changed
  how they handle the vertex block, which is frankly a nightmare."
- **FC5 skeleton portion did NOT change** vs Primal — the Primal importer could
  already extract FC5 skeletons; rigging/weights work was the remaining piece.
  *(volfin, 2018-06-29, t=17888)*
- FC5 vertex-block detail: simplest vertex block starts at `0x13BB0`, is
  `0x146` blocks long, each 16 bytes. *(post, t=17888)*

## 5. FC6 `.xbg` specifics

- **FC6 XBG is a newer format** again. *(Sharppy, 2021-10-07, t=24572)*
- FC6 **verts are split per-LOD** — LOD0/LOD1/LOD2 are packed into the same
  file, so "split" vertex runs are just different LODs. *(dimis9138, t=24572)*
- FC6 vertex component order is **XZY** (not XYZ); verts are **short signed**
  (i16). Example file `A08F7270CE7E2467`: faces short, verts XZY (LOD1–2);
  vert offset `0x8b00`. In `CABBAB432E7A1932`: `0x1130` → 10209 verts (short
  signed, 2 padding); `0x044180` → 5478 verts (short signed, 2 padding).
  *(dimis9138, t=24572)*
- Materials appear at the very **start/top** of the XBG file. Some models
  (helicopter) have interior/exterior as separate parts. *(dimis9138, t=24572)*
- **SDOL chunk structure** (John76, 2021-11-06, t=24572), 20-byte chunk:
  `dword lod` | `dword unknown1 (null)` | `float somefloat` | `dword unknown2` |
  `dword unknown3 (null)` | `dword vertsize` | `dword buffsize` (= vertsize +
  normals + uv) | `dword buffsize1` — cross-checked against the independent
  repo decode of `new_fc6_model.xbg`: SDOL descriptor has the extra fields
  45888 / 114720 / 43 (matching the vertsize/buffsize/extra-dword fields FC5's
  descriptor lacks).

## 6. FC6 UV / vertex quantization gotchas

- **PC FC6 uses Denuvo or an obfuscated tool path** — meshes from the PC
  version wouldn't decode cleanly, while **PS4 files were fine**. Tool authors
  pivoted to PS4 assets for verification. *(Sharppy, 2022-02-19, t=25062)*
- FC6 UV data was extremely hard to "dial in": padding/interval combos
  (0/12, 10/2, 8/4, 6/6, 4/8, 2/10, 12/0) all failed. Consistent with the
  repo's finding that FC6 vertices are **quantized/packed** (i16-scaled to a
  bounding box), not float32. *(Sharppy, t=25062)*
- The export pipeline must respect **UE4 half-float UVs** — keep UVs close to
  the usual 0..1 range or the game shows artifacts. *(t=21425)*

## 7. FC6 model tooling & workflow

- **FC6Model.exe** (daemon1, 2022-08-11, t=25674): drop a `.xbg` onto it (or
  CLI/batch); converts XBG → `.ascii`/SMD. Supports all skeletal & static
  models. Readable via Noesis (export to FBX).
- **Universal heads** need facial bones from separate files — provide XBG +
  all `.item.bwsk` + `.dpdx` files. `.item.bwsk` files describe model parts,
  all skeletons/bones, material overrides, and a link to a `.dpdx` (facial
  pose definition) with final facial bone positions/rotations. Each later file
  **overwrites** earlier bone pos/rot.
  Example: `FC6Model.exe fct_hed_shr_average_01_m.xbg dlc_char501_01_m.item.bwsk fct_fac_dlc_char501_01_m.dpdx` *(daemon1, t=25674)*
- **XBG root-bone naming matters for the game to render the model**: weapons
  need the root named `FRAME`, cars `chassis`; a custom model must use these
  names (`moddedmesh_lod0` → renamed to `FRAME` for a Stalker SoC Makarov
  pistol to work). Tools primarily work with clothes/parts. *(t=21425)*
- Weights/indices for rigged FC5 meshes were found but getting them plugged
  into the right meshes/order was the hard part. *(volfin, t=17888)*

## 8. Textures (`.xbt`) & audio

- Early XBT analysis found mipmap-level fields that "make no logical sense" for
  a naive DXT1/DXT2 interpretation — some textures may be **volume or cube
  textures** where the "mip count" field actually encodes sides. No clean DXT
  match was ever confirmed. *(sababa, 2007-10-02, t=2795)*
- FC5 `.wem` (Wwise) audio files were still **compressed** even after unpacking
  — separate decode step needed. `.bik` files carry an archive header and did
  not unpack cleanly. *(t=17888)*
- FC2 weapons/vehicles audio lives in dedicated archives (see `t=13935`).

## 9. Toolchain summary (what actually worked)

| Task | Tool | Source |
|------|------|--------|
| Unpack/pack FC3 (Dunia2) | Gibbed.Dunia2 (`svn.gib.me/builds/dunia2/`) | t=9927 |
| Unpack FC5/FC6 archives | FCBConverter + file list | t=21425/t=24572 |
| Filename logging (FC5) | FC5Hook_0.01_r1.rar (Ekey) | t=17888 |
| Filename logging (FC6) | FC6.Hook_0.01_r2.zip (Ekey) | t=24572 |
| Lua decompile | unluac (strip LUAC header + 8B) | t=17888 |
| FC5 XBG → Blender | io_scene_FarCry5.zip (Ganic3000) | t=17888 |
| FC6 XBG → SMD/ascii | FC6Model.exe (daemon1) | t=25674 |
| FC6 archive unpack | extractor (ArmanIII, updated by mlleemiles) | t=24572 |
| Textures | XBT viewer / Dragon Unpacker (PNG from map editor) | t=11719/t=17888 |

---

## Open questions / gaps

- Exact FC6 FAT v11 entry layout — **now resolved (see §1)**: 20-byte entries,
  offset `((comp>>29 | off<<3)<<4)`, hash halves byte-swapped on read.
- FC6 `sarb` shader wrapper structure (in `shadersobj.dat`) — not covered in
  any XeNTaX thread; remains open.
- FC6 XBG **field-level** vertex decode (the repo's own open item) — threads
  confirm the short-signed XZY verts + per-LOD splitting but not the full
  descriptor semantics.
- The PC vs PS4 Denuvo difference in FC6 asset decoding is worth chasing —
  PC files reportedly need the obfuscation handled.