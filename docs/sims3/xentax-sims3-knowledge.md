# The Sims 3 — XeNTaX Forum Knowledge

Extracted and organized from XeNTaX forum discussions (2009–2012).
Covers The Sims 3 package (DBPF v2) format, audio, and scripting.

---

## 1. Package Format — DBPF v2

**Topic 3491** (2009). The Sims 3 package files are **DBPF v2** files, a format
similar to those used in *Spore* and already well understood by the community.
(JMPescado, 2009-05-18)

- Packages use **standard zlib compression** for most content.
- One notable exception: the **`scripts.package`** file did not use standard
  zlib — its contents appeared compressed/encrypted with practically no
  repeating bytecode/text strings.
- All such files shared the same ~5-byte starting header.
- The scripts were suspected to be **LUA** (hinted at in the executables),
  compiled or uncompiled, or another homebrew bytecode similar to TS1/TS2.
  (JMPescado, 2009-05-18) — later resolved by the community.

**Tool** (PC): **S3PE** — used to open/edit PC package files. (brendan19, 2010-11-06)

## 2. FullBuild1.package — Music/Audio (PC vs Xbox 360)

**Topics 5349 / 5357** (2010). On PC, the **`FullBuild1.package`** file stores
the music. On **Xbox 360**, the same file exists but could not be opened with
the PC tool (S3PE).

- Likely a **big-endian** variant of the format (the 360 runs on big-endian
  hardware). (huckleberrypie, 2010-11-09)
- Hex Workshop cannot trivially convert endianness for a whole file — a custom
  program is required. (AlphaTwentyThree, 2010-11-17)
- The 360 audio files are **EAlayer3** encoded. (AlphaTwentyThree, 2010-11-17)

## 3. Audio Streams — audiostr.arc / FSB

**Topic 5637** (2010-12-26). `audiostr.arc` — U8Tool failed on this `.ARC`.
(brendan19)

**Topic 5638** (2010-12-26). `FSBext` reported only one `.WAV`; `VGMstream` /
`in_cube` played them (after an initial hiccup).

- FSBext reported: **Stereo, hw2d, GC_ADPCM, 44100Hz, 2 Channel, 16 Bits**.
- Output `.genh` files have no WAV header; VGMstream can play them.

> Note: SimCity-era games commonly use EAlayer3; Sims 3 Xbox 360 music is
> EAlayer3 while the container is DBPF v2.

## 4. Sample Packages

**Topic 9431** (2012). Common package files found in The Sims 3:
- `JazzData.package`
- `scripts.package`
- `ZoneObjects_Pet_World_9.package` (Pets expansion world objects)

## Tools Reference

| Tool | Purpose | Author |
|------|---------|--------|
| S3PE | PC package editor (DBPF v2) | Community |
| FSBext | FSB/audio stream extraction | Community |
| VGMstream | Play extracted audio streams | Community |
| U8Tool | U8 archive tool (did NOT work on audiostr.arc) | Community |

## Key Facts

- Sims 3 uses **DBPF v2** packages (like Spore).
- Most content is **zlib**-compressed; `scripts.package` was the odd one out.
- Xbox 360 packages are big-endian variants; PC tool (S3PE) can't read them.
- Xbox 360 music is **EAlayer3** encoded inside `FullBuild1.package`.
- `audiostr.arc` contains FSB streams (GC_ADPCM, 44100Hz).
