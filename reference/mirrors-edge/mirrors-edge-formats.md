# Mirror's Edge / Catalyst — Frostbite Asset & Audio Pipeline

> **Source**: XeNTaX forum knowledge extraction (2008–2017).
> Covers original Mirror's Edge (UE3, 2008) and **Mirror's Edge: Catalyst** (Frostbite, 2016).
> Catalyst threads contain the technically valuable content.

---

## 1. Mirror's Edge: Catalyst — Frostbite Engine Asset Pipeline

Catalyst uses **Frostbite** engine (Frostbite 3 lineage, same family as Battlefield 4/Hardline and Star Wars Battlefront). Audio and assets packed in shared Frostbite bundle/chunk model — community **Battlefield / Star Wars Battlefront dumpers** work with minor edits.

### Frostbite Asset Model (as used by Catalyst)

| Component | Description |
|-----------|-------------|
| **`.bundles`** | Container files holding game resources |
| **`.chunk`** | Raw resource blobs; filenames NOT meaningful. Resource names live in **EBX** metadata files, not chunk names |
| **EBX** | Object/type metadata database. Each sound/mesh/etc. described by an EBX file referencing chunk data by hash/GUID |

> "Names are in EBX files. That's OK." — daemon1, 2016-04-25

Chunks with no proper filenames are expected — you map them back through EBX.

### Dumper / Decoder Workflow

Working pipeline for Catalyst beta (2016):

1. **Extract bundles → chunks** using Battlefield 4 / Star Wars Battlefront dumper scripts (`bf4dumper.py`, `sw_dumper.py`). Community-edited `swbf_me-c_edit` version adapted specifically for Mirror's Edge Catalyst.
2. **Decode audio** with `fb3decoder.py` (Frostbite 3 audio decoder). Walks `.ebx` files, finds audio chunks, writes decoded `.wav` using **ealayer3** tool for EALayer3 codec (EA-proprietary MPEG layer-3 variant).

### Required Script Edits (brendan19, 2016-04-26)

```python
# sw_dumper.py
targetDirectory = r"<THE DIRECTORY WHERE YOU WANT THE FILES TO EXTRACT TO>"

# fb3decoder.py
dumpDirectory   = r"<THE DIRECTORY WHERE YOU EXTRACTED YOUR FILES>"
targetDirectory = r"<THE DIRECTORY WHERE YOU WANT THE DECODED .WAV FILES PLACED>"
# Download Zench's ealayer3 tool so the script can handle EALayer3:
ealayer3Path    = r"<THE DIRECTORY THAT HAS THE EALAYER3 TOOL>\ealayer3.exe"
```

### ealayer3 Tool Note

- **Zench's ealayer3** extracts multi-file chunks incorrectly — e.g., chunk with 5 sounds produces 15 files, 10 duplicates
- **daemon1's updated ealayer3** fixes this

### Frostbite EBX `numDict` (Field Type Lookup Table)

`fb3decoder.py` decodes EBX fields via `numDict` mapping 16-bit type codes to Python `struct` format chars:

| Code | Struct | Meaning |
|------|--------|---------|
| `0xC12D` | `Q` | uint64 (8 bytes) |
| `0xC0CD` | `B` | uint8 (1 byte) |
| `0x0035` | `I` | uint32 (4 bytes) |
| `0xC10D` | `I` | uint32 (4 bytes) |
| `0xC14D` | `d` | double (8 bytes) |
| `0xC0AD` | `?` | bool (1 byte) |
| `0xC0FD` | `i` | int32 (4 bytes) |
| `0xC0BD` | `b` | int8 (1 byte) |
| `0xC0ED` | `h` | int16 (2 bytes) |
| `0xC0DD` | `H` | uint16 (2 bytes) |
| `0xC13D` | `f` | float (4 bytes) |

**Gotcha**: Newer patches introduce field types absent from table. User hit `KeyError: 49437` (`0xC11D`) decoding non-audio EBX — code not in dict. daemon1's guidance: new type unrelated to audio, safely **delete non-audio EBX folders** and rerun.

Community guess for `0xC11D`: `("q", 8)` — int64 (8 bytes) (low-confidence).

### Practice Notes
- Full beta extraction yielded ~28.5 GB across `/bundles` and `/chunks`
- Successfully decoded named SFX from initial package; then full `cat`/`cas` pair set

---

## 2. Mirror's Edge (Original, 2008) — Textures

Original game uses UE3. Texture ripping via:
- **Ninja Ripper** (D3D runtime ripper)
- **Texmod** (D3D runtime ripper)

No native texture container RE documented in XeNTaX dump.

---

## 3. Tools Reference

| Tool | Purpose | Notes |
|------|---------|-------|
| `bf4dumper.py` / `sw_dumper.py` | BF4 / SWBF Frostbite bundle→chunk dumpers | |
| `swbf_me-c_edit` | Community-edited SWBF dumper for ME Catalyst | |
| `fb3decoder.py` | Frostbite 3 audio decoder (walks EBX, writes .wav) | |
| `ealayer3` (Zench / daemon1) | EALayer3 codec decoder; daemon1's version fixes multi-file chunks | |
| Ninja Ripper | D3D texture ripper (original ME) | |
| Texmod | D3D texture ripper (original ME) | |

---

## Key Facts
- **Catalyst** = Frostbite 3 (same family as BF4, SWBF) → existing dumpers work with edits
- **Bundle/Chunk/EBX** model: names in EBX, not chunk filenames
- Audio = **EALayer3** (EA-proprietary MPEG layer-3)
- **EBX `numDict`** field types: 11 known codes (uint64/uint8/uint32/double/bool/int32/int8/int16/uint16/float)
- New patches may add unknown field types → delete non-audio EBX folders to bypass
- **Original ME (UE3)**: textures via Ninja Ripper / Texmod only