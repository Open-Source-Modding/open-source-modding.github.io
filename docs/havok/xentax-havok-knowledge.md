# Havok - XeNTaX Community Knowledge

Extracted and organized from XeNTaX forum discussions (2010–2022).
Complements the clean-room format specs in [`reference/havok/hkx_format.md`](../reference/havok/hkx_format.md) (FO4-specific, Havok 2014.1.0) and the Watch Dogs HKX collision doc [`reference/disrupt/watch_dogs/hkx-format.md`](../disrupt/watch_dogs/hkx-format.md).

## HKX File Format Basics

### Magic and Header

The Havok binary packfile starts with a 32-bit magic `0x57E0E057` ("WààW" in little-endian ASCII), followed by `0x10C0C010`.
The version tag (e.g. `hk_2013.2.0-r1`) appears at offset `0x24` within the header.
The endian flag is at offset `0x11`: `01` = little-endian, `00` = big-endian (PS3).

*(shakotay2, 2015-06-10; Bastien, 2014-01-24)*

### File Versions and SDK Compatibility

| Havok Version | Packfile Version | Games | Tool Compatibility |
|---|---|---|---|
| 4.1.0-r1 | — | BioShock 1/2 | Too old for most tools |
| 4.6.0-r1 | — | Tenchu (Wii) | Havok SDK required |
| 6.5 | — | Sleeping Dogs | Very old packfiles |
| 2010.2.0-r1 | 8–9 | Skyrim, Dark Souls 1 | hkxcmd (open source) |
| 2011.2.0-r1 | 9 | Sleeping Dogs, Witcher 2 | hkxcmd (with version change) |
| 2012.2.0-r1 | 9 | Alien: Isolation | hkxcmd, SSFADF, havok2fbx |
| 2013.1.0-r1 | 9 | Sleeping Dogs DE | AssetCc (SDK tool) |
| 2013.2.0-r1 | 11 | Various (Alien Isolation DE) | Limited tool support |
| 2014.1.0-r1 | 11 | Dark Souls 2/3, FO4 | havok2fbx, volfin's hkx2smd |

*(SergeantJoe, 2014–2016; JohnHudeski, 2018)*

### Key Difference Between Version 9 and 11 Headers

Version 9 (used by 2010–2013) has simpler header padding. Version 11 (2013.2+ / 2014+) adds:
- `int16 max_predicates` + `int16 unknown` replacing `int32 misc + pad[1]`
- 16 bytes of additional padding after the section table entries

*(JohnHudeski, 2018-06-05)*

### Platform Flags

```c
enum hkPlatform {
    HCL_PLATFORM_WIN32   = 0x0,
    HCL_PLATFORM_X64     = 0x1,
    HCL_PLATFORM_MACPPC  = 0x2,
    HCL_PLATFORM_IOS     = 0x4,
    HCL_PLATFORM_MAC386  = 0x8,
    HCL_PLATFORM_PS3     = 0x10,
    HCL_PLATFORM_XBOX360 = 0x20,
    HCL_PLATFORM_WII     = 0x40,
    HCL_PLATFORM_LRB     = 0x80,
    HCL_PLATFORM_LINUX   = 0x100,
    HCL_PLATFORM_NGP     = 0x400,
    HCL_PLATFORM_ANDROID = 0x800,
    HCL_PLATFORM_CTR     = 0x1000,
};
```

*(Loomy, 2017-12-09)*

PS3 files are big-endian; changing the endianness flag alone is insufficient — byte-swapping of all multi-byte values is required.

## Animation Formats

### Animation Type Enum

```c
enum hkaAnimation::AnimationType {
    HK_UNKNOWN_ANIMATION              = 0,
    HK_INTERLEAVED_ANIMATION          = 1,
    HK_MIRRORED_ANIMATION             = 2,
    HK_SPLINE_COMPRESSED_ANIMATION    = 3,
    HK_QUANTIZED_COMPRESSED_ANIMATION = 4,
    HK_PREDICTIVE_COMPRESSED_ANIMATION= 5,
    HK_REFERENCE_POSE_ANIMATION       = 6,
};
```

*(SergeantJoe, 2014-10-11)*

### hkaSplineCompressedAnimation Structure

