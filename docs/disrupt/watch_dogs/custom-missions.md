# Watch Dogs Custom Missions

Source: community mission script reference and an example mission pack by Silver.

Watch Dogs custom missions are authored as Lua tables rather than binary data. Each
mission is a table assigned into a global `MC`, with an id, an author, localized
title and description, and a `Sequence` array that lists the mission start point,
the objectives in order, any optional enemies, and the finishing rewards.

This page follows the community field reference for the script format, then shows
how those fields appear in a real mission pack. Where the sources are silent on a
value, that is noted rather than guessed.

## File shape and conventions

A mission file initializes the global once and then appends missions:

```lua
MC = MC or {}
MC['250228160902'] = {}
MC['250228160902'].ID = '250228160902'
MC['250228160902'].Author = 'Silver'
MC['250228160902'].Title = { ['?'] = '195343' }
MC['250228160902'].Description = { ['Unknown'] = '41924' }
MC['250228160902'].Completed = 0
MC['250228160902'].Sequence = {}
```

Every setting is written as an indexed assignment of the form `['label'] = value`:

```lua
MC['250228160902'].Sequence[1]['Distance'] = { ['  15m'] = 15 }
MC['250228160902'].Sequence[1]['Requires_Completed'] = { ['none'] = 'MissionList.2585895465' }
MC['250228160902'].Sequence[2]['Timeout'] = { [' none'] = 0 }
```

Two conventions matter when reading or editing these files:

- The table key is a display label, usually padded with spaces so a value lines up
  in the editor (`['  15m'] = 15`, `['   2m']`, `[' 300s']`). The key often carries
  the unit or the text the game shows; the value is the number or id the game uses.
- A missing or disabled option is written with a marker label such as
  `['none'] = 0`, `['none'] = ''`, `['unchanged'] = -1`, or `[' none'] = 0`. The
  marker differs per field, so keep the exact spelling the field uses.

`Title` and `Description` map a localization key to a numeric string id:

```lua
MC['250908050190'].Title = { ['ACT_V'] = '175633' }
MC['250908050190'].Description = { ['Challenge'] = '13950' }
MC['250328194717'].Title = { ['TRACKS'] = '189189' }
```

The key is the localization token and the value is the string id. Tokens observed
in the pack include `?`, `Unknown`, `User_Created_Content`, `Challenge`, `Race`,
`Assassination`, and mission-specific names such as `ACT_V`, `TRACKS`,
`CORRUPT_GUARD`, and `LEFT_TURN`. Objective titles use their own small token set,
for example `Objective`, `Reach`, `Target`, `Suspect`, `Tail`, `Steal`, `Drive`,
`Enter`, and `Finish`.

## Mission table fields

| Field | Meaning |
|-------|---------|
| `ID` | The mission id, repeated as a string. |
| `Author` | The mission author. |
| `Title` | Localized mission title: `{ [token] = '<string id>' }`. |
| `Description` | Localized mission description: `{ [token] = '<string id>' }`. |
| `Completed` | Completion state. Written as `0` in every sampled mission. |
| `Sequence` | Array of steps: the start point, objectives, optional enemies, and rewards. |

## Mission id observation

The mission ids in the pack look like `YYMMDDHHMMSS` timestamps. For example
`250228160902` reads as 2025-02-28 16:09:02 and `250328194717` as 2025-03-28
19:47:17. Missions authored on the same day sort by the time portion (`250908010649`
through `250908050190`). This is an observation from the id format, not a documented
fact, and the reference says nothing about how ids are generated or reserved.

## Sequence order

`Sequence` is a one-based array. In the pack it follows a consistent pattern:

1. `Sequence[1]` is the start step, named `StartInteraction` or `StartProximity`.
2. The middle entries are objectives, named `Objective<Type>` (`ObjectiveHack`,
   `ObjectiveReach`, and so on).
3. The last entry is `FinishRewards`, which the reference says ends the mission on
   completing the previous objective.

Steps carry their own `Position` and `Name`. A start step can also carry an `Extra`
array holding the `ExtraEnvironment` presentation block, and an objective can carry
an `Extra` array holding `ExtraKill` optional enemies.

## Start types

