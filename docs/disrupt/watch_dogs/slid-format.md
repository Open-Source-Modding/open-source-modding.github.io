# Watch Dogs — SLID (Sound ID Link) Format

> `.slid` files map sound events to sound bank packs. Live in `soundbinary/` inside
> `patch.dat`/`patch.fat` archives. Reverse-engineered from
> `soundidlinelinks.slid` (WD1).

## Purpose

Given a sound ID (hash of an event/object name), the SLID table tells which `.spk`
sound bank pack contains the audio. It is a flat lookup table — no strings, no
hierarchy, just hashes.

## Layout

```
Offset  Size  Type          Description
0x00    4     uint32 LE     Entry count (N)
0x04    8×N   (u32,u32) LE  N pairs: (SoundID, LinkID)
```

**No magic number.** File size is always `4 + N*8`.

## Example: `soundidlinelinks.slid` (WD1)

- **Size**: 108,652 bytes
- **Header**: `0d 35 00 00` → `N = 13,581`
- **Check**: `4 + 13581×8 = 108,652` ✓

### Columns

| Column | Meaning | Values |
|--------|---------|--------|
| A (SoundID) | CRC32 hash of sound event/object name | 13,581 unique, range `0x18609`–`0x145620` |
| B (LinkID) | CRC32 hash of sound bank file | 13,581 unique, range `0x1CF3`–`0x39196` |

295 of the 13,581 LinkID values matched `.spk` filenames in `soundbinary/`
(e.g. `0x12937` → `12937.spk`). The rest reference banks absent from that extraction.

## CRC Hashes

Both columns use the same CRC32 flavor as the Disrupt `hash_tool.py`
(`WDL/hash_tool.py` — Disrupt CRC32 / CRC64_WD2). To resolve a SoundID, hash candidate
event names and match against column A; the paired LinkID then locates the `.spk`.

## Companion text list (snd.txt / Eng_snd_Fileslist_Loc.csv) — CONFIRMS columns

The WD1 `soundbinary/` list also ships as human-readable text. Two equivalent files
exist (identical 13,579 unique entries):

- `snd.txt` — plain text, lines `[strid] [sndid] [dialogue text]`
- `Eng_snd_Fileslist_Loc.csv` — CSV with header `String ID,SPK ID,Dialogue`

Both are **two formats of the same 13,579-entry list** (verified: the `sndid` and
`SPK ID` columns are exactly equal sets). Note `snd.txt` has 13,791 raw lines — some
entries' dialogue text wraps onto a continuation line with no `strid sndid` prefix.

### Cross-reference against the binary SLID

| Check | Result |
|-------|--------|
| slid Col A (SoundID) ∩ snd.txt/CSV sound ID | **13,579 of 13,581** |
| slid Col A values with NO text line | 2 only (`0xe2722`, `0xbeb9d`) — sound events with no dialogue string |
| text sound IDs NOT in slid Col A | **0** |
| slid Col B (LinkID) ∩ text sound ID | 104 (coincidental) |

**Conclusion (confirmed):** slid **Col A (SoundID) is the sound-event hash that
resolves to a dialogue string** — the same value as `snd.txt`'s `sndid` column and the
CSV's `SPK ID` column. slid **Col B (LinkID) is the `.spk` sound-bank pack hash** (the
audio container). So the pipeline is: `String ID + SoundID → dialogue text` (text
list), and `SoundID → LinkID → .spk` (binary SLID).

> ⚠️ The community CSV labels column 2 "SPK ID", but that value equals slid Col A
> (the sound event ID), *not* the `.spk` pack. The `.spk` pack hash is slid Col B.
> The two were conflated in the community filelist; the binary SLID disambiguates them.

## Notes

- Multiple SoundIDs can share a LinkID (many events → one bank); within this sample all
  pairs were unique but the format does not require it.
- The `.spk` files themselves are the audio packs referenced by the game's sound system
  (SND_fn_* symbols in the Wii U build — see [Wii U Debug Symbols](wii-u-debug-symbols.md)).