# open-source-modding.github.io

Organization website (GitHub Pages). Static — no build system, no CI, no package manager.

- `README.md` is both the repo README and the Jekyll site homepage (front matter + theme applied).
- `index.md` is intentionally absent — `README.md` serves as the entry file.

## Structure

- `WDL-Reversing/` — Watch Dogs: Legion reverse engineering (has its own `AGENTS.md` with full pipeline details)
- `reference/watch_dogs/materialdescriptors/` — 85 XML material descriptor files (WD1/WD2/WD3 engine formats)

## Conventions

- **No binaries committed**: `.dll`, `.exe`, `.zip`, `.7z` all gitignored in `WDL-Reversing/`
- **WDL-specific RE stays here**; cross-game Disrupt engine work → `~/Documents/Code/re/Disrupt/`
