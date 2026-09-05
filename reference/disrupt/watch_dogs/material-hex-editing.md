# Watch Dogs Material Hex Editing

> **Source**: guru3D Forums thread "Watch_Dogs Material Hex Editing" (mlleemiles, 2017-10-23) + Discord logs from WD1 modding community (2020-2026).
> **Cross-reference**: Material descriptors (85 XML files) → [`materialdescriptors/`](materialdescriptors/); XeNTaX material section → [XeNTaX Watch Dogs knowledge §8–9](../../unsorted/xentax-watchdogs-knowledge.md)

---

## Tools Required

- **Razor's Data Conversion Utility** (1drv.ms link) — String ↔ CRC32 ↔ BinHex conversion
- **Hex Editor** (Hex Workshop recommended) — for raw `.material.bin` editing
- **Disrupt Editor Material Tool** (fan-made, WD1 origin) — XML ↔ .material.bin converter (works for WD1/WD2/WDL)
- **ZModeler3** — import/export with material support (Oleg's plugin)

---

## Material.bin Structure

Opening a `.material.bin` in a hex editor shows:

1. **Header strings** — shader name, parameter names (at start of file, text view)
2. **Parameter blocks** — each parameter has:
   - 4-byte type
   - 4-byte parameter hash (CRC32 of parameter name)
   - Value (size depends on type)

### Parameter Value Types

| Type Byte | Meaning | Size |
|-----------|---------|------|
| `01` | Float | 4 bytes |
| `03` | Integer | 4 bytes |
| `04` | Boolean | 1 byte |
| `06` | Vector2 | 8 bytes |
| `07` | Vector3 | 12 bytes |
| `08` | Vector4 | 16 bytes |

---

## Hex Editing Workflow

### 1. Identify Shader & Parameters

```text
1. Open .material.bin in hex editor
2. Read shader name at file start (e.g., "Character")
3. Go to common_unpack\engine\shader\materialdescriptors
4. Find corresponding XML (e.g., Character.xml)
5. Search for "parameterprovider" section
```

### 2. Find Parameter Hash

```text
1. Copy parameter name from XML (e.g., "RimLightPower")
2. Paste into Razor tool input
3. Convert String → CRC32
4. Copy output, paste back to input
5. Convert BinHex → CRC32 → final hash
```

**Note**: Some materials reference `Generic.xml` as well.

### 3. Locate & Edit Value in Hex

```text
1. Search for the CRC32 hash in hex editor
2. The value follows the hash
3. Example structure:
   01000000  51A7AF2D  00000041
   ^type    ^paramHash ^value
4. Type 01 = Float
5. Use Hex Workshop base converter to convert float ↔ hex
```

### 4. Common Parameters to Edit

| Parameter | Effect |
|-----------|--------|
| `RimLightPower` | Rim lighting intensity |
| `DiffuseColor1` / `DiffuseColor2` | Base color (Vector4) |
| `SpecularPower` | Glossiness/shininess |
| `Reflectance` | Reflection intensity |
| `NormalIntensity` | Normal map strength |
| `DiffuseTexture1` / `DiffuseTexture2` | Texture path hashes |

---

## Known Issues & Workarounds

### Disrupt Editor Material Converter Bug

**Problem**: Converting `.material.bin` → XML → `.material.bin` produces wrong file length (last bytes differ). Causes random crashes on load/reload.

**Root cause**: Float → binhex conversion precision loss (discovered by みる97, 2020-05-31).

**Workarounds**:
1. **Hex edit directly** — keep original, export edited, diff bytes, port only changed values back
2. **Manual process** (Crank Beige, 2020-05-31):
   - Get original material
   - Export to XML, convert back to bin, compare bytes
   - Assign new diffuse textures in XML
   - Compare edited XML→bin vs original
   - Port only diffuse changes to original binary

### Material Converter Crashes (2020-05-31)

- Round-trip conversion (bin→xml→bin) changes trailing bytes
- Second xml conversion throws error
- Not consistent — some materials work, others crash

### ZModeler3 Material Issues

- Sometimes cuts `\graphics\materials` → `raphics\materials` in material group name
- Invisible models in-game = material path truncated
- Check materials tab before export

### WD2 Character Heads (volfin, 2017)

- 5-6 MB each, stored differently from regular meshes
- Likely high-rez cutscene heads with facial expression data
- Standard importers don't handle them correctly

---

## ZOLA Textures (volfin, 2017-01-30)

- **Not standard image format**
- Likely GPU compute data (CUDA) from Nvidia dimensionality reduction research
- Have `c/` `high/` `med/` prefixes but probably not image data
- Do not attempt to convert with standard tools

---

## Per-Vertex Color (Oleg, 2014)

**NOT mesh color** — it's effects masking:
- Wetness areas masked via vertex color channel (R on some shaders, G on others)
- Used for weathering, wetness, damage effects

---

## Material Naming Matters (Oleg, ZModeler forum)

- Material named `VehicleName_CarPaint_01` → auto-applies game's variative paint color scheme
- Settings in material XML are ignored when name matches paint scheme pattern

---

## Damage Property (ZModeler forum)

- User property `Damage` = comma-separated object names
- **Position = damage-part index** (`,,,TireFront_L` → L=#4, R=#5)
- Indices coded in collision `.hkx` — retain original indices or damage maps wrong

---

## Cross-Game Notes

| Game | Material Converter | Notes |
|------|-------------------|-------|
| WD1 | Disrupt Editor (fan) | Works |
| WD2 | Disrupt Editor (fan) | Works (Cobra modified source) |
| WDL | Disrupt Editor (fan) | Material converter works for all 3 |

The fan-made Disrupt Editor material converter originated for WD1 but works across all three games.