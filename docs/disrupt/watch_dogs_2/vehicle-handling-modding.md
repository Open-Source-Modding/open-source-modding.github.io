# Watch Dogs 2 — Vehicle Handling Modding

> **Source**: Community tutorial (rootCBR, Discord). Documents the full pipeline for editing `.handling.bin` vehicle parameters.

## Pipeline

### 1. Unpack Game Archives

Use `WD2Extract.exe` (or Gibbed.Disrupt tools) to extract `.fat` archives:

```bash
WD2Extract.exe common.dat common.fat
```

Vehicle handling files live in:
```
common/drive/car/bin/<vehicle>.handling.bin
common/drive/bike/bin/<vehicle>.handling.bin
```

### 2. Convert .handling.bin → XML

Use `DriveConverter.exe` (rootCBR, GitHub) to convert binary handling files to XML:

```bash
DriveConverter.exe <vehicle>.handling.bin
```

This produces `<vehicle>.handling.xml` with editable parameters.

### 3. Edit XML Values

XML structure:
```xml
<Parameter Key="16" Name="Yaw" Type="float" Value="5337.107421875" />
```

**Rules:**
- Only edit **Value** attributes (floats)
- Never touch Key, Name, or Type attributes
- Keep XML structure intact

### 4. Repack XML → .handling.bin

DriveConverter converts XML back to binary:
```bash
DriveConverter.exe <vehicle>.handling.xml
```

### 5. Pack into Patch Archive

Use `WD2Pack.exe` to pack modified files into `patch3.fat`/`patch3.dat`:

```bash
WD2Pack.exe patch3.fat patch3.dat <modified_folder>
```

Or use the provided batch scripts:
- `pack.bat` — WD2Pack.exe wrapper
- `unpack.bat` — WD2Extract.exe wrapper

### 6. Install

Place `patch3.fat` and `patch3.dat` into `data_win64/` in your WD2 installation.

## Key Differences from WD1

| Feature | WD1 | WD2 |
|---------|-----|-----|
| Patch archive | `patch.fat`/`patch.dat` | `patch3.fat`/`patch3.dat` |
| Pack tool | Gibbed.Disrupt.Pack | WD2Pack.exe |
| Extract tool | Gibbed.WatchDogs.Unpack | WD2Extract.exe |

## Notes

- WD2's handling system is more complex than WD1 — more parameters per vehicle
- Batch editing across multiple vehicles is possible with scripts
- DriveConverter handles the binary↔XML conversion automatically
- The `patch3` naming convention is WD2-specific; WD1 uses `patch`

## Related

- [Archive Load Priority](../watch_dogs/archive-priorities.md) — per-game archive order
- [Hashing (CRC32 / CRC64_WD2)](../watch_dogs/hashing.md) — hash algorithms
- [WD2 Custom Item/Clothing Tutorial](../watch_dogs/wd2-item-clothing-tutorial.md) — another WD2 modding workflow
