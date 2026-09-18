---
title: "FC4/FC5/FC6 .move.bin format"
---

# .move.bin (Animation State Machine)

Compiled animation state machines that define how animations blend, transition,
and are controlled by game parameters. Part of the Dunia "move" system.

## Format — RESOLVED (Sep 2026)

### combinedmovefile.bin (FC5/ND/FC6)
```
u32 version          (64=FC5, 65=ND, 85=FC6)
u32 moveDataSize     (size of animation tree data)
u32 fcbDataSize      (size of FCbn binary object data)
byte[] moveData      (animation trees with classNameType dispatch)
byte[] fcbData       (FCbn: PerMoveResourceInfo + OffsetsHashesArray)
```

PerMoveResourceInfo (from fcbData): 34951 entries with file paths (`.mab` animation
files), sizes, and rootNodeIds. File paths decode to readable animation paths like
`animations\legacy\locomotion\transition\panic\run\stops\...`.

### movedef.move.bin (FC4)
```
u16 version          (22093=FC4 "MV")
u16 unknown
byte[] header        (complex metadata, tree data starts at ~0x523)
byte[] treeData      (classNameType dispatch system)
```

### classNameType dispatch (both formats)
Each node starts with a classNameType byte (7-47) that determines the binary layout:

| File | Game | Size | CRC64 | Description |
|------|------|------|-------|-------------|
| `movedef.move.bin` | FC4 | 11MB | `0x49137233ebb533d4` | Core move definitions |
| `movedefnamed.move.bin` | FC4 | 29MB | — | Named variant (more entries) |
| `combinedmovefile.bin` | FC5 | 40MB | — | Combined root state machine |

Source: sharp_razor8 (Discord), samir (MoveValueDefinitions)

## MoveValueDefinitions

Game parameters that drive the animation state machine:

| Index | Name | Type |
|-------|------|------|
| 5 | HeadingAngle | angle |
| 6 | FacingAngle | angle |
| 7 | Speed | float |
| 12 | AimStance | enum |
| 21 | EquippedWeapon | enum |
| 47 | MentalState | enum |
| 58 | IsSprinting | bool |
| 60 | IronSightTransition | float |
| 82 | LastHitDirection | angle |
| 83 | SpeedWhenHurt | float |
| 86 | PGM_TargetPlantAngle | angle |
| 87 | CoverHidingBehind | bool |
| 91 | PGM_TargetSpeed | float |
| 93 | PGM_TargetMoveDirection | angle |
| 94 | PGM_TargetFaceDirection | angle |
| 101 | Identity_AnimationSet | enum |
| 175 | Identity_MetaAnimationSet | enum |
| 225 | TurnAngle | angle |

Source: samir (Discord), MabTools CombinedMoveFile.cs

## FC4 vs FC5 differences

- FC4 uses simpler state machines (no decision trees or complex blends)
- FC5/ND add decision trees, motion matching, and procedural animation
- FC6 is partial — limited knowledge

## Tools

- **MabTools** (Jakub Mareček) — `.move.bin` ↔ XML conversion via
  `MoveConvertBin()` / `MoveConvertXml()` in FCBConverter fork
- **FCBConverter** — can convert the binary object data to XML

## Source

- MoveBin Research directory (FC4 movedef, FC5 combinedmovefile samples)
- MabTools-main/FCBConverter/CombinedMoveFile.cs (animation parameter system)
- Discord Far Cry modding community (legendhavoc175, BIRDdude12, sharp_razor8)
