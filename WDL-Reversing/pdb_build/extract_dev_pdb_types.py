#!/usr/bin/env python3
"""Extract types, publics, and globals from a non-stripped dev PDB into JSON.

Usage:
    python3 extract_dev_pdb_types.py <path-to-pdb> <tag>

Produces in ../symbols/ (keyed by <tag>):
    dev_types_<tag>.json     All LF_STRUCTURE/CLASS/INTERFACE + LF_FIELDLIST + LF_ENUM records
    dev_publics_<tag>.json   S_PUB32 entries with demangled names
    dev_globals_<tag>.json   S_UDT / S_CONSTANT entries with types

Dependencies: llvm-pdbutil, llvm-undname
"""
import json
import os
import re
import subprocess
import sys
import time

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT = os.path.dirname(SCRIPT_DIR)
SYMBOLS_DIR = os.path.join(REPO_ROOT, "symbols")


def undname(name):
    p = subprocess.run(["llvm-undname", name], capture_output=True, text=True)
    out = p.stdout.strip()
    lines = out.splitlines()
    cls = lines[-1] if lines else name
    if "`RTTI Type Descriptor Name'" in cls:
        cls = cls.replace(" `RTTI Type Descriptor Name'", "").strip()
    return cls if cls else name


def run_pdbutil(pdb, flag):
    return subprocess.run(
        ["llvm-pdbutil", "dump", flag, pdb],
        capture_output=True, text=True, timeout=300
    ).stdout


BTICK_RE = re.compile(r'`([^`]*)`')
TYPE_HEADER_RE = re.compile(
    r'^\s+0x([0-9A-Fa-f]+)\s+\|\s+(LF_\w+)\s+\[size\s*=\s*(\d+)\](?:\s+`([^`]*)`)?\s*$'
)
FIELDITEM_RE = re.compile(
    r'^\s+-\s+(LF_\w+)\s+\[(.+?)\]\s*(?:`([^`]*)`)?\s*$'
)
PUBLIC_RE = re.compile(
    r'^\s+(\d+)\s+\|\s+S_PUB32\s+\[size\s*=\s*(\d+)\]\s+`([^`]*)`$'
)
GLOBAL_RE = re.compile(
    r'^\s+(\d+)\s+\|\s+(S_\w+)\s+\[size\s*=\s*(\d+)\]\s+`([^`]*)`$'
)


def parse_type_record(text):
    """Parse a single type record block into a dict."""
    lines = text.splitlines()
    if not lines:
        return None

    m = TYPE_HEADER_RE.match(lines[0])
    if not m:
        return None

    type_id = int(m.group(1), 16)
    kind = m.group(2)
    size = int(m.group(3))
    name = m.group(4)  # may be None for nameless records (LF_FIELDLIST, LF_MODIFIER, etc.)

    rec = {
        "type_id": type_id,
        "kind": kind,
        "size": size,
    }
    if name is not None:
        rec["name"] = name

    # Parse continuation lines
    for line in lines[1:]:
        stripped = line.strip()

        # LF_FIELDLIST members (lines starting with "    - LF_")
        fm = FIELDITEM_RE.match(line)
        if fm:
            field_kind = fm.group(1)
            field_attrs = fm.group(2)
            field_name = fm.group(3)
            # Names are often embedded as `name = \`Foo\`` inside attrs
            if not field_name:
                nn = re.search(r'name\s*=\s*`([^`]*)`|name\s*=\s*([^,]+?)\(', field_attrs or "")
                if nn:
                    field_name = nn.group(1) if nn.group(1) else nn.group(2).strip()
            member = {
                "kind": field_kind,
                "attrs": field_attrs,
                "name": field_name,
            }
            if field_kind == "LF_MEMBER":
                off = re.search(r'offset\s*=\s*(\-?\d+)', field_attrs or "")
                if off:
                    member["offset"] = int(off.group(1))
                ty = re.search(r'Type\s*=\s*0x([0-9A-Fa-f]+)', field_attrs or "")
                if ty:
                    member["type"] = int(ty.group(1), 16)
            if "members" not in rec:
                rec["members"] = []
            rec["members"].append(member)
            continue

        if "members" not in rec:
            rec.setdefault("continuation", [])

        if stripped.startswith("unique name:"):
            m2 = BTICK_RE.search(stripped)
            if m2:
                rec["unique_name"] = m2.group(1)

        elif stripped.startswith("vtable:"):
            m2 = BTICK_RE.search(stripped)
            if m2:
                rec["vtable"] = m2.group(1)
            vt = re.search(r'vtable:\s*(\S+)', stripped)
            if vt:
                rec["vtable_ref"] = vt.group(1)
            # Same continuation line carries base list + field list
            bl = re.search(r'base list:\s*(\S+)', stripped)
            if bl:
                rec["base_list"] = bl.group(1)
            fl = re.search(r'field list:\s*(<no\s+type>|0x[0-9A-Fa-f]+)', stripped)
            if fl:
                val = fl.group(1)
                rec["field_list"] = None if val.startswith('<') else int(val, 16)

        elif stripped.startswith("base list:"):
            bl = re.search(r'base list:\s*(\S+)', stripped)
            if bl:
                rec["base_list"] = bl.group(1)
            fl = re.search(r'field list:\s*(<no\s+type>|0x[0-9A-Fa-f]+)', stripped)
            if fl:
                val = fl.group(1)
                rec["field_list"] = None if val.startswith('<') else int(val, 16)

        elif stripped.startswith("field list:"):
            fl = re.search(r'field list:\s*(<no\s+type>|0x[0-9A-Fa-f]+)', stripped)
            if fl:
                val = fl.group(1)
                rec["field_list"] = None if val.startswith('<') else int(val, 16)

        elif stripped.startswith("underlying type:"):
            ut = re.search(r'underlying type:\s*(\S+)', stripped)
            if ut:
                rec["underlying_type"] = ut.group(1)

        elif stripped.startswith("options:"):
            rec["options"] = stripped.replace("options:", "").strip()

        elif stripped.startswith("referent"):
            r = re.search(r'referent\s*=\s*(\S+)', stripped)
            if r:
                rec["referent"] = r.group(1)
            mo = re.search(r'modifiers\s*=\s*(.*)', stripped)
            if mo:
                rec["modifiers"] = mo.group(1).strip()

        elif stripped.startswith("mode"):
            mo = re.search(r'mode\s*=\s*(\S+)', stripped)
            if mo:
                rec["mode"] = mo.group(1)
            opts = re.search(r'opts\s*=\s*(\S+)', stripped)
            if opts:
                rec["pointer_opts"] = opts.group(1)
            kind = re.search(r'kind\s*=\s*(\S+)', stripped)
            if kind:
                rec["pointer_kind"] = kind.group(1)

        else:
            # Generic continuation (e.g. ARGLIST entries)
            if "extra" not in rec:
                rec["extra"] = []
            rec["extra"].append(stripped)

    return rec


