# Watch Dogs .mab Animation Format

The `.mab` format is the binary animation container used by Ubisoft's Disrupt/Dunia engine across all its games. The inner bitstream (quaternions, chunked keyframes, constant rotations) is **identical** across every game — only the outer wrapper header differs.

## Version Table

| Game | Version Magic | aNi Offset | Wrapper Differences |
|------|---------------|------------|---------------------|
| Far Cry 2 | `0x4C` | `0x14` | Oldest variant |
| Far Cry 3 | `0x61` | `0x14` | |
| Far Cry 3: Blood Dragon | `0x62` | `0x14` | |
| Far Cry 4 | `0x81` | `0x14` | |
| Far Cry Primal | `0x82` | `0x14` | |
| Far Cry 5 / New Dawn | `0xB0` | `0x14` | |
| **Watch Dogs 1** | `0x329B` | `0x14` | |
| **Watch Dogs Legion** | `0x46B4` | `0x20` | Extra `f32` framerate field at aNi+12 |

All data is little-endian.

## Wrapper Layout

### Header (16 bytes)

```
+0x00  u32     Version magic (see table above)
+0x04  u32     CRC (unknown algorithm)
+0x08  u8[8]   Unknown padding
```

### aNi Block

Immediately after the header (at `aNi_offset` from file start):

```
+0x00  'aNi'   3-byte signature
+0x03  u8      Flags
+0x04  u32     Animation data size (file size - 16)
+0x08  f32     Duration (seconds)
+0x0C  f32     Framerate (WDL only — WD1/FC don't have this field)
+0x10  u16     numBonesInAnim (top bit 0x8000 is a flag, low 15 bits = bone count)
+0x12  u16[7]  dataCounts:
               [0] = ?
               [1] = ?
               [2] = constant-rotation bone count (flag & 0x30 == 0x30)
               [3] = ?
               [4] = animated-rotation bone count (flag & 0x10, not 0x30)
               [5] = ?
               [6] = ?
+0x30  u32[11] sectionOffsets (absolute = value + 0x10 in WD1; aNi-relative in WDL)
```

### WD1-specific: section offsets are **absolute** (`value + 0x10`)
### WDL-specific: section offsets are **aNi-relative** (`value` is relative to aNi start)

### Bone Hashes (after aNi block)

```
+0x60 (WD1) or aNi+96 (WDL)  u32[n_bones]   CRC32 hash of each bone name
+0x60+4*n  u8[n_bones]        Per-bone flags:
                                 bit 0x10 = bone participates in rotation data
                                 bits 0x30 = CONSTANT rotation (JointConstantRotations)
                                 low nibble + 1 = bit depth of compressed track
```

### JointRotations (Section 4 in WD1/WDL, Section 6 in FC3)

Chunked bitstream for per-bone rotation keyframes.

```
+0x00  u32  table_size = (n_chunks + 1) × 4
+0x04  u32[n_chunks]  Chunk END offsets (section-relative, must be read sequentially)
```

Each chunk covers 8 keyframes. The data between consecutive end-offsets is the chunk's bitstream.

**Per chunk, per animated bone** (in bone index order), LSB-first bits:
```
6 bits  cflags:
  b0-b2  Per-component CONSTANT flags (if set, component is constant across all frames)
  b3     Sign bits present (implicit component sign per frame)
  b4-b5  Implicit component index (which of the 4 quaternion components is implicit)

nFrames bits  Sign of implicit component (if b3 set)

For each component c in 0..2:
  if CONSTANT (cflags bit c set):
    u16 → v = u16 / 32768 - 1  (constant for all frames)
  else:
    u16 → base = (u16 & 0xFF) / 127 - 1
           slope = (u16 >> 8) / 127.5 × INTERP_SCALE[bits]
    then nFrames × (bits)-bit raw value → v = raw × slope + base
```

**Smallest-three quaternion reconstruction:**
```
stored components fill slots 0,1,2,3 skipping the implicit index
implicit = ±sqrt(1 - sum of squares of stored components)
sign determined by sign bits
```

Component order is `(x, y, z, w)`.

### INTERP_SCALE Table

```python
INTERP_SCALE = [
    0, 0, 0.33333334, 0.14285715, 0.06666667, 0.032258064,
    0.015873017, 0.0078740157, 0.0039215689, 0.0019569471,
    0.00097751711, 0.00048851978, 0.00024420026, 0.00012208521,
    0.000061038882, 0.000030518509, 0.000015259022,
]
```

Indexed by the bone's bit depth (low nibble of per-bone flag + 1).

### JointConstantRotations (Section 4 in WD1/WDL)

6 bytes per constant-rotation bone, stored in bone index order. Each uses the same quaternion unpack as the keyframe codec:

```python
def unpack_const_quat(w0, w1, w2):
    SCALE, OFF = 4.315969e-05, 0.7071068
    f1 = (w0 & 0x7FFF) * SCALE - OFF
    f2 = (w1 & 0x7FFF) * SCALE - OFF
    f3 = w2 * SCALE - OFF
    f4 = sqrt(max(0, 1.0 - f1*f1 - f2*f2 - f3*f3))
    b0, b1 = w0 & 0x8000, w1 & 0x8000
    if not b0:
        if b1: return (f1, f2, f4, f3)
        return  (f4, f1, f2, f3)
    if b1:
        return (f1, f2, f3, f4)
    return     (f1, f4, f2, f3)
```

Identity rotation = `FF BF FF BF FF 3F` (bytes).

### Root Rotation (Section 3 in FC3/WDL)

Packed quaternions for the root bone, stored contiguously. Count is not explicitly stored — calculated from section size.

### Root Offset Keyframes (Section 6)

Position keyframes for the root bone. Not yet fully decoded in the reference implementation.

### Animation Events (Section 7)

Event data (attachment points, particle effects, sound triggers). Contains event type, timestamp, and parameters.

## Sections That Are Unknown

Sections 1, 2, 5, 8, 9 remain partially or fully undocumented. Section 1 appears to contain root rotation data for some animations.

## References

- [buu342's RE thread](https://knockout.chat/thread/55079) — FC3 format analysis via Ghidra
- [PY-DuniaAnimationExtractor wiki](https://github.com/buu342/PY-DuniaAnimationExtractor/wiki) — FC3 format docs
- [blender-io-xbg parser](https://github.com/Open-Source-Modding/blender-io-xbg/blob/main/modules/Watch_Dogs/import_mab_wd.py) — WD1/WDL reference implementation
- DisruptEditor's `mabFile.cpp` / `mabDecode.cpp` — official Ubisoft reference
