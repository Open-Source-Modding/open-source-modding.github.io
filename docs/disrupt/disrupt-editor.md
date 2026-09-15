# Ubisoft's Internal Disrupt Editor (WDL)

Running the internal Disrupt Editor against Watch Dogs: Legion content — community findings (cccchrona, Encrypted, qstlijku; WD Modding Discord, 2026-08). Distinct from the **fan-made** Disrupt Editor (WD1 material converter, see `watch_dogs/material-hex-editing.md`) and from the `DisruptEditor/` community source clone (Open-Source-Modding/DisruptEditor).

## Local leak copy (this workspace)

The editor binaries + debug symbols ship in the Ubisoft leak (`re/Ubisoft/Disrupt/leak/ubisoft/bin/`):

- **Editor exe**: `Disrupt.exe` / `Disrupt_dx12.exe`
- **Editor DLLs**: `Editor_r64(_dx12).dll`, `EditorCore_r64(_dx12).dll`, `EditorControls_*`, `EditorMove_*`, `EditorExporter_*`, `Disrupt.LevelEditor.dll`, `DuniaCurveEditor.dll`, `MissionManagerEditor.pdb`, `ToolLauncher_r64(_dx12).exe`, `ProcessCreator_r64(_dx12).exe`
- **Editor.log** (26 MB, 2020-04-10 dev build): every line tagged `[Bloomberg,...]` — Bloomberg is the engine's **crash/telemetry client**, not a server-only thing. It runs `CrashAnalyser::RegisterDLL` and opens `bloomberg_settings.ini` (missing in the leak → error spam, non-fatal).
- **Bloomberg binaries**: `BlackBox/Release/bloomberg.pdb`, `Bloomberg.Client.pdb`, `Bloomberg.Message.pdb` + `Bloomberg_*.Log` dumps
- **Editor source data (GLM/XML)**: `leak/ubisoft/data/graphics/` — raw buildings, Characters, cinematics, `Editor/`, gameplay_ingredients (matches `ubisoft\data\graphics\vehicles_nexus` = "source, not what retail loads" in `RETAIL_VEHICLE_CATALOG.md` §1)

## Status: it works

- cccchrona got it loading WD: Legion content. Loading maps works (tested an island prototype); "this was surprisingly much easier to get to work than frostbite."
- Encrypted could not get it to load at all ("it takes forever") — the patched build fixes that.

## Required fixes before it's usable

- **"Bloomberg" server patch (mandatory).** The editor constantly tries to connect to an internal Ubisoft server; when the connection fails it **retries infinitely**, which blocks loading (and likely the "takes forever" symptom). cccchrona patched it so the editor lets you in automatically.
- **Non-fatal debugbreaks crash the editor.** It crashes "for no reason" — Troplo: attach a debugger (Visual Studio or WinDbg) instead of relying on editor logs, because there are "a bunch of non fatal debugbreaks everywhere that will crash the program even if it could continue running fine."

## Bloomberg patch — config-only, no binary edit (SOLVED 2026-08-22)

**What Bloomberg is**: the engine's internal crash/telemetry client (`Dunia.Bloomberg.dll` + `Bloomberg.Client.dll`/`Bloomberg.Message.dll`). On startup it runs `CrashAnalyser::RegisterDLL`, opens `bloomberg_settings.ini`, then opens connections to Ubisoft's internal services. With no connectivity it retries and blocks loading.

**Default (embedded) config** — extracted from strings in `Dunia.Bloomberg.dll`:
```xml
<BloombergConfig IsActive="1" ServerIP="msr-wd3-blo01.ubisoft.org" ServerPort="12090"
  ReporterPort="12092" ... PerforceCodeServer="p4-tor-dunia-code:2326" ...>
  <Proxies>
    <Proxy Address="buc-proxy-bloomberg.ubisoft.org" Port="12090" IPRanges="10.18." .../>
    <Proxy Address="pdc-wd-bloom.ubisoft.org" Port="12090" IPRanges="10.39.;..." .../>
    <Proxy Address="192.168.5.100" Port="12090" IPRanges="192.168." .../>
  </Proxies>
</BloombergConfig>
```
`Editor.log` also shows `bloomberg.kibana.url` → `https://logging-wd3-uat4.ubisoft.onbe...` (Kibana log backend) and `Connected to ORWELL_PC_UAT_B` (Orwell = WDL codename).

**Settings search order** (from `BlackBox/Release/Bloomberg_*.Log`):
1. `c:\bloomberg_settings.ini`
2. `<exe dir>\BlackBox\Release\bloomberg_settings.ini` ← the leak's `leak/ubisoft/bin/BlackBox/Release/`
3. `C:\Users\Public\Documents\Ubisoft\Bloomberg\bloomberg_settings.ini`
4. `C:\Users\<user>\Documents\Ubisoft\Bloomberg\bloomberg_settings.ini`
If none found: "Default settings will be used."

**The fix** — craft `bloomberg_settings.ini` (schema = Bloomberg 2.1.x template, leaked at `td_tools/metrix_beta/bloomberg_settings.ini` and `pybloomberg/data/`). Sections: `[Game]`, `[Bloomberg]`, `[Connections]`, `[RuntimeVar]`, `[Crash]`. The key is `[Connections]`: each `ConnectionTypeN/ConnectionReportTypeN/ConnectionIpN/ConnectionPortN` (max 10, lower index = higher priority), and a **`File` connection supersedes the Server connection** — reports are written to a local folder, no socket is opened, nothing retries:

```ini
[Connections]
ConnectionType0=File
ConnectionReportType0=All
ConnectionPath0=%TEMP%\Bloomberg\
```

**Ready-made artifact**: `reference/disrupt/bloomberg_settings.ini` (docs repo) — a complete override file with the File connection + `MinimalLoggingLevel=OFF`, `EnableStats=0`, fast-fail timeouts, `EnableDebuggerDetectionFor=None`. A copy is deployed at `leak/ubisoft/bin/BlackBox/Release/bloomberg_settings.ini` (search path #2).

**Why this works without a binary patch**: the ini is loaded at initialization and overrides the compile-time embedded `BloombergConfig`. cccchrona's "patch" may have been exactly this file, or a NOP of the connect — but the config-only route needs no hex editing and is reversible.

## Play button

- The play button is hardcoded to look for the first campaign story mission and crashes there (cccchrona).
- Expected behavior per Troplo: play should drop your character into the world and not start anything.

## Load order context (why a patch)

Matches the engine's archive/load rules documented in `WDL/AGENTS.md` (DisruptManager rootCBR only packs `patch*`; EncryptedsPatch loads at prio 0; world packs after). The editor's server handshake happens before world load, so a dead server stalls everything until patched.