# Watch Dogs Wii U — RPX → ELF Conversion & IDA Loading

> Companion to [Wii U Debug Symbols](wii-u-debug-symbols.md). Turns the raw `duniademo.rpx`
> into a standard ELF32 PPC binary that Ghidra/IDA/readelf can consume, plus an IDC script
> that applies all 292,224 demangled names.

## Problem

The RPX uses Cafe OS ELF extensions that break every standard tool:

- `readelf` (GNU and devkitPPC) reports **corrupt** symbols/section names — it cannot
  handle Cafe OS zlib-compressed sections.
- `powerpc-eabi-objcopy` fails with "string table corrupt" / "file format not recognized".
- IDA 9.3 rejects `e_type 0xFE01` and loads the file as a **flat binary at base 0**,
  so a VA-based PDB never resolves (VA−file_offset differs per section).

## Key Discovery: ALL file-backed sections are zlib-compressed

It is not just SYMTAB/STRTAB. **Every** `PROGBITS` section stores its data as a
zlib deflate stream preceded by a **4-byte big-endian header = DECOMPRESSED size**.

`sh_size` in the ELF section header is the **compressed** size. The true memory size
comes from the matching **SECTION symbol's `st_size`** in the symtab:

| Section | Index | sh_size (compressed) | decompressed | source of truth |
|---------|-------|----------------------|--------------|-----------------|
| .text | 2 | 0x2356D9B | **0x4EC5E44** (82.6 MB) | section sym st_size |
| .rodata | 4 | 0x1AF21A | 0x689690 | section sym st_size |
| .data | 5 | 0x15246 | 0x2AEB0 | section sym st_size |
| .module_id | 6 | 0x34 | 0x38 | section sym st_size |
| .sdata | 8 | 0x1C | 0x4C0 | section sym st_size |
| .bss | 7 | — (NOBITS) | 0x1C14A8 | section sym st_size |
| .sbss | 9 | — (NOBITS) | 0x1C | section sym st_size |
| .symtab | 31 | 0x210A35 | 0x4D6870 | — |
| .strtab | 32 | 0x366AE2 | 0x1D993CF | — |
| .shstrtab | 33 | 0xA7 | 0x1A3 | — |

### Cafe OS section types (as seen in section headers)

| Type ID | Mapped to | Sections |
|---------|-----------|----------|
| `0x80000001` | SHT_PROGBITS | .syscall, .text, .fexports, .rodata, .data, .module_id, .sdata |
| `0x80000002` | SHT_PROGBITS | .fimport_*/ .dimport_* (relocation/import stubs) |
| `0x80000003` | SHT_NOTE | .rela.* leftovers |
| `0x80000004` | SHT_NOTE | — |

Section flags must be masked `& 0x3` to get the ELF-visible permission bits.

### Load addresses (these ARE valid ELF VAs — keep them)

```
.syscall  0x02000000   .text  0x02000020   .fexports 0xC0000000
.rodata   0x10000000   .data  0x106896A0   .module_id 0x106B4560
.bss      0x106B4600   .sdata 0x10875AC0   .sbss     0x10875F80
.fimports 0xC226FE80-0xC2276080
```

## The Converter

Script: `Watch_Dogs Wii U Retail/rpx_to_elf.py` (in the Disrupt RE hub).

Algorithm:

1. Parse ELF32 BE header; read 36 section headers at `0x40` (40 bytes each).
2. For each file-backed section: skip 4-byte size header, `zlib.decompress` the rest.
   NOBITS sections (.bss/.sbss) are zero-filled at their VA.
3. Emit a fresh ELF32 PPC **EXEC**: `[ELF hdr @0][phdrs @0x34][section data][shdrs at end]`.
   Reserve `phdr_off + 32*7` bytes up front so the program header table does not
   overwrite the first sections.
4. Build 7 `PT_LOAD` segments:
   - text (secs 1-2, R+X), rodata (4, R), data (5-6, RW), bss (7, RW NOBITS),
     sdata (8-9, RW), fexports (3, R), fimports (14-30, RW)
5. Keep the **original section order** so `st_shndx` values stay valid.
6. Map Cafe OS types as above; entry point stays `0x6E67A08`.

Output: `duniademo.elf` — 142,748,535 bytes. Validated with both `powerpc-eabi-readelf`
and system `readelf`: ELF32 BE, Type EXEC, Machine PowerPC, 317,019 FUNC symbols all
parse with correct names/indices. `.text` lands at file offset `0x140`; entry bytes
`7c0802a6 9421ffe8` (mflr r0 / stwu r1 prologue) match the original decompressed .text
byte-for-byte.

## Applying Demangled Names in IDA

The GHS/Metrowerks demangler (92% coverage of 317K mangled symbols) produced
`duniademo_symbols_demangled.txt` in format:

```
0x02000020 180 __ct__24SEngineNetworkParametersFv → SEngineNetworkParameters::SEngineNetworkParameters(void)
```

`Watch_Dogs Wii U Retail/duniademo_symbols.idc` (41 MB) is a generated IDC script with
**292,224** `MakeNameEx(addr, demangled_name, SN_NOWARN|SN_NOCHECK)` calls — exactly one
per successfully demangled symbol. The remaining ~24.8K (plain C functions, opaque
`__T<digits>` compiler IDs, deep `__tm__` STL templates) are left with their mangled
names, which the ELF symtab already carries.

Usage:

1. Load `duniademo.elf` in IDA 9.3 (it now parses as a proper ELF — no flat binary).
2. `File → Script file…` → run `duniademo_symbols.idc`.
3. Optionally enable GHS demangler via `ida-config` / processor options.

The older `duniademo.pdb` (27 MB, 245K S_PUB32 via `llvm-pdbutil`) is superseded by the
ELF symtab — the PDB only resolved 23.5% of symbols in a flat-loaded IDB because of the
base-address mismatch.

## Demangler Notes

- `__ct__` ctor, `__dt__` dtor, `__nw__` new, `__dl__` delete, `__nwa__` new[],
  `__dla__` delete[], `__vc__` `operator[]`, `__cl__` `operator()`, `__as__` `operator=`,
  `__eq__` `operator==`.
- `Q2_`/`Q3_` = qualified names (`SetAffinity__Q2_4Gear10ThreadBaseFi` →
  `Gear::ThreadBase::SetAffinity(int)`).
- `__CPR<n>__` = copy/ref wrapper (strip).
- `__tm__` = template marker (deeply nested `Z` template params remain undemangled).
- Type codes: `v` void, `i` int, `l` long, `c` char, `f` float, `d` double, `b` bool,
  `P` pointer, `PC` const pointer, `R` ref, `RC` const ref, `Ul/Us/Uc/Ui` unsigned.
- Length-prefixed class names: `<digits><name>` (e.g. `24SEngineNetworkParameters`).

## Files

| File | Purpose |
|------|---------|
| `duniademo.rpx` | Original Cafe OS executable (in `extracted/GM…/code/`) |
| `duniademo.elf` | Converted standard ELF32 PPC (142 MB) |
| `duniademo_symbols.idc` | 292,224 demangled renames for IDA |
| `duniademo_symbols.txt` | Raw mangled symbols (317,019 FUNC) |
| `duniademo_symbols_demangled.txt` | Demangled symbols |
| `rpx_to_elf.py` | RPX → ELF converter |
| `rpx_symbols.py` / `rpx_to_pdb.py` | Older extract/PDB pipeline (superseded) |