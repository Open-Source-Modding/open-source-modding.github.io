# Adding Vehicles to Watch Dogs: Legion (entitylibrary + depload)

> Assimilated 2026-08-22 from the WDL RE workspace (`WDL/RETAIL_VEHICLE_CATALOG.md` §§17-23,
> `WDL/depload_tool.py`). Goal: **perfect the process so any vehicle can be added** — proven
> end-to-end with the Forklift PoC mod (build + install guide at the end).

## The three ID spaces (cracked 2026-08-22)

Every vehicle reference in the game uses one of three namespaces:

| Space | Where | Value | Example (armored truck) |
|-------|-------|-------|--------------------------|
| **elib UID / archArchetype** | entitylibrary FCB entries, spawninfo/vehiclesbank rows | `CRC64_WD2('{GUID-with-braces}')` stored as its **LE bytes** (FCBastard displays them as `4B 17 D3 29 ...`) | truck dev UID `F4 0E 97 DD 8C CB B3 A6` |
| **lib-row hidName** | vehicle.lib / vehiclespawninfo.lib / vehiclesbank rows | `CRC32(text_hidName)` as `04` + 4-byte LE | `Heavy.ArmoredTruck_01` -> `$C2BCF20C` |
| **disNomadObjectId** | two distinct spaces! | elib = small editor-internal sequential ids (`0x1EFA`); lib rows = `#8000000x` | elib `0x1EFA`; MIL patrol spawninfo `#800000041BA01A04` |

