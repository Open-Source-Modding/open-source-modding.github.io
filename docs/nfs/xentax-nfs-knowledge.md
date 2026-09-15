# Need for Speed — XeNTaX Forum Knowledge

Extracted from XeNTaX forum discussions (2005–2010). Covers the **Black Box
era** NFS games (Underground → Shift) plus Hot Pursuit (2010). The formats are
largely shared across the series with per-game variations.

---

## 1. VIV / BIG Archives (Underground / Hot Pursuit 2 era)

### Overview

NFS games (and other EA titles) use **VIV** and **BIG** archives — the same
container format as the FIFA games. These are used by EA across FIFA, Need for
Speed, and Command & Conquer. *(friendsofwatto, 2005-01-09)*

- **`sdata.viv`** — the main archive in Underground 2, containing all music and
  sounds. After decompressing it you find `.big` and `.mus` files.
  *(jomasaco, friendsofwatto, 2005)*
- **MultiEx Commander (MexCom)** can open BIG and VIV archives — same format as
  FIFA. Hot Pursuit 2 is the same. *(friendsofwatto, 2005-01-09)*
- Mr.Mouse: "The format is indeed the same as big, just different ID strings."
  Scripts were implemented in the next MexCom release. *(Mr.Mouse, 2005-01-09)*

---

## 2. BIN / BUN Archive Formats (Underground, Most Wanted, Undercover)

### Overview

NFS `.bin` and `.bun` files are **packages** containing most in-game content:
car textures, vinyls, geometry, and more. *(Jille, 2005-09-09)*

### .BIN format (Need for Speed: Most Wanted)

Reverse-engineered structure (from the XeNTaX wiki / thread analysis, 2006):

```
+-----------------------------------+
| Need For Speed: Most Wanted *.bin |
+-----------------------------------+

// ARCHIVE HEADER
  2 - null
  2 - Unknown
  4 - Archive Length [+8]
  4 - null
  4 - Number Of Nulls (48)
  48 - null
  2 - null
  2 - Unknown
  4 - Directory Length?
  2 - Number Of Directories (1)
  2 - Unknown

  // for each directory
    4 - Directory Offset [+80]
    4 - Directory Name Length (not including nulls)
    28 - Directory Name (null terminated)
    64 - Filename
    4 - Unknown
    24 - null

// DIRECTORIES
  // for each directory
    2 - Unknown (2)
    2 - Unknown
    4 - Length of the Next FOR Loop (ie numFiles*8)
    // for each file
      4 - Hash?
      4 - null
    2 - Unknown (2)
    2 - Unknown
    4 - Length of the Next FOR Loop (ie numFiles*24)
    // for each file
      4 - Hash?
      4 - File Offset
      4 - File Length
      4 - Unknown (263324)
      4 - Padding Multiple? (256)
      4 - null

// FILE DATA
  // for each file
    X - File Data
```

### .BUN format (Need for Speed: Most Wanted)

```
+-----------------------------------+
| Need For Speed: Most Wanted *.BuN |
+-----------------------------------+

// Note: There are padding file entries of varying sizes, including length=0
// Continue reading until the end of the archive has been reached.

// for each file
  4 - File Type ID?
  4 - File Length
  X - File Data
```

The **BIN** and **BUN** formats are **two distinct formats**, not the same.
*(wiki / thread analysis, 2006)*

### BUN contents (Undercover)

Opening a `.bun` (e.g. `Scene_PIPAngie_BundleB.bun`) with Total Commander's
**gaup** plugin shows the inner `.bin` files:
- `xx.tpk.bin` — texture packs
- `xx.geo.bin` — geometry
- `xx.unk.bin` — position info, interface buttons, and other data
*(guki, 2010-07-24)*

### BIN contents (Underground 2)

- Track `.bin` files (343-byte) are **standard DDS with mips**.
- `STREAML4RA.BUN` is mixed DDS and other files.
- Car `.bin` files are **TPK-compressed archives**
  (e.g. `Vehicles\Cars\UG2\350Z\350Z_vinyls.tpk`).
- Textures are **DDS** format (DXT1/2/3/5 headers found).
*(SparedLife, KorNet, 2005-09-09)*

### Tools

