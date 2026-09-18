---
title: "FC4/FC5/FC6 .move.bin format"
---

# .move.bin (Animation State Machine)

Compiled animation state machines that define how animations blend, transition,
and are controlled by game parameters. Part of the Dunia "move" system.

## File structure

```
u16 version     (22093=FC4 "VM", 65=ND, 85=FC6)
u16 unknown
byte[] data     (Gibbed.Dunia2 binary object format — NOT nbCF)
```

The binary object data is the same format as other `.fcb` files in the engine,
parsed by Gibbed.Dunia2.ConvertBinaryObject. It uses hash-based field identifiers,
not human-readable names.

## Three file variants

| File | Game | Size | Description |
|------|------|------|-------------|
| `movedef.move.bin` | FC4 | 11MB | Core move definitions |
| `movedefnamed.move.bin` | FC4 | 29MB | Named variant (more entries) |
| `combinedmovefile.bin` | FC5 | 40MB | Combined root state machine |

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
