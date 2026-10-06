# Watch Dogs 2 status effect and skill hashes

> **Source**: Watch Dogs 2 status effect hash list.

The entries below are 32-bit hash values used by Watch Dogs 2 status effects and
skill upgrades, together with the human-readable names that could still be
resolved. They were recovered from a status-effect hash list, so the set is a
partial extraction rather than a complete enumeration of the game's upgrades.
The names cover the effect families the list could identify: object, vehicle and
NPC hacking, gadgets, the RC jumper and RC drone, weapons, health, the Aggressor,
Takedown, Ghost and Trickster skill trees, and the botnet upgrades.

Every hash is grouped below by family, then split into the entries whose name is
known, the entries that are named but no longer used, and the entries that are
still unnamed.

## Hash algorithm

The values are a standard 32-bit CRC32 of the exact case-sensitive name, stored
byte-swapped (little-endian). In other words, take the CRC32 of the name as a
32-bit integer, reverse its four bytes, and write the result as eight hex digits.

Recipe, verified on twelve resolved names including `ObjectHacking_Shutdown`,
`VehicleHacking_Brakes` and `RCJumper_Taser`: all twelve reproduced their listed
value exactly. For example `CRC32("ObjectHacking_Shutdown")` is `0x91A7795D`, which
byte-reversed is `0x5D79A791`, the value in the list.

```bash
python3 -c "import zlib,sys;print('0x%08X'%int.from_bytes((zlib.crc32(sys.argv[1].encode())&0xFFFFFFFF).to_bytes(4,'big'),'little'))" ObjectHacking_Shutdown
# 0x5D79A791
```

Lowercase or uppercase versions of a name do not match: the hash uses the name
exactly as written, including its underscores and capitalisation.

Two rows in the source list do not fit this rule and are marked `name unverified`
in the tables:

- `0x2EBA26A2` is listed twice as `NPCHacking_ExposeToPolice`, but only one of the
  duplicate rows, `0xDEA3A3F5`, equals the CRC32 of that name. The second hash
  belongs to a different, still unnamed effect.
- `0x9F0B3124` is listed as `Weapon_SniperDamage`, but CRC32 of that name is
  `0xFFEB2E49`, the value the list gives against the placeholder `Weapon_???Damage`.
  So `0xFFEB2E49` is `Weapon_SniperDamage`, and the real name of `0x9F0B3124` is
  still unknown.

## Resolved entries

### Object hacking

| Hash | Name | Status |
|------|------|--------|
| `0x5D79A791` | `ObjectHacking_Shutdown` | Resolved |
| `0xEE5C61CA` | `ObjectHacking_ArmHack` | Resolved |
| `0x8C46BB4A` | `ObjectHacking_CostReduction` | Resolved |

### Vehicle hacking

| Hash | Name | Status |
|------|------|--------|
| `0x1852B252` | `VehicleHacking_Brakes` | Resolved |
| `0x826BD28F` | `VehicleHacking_EngineOverheat` | Resolved |
| `0x733A320D` | `VehicleHacking_CostReduction` | Resolved |
| `0xD16701B2` | `Robot_RobotHacking` | Resolved |
| `0x6CDAD773` | `Vehicle_CarNitroEnhanced` | Resolved |
| `0x5418AA19` | `MassHack_Blackout` | Resolved |

### NPC hacking

| Hash | Name | Status |
|------|------|--------|
| `0xDEA3A3F5` | `NPCHacking_ExposeToPolice` | Resolved |
| `0x2EBA26A2` | `NPCHacking_ExposeToPolice` | Resolved (name unverified) |
| `0xF327D9ED` | `NPCHacking_CostReduction` | Resolved |
| `0xB1363673` | `NPCHacking_DistractCombatUpgrade` | Resolved |
| `0xCD82DD10` | `NPCHackingGangWarLevel2` | Resolved |
| `0x5BB2DA67` | `NPCHackingGangWarLevel3` | Resolved |
| `0x0BBAD103` | `NPCHackingCallCopLevel2` | Resolved |

### Gadgets

| Hash | Name | Status |
|------|------|--------|
| `0x38BEC5E4` | `Gadgets_ZapperQuantityUpgrade` | Resolved |
| `0xC81E5A51` | `Gadgets_IEDQuantityUpgrade` | Resolved |
| `0x22F8748B` | `Gadgets_ImprovedIED` | Resolved |
| `0x22E4BF73` | `Gadgets_ImprovedZapper` | Resolved |
| `0x775AFAD3` | `Gadgets_RadialHackCostReduction` | Resolved |
| `0x3DBF2DA3` | `Gadgets_RadialHackRangeUpgrade` | Resolved |
| `0xBC1BA788` | `Gadgets_Gadgeteer` | Resolved |