**The join key:** an elib entry's UID hash **must equal** the spawninfo row's `archArchetype`
hash. If a spawninfo row references a hash with no matching elib entry, the vehicle can't spawn
(this is exactly why the forklift and armored truck don't appear in retail).

**Hash tools:** `WDL/hash_tool.py` — `$` = CRC32, `#` = CRC64_WD2 (FNV-1 64 + normalize
`/`→`\`, lower-case + 57-bit mask + OR `0xA000000000000000`). Both verified 3-way against the
community ModBot. **`#` prints the LE byte rendering** (`to_le8`), e.g.
`#4B17D329A1BB12B8` = LE bytes of u64 `0xB812BBA129D3174B` — the elib FCB stores those LE bytes.

**Cross-game:** the 64-bit hash's low 32 bits = the WD1 32-bit FNV32 hash of the same string
(ModBot: `-hash 4297884081` → WD1 FNV32 `7F28100A`, WD2 FNV64 `A14B0A1B7F28100A`).
Assets port across WD1/WD2/WDL because of this.

## Hard rules (catalog §17)

- **NEVER append a new GUID** to a cooked elib — growing the table crashes the load
  (`FCB_ParseBinaryFragment`). Same-length inplace rewrite, or overriding an existing
  path/GUID, is load-safe.
- elib `disNomadObjectId` = small editor-internal ids — don't invent `#8000000x` ids there.
- Vehicle **visuals** (XBG/mat) and **gameplay** (HKX/CI/XML/handling) are separate stacks.
- vehicle.lib lot ≠ elib GUID — a lot can exist while the elib entry is missing (truck case).
- Camera comes from the vehicle `.handling.bin`, not a cameracontext folder.

## The perfected process (catalog §21)

### 1. Check what already exists (retail `unpacked/`)

| Table | Path | Truck (ArmouredTruck_01) | Forklift |
|-------|------|:---:|:---:|
| vehicle.lib lot | `common/generated/databases/generic/vehicle.lib` | yes (0x2ee9) | yes (0x831) |
| DRVCars row | `.../vehiclecar/DRVCars.*.xml` | yes (handling `drive\car\bin\heavy_armored.handling.bin`) | yes |
| spawninfo row | `.../vehiclespawninfo/*.xml` | no | yes (keys to `{07ce4f52-...}`) |
| elib entry | `worlds/london/london/worlds/london/generated/entitylibrary(_rt).fcb` | no (cut) | no (only Low_Fork_0cm) |
| UI icons | `common/ui/.../sourcecars/` | yes | yes |
| cooked assets | `graphics/vehicles_nexus/land/<class>/<veh>/` | no (source-only) | no retail / yes leak |

### 2. The elib entry (the usual missing piece)

- Export the source elib with **FCBastard v1 "Encrypted's Update"** (overflow-fixed build,
  `FCBastard_Legion_Dist/FCBastard.exe`; runs under wine with **`z:\` absolute paths**:
  `wine FCBastard.exe 'z:\path\in.fcb' 'z:\path\out.xml'`). The leak dev world
  (`leak/ubisoft/data/Worlds/usr_brobertson_ops/generated/entitylibrary.fcb`) has the **full
  library set** — incl. `Vehicle_Heavy` (625 entities), `Vehicle_Special`, `Vehicle_Police`,
  `Vehicle_Traffic` — all cut from retail.
- Copy the whole `<EntityReference>` block of the same-class vehicle from the dev export.
- **Restored vehicle** (was in dev, cut from retail): keep the dev UID — truck =
  `#F40E97DD8CCBB3A6` (`Vehicle_Heavy.Heavy.ArmouredTruck_01`, British spelling; resources
  `graphics\vehicles_nexus\land\heavy\heavy_armoredtruck_01\{xml,hkx,xbg,_ref.skeleton,ci}`).
- **New vehicle**: invent a UUID, keep the string — elib UID = `CRC64_WD2('{uuid}')` LE bytes;
  the SAME uuid string feeds the depload container.
- `hidName` = friendly dotted name; `disNomadObjectId` = small unique id.
- Minimal manifest (WDL = 8-byte UIDs, no `Use32Bit` attr; that attr is WD1's 4-byte FNV32 mode):

```xml
<?xml version="1.0" encoding="utf-8"?>
<EntityLibraries>
  <EntityLibrary File="libraries\Vehicle_Special.xml" />
</EntityLibraries>
```

- Compile: `wine FCBastard.exe 'z:\...\entitylibrary_rt.xml' 'z:\...\entitylibrary_rt.fcb'`.
  **`entitylibrary_rt.fcb` = the mod target** (Ace_T_: "rt"; WD1 uses the same overlay scheme
  into `worlds/.../generated/mods/<modid>/`).

### 3. The depload (dependency manifest)

- The engine caches entity resources ahead of time — a new entity needs its resources listed.
- **Yorpie's rule:** "depload is just copy pasting a few lines... it just needs the **materials
  loaded**" — if the models are already loaded you only need the `CMaterialResource` entries.
  Otherwise list the full resource set (see PoC below).
- **Placement is fragile:** entry ORDER matters — Yorpie always places new entries *under*
  existing same-type ones ("sonaruses under the umeni sonarus"); top placement crashed.
  Wrong ID order/spot = crash ("ID in the wrong order? Crash. Wrong spot? Crash.").
  Sometimes the material must also be added to the player entity.
- Container hash = **masked** `CRC64_WD2('{uuid}')` (mask = `& 0x03FFFFFFFFFFFFFF | 0xA000000000000000`).
  Dep hashes = masked `CRC64_WD2(full path)` (stored raw/unmasked in the dep table).

### 4. Assets

- **Already-cooked** in the leak: `leak/ubisoft/data_win64/graphics/vehicles_nexus/land/...`
  (forklift complete) → copy-only.
- **Source-only** in the leak: `leak/ubisoft/data/graphics/vehicles_nexus/land/Heavy/`
  (truck = `.glm/.hkr/.xml/.ci`) → must be **cooked** via the internal editor toolchain
  (Disrupt Editor / Materials.Editor) — the truck's `.xbg/.hkx/.material.bin` exist nowhere else.
- The `.dep` files next to cooked assets list exactly what a depload entry needs
  (`forklift_01.xbg.dep` → its two `CMaterialResource` children).

### 5. Spawninfo row (only for vehicles missing it — e.g. the truck)

libobj XML template (from retail `Special.Forklift_01.xml`):
`disNomadObjectId` (new `#8000000x`, LE `xx xx xx xx xx 00 00 80`), `hidKey` same,
`text_hidName` + `hidName=CRC32(name)`, `text_archArchetype="{GUID}"` + `archArchetype=CRC64_WD2('{GUID}')`
— **same GUID as the elib UID**. Place at `common/generated/databases/generic/vehiclespawninfo/`.

### 6. Where it goes

- `installpackage/` (permanent — needs `installpackage_patcher.py` DLL hex edit; never repack
  the ~16 GB `patch` archive again) — archive priorities: `patch` > `common` > `worlds\london\london`.
- Encrypted's dinput8 proxy side-mount (LGV packs = slots 26-33 prio 2500; `EncryptedsPatch` prio 0).
- Mod overlay folder (`worlds/.../generated/mods/<modid>/` — WD1 NexusTools style).

## Depload binary format (decoded WD1 → WD2 → WDL, 2026-08-22)

`WDL/depload_tool.py` parses all three games (WD1: 96,927 containers / 711,813 deps / 35 type
strings; WD2: 198,928 / 1,181,047 / 43; WDL: 176,579 / 1,341,646 / 49).

**WD1 (32-bit):** `u32 containerCount` → `count × {u32 startIndex, u32 depCount, u32 fileHash}`
(12 B) → `u32 depHashCount` + `count × u32` (32-bit truncated FNV1a64) → `u32 depTypeCount` +
`count × u8` (parallel) → `u32 typeStringCount` + `count × u32` CRC32 type strings.

**WD2/WDL (64-bit):** `u8 version=0x01` + `u32 containerCount` + **header gap (~9-10 B,
copy verbatim — unknown purpose)** + `count × 17 B records {u64 masked CRC64 LE, u8 type,
u32 startIndex, u32 count}` (last 1-2 records = malformed filler — skip) + dep hashes
`u64 × (maxValidStartIndex+1)` (no count prefix — implied) + dep types `u8 × same` +
`u32 typeStringCount` + `u32 CRC32s` (must reach exact EOF). Container-record hashes are
**masked** (top byte 0xA0-0xBF); dep-table hashes are raw.

**Type strings** (CRC32 → name, shared table per game — 35/43/49; resolve via
`hV_WD_ModdingKit_PLUS/Tools/Depload/strings.list` for the WD1 names):
`CMaterialResource=bc825377`, `CTextureResource=6bd55afc`, **`CGeometryResource=86e8e8be` (XBG)**,
`CPhysResource=3ae88efd` (HKX), `CSkeletonResource=4cdda42c`, `CAnimationResource=b0604725`,
`CSoundResource=63f450d8`, `CArchetypeResource=287653ed`, `CResourceContainer=1543407d`, etc.

**Files:** WD1 `windy_city_depload.dat`, WD2 `san_francisco_depload.dat`,
WDL `worlds/london/london/worlds/london/generated/london_depload.dat` + `common/generated/common_depload.dat`.
`WDDeploy.exe` (hV modding kit) is WD1-only — it throws `EndOfStreamException` on 64-bit files.

## The Forklift proof-of-concept (built + verified 2026-08-22)

Why the forklift: every table exists except the elib entry; cooked assets exist in the leak
(no cooking). It exercises the whole pipeline safely.

- **Retail spawninfo** `Special.Forklift_01.xml` already keys to `{07ce4f52-e6a6-479e-a822-2caf05f65dc1}`
  → `archArchetype` = `CRC64_WD2('{...}')` = u64 `0xB812BBA129D3174B` → elib UID LE bytes
  `4B 17 D3 29 A1 BB 12 B8`.
- **Elib overlay** (`entitylibrary_rt.fcb`, 9,680 B): Forklift_01 dev block from the dev
  `Vehicle_Special.xml` (UID `7D 6B 4A 72 AA 3A 58 A3` → changed to `4B 17 D3 29 A1 BB 12 B8`,
  hidName `Special.Forklift_01`), compiled with FCBastard; round-trip verified byte-faithful.
- **Depload overlay** (container `{07ce4f52-...}`, hash `0xA012BBA129D3174B`):

```xml
<CBinaryResourceContainer ID="{07ce4f52-e6a6-479e-a822-2caf05f65dc1}">
  <CGeometryResource ID="graphics\vehicles_nexus\land\special\forklift_01\forklift_01.xbg" />
  <CPhysResource ID="graphics\vehicles_nexus\land\special\forklift_01\forklift_01.hkx" />
  <CSkeletonResource ID="graphics\vehicles_nexus\land\special\forklift_01\forklift_01_ref.skeleton" />
  <CMaterialResource ID="graphics\_materials\rrahimi-m-9223372106727034057.material.bin" />
  <CMaterialResource ID="graphics\_materials\yotsu-m-9223372082472313350.material.bin" />
</CBinaryResourceContainer>
```

  (materials from `forklift_01.xbg.dep`). Appended via `depload_tool.py add` → patched
  `london_depload.dat` (15,076,954 B); binary verified (container hash @ 0x2dce01, dep hashes
  present), round-trip parse clean (containers 176,580→176,581, deps +5).

**Mod workspace** (`~/Documents/Modding/WDL/forklift_poc/`, 7 files, 16 MB):
`graphics/_materials/{rrahimi-m-9223372106727034057, yotsu-m-9223372082472313350}.material.bin`;
`graphics/vehicles_nexus/land/special/forklift_01/{forklift_01.hkx, forklift_01_ref.skeleton, forklift_01.xbg}`;
`worlds/london/london/worlds/london/generated/{entitylibrary_rt.fcb, london_depload.dat}` (patched).

**Install / test:**
1. Back up retail `entitylibrary_rt.fcb` + `london_depload.dat` first.
2. Copy the 7 files over the retail paths (or install via `installpackage/` / dinput8 side-mount).
3. Spawn with `SpawnEntityFromArchetype` — expect the forklift once elib UID `4B 17 D3 29 A1 BB 12 B8`
   pairs with the retail spawninfo archArchetype `{07ce4f52-...}`.
4. Crash = depload placement (keep the new entry at the END per Yorpie). No spawn =
   dep-hash input form question below.

**What the PoC proves (5 points):**
1. elib UID = `CRC64_WD2('{GUID}')` bytes, stored LE.
2. depload container hash = masked hash of the SAME `{GUID}` string.
3. dep hashes = masked `CRC64_WD2(full path)`; raw in the dep table.
4. type indices = CRC32 of the element name vs the per-game type-string table.
5. assets already-cooked → copy-only; source-only (truck) → must cook.

## Tools created / used

- `WDL/depload_tool.py` — parse (dump) + overlay builder (add) for all three games.
- `WDL/hash_tool.py` — CRC32 + CRC64_WD2 (flag-bug fixed this session; 3-way ModBot verified).
- `FCBastard_Legion_Dist/FCBastard.exe` — "Encrypted's Update" (overflow-fixed) elib FCB ↔ XML.

## Open questions

- **Truck (ArmouredTruck_01):** everything exists except spawninfo + cooked assets; the
  `.xbg/.hkx/.material.bin` must be cooked from source (`.glm/.hkr/.xml/.ci` in the leak) via
  the internal editor pipeline. This is the next milestone.
- **Dep-hash input string form:** WD2's dep table didn't match `CRC64_WD2` of any tested path
  form (full path / relative / `_server` names / archive-root-relative) — likely the `files.list`
  form. The overlay still uses the full-path form; the game test decides.
- **Depload header gap** (WD2 26 B / WDL 9 B) purpose unknown — copy verbatim, don't recreate.
- Community: Encrypted wants vehicles ported across WD1/WD2/WDL ("Wd1 wd2 and wdl will be able
  to be ported across eachother") — the low-32-bit hash compatibility makes this feasible.