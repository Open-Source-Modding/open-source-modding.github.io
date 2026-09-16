---
title: "Runtime capture for name recovery & disassembly (FC6)"
---

# Runtime capture (FC6)

On-disk scanning can only recover names embedded in unpacked archives. Two
gaps remain:

- Unresolved archive entries referenced only by hash (no path string on disk).
- The game's packed code, which is encrypted until runtime.

Two FC_Plugins plugins cover both. Both build from the same
[`Open-Source-Modding/FC_Plugins`](https://github.com/Open-Source-Modding/FC_Plugins)
tree, deploy with `make install`, and log beside the game exe.

## FileHook — capture the paths the game opens

FileHook hooks `CreateFileW`/`CreateFileA` and appends each opened path to
`FC6FileHook.log`, formatted `ms<TAB>path`. It pre-opens the log handle at
init so the hook never recurses into itself.

Why it matters: the game opens files by their real path, so a capture run
surfaces paths the on-disk scans never saw. Post-process the log with
`Scripts/fc6_filehook_process.py`:

```
python3 Scripts/fc6_filehook_process.py FC6FileHook.log
```

The script computes the raw CRC64 of each path, checks it against the
1,502,018 hashes across all FC6 `.fat` files, and writes the ones not already
in the filelists to `fc6_recovered_from_hook.txt`. Append those to
`binary_recovered.filelist` and re-run RebuildFileLists.

Install: `make install` copies FileHook.dll into the game's `plugins/` folder
(PluginLoader auto-loads `.dll` from there). The hook stays passive until the
game runs.

## DumpModule — snapshot the packed DLL after unpack

FC6's main DLL (`FC_m64d3d12.dll`) is Denuvo + VMProtect packed. The `.code`
section is encrypted on disk — a static `objdump -d` returns no instructions.
Real code exists only in memory after the runtime unpacker runs.

DumpModule loads the target module in-game, polls the `.code` section until
the int3 stubs clear (up to a timeout), snapshots the mapped image, and writes
`FC_m64d3d12.dll.dump.dll` beside the exe. The dump keeps the image base
`0x180000000`, so addresses from `FC6 Dunia2 - Functions Dump.dart` map
directly:

```
x86_64-w64-mingw32-objdump -d --start-address=0x180575D000 \
    --stop-address=0x180575D05A FC_m64d3d12.dll.dump.dll
```

Both plugins need the game to run to do anything. The capture-run step is the
unvalidated part — see the common-ground `OPEN` items.

## Symbol source: Disrupt debug PDB (WDL leak)

A leaked *Watch Dogs: Legion* debug build ships full debug symbols:

- `DuniaDemo_rt64.dll` + **`DuniaDemo_rt64.pdb`**
- Retail (`DuniaDemo_clang_64_dx11.dll`) has different rebuild, so addresses
  do not line up 1:1 — the PDB is a *symbolic/structure* reference, not an
  address map.

Why it matters here: Disrupt is a Dunia 2 fork, so the PDB names transfer to
the Far Cry lineage. The thread author uses it to identify virtualized
anti-debug functions (e.g. `ubiservices::SslCertificateValidator_BF::verifyPinning`)
and cleanly replace the protected code in the retail DLL — the same class of
VMProtect stub that FC6's `FC_m64d3d12.dll` carries. When DumpModule has a
post-unpack image, cross-map the PDB's virtualized-function names against it.
Source: Fearless Revolution (Watch Dogs: Legion thread). The "terabytes of
Ubisoft source leak" referenced there is unverified — not yet banked on.