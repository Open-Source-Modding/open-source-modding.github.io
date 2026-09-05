# PreparePlatformData64 — WDL Build Pipeline

Source: Discord logs (Katana [CB] + Notex), 2021-01-16.

## What it does

`PreparePlatformData64.exe` is the official Ubisoft tool that compiles WDL worlds into runtime archives. It takes map/asset data and produces the `.dat`/`.fat` archives the game loads.

## Shader compilation

The tool also compiles HLSL shaders into `shadersobj.dat`:

```
-shadersobj=[all|generate|bigfileonly]
```

| Flag | Behavior |
|------|----------|
| `-shadersobj=all` | Compiles ALL shaders from scratch. **Wipes the obj folder first.** Takes ~6 hours. |
| `-shadersobj=bigfileonly` | Skips shader generation; uses existing compiled `.obj` files from `data_win64/engine/shaders/obj/` to build `shadersobj.dat` only. |
| `-shadersobj=generate` | Generates only (no archive packing). |

### Shader workflow

- Compiled shaders live in `data_win64/engine/shaders/obj/` (also `obj_editor/` for editor shaders).
- `shadersobj.dat` is built from whatever is in the obj folder.
- If `shadersobj.dat` is deleted and a **debug build** of the game is run, the engine loads shaders directly from the obj folder and compiles any missing ones on-the-fly when encountered in-game.
- Primarily generates **DirectX 11** shaders. DX12 shader compilation path is unclear / not much point since DX11 builds work fine.

## World compilation

### Katana's workflow (repeat-until-success pattern)

1. Enter the map in the editor
2. Repair navmesh and save
3. Attach debug (via x64 debugger) and export the map
4. If generation fails midway → reopen `PreparePlatformData64.exe`, attach debug again after startup
5. Second attempt usually completes the full map

### Why debug attachment matters

Katana's experience: without a debugger attached, the tool often crashes partway through map generation. With a debugger attached (x64 or the Ubisoft debug event logger), it completes successfully.

Notex's counter-experience: has compiled worlds without the debug logger running. The tool is the "debug event logger" — it connects to the engine and gets logs from it, but may not actually "attach debug status" as Katana assumed.

## Performance notes

- `PreparePlatformData64.exe -platform=win64` — basic world compilation
- Adding `-shadersobj=bigfileonly` keeps shader compilation fast (uses existing obj)
- Full `-shadersobj=all` on a beefy machine: ~6 hours
- Archive generation for a world: slow, can take 30+ minutes before archives start appearing

## Gotchas

- `-shadersobj=all` **wipes the obj folder** before running — back up if needed
- Katana's pre-packaged London archive (`london.fat`/`london.dat`) was built by Notex — used because local `shadersobj.dat` generation was never successful
- The debug event logger tool is separate from the debugger — it's a log viewer, not a debug attach tool
