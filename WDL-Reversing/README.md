# WDL-Reversing

Reverse engineering work for **Watch Dogs: Legion** (v1.6.3, Denuvo-packed).

WDL-specific RE hub. Cross-game Disrupt engine work lives in
`~/Documents/Code/re/Disrupt/`.

## Contents

| Path | What |
|------|------|
| `denuvo_analysis.md` | Denuvo internals + DenuvOwO hypervisor bypass RE |
| `wdl_engine_reference.md` | Engine reference: dinput8 proxy, shaders, GLM models, TAM materials, vehicle handling, Wwise audio, Lua scripting, Disrupt toolchain |
| `pdb_build/` | PDB reconstruction pipeline |
| `symbols/` | Extracted JSON artifacts per build tag |
| `pdb/` | Synthetic PDBs rebuilt from extracted symbols + RTTI |
| `dinput8.c` | IDA-decompiled dinput8 proxy DLL (raw reference) |
| `globals.lua` | Live `_G` dump from LoaderWDL (1,181 Lua native names) |
| `ubisoft_leak_contents.txt` | 2020 Ubisoft source leak directory listing (Disrupt toolchain) |
| `AGENTS.md` | Agent instructions |

## Building a PDB

```bash
cd pdb_build
python3 extract_dll_symbols.py <path-to-dll> <tag>   # produces ../symbols/<tag>_*.json
./rebuild_pdb.sh                                       # dx12 (default)
./rebuild_pdb_dx11.sh                                  # dx11
```

**Dependencies**: `python3`, `numpy`, `llvm-pdbutil`, `llvm-undname`

## License

MIT
