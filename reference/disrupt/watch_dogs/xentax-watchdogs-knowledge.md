# Watch Dogs / Disrupt Engine — XeNTaX Knowledge Extraction

Extracted and organized from XeNTaX forum discussions (2014–2023). Technical knowledge only; trivial posts filtered out.

**Cross-references to site reference docs:**
- [Havok packfile format (WD1/WD2, 64-bit)](packfile-format.md)
- [Watch Dogs hashing (FNV/CRC32, WD2 CRC64/FNV64)](hashing.md)
- [Community tools & references (Volfin, FranciscoManzanilla, ZModeler)](community-resources.md)
- [HKX collision format (Disrupt's modified Havok)](hkx-format.md)
- [Compressed mesh format](compressed-mesh-format.md)
- [Injection internals](injection-internals.md)
- [Material descriptors (85 XML files)](materialdescriptors/)

---

## FAT/DAT Archive Format

### WD1 FAT/DAT Structure

**Header** (by Ekey, 2014-05-23):
```c
struct FATHeader {
    DWORD dwID;         // Magic: "3TAF" (0x33544146)
    DWORD dwVersion;    // Always 8
    DWORD dwUnknown;
    DWORD dwTotalFiles;
};

struct FATEntry {
    DWORD dwHash;       // Filename hash
    DWORD dwSize;       // Uncompressed size
    DWORD dwZSize;      // Compressed size
    DWORD dwOffset;     // Offset into .dat
};
```

**Compression per platform** (Ekey, 2014-05-23):
- PC: XMem (modified)
- PS3: Deflate
- Xbox 360: XMem

**Magic header in DAT** (cra0, 2014-05-25): `0x0F 0xF5 0x12 0xEE` — XMem signature.

**Archive behavior** (Rick, 2014-05-28):
- Game loads `patch.fat`/`patch.dat` if present, overriding content (like Far Cry 3).
- Disrupt engine is largely based on Dunia 2 (Far Cry 3).
- The archive format supports uncompressed data fine — no need for modified EXE.
- Don't repack game archives. Put modified files into a new `patch.fat`/`patch.dat`.

### XCompress Header (PC)

**Standard header** (cra0, 2014-05-25):
```c
long Identifier;                  // XCOMPRESS_FILE_IDENTIFIER_LZXNATIVE
long ContextFlags;
long Flags;
long WindowSize;
long CompressionPartitionSize;
long UncompressedSizeHigh;
long UncompressedSizeLow;
long CompressedSizeHigh;
long CompressedSizeLow;
long UncompressedBlockSize;
long CompressedBlockSizeMax;
```

**Modified version** (killerpepo, 2014-05-26; all big endian):
```c
long Identifier;                  // XCOMPRESS_FILE_IDENTIFIER_LZXNATIVE
long ContextFlags;
short Version;
short Reserved;
long Flags;
long WindowSize;
long CompressionPartitionSize;
long UncompressedSizeHigh;
long UncompressedSizeLow;
long CompressedSizeHigh;
long CompressedSizeLow;
long UncompressedBlockSize;
long CompressedBlockSizeMax;
long CompressedBlockSize;
```

### PS3 Compressed Data Header (Ekey, 2014-05-25)

```c
struct DataHeader {
    SHORT wChunksCount;
    SHORT wUnknown;     // 0xFF 0x0F
    // followed by wChunksCount × SHORT chunk sizes
};
```

### WD2 FAT/DAT Structure

**Header** (Ekey, 2016-12-06):
```c
struct FATHeader {
    uint32_t dwID;        // Magic: "5TAF" (0x35544146)
    uint32_t dwVersion;   // 11
    uint32_t dwUnknown1;
    uint32_t dwUnknown2;  // -1
    uint32_t dwUnknown3;  // -1
    uint32_t dwUnknown4;  // 0
    uint32_t dwTotalFiles;
};
```

**Entry format** (Ekey, 2016-12-06):
```c
struct FATEntry {
    uint64_t dwHash;      // NameHash
    uint32_t dwSize;
    uint32_t dwOffset;
    uint32_t dwZSize;
};
```

**Gibbed source mapping** (Ekey, 2016-12-06):
```c
NameHash = a;
NameHash |= ((ulong)b) << 32;
Offset = (long)d << 2;
Offset |= ((c & 0xC0000000u) >> 30);
UncompressedSize = (uint)(e & 0xFFFFFFFC) >> 2;
CompressedSize = (uint)((c & 0x3FFFFFFF) >> 0);
```

**WD2 FAT variant** (Sir Kane, 2016-12-06):
```c
struct SFatFileEntry {
    uint64_t Hash;
    uint64_t CompressedSize : 30;
    uint64_t Offset : 34;
    uint32_t CompressionMethod : 2;   // 0 = LZMA, 2 = modified LZ4
    uint32_t UncompressedSize : 30;
};
```

**WD2 hash algorithm** — FNV1a x64 (Ekey, 2016-12-06):
```c
static ulong gethash64(string value) {
    ulong fnv64Prime = 0x00000100000001B3ul;
    ulong hash = 0xCBF29CE484222325ul;
    for (var i = 0; i < value.Length; i++) {
        hash *= fnv64Prime;
        hash = hash ^ value[i];
    }
    return hash & 0x1FFFFFFFFFFFFFFFul | 0xA000000000000000ul;
}
```

**WD2 FAT analysis** (disastorm, 2016-12-03):
- First 8 bytes always same: `35544146 0B000000` — name/version
- Next 4 bytes: `01064600` (most archives) or `00004600` (sound archives)
- Followed by `FFFFFFFF FFFFFFFF` and `00000000`
- Fourth column values are constantly increasing (not offsets, too high)

### WD2 Compression

**Two compression types** (Sir Kane, 2016-12-06):
- Type 0: LZMA (slightly modified)
- Type 2: Modified LZ4

**LZ4 modification** (Ekey, 2016-12-08): Only the `dec32_table` changed:
- Standard: `{0, 1, 2, 1, 4, 4, 4, 4}`
- Ubisoft: `{0, 3, 2, 3, 0, 0, 0, 0}`

**LZMA presence**: Undoubtedly supported, but may not be used in PC version (Sir Kane, 2016-12-09). Files can be either compressed or uncompressed, with the latter mostly for really small files.

**Key constant for LZMA** (Sir Kane, 2016-12-07): `0xE6C` — useful for locating the modified LZMA code.

---

## XBG Model Format

### WD1 XBG Structure

**Basic format** (cra0/Chipicao, 2014): The `.xbg` and `.geom` formats are the same — Rick's tool saves unknown filenames as `.geom` and known as `.xbg` (Ekey, 2014-05-29).

**File extensions mapping** (Ekey, 2014-05-29):
```
CGeometryResource:      .glm/.dae → .xbg
CTextureResource:       .png/.dds → .xbt
CAnimationResource:     .mac → .mab
CSkeletonResource:      .skel.xml → .skeleton
CPhysResource:          .hkr → .hkx
CMaterialResource:      .material.xml → .material.bin
```

**Vertex format** (Wobble, 2014-07-08; cra0, 2014-07-08):
```
Vertex Format (Stride varies: 36, 40, 44 bytes)
position | uv0 | uv1 | blend_weights | blend_indices | normal | color | tangent | binormal
```

For stride 44:
```
POS → UV → UV2 → BoneWeights → BoneIndices → blankUnknown → VertexNormal → VertexColor → BiNormal → Tangents
```

**UV/Vertex Scaling** (Chipicao, 2014-08-05):
- `@offset 36: VPscale = [float, float]`
- `@offset 48: UVscale = [float, float]`
- Applied via HLSL `mad` function: `Vp = int16 * VPscale.y + VPscale.x`
- Only one set per file/model. Further UV scaling via material tiling.

**Per-model UV scaling** (Oleg, 2014-07-25): The xbg model is "decompression" scaling for UV for entire model, while material UV scaling (tiling) configures scale on per-material level.

**Vertex format flags** (from `vertexdeclaration.inc.fx`):
```
VERTEX_DECL_POSITIONCOMPRESSED
VERTEX_DECL_POSITIONFLOAT
VERTEX_DECL_UV0
VERTEX_DECL_UV1
```

**Character models**: Use "extra skinning" mode with 6 bone weights (Oleg, 2014-07-25). Limited to 255 bone indices in some tools.

**Per-vertex color**: NOT the color of mesh — it's per-vertex effects masking. E.g., wetness areas masked via vertex color channel (R on some shaders, G on others) (Oleg, 2014-07-26).

### WD2 XBG Changes

**Format version bump** (Wobble, 2016-12-14): WD2 XBG follows the old format but with a version bump and new bits. Old WD1 scripts won't load WD2 models.

**Blender importer** (volfin, 2017-01-14): WD2 Blender importer released (2.70+). Mesh only — no skeleton support. UV coordinates arbitrarily scaled.

**WD2 character heads**: Super large heads (5-6 MB) stored differently than regular meshes — likely very high rez heads for cutscenes with extra facial expression data (volfin, 2017-02-13).

---

## XBT Texture Format

**Structure** (cra0, 2014-05-28): XBT is a DDS with a custom header laid on top. To convert back: cut the header off the old XBT and attach to new DDS.

**XBT→DDS tool** (cra0, 2014-05-28): `xbt2dds` — https://github.com/cra0kalo/xbt2dds

**Normal maps** (Oleg, 2014-11-16): Green-alpha normal displacement maps. Green = displacement on X/U, Alpha = Y/V. Move alpha to red, save as GR16, load as bump map in DirectX editor — blue channel computed automatically.

**Tangent space maps** (HeliosAI, 2014-07-14): Originally orange because one channel stored in alpha. Swap channels to get purple form.

**ZOLA textures** (volfin, 2017-01-30): Not a standard graphics format. Likely GPU compute data (CUDA) from Nvidia research on dimensionality reduction. Probably not image data despite having c/high/med prefixes.

---

## Localization Files (.loc)

**Magic bytes** (cenkzenk, 2014-06-10): `53 4C 01 00` ("SL")

**Loc file structure** (rengareng, 2015-01-26):
```
uint magic;
ushort languageCode;
ushort count1;
uint stringFragmentsOffset;
struct { uint a, b; } t[count1];
// at stringFragmentsOffset:
int count2;  // includes zero-zero node
struct { short right, left; } nodes[count2-1];
```

**String fragments** (rengareng, 2015-01-26): Strings are stored as fragments in a table, indexed by byte values. 1-byte indexing works for small files; larger files need multi-byte indexing.

**Unpacking** (rengareng, 2019-01-25): Tool available for WD1 `main_english.loc`. Min/max IDs must be provided since loc files lack explicit IDs. Source code released 2019-05-29.

**QuickBMS script** (cenkzenk, 2014-06-10): Handles SL header parsing with type detection (0 = .unslz, 4 = .xcompress).

---

## Audio Formats

### SBAO Files

**Format** (Paul44, 2014-09-30): `.sbao` files are trailed with an additional 55-byte header. The game uses pre-compiled audio — conversion from SBAO to OGG fails for many files.

**Conversion tools** (filu23, 2014-06-02): Batch scripts for SBAO→MP3 and SBAO→WAV conversion.

**Sox usage** (Paul44, 2014-09-21): Use Sox v14.4 with "sndfile" library for better OGG conversion results.

### WD2 Audio

**In-memory audio streaming** (Paul44, 2014-09-21): Not all files within a DAT are compressed. Some DDS files in windy_city.dat are uncompressed — recompressing them causes the game to not display them.

---

## Material System

### Shader Resources (Ekey, 2014-05-29)

Key resource classes and their extensions:
- `CMaterialResource`: `.material.xml` → `.material.bin`
- `CGeometryResource`: `.glm/.dae` → `.xbg`
- `CTextureResource`: `.png/.dds` → `.xbt`
- `CAnimationResource`: `.mac` → `.mab`
- `CSkeletonResource`: `.skel.xml` → `.skeleton`
- `CPhysResource`: `.hkr` → `.hkx`

### Material Properties

**UV tiling** (Oleg, 2014-07-25): Material descriptors contain `NormalUVTiling`, `DetailUVTiling` etc. Per-model UV scaling gives "decompression" scaling for entire model; material UV scaling configures per-material level.

**DriverCloth shader** (Oleg, 2014-07-26):
- Diffuse texture premultiplied with `DiffuseColor1`
- Specular map: R = glossiness, G = colorization mask
- `DiffuseColor2` masked by SpecularMap.Green for multi-color clothes
- Second diffuse texture: grayscale premultiply/overlay for fabric simulation
- Normal map #2 can be displacement map for fabric texture bumps

**Per-vertex color masking** (Oleg, 2014-07-26): Vertex color is NOT mesh color — it's effects masking (wetness, etc.) via R or G channels depending on shader.

**Hair materials** (HeliosAI, 2014-07-14): Models use two UV channels for hair — same UV layout as alpha texture.

### WD2 Material Hashes

Key material field hashes (disastorm/redcomet, 2016-12):
- `D935FAD9` — graphickit_parts.lib reference
- `A849532C` — material of model part
- `7E23AAAE` — model path (binary hex string)
- `73642E5E` — graphickit part container
- `24436F9E` — texture of clothing item (refers to texturedb.lib)
- `0F43A933`, `54D14F92`, `4CAAD60F` — availability rules (set to FFFFFFFFFFFFFFFF to unlock)
- `E7575742` — category (FFFFFFFFFFFFFFFF = uncategorized)

### WD2 Clothing Category Values (disastorm, 2016-12)
```
DDD0721E02000080  NORMCORE
DED0721E02000080  BIKER
DFD0721E02000080  URBAN
E0D0721E02000080  HIPPIE
C61D21C801000080  HIPSTER
DBD0721E02000080  SUITS
DCD0721E02000080  HIP-HOP
```

---

## Modding Workflow

### WD1 Modding

**Extract/Repack workflow** (Rick, 2014-05-28):
1. Use Gibbed.Disrupt tools: http://svn.gib.me/public/disrupt/trunk
2. Builds: http://svn.gib.me/builds/disrupt
3. Requires XNA Framework: http://www.microsoft.com/en-us/download/details.aspx?id=20914

**Packing best practices** (Rick, 2014-06-04):
- Put modified files in a clean `patch.fat`/`patch.dat`
- Valid hash in FAT = valid filename
- Pack tool handles unknown files via `__UNKNOWN` directory structure
- Root directory given to Pack is NOT included as part of data path

**Filelist rebuilding** (Haoose, 2014-05-28):
- Create `projects\Watch Dogs\files\newfilenames.filelist` with new filenames
- Run `RebuildFileLists.exe`
- Progress tracked: reached 61% (192,362/310,356) by 2014-08-11

**In-game testing** (eycaramba, 2014-05-29): Modifications work without recompression — just use the pack command. Game loads modified archives directly.

### WD2 Modding

**Extraction tool** (Sir Kane, 2016-12-09): `WD2Extract` — handles LZ4 decompression and filename resolution.

**Packing tool** (Sir Kane, 2016-12-12): `WD2Pack` — repacks without compression (files remain uncompressed in archive).

**Full modding flow** (disastorm, 2016-12-13):
1. Extract `patch.fat`/`patch.dat`
2. Convert file to XML using Gibbed tools
3. Modify XML
4. Convert back to binary format
5. Repack `patch.fat`/`patch.dat` using WD2Pack
6. Place in game's `data_win64` directory
7. Run game with anti-cheat disabled

**Gibbed tools for WD2** (redcomet, 2016-12-14):
- Remove `binary objects` folder from `disrupt tool path\project\watch dogs` to handle `.lib` files
- Remove WD1 class definitions from `project/Watch Dogs/binary objects/files/gibbed` directory

**RML converter** (Ekey, 2016-12-14):
```bash
Gibbed.Disrupt.ConvertXml -xml music.rml music.xml
Gibbed.Disrupt.ConvertXml -rml music.xml music.rml
```

### WD2 Model Swapping

**Clothing swap workflow** (disastorm, 2016-12-14):
1. Open `items.lib` clothing XML for source item
2. Open `graphickit_models.lib` for target character
3. Copy the object section containing `D935FAD9` reference
4. Use grep on `graphickit_parts` directory to find what each section represents
5. Replace target sections with source sections

**Head replacement with animation** (redcomet, 2016-12-22):
- Edit `W2CH_MOD_nar_ava.avatar.xml` in `graphickit_models.lib`
- Replace head definition with target character's head definition
- Results in swapped face WITH animation

**Important notes** (disastorm/redcomet, 2016-12):
- Modified items won't appear in locker — must equip before adding modded patch
- Each item maps to one graphickit part
- After modifying patch, game must be restarted
- One item per graphickit part definition

### WD2 Custom Clothing Tutorial (CobraGamer, 2017-04-08)

**Files to edit:**
1. `items.lib` — create new clothing item with graphickit_part reference
2. `itemsshopproperties.lib` — define shop category and price
3. `shopcomponentsettings.lib` — make item appear in specific shop
4. `graphickit_models.lib` — contains model parts

**Key fields:**
- `389F6DA7` — item ID (hex A-F only)
- `D935FAD9` — graphickit_part reference
- `73280CDE` — link to item in items.lib
- `92AF09E1` — shop category
- `33E4CFD7` — price (hex→decimal)

---

## WD2 Archive File Distribution

**Final filelist status** (Ekey, 2016-12-15):
```
common.fat          → 5,779 resolved / 256 unknown
patch.fat           → 51,394 resolved / 4,901 unknown
san_francisco.fat   → 152,515 resolved / 48,495 unknown
san_francisco_preload.fat → 44,247 resolved / 42,466 unknown
shadersobj.fat      → 47,357 resolved / 2,977 unknown
sound.fat           → 50 resolved / 0 unknown
videos.fat          → 129 resolved / 1 unknown
```

**Models location**: `windy_city` and `windy_city_cache` DAT files (Ekey, 2014-06-13).

**Signs/billboards**: Major signs are `.hkx` files (Xecutioner, 2017-04-16). Some signs under `graphics/_geometries/building_dressing/signs`.

---

## Tools Reference

| Tool | Purpose | Author |
|------|---------|--------|
| Gibbed.Disrupt.Unpack/Pack | WD1 extraction/repacking | Rick (Gibbed) |
| WD2Extract / WD2Pack | WD2 extraction/repacking | Sir Kane |
| xbt2dds | XBT→DDS texture conversion | cra0 |
| wd_extractor | WD1 XMem decompression | cra0 |
| Blender 249 XBG Importer | WD1 model import | Szkaradek123 |
| io_scene_WD2 | WD2 Blender importer (2.70+) | volfin |
| ZModeler3 | WD1/WD2 model import/export | Oleg |
| Gibbed.Disrupt.ConvertBinaryObject | Binary↔XML conversion | Gibbed |
| Gibbed.Disrupt.ConvertXml | RML↔XML conversion | Ekey |
| WD_Repack batchfiles | Batch extraction/conversion | Paul44 |
| wd_loc.exe | WD1/WDL loc file unpacker | rengareng |
| RebuildFileLists.exe | Filename database builder | Gibbed |
| QuickBMS | Generic archive extraction | Aluigi |
| xbcompress / xbdecompress | XMem compression/decompression | Microsoft |

---

## WD2 Command-Line Parameters

(Ekey, 2016-12-17):
```
-nosplash          Skip intro videos
-nologfile         Disable log files
-nosound           Disable sound
-norender          Disable rendering
-nophysicstepworkerthread
-skiploggertool
-tf                Time factor
-txtlang           Language override
-spawnpos          Spawn position
-editorpc          Editor mode
```

---

## Feu Files (Flash)

(Ekey, 2016-12-17): `.feu` files are standard Flash files — just change header `UEF` to `FWS`. The WD2 boot sequence is a Flash file at `patch/ui/fire/bin/wd2bootsequence.feu`.

---

## Vehicle Modding

**Car on Demand** (neburas, 2016-12-18):
- Add new vehicles by creating `CarHackingRewards.Generic.Budjet.XXXX.xml` in `items.lib`
- All vehicles from Car on Demand treated as "player owned" — no wanted level
- Vehicle archetypes identified by GUID hashes (griffin02, 2017-01-24)

**Traffic density** (Paul44, 2017-03-05):
- Settings in `common.dat` → `trafficpatternselector.lib`
- XML settings allocated to specific map areas (blocks/streets)
- Density values = spawn timing (lower = more frequent spawns)
- File: `generated/databases/generic/trafficpatternselector.lib`

---

## Cross-Platform Notes

- WD1 formats largely same as Far Cry 3 (Dunia 2 engine base) — Rick, 2014-05-28
- WD2 uses same Disrupt engine but with format version bumps
- PS3 uses Deflate compression (different from PC/Xbox XMem)
- Xbox 360: GDek's tools may work but untested by original author
- WD Legion: Different XBG format, not compatible with WD1/WD2 importers
