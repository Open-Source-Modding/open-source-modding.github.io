# Integration Status

## Sync Status: PASS — no open issues

- SYNC-1 (duplicate Bethesda entries in reference/index.md): **closed** (2026-08-18T12:31:17Z)

## Hub Sync Verification

| Hub | Entry count | Guides | Status |
|-----|------------|--------|--------|
| reference/index.md (Bethesda) | 4 | FO4, Skyrim, Oblivion Remastered, Starfield | ✅ each exactly once |
| README.md (Bethesda Engine References section) | section→hub link | Prose covers all 4 (Skyrim, FO4, Starfield, Oblivion Remastered) | ✅ |

> Note: Per AGENTS.md convention, README links to the `reference/index.md` hub
> (section-level) rather than listing each guide individually. The hub lists each
> guide. Both are in sync.

## File Existence

- reference/bethesda/fallout4-havok-guide.md ✅
- reference/bethesda/skyrim-havok-guide.md ✅ (161 lines)
- reference/bethesda/oblivion-remastered-guide.md ✅ (164 lines)
- reference/bethesda/starfield-guide.md ✅ (99 lines)
- reference/havok/hkx_format.md ✅
- reference/watch_dogs/* ✅

## Link Checks

- README hashing link → reference/watch_dogs/hashing.md (exists) ✅
- No stale reference/hashing.md references ✅
- reference/watch_dogs/index.md `(hashing.md)` → relative link to watch_dogs/hashing.md (exists) ✅
- All internal cross-refs resolve ✅

## Guides Factual Review (S4.1.1)

- **Skyrim guide**: PASS — SE/AE packfile v8, 2010.2.0 SDK, LE 32-bit/SE 64-bit,
  FO4 v11/2014.1.0 correctly contrasted; consistent with reference/havok/hkx_format.md
- **Oblivion Remastered guide**: PASS — UE5.3 + Gamebryo hybrid framing, Io Store
  containers, UE5-native animation, confidence markers; matches research cache
- **Starfield guide**: PASS — CE2/SFSE/Creation Kit/tools, honest open questions

## LSP / Build

- Static Jekyll site (no build step, no package manager). Structural + factual
  checks serve as verification. LSP not applicable to Markdown content.
