# open-source-modding.github.io

Organization website (GitHub Pages) and reverse-engineering reference hub.
Static — no build system, no CI, no package manager. Content is Markdown with
Jekyll front matter; the only config is `_config.yml` (theme
`jekyll-theme-cayman`).

## Entry points

- `README.md` is both the repo README and the Jekyll site homepage (front matter + theme applied). `index.md` is intentionally absent.
- Docs are reachable from the site only via the `index.md` hub files (`reference/index.md`, `reference/watch_dogs/index.md`, ...) and the README section list — keep them in sync when adding docs.

## Structure

- `WDL-Reversing/` — Watch Dogs: Legion RE (synthetic PDB pipeline, Denuvo analysis, engine reference). Has its own `AGENTS.md` — read it before touching anything there. Source DLLs are **not** in this repo; they live in `~/Documents/Code/re/WDL/`.
- `reference/` — cross-game Disrupt/watch-dogs reference: `hashing.md` (CRC32/FNV naming hashes), `watch_dogs/mab-format.md` (animations), `watch_dogs/hkx-format.md` (collision), `watch_dogs/community-resources.md` (third-party tools + ZModeler forum knowledge), `materialdescriptors/` (85 XML files, WD1/WD2/WD3 — files are `r--` read-only), and `bethesda/` (Bethesda engine RE: `fallout4-havok-guide.md` converted community guide, `skyrim-havok-guide.md`, `oblivion-remastered-guide.md` UE5+Gamebryo hybrid, `starfield-guide.md` — note Starfield uses Havok 2019.02 with the new chunked packfile format, NOT the FO4 packfile).

## Conventions

- **No binaries committed**: `.dll`, `.exe`, `.zip`, `.7z`, `.pdb` are gitignored — but only via `WDL-Reversing/.gitignore`; there is **no root `.gitignore`**. Add new exclusions there.
- **WDL-specific RE stays here**; cross-game Disrupt engine work → `~/Documents/Code/re/Disrupt/`.
- `.opencode/` is orchestrator runtime state (untracked, regenerable) — never commit or treat it as site content.