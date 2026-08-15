#!/usr/bin/env bash
# Rebuild a synthetic PDB for Watch Dogs 2's Disrupt_64.dll.
#
# WD2 keeps the same intact RTTI as later Dunia builds (the orwell-editor-dev
# generation), letting us recover vftables + methods. The dev PDB
# (DuniaDemo_rt64.pdb, also orwell-editor-dev) provides real method names
# for the shared SDK/engine classes (Bloomberg, storm, G4, hk*, etc.), which we
# use to rename the recovered vf<slot> placeholders. DEV_TAG=rt64 is the full
# non-stripped editor PDB (5.5k+ renames); dev_fixed = stripped 22MB r64_dx12.
set -euo pipefail
cd "$(dirname "$0")"

DEV_TAG="${DEV_TAG:-dev_fixed}"      # dev PDB extraction tag
GAME_TAG="${GAME_TAG:-wd2}"          # game symbol extraction tag
WD2_DLL="${WD2_DLL:-/home/selene/Documents/Code/re/WD2/bin_plus/Disrupt_64.dll}"
PDB_DIR="../pdb"
mkdir -p "$PDB_DIR"

# 1. Extract WD2 DLL symbols (RTTI, exports, vftables, method strings)
python3 extract_dll_symbols.py "$WD2_DLL" "$GAME_TAG"

# 2. Dev PDB extraction (skip if already done)
if [ ! -f "../symbols/dev_types_${DEV_TAG}.json" ]; then
  python3 extract_dev_pdb_types.py /home/selene/Documents/Code/re/ubisoft/extracted/bin/DuniaDemo_r64_dx12.pdb "$DEV_TAG"
fi

# 3. Merge: rename vfN slots to dev PDB method names
python3 merge_dev_types.py "$DEV_TAG" "$GAME_TAG"
VFTABLE="../symbols/enriched_vftable_${DEV_TAG}_${GAME_TAG}.json"

# 4. Build YAML with the enriched vftable symbols
#    GUID matches the DLL's RSDS (K:\bin\Disrupt_64.pdb, age 1) so the PDB links up.
python3 build_pdb_yaml.py \
  --tag "$GAME_TAG" \
  --dll "$WD2_DLL" \
  --guid '{54015C37-3C0F-4A67-B31D-6C51D2CE0F0A}' \
  --age 1 \
  --vftable "$VFTABLE" \
  --pdb "$PDB_DIR/Disrupt_64_${GAME_TAG}_synthetic.pdb" \
  --yaml "Disrupt_64_${GAME_TAG}_synthetic.pdb.yaml"

# 5. llvm-pdbutil yaml2pdb
rm -f "$PDB_DIR/Disrupt_64_${GAME_TAG}_synthetic.pdb"
llvm-pdbutil yaml2pdb \
  --pdb="$PDB_DIR/Disrupt_64_${GAME_TAG}_synthetic.pdb" \
  "Disrupt_64_${GAME_TAG}_synthetic.pdb.yaml"

echo "OK: $(llvm-pdbutil dump --publics "$PDB_DIR/Disrupt_64_${GAME_TAG}_synthetic.pdb" 2>/dev/null | grep -c S_PUB32) public symbols"
