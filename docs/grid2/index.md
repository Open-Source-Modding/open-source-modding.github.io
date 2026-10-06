# GRID 2 Mod Support

> Source: Codemasters' *GRID 2 Data Modification* document (v1.0).

GRID 2 ships some files protected against modification. This document describes
why they exist, how the game's mod support is enabled, and what a mod may and may
not replace. It is Codemasters' own description of the system, reproduced here for
reference.

## Why protected files exist

Some of the game's files are protected against modification. This is intended as a
defense against cheating, mostly during online play but also when setting lap times
or earning achievements. When a protected file modification is detected the game
disables online access, achievements and also saving. A player who modifies a
protected file cannot save their progress.

Codemasters added basic support for modified data so that players who mod the game
with no intent of inconveniencing others can still have their progress saved. When
that support is enabled the game saves, but into a save separate from the one used
by the unmodified game.

## Enabling mod support

Support is enabled via a configuration file passed to the game when it launches.
The configuration file is a simple XML file that lists one or more directories,
each containing a mod:

```xml
<?xml version="1.0" encoding="utf-8"?>
<ModList>
  <Mod Path=".\My Mod 1" />
  <Mod Path="c:\Some Mods\My Mod 2" />
  <Mod Path="e:\Some More Mods\My Mod 3" />
</ModList>
```

To launch the game with mods:

- Right-click "GRID 2" in the Steam library
- Select "Properties"
- Click "Set Launch Options"
- Insert `-mod ` followed by the path to your configuration file, for example
  `-mod "c:\GRID 2 Mods\configuration.xml"`
- Click "Ok"
- Run the game

Remove this launch option to run the game normally.

### Search order and compatibility

When the game tries to load a file it searches the mod directories in the order
they are listed. If it finds the file it is looking for it loads it, otherwise it
uses the original file. If the same file is present in multiple mods, the game
loads the first one it finds, which may cause compatibility issues between mods.

## Setting up a mod

Start with an empty directory, then take the files from your mod and lay them out
in the same directory structure that the game uses. Add the path to that directory
to the configuration file as described above.

## Files that cannot be replaced

Modification of certain files is not supported by this system. These are:

- `raceload.jpk`
- `dr.nic`

...plus any files in the following directories:

- `system`
- `download`

The game ignores any instances of these files in a mod.

## DLC cars and tracks

The DLC packages are stored in the `download` directory. They cannot be modified.
However, files stored within the packages can be overridden if you own the package
in question. For example, to replace files for the Dallara IndyCar from the IndyCar
pack, add them to a `cars/models/ind` directory within your mod as if it were one
of the normal cars.

The codes the game uses to identify the currently available DLC cars are:

`240`, `370`, `722`, `b12`, `cam`, `civ`, `cr2`, `e32`, `evx`, `go2`, `gtr`,
`ind`, `ma2`, `mpg`, `r32`, `rx2`, `stk`, `wrx`, `z22`

## Local work directory

The local GRID 2 work directory mirrors the unpacked game tree, so a mod directory
can copy its structure from there. It contains, among others, the directories
`ai/`, `anims/`, `audio/`, `cars/`, `effects/`, `frontend/`, `language/`,
`racetypes/`, `replay/`, `shaderpack/`, `tracks/`, `video/`, plus the loose files
`surface_materials.xml`, `wet_materials_remap.xml`, `car_monitor_setup.xml` and
`example_benchmark.xml`.

## Troubleshooting: a mod still will not save

If a mod made with this method still will not save progress, check whether the
game's installation directory was previously modified. If one of the protected
files was modified, saving stays disabled. Restore GRID 2 to its default state and
verify the integrity of the game cache before trying again.

## Disclaimer

The "GRID 2 Data Modification" functionality was developed with the core community
in mind, to aid players who have a keen interest in modifying the game. Codemasters
cannot encourage any modification of GRID 2 as it carries inherent risks to data
integrity and code stability; any modification is carried out at the player's own
risk, and nothing in this document supersedes the GRID 2 Software License Agreement.