The reference defines three start types: `INTERACTION`, `PROXIMITY`, and
`AUTOSTART`. In the pack, the step name is `StartInteraction` or `StartProximity`;
`AUTOSTART` is documented but did not appear in the sampled missions.

### INTERACTION start (`StartInteraction`)

Mission start point with a marker and an interaction prompt.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Marker` | The map marker variant for the mission. | Yes |
| `Title` | The mission title. | No |
| `Description` | The description of the mission marker. | Yes |
| `Requires_Completed` | A mission that must be completed before this one becomes available. | Yes |

### PROXIMITY start (`StartProximity`)

Mission start point with a marker, triggered by proximity rather than interaction.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Marker` | The map marker variant for the mission. | Yes |
| `Title` | The mission title. | No |
| `Description` | The description of the mission marker. | Yes |
| `Distance` | The mission starts when you are closer than this radius. | No |
| `Requires_Completed` | A mission that must be completed before this one becomes available. | Yes |

### AUTOSTART start

Mission start point without a marker that starts automatically after a timeout. Not
present in the sampled pack.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Title` | The mission title. | No |
| `Description` | The description of the mission marker. | Yes |
| `Delay` | The number of seconds after which the mission autostarts. | No |
| `Requires_Completed` | A mission that must be completed before this one becomes available. | Yes |

## Objective types

Objectives are named `Objective<Type>`, for example `ObjectiveReach` and
`ObjectiveKill`. The tables below list the fields the reference documents for each
type. `DESTROY` is documented but did not appear in the sampled pack. A `Timeout`
of `{ [' none'] = 0 }` means no time limit; a real limit is written in seconds, such
as `{ ['  60s'] = 60 }`.

### REACH (`ObjectiveReach`)

Mission objective that requires reaching a destination.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Title` | The title of the objective marker. | No |
| `Description` | The description of the objective marker. | Yes |
| `Marker` | The map marker variant of the objective. | No |
| `Distance` | The objective is reached when you are closer than this radius. | No |
| `GPS` | Display a GPS route toward the objective. | Yes |
| `Timeout` | Complete the objective before the time runs out. | Yes |
| `Cinematic` | Show the objective marker through a brief overhead view of the map. | Yes |

### KILL (`ObjectiveKill`)

Mission objective that requires eliminating a target.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Faction` | The faction of the target, including civilian. | No |
| `Class` | The skill of the target. | No |
| `Weapon` | The weapon of the target. | Yes |
| `Takedown` | Change the takedown action on the target to prevented or required. | No |
| `Status` | Target ignores you until provoked, or is already on the lookout for you. | No |
| `Revealed` | Target is already marked, or you must profile him first. | No |
| `Stealth` | Complete the objective without entering combat. | No |
| `Timeout` | Complete the objective before the time runs out. | Yes |

### HACK (`ObjectiveHack`)

Mission objective that requires hacking a target.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Faction` | The faction of the target, including civilian. | No |
| `Class` | The skill of the target. | No |
| `Weapon` | The weapon of the target. | Yes |
| `Takedown` | Change the takedown action on the target to prevented. | No |
| `Status` | Target ignores you until provoked, or is already on the lookout for you. | No |
| `Revealed` | Target is already marked, or you must profile him first. | No |
| `Stealth` | Complete the objective without entering combat. | No |
| `Timeout` | Complete the objective before the time runs out. | Yes |
| `Duration` | Whether the hack is instant or lasts a set number of seconds. | No |
| `Distance` | Maximum distance during the hack, beyond which it is paused. | No |
| `Chase` | Whether the target faction should chase you during the hack. | Yes |

### STEAL (`ObjectiveSteal`)