### RC jumper

| Hash | Name | Status |
|------|------|--------|
| `0x9750548D` | `RCJumper_DamageResistance` | Resolved |
| `0x0F9D4848` | `RCJumper_CooldownReduction` | Resolved |
| `0xDE185BAE` | `RCJumper_JumpEnhancement` | Resolved |
| `0x82130338` | `RCJumper_Taunt` | Resolved |
| `0x4C3BFB2F` | `RCJumper_SpeedBoost` | Resolved |
| `0xE8121BBD` | `RCJumper_InteractUpgrade` | Resolved |
| `0x79DC6EAB` | `RCJumper_SharedInventory` | Resolved |
| `0x290B85AF` | `RCJumper_SpeedBoostEnhancement` | Resolved |

### RC drone

| Hash | Name | Status |
|------|------|--------|
| `0x72B6506F` | `RCDrone_ProximityScanner` | Resolved |
| `0x85306B5A` | `RCDrone_CooldownReduction` | Resolved |
| `0x7254E994` | `RCDrone_ProximityScannerUpgrade` | Resolved |
| `0xCF5D1D60` | `RCDrone_SharedInventroy` | Resolved |
| `0x7339A4F8` | `RCDrone_SpeedBoost` | Resolved |

### Weapons

| Hash | Name | Status |
|------|------|--------|
| `0x8CED1C27` | `Weapon_TaserFireRate` | Resolved |
| `0x41FD4620` | `Weapon_TaserRange` | Resolved |
| `0xC087D0AC` | `Weapon_TaserReload` | Resolved |
| `0x69ECE8DE` | `Weapon_TaserDamageVsArmor` | Resolved |
| `0xFA97CADC` | `Weapon_PistolDamage` | Resolved |
| `0xC30DBA6F` | `Weapon_PistolReload` | Resolved |
| `0x1B175C01` | `Weapon_PistolFireRate` | Resolved |
| `0x72AF7AE2` | `Weapon_AssaultRifleStabilityImpulseLimit` | Resolved |
| `0x95F15F99` | `Weapon_AssaultRifleStabilityRecoilImpulse` | Resolved |
| `0xC6715EFA` | `Weapon_SniperReload` | Resolved |
| `0x9F0B3124` | `Weapon_SniperDamage` | Resolved (name unverified) |
| `0x9C338AEA` | `Weapon_SniperStabilityImpulseLimit` | Resolved |
| `0x76CEE5DE` | `Weapon_SniperStabilityRecoilImpulse` | Resolved |
| `0xC6625EF2` | `Weapon_SniperStabilityCamShake` | Resolved |
| `0x9D7180DE` | `Weapon_ShotgunReload` | Resolved |
| `0xDAB897AA` | `Weapon_ShotgunStabilityInitialRecoil` | Resolved |
| `0x018FD6F2` | `Weapon_ShotgunStabilityImpulseLimit` | Resolved |
| `0x337D4350` | `Weapon_ShotgunStabilityRecoilImpulse` | Resolved |
| `0xC63904A6` | `Weapon_ShotgunDamageVsArmor` | Resolved |
| `0xCF74293B` | `Weapon_ShotgunDamageVsVehicle` | Resolved |
| `0xC9720B12` | `Weapon_AutoHackBullet` | Resolved |
| `0xBBCF8BA8` | `Weapon_AutoTagBullet` | Resolved |
| `0xBCDCE46E` | `Weapon_InfiniteAmmo` | Resolved |

### Health

| Hash | Name | Status |
|------|------|--------|
| `0xDB69AF6A` | `Health_RegenRate` | Resolved |
| `0x99402BFC` | `Health_RegenDelay` | Resolved |
| `0x8EA29915` | `Health_ShieldSize` | Resolved |
| `0xD8A3E4CA` | `Health_ShieldRechargeSpeed` | Resolved |

### Aggressor

| Hash | Name | Status |
|------|------|--------|
| `0x975BDB71` | `AggressorMeleeKnockout` | Resolved |
| `0xC8316DA4` | `AggressorVehicleHacking` | Resolved |
| `0x2039B9E6` | `AggressorIEDs` | Resolved |
| `0x398785A9` | `AggressorArmoredMelee` | Resolved |

### Takedown

| Hash | Name | Status |
|------|------|--------|
| `0xAD6CC9C9` | `TakedownGates` | Resolved |
| `0xDED5F9F5` | `TakedownSteamPipes` | Resolved |
| `0x9E0F0946` | `TakedownTraficLights` | Resolved |

### Ghost

| Hash | Name | Status |
|------|------|--------|
| `0x256094CE` | `GhostStealthMelee` | Resolved |
| `0xE9F065B1` | `GhostNPCHacking` | Resolved |
| `0x86C81858` | `GhostZappers` | Resolved |
| `0x5BE09ED0` | `GhostHideInCar` | Resolved |
| `0x4A5432B0` | `GhostHijacker` | Resolved |

