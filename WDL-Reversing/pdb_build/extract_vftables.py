#!/usr/bin/env python3
"""Recover vftables + virtual methods from intact RTTI in the main DLL.

The 1.6.3 main DLL keeps the full RTTI chain despite packing:
  - TypeDescriptor strings  (.?AV...@@ / .?AU...@@)
  - Complete Object Locators (24-byte, x64, signature == 1, pSelf == own rva)
  - vftables (COL-pointer slot + code pointers into exec sections)

Outputs ../symbols/vftable_symbols.json: {rva, name} entries for
Class::vftable and Class::vf<slot>, demangled via llvm-undname.
Run from pdb_build/ with the DLL at ../DuniaDemo_clang_64_dx12.dll.
"""
import json
import os
import re
import struct
import subprocess
import numpy as np

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT = os.path.dirname(SCRIPT_DIR)
SYMBOLS_DIR = os.path.join(REPO_ROOT, "symbols")

DLL = os.path.join(REPO_ROOT, "..", "DuniaDemo_clang_64_dx12.dll")
BASE = 0x180000000

data = open(DLL, "rb").read()
msecs = json.load(open(os.path.join(SYMBOLS_DIR, "sections_main.json")))
secs = [(s["Name"], s["VirtualAddress"], s["VirtualSize"],
         s["PointerToRawData"], s.get("RawDataSize", s["VirtualSize"]))
        for s in msecs if "VirtualAddress" in s]


def file_to_rva(off):
    for nm, va, vs, ro, rs in secs:
        if ro <= off < ro + rs:
            return va + (off - ro)
    return None


def rva_to_file(rva):
    for nm, va, vs, ro, rs in secs:
        if va <= rva < va + vs:
            return ro + (rva - va)
    return None


def sec_of_rva(rva):
    for nm, va, vs, ro, rs in secs:
        if va <= rva < va + vs:
            return nm
    return None


# Executable sections (IMAGE_SCN_MEM_EXECUTE = 0x20000000)
exec_secs = {nm for nm, _, _, _, _ in secs if nm in
             {".text", ".MHC", ".shared", ".udata", ".xtext",
              ".rdata", ".debug$P", ".idata", ".debug"}}


def undname(name):
    p = subprocess.run(["llvm-undname", name], capture_output=True, text=True)
    out = p.stdout.strip()
    lines = out.splitlines()
    cls = lines[-1] if lines else name
    if "`RTTI Type Descriptor Name'" in cls:
        cls = cls.replace(" `RTTI Type Descriptor Name'", "").strip()
    return cls if cls else name


def find_cols():
    """Return {col_rva: {'file': off, 'name': str, 'self_ok': bool}}."""
    td = {}
    for m in re.finditer(rb"\.\?[AU][VU][^\x00]{3,120}@@", data):
        try:
            name = m.group(0).decode("ascii")
        except Exception:
            continue
        rva = file_to_rva(m.start())
        if rva:
            td[rva] = name
    td_desc = {r - 16: n for r, n in td.items()}

    n = len(data) // 4
    u32 = np.frombuffer(data, dtype="<u4", count=n)
    desc_arr = np.array(list(td_desc.keys()), dtype="<u4")
    out = {}
    CH = 1 << 25
    for base in range(0, n, CH):
        seg = u32[base:base + CH]
        m = np.isin(seg, desc_arr)
        for i in np.nonzero(m)[0]:
            f = (int(i) + base) * 4
            sig_off = f - 12
            if sig_off < 0:
                continue
            sig = struct.unpack_from("<i", data, sig_off)[0]
            if sig not in (0, 1):
                continue
            offset = struct.unpack_from("<i", data, sig_off + 4)[0]
            cdoff = struct.unpack_from("<i", data, sig_off + 8)[0]
            desc_rva = struct.unpack_from("<I", data, sig_off + 12)[0]
            pself = struct.unpack_from("<I", data, sig_off + 20)[0]
            if not (0 <= offset < 0x10000000 and abs(cdoff) < 0x10000000):
                continue
            col_rva = file_to_rva(sig_off)
            if col_rva is None:
                continue
            out[col_rva] = {
                "file": sig_off,
                "name": td_desc.get(desc_rva, "?"),
                "self_ok": pself == col_rva,
            }
    return out


def find_vftable_slots(col_rvas):
    """Find 8-byte pointers to each COL (the slot before each vftable)."""
    col_set = set(col_rvas)
    col_arr = np.array([BASE + cr for cr in col_set], dtype="<u8")
    u64 = np.frombuffer(data, dtype="<u8", count=len(data) // 8)
    slots = []
    CH = 1 << 24
    for base in range(0, len(u64), CH):
        seg = u64[base:base + CH]
        m = np.isin(seg, col_arr)
        for i in np.nonzero(m)[0]:
            off = (int(i) + base) * 8
            slots.append((off, int(seg[int(i)]) - BASE))
    return slots


def main():
    cols = find_cols()
    valid = {cr: c for cr, c in cols.items() if c["self_ok"]}
    print("COLs:", len(cols), "valid:", len(valid))

    slots = find_vftable_slots(valid.keys())
    print("vftable COL-pointer slots:", len(slots))

    col_rva_set = set(valid.keys())
    vftables = []
    for off, col_rva in slots:
        cls = valid[col_rva]["name"]
        pos = off + 8
        methods = []
        while True:
            ptr = struct.unpack_from("<Q", data, pos)[0]
            if ptr == 0:
                break
            ptr_rva = ptr - BASE
            if ptr_rva in col_rva_set:
                break
            ptr_file = rva_to_file(ptr_rva)
            if ptr_file is None or sec_of_rva(ptr_rva) not in exec_secs:
                break
            methods.append(ptr_rva)
            pos += 8
            if len(methods) > 500:
                break
        if methods:
            vt_rva = file_to_rva(off + 8)
            vftables.append({"class": cls, "col": col_rva,
                             "vt": vt_rva, "methods": methods})

    print("reconstructed vftables:", len(vftables),
          "methods:", sum(len(v["methods"]) for v in vftables))

    symbols = []
    seen = set()
    for v in vftables:
        cls = undname(v["class"])
        syms = [{"rva": v["vt"], "name": cls + "::vftable"}]
        first = {}
        for i, m in enumerate(v["methods"]):
            if m not in first:
                first[m] = i
        for m in v["methods"]:
            syms.append({"rva": m, "name": "%s::vf%d" % (cls, first[m])})
        for s in syms:
            key = (s["rva"], s["name"])
            if key not in seen:
                seen.add(key)
                symbols.append(s)

    json.dump(symbols, open(os.path.join(SYMBOLS_DIR, "vftable_symbols.json"), "w"))
    print("wrote", len(symbols), "symbols to symbols/vftable_symbols.json")


if __name__ == "__main__":
    main()
