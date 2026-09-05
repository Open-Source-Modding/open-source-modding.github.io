# XeNTaX Forum — Havok Knowledge Extract

Extracted from xentax.sql forum dump (2026-08-19). Focused on knowledge NOT
already covered in `reference/havok/hkx_format.md` or `reference/havok-repos.md`.

---

## 1. Havok Version History & Game Mapping

Source: t=11155 (Havok Engine Animations), t=12424 (HAVOK *.hkx), t=12857 (Havok binary files)

| Havok Version | Packfile Version | Games | Notes |
|---|---|---|---|
| 4.1.0-r1 | — | Bioshock 1/2 | Very old, archaic format |
| 6.5 | — | Sleeping Dogs (original) | Binary mode |
| hk_2010.2.0-r1 | 8 | Skyrim LE/SE, Dark Souls 1 | Best tool support (hkxcmd, SSFADF) |
| hk_2011.2.0-r1 | 9 | Sleeping Dogs (original) | Very similar to 2010 |
| hk_2012.2.0-r1 | 9 | Alien: Isolation | SSFADF supports up to this |
| hk_2013.1.0-r1 | 9 | Sleeping Dogs DE (Definitive Edition) | Havok Content Tools version |
| hk_2013.2.0-r1 | 11 | Dark Souls II/III, various | Major header changes vs 2010 |
| hk_2014.1.0-r1 | 11 | FO4, Witcher 2, AC games | Havok Content Tools 2014.1 |

**Key insight (SergeantJoe, t=12857):** The 2010 and 2013 formats are "practically the same" except for a small header prefix. Multiple animations per file appear in 2011+ (single hkx per file in 2010).

---

## 2. Packfile Header Differences (Version 9 vs 11)

Source: t=12857 Post 97 (JohnHudeski), hkx_format.md

Packfile version 9 (2010–2012):
```
int32 misc;
int32 pad[1];
```

Packfile version 11 (2013+):
```
int32 misc;
int16 m_maxPredicates;
int16 unkn;
int32 pad[4];
```

Version 11 adds 16 extra bytes of header padding. The `--strip` flag on AssetCc1 removes "extra junk at the start of the file" (SergeantJoe, Post 68).

---

## 3. Tool Reference (Historical & Current)

### hkxcmd (figment)
- **URL**: https://github.com/figment/hkxcmd
- **Supports**: hk_2010.2.0-r1 only
- **Converts**: HKX ↔ XML
- **Limitation**: Takes 2011 files silently but writes nothing. Does not decompress spline animations.
- **Workaround**: Rename version string from 2011→2010 to force processing (partial success)

### havok2fbx (Highflex)
- **URL**: https://github.com/Highflex/havok2fbx/releases
- **Supports**: hk_2014.1.0-r1 only (specifically stated)
- **Converts**: HKX → FBX
- **Workaround**: Rename version from 2010→2014 to force processing

### SSFADF / dark_souls_hkx (Danilodum/Snaz)
- **URL**: https://github.com/Danilodum/dark_souls_hkx/releases
- **Supports**: Up to hk_2012.2.0-r1
- **Converts**: HKX animations → .damnhavok XML
- **Usage**: Place exe in animation directory, double-click (recursively processes all *.hkx except Skeleton*.hkx)
- **Limitation**: Does not work on 2013+ files ("bad file")

### Volfin's hkx2smd
- **URL**: https://bitbucket.org/Volfin/hkx2smd/overview
- **Language**: C#
- **Supports**: 2010.2.0 and 2012.2.0
- **Converts**: HKX → SMD (skeleton only)
- **Key code location**: `Program.cs` has file structure definitions at the top
- **Note**: Crashes on 2014+ files (1.5 billion element array error). Developers modified it per-game.

### AssetCc (Havok SDK tools)
- **Location**: `hkSDK_Root\Tools\BatchProcess\AssetCc`
- **Usage**:
  ```
  AssetCc1.exe --strip bin.hkx xml.hkx          # binary → XML
  AssetCc1.exe --strip --rules8011 xml.hkx bin.hkx  # XML → binary
  ```
- **Cc1 vs Cc2**: Cc1 keeps original version number; Cc2 converts to the version the exe was built with.
- **Rules**: `--rules4101` (Win32 x64) vs `--rules8011` (Win32 x86) — different binary output
- **Limitation**: AssetCc from 2014 SDK rejects 2013.2.0 files with "Unsupported file format"

### Havok Standalone Tool (ToolStandAlone.exe)
- **Supports**: Visualization and XML export of HKX files
- **Limitation**: "Wrong platform for packfile" error with many game files
- **Workaround**: Import skeleton + animation, then export

