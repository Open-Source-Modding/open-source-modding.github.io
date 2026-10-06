# The Witcher 3: REDkit and the Source Lineage

> **Status: stub.** This page records what is publicly known about The Witcher 3's
> current toolchain and archive format. The measurement work is not done yet, so treat
> the open questions at the bottom as a to-do list.

The Witcher 3 runs on **REDengine 3** (RED3); Cyberpunk 2077 runs on **REDengine 4**
(RED4). Both games use the same `CR2W` resource container, so format knowledge
transfers between them even though a console generation separates them. The RED4 side
lives in [Cyberpunk 2077 Formats](../cyberpunk2077/cyberpunk2077-formats.md).

---

## The leaked engine source

In February 2021 the HelloKitty ransomware group stole the source trees for The Witcher
3, Cyberpunk 2077 and Gwent, and tried to auction them. CD Projekt RED refused to pay.
The archives circulated encrypted, and working passwords surfaced publicly in April
2024, so the trees became readable years after the breach.

Two Witcher 3 trees exist: the original engine and the **next-gen** one that the 2022
"next-gen update" shipped. Retail Witcher 3 today is that build, so the next-gen tree is
the one that matters. You can write up modding-useful facts from it in your own words:
formats, structure, behaviour, tooling. The code itself and any leaked download stay off
this site.

---

## `.bundle` archives

Witcher 3 packs its assets into `.bundle` archives. The format is small and fully
public:

| Field | Value |
|---|---|
| Magic | `POTATO70` (8 bytes) |
| Header size | 32 bytes |
| Entry size | 320 bytes |
| Entry name | 256 bytes, NUL-padded, Windows backslash paths |
| Entry hash | 16 bytes |

The header carries the bundle size, a dummy word, and the metadata-table size. Each
320-byte entry holds the path, a 16-byte hash, packed and unpacked sizes, the data
offset, a timestamp, and a compression-type word. Compression types are `0 = none`,
`1 = zlib`, `2 = Snappy`, `3 = Doboz`, `4 = LZ4` (4 and 5 are both LZ4).

Public tools read and write this format: the Rust crate `w3bundle`, the Java
`bundle-explorer` and the C# `XBundle` library. `w3bundle` ships a from-scratch Doboz
decoder, and WolvenKit reads bundles too.

Cyberpunk 2077 replaced `.bundle` with the `.archive` container (`RDAR` magic, version
12, Oodle-compressed). CDPR rewrote the container between RED3 and RED4.

---

## Resources: the shared `CR2W` container

Individual assets are `CR2W` resources in both games: `.xbm` textures, `.w2mesh` and
`.w2ent` files, quest and gameplay definitions. (`CR2W` is the on-disk byte order of the
literal `W2RC`, "Witcher 2 Resource Class" read backwards.)

A Witcher 3 `CR2W` file has the same shape as the Cyberpunk one: a packed header, a
fixed table of ten chunk descriptors (strings, names, imports, properties, exports,
buffers, inplace data), then the tables themselves. Asset extensions keep the Witcher 2
`w2` prefix (`.w2mesh`, `.w2ent`, `.w2mi`), inherited from REDengine 2.
[Witcher 3 Formats](witcher3-formats.md) covers the per-type fields (textures, mesh bone
data, localization, saves).

---

## The official toolchain

For most of the game's life modders used the **2015 Modkit** (`wcc_lite`) and community
tools. That changed in May 2024 with **The Witcher 3 REDkit**.

- **Yigsoft** built it with CDPR. It is free for anyone who owns the base game on PC,
  distributed through GOG, Steam and Epic, with Steam Workshop integration.
- CDPR calls it a "repurposed, reworked and extended codebase of REDengine 3": an editor
  build of the engine.
- At GDC, CDPR said the work went into making the internal engine usable by outsiders:
  ~10 months of rework for projects, dependency handling and documentation, plus
  cleaning dev in-jokes and debug paths out of the tree.
- It exposes a **virtual depot** merged from three directories: `workspace` (your edits),
  `uncook` (game files you unlock) and `r4data` (extra assets shipped with REDkit). You
  check files out of the read-only depot into the workspace.
- It ships a **Blender plugin** for meshes, rigging and lipsync, plus original UI and
  audio sources.

The game build that pairs with REDkit reports itself as `4.04a_REDkit`.

---

## How a mod is laid out

The layout outlives the toolchain, so it is worth recording even though REDkit changes
who does the work:

- Mods live in a `mods` folder in the game directory. A mod may need extra steps: adds
  keybinds to `input.settings` under `Documents\The Witcher 3\`, or drops a menu
  definition into `bin\config\r4game\user_config_matrix\pc\`.
- Conflicting mods are merged with the community **Script Merger** tool, which the
  [Witcher wiki](https://witcher-games.fandom.com/wiki/Witcher_3_Modding) treats as
  standard practice because the official support never handled load order.
- Gameplay and item data sit in XML: prices, loot tables, skill costs, weapon and armour
  values.
- Scripts use **Witcher Script** and control movement, combat, stamina and signs. Menus
  are XML with their text in `.w3strings` files.
- The UI runs on **Scaleform GFx** (Flash) and ships as `.redswf` files, with embedded
  ActionScript driving much of the behaviour.

---

## Load order and conflict resolution

Witcher 3 sorts loads from the `mods.settings` file in `Documents\The Witcher 3\`, and
**the first mod to load wins a conflict** (the topmost entry overrides the ones below it).
That is backwards from Skyrim, where the last plugin wins, and it trips people up.

- A mod that is not listed in `mods.settings` does not load at all. Add a mod you
  installed by hand as a `[modName]` entry; the manager fills in the `Enabled`,
  `Priority` and `VK` parameters on the next deploy.
- Vortex exposes a Load Order tab where drag order maps to priority, and locks the
  output of the Script Merger to the first slot. NMM, MO2 and Witcher 3 Mod Manager
  handle the same file.
- [Script Merger](https://www.nexusmods.com/witcher3/mods/484) resolves `.ws` script and
  `.xml` conflicts. Merge `.csv` files never; merge in small batches rather than all at
  the end, so a bad merge is easy to trace.
- [Community Patch - Base](https://www.nexusmods.com/witcher3/mods/3652) is a
  prerequisite for many gameplay mods, and the Steam GOTY build needs it to match the
  GOG script set.
- The engine handles a large mod count badly on its own, with long loads, freezes and
  crashes that scale with mod count rather than hitting a fixed limit. The community
  fix is [Mod Limit Adjuster](https://www.nexusmods.com/witcher3/mods/3711) plus
  [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases):
  drop the 64-bit `dinput8.dll` into `bin\x64`, and rename it to `d3d11.dll` if the game
  crashes at launch on your machine.
- Mods written for classic 1.32 and for 4.x are not interchangeable. Install the build
  the mod was made for, which is why so many mod pages carry a separate "Old Gen" file.
- Authoring mods means [WolvenKit](https://www.nexusmods.com/witcher3/mods/3161) plus
  CDPR's official modkit; WolvenKit reads and writes the game files and packs mods.

---

## Next-gen vs classic

The **next-gen update (4.0)** landed in December 2022 and changed more than graphics:
CDPR reworked scripts and content enough that mod makers keep running diffs between
classic **1.32** and the 4.x line. Re-check any Witcher 3 modding fact older than that
against 4.x before you trust it.

---

## Open questions (what completing this stub needs)

The method used on the Cyberpunk side applies here, and none of it needs the leaked
download. A retail install is enough:

1. Parse a real `4.04a_REDkit` `.bundle`: confirm `POTATO70`, the 32-byte header and
   320-byte entries still hold, and record which compression types the shipped bundles
   use.
2. Decode a `CR2W` resource out of a bundle and record its **version word**, then compare
   that against the version the next-gen source tree considers current. This is the
   Witcher counterpart to the Cyberpunk finding that the 2021 source's resource version
   still matches retail.
3. Map the source tree's archive and resource modules onto what the retail bundles
   contain, and note where they have drifted.
4. Cross-check REDkit's own output (a cooked mod bundle) against the same parser.

Until then, everything above is public format documentation or CDPR's own statements;
none of it is a verified source-to-retail mapping.

---

## Key Facts
- Engine: REDengine 3 (RED3); shares the `CR2W` resource container with REDengine 4.
- Archives: `.bundle`, magic `POTATO70`, 32-byte header, 320-byte entries, zlib/Snappy/Doboz/LZ4.
- Resources: `CR2W` (reversed `W2RC`), ten-chunk table, `w2*` extension legacy from REDengine 2.
- Toolchain: 2015 Modkit (`wcc_lite`) → 2024 REDkit by Yigsoft, an editor build of the engine with a virtual depot (`workspace`/`uncook`/`r4data`) and a Blender plugin.
- Next-gen patch 4.0 (Dec 2022) reworked scripts and content; classic is 1.32, REDkit pairs with 4.04a.
- Layout: `mods/` folder, `input.settings` + `user_config_matrix/pc/` for keybinds and menus, XML game data, Witcher Script, `.w3strings` text, Scaleform `.redswf` UI.
- Load order: `mods.settings` in `Documents\The Witcher 3\`, first mod to load wins; Script Merger for `.ws`/`.xml` conflicts; Mod Limit Adjuster + Ultimate ASI Loader lift the engine's mod-count ceiling.
- Source lineage: the February 2021 breach published the trees; passwords surfaced April 2024. Retail is the next-gen tree.
- Sources: [Witcher 3 Modding (Witcher wiki)](https://witcher-games.fandom.com/wiki/Witcher_3_Modding), [Modding The Witcher 3 with Vortex (Nexus)](https://github.com/Nexus-Mods/Vortex/wiki/MODDINGWIKI-Users-GameGuides-Modding-The-Witcher-3-with-Vortex), [Sinitar's TW3 guide](https://www.sinitargaming.com/tw3.html).
