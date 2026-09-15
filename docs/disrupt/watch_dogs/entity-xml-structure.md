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
