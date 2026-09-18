---
title: MAB Animation Format
---

The `.mab` animation format used by the Dunia engine (Far Cry 2 through Far Cry 5 / New Dawn). All data is little-endian.

Sources: [buu342's RE thread](https://knockout.chat/thread/55079) (Nov 2023 – Jun 2024), [PY-DuniaAnimationExtractor wiki](https://github.com/buu342/PY-DuniaAnimationExtractor/wiki), [MabTools/FCBConverter](https://github.com/buu342/PY-DuniaAnimationExtractor) source code by ArmanIII. Method: Cheat Engine watchpoints on FC3.dll + Ghidra + FC2 cross-referencing.

## Why this matters

The community's long-standing blocker: *"you can't add animations"* / *"no one has researched .mab enough"*. MAB = animation, XBG = model, XBT = texture, HKX = Havok physics. FC3/BD share an engine; FC5/ND share one; FC6 is trickier; FC4/FC3 need a `move.bin` edit too.

## Version bytes

The first 4 bytes identify the game. This also determines the header size and overall layout.

| Value | Game | Header size |
|-------|------|-------------|
| `0x0000004C` (76) | Far Cry 2 | 16 bytes |
| `0x00000061` (97) | Far Cry 3 | 16 bytes |
| `0x00000062` (98) | Far Cry 3: Blood Dragon | 16 bytes |
| `0x00000081` (129) | Far Cry 4 | 16 bytes |
| `0x00000082` (130) | Far Cry Primal | 16 bytes |
| `0x000000B0` (176) | Far Cry 5 + New Dawn | 100 bytes |
| `0x000000B6` (182) | Far Cry 6 | 100 bytes |

FCBConverter detects FC5 vs FC6 by checking if the version equals `176` (0xB0). FC6 MABs use `0xB6` (182).

## FC3 header (16 bytes)

| Offset | Size | Description |
|--------|------|-------------|
| `0x00` | 4 bytes | Version |
| `0x04` | 4 bytes | CRC (algorithm unknown) |
| `0x08` | 8 bytes | Unknown |

## FC5/FC6 header (100 bytes)

Verified from FCBConverter source. The header extends the FC3 layout significantly:

| Offset | Size | Field | Notes |
|--------|------|-------|-------|
| `0x00` | 4 | Version | 0xB0 = FC5, 0xB6 = FC6 |
| `0x04` | 4 | Unknown | |
| `0x08` | 4 | Unknown | |
| `0x0C` | 4 | Unknown | |
| `0x10` | 20 | Hash[5] | 5 × uint32 |
| `0x24` | 4 | Animation file length | Total MAB size in bytes |
| `0x28` | 4 | Clip length | Float, seconds |
| `0x2C` | 4 | Frame rate | Float |
| `0x30` | 2 | Bone count | |
| `0x32` | 14 | Unknown[7] | 7 × uint16 |
| `0x40` | 44 | Unknown[11] | 11 × uint32; last entry is adjusted when repacking |
| `0x6C` | 4 | Zero | |
| `0x70` | 4 | Frame count | |
| `0x74` | 4 | Zero | |
| `0x78` | 4 | Section 9 offset | Points to anchor/bone data; 0 = absent |

## Header after the fixed fields (FC3)

All versions. This block sits at offset `0x10` for FC3, offset `0x78` for FC5/FC6.

## Section 0 — Section Information (FC3 only)

Starts at offset `0x10` (right after the 16-byte header). Always 224 bytes (220 + 4 bytes zero padding).

All offsets are relative to this section. Add 16 bytes for absolute file offsets.

### Animation length

At offset `0xB4` (relative), 4 bytes: a float — animation duration in seconds (`ANIMLENGTH`).

### Section offsets

Nine 4-byte integers — one per section. Offsets are **relative to Section 0**.

- Sections 1 and 2 are swapped (offset 0 → section 2, offset 1 → section 1). All others ascending.
- Offset 0 = section does not exist.

### Section sizes

Subtract consecutive offsets. For the last section, subtract its offset from the file size minus 16.

Final 4 bytes are zero padding.

## FC5/FC6 post-header layout

After the 100-byte header, FC5/FC6 MABs contain:

1. **Bone array** — `boneCount` × uint32 (bone IDs)
2. **Bone values** — `boneCount` × byte
3. **Padding** — align to 4 bytes
4. **Frame array** — `frameCount` × uint16 (defines playback order; 0,1,2,3 = non-culled; 0,4,9,16 = culled/skipped)
5. **Section data** — referenced by offsets in the header

## Section 3 — Root Rotation (FC3)

After the first 8 bytes, describes root bone rotation. Values are 4-byte floats in Y, Z, X order. Some animations store multiple rotations; many contain only one set (12 bytes total).

## Section 4 — Rotation Keyframes (FC3)

### Preamble

First 4 bytes: two 2-byte values (purpose unknown). Then a 4-byte "magic float".

### Offset table

After the first 8 bytes, a sequence of 4-byte offsets pointing to rotation data blocks.

Last offset formula:

```
((((int)(magicfloat * ANIMLENGTH)) >> 3) * 4) + 8
```

One more 4-byte value after the last offset stores the section size (excluding padding).

### Rotation data (packed quaternions)

FC3 quaternions are **48-bit compressed**: 1 unused bit + 2 bits (largest component) + 15 bits × 3 components. W reconstructed via square root — the `0.7071068` constant is `sin(45°)`, spotted by buu342 in FC2's lighter-optimized DLL.

**Component extraction:**

```
fVar1 = (FirstWord & 0x7FFFF) * 4.315969e-05 - 0.7071068
fVar2 = (SecondWord & 0x7FFFF) * 4.315969e-05 - 0.7071068
fVar3 = ThirdWord * 4.315969e-05 - 0.7071068
fVar4 = sqrt(1.0 - fVar1*fVar1 - fVar2*fVar2 - fVar3*fVar3)
```

**Component order** (sign bit of first two packed values):

```
if ((FirstWord & 0x8000) == 0)
{
    if ((SecondWord & 0x8000) != 0)
        return Quaternion(fVar1, fVar2, fVar4, fVar3);
}
else if ((SecondWord & 0x8000) != 0)
    return Quaternion(fVar1, fVar2, fVar3, fVar4);

if ((FirstWord & 0x8000) != 0)
    return Quaternion(fVar1, fVar4, fVar2, fVar3);
return Quaternion(fVar4, fVar1, fVar2, fVar3);
```

where `Quaternion` is `(x, y, z, w)`.

### Keyframe mystery byte

Each keyframe section starts with the first quaternion + 2 mystery bytes, then subsequent quaternions sequentially.

The mystery byte is a **bitmask counting quaternions**: popcount = quaternions packed after the first. `0x47` (01000111) = 4 ones = 4 quats. First nibble = transition speed; second nibble = animation breaking. Second mystery byte possibly a bone selector.

Open: why two bit representations for same count; >8 quat behavior (8-bit mask limit); multi-bone mapping (bone-to-offset table likely built at load from XBG, not in MAB).

## Section 5 — Unknown

No documentation yet.

## Section 6 — Root Offset Keyframes (FC3)

Root bone offset keyframes. Under-investigated.

## Section 7 — Animation Events (FC3)

Strings describing animation events (e.g., particle effects) and likely timestamps. Under-investigated.

## Section 8 — Unknown

No documentation yet.

## Section 9 — Unknown / Anchor Data

In FC5/FC6, the offset at header `0x78` points to an anchor/bone data structure. FCBConverter reads it as:

- uint32 `anchorCount`
- uint32 zero
- uint32 `sectionSize`

Then per anchor:

| Size | Field | Notes |
|------|-------|-------|
| 4 | Signature | |
| 4 | Child bone | |
| 4 | Parent bone | |
| 4 | Target bone | |
| 1 | Anchor type | |
| 1 | Blank (always 3) | |
| 2 | Name start | Offset into string table |
| 4 | Subsection end | Not present for last anchor |
| 4 | Maybe zero | Not present for last anchor |

## Frame interpolation (legendhavoc175, Jun 2024)

MAB supports interpolation. Extracted "keyframes" are interpolation targets, not every frame. FC4+ have a frame array defining playback order (e.g., `0,1,2,3` non-culled; `0,4,9,16` culled/skipped).

Quaternion data per section: typically 6 bytes quat + 2 bytes pad (0 or random). Complex animations vary.

## RE methodology

Reusable techniques for further MAB investigation:

1. **FCBConverter patch workflow**: create `mymod/foo/foo2/bar.tex` matching the asset path, feed the folder to FCBConverter, place the resulting `.dat`/`.fat` patch in `data_win32` (overrides the default patch; works in map editor + Linux/Wine).
2. **Cheat Engine watchpoints**: set a watchpoint on known animation bytes, trace the stack to FC3.dll + function offset, then open in Ghidra.
3. **FC2 cross-reference**: FC2 has lighter compiler optimizations → cleaner Ghidra output for the same algorithm. Cross-search distinctive constants (`4.315969e-05`, `0.7071068`) in the FC3.dll C export.
4. **Bone data**: `.skeleton` only exists for physics models. First-person models need XBG parsing for bone data.
5. **FCBConverter MAB support**: pass a `.mab` to FCBConverter — it extracts to XML (`_converted.xml`). Pass the XML back to repack (`_new.mab`). FC5/FC6 are auto-detected by version byte.

## Open questions

- Exact rotation data storage beyond 48-bit quats (padding semantics, multi-bone mapping)
- Which XBG structures set up the bone-to-offset lookup table
- FC4+ frame array exact format
- FC6 MAB differences beyond the version byte
- Header unknowns at `0x04`–`0x0C` and `unkArrayA[11]`

## References

- [Deciphering Far Cry 3's animation format](https://knockout.chat/thread/55079) — buu342's RE thread
- [PY-DuniaAnimationExtractor](https://github.com/buu342/PY-DuniaAnimationExtractor) — companion code + wiki
- [MabTools/FCBConverter](https://github.com/buu342/PY-DuniaAnimationExtractor) — ArmanIII's converter (GPLv3)
- [ResHax RE tutorials](https://reshax.com/topic/47-reverse-engineering-tutorials-collection/)
- [3D Model Researcher](https://mr.game-viewer.org/)
- FC4 (Oct 17, 2014 X360 prototype) + Watch Dogs 1 X360 prototypes have debug symbols at [Hidden Palace](https://hiddenpalace.org/)
