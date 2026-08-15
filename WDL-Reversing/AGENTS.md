# WD Legion RE

Reverse engineering for **Watch Dogs: Legion** (v1.6.3, Denuvo-packed). This repo
reconstructs synthetic PDBs from the stripped DLL via RTTI + string symbols.

Cross-game Disrupt engine work (tool RE/creation) lives in `~/Documents/Code/re/Disrupt/`.

**No binaries/compiled material in this repo.** Source DLLs sit next door in `re/WDL/`.

## PDB pipeline

Run from `pdb_build/`:

```
python3 extract_dll_symbols.py <path-to-dll> <tag>   # -> symbols/<kind>_<tag>.json
./rebuild_pdb.sh                                       # dx12 (tag=main)
./rebuild_pdb_dx11.sh                                  # dx11 (tag=dx11)
```

**Deps**: `python3`, `numpy`, `llvm-pdbutil`, `llvm-undname`

## Dev PDB enrichment (Ubisoft leak)

`~/Documents/Code/re/ubisoft/extracted/bin/` holds **non-stripped dev PDBs** from
the WD2-era engine (*orwell-editor-dev* = Disrupt Editor project codename):
- `DuniaDemo_rt64.pdb` (1.88GB) — **full non-stripped editor build**, 8,499,128 type records. Recovered from Troplo's Nexus mirror (see `re/ubisoft/AGENTS.md`) after the 7z extraction was truncated. GUID `{EA9CFCDC-20B7-4840-8F44-9F73C8A106AA}`, age 2, referenced by `mirror/bin/DuniaDemo_rt64.dll` (138MB bin). **This is the primary dev-type source now** — `--compact` extraction keeps 1,445,406 class/struct/union + referenced field-list records (1.27GB JSON, `dev_types_rt64.json`).
- `DuniaDemo_r64_dx12.pdb` (22MB) — 266,941 type records, 2,113 publics (stripped, incremental), used to be the dev source (`dev_fixed`)
- `DuniaDemo_r64.pdb` (19MB) — DX11 variant (stripped)
- `gear_core_win64DLLR.pdb`, `gxcore_release_x64.pdb`, etc. — Gear EAL / GX layer
- `DuniaDemo_rt64.pdb.truncated.bak` — the ~323MB truncated stub from the 7z leak (superseded)

WDL and WD2 both derive from this engine generation, so dev PDB **types+methods**
annotate shared SDK/engine classes (Bloomberg crash-report, storm, G4, hk*, ATL).

**Pipeline** (from `pdb_build/`):
1. `extract_dev_pdb_types.py <pdb> <dev_tag>` → `symbols/dev_types_<dev_tag>.json`,
   `dev_publics_<dev_tag>.json`, `dev_globals_<dev_tag>.json`. For the full rt64
   PDB, use `--compact` (drops enum/pointer/arglist noise + standalone field
   lists) → ~4.8m types, ~290s. `dev_fixed` = the 22MB r64_dx12 PDB, `rt64` = the
   full 1.88GB PDB.
2. `merge_dev_types.py <dev_tag> <game_tag>` → `enriched_vftable_<dev_tag>_<game_tag>.json`
   (renames recovered `Class::vf<slot>` placeholders to real method names), `enriched_types_<dev_tag>_<game_tag>.json`
3. `build_pdb_yaml.py --vftable <enriched...>` to use the renamed vftables