### Trickster

| Hash | Name | Status |
|------|------|--------|
| `0xE821B092` | `TricksterNetHacking` | Resolved |
| `0x35D7920B` | `TricksterXIngredientsHack` | Resolved |
| `0x48089EB4` | `TricksterSecurityCamera` | Resolved |
| `0x430D9325` | `TricksterRCHacks` | Resolved |
| `0x866265B7` | `TricksterRCJumper` | Resolved |
| `0x7C53D05E` | `TricksterMassHack` | Resolved |
| `0x43A07622` | `TricksterRCDrone` | Resolved |
| `0x14886E70` | `TricksterImprovedProfiler` | Resolved |
| `0x3F802BA6` | `TricksterMassDistact` | Resolved |
| `0xB2FE3DBC` | `TricksterMassVehicleHack` | Resolved |
| `0x193D4F6A` | `TricksterRobotHacks` | Resolved |
| `0x15A0608F` | `TricksterCarNitro` | Resolved |
| `0xC1C65D9F` | `TricksterChopperHacks` | Resolved |

### Botnet upgrades

| Hash | Name | Status |
|------|------|--------|
| `0x66C3B440` | `UpgradeBotnetLevel1` | Resolved |
| `0xDC92BDD9` | `UpgradeBotnetLevel2` | Resolved |
| `0x4AA2BAAE` | `UpgradeBotnetLevel3` | Resolved |
| `0xE937DE30` | `UpgradeBotnetLevel4` | Resolved |
| `0x7F07D947` | `UpgradeBotnetLevel5` | Resolved |
| `0xC556D0DE` | `UpgradeBotnetLevel6` | Resolved |
| `0x5366D7A9` | `UpgradeBotnetLevel7` | Resolved |

## Unused entries

These hashes have a resolved name but are flagged as no longer used by the game.

| Hash | Name as listed | Status |
|------|----------------|--------|
| `0xCE131936` | `RCJumper_Taser (UNUSED)` | Unused |
| `0x8E350C5F` | `RCDrone_SpeedIncrease (UNUSED)` | Unused |
| `0xC2446734` | `RCDrone_NoiseReduction (UNUSED)` | Unused |
| `0x617403B5` | `RCDrone_ElectronicStealth (UNUSED)` | Unused |
| `0xBC5F4246` | `Weapon_PistolRange (UNUSED)` | Unused |
| `0xCF7A8102` | `Weapon_PistolStabilityInitialRecoil (UNUSED)` | Unused |
| `0xACCFCB83` | `Weapon_PistolStabilityImpulseLimit (UNUSED)` | Unused |
| `0x26BF55F8` | `Weapon_PistolStabilityRecoilImpulse (UNUSED)` | Unused |
| `0x12CD9E09` | `Weapon_SMGReload (UNUSED, NOT REAL NAME)` | Unused |
| `0x48B7EFC6` | `Weapon_SMGInitialRecoil (UNUSED, NOT REAL NAME)` | Unused |
| `0x55F04C8C` | `Weapon_SMGStabilityImpulseLimit (UNUSED, NOT REAL NAME)` | Unused |
| `0xA1723B3C` | `Weapon_SMGStabilityRecoilImpulse (UNUSED, NOT REAL NAME)` | Unused |
| `0xD1471B02` | `Weapon_SMGFireRate (UNUSED, NOT REAL NAME)` | Unused |
| `0x10D801D1` | `Weapon_SMGDamageVsArmor (UNUSED, NOT REAL NAME)` | Unused |
| `0x3869F55D` | `Weapon_AssaultRifleReload (UNUSED)` | Unused |
| `0x90474AD1` | `Weapon_AssaultRifleRange (UNUSED)` | Unused |
| `0x63B8CBBC` | `Weapon_AssaultRifleFireRate (UNUSED)` | Unused |
| `0x7C348B63` | `Weapon_AssaultRifleStabilityInitialRecoil (UNUSED)` | Unused |
| `0x59682E4F` | `Weapon_AssaultRifleClipSize (UNUSED)` | Unused |
| `0xAD4B9825` | `Weapon_SniperFireRate (UNUSED)` | Unused |
| `0x8725D777` | `Weapon_ShotgunRange (UNUSED)` | Unused |
| `0xF57327D6` | `Weapon_GrenadeLauncherFireRate (UNUSED)` | Unused |
| `0xBAE68BB6` | `Weapon_GrenadeLauncherDamage (UNUSED)` | Unused |
| `0x24502D5E` | `AggressorGunFinisher (UNUSED)` | Unused |
| `0xD0ABAB01` | `AggressorBruiserSpeedModifier (UNUSED)` | Unused |
| `0xAF299E37` | `AggressorBruiserDamageModifier (UNUSED)` | Unused |
| `0x8CDFCA48` | `GhostParkourMaster (UNUSED)` | Unused |
| `0x0D0AAC7A` | `GhostNinja (Unused)` | Unused |
| `0x9D8AD674` | `NPCHackingCallCopLevel3 (UNUSED)` | Unused |

