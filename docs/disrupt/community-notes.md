# Disrupt Community Notes (Discord)

Signal extracted from raw Discord dumps. Original authors credited per topic.

---

## Watch Dogs 1

### depload entries and crash stability (Olanov, 2022-07-22)

- Adding peds with new meshes, textures, or material bins does **not**
 necessarily require depload entries. Olanov tested cop-ped assets without
 crashes.
- A crash he blamed on depload was actually the model itself: materials were
 defined incorrectly. After fixing the model, removing the depload entries,
 and re-testing, no crashes occurred.
- Crash signature (one member): random crashes after reloading the game
 following a cop shootout; another reported a weapon whose model caused an
 instant crash when loaded (gun + model loaded → crash).
- Advice: check materials first — "they are a source of a lot of headache".
 One member's crash traced to a material using the wrong texture
 (a 1911 texture on a different model).

### Cop ped overhaul (Olanov + FenderBender + DukeOfFail, 2022-07-22)

- Reused base cop assets, edited in ZModeler (changed some parts), added as
 stand-alone items; cars replaced outright.
- Shared assets (e.g. default cop hats) used where needed; some peds get
 caps/goggles; the enforcer loses its sci-fi body armor; minor re-textures.
- Goal was authenticity in gameplay; texture re-textures by Duke, UV-mapped
 cars by FenderBender.
- Caution: avoid breaking other peds when introducing tweaks.

---

## Watch Dogs 2

### ConvertBatchFile.exe — cbatch converter (qstlijku, 2026)

- Converts **all 3409 compound cbatch files**.
- Usage:
 - `ConvertBatchFile.exe ./cbatch` — converts in place
 - `ConvertBatchFile.exe ./cbatch ./cbatch_converted` — output to a folder
- Release version (`ConvertBatchFile_Release.zip`, 11.47 MB) needs no Visual
 Studio debug runtime; supports hi-res cbatch.
- Round-trip conversion (cbatch → XML → cbatch) works.
- Build notes: the existing DisruptEditor already had much of the format
 mapped; the converter was cleaned up to be similar to ConvertXBG /
 ConvertMaterials. Looked at the Wii U version in IDA (the only version with
 partial debug symbols) plus the WDL PDB. ~6 months of total tooling work.

### WD2_FileLogger-2.lua — streamed file debug logger (Jrjman05 / Legi0n)

- Lua script for Cheat Engine that prints everything streamed from game
 files — mostly useful for locating `.wem` audio.
- Usage: Cheat Engine → Open process → Memory View → Ctrl+L → Paste script →
 Execute.
- Hook: `Disrupt_64.dll` `BFFileOpen` RVA `0x4AD3040`, `fileId` as
 `uint64*` in RDX.
- Use case: play an audio log or song in-game; the CE Memory View shows the
 files used live; copy the filename into a filelist.

---

## Watch Dogs Legion

### Kelly clan drone livery spawning (Junk / □ΞnCrypTΞD□, Discord)

- Problem: spawning drones via NOMAD Scripthook gives the Albion livery
 even when the drone is labeled as a Kelly Clan drone with Kelly's GUID.
- Solution: the drone needs both the drone GUID **and** its **material
 override ID**:
 1. Find the Kelly drone GUID and the material override script in the
 vehicle spawn script.
 2. Place the drone's material override IDs into the **material override
 bank**.
 3. Extract the Legion `.dat`/`.fat` (common + london) to locate where
 override IDs are banked and get the drone's ID.
- Change both the drone ID and its livery (material override) ID.