# Left 4 Dead — XeNTaX Forum Knowledge

Extracted and organized from XeNTaX forum discussions (2008–2017).
Covers Left 4 Dead PC (Source engine) and Xbox 360 (L4D360) formats.

---

## 1. VPK Archives (PC — Source engine)

**Topic 3219** (2008). With the L4D demo release, Valve shipped game files not in
the standard `.gcf` format used by other Source games, but in a series of
`.vpk` files. These VPKs differ from the earlier `.vpk` used in
*Vampire: The Masquerade — Bloodlines*. (Mystfit, 2008-11-13)

**Tools** (Rick/Gibbed, 2008-11-14):
- `Gibbed.Valve.ExtractPackage` — VPK extraction tool by Rick.
- Requires **.NET Framework 2.0**; without it, throws
  `System.IO.FileNotFoundException` for `System.Windows.Forms`. (asmxtx, 2008-11-26; fix: nicoli_s)

> **Practical note**: On XP, the tool fails silently unless the GUI .EXE is
> patched to console mode; missing .NET 2.0 is the usual cause.

## 2. Xbox 360: VTFX Textures

**Topic 3257** (2008–2017). The Xbox 360 version of L4D uses a **slightly
modified version of Valve's VTF format**, called **VTFX**. (TheGeneral, 2008-12-19)

### VTFX File Header

TheGeneral's initial structure (files are **big-endian** — byte-swap before viewing):

```
struct VTFXFileHeader {
    int   fileSignature;   // 0x56544658 ("VTFX")
    int   version1;        // 0x00000360
    int   version2;        // 0x00000008
    int   headerSize;      // 0x00000044
    int   unknown;
    short width;
    short height;
    short unknown;  (x4)
    float reflectivityX;
    float reflectivityY;
    float reflectivityZ;
    float bumpMapScale;
    int   imageFormat;
    int   unknown;  (x3)
    int   headerSize2;     // 0x00000044
};
```

**Key header offsets** (Acewell, 2017-12-01):
- `0x0C` — `LZMA_OFF` (long, big-endian): offset where LZMA data begins
- `0x14` — width (2 bytes)
- `0x16` — height (2 bytes)

### Structure of the embedded image

- The image data inside is **LZMA-compressed** (cfarl, 2017-11-20).
- The decompressed payload is a **`.textures`** file containing a **DXT5**
  big-endian image (1024×512 example).
- **Mipmap order is inverted**: the *smallest* mip comes first, the *largest*
  (main) mip is **last** in the uncompressed data. (fatduck, 2009-02-04; Acewell, 2017-12-01)
- DXT5 data size = width × height. To extract the main mip, count back
  `width × height` bytes from the end of the decompressed file.

### LZMA payload header (at `LZMA_OFF`, little-endian)

| Offset | Type | Field |
|--------|------|-------|
| +0x00 | dstring(4) | `LZMA` id |
| +0x04 | long | `SIZE` (uncompressed size) |
| +0x08 | long | `ZSIZE` (compressed size) |
| +0x0C | dstring(5) | `LZMA_PROP` (5-byte LZMA properties) |

### QuickBMS extraction script (cfarl, 2017-12-04)

