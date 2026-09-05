# The Witcher 3: Wild Hunt — XeNTaX Forum Knowledge

Extracted and organized from XeNTaX forum discussions (2015–2020). Covers the
**REDEngine** file formats of The Witcher 3: Wild Hunt — the `.cache`/`.bundle`
archives, `.xbm` textures, `.w3strings` localization, `.w3speech` voices, save
files, and mesh/skeleton data — plus the tools (QuickBMS, Lua-utils-for-Witcher-3,
Wolven-kit, Witcher 3 Modkit). Witcher 3 shares **REDEngine** with Cyberpunk
2077 (see the Cyberpunk knowledge doc).

---

## 1. Archives: `.bundle` and `.cache`

The Witcher 3 packs data in **`.bundle`** and **`.cache`** files.

- aluigi's official **`witcher3.bms`** QuickBMS script extracts `.bundle`
  files. *(michalss, 2015-05-12, t=12835)*
- The same script **does not** handle `.cache` files directly. *(ShivShubh,
  2015-05-12, t=12835)*
- `.cache` files (e.g. `texture.cache`) hold the **bulk of the textures**; most
  `.xbm` files only contain a **small preview image**, the real textures live in
  `content?\texture.cache`. *(hhrhhr, 2015-05-16, t=12835)*
- The `.bundle` files were initially **not re-packable**; repacking was a
  requested feature. *(dubh, 2015-05-18, t=12835)*
- A **`witcher3.map`** file (for v1.0.2) was shared to help map archive
  contents. *(Haoose, 2015-05-22, t=12835)*

## 2. Textures: `.xbm` → DDS

`.xbm` files are **CR2W** files (the REDEngine container) that wrap **DDS
texture data without a DDS header**. *(nerdyluke, 2016-06-23, t=14506)*

### XBM structure (as reverse-engineered on the forum)

A `.xbm` header hexdump (shakotay2, 2015-05-12, t=12835) shows the CR2W magic
`CR2W` followed by version and a string table that names the `CBitmapTexture`
struct and its fields:

```
00000000h: 43 52 32 57 A2 00 00 00 00 00 00 00 00 A0 F1 7D  ; CR2W.....
000000a0h: 00 43 42 69 74 6D 61 70 54 65 78 74 75 72 65 00  ; .CBitmapTexture.
000000b0h: 77 69 64 74 68 00 55 69 6E 74 33 32 00 68 65 69  ; width.Uint32.hei
000000c0h: 67 68 74 00 63 6F 6D 70 72 65 73 73 69 6F 6E 00  ; ght.compression.
000000d0h: 45 54 65 78 74 75 72 65 43 6F 6D 70 72 65 73 73  ; ETextureCompress
000000e0h: 69 6F 6E 00 54 43 4D 5F 44 58 54 4E 6F 41 6C 70  ; ion.TCM_DXTNoAlp
000000f0h: 68 61 00 74 65 78 74 75 72 65 47 72 6F 75 70 00  ; ha.textureGroup.
00000100h: 43 4E 61 6D 65 00 43 68 61 72 61 63 74 65 72 44  ; CName.CharacterD
00000110h: 69 66 66 75 73 65 00 72 65 73 69 64 65 6E 74 4D  ; iffuse.residentM
00000120h: 69 70 49 6E 64 65 78 00 55 69 6E 74 38 00 74 65  ; ipIndex.Uint8.te
00000130h: 78 74 75 72 65 43 61 63 68 65 4B 65 79 00 00 00  ; xtureCacheKey...
```

- **Conversion trick:** just load the DXT1 data **without a header** (it's DDS
  without header), or strip the `.xbm` header and add a DDS one. *(shakotay2,
  2015-05-12, t=12835)*
- The `CBitmapTexture` fields are described by a string table: `width` (Uint32),
  `height` (Uint32), `compression` (`ETextureCompression` = `TCM_DXTNoAlpha`),
  `textureGroup` (`CName` e.g. `CharacterDiffuse`), `residentMipIndex` (Uint8),
  `textureCacheKey` (Uint32). *(shakotay2, 2015-05-13, t=12835)*