## Unresolved entries

These hashes have no confirmed name. Some are marked `NO CODE LEFT` in the source,
some are outright `UNKNOWN`, and the `?` and `*` entries only preserve a family
guess. They remain open and need a name source.

| Hash | Label | Status |
|------|-------|--------|
| `0x8D0F8D1C` | UNKNOWN | Unknown |
| `0x718FD8BD` | UNKNOWN | Unknown |
| `0xE5F69113` | UNKNOWN | Unknown |
| `0xF6F63D25` | NO CODE LEFT, UNKNOWN | No code left |
| `0xB5A1A039` | NO CODE LEFT, UNKNOWN | No code left |
| `0x37731C71` | UNKNOWN | Unknown |
| `0x5E5AA7F6` | UNKNOWN | Unknown |
| `0xE756459C` | UNKNOWN | Unknown |
| `0x9CD2DC57` | UNKNOWN | Unknown |
| `0x60975B2F` | UNKNOWN | Unknown |
| `0x47E4D16F` | UNKNOWN | Unknown |
| `0x5F5F2FAE` | UNKNOWN | Unknown |
| `0xBFA0891B` | UNKNOWN | Unknown |
| `0x71640D7D` | UNKNOWN | Unknown |
| `0xC3C257AC` | UNKNOWN | Unknown |
| `0x9A583E6D` | UNKNOWN | Unknown |
| `0x987B7833` | UNKNOWN | Unknown |
| `0x7E0F2154` | UNKNOWN | Unknown |
| `0x02B24ED6` | UNKNOWN | Unknown |
| `0xC5CBB977` | UNKNOWN | Unknown |
| `0x92B53022` | UNKNOWN | Unknown |
| `0x90C36E43` | NO CODE LEFT, UNKNOWN | No code left |
| `0xC6FC45C1` | NO CODE LEFT, UNKNOWN | No code left |
| `0xBFAD31C8` | UNKNOWN | Unknown |
| `0xEB87DAB3` | NO CODE LEFT, UNKNOWN | No code left |
| `0x88114346` | UNKNOWN | Unknown |
| `0xEE4966B2` | UNKNOWN | Unknown |
| `0x8B49870E` | UNKNOWN | Unknown |
| `0x5E5CEC97` | Weapon_???InitialRecoil (UNUSED) | Unknown name |
| `0x64DCA751` | Weapon_???StabilityImpulseLimit (UNUSED) | Unknown name |
| `0xB799386D` | Weapon_???StabilityRecoilImpulse (UNUSED) | Unknown name |
| `0x4D1BECCA` | Weapon_???FireRate (UNUSED) | Unknown name |
| `0xFFEB2E49` | Weapon_???Damage (UNUSED) | Unknown name |
| `0x486D696A` | Aggressor* UNKNOWN (UNUSED) | Unknown name |
| `0x267DC2EC` | NO CODE LEFT, UNKNOWN | No code left |
| `0x8C73E146` | Aggressor* UNKNOWN (UNUSED) | Unknown name |
| `0xBC03A3CD` | Aggressor* UNKNOWN (UNUSED) | Unknown name |
| `0xDC7AEBAA` | Aggressor* UNKNOWN (UNUSED) | Unknown name |
| `0x8FE8469E` | Ghost* UNKNOWN (UNUSED) | Unknown name |
| `0xD1D3E531` | Ghost* UNKNOWN (UNUSED) | Unknown name |
| `0x5044F364` | Ghost* UNKNOWN (UNUSED) | Unknown name |
| `0xAEFE2479` | Ghost* UNKNOWN (UNUSED) | Unknown name |
| `0xCF927038` | Ghost* UNKNOWN (UNUSED) | Unknown name |
| `0x7386821F` | Ghost* UNKNOWN (UNUSED) | Unknown name |
| `0xF4E29C67` | Trickster* UNKNOWN (UNUSED) | Unknown name |

## Status of this list

The list is incomplete. It records the hashes that were recovered from one
status-effect dump, but it does not cover every status effect or skill upgrade in
the game, and a large block of hashes still has no name. Matching a hash back to a
name only works in one direction: a known name can be hashed to confirm its value,
but an unnamed hash cannot be reversed. The unresolved entries therefore need a
name source, such as the item and upgrade definitions that ship with the game, to
be identified.
