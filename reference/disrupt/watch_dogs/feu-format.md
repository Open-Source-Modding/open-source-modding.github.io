# FEU Format (Flash UI)

`.feu` files are the Disrupt engine's UI definition format. They are **standard Flash SWF files** with a 3-byte magic override.

## Format

| Offset | Size | Value | Notes |
|--------|------|-------|-------|
| 0x00 | 3 | `UEF` | SWF signature replaced (`FWS` → `UEF`) |
| 0x03 | 1 | `0x08` | SWF version (Flash 8) |
| 0x04 | 4 | File length (LE u32) | Same as total file size |
| 0x08 | ... | Standard SWF body | Rectangle, frame rate, frame count, tags |

The rest of the file is **unmodified Flash SWF** — no encryption, no compression beyond standard SWF zlib. The only difference from a valid SWF is the first 3 bytes.

## Conversion

### FEU → Openable SWF

```bash
# Method 1: sed (Linux/Mac)
sed -i '1s/UEF/FWS/' file.feu

# Method 2: dd
dd if=file.feu of=file.swf bs=1 skip=0
printf 'FWS' | dd of=file.swf bs=1 count=3 conv=notrunc 2>/dev/null
```

### SWF → FEU (repack after editing)

```bash
sed -i '1s/FWS/UEF/' file.swf
mv file.swf file.feu
```

### Using hV_WD1ModdingKit (Windows)

The modding kit swaps `UEF` ↔ `GFX` (bytes `47 46 58`) instead of `FWS`. Both work with JPEXS. The kit is a .NET 4.7.2 VB app — does not run natively on Linux (needs `Microsoft.VisualBasic` runtime). Decompiled source: `hV_WD1ModdingKit_decompiled/` in the Disrupt RE repo.

### Using JPEXS Flash Decompiler

JPEXS (`ffdec-cli.jar` v22.0.1) works on Linux natively (Java required). It accepts files with `FWS`, `GFX`, or `UEF` prefixes — but `UEF` may produce warnings. Safest to rename to `.swf` with `FWS` header first.

```bash
# Export all resources
java -jar ffdec-cli.jar -export all output_dir file.swf

# Export scripts only
java -jar ffdec-cli.jar -export script output_dir file.swf

# Export specific frame range
java -jar ffdec-cli.jar -select 1-10 -export frame output_dir file.swf
```

## Content Structure

FEU files contain Flash movieclips with ActionScript 2 classes. The engine loads them as UI containers.

### ActionScript Class Hierarchy

```
MovieClip (Flash built-in)
  └── driver.LoadableContainer
        ├── driver.gamehud.Gh_CompanionBoard
        ├── driver.gamehud.Gh_* (other HUD elements)
        └── driver.* (menu classes)
```

Key classes from `gh_companionboard.feu`:

| Class | Purpose |
|-------|---------|
| `driver.LoadableContainer` | Base class for all UI panels. Handles event registration, DPS IDs, visibility toggling. |
| `driver.gamehud.Gh_CompanionBoard` | Digital billboard overlay. Registers `digitalBoard` / `digitalBoardTxt` DPS IDs. |
| `fire.managers.ExtendedEventRegister` | Singleton event bus. Wraps Flash's `_global.ExtendedEvents` for cross-panel communication. |
| `driver.utils.EventInfo` | Simple event descriptor (name + delegate). |

### SWF Tags

Typical FEU files contain:

- **DefineShape** (16 shapes per file, SVG-exportable)
- **DefineSprite** (nested animation containers, 1–58 frames each)
- **DefineEditText** (text fields with font references)
- **DoAction** (frame scripts, mostly `stop()`)
- **SymbolClass** (maps SWF symbols to AS2 class names)

### Resource References

FEU files reference engine resources via string paths embedded in the SWF:

```
UI\fonts\WD_Tech_Plain_Default.ffd      — font definitions
UI\supertextures\temp_idle.bfd          — texture atlases
UI\supertextures\lib_icons_greenalpha.bfd
```

Font names map to localization: `WD_Tech_Plain` (English/French/German/etc.), `DFGGothicP-W5_Japanese`, `DFPHeiMedium-B5_ChineseTrad`, `mdmorrism_ps3_korean`.

## File Inventory

### `Watch_Dogs/FEU/` (167 files)

All `.feu` UI definitions. Covers: HUD elements (`gh_*`), menus (`menu_*`), phone apps (`pda_*`), multiplayer (`mp_*`), system screens (`demostart`, `creditroll`), and more.

### `Watch_Dogs/FEU1/` (164 files)

Subset with minor differences. 46 files exist in FEU/ but not in FEU1/:

- **Menus**: `menu_start.feu`, `menu_shop.feu`, `menu_techtree.feu`, `menu_techtree2.feu`, `menu_hideout.feu`
- **Phone apps**: `pda_alarmclock.feu`, `pda_alieninvasion.feu`, `pda_citytraveller.feu`, `pdajournalapp.feu`, `pda_musicplayer.feu`, and 8 more
- **HUD**: `appshop.feu`, `citymap.feu`, `deadsecfieldguide.feu`, `gh_application.feu`, `gh_badge.feu`, `gh_battery.feu`, `gh_itemfeed.feu`, `gh_missionfeed.feu`, `gh_reputation.feu`
- **System**: `journal.feu`, `news.feu`, `monitoringapp.feu`, `menuuplay.feu`, `overlay_loadouteditor.feu`

## WD2 Notes

WD2 also uses `.feu` for its boot sequence (`patch/ui/fire/bin/wd2bootsequence.feu`) and UI screens. The format is identical — same `UEF\x08` magic, same Flash SWF body. (Source: Ekey, XeNTaX, 2016)

## Community Source

- Ekey (XeNTaX, 2016-12-17): `.feu` = Flash with `UEF` header
- dvdbluray (Discord, 2026-02-08): JPEXS editing tutorial for `gh_companionboard.feu`
- hardVatsuki: `hV_WD1ModdingKit.exe` converter (decompiled → `hV_WD1ModdingKit_decompiled/`)
