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

| Type ID | Name | Purpose |
|---------|------|---------|
| `0x80000001` | CAFE_PROLOG | Function prolog data |
| `0x80000002` | CAFE_EXIDX | Exception handling index |
| `0x80000003` | CAFE_UNWIND | Stack unwinding info |
| `0x80000004` | CAFE_EXTAB | Exception table |

## Symbol Table

317,063 symbols with full mangled C++ names. The Wii U retail build retained symbols stripped from PS4/X360/PC.

### Compression

Both SYMTAB and STRTAB are zlib-compressed with a 4-byte header:

```python
import zlib
with open('duniademo.rpx', 'rb') as f:
    f.seek(offset)  # SYMTAB: 0x1C5180, STRTAB: 0x3D5BC0
    raw = f.read(size)  # SYMTAB: 0x210A35, STRTAB: 0x366AE2
    data = zlib.decompress(raw[4:])
```

### Section Layout

| Section | Index | Offset | Compressed | Decompressed |
|---------|-------|--------|------------|--------------|
| SYMTAB | 31 | `0x1C5180` | 2,165,301 | 5,073,008 |
| STRTAB | 32 | `0x3D5BC0` | 3,566,242 | 31,036,367 |
| SH strtab | 33 | `0x73C6C0` | 167 | 167 |

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

### Symbol Format (ELF32_Sym, 16 bytes, big-endian)

| Offset | Size | Field |
|--------|------|-------|
| 0x00 | 4 | st_name (STRTAB index) |
| 0x04 | 4 | st_value (virtual address) |
| 0x08 | 4 | st_size (bytes) |
| 0x0C | 1 | st_info (bind << 4 | type) |
| 0x0D | 1 | st_other (visibility) |
| 0x0E | 2 | st_shndx (section index) |

### Notable Symbol Categories

- Game engine: SEngineParameters, CDuniaEngineInitBase (~500+)
- BehaviorTree tasks: BTCallRegisterNodeCTask (~155+)
- Sound: SND_fn_b*, SND_fn_v* (~107+)
- XML: XmlNodeRef, XmlConstNodeRef (~42+)
- Ref counting: CRefCounted, CRefCountWith (~58+)
- Threading: CMessageQueue, CTaskSynchLock (~30+)
- Bink video: Bink* (~28+)
- Memory: MemHelper, MEMAllocFromDefaultHeap (~29+)
- Expat XML: normal_*, utf8_*, little2_*, big2_* (~80+)

### Why This Matters

PC/PS4/X360 retail builds have symbols **completely stripped**. The Wii U build retained the full SYMTAB/STRTAB, making it the **best source** for Disrupt engine RE. No DWARF sections -- Cafe OS uses SYMTAB/STRTAB as its sole symbol mechanism.

## Parser

- Script: `/tmp/opencode/rpx_parse_v2.py`
- CSV: `/tmp/opencode/rpx_symbols.csv` (317,063 rows)

## Extraction Steps

1. Decrypt `.wux` with JWUDTool using USA WUD key
2. Locate `GM0005000010142000000000000000/code/duniademo.rpx`
3. Parse ELF32 header (52 bytes, big-endian)
4. Read 36 section headers at offset `0x40` (40 bytes each)
5. Decompress SYMTAB (section 31) and STRTAB (section 32) with zlib
6. Parse ELF32_Sym entries (16 bytes) from decompressed SYMTAB
7. Resolve names from decompressed STRTAB