| Tool | Purpose |
|------|---------|
| MWTex | Open NFS BIN textures (Underground–Undercover) |
| BinTex v0.4 | NFSU1 BIN texture extraction to DDS (KorNet, 2005) |
| NFS TexEd | Texture editing (Undercover / Pro Street) |
| Total Commander + gaup plugin | Open NFS .bin/.bun archives |
| bin2ase | Convert some Underground 1/2 geometry (buggy) |
| ZModeler 2.0.7 | Open Most Wanted geometry (buggy) |

*(rafal345, 2010-06-07; guki, 2010-07-24)*

---

## 3. TPK Archives

`.tpk` files are the compressed texture-pack archives inside NFS BIN files.
As of 2006 they could **not be opened** — nearly all files inside the `.bins`
are `.tpk`, and no tool could decompress them at the time. *(thread, 2006)*

---

## 4. BFF Archive Format (Need for Speed: Shift)

### Overview

NFS Shift packs data in **`.bff`** files (in `Pakfiles`). They are **not** zlib;
the most probable scheme is **0x800-byte chunks compressed with an LZX-like
algorithm** — aluigi identified it as **XMemDecompress** (the cross-platform
Xbox LZX function). Some BFFs also use **RC4 encryption**. *(aluigi, 2009-09)*

**RC4 key (PC):** `@lLy0urRaC3ar3bE` — which decodes to **"All Your Race Are
Belong to us"**. *(aluigi, 2009-09/2010)* This is the key the `nfsshift.bms`
script applies when the `X12d` flag equals `2` (`encryption rc4
"@lLy0urRaC3ar3bE"`).

**Key tool:** aluigi's `nfsshift.bms` QuickBMS script extracts BFF files.
*(aluigi, 2009-09-19)*

### BFF Header (aluigi, 2009-09-22)

```
BFF header:
bytes   description
4       " KAP"  ("PAK " in big endian)
4       0x03004010
4       files
4       alignment
4       zero
4       0x800
0x100   name of the archive
4       X118, size of the information table
4       zero
4       X120, size of the name table + 0x308
4       DHSA field offset
4       DHSA field size
1       zero
1       X12d, set it to 0 (0 = no encryption)
1       zero
1       zero
X118    information table
4       "TXEP"
4       not verified
0x100   Gecko.xml or GeckoDemo.xml
0x100   C:\DEV\Gecko\ or C:\DEV\GeckoDemo\
0x100   PC
X120    name table
```

### Information Table entry (per file, 48 bytes)

```
4       unused (set as you want, game works in any case)
4       unused (set as you want, game works in any case)  [64-bit timestamp-ish]
4       absolute offset of the file
4       ever zero
4       size of the file in the archive (compressed size)
4       size of the extracted file
4       zero
4       zero
4       type, set it to 0 (2 = compressed files)
4       zero
4       CRC of the file as-is in the archive
4       extension (example: fxo)
```

> **Correction (aluigi, 2009-09-22):** the first two fields are **not** the CRC
> — they are probably a 64-bit timestamp and can be set to any value. The real
> CRC of the stored file is the field **immediately before the extension**.

### Name Table entry (16 bytes)

```
4       absolute offset where the name string is located
4       zero
4       not verified
4       not verified (seems a date timestamp)
```

### CRC semantics (aluigi, 2009-09-22)

- **CRC1** — calculated on the file **as stored** in the archive (i.e. over the
  decrypted-but-still-compressed data).
- **CRC2** — should be calculated on the **final uncompressed** file
  (not verified).
- The CRC is the standard zlib polynomial `0x77073096` table-based CRC32.

```
unsigned int crc = 0xffffffff;
for(i = 0; i < len; i++) {
    crc = crctable[(data[i] ^ crc) & 0xff] ^ (crc >> 8);
}
return crc;
```

### Repacking (japamd, 2009-09-23)

To repack a BFF after editing:
- Insert the **uncompressed** file generated by QuickBMS, changing the `type`
  field to `0` and recalculating CRC, `zsize`/`size`, and offsets.
- Set `X12d = 0` (no encryption) and `type = 0` (no compression) to avoid
  needing compression.
- The first two info-table fields **don't need recalculation** if contents
  change, but must be preserved — the game crashes if they differ from originals
  (only for certain files).
- **You cannot add new files** — the game executable won't understand them.
  Extracted files can be modified freely.

### `-loose` command-line mode (test mods without repacking)

Instead of repacking BFFs after every edit, launch Shift with the **`-loose`**
command-line parameter (e.g. `shift.exe -loose`): the game then reads the
**unpacked** files from disk rather than the archives. *(redilS, 2009-09-27)*

