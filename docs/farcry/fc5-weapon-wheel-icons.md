---
title: "FC5 weapon wheel icon encoding (Base64/FEU)"
---

# FC5 weapon wheel icon encoding

FC5 weapon wheel icons are stored as Base64-encoded binary blobs in `.feu` (Flash
Export Unit) files. The binary structure is a simple packed format with character ID,
dimensions, and texture path.

## Binary format

```
Offset  Size  Type    Description
------  ----  ------  -----------
0x00    2     u16 LE  Character ID (weapon/character index)
0x02    2     u16 LE  Width (pixels)
0x04    2     u16 LE  Height (pixels)
0x06    var   string  Texture path (UTF-8, null-terminated)
```

The entire structure is then Base64-encoded for storage in the `.feu` XML.

## Encoding/decoding

The `base64 crap.exe` tool (C# / .NET 8) handles conversion:

```
1 — Decode Base64 → extract character ID, width, height, texture path
2 — Encode new entry → pack fields → output Base64 string
```

Source: `re/Ubisoft/Dunia/base64/Program.cs`

## FC5-specific notes

- FC5 FEU files require trailing `AAAA` padding (4+ bytes) after the icon data
- Sometimes 1-2 `=` padding characters are also needed
- Best results with 22-character filenames
- The FEU may reject icons — use Flash decompiler as fallback for SWF-based FEUs
- Icon dimensions should match in-game display for 1:1 scaling

## Hash format

The icon data uses standard Base64 encoding. The binary payload is little-endian
packed: u16(char_id) + u16(width) + u16(height) + null-terminated UTF-8 string.

## Source

BIRDdude12, Discord Far Cry modding community, Feb 2026.