```c
struct hkaSplineCompressedAnimation : hkaAnimation {
    int  m_numFrames;
    int  m_numBlocks;
    int  m_maxFramesPerBlock;
    int  m_maskAndQuantizationSize;
    float m_blockDuration;
    float m_blockInverseDuration;
    float m_frameDuration;
    hkArray<unsigned int> m_blockOffsets;
    hkArray<unsigned int> m_floatBlockOffsets;
    hkArray<unsigned int> m_transformOffsets;
    hkArray<unsigned int> m_floatOffsets;
    hkArray<unsigned char> m_data;
    int  m_endian;
};
```

*(SergeantJoe, 2014-10-11, 2015-05-18)*

### Spline Compression Parameters

The Havok SDK exposes `hkaSplineCompressedAnimationTrackCompressionParams` (signature `0x42e878d3`, 28 bytes):

```c
struct TrackCompressionParams {
    float  rotationTolerance;       // +0
    float  translationTolerance;    // +4
    float  scaleTolerance;          // +8
    float  floatingTolerance;       // +12
    uint16 rotationDegree;          // +16
    uint16 translationDegree;       // +18
    uint16 scaleDegree;             // +20
    uint16 floatingDegree;          // +22
    RotationQuantization rotationQuantizationType; // +24
    ScalarQuantization   translationQuantizationType; // +25
    ScalarQuantization   scaleQuantizationType;       // +26
    ScalarQuantization   floatQuantizationType;       // +27
};
```

### Quantization Enums

```c
enum RotationQuantization {
    POLAR32     = 0,
    THREECOMP40 = 1,
    THREECOMP48 = 2,
    THREECOMP24 = 3,
    STRAIGHT16  = 4,
    UNCOMPRESSED= 5,
};

enum ScalarQuantization {
    BITS8  = 0,
    BITS16 = 1,
};
```

*(SergeantJoe, 2015-05-20 — from hkxcmd report output and Havok 2013 PBD)*

### Default Compression Settings

From the Havok 2013 SDK documentation:
- Position: 16 bits
- Rotation: 40 bits
- Scale: 16 bits
- Float: 16 bits
- Frames per block: 256

Per-bone customization is possible. Tighter tolerances for "major bones" (spine, arms) and relaxed for "minor bones" (fingers) reduces data size.

*(philgrf, 2017-03-12)*

### Spline Compression Theory

The rotation encoding likely works as:
1. Quaternion `(x,y,z,w)` where `x²+y²+z²+w²=1`
2. Discard the highest-magnitude component (reconstructable from the other three)
3. Remaining components stored with `m` bits for the discarded value index, `n` bits for each remaining component
4. Bytes read according to a per-bone quantization key

References:
- Riot Games engineering blog on animation compression: https://engineering.riotgames.com/news/animating-valorable-characters
- Bitsquid blog on quantized compression: http://bitsquid.blogspot.fr/2009_11_01_archive.html

*(philgrf, 2017-03-06)*

### BIN Animation Containers

Animation `.bin` files are concatenated compressed HKX files. Each individual `.hkx` starts with hex value `6499` and can be split by scanning for this marker.

To process: split into individual HKX files, change the version tag from 2011→2010 or 2014→2010, then feed to hkxcmd.

*(SergeantJoe, 2014-11-10, 2016-03-06)*

## Tools

### hkxcmd (figment)

Open-source tool for HKX↔XML conversion. Only supports up to Havok 2010.2.0.
Will silently fail on newer versions (accepts file but produces no output).

**Workaround for 2011 files**: Cut data before `WààW` header, change version string from `hk_2011.2.0-r1` to `hk_2010.2.0-r1`.

https://github.com/figment/hkxcmd

*(Bastien, 2014-01-24; SergeantJoe, 2016-03-06)*

### SSFADF / dark_souls_hkx (Snaz / Danilodum)

HKX to XML converter (`.damnhavok` format). Reads up to 2012.2.0. Works with Dark Souls 1 files.

Usage: Place `SSFADF.exe` in root of hkx animation directory; it recursively converts all `.hkx` files except `Skeleton.hkx` and `Skeleton-out.hkx`. Open resulting `.damnhavok` in Noesis with a converted skeleton in the same folder.

https://github.com/Danilodum/dark_souls_hkx/releases

*(Snaz, 2016-02-29, 2016-03-05)*

### havok2fbx (Highflex)

Reads up to Havok 2014.1.0. Converts HKX to FBX format.