- **Caveat:** `-loose` crashes if the loose files aren't in the exact location
  the game expects — you must extract them to the correct paths and remove the
  `Pakfiles` folder. *(redilS, 2009-09-28)*
- **`partpak`** — a related switch that, in combination with `-loose`, allows
  using **both packed and unpacked** files; on its own `partpak` makes Shift
  behave "awkward". *(redilS, 2009-09-28)*
- For shipping mods, the BFF **Compiler** path is preferred over `-loose`:
  compiled BFFs install on an original/compressed game (no `-loose` needed) and
  keep loading times reasonable — `-loose` slows race loading badly.
  *(japamd, 2009-10-04)*

### BFF tools

- **QuickBMS + `nfsshift.bms`** — extraction (aluigi).
- **BFF Repacker / Injector / Compiler** (japamd, 2009-09-25, requires .NET
  Framework 2.0) — repack extracted folders back to `.bff`. Repacking works for
  physics files; vehicle BFFs needed the original name table copied in v0.4.
- **Jaeder Naub** — scan/rip files inside BFF archives (crude).

### Localization caveat

`language_English.bff` (in `/UI/Languages`) contains `english.bin` (game strings
starting at offset `0x010164`) and `english_histogram.bin`. The string
indexing was **not** fully reverse-engineered — changing a string's length broke
the menus. *(japamd, 2009-09-23)*

### PC vs Xbox 360 (endianness)

The PC and Xbox 360 versions of Shift store **mirrored** data:

| Platform | BFF magic | Endianness |
|----------|-----------|------------|
| PC | `" KAP"` | **little**-endian |
| Xbox 360 | `"PAK "` | **big**-endian |

*(paldrive, 2010-07-25; desperados, 2011-04-29)*

- The QuickBMS script originally failed on X360 because of the different
  endianness; aluigi adapted `nfsshift.bms` for it. *(Mr.Mouse, aluigi,
  2010-02/03)*
- **BFF Repacker / Injector** work only for the **PC** version — no X360 repack
  tool existed (the difference is the mirrored `kap`/`pak` data).
  *(paldrive, 2010-07-25; desperados, 2011-04-29)*
- Beyond endianness, X360 content is broadly **360-optimised**: swizzled
  textures, model coordinates probably big-endian, and a **different `.fsb`
  format** that the PC FSB extractor chokes on. ZModeler declined 360 model
  support. *(dieinafire, 2010-04-22)*

---

## 5. BML / BMT / BMx — Binary Material/Setup Files (Shift)

### Overview

NFS Shift has many `.BML` and `.BMx` files that look like **compiled (binary)
versions of plain XML files**. Editing the XML doesn't change the game — you
must edit the compiled `.bml`/`.bmt`. *(japamd, 2009-09-30; peppe, 2010-08-01)*

- `.bml` files are the **same format as `.bmt`** files. *(peppe, 2010-08-01)*
- `.bml`/`.bmt` are a different format from `.sgb` track files. *(peppe)*
- `3D SimEd` can open/read `.bml` to display the whole track.
- BML files convert to **human-readable XML** which Shift also reads (used for
  modding). Binary **Material Files** can be converted to XML manually.
  *(Simon, 2010-05-24)*

### Related: MTX files

There are also **BMT files** and **decrypted MTX files** (material XML).
*(Simon via Quadcoremax, 2010-05-17)*

### bmt2xml tool

**`bmt2xml`** (peppe, 2010-07-10, http://projects.pappkartong.se/bmt2xml/)
generates **XML** from BMT/binary material files (as of mid-2010 it could
generate XML but the `COLL` block was still unknown). It requires the tag and
attribute names to be known, so unknown tags may go unhandled. *(peppe,
2010-07-10)*

### Endianness note (materials)

- Binary material/BML files may exist as **big-endian X360 materials**; a
  converter was requested to read them, but `bmt2xml` author noted no
  big-endian samples were seen, so it doesn't handle them. *(Simon,
  2010-07-03; peppe, 2010-07-10)*

---

## 6. SGB — Track/Scene Files (Shift)

### Overview

`.sgb` files define **tracks/scenes** (the equivalent of the XML `.sgx` scene
format). They are binary. `3D SimEd` can read/display them. The goal was to
create new tracks by authoring `.sgb`. *(Simon, 2010-05-12)*

### Structure (peppe, 2010-07-19)

A `.sgb` contains six sections: **SGB, OCCL, NODE, FLAT, SUMM, END**.