### hkxpack (Dexesttp)
- **URL**: https://github.com/Dexesttp/hkxpack
- **Converts**: HKX binary → Tag-XML (not regular XML)
- **Note**: Tag-XML is different from regular XML; hkxcmd cannot read it

### Havok Content Tools (3ds Max plugin)
- **Use**: Export animations as spline compressed or uncompressed for comparison
- **Location**: Available via Intel Havok site (now defunct, Microsoft acquired)
- **Key use case**: Create reference hkx files to compare headers with game files

---

## 4. hkaSplineCompressedAnimation Deep Dive

Source: t=12857, t=11155, t=4066

### Animation Type Enum
```cpp
enum hkaAnimation::AnimationType {
    HK_UNKNOWN_ANIMATION = 0x0,
    HK_INTERLEAVED_ANIMATION = 0x1,
    HK_MIRRORED_ANIMATION = 0x2,
    HK_SPLINE_COMPRESSED_ANIMATION = 0x3,
    HK_QUANTIZED_COMPRESSED_ANIMATION = 0x4,
    HK_PREDICTIVE_COMPRESSED_ANIMATION = 0x5,
    HK_REFERENCE_POSE_ANIMATION = 0x6,
};
```

### TrackCompressionParams (from PBD analysis, SergeantJoe)
```cpp
struct hkaSplineCompressedAnimation::TrackCompressionParams {
    float m_rotationTolerance;
    float m_translationTolerance;
    float m_scaleTolerance;
    float m_floatingTolerance;
    unsigned __int16 m_rotationDegree;
    unsigned __int16 m_translationDegree;
    unsigned __int16 m_scaleDegree;
    unsigned __int16 m_floatingDegree;
    hkEnum<RotationQuantization, unsigned char> m_rotationQuantizationType;
    hkEnum<ScalarQuantization, unsigned char> m_translationQuantizationType;
    hkEnum<ScalarQuantization, unsigned char> m_scaleQuantizationType;
    hkEnum<ScalarQuantization, unsigned char> m_floatQuantizationType;
};
```

### Quantization Types
```cpp
enum RotationQuantization {
    POLAR32 = 0x0,      // 32-bit: 10-bit magnitude + 18-bit phi/theta
    THREECOMP40 = 0x1,  // 40-bit: three 11-bit components
    THREECOMP48 = 0x2,  // 48-bit: three 15-bit components
    THREECOMP24 = 0x3,  // 24-bit
    STRAIGHT16 = 0x4,   // 16-bit quat: 8-bit components
    UNCOMPRESSED = 0x5, // 4x float32
};

enum ScalarQuantization {
    BITS8 = 0x0,
    BITS16 = 0x1,
};
```

### Default Havok Spline Compression Settings (philgrf, t=12857 Post 80)
- Position: 16 bits
- Rotation: 40 bits (THREECOMP40)
- Scale: 16 bits
- Float: 16 bits
- Frames per block: 256

**Critical note**: Compression is highly customizable per-bone. The defaults are just a starting point. Per-bone customization allows grouping bones by hierarchy depth for different compression levels.

### Quaternion Encoding (philgrf, Post 78)
- Rotation stored as quaternion (x, y, z, w)
- Highest magnitude component discarded (since x²+y²+z²+w²=1)
- Remaining 3 components encoded with quantization bits
- Reference: Riot Games engineering blog on spline compression, Bitsquid blog

---

## 5. Known Issues & Workarounds

### Version String Manipulation (SergeantJoe, t=12857)
Files from games often have extra data before the "WaaW" magic:
```
// To process: cut data before "WaaW", rename version string
// 2011 → 2010 for hkxcmd
// 2010 → 2014 for havok2fbx
```
Each tool only supports specific versions, but the underlying format is often identical.

### Header Replacement Strategy (shakotay2, t=12857)
1. Create a reference hkx file using Havok Content Tools + 3ds Max
2. Compare headers between reference and game file
3. Identify which header bytes differ
4. Replace game header with reference header (carefully)

### Animation Swap (SergeantJoe, Post 70)
To swap animations that are already in-game:
1. Replace data starting with "WaaW" and ending with "+    yyyyyy"
2. The two header numbers before "WaaW" are memory size pointers
3. Set them to minimum possible values to avoid crashes

### blendHint Fix (SergeantJoe, Post 67)
Changing `hkaAnimationBinding.blendHint` from NORMAL to ADDITIVE can reveal animation data that appears "locked" during conversion.

