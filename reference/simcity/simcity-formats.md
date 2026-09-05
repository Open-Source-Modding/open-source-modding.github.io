# SimCity (2013) — Audio Format

> **Source**: XeNTaX forum knowledge extraction (2014).
> Covers SimCity 5 (2013) audio: RIFF Vorbis `.wav` files and conversion pipeline.

---

## 1. SimCity 5 Audio: RIFF Vorbis `.wav` Files

### Overview
`.wav` files extracted from SimCity 5 are **unplayable** as standard WAV and import as "only noise" in raw-data editors. They are not PCM WAV at all — they are **RIFF Vorbis** containers: a RIFF wrapper around a Vorbis (OGG) stream.

### Conversion Workflow
Use **hcs64's `ww2ogg`** tool to convert RIFF Vorbis `.wav` → standard OGG:

```bash
ww2ogg.exe SCP_0x0d9e5710-0x40490000-0x0b67f070.wav \
  --pcb packed_codebooks_aoTuV_603.bin
```

- `--pcb` points to the **packed codebooks** file (`packed_codebooks_aoTuV_603.bin`) required by ww2ogg to decode the Vorbis streams
- Output is a standard `.ogg` file

### Seeking Fix: `revorb`
Optionally run **`revorb`** (also from hcs64's site) on the resulting OGG to fix **seeking** in the sound file:

```bash
revorb.exe SCP_0x0d9e5710-0x40490000-0x0b67f070.ogg \
  new_SCP_0x0d9e5710-0x40490000-0x0b67f070.ogg
```

### Filename Pattern
Extracted audio uses structured names:
`SCP_<hash1>-<hash2>-<hash3>.wav` — components are game-internal resource hashes, not meaningful filenames.

---

## 2. Tools

| Tool | Purpose |
|------|---------|
| `ww2ogg` (hcs64) | RIFF Vorbis `.wav` → standard OGG converter |
| `revorb` (hcs64) | Fixes seeking in converted OGG files |
| `packed_codebooks_aoTuV_603.bin` | Codebook pack required by ww2ogg |

---

## Key Facts
- SimCity 5 `.wav` files = **RIFF Vorbis** (not PCM)
- Convert with `ww2ogg` + `packed_codebooks_aoTuV_603.bin`
- Fix seeking with `revorb`
- Filenames = internal resource hashes (`SCP_<hash>-<hash>-<hash>.wav`)