```bms
# script for QuickBMS http://aluigi.altervista.org/quickbms.htm

endian big
idstring "VTFX"

# Step 1. Extract lzma data from the vtfx file
goto 0xc
get LZMA_OFF long

goto 0x14
get IMG_WIDTH short
get IMG_HEIGHT short

goto LZMA_OFF
endian little
getdstring LZMA 4
get SIZE long
get ZSIZE long
getdstring LZMA_PROP 5

get NAME basename
string NAME + ".textures"
comtype lzma_dynamic LZMA_PROP
savepos OFFSET
clog NAME OFFSET ZSIZE SIZE

# Step 2. Extract dds data from end of .textures file
Open . NAME 1
get SIZE_TEXTURES_FILE asize 1
XMath IMG_SIZE "(IMG_WIDTH * IMG_HEIGHT)"
XMath OFFSET_IMG "(SIZE_TEXTURES_FILE - IMG_SIZE)"
get NAME basename
string NAME + ".texture"
log NAME OFFSET_IMG IMG_SIZE 1

# Step 3. Byte-swap (big-endian -> little-endian) the .texture data
Open . NAME 2
xmath NUM_SHORTS "IMG_SIZE / 2"
for i = 0 < NUM_SHORTS
   encryption swap 2
next i
get NAME basename
string NAME + "_swp.texture"
log NAME 0x0 IMG_SIZE 2

# Step 4. Prepend a DXT5 DDS header
Open . NAME 3
encryption "" ""
set MEMORY_FILE binary "\x44\x44\x53\x20\x7c\x00..."  # DDS "DDS " header
putVarChr MEMORY_FILE 0xc IMG_HEIGHT short
putVarChr MEMORY_FILE 0x10 IMG_WIDTH short
string NAME += ".dds"
get SIZE asize MEMORY_FILE
append
log NAME 0 SIZE MEMORY_FILE
log NAME 0 IMG_SIZE 3
```

### Practical workflow (cfarl, 2017-12)

1. Use the BMS script to extract the LZMA data → `.textures` file.
2. Take the last `width × height` bytes (largest mip) as the image data.
3. Byte-swap back to little-endian.
4. Prepend a DXT5 DDS header.
5. To re-import: recompress with your own LZMA (QuickBMS reimport is unreliable —
   compressed size differs). (Acewell, cfarl, 2017-12-03)
6. **Maintain the same image format, size, and mipmaps**, and byte-swap back to
   big-endian after editing. (Acewell, 2017-12-02)
7. cfarl's fix for the "english shadow" (stale mipmaps): zero out all bytes
   before the DDS image so leftover mip data doesn't display. (cfarl, 2017-12-03)

> **Relevance**: VTFX also applies to **Portal: Still Alive** on Xbox 360
> (cfarl extracted `howtoplay01.360.vtf` for localization).
> Reference: Valve VTF spec — http://developer.valvesoftware.com/wiki/VTF

## 3. Xbox 360: 360.zip Archives

**Topic 3258** (2008–2011). The Xbox 360 version of L4D stores all its files in
**`360.zip`** files. These cannot be extracted by normal zip tools even though
the format looks identical to a standard zip. (TheGeneral, 2008-12-19)

- **Tool**: `XZIP.rar` extractor (TheGeneral) — works on the `360.zip` files of
  *Left 4 Dead* and *Portal: Still Alive*.
- **Rebuilding**: rezip the files into an **uncompressed** zip archive and the
  game still loads everything. (TheGeneral, 2008-12-19)
- **Gotcha**: rebuilding must produce an uncompressed archive; compression
  breaks the injection (Frosty, 2009-06-15; adamharms, 2011-12-22).

## 4. Script/Data Encryption (Source SDK)

**Topic 7964** (2011-2012). For Steam versions, the **Source SDK** includes a
tool to encrypt/decrypt the `.dat`/`.txt` files. (qabRieL, 2012-01-02)

## Tools Reference

| Tool | Purpose | Author |
|------|---------|--------|
| Gibbed.Valve.ExtractPackage | VPK extraction (PC) | Rick (Gibbed) |
| XZIP | 360.zip extraction (Xbox 360) | TheGeneral |
| QuickBMS + VTFX script | VTFX LZMA → DDS extraction | cfarl / Acewell |
| Source SDK | .dat/.txt encrypt/decrypt | Valve |

## Key Facts

- PC L4D uses `.vpk` archives (not `.gcf`).
- Xbox 360 L4D uses `360.zip` archives and `VTFX` textures.
- VTFX = modified Valve VTF with LZMA-compressed payload, big-endian DXT5,
  inverted mip order (smallest first, largest last).
- VTFX header magic `0x56544658` ("VTFX"); image dims at `0x14`/`0x16`;
  LZMA offset pointer at `0x0C`.
