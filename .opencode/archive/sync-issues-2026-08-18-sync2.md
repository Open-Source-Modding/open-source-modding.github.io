# Sync Issues

## SYNC-2: starfield-guide.md — animation-system claim (closed: false positive)

**Status**: closed
**Found by**: Reviewer (2026-08-18T12:34Z, full-system verification)
**Closed by**: Reviewer (2026-08-18T12:36Z)
**File**: reference/bethesda/starfield-guide.md
**Problem (initial)**: Guide asserted CE2 "keeps the Havok .hkx animation and
behavior file lineage from FO4"; appeared to contradict the research cache
`.opencode/docs/starfield.md` (126-line version), which claimed "no Havok classes
exposed".
**Resolution**: The research doc was UPDATED by a parallel session (now 173 lines)
with primary-source evidence that CONFIRMS the guide: CommonLibSF exposes the
AnimGraph system (BSAnimationGraph, BSAnimationGraphManager,
BGSAnimationGraphComponent, IAnimationGraphManagerHolder - starfield.md:29-39);
`.hkx` files carry hkbBehaviorGraph/hkaSkeleton/hkaAnimationBinding in
project/character/behavior sets (starfield.md:42-44); `.hkx` is a first-class
`meshes` asset-folder extension in xEdit wbBSArchive.pas (starfield.md:54, 90-91).
The guide's Open Questions mirror the research gaps exactly (Havok contents-version
string, .hkx class layout vs FO4, behavior-authoring tooling - starfield.md:47-48,
55-56). Guide requires NO changes.
**Verification evidence**:
- starfield.md:26-27 "Starfield retains the Havok behavior/animation .hkx lineage"
- starfield.md:42-44 ".hkx files ... carry behavior graphs (hkbBehaviorGraph),
  skeletons (hkaSkeleton), and animation bindings (hkaAnimationBinding)"
- starfield.md:54 ".hkx is a first-class meshes asset-folder extension in BSArch"
- Guide lines 19-22, 72-81 consistent with all of the above
**Impact**: S2.3.1 and S4.1.1 marked [x] after re-verification. No sync issues remain.
