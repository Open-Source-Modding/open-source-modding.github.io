# Need for Speed — Archive, Model & Texture Formats

> **Source**: XeNTaX forum knowledge extraction (2005–2010), 20 threads.
> Covers Black Box era NFS (Underground → Shift) plus Hot Pursuit 2010.
> Formats largely shared across the series with per-game variations.

---

## 1. VIV / BIG Archives (Underground, Hot Pursuit 2)

### Structure
Same container format as FIFA and Command & Conquer.

| File | Description |
|------|-------------|
| `sdata.viv` | Main archive in Underground 2 (music, sounds). Contains nested `.big` and `.mus` files |

### Tools
- **MultiEx Commander (MexCom)** — opens BIG and VIV (Mr.Mouse, 2005)

---

## 2. BIN / BUN Archives (Underground, Most Wanted, Undercover)

### Key Distinction
**BIN** and **BUN** are **two distinct formats**, not the same (wiki/thread analysis, 2006).

### .BIN Format (Most Wanted)

```
Archive Header:
  2  - null
  2  - Unknown
  4  - Archive Length [+8]
  4  - null
  4  - Number Of Nulls (48)
  48 - null
  2  - null
  2  - Unknown
  4  - Directory Length?
  2  - Number Of Directories (1)
  2  - Unknown

Per Directory:
  4  - Directory Offset [+80]
  4  - Directory Name Length (excl. nulls)
  28 - Directory Name (null-terminated)
  64 - Filename
  4  - Unknown
  24 - null

Directory Entries (per file):
  2  - Unknown (2)
  2  - Unknown
  4  - Length of Next FOR Loop (numFiles * 8)
  // for each file:
    4  - Hash?
    4  - null

  2  - Unknown (2)
  2  - Unknown
  4  - Length of Next FOR Loop (numFiles * 24)
  // for each file:
    4  - Hash?
    4  - File Offset
    4  - File Length
    4  - Unknown (263324)
    4  - Padding Multiple? (256)
    4  - null

File Data:
  X  - File Data (per file)
```

### .BUN Format (Most Wanted)

```
Per file:
  4  - File Type ID?
  4  - File Length
  X  - File Data
// Padding entries exist with varying sizes including length=0
// Continue reading until end of archive reached.
```

### BUN Contents (Undercover)
Opening `.bun` (e.g., `Scene_PIPAngie_BundleB.bun`) with Total Commander **gaup** plugin reveals inner `.bin` files:
- `xx.tpk.bin` — texture packs
- `xx.geo.bin` — geometry
- `xx.unk.bin` — position info, interface buttons, other data (guki, 2010)

### BIN Contents (Underground 2)
- Track `.bin` files (343-byte) = **standard DDS with mips**
- `STREAML4RA.BUN` = mixed DDS + other files
- Car `.bin` files = **TPK-compressed archives** (e.g., `350Z_vinyls.tpk`)
- Textures = **DDS** (DXT1/2/3/5 headers found)

### Tools
| Tool | Purpose | Notes |
|------|---------|-------|
| MWTex | Open NFS BIN textures (UG–Undercover) | |
| BinTex v0.4 | NFSU1 BIN texture extraction to DDS | KorNet, 2005 |
| NFS TexEd | Texture editing (Undercover / Pro Street) | |
| Total Commander + gaup plugin | Open NFS .bin/.bun archives | |
| bin2ase | Convert UG1/2 geometry | Buggy |
| ZModeler 2.0.7 | Open Most Wanted geometry | Buggy |

---

## 3. TPK Archives

`.tpk` files = compressed texture-pack archives **inside** NFS BIN files.
- As of 2006: **could not be opened** — nearly all files inside `.bins` are `.tpk`
- No tool could decompress them at the time (thread, 2006)

---

## 4. BFF Archive Format (Need for Speed: Shift)

### Overview
NFS Shift packs data in `.bff` files (in `Pakfiles`).
- **Not zlib** — probable scheme: **0x800-byte chunks compressed with LZX-like algorithm**
- Identified as **XMemDecompress** (cross-platform Xbox LZX function) (aluigi, 2009)
- Some BFFs also use **RC4 encryption**

### RC4 Key (PC)
```
@lLy0urRaC3ar3bE  →  "All Your Race Are Belong to us"
```
Applied when `X12d` flag = 2 (`encryption rc4 "@lLy0urRaC3ar3bE"`) in QuickBMS script.

### Key Tool
- **QuickBMS + `nfsshift.bms`** — extracts BFF files (aluigi, 2009)

