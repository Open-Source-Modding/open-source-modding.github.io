# Trackmania / Maniaplanet — XeNTaX Forum Knowledge

Extracted from XeNTaX forum discussions (2013–2023). Covers the **GBX file
format**, Nadeo's proprietary **Pak (NadeoPak) archive** format, and community
tools/scripts for extracting 3D models and audio from Trackmania, TrackMania 2
and TrackMania Turbo (Maniaplanet).

---

## 1. GBX File Format (Trackmania Wiki + forum)

**GBX** is the proprietary format used by Nadeo games (Trackmania,
Maniaplanet). *(Trackmania Wiki; forum threads t=11078, t=24378)*

### Format characteristics

- **Binary** format; **text variants** exist in older games (e.g. the TM
  Nations ESWC game config). *(Trackmania Wiki)*
- Content is **serialized objects** with an additional header describing the
  **main object type**. *(Trackmania Wiki)*
- The file magic is the ASCII string **`GBX`** (3 bytes) at offset 0.
  *(shakotay2's patched Noesis script, t=24378)*
- Extension is sometimes **lowercase** (`.gbx`). *(Trackmania Wiki)*

### Double-extension naming

| Extension | Contents |
|-----------|----------|
| `.Map.Gbx` | Map file |
| `.Replay.Gbx` | Replay |
| `.Pack.Gbx` | Title / content pack (the Pak format) |

*(Trackmania Wiki)*

## 2. Pak Archives (NadeoPak)

### NadeoPak header

TrackMania Turbo (Maniaplanet4, 2016) uses Nadeo's proprietary **Pak**
format; the header identifies it as a **`NadeoPak r18`** container. *(WRS,
2016-11-15, t=14143)*

### Protection / encryption

- As of 2016, there were **no working extractors for Maniaplanet Paks** because
  the archives are **protected by a key — apparently Blowfish encryption**.
  *(Matsilagi, 2016-11-18, t=14143)*
- A Maniaplanet forum tool (version 1.57) was expected to support `r18` Paks,
  but it only **read the headers and showed no data**. *(WRS, 2016-11-15,
  t=14143)*
- A Pastebin link with further analysis was shared but **expired/removed** by
  2021. *(Matsilagi 2016-11-18; fajNYgosciu1234, 2021-08-21, t=14143)*

### Known PAK specification

The PAK specification was documented on the community wiki
(`en.tm-wiki.org/wiki/PAK`), but no working extractor was found at the time.
*(friuns, 2013-12-25, t=11078)*

## 3. Extracting 3D Level Parts

### Whole-level ripping with dxripper

**dxripper** could extract a Trackmania 2 level as **one single mesh**, but it
could not produce the **individual level parts with correct file names** that
modders needed. *(friuns, 2013-12-25, t=11078)*

### GBX → OBJ conversion

After unpacking, level parts appear as **.gbx files**; converting them to OBJ
was a repeated request — via a Noesis script (see §4) or a Blender script.
*(friuns, 2013-12-26, t=11078; djibsone2, 2023-04-09, t=24378)*

## 4. Noesis GBX Plugin / Script (t=24378, 2021–2023)

### Origin

The Noesis plugin for `.gbx` came from the **VG Resource forums**
(`vg-resource.com/thread-37380.html`), written by **Tuliopilloto**, and was
tested only against a single sample file. *(fajNYgosciu1234, shakotay2,
2022-11-23)*

### Known limitations

| Limitation | Detail |
|------------|--------|
| **ASCII GBX unsupported** | The plugin fails on ASCII-variant `.gbx` files. *(fajNYgosciu1234, 2021-12-22)* |
| **Decompressed GBX partial support** | Cannot read some/most cars after GBX decompression; errors like `uv size`. *(fajNYgosciu1234, 2022-11-23, 2023-05-20)* |
| **Single sub-mesh only** | Shakotay2's patched script loads **one sub mesh**; fails on `MainbodyHigh.Solid.Decompressed.Gbx`. *(shakotay2, 2022-11-23)* |
| **Clumped mesh problem** | All vertices appear at the same location — vertices need transformation with **bone transformation info**. *(fajNYgosciu1234, shakotay2, 2022-01-17)* |
| **Mesh count** | Mesh-amount problem was fixed by the user, but remaining issues persisted. *(fajNYgosciu1234, 2021-08-29)* |

### Mesh layout hints (from shakotay2's patched script)

The GBX mesh format "looks simple" *(shakotay2, 2022-01-17)*. Structural
offsets observed in the Noesis script:

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

*(shakotay2, 2022-11-23, t=24378)*

> **Note:** Fails on `MainbodyHigh.Solid.Decompressed.Gbx`; no working sample
> accompanied the script, which limited further debugging. *(shakotay2,
> 2022-11-23)*

### Textures in mappack samples

Some samples extracted from mappacks contain **no embedded textures** —
textures are stored separately, "like Table Top Racing stores it in
`.amodel`". *(fajNYgosciu1234, 2023-05-20, t=24378)*

## 5. Audio Ripping (TrackMania Turbo)

TrackMania Turbo's **car sounds** are packed inside the proprietary Pak format
(rather than ZIP); unpacking was requested to rip them, but the Blowfish
protection (see §2) blocked extraction as of 2016. *(Matsilagi, 2016-03-25 to
2016-11-18, t=14143)*

## 6. Tools Mentioned

| Tool | Purpose | Notes |
|------|---------|-------|
| dxripper | Rips whole level as a single mesh | No per-part filenames |
| Noesis + Tuliopilloto's `.gbx` plugin | GBX model import → OBJ | From vg-resource.com thread-37380; ASCII GBX unsupported; partial decompressed-GBX support |
| Maniaplanet forum tool (v1.57) | Pak extraction | Reads headers only; no data shown on r18 |
| en.tm-wiki.org/wiki/PAK | PAK spec documentation | Wiki spec only; no working extractor found |

---

*Source: XeNTaX forum threads t=11078 ([Request] Trackmania 2), t=14143
(TrackMania Turbo Maniaplanet4 PAK), t=24378 (trackmania noesis problem);
GBX format knowledge from the Trackmania Wiki.*