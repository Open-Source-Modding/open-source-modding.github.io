# Watch Dogs Wii U — RPX Binary & Symbol Table

> Source: `duniademo.rpx` from Watch Dogs (USA) Wii U retail disc
> Extracted via JWUDTool with USA WUD key `5a5b5fef63fade1603497770fa43a394`

## RPX Binary Format

| Field | Value |
|-------|-------|
| File | `duniademo.rpx` (51,080,768 bytes) |
| Format | ELF32 big-endian, Cafe OS (ABI `0xCA`) |
| Type | `0xFE01` (Cafe OS executable) |
| Machine | `0x0014` (Cafe OS PowerPC) |
| Entry point | `0x06E67A08` |
| Build path | `F:\master\code\duniademo.rpx` |
| Sections | 36 ELF section headers |

### Cafe OS Section Types

Cafe OS extends ELF with custom section types:

| Type ID | Name | Purpose |
|---------|------|---------|
| `0x80000001` | CAFE_PROLOG | Function prolog data |
| `0x80000002` | CAFE_EXIDX | Exception handling index |
| `0x80000003` | CAFE_UNWIND | Stack unwinding info |
| `0x80000004` | CAFE_EXTAB | Exception table |

## Symbol Table

The RPX contains a **massive symbol table** -- 317,063 symbols with full mangled C++ names. This is unusual for a retail build; the Wii U version retained symbols that were stripped from PS4, Xbox 360, and PC retail releases.

### Compression

Both SYMTAB and STRTAB are zlib-compressed with a 4-byte header:

```
Offset  Size  Description
0x00    4     Header (skip this)
0x04    var   zlib deflate data
```

**Decompression (Python):**

```python
import zlib
with open('duniademo.rpx', 'rb') as f:
    f.seek(offset)  # SYMTAB: 0x1C5180, STRTAB: 0x3D5BC0
    raw = f.read(size)  # SYMTAB: 0x210A35, STRTAB: 0x366AE2
    data = zlib.decompress(raw[4:])  # Skip 4-byte header
```

### Section Layout

| Section | Index | Offset | Size (compressed) | Size (decompressed) |
|---------|-------|--------|-------------------|---------------------|
| SYMTAB | 31 | `0x1C5180` | 2,165,301 bytes | 5,073,008 bytes |
| STRTAB | 32 | `0x3D5BC0` | 3,566,242 bytes | 31,036,367 bytes |
| SH strtab | 33 | `0x73C6C0` | 167 bytes | 167 bytes (uncompressed) |

### Symbol Counts

| Type | Count |
|------|-------|
| FUNC | 317,019 |
| OBJECT | 11 |
| SECTION | 32 |
| NOTYPE | 1 |
| **Total** | **317,063** |

| Bind | Count |
|------|-------|
| LOCAL | 110,040 |
| GLOBAL | 135,633 |
| WEAK | 71,389 |

### Symbol Format

Each symbol entry is 16 bytes (ELF32_Sym, big-endian):

```
Offset  Size  Field
0x00    4     st_name (index into STRTAB)
0x04    4     st_value (virtual address)
0x08    4     st_size (symbol size in bytes)
0x0C    1     st_info (bind << 4 | type)
0x0D    1     st_other (visibility)
0x0E    2     st_shndx (section index)
```

`st_info` encoding: `bind = st_info >> 4`, `type = st_info & 0xF`

Symbol types: NOTYPE(0), OBJECT(1), FUNC(2), SECTION(3), FILE(4)
Symbol binds: LOCAL(0), GLOBAL(1), WEAK(2)

### Notable Symbol Categories

| Category | Count | Examples |
|----------|-------|---------|
| Game engine | ~500+ | `SEngineParameters`, `CDuniaEngineInitBase` |
| BehaviorTree tasks | ~155+ | `BTCallRegisterNodeCTask`, `RegisterProperties` |
| Sound (SND) | ~107+ | `SND_fn_b*`, `SND_fn_v*` |
| XML parsing | ~42+ | `XmlNodeRef`, `XmlConstNodeRef`, `XML_DefaultAllocator` |
| Ref counting | ~58+ | `CRefCounted`, `CRefCountWith` |
| Threading | ~30+ | `CMessageQueue`, `CTaskSynchLock`, `Gear::ThreadBase` |
| Bink video | ~28+ | `Bink*` functions |
| Memory | ~29+ | `MemHelper`, `MEMAllocFromDefaultHeap` |
| Archive | 1+ | `CCryArchiveManagerNative` |
| Expat XML | ~80+ | `normal_*`, `utf8_*`, `little2_*`, `big2_*` encoding functions |

### Why This Matters

The PC, PS4, and Xbox 360 retail builds of Watch Dogs have their symbol tables **completely stripped**. The Wii U retail build retained the full SYMTAB/STRTAB, making it the **best source** for reverse engineering the Disrupt engine's C++ class hierarchy.

No DWARF debug sections (`.debug_*`) are present -- Cafe OS uses SYMTAB/STRTAB as its sole symbol mechanism. The symbol table IS the debug info.

## Parser Script

The Python parser used to extract these symbols is at:
`/tmp/opencode/rpx_parse_v2.py`

Full symbol CSV (317,063 rows) at:
`/tmp/opencode/rpx_symbols.csv`

## Extraction Steps

1. Obtain USA WUD key: `5a5b5fef63fade1603497770fa43a394`
2. Decrypt `.wux` with JWUDTool (or equivalent WUD decryptor)
3. Locate `GM0005000010142000000000000000/code/duniademo.rpx`
4. Parse ELF32 header (big-endian, 52 bytes)
5. Read 36 section headers at offset `0x40` (40 bytes each)
6. Decompress SYMTAB (section 31) and STRTAB (section 32) with zlib
7. Parse ELF32_Sym entries (16 bytes each) from decompressed SYMTAB
8. Resolve names from decompressed STRTAB
