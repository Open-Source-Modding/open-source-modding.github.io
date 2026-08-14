#!/usr/bin/env python3
"""Regenerate the synthetic PDB YAML for a Watch Dogs Legion DLL.

Merges per-build symbol sources into a PDB YAML that llvm-pdbutil yaml2pdb
can turn into a real PDB matching the DLL's RSDS:

  1. PE exports                          -> exports_<tag>.json
  2. engine/shaders method-trace strings -> extra_symbols_<tag>.json
  3. real Lua C API RVAs (main build)    -> hardcoded LUA_SYMS
  4. Lua engine native names (main build)-> lua_natives.json
  5. RTTI type descriptors               -> rtti_<tag>.json   (TpiStream)
  6. vftable virtual methods             -> vftable_symbols_<tag>.json

Default tag=main reproduces the dx12 pipeline (rebuild_pdb.sh). For the dx11
build use: --tag dx11 --guid {84E8439A-8917-4C7B-988D-AEF5139AA630} --age 2
--dll <dx11 path> --pdb <output.pdb>. Lua natives/LUA_SYMS are dx12-only.
"""
import argparse
import json
import os
import re
import subprocess

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT = os.path.dirname(SCRIPT_DIR)
SYMBOLS_DIR = os.path.join(REPO_ROOT, "symbols")
PDB_DIR = os.path.join(REPO_ROOT, "pdb")

DEFAULTS = dict(
    tag="main",
    dll=os.path.join(REPO_ROOT, "..", "DuniaDemo_clang_64_dx12.dll"),
    guid="{84BDE31D-654B-46B9-8B13-D60A27C33314}",
    age=2,
    pdb=os.path.join(PDB_DIR, "DuniaDemo_clang_64_dx12_steam_denuvo.pdb"),
    yaml=os.path.join(SCRIPT_DIR, "DuniaDemo_clang_64_dx12_steam_denuvo.pdb.yaml"),
)
IMAGE_BASE = 0x180000000


