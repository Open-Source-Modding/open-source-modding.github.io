#!/usr/bin/env python3
"""Extract all symbol sources for a WDL DLL into JSON files.

Usage:
    python3 extract_dll_symbols.py <dll_path> <tag> [image_base_hex]

Produces (in ../symbols/, keyed by <tag> so dx11/dx12 can coexist):
    sections_<tag>.json         PE section table
    exports_<tag>.json          PE export table        {ord, name, rva}
    rtti_<tag>.json             RTTI type descriptors  {mangled, demangled}
    extra_symbols_<tag>.json    engine method strings  {rva, seg, name}
    vftable_symbols_<tag>.json  vftables + vf methods  {rva, name}

lua_natives is produced for every build: reference list = globals.lua (a live
dump of the game's _G function globals, ../globals.lua). Each name is located
as a NUL-terminated string in the target DLL (see lua_natives_<tag>.json).
"""
import json
import re
import struct
import subprocess
import sys
import time

import numpy as np

IMG_EXECUTE = 0x20000000  # IMAGE_SCN_MEM_EXECUTE


def load_pe(data):
    e = struct.unpack_from("<I", data, 0x3C)[0]
    assert data[e:e + 4] == b"PE\x00\x00", "not a PE"
    nsec = struct.unpack_from("<H", data, e + 6)[0]
    optsz = struct.unpack_from("<H", data, e + 20)[0]
    magic = struct.unpack_from("<H", data, e + 24)[0]
    if magic == 0x20B:  # PE32+
        image_base = struct.unpack_from("<Q", data, e + 24 + 24)[0]
        exp_off = e + 24 + 112
    else:
        image_base = struct.unpack_from("<I", data, e + 24 + 28)[0]
        exp_off = e + 24 + 96
    exp_rva = struct.unpack_from("<I", data, exp_off)[0]

    secs = []
    for i in range(nsec):
        so = e + 24 + optsz + i * 40
        name = data[so:so + 8].rstrip(b"\x00").decode("latin1")
        secs.append({
            "Number": i + 1,
            "Name": name,
            "VirtualSize": struct.unpack_from("<I", data, so + 8)[0],
            "VirtualAddress": struct.unpack_from("<I", data, so + 12)[0],
            "RawDataSize": struct.unpack_from("<I", data, so + 16)[0],
            "PointerToRawData": struct.unpack_from("<I", data, so + 20)[0],
            "Characteristics": struct.unpack_from("<I", data, so + 36)[0],
        })
    return image_base, secs, exp_rva


def rva2file(secs, rva):
    for s in secs:
        va, vs = s["VirtualAddress"], s["VirtualSize"]
        if va <= rva < va + vs:
            return s["PointerToRawData"] + (rva - va)
    return None


def file2rva(secs, off):
    for s in secs:
        ro, rs = s["PointerToRawData"], s["RawDataSize"]
        if ro <= off < ro + rs:
            return s["VirtualAddress"] + (off - ro)
    return None


def cstr(data, off):
    end = data.find(b"\x00", off)
    if end < 0:
        return ""
    return data[off:end].decode("ascii", "replace")


def extract_exports(data, secs, exp_rva):
    if not exp_rva:
        return []
    f = rva2file(secs, exp_rva)
    if f is None:
        return []
    base = struct.unpack_from("<I", data, f + 12)[0]
    nf = struct.unpack_from("<I", data, f + 20)[0]
    nn = struct.unpack_from("<I", data, f + 24)[0]
    aof = struct.unpack_from("<I", data, f + 28)[0]
    aon = struct.unpack_from("<I", data, f + 32)[0]
    aoo = struct.unpack_from("<I", data, f + 36)[0]
    funcs = [struct.unpack_from("<I", data, rva2file(secs, aof) + i * 4)[0]
             for i in range(nf)]
    names = [struct.unpack_from("<I", data, rva2file(secs, aon) + i * 4)[0]
             for i in range(nn)]
    ords = [struct.unpack_from("<H", data, rva2file(secs, aoo) + i * 2)[0]
            for i in range(nn)]
    out = []
    for i in range(nn):
        name = cstr(data, rva2file(secs, names[i]))
        out.append({"ord": base + ords[i], "name": name, "rva": funcs[ords[i]]})
    return out


