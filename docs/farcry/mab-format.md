---
title: MAB Animation Format
---

The `.mab` animation format used by the Dunia engine (Far Cry 2 through Far Cry 5 / New Dawn). All data is little-endian.

A MAB file is composed of a header, a Section Information block, and 9 sections. Every section aligns to 16 bytes, padded with zeroes.

Source: [buu342/PY-DuniaAnimationExtractor wiki](https://github.com/buu342/PY-DuniaAnimationExtractor/wiki).

## Header

16 bytes at the start of the file.

| Offset | Size | Description |
|--------|------|-------------|
| `0x00` | 4 bytes | MAB file version |
| `0x04` | 4 bytes | CRC (algorithm unknown) |
| `0x08` | 8 bytes | Unknown |

### File versions

| Value | Game |
|-------|------|
| `0x0000004C` | Far Cry 2 |
| `0x00000061` | Far Cry 3 |
| `0x00000062` | Far Cry 3: Blood Dragon |
| `0x00000081` | Far Cry 4 |
| `0x00000082` | Far Cry Primal |
| `0x000000B0` | Far Cry 5 + New Dawn |

## Section 0 — Section Information

Starts at offset `0x10` (right after the header). Always 224 bytes (220 + 4 bytes zero padding).

All offsets below are relative to this section. Add 16 bytes when reading from the file start.

### Animation length

At offset `0xB4` (relative), 4 bytes: a float describing the animation duration in seconds. Referenced as `ANIMLENGTH` in other sections.

### Section offsets

The next 36 bytes contain nine 4-byte integers — one offset per section. These offsets are **relative to Section 0** (add 16 for absolute file offsets).

- Offsets are not guaranteed ascending. In practice, sections 1 and 2 are swapped (offset 0 points to section 2, offset 1 points to section 1). All others follow ascending order.
- An offset of 0 means the section does not exist.

### Section sizes

To find the size of a section, subtract its offset from the next largest offset. This does not account for padding. For the last section, subtract its offset from the total size of the file minus 16 (the header).

The final 4 bytes of Section 0 are always zero padding.

## Section 1 — Unknown

No documentation yet.

## Section 2 — Unknown

No documentation yet.

## Section 3 — Root Rotation

After the first 8 bytes, this section describes root bone rotation values. The values are 4-byte floats in Y, Z, X order. Some animations store multiple rotations; many contain only one set (12 bytes total).

Partial documentation — more investigation needed.

## Section 4 — Rotation Keyframes

### Preamble

The first 4 bytes contain two 2-byte values (purpose unknown, skipped for now). After that comes a 4-byte float called the "magic float value".

### Offset table

After the first 8 bytes, a sequence of 4-byte offsets begins. Each points to the start of a rotation data block.

The offset to the **last** entry in this table is calculated as:

```
((((int)(magicfloat * ANIMLENGTH)) >> 3) * 4) + 8
```

One more 4-byte value follows the last offset — it stores the total size of this section (excluding zero padding).

To find the size of each rotation data block, subtract consecutive offsets.

### Rotation data (packed quaternions)

Quaternions are stored as three groups of 16-bit values. Decode them as follows:

**Component extraction:**

```
fVar1 = (FirstWord & 0x7FFFF) * 4.315969e-05 - 0.7071068
fVar2 = (SecondWord & 0x7FFFF) * 4.315969e-05 - 0.7071068
fVar3 = ThirdWord * 4.315969e-05 - 0.7071068
fVar4 = sqrt(1.0 - fVar1*fVar1 - fVar2*fVar2 - fVar3*fVar3)
```

**Component order** (determined by the sign bit of the first two packed values):

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

Quaternions are stored contiguously — layout details still under investigation.

## Section 5 — Unknown

No documentation yet.

## Section 6 — Root Offset Keyframes

Contains root bone offset keyframes. Minimal investigation done — more details to come.

## Section 7 — Animation Events

Contains strings describing animation events (e.g., particle effects) and likely their timestamps. Under-investigated.

## Section 8 — Unknown

No documentation yet.

## Section 9 — Unknown

No documentation yet.