### BFF Header
```
Offset  Size  Description
0x00    4     " KAP"  ("PAK " in big endian)
0x04    4     0x03004010
0x08    4     file count
0x0C    4     alignment
0x10    4     zero
0x14    4     0x800 (chunk size)
0x18    0x100 name of archive
0x118   4     X118 - size of information table
0x11C   4     zero
0x120   4     X120 - size of name table + 0x308
0x124   4     DHSA field offset
0x128   4     DHSA field size
0x12C   1     zero
0x12D   1     X12d - 0 = no encryption, 2 = RC4
0x12E   1     zero
0x12F   1     zero
0x130   X118  information table
...           "TXEP" magic
...           not verified
...           0x100  Gecko.xml or GeckoDemo.xml
...           0x100  C:\DEV\Gecko\ or C:\DEV\GeckoDemo\
...           0x100  PC
...           X120  name table
```

### Information Table Entry (48 bytes per file)
```
4  - unused (any value, game works)  [64-bit timestamp-ish]
4  - unused (any value, game works)
4  - absolute offset of file
4  - ever zero
4  - compressed size (size in archive)
4  - extracted size (uncompressed size)
4  - zero
4  - zero
4  - type: 0 = uncompressed, 2 = compressed
4  - zero
4  - CRC of file **as stored in archive** (decrypted but still compressed)
4  - extension (e.g., "fxo")
```
> **Correction** (aluigi, 2009): First two fields are NOT CRC — they are likely 64-bit timestamp. Real CRC is field immediately before extension.

### Name Table Entry (16 bytes)
```
4  - absolute offset where name string is located
4  - zero
4  - not verified
4  - not verified (seems a date timestamp)
```

### CRC Semantics
- **CRC1** — calculated on file **as stored** in archive (decrypted-but-still-compressed data)
- **CRC2** — should be calculated on **final uncompressed** file (not verified)
- Polynomial: standard zlib `0x77073096` table-based CRC32

```c
unsigned int crc = 0xffffffff;
for(i = 0; i < len; i++) {
    crc = crctable[(data[i] ^ crc) & 0xff] ^ (crc >> 8);
}
return crc;
```

### Repacking (japamd, 2009)
1. Insert **uncompressed** file from QuickBMS extraction
2. Change `type` field to `0`, recalculate CRC, `zsize`/`size`, and offsets
3. Set `X12d = 0` (no encryption), `type = 0` (no compression) to avoid compression
4. First two info-table fields don't need recalculation if contents change — **but must be preserved** (game crashes if they differ for certain files)
5. **Cannot add new files** — executable won't understand them. Extracted files can be modified freely.

### `-loose` Command-Line Mode (test mods without repacking)
```bash
shift.exe -loose
```
- Game reads **unpacked** files from disk instead of archives (redilS, 2009)
- **Caveat**: Crashes if loose files aren't in exact expected location — extract to correct paths, remove `Pakfiles` folder
- **`partpak`** — combined with `-loose`, allows both packed + unpacked; alone makes Shift behave "awkward"
- For shipping mods: **BFF Compiler** path preferred — compiled BFFs install on original/compressed game (no `-loose` needed), keep loading times reasonable. `-loose` slows race loading badly.

### BFF Tools
| Tool | Purpose | Notes |
|------|---------|-------|
| QuickBMS + `nfsshift.bms` | Extraction | aluigi |
| BFF Repacker / Injector / Compiler | Repack extracted folders to `.bff` | japamd, requires .NET 2.0; physics files work; vehicle BFFs needed original name table copied in v0.4 |
| Jaeder Naub | Scan/rip files inside BFF | Crude |

### Localization Caveat
`language_English.bff` (in `/UI/Languages`) contains `english.bin` (game strings at offset `0x010164`) and `english_histogram.bin`. String indexing not fully RE'd — changing string length breaks menus.

### PC vs Xbox 360 (Endianness)
| Platform | BFF Magic | Endianness |
|----------|-----------|------------|
| PC | `" KAP"` | **little-endian** |
| Xbox 360 | `"PAK "` | **big-endian** |

- QuickBMS script originally failed on X360 due to endianness; aluigi adapted `nfsshift.bms`
- BFF Repacker/Injector work **only for PC** — no X360 repack tool existed
- X360 content is **360-optimised**: swizzled textures, probably big-endian model coordinates, different `.fsb` format (PC FSB extractor chokes). ZModeler declined 360 model support.

---

## 5. BML / BMT / BMx — Binary Material/Setup Files (Shift)

