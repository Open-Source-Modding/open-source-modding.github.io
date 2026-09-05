# open-source-modding.github.io

GitHub Pages site and reverse-engineering reference hub.
Static: no build system, no CI, no package manager. Markdown with Jekyll
front matter; only config is `_config.yml` (theme `jekyll-theme-cayman`).

## Entry points

- `README.md` is both the repo README and the Jekyll site homepage (front matter + theme applied). `index.md` is intentionally absent.
- Docs are reachable from the site only via the hub `index.md` files (`reference/index.md`, `reference/disrupt/watch_dogs/index.md`, ...) and the README section list — keep them in sync when adding docs.

## Working tree is mid-migration

`git status` is noisy by design: `WDL-Reversing/` and the old
`reference/watch_dogs/` tree were deleted from disk (untracked), while the new
`reference/disrupt/` and per-game `reference/<game>/` trees are untracked.
The WDL docs were folded into `reference/disrupt/watch_dogs_legion/`.
Do not restore `WDL-Reversing/` or "fix" this state — it is the intended
transition.

## Structure

```
reference/
├── index.md                          # site hub — links all sections
├── disrupt/                          # Disrupt engine (WD1/WD2/WDL)
│   ├── compressed_mesh_format.md, depload-format.md, disrupt-editor.md,
│   │   havok-repos.md, installpackage-patch.md, xbt-format.md            # cross-game notes
│   ├── blender-addon.md              # Blender 5.0 add-on: import/export/inject XBG/GLM/HKX/MAB/.material.bin
│   ├── glm-format.md, custom-model-workflow.md, weapon-adding-guide.md    # modeling guides
│   ├── unsorted/                     # raw engine notes: disrupt_engine.txt, MaterialBin/, modding/, xbg/, xbt/, PFO_SPAWN_SYSTEM.md
│   ├── watch_dogs/                   # WD1 — hkx/packfile/hashing/mab formats, modding tutorials, Wii U symbols, shaders, materialdescriptors/
│   ├── watch_dogs_2/                 # WD2 — TAG0 collision, model formats, vehicle handling, community resources
│   └── watch_dogs_legion/            # WDL — HKX_FORMAT_REVERSE (Havok 2017.2), denuvo, engine ref, PDB pipeline
├── bethesda/                         # 5 guides + research drafts + xentax knowledge
├── havok/                            # hkx_format.md + xentax-havok knowledge
├── <game>/                           # one *-formats.md + xentax-*-knowledge.md per game
```

## Conventions & gotchas

- **No CI/CD**. No workflows, no checks.
- **Binaries are NOT gitignored**. Do not commit attachments.
- `.opencode/` is orchestrator runtime state (untracked, regenerable).
- **Havok docs are distinct — do NOT merge**: standard packfile (FO4 2014) vs WDL Dunia wrapper (2017.2) vs WD1/WD2 Disrupt fork vs WD2 TAG0.
- **XeNTaX links in `reference/index.md` are broken** — ~~they point at `.opencode/docs/` which no longer exists; relink to `reference/unsorted/` when organizing.~~ ✅ FIXED: all XeNTaX links now resolve correctly.
- **Duplicate WD2 TAG0 entry** in `reference/index.md` — ~~consolidate when fixing XeNTaX links.~~ ✅ FIXED: duplicate removed.
- Cross-game Disrupt engine work (hook code, symbols, PDB tooling) stays out of this repo — only reference docs live here.

## Tools & converters

| Tool | What | Location |
|------|------|----------|
| ConvertMaterials | `.material.bin` → XML | `~/Documents/Code/re/Ubisoft/Disrupt/DisruptEditor/ConvertMaterials.exe` |
| Gibbed.Disrupt.ConvertXml | XML ↔ RML | `~/Documents/Code/game-tools/Ubisoft/Disrupt/Gibbed.Disrupt/projects/` |
| watch-dogs-loc-tool | `.loc` extract/import | `~/Documents/Code/re/Ubisoft/Disrupt/watch-dogs-loc-tool/` |
| DriveConverter | `.handling.bin` ↔ XML (WD2) | rootCBR, GitHub |

### Material format versions

- **v1 (retail)**: Standard `.material.bin` — little-endian, current game format.
- **v5 (beta, 2012)**: Big-endian variant found in early builds. Not compatible with retail tools.

## Known pending work

- Organize `reference/unsorted/`: distribute `xentax-*-knowledge.md` and game drafts into proper section folders; fix broken `reference/index.md` links. ✅ DONE: `reference/unsorted/` is now empty. Knowledge files live alongside their game sections. All index links resolve.
- GitHub Pages deploys (#7, #8, Aug 2026) failed on GitHub-side errors — environmental, not a repo problem.
