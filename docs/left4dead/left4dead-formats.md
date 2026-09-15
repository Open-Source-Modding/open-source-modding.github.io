# Left 4 Dead — Archive & Texture Formats

> **Source**: XeNTaX forum knowledge extraction (2008–2017).
> Covers Left 4 Dead PC (Source engine) and Xbox 360 (L4D360).
> Also applies to **Portal: Still Alive** (Xbox 360).

---

## 1. VPK Archives (PC — Source Engine)

With the L4D demo release, Valve shipped game files not in the standard `.gcf` format used by other Source games, but in `.vpk` files. These VPKs differ from the earlier `.vpk` used in *Vampire: The Masquerade — Bloodlines*.

### Tools
| Tool | Purpose | Notes |
|------|---------|-------|
| `Gibbed.Valve.ExtractPackage` | VPK extraction | Rick (Gibbed); requires **.NET Framework 2.0**; fails silently on XP unless .NET 2.0 installed |

---

## 2. Xbox 360: VTFX Textures

Xbox 360 version uses a **slightly modified Valve VTF format** called **VTFX**. Files are **big-endian** — byte-swap before viewing.

### VTFX File Header
```c
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

### Key Header Offsets
| Offset | Type | Field |
|--------|------|-------|
| `0x0C` | long (BE) | `LZMA_OFF` — offset where LZMA data begins |
| `0x14` | short (BE) | width |
| `0x16` | short (BE) | height |

### Embedded Image Structure
- Image data is **LZMA-compressed** (cfarl, 2017)
- Decompressed payload = **`.textures`** file containing **DXT5 big-endian** image
- **Mipmap order is inverted**: smallest mip first, largest (main) mip **last**
- DXT5 data size = `width × height`. Extract main mip by counting back `width × height` bytes from end of decompressed file.

### LZMA Payload Header (at `LZMA_OFF`, little-endian)
| Offset | Type | Field |
|--------|------|-------|
| +0x00 | dstring(4) | `LZMA` id |
| +0x04 | long | `SIZE` (uncompressed size) |
| +0x08 | long | `ZSIZE` (compressed size) |
| +0x0C | dstring(5) | `LZMA_PROP` (5-byte LZMA properties) |

### QuickBMS Extraction Script (cfarl / Acewell, 2017)
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

### Practical Workflow (cfarl / Acewell, 2017)
1. Use BMS script to extract LZMA data → `.textures` file
2. Take last `width × height` bytes (largest mip) as image data
3. Byte-swap back to little-endian
4. Prepend DXT5 DDS header
5. **To re-import**: Recompress with own LZMA (QuickBMS reimport unreliable — compressed size differs)
6. Maintain same image format, size, mipmaps; byte-swap back to big-endian after editing
7. **Fix for "english shadow" (stale mipmaps)**: Zero out all bytes before DDS image so leftover mip data doesn't display

---

## 3. Xbox 360: 360.zip Archives

Xbox 360 version stores all files in **`360.zip`** files. These cannot be extracted by normal zip tools despite looking identical to standard zip.

### Extraction
- **Tool**: `XZIP.rar` extractor (TheGeneral) — works on L4D and *Portal: Still Alive* `360.zip` files

### Rebuilding
- Rezip files into an **uncompressed** zip archive — game still loads everything
- **Gotcha**: Must produce uncompressed archive; compression breaks injection

---

## 4. Script/Data Encryption (Source SDK)

For Steam versions, the **Source SDK** includes a tool to encrypt/decrypt `.dat`/`.txt` files.

---

## Tools Reference

| Tool | Purpose | Author |
|------|---------|--------|
| Gibbed.Valve.ExtractPackage | VPK extraction (PC) | Rick (Gibbed) |
| XZIP | 360.zip extraction (Xbox 360) | TheGeneral |
| QuickBMS + VTFX script | VTFX LZMA → DDS extraction | cfarl / Acewell |
| Source SDK | .dat/.txt encrypt/decrypt | Valve |

---

## Key Facts Summary
- PC L4D uses `.vpk` archives (not `.gcf`)
- Xbox 360 L4D uses `360.zip` archives and `VTFX` textures
- VTFX = modified Valve VTF with LZMA-compressed payload, big-endian DXT5, inverted mip order (smallest first, largest last)
- VTFX header magic: `0x56544658` ("VTFX"); image dims at `0x14`/`0x16`; LZMA offset pointer at `0x0C`
- VTFX also applies to **Portal: Still Alive** on Xbox 360