def undname(name):
    p = subprocess.run(["llvm-undname", name], capture_output=True, text=True)
    lines = p.stdout.strip().splitlines()
    cls = lines[-1] if lines else ""
    if "`RTTI Type Descriptor Name'" in cls:
        cls = cls.replace(" `RTTI Type Descriptor Name'", "").strip()
    return cls


def scan_rtti(data, secs):
    out = []
    cache = {}
    for m in re.finditer(rb"\.\?[AU][VU][^\x00]{3,120}@@", data):
        name = m.group(0).decode("ascii", "replace")
        if name not in cache:
            d = undname(name)
            cache[name] = d
        d = cache[name]
        demangled = name if not d else name + "\n" + d
        out.append({"mangled": name, "demangled": demangled})
    return out


def scan_extra_strings(data, secs):
    """Class::method trace strings + shader/format templates.

    The method-trace literals sit in different sections per build (.ecode in
    dx12, .rodata in dx11), so scan every section and rely on the patterns.
    """
    cls_method = re.compile(rb"^[A-Za-z_][A-Za-z0-9_]*::[A-Za-z_~][A-Za-z0-9_:~<>]*$")
    shader = re.compile(rb"^(engine\\shaders\\|SOSPengine\\shaders\\)[\x20-\x7e]+$")
    printable = re.compile(rb"[\x20-\x7e]{4,120}")

    seen = set()
    out = []
    for sec in secs:
        ro, rs = sec["PointerToRawData"], sec["RawDataSize"]
        blob = data[ro:ro + rs]
        for m in printable.finditer(blob):
            raw = m.group(0)
            if len(set(raw)) == 1 or raw.count(raw[:1]) > len(raw) * 0.5:
                continue  # drop run-garbage like bbbb::::...
            is_cls = bool(cls_method.match(raw))
            if is_cls:
                cls, _, meth = raw.decode().partition("::")
                if len(cls) < 3 or len(meth) < 2:
                    continue  # noise like K::A, NH::o
            if not (is_cls or shader.match(raw)):
                continue
            s = raw.decode("ascii", "replace")
            rva = sec["VirtualAddress"] + m.start()
            key = (rva, s)
            if key in seen:
                continue
            seen.add(key)
            out.append({"rva": rva, "seg": sec["Number"], "name": s})
    return out


