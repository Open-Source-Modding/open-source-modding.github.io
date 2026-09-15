# Far Cry MAB animation format (buu342's RE thread, knockout.chat/thread/55079)

Source: [Deciphering Far Cry 3's animation format](https://knockout.chat/thread/55079) by **buu342** (Nov 2023 – Jun 2024, Portugal). Thread archived locally at `/home/selene/Documents/Code/re/Ubisoft/Dunia/knockout-chat-thread-55079.md` (803 lines). Companion code: **github.com/buu342/PY-DuniaAnimationExtractor** (Python + GitHub Wiki with all findings). Focus: FC3 `.mab`; method = Cheat Engine watchpoints + Ghidra on FC3.dll + FC2 cross-referencing.

## Why this matters
The community's long-standing blocker: *"you can't add animations"* / *"no one has researched .mab enough"* (sharp_razor8, FC3/BD server, Aug 2025). MAB = animation, XBG = model, XBT = texture, HKX = Havok physics. FC3/BD share an engine; FC5/ND share one; FC6 is trickier; FC4/FC3 need a move.bin edit too.

## Version bytes (first 4 bytes, little-endian)
| Value | Game |
|-------|------|
| 0x0000004C | Far Cry 2 |
| 0x00000061 | Far Cry 3 |
| 0x00000062 | Far Cry 3: Blood Dragon |
| 0x00000081 | Far Cry 4 |
| 0x00000082 | Far Cry Primal |
| 0x000000B0 | Far Cry 5 + New Dawn |

## Section offset table (at file offset 200 = 0xC8)
- Nine 4-byte numbers = byte offsets of 9 sections; **offsets are relative to +16** (add 16 to get real offset).
- Offset 0 = section does not exist.
- No size fields — sizes = sort offsets, subtract neighbors. First two sections are the exception (not in ascending order).
- Sections aligned to 16 bytes, zero-padded.
- Section roles (FC3): 1-2 unknown; 3 = YZX rotations for the model root bone; 4 = per-bone rotation data; 5 unknown; 6 = offset animations (whole model only?); 7 = event data (attachments, particles); 8-9 unknown.

## Key fields
- **0xC4: float ANIMLENGTH** — animation length in seconds.
- Rotation section: first 4 bytes = two u16 (unknown), then a **4-byte float "magic float"**, then per-bone offsets into the rotation data (last offset = section size in bytes).
- Keyframe-section-offset formula: `((int)(magicfloat*ANIMLENGTH)) >> 3)*4 + 8`.

## Quaternion storage (the big win)
- FC3 quaternions are **48-bit compressed**: 1 unused bit + 2 bits (largest component) + 15 bits × 3 components. W is reconstructed via square root (the unpack code does `sqrt(1 - x²-y²-z²)`; 0.7071 = sin(45°) is the tell — that's how buu342 spotted it in FC2's lighter-optimized DLL).
- The quaternion unpacker was located in **FC3.dll** (not FC3Editor.exe) via Cheat Engine watchpoint on rotation data; the game keeps the entire MAB in memory and reads from it directly (memory edits take effect live — no repack needed for testing).
- Function args learned from watchpoints: 5th/6th = pointers to animation data (+16), 3rd = bone count.

## Keyframe section layout (mystery byte decoded)
- Each keyframe section: first quaternion + **2 mystery bytes**, then subsequent quaternions stored sequentially.
- **Mystery byte = bitmask counting quaternions**: popcount = number of quaternions packed after the first (0x47 = 01000111 = 4 ones = 4 quats; 0x0F = 00001111 = 4). First nibble = transition speed to next keyframe; second nibble breaks animation; second mystery byte unknown (bone selector?).
- Open: why two bit representations for same count; behavior with >8 quats (8-bit mask limit); multi-bone mapping (bone→offset table likely built at load from XBG, not in the MAB).

## Frame interpolation (legendhavoc175, Jun 2024)
- MAB supports interpolation; extracted "keyframes" are interpolation targets, not every frame. FC4+ have a **frame array** defining playback order (0,1,2,3... non-culled; 0,4,9,16... culled/skipped).
- Quaternion data per section: typically 6 bytes quat + 2 bytes pad (0 or random); complex animations vary.

## Method notes (reusable)
1. FCBConverter patch workflow: create `mymod/foo/foo2/bar.tex` matching asset path → feed folder to FCBConverter → small binary .dat/.fat patch placed in data_win32 (overrides the default patch; works in map editor + Linux/Wine).
2. CE watchpoint on known animation bytes → stack trace → FC3.dll + function offset → Ghidra.
3. FC2 has lighter compiler optimizations → cleaner Ghidra output for the same algorithm; cross-search distinctive constants (4.315969e-05, 0.7071068) in the FC3.dll C export to find the equivalent function.
4. The .skeleton file only exists for physics models — first-person models need XBG parsing for bone data instead.

## Related resources (from thread)
- [ResHax RE tutorials master thread](https://reshax.com/topic/47-reverse-engineering-tutorials-collection/) (many links → archived XeNTaX)
- [3D Model Researcher](https://mr.game-viewer.org/) (3D model RE tool)
- ID-Daemon YouTube tutorials (3D model RE; filter out animation test videos)
- FC4 (Oct 17, 2014 X360 prototype) + Watch Dogs 1 X360 prototypes have **debug symbols** (hiddenpalace.org) — named duniademo executables
- Watch Dogs: Legion source code was leaked (several years ago) — avoid unless desperate
- FC2 fire-spreading dev blog (jflevesque.com/2012/12/06) as example of devs discussing systems

## Open questions
- Exact rotation data storage beyond 48-bit quats (padding semantics, multi-bone mapping)
- Which structures in XBG set up the bone→offset lookup table
- FC4+ frame-array exact format
- FC6 MAB differences (v0xB0-family = FC5+ND; FC6 version unknown)