def extract_types(pdb):
    """Extract all type records from PDB dump into structured list."""
    raw = run_pdbutil(pdb, "--types")

    types = []
    current_lines = None
    in_table = False

    for line in raw.splitlines():
        # Detect table start
        if "Showing" in line and "records" in line:
            in_table = True
            continue
        if not in_table:
            continue

        # Check for header line (starts with hex type ID)
        if re.match(r'^\s+0x[0-9A-Fa-f]+\s+\|', line):
            if current_lines is not None:
                rec = parse_type_record(current_lines)
                if rec:
                    types.append(rec)
            current_lines = line
        elif current_lines is not None:
            current_lines += "\n" + line

    # Last record
    if current_lines is not None:
        rec = parse_type_record(current_lines)
        if rec:
            types.append(rec)

    return types


def extract_publics(pdb):
    """Extract S_PUB32 public symbols."""
    raw = run_pdbutil(pdb, "--publics")
    publics = []
    current_name = None
    current_flags = None
    current_addr = None
    current_size = None

    for line in raw.splitlines():
        m = PUBLIC_RE.match(line)
        if m:
            # Save previous
            if current_name is not None:
                publics.append({
                    "mangled": current_name,
                    "demangled": undname(current_name),
                    "size": current_size,
                    "flags": current_flags,
                    "addr": current_addr,
                })
            current_size = int(m.group(2))
            current_name = m.group(3)
            current_flags = None
            current_addr = None
        elif current_name is not None:
            s = line.strip()
            if s.startswith("flags"):
                current_flags = s.replace("flags", "").strip().lstrip("=").strip()
            if s.startswith("addr"):
                current_addr = s.replace("addr", "").strip().lstrip("=").strip()

    if current_name is not None:
        publics.append({
            "mangled": current_name,
            "demangled": undname(current_name),
            "size": current_size,
            "flags": current_flags,
            "addr": current_addr,
        })

    return publics


def extract_globals(pdb):
    """Extract S_UDT and S_CONSTANT global symbols."""
    raw = run_pdbutil(pdb, "--globals")
    globals_list = []
    current_name = None
    current_kind = None
    current_size = None
    current_detail = None

    for line in raw.splitlines():
        m = GLOBAL_RE.match(line)
        if m:
            if current_name is not None:
                globals_list.append({
                    "kind": current_kind,
                    "name": current_name,
                    "size": current_size,
                    "detail": current_detail,
                })
            current_kind = m.group(2)
            current_size = int(m.group(3))
            current_name = m.group(4)
            current_detail = None
        elif current_name is not None:
            s = line.strip()
            if s:
                current_detail = s

    if current_name is not None:
        globals_list.append({
            "kind": current_kind,
            "name": current_name,
            "size": current_size,
            "detail": current_detail,
        })

    return globals_list


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 extract_dev_pdb_types.py <path-to-pdb> [tag]")
        sys.exit(1)

    pdb = sys.argv[1]
    tag = sys.argv[2] if len(sys.argv) > 2 else "dev"

    t0 = time.time()

    print("Extracting types...", file=sys.stderr)
    types = extract_types(pdb)
    print(f"  {len(types)} type records in {time.time() - t0:.1f}s", file=sys.stderr)

    t1 = time.time()
    print("Extracting publics...", file=sys.stderr)
    publics = extract_publics(pdb)
    print(f"  {len(publics)} public symbols in {time.time() - t1:.1f}s", file=sys.stderr)

    t2 = time.time()
    print("Extracting globals...", file=sys.stderr)
    globals_list = extract_globals(pdb)
    print(f"  {len(globals_list)} global symbols in {time.time() - t2:.1f}s", file=sys.stderr)

    os.makedirs(SYMBOLS_DIR, exist_ok=True)

    json.dump(types, open(os.path.join(SYMBOLS_DIR, f"dev_types_{tag}.json"), "w"), indent=1)
    json.dump(publics, open(os.path.join(SYMBOLS_DIR, f"dev_publics_{tag}.json"), "w"), indent=1)
    json.dump(globals_list, open(os.path.join(SYMBOLS_DIR, f"dev_globals_{tag}.json"), "w"), indent=1)

    print(f"\nTotal: {time.time() - t0:.1f}s", file=sys.stderr)
    print(f"Wrote symbols/dev_types_{tag}.json", file=sys.stderr)
    print(f"Wrote symbols/dev_publics_{tag}.json", file=sys.stderr)
    print(f"Wrote symbols/dev_globals_{tag}.json", file=sys.stderr)


if __name__ == "__main__":
    main()
