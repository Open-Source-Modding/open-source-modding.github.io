# Entity XML Structure (Disrupt Engine)

Entity prototypes define game objects (vehicles, characters, props) as XML.
Extracted from `.fcb` binary objects via Gibbed.Disrupt.ConvertBinaryObject or
FCBastard.

## Header Fields

```xml
<EntityPrototype UID="#A3E9ECF28981E387">
 <Entity
 disNomadObjectId="#000000000000293B"
 hidName="Vehicle_Brawler.Brawler.Brawler_Civ_Truck.Brawler_Civ_Truck_Military"
 text_hidEntityClass="CEntity"
 hidEntityClass="$CEntity"
 ...>
```

| Field | Type | Description |
|-------|------|-------------|
| `UID` | `#` hex | Entity prototype unique identifier (CRC64_WD2 hash) |
| `disNomadObjectId` | `#` hex | Sequential numeric ID from binary export — **NOT a hash** |
| `hidName` | string | Dot-separated entity path (input for hash computation) |
| `text_hidEntityClass` | string | Entity class name |
| `hidEntityClass` | `$` hex | CRC32 hash of entity class name |

## Hash Field Conventions

| Prefix | Algorithm | Example | Use |
|--------|-----------|---------|-----|
| `$` | CRC32 | `$CEntity`, `$Vehicle` | Entity class, group IDs |
| `#` | CRC64_WD2 | `#AE95FBB4F84A9F78` | File paths, resource IDs, UIDs |
| `#FFFFFFFFFFFFFFFF` | — | Special value | "No value" / null hash |
| `text_*` | — | `text_fileName="..."` | Human-readable input for corresponding hash field |

## Components

Entities contain components that define behavior:

```xml
<Components>
 <CFileDescriptorComponent
 text_fileName="graphics\vehicles_nexus\land\heavy\heavy_armoredtruck_01\Heavy_ArmoredTruck_01.xml"
 fileName="#AE95FBB4F84A9F78" />
 <CVehicleCarPhysComponent
 text_hidResourceId="...\Heavy_ArmoredTruck_01.hkx"
 hidResourceId="#AECCD9B4F879A7C6"
 ... />
 ...
</Components>
```

Each component has:
- `hidHasAliasName="0"` — whether this is an alias
- `text_*` fields — human-readable strings
- `#` fields — CRC64_WD2 hashes of the corresponding strings

## Important Notes

- `disNomadObjectId` is **sequential** (e.g. `0x293B`, `0x2942`), assigned by
 the editor — it is NOT a hash of the entity name
- `UID` is the entity's unique identifier, used by `SpawnEntityFromArchetype`
- Hashes in hand-edited XMLs may be incorrect if computed before the algorithm
 was known — always verify with `hash_tool.py`
- Entity paths use dots as separators: `Vehicle_Brawler.Brawler.Brawler_Civ_Truck`
- File paths use backslashes: `graphics\vehicles_nexus\land\...`

## Graphickit Part XML Fields

Entity prototypes for characters and clothing contain `graphickit_parts` components with additional fields beyond the base entity structure:

| Field | Type | Description |
|-------|------|-------------|
| `hidVersion` | hash | Version identifier for the part |
| `selPoseDefinitionId` | hash | Pose definition reference (body position for clothing) |
| `selMaterialOverridesId` | hash | Material override set reference |
| `oneAnchor` | bool | Whether this part has a single anchor point |
| `BoneName` | string | Target bone for attachment |
| `Vector3` translation | float×3 | Position offset |
| `Vector3` rotation | float×3 | Rotation offset |
| `Vector3` scale | float×3 | Scale offset |

Source: OCR of graphickit part XML dumps (`wd1_modelinfo_1.txt`).

## Community editing workflows

### Editing a `.lib` library (SilverStar, 2025-11-12)

Libraries live at `generated\databases\generic` (e.g. `Graphickit_models.lib` in patch):

1. Drag the `.lib` over Gibbed's `ConvertBinaryObject` → unpacks to a header XML + folder.
2. Put mod files in the folder and overwrite.
3. In the header XML, strip the `.lib` from the name it's Gibbed'd against: the folder
   keeps `Graphickit_models.lib.xml`, but the header entry becomes `Graphickit_models.xml`.
4. Delete the original `.lib` first — **Gibbed can't overwrite** — then drag the header
   XML back over `ConvertBinaryObject` to repack; put it back in the patch file.

### Which files break when edited

- **90% of the time a `.lib` file is the culprit** — "a common one to break for
  beginners" (SilverStar, 2025-11-12); unpack with ConvertBinaryObject, diff the two
  XML/header sets and make them fit.
- The other 9% is `entity_library.fcb`-related — FCBastard does basically the same
  process as Gibbed's CBO tool.
- **Deploadify XML bug** (SilverStar, 2025-10-14): a depload conversion can die with
  `System.Xml.XmlException '<', hexadecimal value 0x3C` because a resource path
  containing an ampersand (`...adpaint_db&...` in `windy_city.dat`) isn't escaped —
  grab the intermediate `.converted.xml` and fix that line manually before rerunning.

## Graphic-kit lib recipe (WDL, from the game's `patch` archive)

The graphic-kit library lives in `generated\databases\generic` inside the patch archive. To edit it:

1. Drag the `.lib` onto `ConvertBinaryObject` — this unpacks the library and produces a `header.xml` plus a folder of member objects.
2. Drop the replacement asset into that folder and overwrite the existing file.
3. In `header.xml`, **strip the `.lib` from the entry name that refers to the folder** — `Graphickit_models.lib.xml` becomes `Graphickit_models.xml` — then repack with `ConvertBinaryObject`.

The same header-name rule applies to any `.lib` unpacked this way; forgetting it makes the game reject the rebuilt library.