Each section starts with:
```
32bit int  section name
32bit int  section length
32bit int  number of blocks in section
```

- **SGB** — just a section header + 1 all-zero byte.
- **OCCL** — each block is a **56-byte** structure: two 32-bit ints with string
  positions (a name + file path, e.g. `OCCLUDER02`, `Tracks\Alpental\
  OCCLUDER02.meb`), then **four 3D points (4×3 floats)** per block.
- **NODE / SUMM** — each block starts with a 32-bit block-size int. Blocks
  contain repeated references (file-position ints) to other places, which
  reference strings. These mostly point at the start of a **40-byte block**
  starting with three ints containing string positions (e.g. `OBJECT`,
  `Tyres2_LODA`, `Tracks\Alpental\Tyres2_LODA.imb`) followed by repeated values.
- **FLAT** — always seems to consist of one block: a 32-byte header (a min point
  = 3 floats, a max point = 3 floats, a count, an unknown length) followed by
  that many 64-byte blocks (16 zero bytes, a sphere center/radius, etc.).
- **END** — just a section header.

**Endianness quirk:** the SGB section **length** is in the same endianness as
the section names; **all other lengths and data are in the opposite**
endianness. Section names may be big- or little-endian depending on the file.
*(peppe, 2010-07-19)*

### NODE/SUMM block semantics (peppe, 2010-07-25)

