#!/usr/bin/env bash
# Rebuild DuniaDemo_clang_64_dx11_steam_denuvo.pdb from the YAML.
# Regenerate the dx11 symbol sources first with:
#   python3 extract_dll_symbols.py <dx11.dll> dx11
set -euo pipefail
cd "$(dirname "$0")"

PDB_DIR="../pdb"
mkdir -p "$PDB_DIR"

DX11_DLL="${1:-../DuniaDemo_clang_64_dx11.dll}"

python3 build_pdb_yaml.py \
  --tag dx11 \
  --dll "$DX11_DLL" \
  --guid '{84E8439A-8917-4C7B-988D-AEF5139AA630}' \
  --age 2 \
  --pdb "$PDB_DIR/DuniaDemo_clang_64_dx11_steam_denuvo.pdb" \
  --yaml DuniaDemo_clang_64_dx11_steam_denuvo.pdb.yaml

rm -f "$PDB_DIR/DuniaDemo_clang_64_dx11_steam_denuvo.pdb"
llvm-pdbutil yaml2pdb \
  --pdb="$PDB_DIR/DuniaDemo_clang_64_dx11_steam_denuvo.pdb" \
  DuniaDemo_clang_64_dx11_steam_denuvo.pdb.yaml

echo "OK: $(llvm-pdbutil dump --publics "$PDB_DIR/DuniaDemo_clang_64_dx11_steam_denuvo.pdb" 2>/dev/null | grep -c S_PUB32) public symbols"