### Platform Flag Issues (Loomy, Post 88)
```
public enum hkPlatform {
    HCL_PLATFORM_WIN32 = 0x0,
    HCL_PLATFORM_X64 = 0x1,
    HCL_PLATFORM_PS3 = 0x10,
    HCL_PLATFORM_XBOX360 = 0x20,
    HCL_PLATFORM_WII = 0x40,
    HCL_PLATFORM_LINUX = 0x100,
    HCL_PLATFORM_ANDROID = 0x800,
    // ...
}
```
PS3/big-endian files require byte-swapping for all multi-byte values.

---

## 6. Disrupt Engine HKX specifics (from xentax)

Source: t=12424 (Oleg's post about Ubisoft Disrupt)

- **Vehicle physics hkx files** contain: `hkpRigidBody`, `hkpListShape`, `hkpConvexVerticesShape`, `hkpConvexVerticesConnectivity`, `hkpCylinderShape`, `hkpConvexTransformShape`, `hkpBoxShape`, `hkpConvexTranslateShape`, `hkpConstraintInstance`, `hkpLimitedHingeConstraintData`, `hkpPositionConstraintMotor`, `hkpStaticCompoundShape`, `hkpBvCompressedMeshShape`
- **Geometry file (.xbg)** contains pre-defined vertex deformation data
- **hkx file** may contain mapping of physical volume to geometry vertices (hit/damage on collision → visual deformation)
- **Skeleton is external**: Each model has a "reference skeleton" file (XML converted to binary), included for reference only — damaging it doesn't affect gameplay
- **nomadVehiclePhysResourceData** class wraps all vehicle physics

---

## 7. Convex Mesh Decomposition for Collision

Source: t=18206 (Bloodborne collision), t=11434 (Killzone meshes)

- Havok packfiles saved on 32-bit machines with big-endian integers (PS3 games)
- Packfiles are NOT cross-platform — require exact same layout rules to load
- Old packfiles need versioning enabled (deprecated in newer SDKs)
- For long-lasting assets, Havok recommends tagfiles over packfiles

### hkpBvCompressedMeshShape
Used for compressed collision meshes. Contains:
- Vertices (can be quantized)
- Face indices
- Connectivity data
- Bounding volume hierarchy

---

## 8. Practical Workflow Summary

### Extracting Skeleton from HKX
1. **Best tool**: Volfin's hkx2smd (C#, source available)
2. **Manual**: shakotay2's method (search for bone names, extract transforms)
3. **Alternative**: hkxcmd (2010 files only)

### Extracting Animations from HKX
1. **2010 files**: hkxcmd → XML → Havok Standalone Tool
2. **2012 files**: havok2fbx (with version rename), or SSFADF
3. **2013+ files**: No reliable method exists as of 2022
4. **Workaround**: Use Havok Content Tools to create reference animations, compare with game files

### Converting HKX → 3D Software
1. HKX → SMD (Volfin's hkx2smd) → Blender/Max/Maya
2. HKX → FBX (havok2fbx) → any 3D software
3. HKX → XML (AssetCc/hkxcmd) → Havok Standalone Tool → export

---

## 9. Key Repository Links

| Tool | URL | Supports |
|---|---|---|
| hkxcmd | https://github.com/figment/hkxcmd | 2010 |
| havok2fbx | https://github.com/Highflex/havok2fbx | 2014 |
| dark_souls_hkx | https://github.com/Danilodum/dark_souls_hkx | ≤2012 |
| hkx2smd (Volfin) | https://bitbucket.org/Volfin/hkx2smd | 2010, 2012 |
| hkxpack | https://github.com/Dexesttp/hkxpack | Tag-XML |
| HavokLib (PredatorCZ) | https://github.com/PredatorCZ/HavokLib | 5.0–2017 |
| botw_havok | https://github.com/krenyy/botw_havok | Python parser |

---

## 10. Attribution

Key contributors from XeNTaX:
- **SergeantJoe** — Extensive 2013 format analysis, animation swap technique
- **shakotay2** — Manual skeleton extraction, header comparison methodology
- **JohnHudeski** — SDK demo code for animation conversion, format analysis
- **Bastien** — Early 2010 format analysis, AssetCc documentation
- **Snaz (Danilodum)** — Dark Souls HKX tools, SSFADF converter
- **Volfin** — hkx2smd C# converter (Alien: Isolation)
- **philgrf** — Spline compression theory and documentation analysis
- **Oleg** — Disrupt engine HKX structure analysis (vehicle physics)
- **Loomy** — Big-endian/PS3 parsing, platform flags, hkxpack integration
- **HunterAP** — Dark Souls III format testing, 64-bit pointer issues