def scan_vftables(data, secs, image_base):
    exec_names = {s["Name"] for s in secs if s["Characteristics"] & IMG_EXECUTE}

    def sec_of_rva(rva):
        for s in secs:
            if s["VirtualAddress"] <= rva < s["VirtualAddress"] + s["VirtualSize"]:
                return s["Name"]
        return None

    # TypeDescriptor strings -> pTypeDescriptor = name_rva - 16
    td = {}
    for m in re.finditer(rb"\.\?[AU][VU][^\x00]{3,120}@@", data):
        rva = file2rva(secs, m.start())
        if rva:
            td[rva] = m.group(0).decode("ascii", "replace")
    td_desc = {r - 16: n for r, n in td.items()}

    n = len(data) // 4
    u32 = np.frombuffer(data, dtype="<u4", count=n)
    desc_arr = np.array(list(td_desc.keys()), dtype="<u4")
    cols = {}
    CH = 1 << 25
    for base in range(0, n, CH):
        seg = u32[base:base + CH]
        for i in np.nonzero(np.isin(seg, desc_arr))[0]:
            f = (int(i) + base) * 4
            sig_off = f - 12
            if sig_off < 0:
                continue
            sig = struct.unpack_from("<i", data, sig_off)[0]
            if sig not in (0, 1):
                continue
            off = struct.unpack_from("<i", data, sig_off + 4)[0]
            cdoff = struct.unpack_from("<i", data, sig_off + 8)[0]
            desc_rva = struct.unpack_from("<I", data, sig_off + 12)[0]
            pself = struct.unpack_from("<I", data, sig_off + 20)[0]
            if not (0 <= off < 0x10000000 and abs(cdoff) < 0x10000000):
                continue
            crva = file2rva(secs, sig_off)
            if crva is None:
                continue
            cols[crva] = {"file": sig_off, "name": td_desc.get(desc_rva, "?"),
                          "self_ok": pself == crva}

    valid = {cr: c for cr, c in cols.items() if c["self_ok"]}

    col_set = set(valid.keys())
    col_arr = np.array([image_base + cr for cr in col_set], dtype="<u8")
    u64 = np.frombuffer(data, dtype="<u8", count=len(data) // 8)
    slots = []
    CH = 1 << 24
    for base in range(0, len(u64), CH):
        seg = u64[base:base + CH]
        for i in np.nonzero(np.isin(seg, col_arr))[0]:
            off = (int(i) + base) * 8
            slots.append((off, int(seg[int(i)]) - image_base))

    vftables = []
    for off, col_rva in slots:
        cls = valid[col_rva]["name"]
        pos = off + 8
        methods = []
        while True:
            ptr = struct.unpack_from("<Q", data, pos)[0]
            if ptr == 0:
                break
            ptr_rva = ptr - image_base
            if ptr_rva in col_set:
                break
            if sec_of_rva(ptr_rva) not in exec_names:
                break
            methods.append(ptr_rva)
            pos += 8
            if len(methods) > 500:
                break
        if methods:
            vt_rva = file2rva(secs, off + 8)
            if vt_rva is not None:
                vftables.append({"class": cls, "vt": vt_rva, "methods": methods})

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
    return cols, valid, vftables, symbols


def main():
    dll, tag = sys.argv[1], sys.argv[2]
    image_base = int(sys.argv[3], 16) if len(sys.argv) > 3 else 0x180000000

    t0 = time.time()
    data = open(dll, "rb").read()
    image_base, secs, exp_rva = load_pe(data)

    exports = extract_exports(data, secs, exp_rva)
    rtti = scan_rtti(data, secs)
    extra = scan_extra_strings(data, secs)
    cols, valid, vftables, vf_syms = scan_vftables(data, secs, image_base)

    # Lua native names: the Lua engine is identical in dx12/dx11 (same game,
    # same scripts). Reference = globals.lua, a live dump of the game's _G
    # table of function globals. Locate each name as a NUL-terminated string
    # in THIS dll and record its rva. Names not present in the file (packed/
    # runtime-registered) are skipped.
    import os
    globals_path = os.path.join(os.path.dirname(__file__), "..", "..", "globals.lua")
    out_dir = os.path.join(os.path.dirname(__file__), "..", "symbols")
    os.makedirs(out_dir, exist_ok=True)
    lua_names = []
    if os.path.isfile(globals_path):
        for line in open(globals_path):
            m = re.match(r"^\[([^\]]*)\] \(function\) = ", line)
            if m:
                lua_names.append(m.group(1))
        lua_names = list(dict.fromkeys(lua_names))
    else:
        print(f"note: {globals_path} not found; skipping Lua native scan")

    printable = re.compile(rb"[\x20-\x7e]{2,120}\x00")
    strset = {}
    for m in printable.finditer(data):
        strset.setdefault(m.group(0)[:-1].decode("ascii", "replace"), m.start())
    lua = []
    for name in lua_names:
        off = strset.get(name)
        if off is not None:
            rva = file2rva(secs, off)
            if rva is not None:
                lua.append({"rva": rva, "name": name})

    json.dump(secs, open(os.path.join(out_dir, f"sections_{tag}.json"), "w"), indent=1)
    json.dump(exports, open(os.path.join(out_dir, f"exports_{tag}.json"), "w"), indent=1)
    json.dump(rtti, open(os.path.join(out_dir, f"rtti_{tag}.json"), "w"), indent=1)
    json.dump(extra, open(os.path.join(out_dir, f"extra_symbols_{tag}.json"), "w"), indent=1)
    json.dump(vf_syms, open(os.path.join(out_dir, f"vftable_symbols_{tag}.json"), "w"), indent=1)
    json.dump(lua, open(os.path.join(out_dir, f"lua_natives_{tag}.json"), "w"), indent=1)

    print(f"sections:   {len(secs)}")
    print(f"exports:    {len(exports)}")
    print(f"rtti:       {len(rtti)}  (COLs {len(cols)}, valid {len(valid)})")
    print(f"extra:      {len(extra)}")
    print(f"vftables:   {len(vftables)}  methods {sum(len(v['methods']) for v in vftables)}")
    print(f"vf symbols: {len(vf_syms)}")
    print(f"lua natives:{len(lua)}  (of {len(lua_names)} referenced names)")
    print(f"done in {time.time() - t0:.1f}s")


if __name__ == "__main__":
    main()