- stalja observed the XBM as ~5 parts (Header, variable list, data1, data2,
  texture) but confirmed that was an oversimplification; the texture data is
  DDS-without-header and its length/offset logic was not fully nailed down.
  *(stalja, 2015-05-13, t=12835)*

### Extraction tools for `texture.cache`

**hhrhhr's `Lua-utils-for-Witcher-3`** (github.com/hhrhhr/Lua-utils-for-Witcher-3)
extracts textures from `texture.cache`. Requires **Lua 5.3** and **lua-zlib**.
*(hhrhhr, 2015-05-16, t=12835)*

```
lua unpack_textures.lua texture.cache output_dir
```

- Needs **Lua 5.3** specifically (uses `goto`, `string.unpack`); on 64-bit
  platforms use `"<I4"` instead of `"<L"` for 4-byte little-endian ints, or the
  script fails with `'=' expected near 'skip'` / `bad argument #2 to 'unpack'`.
  *(hhrhhr, designgears, Alianin, 2015-05-19..07-06, t=12835)*
- The extracted files need DDS headers added via `mod_dds_header.lua`.
  *(hhrhhr, 2015-05-19, t=12835)*

### Witcher 2 texture converter (reused for W3)

The **witcher2_texture_converter** pack (`w2_xbm2dds.bms`, `w2_dds2xbm.bms`,
plus nvcompress/nvdecompress) is often referenced for W3 `.xbm` work too.
*(srcs34k, 2018-04-17, t=13838)*

## 3. Localization: `.w3strings`

`.w3strings` files are the W3 localization string files (a new format vs the
W2 `.w2strings`).

- They are **not simply XOR'd** — early analysis found "almost clean text
  (UTF16 with some html tags)" inside, but decoding required more than a fixed
  XOR. aluigi noted it "looks like a XOR with a long sequence of bytes" but
  couldn't confirm fixed-key vs increment-key. *(hhrhhr, slayer983, 2015-05-19..23,
  t=12835/t=12869)*
- hhrhhr: "unpacker is quite simple, but the decoder - not yet; strings are not
  just xored." *(hhrhhr, 2015-05-21, t=12835)*
- **Haoose released an unpacker/repacker** for `.w3strings` (hosted on zenhax,
  t=964). *(Haoose, 2015-05-26, t=12869)*

## 4. Voices: `.w3speech`

- The **voice audio is NOT in the `.cache` files** — it lives in
  `enpc.w3speech` files (per-language, e.g. `enpc` = English). *(mikaelN7,
  2015-05-19, t=12861)*
- The `.w3speech` format differs entirely from the W2 `.w2speech`; extraction
  was a repeated request. *(mikaelN7, 2015-05-19, t=12861)*
- Note: the Witcher 3 `.w3speech` tools are **not** the same as the W2
  `.w2speech` tools (see the Witcher 2 doc). *(micTronic, 2021-12-06)*

## 5. Save Files (`.sav`)

Witcher 3 save files are a REDEngine save format. *(Harime Nui, RedEyeX32,
2015-10..11, t=13435)*

- A save is **not** an archive of multiple files — it is **one file split into
  1,048,576-byte (0x100000) parts**, each part **LZ4-compressed** (LZ4 r131
  worked for both compress and decompress on default settings). *(Harime Nui,
  2015-11-01, t=13435)*
- The header's apparent "checksum" was actually just the **uncompressed file
  length in bytes**. *(Harime Nui, 2015-10-31, t=13435)*
- Unpacking/repacking works, but editing breaks the game easily.
  *(Harime Nui, 2015-10-31, t=13435)*
- The decompressed payload resembles **binary XML**. *(Harime Nui, 2015-11-12,
  t=13435)*
- Tools: **W3SavegameEditor** (github.com/Atvaark/W3SavegameEditor) supports
  unpacking; W2 save-format references: Gibbed.RED
  (github.com/yole/Gibbed.RED/tree/.../Formats/Save) and 13xforever's
  Witcher-2 SaveFormat. *(ner0, 2016-01-02, t=13435)*