Mission objective that requires acquiring a vehicle.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Title` | The title of the objective marker. | Yes |
| `Description` | The description of the objective marker. | Yes |
| `Vehicle` | The target vehicle to acquire. | No |
| `GPS` | Display a GPS route toward the objective. | Yes |
| `Timeout` | Complete the objective before the time runs out. | Yes |
| `Cinematic` | Show the objective marker through a brief overhead view of the map. | Yes |
| `Locked` | Require breaking the glass or the Car Unlock skill to enter the vehicle. | No |
| `Destination` | Also deliver the vehicle to a destination. | Yes |
| `Chase` | A faction chases you during the delivery. | Yes |
| `Faction` | The faction of the chasers. | Yes |

### DESTROY (`ObjectiveDestroy`)

Mission objective that requires destroying a vehicle. Documented but not present in
the sampled pack.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Title` | The title of the objective marker. | Yes |
| `Description` | The description of the objective marker. | Yes |
| `Vehicle` | The target vehicle to destroy. | No |
| `GPS` | Display a GPS route toward the objective. | Yes |
| `Timeout` | Complete the objective before the time runs out. | Yes |
| `Cinematic` | Show the objective marker through a brief overhead view of the map. | Yes |
| `Locked` | Require breaking the glass or the Car Unlock skill to enter the vehicle. | No |

### CONVOY (`ObjectiveConvoy`)

Mission objective that requires eliminating a target in a convoy.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Vehicle_Target` | The vehicle the target drives. | No |
| `Vehicle_Lead` | The guards vehicle leading the convoy. | Yes |
| `Vehicle_Trail` | The guards vehicle following the target. | Yes |
| `Faction` | The faction of the target and any guards. | No |
| `Takedown` | Change the takedown action on the target to prevented or required. | No |
| `Destination` | The target should reach a destination, or just drive away. | No |
| `Speed` | The target rushes to its destination, or moves leisurely. | No |
| `GPS` | Display a GPS route to the convoy and eventually to its destination. | Yes |
| `Timeout` | Complete the objective before the time runs out. | Yes |
| `Cinematic` | Show the objective marker through a brief overhead view of the map. | Yes |

### RACE (`ObjectiveRace`)

Mission objective that requires reaching a destination before an opponent.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Title` | The title of the destination marker. | Yes |
| `Faction` | The faction of the opponent, including civilian. | No |
| `Opponent` | The vehicle of the opponent. | No |
| `Takedown` | Whether you can use hack takedowns against the opponent. | No |
| `Destination` | The destination to reach. | No |
| `Countdown` | Display the 3-2-1 race start countdown. | Yes |
| `GPS` | Display a GPS route toward the destination. | Yes |
| `Timeout` | Complete the objective before the time runs out. | Yes |
| `Cinematic` | Show the destination marker through a brief overhead view of the map. | Yes |
| `Chase` | The opponent faction chases you during the race. | Yes |

### TAIL (`ObjectiveTail`)

Mission objective that requires tailing a target.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Title` | The title of the objective marker. | Yes |
| `Faction` | The faction of the target, including civilian. | No |
| `Class` | The skill of the target. | No |
| `Weapon` | The weapon of the target. | Yes |
| `Vehicle` | The vehicle of the target. | Yes |
| `Status` | Target ignores you until provoked, or is already on the lookout for you. | No |
| `Revealed` | Target is already marked, or you must profile him first. | No |
| `Destination` | The destination of the target. | No |
| `Speed` | The target rushes to its destination, or moves leisurely. | No |

### CHASE (`ObjectiveChase`)

Mission objective that requires escaping a chase.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Faction` | The faction of the chasers. | No |
| `Level` | The difficulty level of the chase. | No |
| `Police_Scan` | Whether the chase is preceded by a ctOS Scan (Police faction only). | No |
| `Beginning` | Chasers already know your position, or start by searching for you. | No |
| `Timeout` | Complete the objective before the time runs out. | Yes |

## Optional enemies (`ExtraKill`)

An objective's `Extra` array can hold `ExtraKill` entries: optional enemies for the
current objective. The reference states a limit of up to 20 NPCs total.

| Field | Meaning | Optional |
|-------|---------|----------|
| `Faction` | The faction of the optional enemy, including civilian. | No |
| `Class` | The skill of the optional enemy. | No |
| `Weapon` | The weapon of the optional enemy. | Yes |
| `Status` | The optional enemy ignores you until provoked, or is on the lookout. | No |
| `Patrol` | Allow the optional enemy to patrol the nearby area (for groups). | No |

In the pack, each `ExtraKill` also carries its own `Position`, and the entries are
indexed from `Extra[1]` upward. The block sits directly under the objective that
owns it:

