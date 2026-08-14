#!/usr/bin/env python3
"""Merge dev PDB type/symbol data into a game PDB pipeline (WDL or WD2).

Usage:
    python3 merge_dev_types.py <dev_tag> <game_tag>

Reads (all under ../symbols/):
    dev_types_<dev_tag>.json        dev PDB type records (extract_dev_pdb_types.py)
    vftable_symbols_<game_tag>.json game PDB vftable slots (extract_dll_symbols.py)

Writes:
    enriched_vftable_<dev_tag>_<game_tag>.json  vftable symbols with vfN renamed to dev method names
    enriched_types_<dev_tag>_<game_tag>.json    class/structure type data for the game build

The dev PDB (DuniaDemo_r64_dx12.pdb, the WD2-era orwell-editor build) has complete
class definitions with ordered LF_ONEMETHOD lists (vftable order) and LF_MEMBER
field offsets. Where a game build's vftable class name matches a dev PDB class,
the placeholder vf<slot> names are replaced with the real method names.
"""
import json
import os
import re
import sys
import time

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_ROOT = os.path.dirname(SCRIPT_DIR)
SYMBOLS_DIR = os.path.join(REPO_ROOT, "symbols")


def src(tag, kind):
    if tag == "main":
        name = {
            "vftable_symbols": "vftable_symbols.json",
            "dev_types": "dev_types_main.json",
        }.get(kind, f"{kind}_{tag}.json")
        return os.path.join(SYMBOLS_DIR, name)
    return os.path.join(SYMBOLS_DIR, f"{kind}_{tag}.json")


def load_json(path):
    try:
        return json.load(open(path))
    except FileNotFoundError:
        return []


def classname_norm(name):
    name = name.strip()
    m = re.match(r'^(?:class|struct|enum|union)\s+(.*)$', name)
    if m:
        name = m.group(1)
    return name


def build_dev_maps(dev_types):
    """Return (class_method_order, class_fields).
    class_method_order: dict classname -> [LF_ONEMETHOD names in declaration order]
    class_fields:       dict classname -> {fieldname: offset}
    """
    by_id = {t["type_id"]: t for t in dev_types}
    method_order = {}
    fields = {}
    for t in dev_types:
        if t["kind"] not in ("LF_CLASS", "LF_STRUCTURE", "LF_INTERFACE"):
            continue
        name = t.get("name")
        fl = by_id.get(t.get("field_list"))
        if not name or not fl:
            continue
        methods = []
        field_map = {}
        for m in fl.get("members", []):
            if m["kind"] in ("LF_ONEMETHOD", "LF_METHOD") and m.get("name"):
                methods.append(m["name"])
            elif m["kind"] == "LF_MEMBER" and m.get("name"):
                field_map[m["name"]] = m.get("offset")
        if methods:
            method_order[name] = methods
        if field_map:
            fields[name] = field_map
    return method_order, fields


def rename_vftable(vftable_symbols, method_order):
    renamed = 0
    out = []
    for sym in vftable_symbols:
        name = sym.get("name", "")
        entry = {"rva": sym.get("rva"), "name": name}
        rp = name.rpartition("::")
        if (len(name.split("::")) > 2 and rp[2].startswith("vf")) or \
           (rp[2].startswith("vf") and "::" in name):
            cls_raw, _, slot_part = name.rpartition("::")
            cls = classname_norm(cls_raw)
            try:
                idx = int(slot_part[2:])
            except ValueError:
                idx = None
            if idx is not None and cls in method_order and idx < len(method_order[cls]):
                real = method_order[cls][idx]
                if real:
                    entry["name"] = f"{cls_raw}::{real}"
                    renamed += 1
        out.append(entry)
    return out, renamed


def main():
    if len(sys.argv) < 3:
        print("Usage: python3 merge_dev_types.py <dev_tag> <game_tag>")
        print("  dev_tag:  tag used for dev PDB extraction (e.g. 'dev_fixed')")
        print("  game_tag: tag for game vftable symbols (e.g. 'main', 'dx12', 'dx11', 'wd2')")
        sys.exit(1)

    dev_tag, game_tag = sys.argv[1], sys.argv[2]
    t0 = time.time()

    dev_types = load_json(src(dev_tag, "dev_types"))
    print(f"dev types: {len(dev_types)}", file=sys.stderr)

    method_order, fields = build_dev_maps(dev_types)
    print(f"dev classes w/ methods: {len(method_order)}, w/ fields: {len(fields)}",
          file=sys.stderr)

    vftable = load_json(src(game_tag, "vftable_symbols"))
    print(f"{game_tag} vftable symbols: {len(vftable)}", file=sys.stderr)

    enriched_vftable, renamed = rename_vftable(vftable, method_order)
    print(f"renamed vfN slots: {renamed}", file=sys.stderr)

    # Enriched types: game vftable classes annotated with dev PDB methods+fields
    matched_classes = {}
    for sym in vftable:
        name = sym.get("name", "")
        if "::vftable" not in name:
            continue
        cls = classname_norm(name.replace("::vftable", ""))
        rec = {}
        if cls in method_order:
            rec["methods"] = method_order[cls]
        if cls in fields:
            rec["fields"] = fields[cls]
        if rec:
            matched_classes[cls] = rec

    print(f"vftable classes matched to dev PDB: {len(matched_classes)}", file=sys.stderr)

    out_v = os.path.join(SYMBOLS_DIR, f"enriched_vftable_{dev_tag}_{game_tag}.json")
    out_t = os.path.join(SYMBOLS_DIR, f"enriched_types_{dev_tag}_{game_tag}.json")
    json.dump(enriched_vftable, open(out_v, "w"))
    json.dump(matched_classes, open(out_t, "w"), indent=1)

    print(f"\nWrote:", file=sys.stderr)
    print(f"  {out_v}", file=sys.stderr)
    print(f"  {out_t}", file=sys.stderr)
    print(f"time: {time.time() - t0:.1f}s", file=sys.stderr)


if __name__ == "__main__":
    main()
