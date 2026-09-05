# WLU Format (World Location Unit)

> **Status**: Early RE by FrankMK04 (Open Dogs project, April 2026). Format details below are from community analysis, not full spec.

WLU files handle all world coordinates in the Disrupt engine — object placement,
lights, shadows, action checkers, and spatial triggers. They were originally XML
files that got compiled into binary for retail builds.

## Purpose

Every physical object in the game world has a WLU entry defining its position,
rotation, and associated metadata. This includes:

- Buildings, props, vegetation
- Light sources and shadow casters
- Trigger volumes (action checkers)
- Spawn points and navigation nodes
- Environmental effects

## Known facts

- Binary compilation of what were once XML files (like most Disrupt formats)
- Editable and modifiable to a certain extent (FrankMK04, April 2026)
- No hard memory limit on world size — `patch3.fat/dat` is the known modification route
- Terrain generation data exists but interpretation is unknown

## Implications

WLU parsing enables a **World Map Editor** for WD2 — the next step in the Open
Dogs project (FrankMK04). This would allow:

- Moving/replacing objects in the world
- Adding new objects (buildings, props)
- Potentially extending the map (San Jose, Liberty City test)

## Open questions

- Exact binary layout (header, entry format, coordinate system)
- How terrain atlas data relates to WLU
- Whether new entries can be added or only existing ones modified
- Navigation mesh / traffic path integration
- Camera limit removal for out-of-bounds areas

## Source

Discord: FrankMK04, WD2 modding server, April 22-23 2026.
