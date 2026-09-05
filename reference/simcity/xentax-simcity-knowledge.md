# SimCity (2013) — XeNTaX Forum Knowledge

Extracted from XeNTaX forum discussions (2014). Covers the **SimCity 5 (2013)**
audio format: `.wav` files that are actually **RIFF Vorbis** containers, and the
conversion pipeline to standard OGG.

---

## 1. SimCity 5 Audio: RIFF Vorbis `.wav` files

### Overview

`.wav` files extracted from SimCity 5 are **unplayable** as standard WAV and
import as "only noise" in raw-data editors. They are not PCM WAV at all — they
are **RIFF Vorbis** containers: a RIFF wrapper around a Vorbis (OGG) stream.
*(Apollo, 2014-08-28: "RIFF Vorbis.")*

### Conversion workflow

Use **hcs64's `ww2ogg`** tool to convert RIFF Vorbis `.wav` → standard OGG.
*(merlinsvk, 2014-08-28)*

```bash
ww2ogg.exe SCP_0x0d9e5710-0x40490000-0x0b67f070.wav \
  --pcb packed_codebooks_aoTuV_603.bin
```

- `--pcb` points to the **packed codebooks** file (`packed_codebooks_aoTuV_603.bin`)
  required by ww2ogg to decode the Vorbis streams.
- The output is a standard `.ogg` file.

### Seeking fix: `revorb`

Optionally run **`revorb`** (also from hcs64's site) on the resulting OGG to
fix **seeking** in the sound file. *(merlinsvk, 2014-08-28)*

```bash
revorb.exe SCP_0x0d9e5710-0x40490000-0x0b67f070.ogg \
  new_SCP_0x0d9e5710-0x40490000-0x0b67f070.ogg
```

### Filename pattern

Extracted audio uses a structured name of the form
`SCP_<hash1>-<hash2>-<hash3>.wav` — the components are game-internal resource
hashes, not meaningful filenames. *(observed in the sample, 2014-08)*

---

## 2. Tools Mentioned

| Tool | Purpose |
|------|---------|
| `ww2ogg` (hcs64) | RIFF Vorbis `.wav` → standard OGG converter |
| `revorb` (hcs64) | Fixes seeking in converted OGG files |
| `packed_codebooks_aoTuV_603.bin` | Codebook pack required by ww2ogg |

---

*Source: XeNTaX forum thread t=11840 (SimCity 5 .wav).*
