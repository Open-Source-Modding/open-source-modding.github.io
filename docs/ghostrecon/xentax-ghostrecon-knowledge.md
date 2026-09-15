# Tom Clancy's Ghost Recon — XeNTaX Forum Knowledge

Extracted and organized from XeNTaX forum discussions (2005–2018).
Covers the Ghost Recon series across multiple engines and platforms:
Ghost Recon 2 (Unreal), GRAW (Yeti/GEAR), GRAW PS2, and Ghost Recon Future Soldier.

---

## 1. Ghost Recon 2 (Unreal Engine)

**Topic 1157** (2005). Ghost Recon 2, like Ghost Recon 1 and other Tom Clancy
games, uses the **Unreal engine**. As of 2005, **MexCom (MultiEx)** could not
handle these files. (friendsofwatto, 2005-04-06)

- Contact: WATTO (watto@watto.org, http://www.watto.org).

## 2. Ghost Recon Advanced Warfighter (GRAW) — *.bundle (Yeti/GEAR)

**Topic 2712** (2007). GRAW (PC) uses **`.bundle`** files, which employ an
**XMB-based** structure similar to Ensemble Studios (Age of Empires).
(Rheini, 2007-07-17)

### .bundle File Format (itg, 2007-07-24)

Header:
```
DWORD: Header    // "BNDL"
DWORD: Version   // 0x02
ULONG: TOC size  // e.g. 0x021F6
```

**TOC markers:**
| Marker | Meaning |
|--------|---------|
| `01` | file name |
| `0101` | directory |
| `02` | file info |
| `03` | move to parent dir |

**Entry layout:**
```
string   filename = data/anims/ghost/ghost_machine.xmb
ulong    offset   = 0x021cbdfc
DWORD    filesize = 0x0703D82
```

**Example TOC hex:**
```
00000010 0101 6461 7461 0001 0161 6E69 6D73 0001 ..data...anims..
00000020 0167 686F 7374 0002 FCBD 1C02 0000 0000 .ghost..........
00000030 823D 7000 0167 686F 7374 5F6D 6163 6869 .=p..ghost_machi
00000040 6E65 2E78 6D62 00                       ne.xmb.
```
(`0101` = dir "data", `01` = "anims", `01` = "ghost", `02` = file info,
then filename `ghost_machine.xmb` + offset + filesize)

```
000001C5 0301 0167 7569 0002 308A 2E01 0000 0000 ...gui..0.......
000001D5 52D7 0200 0161 6C65 7274 2E78 6D62 00   R....alert.xmb.
```
(`03 0101` = go to parent dir and enter `gui`)

### .bundle Tools

- **GRAWbundle-reader v1.0** (`GRAWbundle-readerv1.0.zip`) — *.bundle unpacker.
  (KorNet, 2007-07-17)
- **itg's tool** — extraction + **packing** abilities (repacking `patch.bundle`
  works; game loads fine after repack). (itg, 2007-07-23)

> **Ubisoft SS files**: Users asked whether a tool could be written for
> Ubisoft's **SS0** / SS-numbered files. (kimkalisto, 2007-07-20)

## 3. GRAW (PS2) — *.PKG Archives

**Topic 4034** (2010). Two QuickBMS scripts for GRAW PS2:

### PS2 IMG/container script (aluigi, 2010-01-11)

A script reading a full-size container: header `FULLSIZE`, `OFFSET1`,
`INFO_OFF`, `OFFSET3`, `NAMES_OFF`, `BASE_OFF`. The number of files is
`(OFFSET3 - INFO_OFF) / 0x30`. Each entry record is **0x30 bytes**:

| Offset | Type | Field |
|--------|------|-------|
| +0x00 | long | NAME_OFF (relative to NAMES_OFF) |
| +0x04 | long | CURR_ID |
| +0x08 | long | PREV_ID (0 = this is the leaf/full name) |
| +0x0C | long | NEXT_ID |
| +0x10 | long | DUMMY4 |
| +0x14 | long | TYPE |
| +0x18 | long | ZSIZE |
| +0x1C | long | SIZE |
| +0x20 | long | OFFSET |
| +0x24..0x2C | long | DUMMY6/7/8 |

Filenames are reconstructed from the linked-list via PREV_ID (each node prepends
its name to the child path). If `ZSIZE == SIZE` the file is stored raw, else
`clog` (compressed).

### GRAW PS2 *.PKG script (AlphaTwentyThree, 2010-01-23)

```bms
# extracts the PKG archives used in
# Ghost Recon: Advanced Warfighter (PS2)
# (c) 2010 by AlphaTwentyThree of Xentax

get DATASTART long
get FILES long

for i = 1 <= FILES
   savepos POS
   get NAMEPOS long
   math NAMEPOS + POS
   get OFFSET long
   savepos POS

   goto NAMEPOS
   getdstring NAME 0x40
   goto POS

   if i == FILES
      get SIZE asize
   else
      get DUMMY long
      get SIZE long
   endif
   math SIZE - OFFSET
   log NAME OFFSET SIZE

   goto POS
next i
```

- Header: `DATASTART` (long) + `FILES` (long).
- Per entry: NAMEPOS (relative to current pos) then OFFSET; filename is a
  fixed **0x40-byte** string; size computed as next offset minus current.

## 4. GRAW (Xbox 360) — Audio

**Topic 4089** (2010). Xbox version audio is **xbadpcm** (Xbox ADPCM).