## 6. Meshes, Bones & Skeletons

- W3 meshes are **CR2W** files (like W2), `.w2mesh` → `.w2mesh` lineage; the
  W3 `.w2mesh`/`.w2cutscene`/`.w2scene` **cannot** be loaded by the W2 REDkit —
  it reports "Version 162 is not supported" (highest supported is 115); faking
  the version from `0xA2` to `0x59`/`0x73` just crashes REDkit. *(shakotay2,
  2015-05-12, t=12835)*
- The mesh tools convert meshes with **bones and weights**, but **bone parenting
  data does not export** with the meshes (or doesn't exist) — assembling the
  skeleton manually was needed. *(ssringo, 2015-08-17, t=12835)*
- **Bone data analysis (48 bytes/bone):** a researcher parsing `CSkeleton`
  found ~48 bytes per bone (16 bytes each for position/rotation/scale as 32-bit
  floats), but the floats were all **between -1 and 1** (with tiny values like
  `2.0e-12`), so they need extra processing — not usable as-is. Vertex data is
  stored in **half-floats** needing division by 65535 plus a scale/offset.
  `CSkeleton` only exposes bone names + parent indices; the scale/offset
  (cookedData) lives on `CMesh`. *(digitalutopia1, 2018-11-04, t=19016)*

### Noesis / model conversion tools

- **fmt_TW3_to_XPS.py** — an old Noesis plugin for W3 models (links now dead).
  *(Mirrorman95, 2020-02-14, t=21755)*
- **RedTools** by JLouis-B — converts W3 (and CP2077) models, with source
  available. *(reh, 2020-02-16, t=21755)*
- **Wolven-kit** (github.com/Traderain/Wolven-kit) — the CDPR modding toolkit
  with releases supporting W3 (and later CP2077). *(reh, 2020-02-16, t=21755)*
- **The Witcher 3D Model Converter (JLouis-B)** — the common tool, but it did
  **not** export skeletons, which prompted custom work. *(digitalutopia1,
  2018-11-04, t=19016)*
- **AMD GPU PerfStudio / texture rippers** — ripping textures (e.g. foliage)
  was possible, but the "smudging" around foliage/hair textures is just alpha
  bleed-prevention, not a data problem. *(NuclearTester, ssringo, stiffy360,
  2015-06..06, t=12835)*

## 7. Official Modkit

- CD Projekt RED released **Witcher 3 Modkit** (mod support, announced 2015-08-14).
  *(Haoose, 2015-08-14, t=12835)*

## 8. Tools Reference

| Tool | Purpose | Notes |
|------|---------|-------|
| QuickBMS + `witcher3.bms` | Extract `.bundle` files | aluigi; does not handle `.cache` directly |
| Lua-utils-for-Witcher-3 | Extract `texture.cache` textures | hhrhhr; needs Lua 5.3 + lua-zlib; `unpack_textures.lua` |
| witcher2_texture_converter | `.xbm` ⇄ `.dds`/`.tga` | `w2_xbm2dds.bms`, `w2_dds2xbm.bms`; reused for W3 |
| Haoose `.w3strings` unpacker/repacker | W3 localization | hosted on zenhax t=964 |
| W3SavegameEditor | Save unpack/edit | github.com/Atvaark/W3SavegameEditor |
| Wolven-kit | CDPR modding toolkit | github.com/Traderain/Wolven-kit; W3 + CP2077 |
| RedTools (JLouis-B) | Model conversion | github.com/JLouis-B/RedTools |
| Witcher 3 Modkit | Official modding tools | CDPR, 2015-08-14 |
| Witcher 3D Model Converter | Model conversion | No skeleton export |

---

*Source: XeNTaX forum threads t=12835 (Witcher 3 *.cache *.bundle), t=12861
(voices .w3speech), t=12869 (.w3strings), t=13435 (W3 save files), t=13838
(Unpacking XBM files? W2/W3), t=14506 (.xbm files), t=14591 (.redcloth as mesh),
t=19016 (W3 Bone Data), t=21755 (W3 Noesis Plugin? / model tools).*
