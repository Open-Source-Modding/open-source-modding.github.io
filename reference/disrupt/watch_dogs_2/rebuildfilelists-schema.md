# RebuildFileLists Schema (ToIndex Types)

> **Source**: `WD2/wd2_strings.txt` — XML schema definitions for the engine's file indexing system.

The Disrupt engine indexes files via `ToIndex` XML elements that define what
file types to scan, where to find them, and how to preprocess them. These
schemas drive the `RebuildFileLists` tool (Gibbed.Disrupt).

## ToIndex types

| Type | Version | Extensions | FolderPath | Notes |
|------|---------|------------|------------|-------|
| `CollectionShapeToIndex` | 3 | `.xml` | `editor\collectionsystem` | World data, not pre-indexed |
| `CollectionToIndex` | 2 | `.xml` | `editor\collectionsystem` | World data |
| `DatabaseToIndex` | 1 | `.xml` | `databases\building`, `databases\generic`, `databases\particlesemitters`, `databases\particlessystems`, `databases\prefabs` | Pre-indexed, ItemName=Generic/PartSys/Prefab |
| `DominoToIndex` | 2 | `.domino` | `domino` | Scripting domino nodes |
| `EntityToIndex` | 1 | `.xml` | `worlds;missions;entitylayers` | World data — entities |
| `GenericToIndex` | 1 | `.entityregistry.xml`, `.xml` | `engine`, `engine\gamemodes\gamemodesconfig` | EntityClass, GameMode |
| `GLMToIndex` | 1 | `.glm` | `graphics` | Graphics level metadata |
| `HkrToIndex` | 1 | `.hkr` | `graphics` | Havok resource, XmlInvokeDep=false |
| `NomadToIndex` | 2 | `.tree.xml` | `graphics` | NOMAD tree files |
| `RealtreeToIndex` | 2 | `.rta` | `graphics` | SpeedTree assets |
| `ResourceFileToIndex` | 1–2 | `.dae`, `.dpas`, `.gamex`, `.lua`, `.markup`, `.proxy.xml`, `.skel.xml`, `.animtrackcol.xml`, `.dds`, `.seq`, `.naseq`, `.anseq`, `.liseq`, `.fxseq`, `.stimuli.dsc` | Various | XmlInvokeDep varies per type |
| `SoundInfoToIndex` | 1 | `.xml` | `sound` | Sound definitions |
| `StateMachineBlobToIndex` | 1 | `.xml` | `engine\scripts\game\objects\pawn\stateMachine` | Pawn state machines |
| `TextureToIndex` | 1 | `.png` | `graphics;ui;editor` | SmallMax=16384, MediumMax=393216, BigMax=4194304 |
| `WorldLayerToIndex` | 1 | `terrain.xml` | `worlds` | World data — terrain layers |
| `XmlDepToIndex` | 1 | `.xml` | `graphics;editor\scenes` | Dependencies: glm;hkr;gamex;dae |

## Key flags

- **IsPreIndexed** — `true` for databases (already compiled), `false` for everything else
- **IsWorldData** — `true` for entities, collections, world layers; `false` for graphics/sound
- **TopDirectoryOnly** — `true` for sound, state machines, entity registries, game modes
- **XmlInvokeDep** — `true` for `.markup`, `.dae`, resource files that reference other assets

> **Note on `.xml` overlap**: Multiple ToIndex types handle `.xml` files — the
> distinction is FolderPath and flags, not extension. For example,
> `EntityToIndex` handles `.xml` in `worlds;missions;entitylayers` (IsWorldData=true),
> while `SoundInfoToIndex` handles `.xml` in `sound` (TopDirectoryOnly=true).
> `GenericToIndex` handles both `.entityregistry.xml` (specific pattern) and
> generic `.xml` in `engine` subdirectories.
>
> **Note on `ResourceFileToIndex` version**: The `1–2` range indicates this type
> has two schema versions. Version 2 adds support for additional resource file
> types or changes XmlInvokeDep behavior per extension.

## RebuildFileLists tool

```bash
dotnet run --project projects/Gibbed.WatchDogs2.RebuildFileLists -- \
  --install-path="/path/to/WATCH_DOGS2" \
  --output-dir="output/" \
  --data-prefix="data_win64"
```

Output: `data_win64/filelist.txt` — one path per line, backslash-separated.
