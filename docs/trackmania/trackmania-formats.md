# Trackmania / Maniaplanet — GBX, Pak & Model Formats

> **Source**: XeNTaX forum knowledge extraction (2013–2023).
> Covers GBX file format, NadeoPak archives, model/audio extraction.

---

## 1. GBX File Format

GBX is the proprietary format used by Nadeo games (Trackmania, Maniaplanet).

### Format Characteristics
- **Binary** format; **text variants** exist in older games (e.g., TM Nations ESWC game config)
- Content is **serialized objects** with an additional header describing the **main object type**
- File magic: ASCII string **`GBX`** (3 bytes) at offset 0
- Extension sometimes **lowercase** (`.gbx`)

### Double-Extension Naming
| Extension | Contents |
|-----------|----------|
| `.Map.Gbx` | Map file |
| `.Replay.Gbx` | Replay |
| `.Pack.Gbx` | Title / content pack (the Pak format) |

---

## 2. Pak Archives (NadeoPak)

### Header
TrackMania Turbo (Maniaplanet4, 2016) uses Nadeo's proprietary **Pak** format; header identifies it as **`NadeoPak r18`** container.

### Protection / Encryption
- As of 2016: **no working extractors** for Maniaplanet Paks — archives are **protected by a key, apparently Blowfish encryption**
- Maniaplanet forum tool (v1.57) expected to support `r18` Paks, but only **reads headers and shows no data**
- PAK specification documented on community wiki (`en.tm-wiki.org/wiki/PAK`), but no working extractor found

---

## 3. Extracting 3D Level Parts

### Whole-Level Ripping
- **dxripper** extracts a Trackmania 2 level as **one single mesh**, but cannot produce individual level parts with correct file names

### GBX → OBJ Conversion
- After unpacking, level parts appear as **.gbx files**; converting to OBJ was requested repeatedly
- Via Noesis script or Blender script

---

## 4. Noesis GBX Plugin / Script (t=24378, 2021–2023)

### Origin
Noesis plugin for `.gbx` from **VG Resource forums** (`vg-resource.com/thread-37380.html`), written by **Tuliopilloto**, tested only against a single sample file.

### Known Limitations
| Limitation | Detail |
|------------|--------|
| **ASCII GBX unsupported** | Plugin fails on ASCII-variant `.gbx` files |
| **Decompressed GBX partial support** | Cannot read some/most cars after GBX decompression; errors like `uv size` |
| **Single sub-mesh only** | Shakotay2's patched script loads **one sub mesh**; fails on `MainbodyHigh.Solid.Decompressed.Gbx` |
| **Clumped mesh problem** | All vertices appear at same location — vertices need transformation with **bone transformation info** |
| **Mesh count** | Mesh-amount problem fixed by user, but remaining issues persisted |

### Mesh Layout Hints (from shakotay2's patched script)
```
header: "GBX" magic (3 bytes)
seek +0x89 (relative) -> NumMeshes (uint32)
per mesh:
  pattern search \x0E\x60\x00\x09\x38\x00\x00 -> mesh block
  MaterialName: 8 bytes ASCII
  VCount:  uint32
  UVs:     VCount * 8 bytes (float2, stride 8)
  Vertex buffer: FVF size 40 bytes per vertex (position float3 + ...)
  FCount:  uint32
  Indices: FCount * 2 bytes (uint16, triangles)
```

> **Note:** Fails on `MainbodyHigh.Solid.Decompressed.Gbx`; no working sample accompanied the script, limiting further debugging.

### Textures
Some samples extracted from mappacks contain **no embedded textures** — textures stored separately, "like Table Top Racing stores it in `.amodel`".

---

## 5. Audio Ripping (TrackMania Turbo)
TrackMania Turbo's **car sounds** packed inside proprietary Pak format (not ZIP); unpacking blocked by Blowfish protection as of 2016.

---

## 6. Tools Reference

| Tool | Purpose | Notes |
|------|---------|-------|
| dxripper | Rips whole level as single mesh | No per-part filenames |
| Noesis + Tuliopilloto's `.gbx` plugin | GBX model import → OBJ | From vg-resource.com thread-37380; ASCII GBX unsupported; partial decompressed-GBX support |
| Maniaplanet forum tool (v1.57) | Pak extraction | Reads headers only; no data shown on r18 |
| en.tm-wiki.org/wiki/PAK | PAK spec documentation | Wiki spec only; no working extractor found |

---

## Key Facts
- **GBX magic**: `GBX` (3 bytes at offset 0)
- **Pak format**: `NadeoPak r18`, Blowfish encrypted
- **No working Pak extractor** as of 2023
- **GBX mesh structure**: ~40 bytes/vertex (FVF), uint16 indices, 8-byte material name
- **Textures**: Stored separately (not in GBX)
- **Audio**: Inside encrypted Pak