```lua
MC['250328194717'].Sequence[5]['Extra'] = {}
MC['250328194717'].Sequence[5]['Extra'][1] = {}
MC['250328194717'].Sequence[5]['Extra'][1]['Name'] = 'ExtraKill'
MC['250328194717'].Sequence[5]['Extra'][1]['Faction'] = { ['Fixers'] = 'Fixers' }
MC['250328194717'].Sequence[5]['Extra'][1]['Class'] = { ['Grunt'] = 'Grunt' }
MC['250328194717'].Sequence[5]['Extra'][1]['Weapon'] = { ['Pistol'] = 'Pistol' }
MC['250328194717'].Sequence[5]['Extra'][1]['Status'] = { ['Hostile'] = 1 }
MC['250328194717'].Sequence[5]['Extra'][1]['Takedown'] = { ['Allowed'] = 1 }
MC['250328194717'].Sequence[5]['Extra'][1]['Patrol'] = { ['Yes'] = 1 }
```

## Environment block (`ExtraEnvironment`)

`ENVIRONMENT` defines the mission presentation settings. In the pack these fields do
not sit directly on the start step; they appear inside `Sequence[n]['Extra'][n]`
with `Name = 'ExtraEnvironment'`.

| Field | Meaning | Optional |
|-------|---------|----------|
| `TOD` | Change to a defined time of day for the mission. | Yes |
| `Timescale` | Whether the time of day stays frozen during the mission. | Yes |
| `Weather` | Change to a defined weather for the mission. | Yes |
| `Blackout` | Whether a blackout is in effect for the mission. | Yes |
| `Traffic` | Allow or disable vehicle traffic. | No |
| `Boats` | Allow or disable boat traffic. | No |
| `Trains` | Allow or disable trains. | No |
| `Civilians` | Allow or disable civilians. | No |
| `Cinematic` | Play a cinematic masking the environment changes. | Yes |
| `Voiceover` | Select a dialog that describes the mission theme. | Yes |

Observed values in the pack:

- `TOD`: `{ ['unchanged'] = -1 }`, or a clock value such as `{ ['07:00'] = 7 }`,
  `{ ['09:00'] = 9 }`, `{ ['18:00'] = 18 }`, `{ ['19:00'] = 19 }`.
- `Timescale`: `{ ['Freeze'] = 0 }` or `{ ['Normal'] = -1 }`.
- `Weather`: `{ ['unchanged'] = -1 }`, `{ ['Clear'] = 0 }`, `{ ['Rain'] = 1 }`,
  `{ ['Storm'] = 2 }`.
- `Blackout`, `Traffic`, `Boats`, `Trains`, `Civilians`: `{ ['Yes'] = 1 }` or
  `{ ['No'] = 0 }`.
- `Cinematic`: `{ ['Yes'] = 1 }` or `{ ['No'] = 0 }`.
- `Voiceover`: `{ ['none'] = '0x00000000' }`, or a named hash such as
  `{ ['Lair_06'] = '0x000dad2e' }`, `{ ['Convoy_Aiden_15'] = '0x0013225a' }`.

## Objectives: observed values

The reference names the meaning of each field but not the encoding of every value.
The pack shows these labels, which are worth keeping exact:

| Field | Observed values |
|-------|-----------------|
| `Takedown` | `{ ['Allowed'] = 1 }`, `{ ['Prevented'] = 0 }`, `{ ['Required'] = 2 }` |
| `Status` | `{ ['Neutral'] = 0 }`, `{ ['Hostile'] = 1 }` |
| `Revealed` | `{ ['No'] = 0 }`, `{ ['Yes'] = 1 }` |
| `Stealth` | `{ ['Optional'] = 1 }`, `{ ['Required'] = 2 }` |
| `Patrol` | `{ ['No'] = 0 }`, `{ ['Yes'] = 1 }` |
| `GPS` | `{ ['No'] = 0 }`, `{ ['Yes'] = 1 }` |
| `Cinematic` | `{ ['No'] = 0 }`, `{ ['Yes'] = 1 }` |
| `Locked` | `{ ['No'] = 0 }`, `{ ['Yes'] = 1 }` |
| `Countdown` | `{ ['Yes'] = 1 }` |
| `Speed` | `{ ['Slow'] = 0 }`, `{ ['Fast'] = 1 }` |
| `Beginning` | `{ ['Search'] = 0 }`, `{ ['Chase'] = 1 }` |
| `Police_Scan` | `{ ['No'] = 0 }` |
| `Level` | `{ [' 1'] = 1 }`, `{ [' 2'] = 2 }`, `{ [' 5'] = 5 }` |
| `Duration` | seconds, for example `{ ['   0s'] = 0 }`, `{ ['   3s'] = 3 }`, `{ [' 300s'] = 300 }` |
| `Timeout` | `{ [' none'] = 0 }` for none, or seconds such as `{ ['  60s'] = 60 }` |
| `Distance` | metres, for example `{ ['  15m'] = 15 }`, `{ ['   2m'] = 2 }`, `{ [' 100m'] = 100 }` |