**WD2** (also uses this repo's toolkit): `./rebuild_pdb_wd2.sh` — extracts WD2's
`Disrupt_64.dll` (3,382 RTTI types, 8,886 vf methods), merges dev method names,
builds `pdb/Disrupt_64_wd2_synthetic.pdb` (~13.8k publics). WD2 RTTI is richer than
WDL's (no Lua natives; `globals.lua` absent → extraction auto-skips Lua). RSDS
GUID `{54015C37-3C0F-4A67-B31D-6C51D2CE0F0A}` / age 1 matches the DLL's debug
directory (`K:\bin\Disrupt_64.pdb`, identical in `bin/` and `bin_plus/`), so the
PDB links up. DLL has 0 exports; imports msvcr110, steam_api64, bink2w64,
gfsdk_shadowlib/txaa, libScePad + Win32. `DEV_TAG=rt64` (full editor PDB) gives
4,148 vfN→real renames / 621 classes; `DEV_TAG=dev_fixed` (stripped 22MB) gives 178.

**Enrichment yields (rt64, from the full non-stripped PDB — use this, not dev_fixed)**:
- WDL: **5,507 of 14,992 vfN slots renamed** (37%), 769 classes matched
- WD2: **4,148 of 9,708 renamed**, 621 classes matched
- (dev_fixed/r64_dx12, the stripped 22MB PDB, renamed only 156 WDL / 178 WD2 — engine
  classes don't overlap across the version gap, but rt64's complete type graph covers
  far more shared surface.)

**Extraction caveat**: `DuniaDemo_rt64.pdb` is the *orwell-editor-dev* engine — a
WD2-era *editor* build with the full D3D11/TypeScript surface included (8.5M types),
so most renames hit shared SDK/engine classes; game-specific classes (Dunia, gameplay)
still won't resolve. Class method order is taken from the dev PDB's LF_ONEMETHOD
lists, which assume the same vftable layout — renames that mismatch layout are a
small risk and can be pruned by reverting a slot to `vf<slot>`.

**Rebuild with rt64 dev data** (WD2 + WDL dx12):
```
DEV_TAG=rt64 ./rebuild_pdb_wd2.sh          # -> pdb/Disrupt_64_wd2_synthetic.pdb
python3 merge_dev_types.py rt64 main        # -> symbols/enriched_vftable_rt64_main.json
python3 build_pdb_yaml.py --tag main \
  --dll /home/selene/Documents/Code/re/WDL/DuniaDemo_clang_64_dx12.dll \
  --guid '{84BDE31D-654B-46B9-8B13-D60A27C33314}' --age 2 \
  --vftable ../symbols/enriched_vftable_rt64_main.json \
  --pdb ../pdb/DuniaDemo_clang_64_dx12_steam_denuvo.pdb \
  --yaml DuniaDemo_clang_64_dx12_steam_denuvo.pdb.rt64.yaml
```
Note: `extract_dev_pdb_types.py DuniaDemo_rt64.pdb rt64 --compact` must be run
once first (creates the 1.27GB `dev_types_rt64.json`).

**Pipeline**: `extract_dll_symbols.py` → JSON artifacts → `build_pdb_yaml.py` → YAML → `llvm-pdbutil yaml2pdb` → `.pdb`

### Symbol file naming

| tag | sections | exports | extra | rtti | vftable | lua_natives |
|-----|----------|---------|-------|------|---------|-------------|
| main | sections_main.json | exports_dunia.json | extra_symbols.json | rtti_main.json | vftable_symbols.json | lua_natives.json |
| dx11 | sections_dx11.json | exports_dx11.json | extra_symbols_dx11.json | rtti_dx11.json | vftable_symbols_dx11.json | lua_natives_dx11.json |
| dx12 | sections_dx12.json | exports_dx12.json | extra_symbols_dx12.json | rtti_dx12.json | vftable_symbols_dx12.json | lua_natives_dx12.json |

### PDB output

| Build | PDB | RSDS GUID |
|-------|-----|-----------|
| dx12 (main) | `pdb/DuniaDemo_clang_64_dx12_steam_denuvo.pdb` | `{84BDE31D-654B-46B9-8B13-D60A27C33314}` |
| dx11 | `pdb/DuniaDemo_clang_64_dx11_steam_denuvo.pdb` | `{84E8439A-8917-4C7B-988D-AEF5139AA630}` |

### What's in the PDB (17,865 public symbols)

- 356 PE exports (Wwise, NGX, RunGame)
- 1,745 C++ method/shader trace strings
- 6 Lua C-API RVAs (dx12-only, from LoaderWDL `Main.h`)
- 766 Lua native names (from `globals.lua`, matched as NUL-terminated strings in DLL)
- 14,992 virtual-method symbols from RTTI vftables (1,542 vftables / 1,232 classes)
- TpiStream: 1,913 `LF_STRUCTURE` type descriptors (empty shells)

### RTTI: intact despite Denuvo

Full chain survives in the 1.6.3 main DLL:
- 1,933 type-descriptor strings (`.?AV...@@`) in `.ecode`
- 1,577 valid COLs (24 bytes, x64, signature=1)
- 1,542 vftables in `.ecode`

COL format (x64): `+0 sig(1), +4 offset, +8 cdOffset, +0xC pTypeDescriptor, +0x10 pClassDescriptor, +0x14 pSelf`. TypeDescriptor is 16 bytes before the name string.

### dx11 build quirks

Method-trace strings in `.rodata` (not `.ecode`), `.text` is 5 bytes — code in `.idata`/`.rdata`/`.yPD`. RTTI counts identical to dx12.

## Repo contents

| Path | Content |
|------|---------|
| `denuvo_analysis.md` | Denuvo internals (Connor Jay Dunn) + DenuvOwO hypervisor bypass (0xPacman) |
| `wdl_engine_reference.md` | Engine reference: dinput8 proxy, shaders, GLM/TAM formats, vehicle handling, Wwise, Lua, Disrupt toolchain |
| `dinput8.c` | IDA-decompiled dinput8 proxy DLL (mod loader entrypoint) |
| `globals.lua` | Live `_G` dump from LoaderWDL (1,181 Lua native names) |
| `symbols/` | Extracted JSON artifacts per build tag |
| `pdb/` | Rebuilt synthetic PDBs |
| `pdb_build/` | PDB reconstruction scripts |

## AOB signature methodology

For cross-build compatibility: extract first 48 bytes of function at known RVA
in both builds, diff position-by-position — identical bytes stay literal,
differing bytes (RIP-relative displacements, relative calls) become wildcards
(`??`). Validate each sig matches **exactly once** at true function start.

## Related projects

| Path | Connection |
|------|------------|
| `~/Documents/Code/re/WDL/` | Source DLLs, Ghidra project (`WDL.gpr`), IDA analysis files |
| `~/Documents/Modding/WDL/` | Game data mods; `globals.lua` consumed from here |
| `~/Documents/Code/game-tools/LoaderWDL/` | dinput8 proxy + Lua script hook; consumes Lua C-API RVAs; `extractGtable.lua` produced `globals.lua` |
| `~/Documents/Code/game-tools/blender-io-xbg/` | Blender addon for .xbg/.glm import/export |
| `~/Documents/Code/re/Disrupt/` | Cross-game Disrupt engine RE hub |