**Workaround**: Rename version string from `2010` to `2014` to make 2010/2011/2012 files loadable.

Limitation: Only works with 32-bit files; 64-bit Dark Souls III files fail.

https://github.com/Highflex/havok2fbx/releases

*(Snaz, 2016-03-06; SergeantJoe, 2016-03-06)*

### volfin's hkx2smd (volfin)

C# converter from HKX to SMD (Source engine format). Works with 2012.2.0 and 2010.2.0.
Supports skeleton extraction; animation output was planned but not completed.

Written in C# — file structure definitions at the top of `Program.cs`. Will need per-game tweaking because HKX is highly customizable.

https://bitbucket.org/Volfin/hkx2smd/overview

*(volfin, 2016-05-17, 2016-05-18)*

### AssetCc (Havok SDK tool)

Batch converter between binary HKX and XML HKX.

```
AssetCc1.exe --strip bin.hkx xml.hkx      # Binary → XML (keeps version)
AssetCc1.exe --strip --rules8011 xml.hkx bin.hkx  # XML → Binary
```

- `--strip` removes the extra junk at file start
- `--rules4101` vs `--rules8011`: different platform rule sets (4101 = Win32, preferred)
- Cc1 keeps original version; Cc2 converts to the SDK version it was built with
- AssetCc2 from hk2013 SDK: works with `hk_2013.2.0-r1` but NOT `hk_2013.1.0-r1`

*(SergeantJoe, 2016-04-04, 2016-04-05; Skykila, 2014-12-27)*

### Havok Content Tools for 3ds Max

Plugin for 3ds Max that can export HKX files (spline compressed and uncompressed).
The `Prune` modifier removes extra junk, producing files nearly identical to originals.
Export requires selecting "Packfile" in the "Write to Platform" option.

Used by shakotay2 to successfully create compressed animations for comparison.

*(shakotay2, 2015-07-05; SergeantJoe, 2015-06-18)*

### HavokLib (Lukas Cone / PredatorCZ)

Contains Havok spline compressor/decompressor implementation in C++.
Source code is publicly available and useful for understanding the decompression algorithm.

https://github.com/PredatorCZ/HavokLib/blob/master/source/phys/Havok/Compressor.hpp

*(ronwall, 2020-10-29)*

### hkxpack (Dexesttp)

Java-based HKX to XML converter. Converts to Tag-XML format (not regular XML).
Tools like hkxcmd and Noesis damnhavok plugin expect regular XML, not tag-XML.

https://github.com/Dexesttp/hkxpack

*(HunterAP, 2016-05-15; Loomy, 2017-12-21)*

### Havok SDK (Intel / Microsoft)

The Havok SDK was freely available from Intel until Microsoft acquired Havok.
The SDK contains:
- AssetCc batch converter
- Havok Standalone Tool (visualization)
- Havok Content Tools (3ds Max plugin)
- Full documentation files including compression algorithm details
- Demo projects (AnimatedSkeletonDemo for animation extraction)

The SDK demo approach: load skeleton + animation files, then read data programmatically via the `AnimatedSkeletonDemo.cpp` class.

*(SergeantJoe, 2017-03-10; JohnHudeski, 2015-06-13)*

## Practical Techniques

### Extracting Skeletons to SMD

shakotay2's method (partially manual):
1. Find bone names in the HKX binary (look for name strings after `hkClass`)
2. Map parent indices from the `parentIndices` array
3. Extract reference pose transforms (quaternion + translation + scale per bone)
4. Write as SMD format

The process was semi-automated: three offsets needed manual searching, but the rest was scripted.

*(shakotay2, 2015-05-19, 2015-06-04)*

### Anim Swap Technique

To replace animations in-game:
1. Swap data starting with `"WààW"` (magic header) and ending with `"+    yyyyyy"`
2. Two header size values control memory allocation — set both to minimum if swapping with same-sized animation
3. Works for swapping existing in-game animations

Creating fully custom animations requires converting XML→binary via AssetCc and injecting via the same swap method.

*(SergeantJoe, 2016-04-10)*

### Version Renaming Trick

Many HKX tools are version-gated but the internal format is nearly identical between versions:
- 2011 → 2010: `hkxcmd` works (with data before magic stripped)
- 2012 → 2010: `hkxcmd` works
- 2010 → 2014: `havok2fbx` works

