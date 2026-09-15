# The Sims 3 — Package, Audio & Script Formats

> **Source**: XeNTaX forum knowledge extraction (2009–2012).
> Covers DBPF v2 package format, audio streams, and scripting.

---

## 1. Package Format — DBPF v2

The Sims 3 package files are **DBPF v2** files, similar to those used in *Spore* and well understood by the community.

### Key Characteristics
- **Standard zlib compression** for most content
- Exception: **`scripts.package`** did not use standard zlib — contents appeared compressed/encrypted with no repeating bytecode/text strings
- All such files shared the same ~5-byte starting header
- Scripts suspected to be **LUA** (hinted in executables), compiled or uncompiled, or homebrew bytecode similar to TS1/TS2 — later resolved by community

### Common Package Files
| Package | Description |
|---------|-------------|
| `JazzData.package` | Jazz script data |
| `scripts.package` | Core scripts (non-standard compression) |
| `ZoneObjects_Pet_World_9.package` | Pets expansion world objects |

### Tools
| Tool | Purpose | Notes |
|------|---------|-------|
| **S3PE** | PC package editor (DBPF v2) | Community tool |

---

## 2. FullBuild1.package — Music/Audio (PC vs Xbox 360)

On PC, **`FullBuild1.package`** stores the music. On **Xbox 360**, the same file exists but could not be opened with the PC tool (S3PE).

### Platform Differences
- Xbox 360 version = **big-endian** variant (360 runs on big-endian hardware)
- Hex Workshop cannot trivially convert endianness for whole file — custom program required
- 360 audio files are **EAlayer3** encoded

---

## 3. Audio Streams — audiostr.arc / FSB

### audiostr.arc
- U8Tool **failed** on this `.ARC` file
- Contains FSB streams

### FSB Streams
- `FSBext` reported only one `.WAV`; `VGMstream` / `in_cube` played them (after initial hiccup)
- **Format**: Stereo, hw2d, **GC_ADPCM**, 44100Hz, 2 Channel, 16 Bits
- Output `.genh` files have no WAV header; VGMstream can play them
- Note: SimCity-era games commonly use EAlayer3; Sims 3 Xbox 360 music is EAlayer3 while container is DBPF v2

### Audio Tools
| Tool | Purpose | Notes |
|------|---------|-------|
| FSBext | FSB/audio stream extraction | Community |
| VGMstream | Play extracted audio streams | Community |
| U8Tool | U8 archive tool | **Did NOT work** on audiostr.arc |

---

## 4. Platform Summary

| Aspect | PC | Xbox 360 |
|--------|-----|----------|
| Package Format | DBPF v2 (little-endian) | DBPF v2 (big-endian) |
| Compression | zlib (standard) | zlib (standard) |
| scripts.package | Non-standard (encrypted?) | Non-standard |
| Music Container | FullBuild1.package | FullBuild1.package |
| Music Codec | Various | EAlayer3 |
| Audio Streams | FSB (GC_ADPCM) | FSB (GC_ADPCM) / EAlayer3 |

---

## 5. Tools Reference Summary

| Tool | Purpose | Author |
|------|---------|--------|
| S3PE | PC package editor (DBPF v2) | Community |
| FSBext | FSB/audio stream extraction | Community |
| VGMstream | Play extracted audio streams | Community |
| U8Tool | U8 archive tool (failed on audiostr.arc) | Community |

---

## Key Facts
- Sims 3 uses **DBPF v2** packages (like Spore)
- Most content is **zlib**-compressed; `scripts.package` is the odd one out
- Xbox 360 packages are **big-endian** variants; PC tool (S3PE) can't read them
- Xbox 360 music is **EAlayer3** encoded inside `FullBuild1.package`
- `audiostr.arc` contains FSB streams (GC_ADPCM, 44100Hz)