- Use **Luigi's Xbox ADPCM decoder** (`xbadpdec.exe`):
  http://aluigi.org/papers.htm#xbox
- The container has several tracks separated by **zero blocks** — no header
  inside the container. Find the correct stream start.
  - First file offset `0`, second offset `0x4B800`. (Kataah, 2010-02-03)
  - `xbadpdec.exe mp_01_nl.SS2 stream.wav` (all tracks need correct start).
- PS2 version: audio is **PS2 ADPCM**, not `.vox`. Track offsets need manual
  locating (e.g. `0x227810` for the next track). (Kataah, 2010-02-03)

> **Issue**: streams can be "scratched" (interleave change ~3.5s before the end
> of each decoded PS2 stream). (AlphaTwentyThree, 2010-02-06)

## 5. Ghost Recon Future Soldier — Yeti/GEAR BIG Archives

**Topic 9259 / 9554 / 9555** (2012–2018). GRFS uses **Yeti Engine (GEAR BigFile)**
archives.

### Yeti Engine (GEAR BigFile) script (Ekey, 2012-07-13)

```bms
# Yeti Engine (GEAR BigFile)
# script for QuickBMS http://quickbms.aluigi.org

idstring "[ GEAR BigFile ]"
get DUMMY long
get DUMMY long
get FILES long
get DUMMY long

savepos OFFSET_OFF
for i = 0 < FILES
    get DUMMY long
    get DUMMY long
next i

savepos SIZE_OFF
for i = 0 < FILES
    get DUMMY long
next i

savepos CRC_OFF         # crc of the filenames?
for i = 0 < FILES
    get DUMMY long
next i

savepos DUMMY_OFF
for i = 0 < FILES
    get DUMMY string    # or byte?
next i

for i = 0 < FILES
    goto OFFSET_OFF
    get OFFSET long
    get OFFSET64 long
    savepos OFFSET_OFF

    goto SIZE_OFF
    get SIZE long
    savepos SIZE_OFF

    goto CRC_OFF
    get CRC long
    savepos CRC_OFF

    string CRC p= "%08x" CRC
    log CRC OFFSET SIZE
next i
```

- Magic: `[ GEAR BigFile ]` (with brackets).
- Filenames are **hashed (CRC32)**; the `.report` file gives original names.
- Works with files in the **Sound_Bin_PC** folder. (deepshit, 2012-07-15)
- For the main-dir archives, change the idstring to `"YBIG"`. (Ekey, 2012-07-17)

### YBIG / multi-part BIG archives

- **`yeti2.bms`** (aluigi): http://aluigi.org/papers/bms/yeti2.bms — for `YBIG`,
  but reported **incomplete/broken** (extracts 1KB files). (deepshit, 2012-07-18)
- **`yeti_gear.bms`** (aluigi): http://aluigi.org/papers/bms/yeti_gear.bms
- **`yeti_ybig.bms`** (aluigi, 2013-03-07): http://aluigi.org/papers/bms/yeti_ybig.bms
  — fixed 2013-03-17: **non-compressed files don't have the SIZE field**.
  Output files have **no extensions** in the names. (aluigi, 2013-03-17)

### Manual multi-part merge workaround (GRiNDERKILLER, 2018-09-23)

For GRFS 1.8 (non-Steam), `Yeti.big`, `Yeti.big1`, `Yeti.big2`, `Yeti.big3`
can be merged into a single `Yeti_1.big`:

1. Open `Yeti.big` and go to byte offset `1487572640` (hex `0x58AA8EA0`).
2. `Yeti.big1`: remove first **65536 bytes**, paste the rest at that offset.
3. `Yeti.big2`: remove first 65536 bytes, paste at `2975102352` (`0xB1547590`).
4. `Yeti.big3`: remove first 65536 bytes, paste at `4463445720` (`0x10A0AC6D8`).
5. Save as `Yeti_1.big` and use a BMS script to unpack.
6. Expected total size: **5,896,484,568 bytes** (else wrong version).

> The multi-part BIG scheme splits the archive into 64KB-chunked continuation
> files (Yeti.big1–3 are the 65536-byte-aligned continuation chunks).

## Tools Reference

| Tool | Purpose | Author |
|------|---------|--------|
| GRAWbundle-reader v1.0 | *.bundle unpacker | KorNet |
| itg's bundle tool | bundle extract + repack | itg |
| MexCom (MultiEx) | general archives (can't do GR2 Unreal) | WATTO |
| QuickBMS + scripts | GRAW PS2 PKG/IMG, GRFS BIG | aluigi / AlphaTwentyThree |
| xbadpdec.exe | Xbox ADPCM decode | Luigi Auriemma |
| yeti2.bms / yeti_gear.bms / yeti_ybig.bms | Yeti BIG extraction | Luigi Auriemma |

## Key Facts

- **Ghost Recon 2**: Unreal engine (MexCom unsupported as of 2005).
- **GRAW PC**: `.bundle` archives, XMB TOC structure, magic `BNDL` v0x02.
- **GRAW PS2**: `.PKG` archives (0x40-char names, DATASTART/FILES header) and
  IMG containers with 0x30-byte linked-list entries.
- **GRAW Xbox 360**: audio = xbadpcm, track-separated by zero blocks, no header.
- **GRFS**: Yeti/GEAR BigFile — `[ GEAR BigFile ]` and `YBIG` magics, CRC32
  filenames, multi-part BIG1–3 chunks (65536-byte alignment).
