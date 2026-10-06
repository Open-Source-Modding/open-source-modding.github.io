# WD2 Model Hash Mappings

Model hashes map CRC64 identifiers to XBG geometry paths. These are the
`fileModel` values used in entity XML files (`WD2_*.xml`) to reference 3D models.

## Format

Each line maps a model hash to its XBG path:

```
XML_FILE MODEL_HASH CRC64_BYTES XBG_PATH
W2BK_Vegetation.xml 0x80000007defa4dd1.model 7D E4 FA DE ... graphics\_geometries\vegetation\static\american_aloe\american_aloe_kit_01_med_01_bk.xbg
```

Fields:
- **XML source file** — entity definition XML that references this model
- **Model hash** — `0x8000000` prefix + CRC64 of the XBG path (WD2 uses CRC64, not FNV1a32)
- **CRC64 bytes** — raw 8-byte hash in hex
- **XBG path** — backslash-separated path to the `.xbg` geometry file

## Hash scheme

WD2 model hashes use CRC64 with mask: `(hash & 0x1FFFFFFFFFFFFFFF) | 0xA000000000000000`.

```python
def model_hash(path: str) -> int:
 text = path.replace("/", "\\").lower()
 num = 14695981039346656037
 for c in text:
 num *= 1099511628211
 num ^= ord(c)
 return (num & 0x1FFFFFFFFFFFFFFF) | 0xA000000000000000
```

## Resolving a hash

Gibbed resolves the model hash from the path itself, so no hash table needs publishing: the
XBG paths are already in the WD2 archive file lists, and the hash is recomputed from the path
whenever a tool needs it.

The hash-to-path tables are a local dump. To rebuild them, read the `fileModel` values out of
the `WD2_*.xml` files you unpacked and index them against the `graphics\_geometries\...` paths
in the same unpack; each row is the entity XML that references the model, the `fileModel` hash,
the raw 8-byte hash and the XBG path.

## Related

- `WD2/UnusedCars.txt` — 141 unused/barely-used vehicle hashnames
- `WD2/wd2_depload_info.txt` — depload resource references (material paths, archetype IDs)
- [depload-format.md](../depload-format.md) — dependency preload manifest format
