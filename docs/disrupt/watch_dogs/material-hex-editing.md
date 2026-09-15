# Watch Dogs Material Hex Editing — Community Tips

> **Source**: guru3D Forums thread "Watch_Dogs Material Hex Editing" (mlleemiles, 2017-10-23) + Discord logs from WD1 modding community (2020-2026).
> **For the authoritative binary layout, type codes, and editing workflow**, see [`material-bin-format.md`](material-bin-format.md).
> **Cross-reference**: Material descriptors (85 XML files) → [`materialdescriptors/`](materialdescriptors/); XeNTaX material section → [XeNTaX Watch Dogs knowledge §8–9](xentax-watchdogs-knowledge.md)

---

## Tools Required

- **Razor's Data Conversion Utility** (1drv.ms link) — String ↔ CRC32 ↔ BinHex conversion
- **Hex Editor** (Hex Workshop recommended) — for raw `.material.bin` editing
- **Disrupt Editor Material Tool** (fan-made, WD1 origin) — XML ↔ .material.bin converter (works for WD1/WD2/WDL)
- **ZModeler3** — import/export with material support (Oleg's plugin)

For the full editing workflow using `ConvertMaterials.exe`, see [material-bin-format.md § Authoritative Editing Workflow](material-bin-format.md#authoritative-editing-workflow).

---

## Known Issues & Workarounds

### Disrupt Editor Material Converter Bug (fixed in later builds)

**Problem**: Older `ConvertMaterials.exe` writes wrong header offset values on XML→BIN round-trip, causing random in-game crashes. Discovered by みる97 (2020-05-31); fixed by qstlijku in later converter builds. See [material-bin-format.md § Crash Bug](material-bin-format.md#crash-bug) for the technical details.

**Workarounds** (if stuck on an old converter build):
1. **Hex edit directly** — keep original, export edited, diff bytes, port only changed values back
2. **Manual process** (Crank Beige, 2020-05-31):
   - Get original material
   - Export to XML, convert back to bin, compare bytes
   - Assign new diffuse textures in XML
   - Compare edited XML→bin vs original
   - Port only diffuse changes to original binary
3. **Run `res/material_sizefixer.1sc`** from the converter's own directory to fix the header values after the fact

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
