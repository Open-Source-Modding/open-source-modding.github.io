# Mirror's Edge — XeNTaX Forum Knowledge

Extracted from XeNTaX forum discussions (2008–2017). Covers the original
Mirror's Edge (UE3-era, 2008) and **Mirror's Edge: Catalyst** (Frostbite,
2016). The Catalyst threads are the technically valuable content — the original
game thread is largely off-topic chatter.

---

## 1. Mirror's Edge: Catalyst — Frostbite Engine Asset Pipeline

### Overview

Mirror's Edge: Catalyst uses the **Frostbite** engine (Frostbite 3 lineage,
same family as Battlefield 4/Hardline and Star Wars Battlefront). Its audio and
assets are packed in the shared Frostbite bundle/chunk model, so the community
**Battlefield / Star Wars Battlefront dumpers** work with minor edits.

*(daemon1, brendan19, 2016-04)*

### Frostbite asset model (as used by Catalyst)

- **`.bundles`** — the container files holding game resources.
- **`.chunk`** files — raw resource blobs; filenames are **not** meaningful.
  Resource names live in **EBX** metadata files, not the chunk names.
  *(daemon1, 2016-04-25: "Names are in EBX files. That's OK.")*
- **EBX** — object/type metadata database. Each sound/mesh/etc. is described
  by an EBX file that references chunk data by hash/GUID.
- Chunks with no proper filenames are expected — you map them back through EBX.

### Dumper / decoder workflow

The working pipeline for Catalyst beta (2016):

1. **Extract bundles → chunks** using a Battlefield 4 / Star Wars Battlefront
   dumper script (`bf4dumper.py`, `sw_dumper.py`). The community-edited
   `swbf_me-c_edit` version is adapted specifically for Mirror's Edge Catalyst.
   *(brendan19, daemon1, 2016-04-25)*
2. **Decode audio** with `fb3decoder.py` (the "FB3" Frostbite 3 audio decoder
   script). It walks the `.ebx` files, finds the audio chunks, and writes
   decoded `.wav` files using the **ealayer3** tool for EALayer3 codec
   (the EA-proprietary MPEG layer-3 variant used by Frostbite).
   *(daemon1, 2016-04-25)*

**Required script edits** (from brendan19, 2016-04-26):

```python
# sw_dumper.py
targetDirectory = r"<THE DIRECTORY WHERE YOU WANT THE FILES TO EXTRACT TO>"

# fb3decoder.py
dumpDirectory   = r"<THE DIRECTORY WHERE YOU EXTRACTED YOUR FILES>"
targetDirectory = r"<THE DIRECTORY WHERE YOU WANT THE DECODED .WAV FILES PLACED>"
# Download Zench's ealayer3 tool so the script can handle EALayer3:
ealayer3Path    = r"<THE DIRECTORY THAT HAS THE EALAYER3 TOOL>\ealayer3.exe"
```

**Tool note (ealayer3 versions):**
- Zench's ealayer3 extracts **multi-file chunks incorrectly** — e.g. a chunk
  with 5 sounds produces 15 files, 10 of which are duplicates.
- daemon1's **updated** ealayer3 fixes this. *(daemon1, 2016-04-26)*

### Frostbite EBX `numDict` (field type lookup table)

`fb3decoder.py` decodes EBX fields via a `numDict` mapping of 16-bit type codes
to Python `struct` format chars. The known entries (from the script, 2016):

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

**Gotcha:** Newer patches introduce field types absent from the table. A user
hit `KeyError: 49437` (`0xC11D`) while decoding non-audio EBX files — the code
was **not** in the dict. daemon1's guidance: the new type is unrelated to audio,
so you can safely **delete non-audio EBX folders** and rerun.
*(Hello, daemon1, kkdf2, 2016-06/08)*

- `kkdf2` (2016-08-29) suggested `0xC11D: ("q", 8)` — an int64 (8 bytes) — as a
  likely fix (low-confidence community guess).

### Practice notes

- Extraction of the full beta yielded ~28.5 GB across `/bundles` and `/chunks`.
  *(Hello, 2016-06-11)*
- Successfully decoded named SFX from the initial package; then decoded the
  full `cat`/`cas` pair set. *(brendan19, 2016-04-25)*

---

## 2. Mirror's Edge Textures (original game)

### Ripping approach

- Textures ripped using **Ninja Ripper** and **Texmod** (runtime D3D rippers)
  for the original Mirror's Edge. *(kellne, 2017-06-05)*
- Both tools work on the UE3-era original; no native texture container
  reverse-engineering is documented in this dump.

---

## 3. Tools Mentioned

| Tool | Purpose |
|------|---------|
| `bf4dumper.py` / `sw_dumper.py` | Battlefield 4 / Star Wars Battlefront Frostbite bundle→chunk dumpers |
| `swbf_me-c_edit` | Community-edited SWBF dumper adapted for Mirror's Edge Catalyst |
| `fb3decoder.py` | Frostbite 3 audio decoder (walks EBX, writes .wav) |
| `ealayer3` (Zench / daemon1) | EALayer3 codec decoder; daemon1's version fixes multi-file chunks |
| Ninja Ripper | D3D texture ripper (original ME) |
| Texmod | D3D texture ripper (original ME) |

---

*Source: XeNTaX forum threads t=3245 (Mirror's Edge), t=14259 (Mirror's Edge
Catalyst), t=16367 (Mirror's Edge Textures).*