Two encodings need care because the same field appears in different forms:

- `Faction` is sometimes a name mapped to itself (`{ ['Fixers'] = 'Fixers' }`) and
  sometimes a name mapped to a number (`{ ['Madness'] = 5 }`, `{ ['Police'] = 0 }`,
  `{ ['Club'] = 1 }`). Factions observed include `Civilian`, `Fixers`, `Police`,
  `Club`, `Madness`, and `Viceroys`.
- `Chase` is a flag in the HACK, STEAL, and RACE objectives. In the pack it is
  `{ ['     No'] = 0 }` on those objectives, with one STEAL instance written as
  `{ ['Level 3'] = 3 }`.

`Class` values observed include `Grunt`, `Veteran`, `Enforcer`, `Marksman`, and
`Elite`. `Weapon` values include `AR`, `Pistol`, `Shotgun`, `SMG`, and
`{ ['none'] = 'none' }`.

## Position layout

Each step carries a `Position` field holding six floats. For example:

```lua
MC['250328194717'].Sequence[1]['Position'] = { -490.960632, -176.593796, 73.254982, 0.000000, 0.000000, 334.853973 }
```

The first three appear to be world coordinates (x, y, z). The last three appear to be
rotation values, and in the sampled data they are most often `0.000000, 0.000000,
<heading>`, where the heading is a value such as `229.474579` or `334.853973`.
Occasionally the first of the three is `360.000000` instead of `0.000000`, for
example `{ 217.901459, -1462.546143, 60.906361, 360.000000, 360.000000, 229.474579 }`.
The sources do not define the axis names or the rotation order, so treat this
description as an observation from the sampled values.

A few steps replace the flat six floats with an array of arrays, one entry per
convoy vehicle or race waypoint:

```lua
MC['250407173212'].Sequence[4]['Position'] = {}
MC['250407173212'].Sequence[4]['Position'][1] = { -1656.942383, 1717.585815, 61.941315, 359.723633, 0.000000, 29.032457 }
MC['250407173212'].Sequence[4]['Position'][2] = { -1652.893311, 1709.658081, 61.991455, 359.657928, -0.000000, 29.024164 }
MC['250407173212'].Sequence[4]['Position'][3] = { -1648.984375, 1702.833496, 61.935783, 0.000000, 0.000000, 29.125475 }
```

`Destination` uses the same six-float form on the objectives that have it, and is a
plain six-float table rather than a `['label'] = value` table.

## Cross-references in data

Several fields point at other game data rather than holding a literal value:

- `Marker` maps a marker variant to an asset reference. The value is either a GUID
  in braces, such as `{8B4A3B31-6D96-4F18-B348-F15325834543}`, or a plain name. The
  pack uses `Missing_Persons`, `Gang_Hideout`, `Target_Normal`, `Convoy`,
  `Online_Race`, and `Weapon_Traffic`.
- `Requires_Completed` references another mission. When it points at the mission
  list, the value is a list path such as `{ ['none'] = 'MissionList.2585895465' }`.
  When it chains to an authored mission, the label and value are both the mission id,
  for example `{ ['250908040725'] = '250908040725' }`. The pack uses both forms.
- `Money` references an item hash, for example
  `{ ['10000$'] = 'Items.2532779827' }` or `{ [' 1000$'] = 'Items.2217939846' }`.
- `XP` and `Components` use an empty string for none,
  `{ ['none'] = '' }`. `Components` can also name a component item, for example
  `{ ['System_Key_1'] = 'Items.2496339441' }`.