Always strip any data before the `WààW` magic first.

*(SergeantJoe, 2016-03-06; Snaz, 2016-03-05)*

### Converting HKX to SMD via Havok SDK Demo

JohnHudeski's approach using the AnimatedSkeletonDemo:
1. Place skeleton `.hkx` in the demo's resource path
2. Modify `AnimatedSkeletonDemo.cpp` to export to SMD instead of rendering
3. Compile with VS2012+ and the matching Havok SDK
4. The demo loads skeleton and animation, reads bone data, and can be modified to write to file

The key code modification points:
- Line ~1260: Set asset filename
- Line ~1276: Set output file and format
- Lines ~1313-1316: Animation input/output folder paths

*(JohnHudeski, 2015-06-13)*

### Dark Souls III (64-bit HKX)

DS3 uses Havok 2014.1.0-r1 64-bit. The hkx2smd tool crashes with `OutOfMemoryException` because newer formats report 1.5 billion elements in arrays. The `PointerSize` flag (value 8 for 64-bit, 4 for 32-bit) affects section size calculations.

Skeleton extraction is possible manually by reading bone names and parent indices directly from the binary.

*(HunterAP, 2016-05-17, 2017-12-10; shakotay2, 2016-05-17)*

## Common Issues and Solutions

### "Wrong platform for packfile"

Caused by endianness or architecture mismatch. The HKX was compiled for a different platform than the tool expects. Solutions:
- Check endian flag at offset 0x11
- For PS3 (big-endian) files: byte-swap all multi-byte values
- For 64-bit files: tools must support 64-bit pointers (many only handle 32-bit)

*(shakotay2, 2015-05-30; HunterAP, 2016-05-15)*

### hkxcmd Silently Fails

hkxcmd only properly supports Havok 2010. It will accept 2011+ files without error but produce no output. Always verify the `-out.hkx` file exists and has non-zero size.

*(SergeantJoe, 2016-03-06)*

### Animation Bones Not Moving

When converting via hkxcmd+havok2fbx, skeletons transfer correctly but animations may show only root motion with all other bones frozen. Possible causes:
- Version mismatch in the conversion chain
- `blendHint` in `hkaAnimationBinding` — try changing from NORMAL to ADDITIVE
- Binary→XML conversion may lose quantization data

*(SergeantJoe, 2016-03-08)*

### Havok Standalone Tool Issues

The Standalone Tool (ToolStandAlone.exe) can visualize HKX files and export to XML, but:
- Requires matching SDK version for the file
- Some HKX files from games cannot be loaded even with correct version
- Skeleton may not render if bones are connected to root in unusual ways
- The Filter Manager rarely works on extracted files

*(shakotay2, 2014-12-25, 2015-06-14)*

## Key Classes in HKX Files

### Physics Classes (for Watch Dogs / Disrupt)

```
hkClass, hkClassMember, hkClassEnum, hkClassEnumItem
nomadVehiclePhysResourceData
hkpRigidBody, hkpListShape
hkpConvexVerticesShape, hkpConvexVerticesConnectivity
hkpCylinderShape, hkpConvexTransformShape
hkpBoxShape, hkpConvexTranslateShape
hkpConstraintInstance, hkpLimitedHingeConstraintData
hkpPositionConstraintMotor
hkpStaticCompoundShape
hkpBvCompressedMeshShape
```

*(Oleg, 2014-12-25)*

### Animation Classes

```
hkaAnimationContainer    — top-level container
hkaSkeleton              — bone hierarchy
hkaBone                  — individual bone
hkaAnimation             — base animation class
hkaSplineCompressedAnimation — spline-compressed (most common)
hkaAnimationBinding      — maps animation tracks to skeleton bones
hkaAnimatedReferenceFrame — root motion reference
hkaAnnotationTrack       — named annotation markers
```

## References and Resources

- Havok SDK documentation (saved copies available — contact SergeantJoe, 2017)
- Havok SDK download mirrors: http://forum.keenswh.com/threads/havok-tools.7374695/
- Intel Havok archives: https://software.intel.com/sites/havok/en/ (now defunct)
- Allanoon's tool collection: https://drive.google.com/drive/folders/1D2ptUVVwVVk
- 010 Editor template for HKX parsing: http://pastebin.com/HerN0A2U (Loomy, 2016)