def src(tag, kind):
    main_names = {
        "sections": "sections_main.json",
        "exports": "exports_dunia.json",
        "extra": "extra_symbols.json",
        "rtti": "rtti_main.json",
        "vftable_symbols": "vftable_symbols.json",
        "lua_natives": "lua_natives.json",
    }
    if tag == "main":
        name = main_names[kind]
    else:
        suffix = {"extra": "extra_symbols", "sections": "sections",
                  "exports": "exports", "rtti": "rtti",
                  "vftable_symbols": "vftable_symbols",
                  "lua_natives": "lua_natives"}[kind]
        name = f"{suffix}_{tag}.json"
    return os.path.join(SYMBOLS_DIR, name)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tag", default=DEFAULTS["tag"])
    ap.add_argument("--dll", default=DEFAULTS["dll"])
    ap.add_argument("--guid", default=DEFAULTS["guid"])
    ap.add_argument("--age", type=int, default=DEFAULTS["age"])
    ap.add_argument("--pdb", default=DEFAULTS["pdb"])
    ap.add_argument("--yaml", default=DEFAULTS["yaml"])
    ap.add_argument("--vftable", default=None,
                    help="override vftable symbols JSON (e.g. an enriched_dev_<tag> file)")
    args = ap.parse_args()
    tag = args.tag

    sections = json.load(open(src(tag, "sections")))
    exports = json.load(open(src(tag, "exports")))
    extra = json.load(open(src(tag, "extra")))
    rtti = json.load(open(src(tag, "rtti")))
    vftables = json.load(open(args.vftable if args.vftable else src(tag, "vftable_symbols")))

    # Real Lua C API offsets (LoaderWDL Main.h) are hardcoded 1.6.3 dx12
    # addresses, so LUA_SYMS stays main-only. Lua native *names* are the same
    # engine in every build and come from lua_natives_<tag>.json instead.
    lua_syms = {}
    lua_natives = []
    if tag == "main":
        lua_syms = {
            "luaL_loadbuffer": 0x6907330,
            "lua_pcall": 0x690F590,
            "lua_tolstring": 0x690D8F0,
            "lua_newstate": 0x6907400,
            "lua_gettop": 0x6907570,
            "lua_settop": 0x6907590,
        }
    try:
        lua_natives = json.load(open(src(tag, "lua_natives")))
    except FileNotFoundError:
        lua_natives = []

    segs = []
    for s in sections:
        va = s.get("VirtualAddress")
        if va is None or va == 0 or not s.get("Name", ""):
            continue
        segs.append((s["Number"], s["Name"], va, s["VirtualSize"]))


    def rva_to_segment_offset(rva):
        va = IMAGE_BASE + rva
        for num, name, sva, ssize in segs:
            start = IMAGE_BASE + sva
            end = start + ssize
            if start <= va < end:
                return num, va - start
        return None, None


    def yaml_quote(s):
        return json.dumps(s)


    syms = []
    seen = set()
    for e in exports:
        syms.append((e["rva"], e["name"]))
        seen.add((e["rva"], e["name"]))
    for e in extra:
        if e.get("seg") == 11:
            continue
        key = (e["rva"], e["name"])
        if key in seen:
            continue
        seen.add(key)
        syms.append(key)
    for name, rva in lua_syms.items():
        key = (rva, name)
        if key in seen:
            continue
        seen.add(key)
        syms.append(key)
    for e in lua_natives:
        key = (e["rva"], e["name"])
        if key in seen:
            continue
        seen.add(key)
        syms.append(key)
    for e in vftables:
        key = (e["rva"], e["name"])
        if key in seen:
            continue
        seen.add(key)
        syms.append(key)

    recs = []
    for rva, name in syms:
        seg, off = rva_to_segment_offset(rva)
        if seg is None:
            continue
        recs.append(
            f'''    - Kind: S_PUB32
      PublicSym32:
        Flags: []
        Offset: 0x{off:X}
        Segment: {seg}
        Name: {yaml_quote(name)}'''
        )

    # Section characteristics: take from the sections JSON when present (dx11),
    # else fall back to llvm-readobj on the DLL (dx12 sections_main.json lacks
    # the field).
    char_by_num = {}
    for s in sections:
        if "Characteristics" in s:
            char_by_num[s["Number"]] = s["Characteristics"]
    if not char_by_num:
        out = subprocess.run(
            ["llvm-readobj", "--sections", args.dll], capture_output=True, text=True
        ).stdout
        for block in re.findall(r"Section \{([^}]*)\}", out):
            num = int(re.search(r"Number:\s*(\d+)", block).group(1))
            m = re.search(r"Characteristics", block)
            if not m:
                continue
            m2 = re.search(r"\(0x([0-9A-Fa-f]+)\)", block[block.find("Characteristics"):])
            if m2:
                char_by_num[num] = int(m2.group(1), 16)

    sec_lines = []
    for s in sections:
        va = s.get("VirtualAddress")
        name = s.get("Name", "")
        if va is None or va == 0 or not name:
            continue
        sec_lines.append(
            f'''  - Name: "{name}"
    VirtualSize: 0x{s['VirtualSize']:X}
    VirtualAddress: 0x{va:X}
    SizeOfRawData: 0x{s.get('RawDataSize', s['VirtualSize']):X}
    PointerToRawData: 0x{s.get('PointerToRawData', 0):X}
    Characteristics: 0x{char_by_num.get(s['Number'], 0):X}'''
        )

    tpi_recs = []
    for t in rtti:
        name = t["demangled"].replace('"', '\\"')
        tpi_recs.append(
            f'''    - Kind: LF_STRUCTURE
      Class:
        MemberCount: 0
        Options: [ None ]
        FieldList: 0
        Name: "{name}"
        UniqueName: "{t['mangled'].replace(chr(34), chr(92)+chr(34))}"
        DerivationList: 0
        VTableShape: 0
        Size: 0'''
        )

    yaml = f"""--- !Pdb
PdbStream:
  Age: {args.age}
  Guid: "{args.guid}"
  Signature: 0
  Version: VC70
TpiStream:
  Version: VC80
  Records:
{chr(10).join(tpi_recs)}
DbiStream:
  VerHeader: V70
  Age: {args.age}
  BuildNumber: 0
  PdbDllVersion: 0
  PdbDllRbld: 0
  Flags: 0x1
  MachineType: Amd64
  SectionHeaders:
{chr(10).join(sec_lines)}
PublicsStream:
  Records:
{chr(10).join(recs)}
"""
    open(args.yaml, "w").write(yaml)
    print(
        f"wrote {len(recs)} public symbols + {len(tpi_recs)} RTTI types "
        f"to {args.yaml} ({len(yaml):,} bytes)"
    )


if __name__ == "__main__":
    main()