- `Vehicle` in STEAL maps a vehicle name to a GUID, such as
  `{ ['Heavy_ArmouredTruck'] = '{44C08C11-6846-784C-37A7-909269BEB2A6}' }`. In
  CONVOY, `Vehicle_Target`, `Vehicle_Lead`, and `Vehicle_Trail` are plain strings,
  such as `'Police_02'` and `'Police_SWAT'`.
- `Opponent` in RACE is a plain string such as `'A04_M05_Limbik'`.

## Worked example

The mission `250908050190` is a chain of a `StartInteraction`, an `ObjectiveHack`, an
`ObjectiveReach`, and `FinishRewards`. It is shown below with the nineteen
`ExtraKill` entries of the hack objective omitted for length; they differ only in
`Position` and `Patrol`.

```lua
MC = MC or {}
MC['250908050190'] = {}
MC['250908050190'].ID = '250908050190'
MC['250908050190'].Author = 'Silver'
MC['250908050190'].Title = { ['ACT_V'] = '175633' }
MC['250908050190'].Description = { ['Challenge'] = '13950' }
MC['250908050190'].Completed = 0
MC['250908050190'].Sequence = {}

-- Start step. Unlocks after mission 250908040725. ExtraEnvironment sets the
-- presentation: 09:00, clear weather, frozen time, a dialog line, and no traffic.
MC['250908050190'].Sequence[1] = {}
MC['250908050190'].Sequence[1]['Name'] = 'StartInteraction'
MC['250908050190'].Sequence[1]['Title'] = { ['ACT_V'] = '175633' }
MC['250908050190'].Sequence[1]['Description'] = { ['Challenge'] = '13950' }
MC['250908050190'].Sequence[1]['Marker'] = { ['Gang_Hideout'] = '{59D56948-08F9-42F9-9509-372545E5989D}' }
MC['250908050190'].Sequence[1]['Requires_Completed'] = { ['250908040725'] = '250908040725' }
MC['250908050190'].Sequence[1]['Position'] = { -1637.942139, 1795.205322, 64.780289, 0.000000, 0.000000, 346.813049 }
MC['250908050190'].Sequence[1]['Extra'] = {}
MC['250908050190'].Sequence[1]['Extra'][1] = {}
MC['250908050190'].Sequence[1]['Extra'][1]['Name'] = 'ExtraEnvironment'
MC['250908050190'].Sequence[1]['Extra'][1]['TOD'] = { ['09:00'] = 9 }
MC['250908050190'].Sequence[1]['Extra'][1]['Weather'] = { ['Clear'] = 0 }
MC['250908050190'].Sequence[1]['Extra'][1]['Timescale'] = { ['Freeze'] = 0 }
MC['250908050190'].Sequence[1]['Extra'][1]['Voiceover'] = { ['Download_06'] = '0x000dadb8' }
MC['250908050190'].Sequence[1]['Extra'][1]['Cinematic'] = { ['Yes'] = 1 }
MC['250908050190'].Sequence[1]['Extra'][1]['Blackout'] = { ['No'] = 0 }
MC['250908050190'].Sequence[1]['Extra'][1]['Civilians'] = { ['No'] = 0 }
MC['250908050190'].Sequence[1]['Extra'][1]['Traffic'] = { ['No'] = 0 }
MC['250908050190'].Sequence[1]['Extra'][1]['Boats'] = { ['No'] = 0 }
MC['250908050190'].Sequence[1]['Extra'][1]['Trains'] = { ['No'] = 0 }

-- HACK objective: an Enforcer of the Fixers, already revealed and hostile, with
-- takedowns allowed, stealth optional, a 300 second hack, a 25 m pause distance,
-- and a 60 second limit.
MC['250908050190'].Sequence[2] = {}
MC['250908050190'].Sequence[2]['Name'] = 'ObjectiveHack'
MC['250908050190'].Sequence[2]['Faction'] = { ['Fixers'] = 'Fixers' }
MC['250908050190'].Sequence[2]['Class'] = { ['Enforcer'] = 'Enforcer' }
MC['250908050190'].Sequence[2]['Weapon'] = { ['AR'] = 'AR' }
MC['250908050190'].Sequence[2]['Status'] = { ['Hostile'] = 1 }
MC['250908050190'].Sequence[2]['Revealed'] = { ['Yes'] = 1 }
MC['250908050190'].Sequence[2]['Takedown'] = { ['Allowed'] = 1 }
MC['250908050190'].Sequence[2]['Stealth'] = { ['Optional'] = 1 }
MC['250908050190'].Sequence[2]['Duration'] = { [' 300s'] = 300 }
MC['250908050190'].Sequence[2]['Distance'] = { ['  25m'] = 25 }
MC['250908050190'].Sequence[2]['Timeout'] = { ['  60s'] = 60 }
MC['250908050190'].Sequence[2]['Chase'] = { ['     No'] = 0 }
MC['250908050190'].Sequence[2]['Position'] = { -1700.063477, 1767.643921, 61.929817, 0.000000, 0.000000, 215.520721 }
MC['250908050190'].Sequence[2]['Extra'] = {}
-- ... 19 ExtraKill entries omitted ...

-- REACH objective: return to the start area, within 2 m, no GPS, no cinematic.
MC['250908050190'].Sequence[3] = {}
MC['250908050190'].Sequence[3]['Name'] = 'ObjectiveReach'
MC['250908050190'].Sequence[3]['Title'] = { ['Finish'] = '201001' }
MC['250908050190'].Sequence[3]['Description'] = { ['Challenge'] = '13950' }
MC['250908050190'].Sequence[3]['Marker'] = { ['Target_Normal'] = '{B2426DCE-2C6F-4577-85BD-8FED8E07A66E}' }
MC['250908050190'].Sequence[3]['Distance'] = { ['   2m'] = 2 }
MC['250908050190'].Sequence[3]['GPS'] = { ['No'] = 0 }
MC['250908050190'].Sequence[3]['Cinematic'] = { ['No'] = 0 }
MC['250908050190'].Sequence[3]['Timeout'] = { [' none'] = 0 }
MC['250908050190'].Sequence[3]['Position'] = { -1637.942139, 1795.205322, 64.780289, 0.000000, 0.000000, 346.813049 }

-- Rewards: 10000 money, no XP, no components. FinishRewards ends the mission.
MC['250908050190'].Sequence[4] = {}
MC['250908050190'].Sequence[4]['Name'] = 'FinishRewards'
MC['250908050190'].Sequence[4]['Position'] = { -1700.343018, 1767.954834, 61.929817, 0.000000, 0.000000, 233.486237 }
MC['250908050190'].Sequence[4]['Money'] = { ['10000$'] = 'Items.2532779827' }
MC['250908050190'].Sequence[4]['XP'] = { ['none'] = '' }
MC['250908050190'].Sequence[4]['Components'] = { ['none'] = '' }
```

