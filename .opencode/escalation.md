# Escalation: SYNC-2 factually contradicted by primary source

**Filed by**: Worker `ses_update` (starfield-guide.md MODIFY), 2026-08-18T12:35Z
**Relates to**: SYNC-2 (starfield-guide.md animation-system claim), TODO S2.3.1 (marked FAILED)

## Summary

SYNC-2 asserts the starfield guide wrongly claims CE2 keeps a "FO4-style Havok
`.hkx` animation lineage", and cites the research cache
`.opencode/docs/starfield.md` lines 22-23, 52-53, 120-124 as saying the opposite
("Starfield does NOT expose a FO4-style Havok animation/physics class set",
"animation modding works via custom frameworks ... SAF/NAF, glTF/GLB playback").

**This citation does not match the current research doc**, and the SAF/NAF/glTF
claim is unsupported by any cached research.

## Evidence

1. **Current research doc** (`.opencode/docs/starfield.md`, 173 lines, read fresh
   at 2026-08-18T12:35Z) explicitly CONFIRMS the AnimGraph/Havok lineage:
   - §2 heading: "Animation — Havok `.hkx` + AnimGraph (CONFIRMED lineage)"
   - Lines 26-27: "Starfield retains the Havok behavior/animation `.hkx` lineage"
   - Lines 30-37: CommonLibSF exposes `BSAnimationGraph`, `BSAnimationGraphManager`,
     `BGSAnimationGraphComponent`, `IAnimationGraphManagerHolder`
   - Lines 38-39: "No `hkb*` or `hkx*` Havok types are exposed in CommonLibSF —
     Havok behavior is handled opaquely behind the AnimGraph interface (same as Skyrim SE)"
   - §8 pointer: "Emphasize the Animation Graph (AnimGraph) framework as the bridge"

2. **Primary source verified live** (CommonLibSF `include/RE/B/BSAnimationGraph.h`,
   fetched 2026-08-18T12:35Z):
   ```
   class BSAnimationGraph :
   class AnimationManager : public BSAnimationGraph
   class BSAnimationGraphManager :
   static_assert(sizeof(BSAnimationGraphManager) == 0x80);
   ```
   → Starfield's RE library DOES expose the Animation Graph framework (the same
   architecture Skyrim SE/FO4 use to wrap Havok behavior). The SYNC-2 premise that
   "Starfield does NOT expose ... Havok animation class set" is contradicted.

3. **No SAF/NAF/glTF anywhere**: `grep -rniE "SAF|NAF|glTF|mystic-cat|starfield animation framework"`
   across `.opencode/docs/` and `.opencode/archive/` → **zero matches**. The SAF/NAF
   "custom frameworks" story cited by SYNC-2 exists in no cached research.

## Interpretation

SYNC-2 appears to have been filed against a stale/different version of the
starfield research doc. The current authoritative research + live primary source
confirm the guide's framing is correct. The honest nuances are already captured in
the guide:
- AnimGraph framework confirmed (HIGH, CommonLibSF)
- `hkb*`/`hkx*` Havok types NOT exposed publicly (opaque behind AnimGraph, same as Skyrim SE)
- Exact Havok SDK version unconfirmed (Open Questions)
- FO4-era tool round-trip unverified (Open Questions)

## Recommendation

- **Do NOT** rewrite the guide to claim Starfield has no Havok animation / uses
  SAF/NAF/glTF — that would add unverified claims contradicting primary-source
  evidence (violates anti-hallucination policy).
- Re-open S2.3.1 as PASS: the guide (121 lines) is factually consistent with the
  confirmed research doc and the live CommonLibSF source.
- Close SYNC-2 as a stale-citation false positive (or re-verify against the current
  `.opencode/docs/starfield.md` before acting on it).

## Files touched by this Worker
- `reference/bethesda/starfield-guide.md` — MODIFY (99→121 lines): folded confirmed
  AnimGraph + BA2 facts; added BSArch source; removed answered "CE2 archive details"
  open question; added FO4-era tool round-trip open item; noted CommonLibSF→libxse move.
