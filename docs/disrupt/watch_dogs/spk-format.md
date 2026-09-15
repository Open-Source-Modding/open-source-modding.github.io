# WD1 SPK Sound Package Format

The `.spk` format is Ubisoft's Disrupt engine wrapper for Wwise audio. It packages
a header (Wwise PlayEvent/Resource descriptors + audio metadata) with raw audio
data (PCM or OGG Vorbis). Used by Watch Dogs 1 for all in-game sounds.

## Structure

SPK = header (template-based, ~0x368 bytes) + body (raw audio data).

### Header Layout

| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| +0x00 | 4 | Magic | Template-dependent (not validated) |
| +0x08 | 4 | PlayEventDescriptor[0:4] | MD5 hash bytes 0-3 (little-endian) |
| +0x0C | 4 | ResourceDescriptor[0:4] | MD5 hash bytes 4-7 |
| +0x10 | 4 | sndData[0:4] | MD5 hash bytes 8-11 |
| +0x38 | 4 | PlayEventDescriptor[4:8] | MD5 hash bytes 12-15 |
| +0x68 | 4 | delayTime | f32 — delay before playback (Song = 1.2s) |
| +0x84 | 4 | ResourceDescriptor[4:8] | MD5 hash bytes 16-19 |
| +0xC0 | 4 | ResourceDescriptor[8:12] | MD5 hash bytes 20-23 |
| +0xC8 | 4 | m_volume_dB | f32 — volume offset (Song = -12 dB) |
| +0x104 | 4 | ulLoopByte | u32 — loop point (commented out in tool) |
| +0x114 | 1 | CompressionFormat | u8 — 1=PCM, 4=OGG Vorbis |
| +0x118 | 4 | ulNbChannels | u32 — channel count |
| +0x11C | 4 | ulFreq | u32 — sample rate (Hz) |
| +0x124 | 4 | ulNbBytes | u32 — audio data size in bytes |
| +0x134 | 4 | sndData[4:8] | MD5 hash bytes 24-27 (copy) |
| +0x144 | 4 | busId | u32 — Wwise bus ID (see table below) |
| +0x158 | 4 | ulAttackLengthByte | u32 — attack length (commented out in tool) |
| +0x164 | 4 | sndDataTotalSize | u32 — audio data size + 28 |

### Wwise Bus IDs

| Bus | ID | Volume | Delay |
|-----|----|--------|-------|
| SFX | `0x0004C4C9` | 0 dB | 0s |
| Voice | `0x00020ECD` | 0 dB | 0s |
| Song | `0x00127698` | -12 dB | 1.2s |
| Music | `0x0001825A` | 0 dB | 0s |

### Audio Format

| Format | Value | Description |
|--------|-------|-------------|
| PCM | 1 | Uncompressed WAV (RIFF header stripped) |
| OGG Vorbis | 4 | Compressed — tool converts non-WAV/non-OGG to OGG via ffmpeg |

## File Naming

SPK files are named by the first 8 characters of the MD5 hash of the original
audio filename (before the extension). Example: `000e9bd7.spk`.

If the input filename is already a valid 8-character hex sound ID (e.g., `000b1d2c.mp3`),
the tool prepends the MD5 to it. Otherwise, a new ID is generated.

## Audio Bus Assignment

The bus determines how the engine routes the sound:
- **SFX** (`0x0004C4C9`): Sound effects, UI sounds
- **Voice** (`0x00020ECD`): NPC dialogue, narration
- **Song** (`0x00127698`): In-game radio music (CR-FM, etc.) — gets volume/delay
- **Music** (`0x0001825A`): Background/ambient music

## Integration with Disrupt

SPK files go into the `soundbinary/` folder in the game's `patch/` archive.
The engine references them by the 8-char hex ID (CRC32-based hash of the path).

### Music Library XML

Custom songs also require entries in:
- `patch/generated/databases/generic/musiclibrary_*.obj.xml` — song metadata
- `patch/music/music.xml` — music library definitions
- `patch/worlds/windy_city/generated/windy_city_depload.dat` — sound references
- `patch/languages/patch1_english.loc` — song names/descriptions

## Tools

| Tool | What | Where |
|------|------|-------|
| CONVERT2SPK.bat | Audio → SPK converter (The_Silver v0.6) | `hV_WD_ModdingKit_PLUS/Tools/CONVERT2SPK/` |
| hV_WD1_AddCustomSongs | GUI for adding songs to WD1 radio | `hV_WD_ModdingKit_PLUS/Tools/hV_WD1_AddCustomSongs/` |
| FusionTools 2.0 | Wwise PCK/BNK/AESP editor (multi-game) | [Nexus Mods](https://www.nexusmods.com/site/mods/1812) |

## Known Issues

- **Loading screen music**: Now possible (beta FP loading music mod, Aug 2026). Was "supposedly not possible before."
- **FusionTools bulk editing**: One-by-one injection only. Bad for large projects (15+ sounds).
- **Song genres**: WD2 leftover genres may be from WD1 (community speculation).
- **.wem decoding**: Full WDL/WD2 .wem decoding still open on our end. FusionTools handles WD2 but tediously.

## Source

RE'd from `CONVERT2SPK.bat` (The_Silver, v0.6) and `hV_WD1_AddCustomSongs.exe`
(.NET 4.8 WinForms, decompiled via monodis). Discord: □ΞnCrypTΞD□, Cobra, The Silver.