Walking through it:

- The header gives the mission its id, author, localized title (`ACT_V`, string
  `175633`), localized description (`Challenge`, string `13950`), and completion
  state.
- `Sequence[1]` is the start step. Its `Marker` uses the `Gang_Hideout` variant and
  its `Requires_Completed` chains from mission `250908040725`, so the mission only
  appears once that one is done.
- The `ExtraEnvironment` block under the start sets the presentation: time of day
  09:00, clear weather, frozen timescale, the `Download_06` voiceover hash, a
  cinematic, no blackout, and no civilians, traffic, boats, or trains.
- `Sequence[2]` is the hack. The target is a hostile, already revealed Fixers
  Enforcer carrying an AR, with takedowns allowed and stealth optional. The hack
  takes 300 seconds, pauses if you move beyond 25 m, and has a 60 second limit. The
  omitted `ExtraKill` entries add up to nineteen more Fixers Enforcers around the
  area.
- `Sequence[3]` is the reach. The marker is `Target_Normal`, the radius is 2 m, and
  there is no GPS route or cinematic, so the objective completes on arrival.
- `Sequence[4]` is `FinishRewards`, which ends the mission and grants
  `Items.2532779827` (10000 money) with no XP or components.

To place a mission, write its position floats near a known location and keep the
marker and vehicle references copied from an existing mission, since the sources do
not document where marker GUIDs and vehicle names are defined. Character and
vehicle identity fields are the same names used elsewhere in the engine; see
[Entity XML Structure](entity-xml-structure.md) and the
[weapon adding guide](../weapon-adding-guide.md) for related references.
