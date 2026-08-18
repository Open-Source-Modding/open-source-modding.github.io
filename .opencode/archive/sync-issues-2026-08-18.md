# Sync Issues

## SYNC-1: reference/index.md — duplicate Bethesda entries (resolved)

**Status**: resolved
**Found by**: Commander (2026-08-18T12:30:42Z)
**File**: reference/index.md
**Problem**: write race between two parallel hub-sync Workers produced duplicate
Bethesda entries (6 instead of 4).
**Fix**: duplicate trio removed — each guide now listed exactly once.
**Verified by**: Reviewer (2026-08-18T12:31:30Z)
**Verification evidence**:
- `grep -c 'skyrim-havok-guide.md' reference/index.md` → 1 ✅
- `grep -c 'oblivion-remastered-guide.md' reference/index.md` → 1 ✅
- `grep -c 'starfield-guide.md' reference/index.md` → 1 ✅
- All 11 files referenced in index.md exist ✅
- README Bethesda section lists all 4 guides; hashing link → reference/watch_dogs/hashing.md (exists) ✅