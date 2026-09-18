# Far Cry 5 — EStimType / EStimFilterSubType (from the FC5 DLL)

Recovered from the Far Cry 5 executable via IDA (attributed to Legendhavoc; values
match the FC5 DLL's shipped enum set and are expected to be shared with New Dawn /
the Dunia-lineage engine). Reference for AI pawn stim/filter tuning in mods and
entity libraries. These appear in `entitylibrary.fcb` / AI definitions as the
stim type and filter-subtype fields.

## EStimType

| Value | Name | Notes |
|------:|------|-------|
| 0 | STIM_TYPE_NONE | |
| 1 | STIM_TYPE_BANG | |
| 2 | STIM_TYPE_SNAP | |
| 3 | STIM_TYPE_PIERCE | DamageType: Pierce |
| 4 | STIM_TYPE_CRUSH | DamageType: Crush |
| 5 | STIM_TYPE_CUT | DamageType: Cut |
| 6 | STIM_TYPE_HEALTH | only affects health |
| 7 | STIM_TYPE_BURN | fire damage |
| 8 | STIM_TYPE_WATER | |
| 9 | STIM_TYPE_PANIC | |
| 10 | STIM_TYPE_FEAR | |
| 11 | STIM_TYPE_MORALE | |
| 12 | STIM_TYPE_DIRT | |
| 13 | STIM_TYPE_UNUSED_0 | community labels "Reliability/Bark" |
| 14 | STIM_TYPE_INTEREST | |
| 15 | STIM_TYPE_STUN | drops player to ground in Play mode |
| 16 | STIM_TYPE_EXPLOSION | |
| 17 | STIM_TYPE_BOOM | does nothing |
| 18 | STIM_TYPE_TOXIC | |
| 19 | STIM_TYPE_BLEEDOUT | |
| 20 | STIM_TYPE_POISON_APPLY | |
| 21 | STIM_TYPE_POISON_DMG | |
| 22 | STIM_TYPE_ANIMAL_CRUSH | |
| 23 | STIM_TYPE_LAST | terminal sentinel (enum ends here in DLL) |

## EStimFilterSubType

| Value | Name | Notes |
|------:|-------|-------|
| 0 | STIM_FILTER_SUBTYPE_NONE | |
| 1 | STIM_FILTER_SUBTYPE_EXPLOSION_GRENADE | |
| 2 | STIM_FILTER_SUBTYPE_EXPLOSION_RPG | |
| 3 | STIM_FILTER_SUBTYPE_EXPLOSION_IED | |
| 4 | STIM_FILTER_SUBTYPE_EXPLOSION_SPLASH | |
| 5 | STIM_FILTER_SUBTYPE_ARROW | |
| 6 | STIM_FILTER_SUBTYPE_BURN_PAWNSONLY | Fire/Tamang_Oven |
| 7 | STIM_FILTER_SUBTYPE_RIDDEN_ANIMAL_CRUSH | |
| 8 | STIM_FILTER_SUBTYPE_INSANITY | |
| 9 | STIM_FILTER_SUBTYPE_EXPLOSION_MOLOTOV | |
| 10 | STIM_FILTER_SUBTYPE_SPREADING_POISON | Base_NPC_Brawler_Cult_Blessed_Knife_DLZ_BEHEMOTH |
| 11 | STIM_FILTER_SUBTYPE_ANIMAL | |
| 12 | STIM_FILTER_SUBTYPE_EXPLOSION_SCATTER | |
| 13 | STIM_FILTER_SUBTYPE_KICKING | |
| 14 | STIM_FILTER_SUBTYPE_CUT_BACKSTAB | |
| 15 | STIM_FILTER_SUBTYPE_BEES | |
| 16 | STIM_FILTER_SUBTYPE_KNIFE | |
| 17 | STIM_FILTER_SUBTYPE_VEHICLE | |
| 18 | STIM_FILTER_SUBTYPE_COUNT | terminal sentinel (enum ends here in DLL) |

## Notes / divergences vs community-extracted lists

- Community threads sometimes list up to 30/31 stim values (with labels like
 Alcohol/Drunk, Drugs, bliss variants, Gravity_Factor, Slowdown, Freeze,
 Disable). Those extra labels beyond the DLL enum (23/18) appear to be runtime
 AI-flavor/behavior tags in newer games (FCND or FC6), not the FC5 shipped enum.
 The canonical FC5 DLL enum terminates at `STIM_TYPE_LAST = 23` and
 `STIM_FILTER_SUBTYPE_COUNT = 18`.
- `STIM_TYPE_BOOM` and `STIM_FILTER_SUBTYPE_BURN_PAWNSONLY` are documented
 engine-side as intentionally inert ("does nothing").