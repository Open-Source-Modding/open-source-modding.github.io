# GitHub Repos for HKX/Havok Format Research

> **Cross-reference**: Community tools (hkxcmd, havok2fbx, AssetCc, HavokLib, volfin hkx2smd) → [XeNTaX Havok knowledge §8–9](../havok/xentax-havok-knowledge.md)
>
> **Cross-reference**: Full tool workarounds, version compatibility, practical techniques → [`reference/havok/hkx_format.md`](../../havok/hkx_format.md)

## hkxparse (exyorha)
- **URL**: https://github.com/exyorha/hkxparse
- **What**: C++ library for parsing Havok packfiles and tagfiles
- **Key feature**: Layout system (`HavokPackfileLayouts`) defines class member layouts with names, types, offsets
- **Current support**: Havok 2010 only (JSON layout file)
- **Use for us**: If we generate a 2017 layout JSON, we can compile it into hkxparse and use it to parse compressed mesh data
- **Layout format**: JSON with `classes[].members[].{name, type, offset, arraySize, flags}`
- **Generator**: `make_layout.rb` converts JSON → C++ header/source

## hkxImport (SebboHN)
- **URL**: https://github.com/SebboHN/hkxImport
- **What**: 3ds Max plugin for HKX import (Havok 2010-2023)
- **Key find**: `x64_2017/HavokMax.dlu` — Havok 2017 runtime DLL for 3ds Max
- **Use for us**: The DLL links against Havok 2017 and might contain the class layouts

## fbxbimporter (Bewolf2)
- **URL**: https://github.com/Bewolf2/fbximporter
- **What**: FBX to HKX converter using Project Anarchy/Havok
- **Use for us**: Limited — focuses on animation, not collision

## SSFADF / dark_souls_hkx (Snaz / Danilodum)
- **URL**: https://github.com/Danilodum/dark_souls_hkx/releases
- **What**: HKX to XML converter (`.damnhavok` format). Reads up to 2012.2.0. Works with Dark Souls 1 files.
- **Usage**: Place `SSFADF.exe` in root of hkx animation directory; recursively converts all `.hkx` except `Skeleton.hkx` and `Skeleton-out.hkx`. Open resulting `.damnhavok` in Noesis with converted skeleton in same folder.

## havok2fbx (Highflex)
- **URL**: https://github.com/Highflex/havok2fbx/releases
- **What**: Reads up to Havok 2014.1.0. Converts HKX to FBX.
- **Workaround**: Rename version string from `2010` to `2014` to make 2010/2011/2012 files loadable.
- **Limitation**: Only works with 32-bit files; 64-bit Dark Souls III files fail.

## volfin's hkx2smd (volfin)
- **URL**: https://bitbucket.org/Volfin/hkx2smd/overview
- **What**: C# converter from HKX to SMD (Source engine format). Works with 2012.2.0 and 2010.2.0.
- **Supports**: Skeleton extraction; animation output was planned but not completed.
- **Note**: Written in C# — class structure definitions at top of `Program.cs`. Will need per-game tweaking because HKX is highly customizable.

## AssetCc (Havok SDK tool)
- **What**: Batch converter between binary HKX and XML HKX.
- **Commands**:
  ```
  AssetCc1.exe --strip bin.hkx xml.hkx      # Binary → XML (keeps version)
  AssetCc1.exe --strip --rules8011 xml.hkx bin.hkx  # XML → Binary
  ```
- `--strip` removes extra junk at file start
- `--rules4101` vs `--rules8011`: different platform rule sets (4101 = Win32, preferred)
- Cc1 keeps original version; Cc2 converts to SDK version it was built with
- AssetCc2 from hk2013 SDK: works with `hk_2013.2.0-r1` but NOT `hk_2013.1.0-r1`

## Havok Content Tools for 3ds Max
- Plugin for 3ds Max that can export HKX files (spline compressed and uncompressed).
- The `Prune` modifier removes extra junk, producing files nearly identical to originals.
- Export requires selecting "Packfile" in the "Write to Platform" option.

## HavokLib (Lukas Cone / PredatorCZ)
- **URL**: https://github.com/PredatorCZ/HavokLib/blob/master/source/phys/Havok/Compressor.hpp
- **What**: Contains Havok spline compressor/decompressor implementation in C++.
- **Use for us**: Source code publicly available and useful for understanding the decompression algorithm.

## hkxpack (Dexesttp)
- **URL**: https://github.com/Dexesttp/hkxpack
- **What**: Java-based HKX to XML converter. Converts to Tag-XML format (not regular XML).
- **Note**: Tools like hkxcmd and Noesis damnhavok plugin expect regular XML, not tag-XML.
## Havok SDK (Intel / Microsoft)

- **What**: The Havok SDK was freely available from Intel until Microsoft acquired Havok.
- **Contains**:
  - AssetCc batch converter
  - Havok Standalone Tool (visualization)
  - Havok Content Tools (3ds Max plugin)
  - Full documentation files including compression algorithm details
  - Demo projects (AnimatedSkeletonDemo for animation extraction)
- **SDK demo approach**: load skeleton + animation files, then read data programmatically via the `AnimatedSkeletonDemo.cpp` class.

### Concrete SDK sources (shared by Encrypted, WD Modding Discord, 2026-08)

> "everything i used to crack HKX" — community-proven sources for SDKs and tagfile tooling.

- **TagTools** (blueskythlikesclouds): https://github.com/blueskythlikesclouds/TagTools — tools for editing Havok **2015/2016 binary tag files**; Havoc branch kept for reference.
- **Havok 2014.1 SDK torrent** (rutracker): https://rutracker.org/forum/viewtopic.php?t=5174063 — "HAVOK PHYSICS 2012 SDK + HAVOK ANIMATION, NoSimd, Win32 2014.1.0" (`[rutracker-5174063].torrent`).
- **Google Drive SDK folders**:
  - https://drive.google.com/drive/folders/0B8SgSQGjqypSaUxIT2ptUVVwVVk?resourcekey=0-auzKqYyyhxEMzdQNNA23sA
  - https://drive.google.com/drive/folders/0B3SZOCCKd7Ipb2w2SkN4bjJwSzg?resourcekey=0-vPD5gmb33TWGaysgqSKo-w
  - https://drive.google.com/drive/folders/0B-85OBEi6SEkV2RhTTl0bzliZ28?resourcekey=0-Ng8WCMDV0TZ8DBsgV9TBvA

## Key Insight
To parse the compressed mesh from TAG0 files, we need:
1. A Havok 2017.2 layout JSON (class member definitions with offsets)
2. Either generate it from primitive files' TYPE section, or extract from HavokMax.dlu
3. Compile it into hkxparse's layout system
4. Use hkxparse's tagfile parser to extract the compressed mesh data

The primitive files at `data_win64/generated/primitives/prim_4_*.hkx` are the source of truth for the 2017.2 layout.