A block maps roughly to the XML scene (`sgx`) format:
```
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

Fields decoded: ID (unique object number), three string references (type, name,
resource path + `.vhf`/`.meb`/`.imb` file), a count of elements with that name,
an unknown flag, a SPHERE (center x,y,z + radius), a MATRIX (3 position floats
+ parent index, then orientation), and a `userflags` bitfield.

### Conversion tool

**`sgbconverter`** (peppe, 2010-08-04, http://projects.pappkartong.se/sgbconverter/):
- `sgb → sgb` round-trip produces **binary-equal** files.
- `sgb → xml → sgb` produces **non-binary-equal** files (float precision loss
  via text round-trip), so it's for editing, not exact reproduction.
- Note: it **fails on Shift 2 Unleashed** `.sgb` files (format changed).
  *(rollobollo, 2011-04-06)*

---

## 7. Audio Formats

### MUS (Undercover)

- NFS Undercover's `.mus` files have **no MUS segment headers** — they are
  **headerless EA/XA streams** with a new stream system. Without headers there's
  no way to know where one sound ends and the next begins.
  *(OrangeC, Rheini, 2008-11)*
- Still the **EA/XA codec** (the same codec as other MUS files), just a
  different stream system and no `SCHI`/`SCCI` headers.
- The codec is determined by the header (EA SCxl family).
  *(Rheini, via wiki.multimedia.cx)*
- **FFmpeg** supports a number of EA custom ADPCM variants (LGPL).
  *(jfwfreo, 2008-11-27)*
- A simple `SCHII` header isn't enough — the crucial `SCHI` header supplies the
  number of samples in the stream; the `.mpf` sidecar files don't contain the
  MUS headers. *(Rheini, OrangeC, 2008-11)*
- **CTPAX-X Team's** NFS tool was updated to support Undercover, but it does
  **not** support interactive (adaptive) music — only licensed tracks.
  *(OrangeC, 2008-12-21)*

### Audio / SPS (Hot Pursuit 2010, Xbox 360)

- `.sps` audio looks like **packed XMA** (multichannel). Tools: `ea_multi_xma`
  (EA Multi XMA), `xma_test`.
- To decode with `ea_multi_xma`, the correct **offset is `0x0`** — the error
  "doesn't look like an EA multi XMA stream" comes from using a wrong offset.
  *(OrangeC, 2010-10-26; RedDeadRedemption, 2010-12-01)*

### GSTR audio streams (Most Wanted)

- Most Wanted uses **GSTR audio streams**; extracting/unscompressing and
  reimporting edited audio was a stated goal. *(Gocha, 2009-08-16)*

### Cop speech / ASF audio (Most Wanted)

- The **cop speech** audio lives in `SOUND\SPEECH\copspeech.big`. It was
  extracted with Dandapani's **`schl`** extractor (source:
  `schl.cpp`) and converted from **ASF** format to WAV using the **SX
  (Sound eXchange)** tool. *(Dandapani, 2007-07-13)*

### Sound effect extraction (Shift)

- Car sound effect extraction for Shift was a requested feature. *(diablojin,
  2010-01-05)*

### FSB audio (Shift)

- NFS Shift packs music/sound in **FSB** archives (e.g. `nfs_racemusic.fsb`).
- **Demo password:** the FSB archives of the **demo** version are encrypted;
  when `fsbext` prompts for a password during extraction, use
  `kua0@z^y0$Wsh9HkhG6%Ih*gICV$Do8`. The **retail** FSBs are **not** encrypted.
  *(aluigi, 2009-10-04)*
- **MusicPlayerEx** can extract the FSB sound archives **without** a password.
  *(Baron Greenback, 2009-10-04)*
- **`fsbext -a`** (aluigi's FSB extractor) dumps the files and builds their
  headers; however **`xmaencode.exe`** is sometimes too rigid and fails to
  decode them — **`towav`** (hcs) handles the result fine. *(aluigi, hcs,
  2010-04/05)*

### Shift (PC) sound decoding — XMA pipeline

- **`fsbext`** extracts FSB archives; **`xmaencode.exe`** / **`towav`** decode
  the contained XMA streams. *(aluigi, 2010-04-30; hcs, 2010-05-10)*

---

## 8. XTX Textures (Shift, Xbox 360)

### Overview

`.xtx` files in NFS Shift (Xbox 360) are **partial XPR2** (Xbox 360 texture
container) files. They can be converted to **TGA**.
*(shadowmoy, 2010-04-28)*

### Tools

- **`XTX_TO_TGA.exe`** (shadowmoy, 2010-04-29) — drop an `.xtx` onto it to get a
  `.tga`. Also converts **Forza 3 `.xds`** textures. Known issues: filenames get
  cut off, no batch mode, normal-map blue channel sometimes inverted.
  *(Veegie, 2010-05-06)*
- Works on Shift X360 and Forza 3 textures. *(TomWin, bigBear, 2010)*

---

## 9. Materials / Shaders / Misc

- **Shift files** are mostly XML-styled or **binary XML** except shaders,
  textures, models, sounds, and MoTeC/HUD files. *(Simon, 2010-05-24)*
- **Vinyls** (Pro Street): vinyls live in a ~40 MB file with ~150 vinyls; likely
  **vector** data (always high quality). NFS TexEd did not handle them.
  *(rafal345, 2010-08-04)*
- Most Wanted **PS2**: textures/geometry/vinyls `.bin` names also appear in the
  PS2 ELF. *(Andreas, 2009-10-03)*

---

## Tools Reference

| Tool | Purpose | Author |
|------|---------|--------|
| MultiEx Commander | VIV/BIG archives | WATTO |
| QuickBMS + `nfsshift.bms` | BFF extraction | aluigi |
| BFF Repacker/Injector/Compiler | BFF repack | japamd |
| Jaeder Naub | scan/rip inside BFF | — |
| MWTex / BinTex / TexEd | BIN texture editing | community |
| Total Commander + gaup | open .bin/.bun | ghisler / totalcmd.net |
| bin2ase | UG1/2 geometry convert (buggy) | — |
| ZModeler 2.0.7 | Most Wanted geometry | — |
| `XTX_TO_TGA.exe` | XTX/XDS → TGA | shadowmoy |
| `ea_multi_xma` / `xma_test` | XMA audio decode | EA / community |
| `ww2ogg` / `revorb` | (SimCity RIFF Vorbis, see simcity doc) | hcs64 |
| sgbconverter | SGB ↔ XML scene conversion | peppe |
| 3D SimEd | view/read SGB tracks | sim-garage.co.uk |
| `fsbext` (`-a`) | FSB archive extraction | aluigi |
| `towav` / `xmaencode.exe` | XMA stream decode | hcs / EA |
| `bmt2xml` | BMT binary material → XML | peppe |
| CTPAX (CTPAX-X Team) | NFS audio tool (no interactive music) | CTPAX-X |
| `schl` (schl.cpp) | Most Wanted cop-speech extraction | Dandapani |
| SX (Sound eXchange) | ASF → WAV conversion | — |
| MusicPlayerEx | FSB sound extraction (no password) | — |

---

*Source: XeNTaX forum threads t=1038, t=1494, t=1632, t=3225, t=3655, t=3717,
t=3752, t=3757, t=4015, t=4155, t=4158, t=4381, t=4411, t=4451, t=4467,
t=4523, t=4573, t=4797, t=4840, t=5296, t=5397, t=5401.*