### Overview
NFS Shift has many `.BML` and `.BMx` files = **compiled (binary) versions of plain XML files**.
- Editing XML doesn't change game — must edit compiled `.bml`/`.bmt`
- `.bml` files = **same format as `.bmt`** files
- `.bml`/`.bmt` = different format from `.sgb` track files
- **3D SimEd** can open/read `.bml` to display whole track
- BML files convert to human-readable XML which Shift also reads (used for modding)
- Binary Material Files can be converted to XML manually

### MTX Files
Also **BMT files** and **decrypted MTX files** (material XML).

### bmt2xml Tool
- **`bmt2xml`** (peppe, 2010, http://projects.pappkartong.se/bmt2xml/)
- Generates XML from BMT/binary material files
- As of mid-2010: could generate XML but `COLL` block still unknown
- Requires tag/attribute names known — unknown tags may go unhandled

### Endianness Note
- Binary material/BML files may exist as **big-endian X360 materials**
- Converter requested but `bmt2xml` author noted no big-endian samples seen — doesn't handle them

---

## 6. SGB — Track/Scene Files (Shift)

### Overview
`.sgb` files define **tracks/scenes** (binary equivalent of XML `.sgx` scene format).
- **3D SimEd** can read/display them
- Goal: create new tracks by authoring `.sgb`

### Structure (peppe, 2010)
Six sections: **SGB, OCCL, NODE, FLAT, SUMM, END**

Each section starts with:
```
32bit int  section name
32bit int  section length
32bit int  number of blocks in section
```

| Section | Description |
|---------|-------------|
| **SGB** | Header + 1 all-zero byte |
| **OCCL** | 56-byte blocks: two 32-bit ints with string positions (name + file path, e.g., `OCCLUDER02`, `Tracks\Alpental\OCCLUDER02.meb`), then four 3D points (4×3 floats) per block |
| **NODE / SUMM** | Blocks start with 32-bit block-size int. Contain repeated references (file-position ints) pointing to 40-byte blocks: three ints with string positions (type, name, resource path + `.vhf`/`.meb`/`.imb`) + repeated values |
| **FLAT** | One block: 32-byte header (min point=3 floats, max point=3 floats, count, unknown length) + N × 64-byte blocks (16 zero bytes, sphere center/radius, etc.) |
| **END** | Section header only |

**Endianness quirk**: SGB section **length** is in same endianness as section names; **all other lengths and data are in opposite endianness**. Section names may be big- or little-endian depending on file.

### NODE/SUMM Block Semantics (maps to XML `sgx`)
```xml
<OBJ_ID no="1930">
  <NODE type="LOD" Name="GEN_TREE10_LOD0" MatrixNumber="-1">
    <SPHERE Centre="-0.960997 10.404318 -0.533839 1.0" Radius="13.516625" />
    <MATRIX Offset="-449.536987 -4.304690 -196.947006"
            Orientation="0.0 0.0 -0.114994 1.0" />
    <CONTROL Distances="0.000000 " />
    <NODE type="OBJECT" Name="Gen_Tree10_LODA" MatrixNumber="0">
      <RESOURCE Filename="Tracks\_Data\Instances\Gen_Tree10_LODA.imb" />
      <SPHERE Centre="-0.960997 10.404318 -0.533839 1.0" Radius="13.516625" />
    </NODE>
  </NODE>
</OBJ_ID>
```
Fields: ID (unique object number), three string references (type, name, resource path + `.vhf`/`.meb`/`.imb`), count of elements with that name, unknown flag, SPHERE (center x,y,z + radius), MATRIX (3 position floats + parent index, then orientation), `userflags` bitfield.

### Conversion Tool
**`sgbconverter`** (peppe, 2010, http://projects.pappkartong.se/sgbconverter/):
- `sgb → sgb` round-trip = **binary-equal** files
- `sgb → xml → sgb` = **non-binary-equal** (float precision loss via text round-trip) — for editing, not exact reproduction
- **Fails on Shift 2 Unleashed** `.sgb` files (format changed)

---

## 7. Audio Formats

### MUS (Undercover)
- No MUS segment headers = **headerless EA/XA streams** with new stream system
- Without headers: no way to know where one sound ends and next begins
- Still **EA/XA codec** (same as other MUS), different stream system, no `SCHI`/`SCCI` headers
- Codec determined by header (EA SCxl family)
- **FFmpeg** supports several EA custom ADPCM variants (LGPL)
- Simple `SCHII` header insufficient — crucial `SCHI` header supplies number of samples in stream; `.mpf` sidecar files don't contain MUS headers
- **CTPAX-X Team's** NFS tool updated for Undercover, but **does not support interactive (adaptive) music** — only licensed tracks

### Audio / SPS (Hot Pursuit 2010, Xbox 360)
- `.sps` = **packed XMA** (multichannel)
- Tools: `ea_multi_xma` (EA Multi XMA), `xma_test`
- Correct offset for `ea_multi_xma` = `0x0` — error "doesn't look like EA multi XMA stream" comes from wrong offset

### GSTR Audio Streams (Most Wanted)
- Uses **GSTR audio streams**; extracting/uncompressing and reimporting edited audio was a stated goal

### Cop Speech / ASF Audio (Most Wanted)
- Cop speech in `SOUND\SPEECH\copspeech.big`
- Extracted with Dandapani's **`schl`** extractor (`schl.cpp`)
- Converted from **ASF** to WAV using **SX (Sound eXchange)** tool

### FSB Audio (Shift)
- Music/sound packed in **FSB** archives (e.g., `nfs_racemusic.fsb`)
- **Demo password**: `kua0@z^y0$Wsh9HkhG6%Ih*gICV$Do8` (retail FSBs not encrypted)
- **MusicPlayerEx** extracts FSB without password
- **`fsbext -a`** (aluigi) dumps files and builds headers; `xmaencode.exe` sometimes too rigid and fails — **`towav`** (hcs) handles result fine

### Shift (PC) Sound Decoding Pipeline
```
FSB → fsbext → XMA streams → xmaencode.exe / towav → WAV
```

---

## 8. XTX Textures (Shift, Xbox 360)

### Overview
`.xtx` files in NFS Shift (Xbox 360) = **partial XPR2** (Xbox 360 texture container) files.
- Convertible to **TGA**

### Tools
- **`XTX_TO_TGA.exe`** (shadowmoy, 2010) — drop `.xtx` to get `.tga`
- Also converts **Forza 3 `.xds`** textures
- Known issues: filenames cut off, no batch mode, normal-map blue channel sometimes inverted
- Works on Shift X360 and Forza 3 textures

---

## 9. Materials / Shaders / Misc

- Shift files mostly **XML-styled or binary XML** except shaders, textures, models, sounds, MoTeC/HUD files
- **Vinyls** (Pro Street): ~40 MB file with ~150 vinyls; likely **vector** data (always high quality). NFS TexEd did not handle them
- Most Wanted **PS2**: textures/geometry/vinyls `.bin` names also appear in PS2 ELF

---

## 10. Tools Reference Summary

| Tool | Purpose | Author |
|------|---------|--------|
| MultiEx Commander | VIV/BIG archives | WATTO |
| QuickBMS + `nfsshift.bms` | BFF extraction | aluigi |
| BFF Repacker/Injector/Compiler | BFF repack | japamd |
| Jaeder Naub | Scan/rip inside BFF | — |
| MWTex / BinTex / TexEd | BIN texture editing | community |
| Total Commander + gaup | Open .bin/.bun | ghisler / totalcmd.net |
| bin2ase | UG1/2 geometry convert | — (buggy) |
| ZModeler 2.0.7 | Most Wanted geometry | — (buggy) |
| `XTX_TO_TGA.exe` | XTX/XDS → TGA | shadowmoy |
| `ea_multi_xma` / `xma_test` | XMA audio decode | EA / community |
| `sgbconverter` | SGB ↔ XML scene conversion | peppe |
| 3D SimEd | View/read SGB tracks | sim-garage.co.uk |
| `fsbext` (`-a`) | FSB archive extraction | aluigi |
| `towav` / `xmaencode.exe` | XMA stream decode | hcs / EA |
| `bmt2xml` | BMT binary material → XML | peppe |
| CTPAX (CTPAX-X Team) | NFS audio tool (no interactive music) | CTPAX-X |
| `schl` (schl.cpp) | Most Wanted cop-speech extraction | Dandapani |
| SX (Sound eXchange) | ASF → WAV conversion | — |
| MusicPlayerEx | FSB sound extraction (no password) | — |

---

## Cross-References
- **SimCity audio** (RIFF Vorbis, `ww2ogg`/`revorb`) → [`xentax-simcity-knowledge.md`](../simcity/xentax-simcity-knowledge.md)
- **Generic EA formats** (FIFA, C&C share VIV/BIG) — see EA modding communities
- **XPR2 / XDS** (Xbox 360 textures) — Forza 3 uses same XTX_TO_TGA tool