# The Witcher 3: REDkit and the Source Lineage

> **Status: stub.** This page records what is publicly known about The Witcher 3's
> current toolchain and archive format. The measurement work is not done yet, so treat
> the open questions at the bottom as a to-do list. The `.bundle` notes and the `CR2W`
> container were re-measured on the Remastered retail build; the load-order notes come from
> the leaked next-gen source tree and from public tooling.

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
"next-gen update" shipped. The 2026 Remastered build grew out of that next-gen line, so
the next-gen tree is its closest public ancestor. The trees still predate Remastered, so
expect drift. You can write up modding-useful facts from them in your own words: formats,
structure, behaviour, tooling. The code itself and any leaked download stay off this site.

---

## Build lines

The game has three live build lines, and Valve names the Steam branches after them.

| Line | Steam branch | Version | Last changed |
|---|---|---|---|
| Classic | `classic` | 1.32 | 2022-12-13 |
| Next-gen | `next-gen` | 4.04 | 2024-06-06 |
| Remastered | `public` | 2026-09-29 release | 2026-09-29 |

**The Witcher 3: Wild Hunt — Remastered** launched on **2026-09-29** as a free upgrade
for existing owners on GOG, Steam, Epic, PS5 and Xbox Series X|S. It also sells on
Battle.net and the Microsoft Store, and a separate Switch 2 release is free for Switch
owners. It replaces the Complete Edition and includes Hearts of Stone and Blood Wine.
CDPR built it in close collaboration with **Yigsoft**. The PC download runs about 45 GB.
An expansion, **Songs of the Past**, arrives in 2027.

The rendering changes matter less to a modder than the content ones: path tracing, DLSS
4.5 with Ray Reconstruction, FSR 4, XeSS 2.0, native DX12, an improved shading model, and
textures derived from the HD Reworked Project. Gameplay changed too. The skill tree was
reimagined and skill points reset, animations and camera were reworked, the UI was
refreshed, and a photo mode was added.

---

## What Remastered changed for mods

Remastered ships an in-game **Mods menu powered by mod.io**, on PC, PS5, Xbox Series X|S
and Switch 2. It needs a CD PROJEKT RED account linked to a mod.io account, and it is the
only route to console mods. The menu cannot carry mod menus, custom inputs, configuration
mods that edit `.ini` files, or mods that inject code through a `.dll`.

The update broke a large slice of the existing catalogue. Per CD PROJEKT RED's support
article, **mods containing XML files, script files or w3strings stop working** until
someone rebuilds them in REDkit. Script mods for console must be built in REDkit; texture
and bundle mods need no REDkit. The mod.io guide adds that **every text file must now be
UTF-8** where the game previously used UTF-16, which covers scripts, XML, CSV and
w3strings. `collision.cache`, `soundspc.cache` and SpeedTree `.srt` files also stop
working, and `w2rc` files such as `.w2ent`, `.w2l` and `.w2quest` should be re-exported
so they reflect Remastered's changes. Textures, models and bundles are not on the broken
list, and nobody promises they work either.

Nexus Mods keeps one Witcher 3 page and adds a **"Remastered Compatible"** tag, applied
by the author on upload and curated by three Community Champions. It runs a Remastered
Modathon from **2026-10-06 to 2026-11-30**. Its guidance for authors is to use XML
mounters and scope-based overrides instead of replacing vanilla files.

---

## `.bundle` archives

Witcher 3 packs its assets into `.bundle` archives. The format is small and fully
public. These numbers come from the 31 bundles in the Remastered retail build
(`content/content0/bundles/`, Steam build 25646871):

| Field | Value |
|---|---|
| Magic | `POTATO70` (8 bytes) |
| Preamble | 32 bytes |
| Entry size | 304 bytes |
| Header version | `u16` at `0x14`; `5` in all 31 bundles |
| Entry name | 256 bytes, NUL-padded, Windows backslash paths |
| Entry hash | 16 bytes |

The 32-byte preamble holds seven fields:

| Offset | Field |
|---|---|
| `0..7` | stamp `POTATO70` |
| `u32@8` | file size |
| `u32@12` | burst size; `1` on the two bundles above 4 GiB, `0` on the other 29 |
| `u32@16` | header size; file data begins 32 bytes past its end |
| `u16@20` | header version |
| `u32@22` | data offset, a repeat of `32 + headerSize` |
| `26..31` | padding, zero |

The file-size word is 32 bits, so a bundle past 4 GiB wraps: `buffers.bundle` is
7,640,452,656 bytes on disk and reports 3,345,485,360, and `movies.bundle` is 7,680,309,920
and reports 3,385,342,624. The data-start field always equals `32 + headerSize`, and the
entry table always divides by 304.

Each entry is a 256-byte name followed by twelve `u32`s: a 16-byte resource hash, the data
offset, one word that is `0` except inside the two bundles above 4 GiB, the packed size,
the unpacked (in-memory) size, a checksum, a compression type, then two zero words. Of
365,866 entries across the 31 bundles, 188,762 are compressed.

Compression has two shipping values. Type `0` means packed and unpacked sizes are equal,
type `1` means packed is smaller, and the payload is raw zlib with no extra wrapper.
Decompressing every sampled type-1 entry reproduced the unpacked size exactly, and the
output begins with the `CR2W` magic.

**Offsets are 32-bit and wrap in bundles past 4 GiB.** `buffers.bundle` is 7.6 GB and its
stored offsets stop at 4,128,599,408; of 251 sampled compressed entries, 140 decoded at the
stored offset and 111 only after adding 2^32. `movies.bundle` wraps the same way. A reader
must add 2^32 when `offset + size` passes 2^32.

Public notes give 320-byte entries. The measured stride is 304. The 320 figure is the
debug entry struct, which keeps a modified timestamp and a dependency field that the
shipping build drops.

The 31 bundles hold 365,866 entries. `levels` (117,802), `dlc` (94,896), `environment`
(52,407) and `characters` (30,633) dominate the paths, and the commonest extension is
`.buffer` (113,009), every one of which sits in `buffers.bundle`. The game's own content
tree uses the same top-level names, so the cooked layout has not moved.

Public tools read and write this format: the Rust crate `w3bundle`, the Java
`bundle-explorer` and the C# `XBundle` library. `w3bundle` ships a from-scratch Doboz
decoder, and WolvenKit reads bundles too. The compression enum those tools use
(`0 = none`, `1 = zlib`, `2 = Snappy`, `3 = Doboz`, `4 = LZ4`, with 4 and 5 both LZ4)
matches the two values retail ships.

Cyberpunk 2077 replaced `.bundle` with the `.archive` container (`RDAR` magic, version
12, Oodle-compressed). CDPR rewrote the container between RED3 and RED4.

---

## Resources: the shared `CR2W` container

Individual assets are `CR2W` resources in both games: `.xbm` textures, `.w2mesh` and
`.w2ent` files, quest and gameplay definitions. (`CR2W` is the on-disk byte order of the
literal `W2RC`, "Witcher 2 Resource Class" read backwards.)

A Witcher 3 `CR2W` file has the same shape as the Cyberpunk one: a packed header, a fixed
table of ten chunk descriptors, then the tables themselves. On the retail Remastered build
the header is 160 bytes: the magic, the version, flags, an 8-byte timestamp, a build
version, the object-data end, the buffer-data end (the file size), a header CRC, the number
of live chunks, then ten 12-byte descriptors of `(offset, count, crc)`. The named chunk
types run strings, names, imports, properties, exports, buffers, inplace data, and the
remaining descriptors are reserved.

The header carries a version word. Every resource unpacked from the retail Remastered
bundles reports **164**, while the leaked next-gen tree's current version macro is **163**,
so retail runs one version ahead of the tree. Cyberpunk 2077 landed differently: its retail
resources and its leaked tree both report 195.

The tables chain without padding, which is a cheap way to check a reader. An uncompressed
`.w2ent` sampled from a retail bundle has six live chunks: strings at 160 (456 bytes), names
at 616 (39 entries of 8 bytes), imports at 928 (1 entry of 8 bytes), properties at 936
(1 entry of 16 bytes) and exports at 952 (3 entries of 24 bytes). Each end lands exactly on
the next offset, and the first begins at the 160-byte header end. Asset extensions keep the Witcher 2
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

**REDkit 5.0.1042178** shipped on 2026-09-29, the same day as Remastered, and that is the
version to use against the current retail build. It has one hotfix, `v.5.0.1044630`,
which fixes a crash when the localised strings editor opens. Earlier releases paired with
the next-gen build that reported itself as `4.04a_REDkit`: `4.0.114968` (21.11.2024),
`4.0.104755` (03.07.2024), `4.0.103444` (06.06.2024), `4.0.102771` (21.05.2024),
`4.0.101539` (30.04.2024) and `4.0.101166` (18.04.2024). The 5.0 changelog adds:

- **Witcherscript scope-based overrides.** A mod can declare just a class and add or
  override a single function; the runtime merges it into the class at load. CDPR notes
  this "may make the Script Merger tool no longer necessary", since the merge happens in
  the engine instead of on whole script source files. It builds on **script annotations**,
  which the 4.0.103444 patch added in June 2024 for the same reason, so merge-on-load is
  not new to 5.0.
- **Script blobs.** Script mods compile to a small `precompiled.rsblob` that the engine
  loads at start-up. Console builds require them, because loose `.ws` files are not
  allowed there.
- **New DLC mounters**, for example `LoadingScreenDLCMounter`, for conflict-free mods
  that edit base-game features.
- **Fine-grained XML overrides.** A DLC can redefine an XML entry or tweak individual
  fields in one, with an `onConflict` attribute: `default` errors on a redefinition,
  `replace` swaps the earlier definition, and `extend` merges sub-nodes over it.
- **Gwent moves to name-based logic**, with game and opponent decks held in XML and
  reloadable.
- A new **`compile scripts`** commandlet compiles scripts from the command line.
- Script fixes: native functions can be wrapped, and events and states can be annotated.
- Scripts now ship with REDkit, and the depot should hold no vanilla scripts.
- The UI supports up to 30 user pins, raised from 10.
- REDkit **requires Wwise 2023 with the Mastering Suite and Motion plugins**.
- REDkit **no longer ships `.w3speech` and `.w3strings`**. Setup copies the language
  files from the game install.
- Terrain generates at a default tile count of 1024 (32×32).
- WitcherScript gains a `map` syntax.

### What the shipped build says about itself

The installed REDkit (Steam app `2684660`, buildid `25651183`, 104 GB) reports its build as
`5.0.1044630  P4CL: 13360103  Stream: //Red_engine/Main.Lava.Release`. The editor binary
also carries `v 5.00c`, the same version word the retail game reports. The tree in the 2021
leak sits at `Main.Lava`, so that stream name ties the two together: REDkit is a build of
the leaked tree.

Its packing tools sit in `bin/`:

- `bin/x64_RedKit/bundlebuilder.exe` (156.7 MB, internal name "Bundle Builder Tool.
  Version 0.92") writes the `.bundle` files, and it was built from
  `E:\Main.Lava\dev\src\win32\bundlebuilder\options.cpp`, the source the leak holds.
- `bin/tools/cooker/` holds `W3CookerTool.exe` and the step scripts (`analyze_game`,
  `cook_textures`, `cook_shaders`, `bundle_export`, `bundle_creation`, `deploy_common`).
  The legacy `wcc_lite.exe` ships as well.
- The cook script runs `wcc.exe exportbundles ... -out=...bundles.json`, then
  `bundlebuilder.exe -verbose -platform <platform> -depotpath <cook dir> -cookedpath
  <cook dir> -definition ...bundles.json -outputdir <output>`, then `wcc metadatastore`.

The packer will not run standalone. It reads its definition through an initialised depot
(`GDepot->GetBundles()`), and REDkit creates that depot only after you generate it, an empty
folder of about 60 GB extracted from the game install. Run it without one and it rejects
every definition, a minimal one included, as invalid JSON. A sampled cooked bundle has to
come from the editor or a full cook.

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
- Remastered moves conflict handling into the engine. Scope-based overrides merge script
  changes at run time, and XML mounters override vanilla entries through the `onConflict`
  policy, so an author can ship one mod that stacks with others instead of a whole file
  that replaces theirs. On the classic and next-gen lines `mods.settings` sets load order and
  Script Merger merges conflicting script changes; the Remastered build does not carry the
  `mods.settings` name at all. Its mod strings are `\mods\`, `modsList`, `modsMetadata`,
  `mods_enabled`, `AreModsEnabled`, `-disablemods` and the mod.io API routes, and
  `bin/config/r4game/user_config_matrix/pc/` ships ten menu XMLs with no `mods.xml`. The
  classic load-order file and the per-mod config menu have no counterpart in the shipped tree,
  and how a hand-installed mod is ordered on Remastered is not established. One missing string
  is not proof, but it is the only signal a retail install gives.
- Authoring mods means [WolvenKit](https://www.nexusmods.com/witcher3/mods/3161) plus
  CDPR's official modkit; WolvenKit reads and writes the game files and packs mods.

---

## Open questions (what completing this stub needs)

The method used on the Cyberpunk side applies here, and none of it needs the leaked
download. A retail install is enough:

1. **Done for the container.** The Remastered build self-identifies as `v 5.00c` (string
   in `bin/x64_dx12/witcher3.exe`, Steam build 25646871), and its 31 shipped bundles hold
   `POTATO70`, a 32-byte preamble, **304**-byte entries, header version 5, zlib payloads and
   a 32-bit offset that wraps past 4 GiB.
2. **Done.** The `CR2W` resources unpacked from those bundles report version **164**; the
   tree's current macro is **163**. The one-version gap is the W3 counterpart to the
   Cyberpunk result, where retail and the 2021 tree agree at 195.
3. **Done.** The source preamble matches the retail bundle one field for field, except
   that its declared header version 3 ships as 5, and the source `CR2W` header matches the
   retail resource one exactly (160-byte header, ten 12-byte chunk descriptors, chunk order
   and entry sizes). The remaining drift is the version numbers.
4. Cross-check REDkit 5.0's own output (a cooked mod bundle, and a scope-based override
   merged at run time) against the same parser. **Partly done.** The shipped build reports
   itself as `5.0.1044630  P4CL: 13360103  Stream: //Red_engine/Main.Lava.Release`, and its
   packer was built from the same `bundlebuilder/options.cpp` the leak holds, so the lineage
   is measured. The packer needs a generated depot before it will read a bundle definition,
   so a sampled cooked bundle still needs the editor or a full cook.

The load-order notes above are still public documentation or CDPR's own statements, not a
verified source-to-retail mapping.

---

## Key Facts
- Engine: REDengine 3 (RED3); shares the `CR2W` resource container with REDengine 4.
- Builds: classic 1.32 (Steam `classic`), next-gen 4.04 (Steam `next-gen`), Remastered launched 2026-09-29 (Steam `public`, self-identifies as `v 5.00c`).
- Archives: `.bundle`, magic `POTATO70`, 32-byte preamble, 304-byte entries, header version 5, zlib payloads, 32-bit offsets that wrap past 4 GiB.
- Resources: `CR2W` (reversed `W2RC`), 160-byte header, ten 12-byte chunk descriptors, version 164 in retail Remastered (tree macro 163), `w2*` extension legacy from REDengine 2.
- Toolchain: 2015 Modkit (`wcc_lite`) → 2024 REDkit by Yigsoft → REDkit 5.0.1042178 (2026-09-29), an editor build of the engine with a virtual depot (`workspace`/`uncook`/`r4data`) and a Blender plugin. The installed 5.0.1044630 reports `Stream: //Red_engine/Main.Lava.Release` and ships `bundlebuilder.exe` (writes the `.bundle`) plus the `cooker/` step scripts; its packer needs a generated depot to read a definition.
- Remastered broke mods carrying XML, scripts or w3strings until they are rebuilt in REDkit, and moved all text files to UTF-8.
- Remastered adds an in-game Mods menu on mod.io (PC, PS5, Xbox Series X|S, Switch 2); it cannot ship mod menus, custom inputs, `.ini` config mods or `.dll` mods.
- REDkit 5.0 adds scope-based script overrides, `precompiled.rsblob` script blobs, XML overrides with `onConflict` policies, and needs Wwise 2023 plus its Mastering Suite and Motion plugins.
- Layout: `mods/` folder, `input.settings` + `user_config_matrix/pc/` for keybinds and menus, XML game data, Witcher Script, `.w3strings` text, Scaleform `.redswf` UI.
- Load order: `mods.settings` in `Documents\The Witcher 3\`, first mod to load wins; Script Merger for `.ws`/`.xml` conflicts; Mod Limit Adjuster + Ultimate ASI Loader lift the engine's mod-count ceiling. Classic and next-gen only: the Remastered binary carries no `mods.settings` string and no `mods.xml` menu template.
- Source lineage: the February 2021 breach published the trees; passwords surfaced April 2024. The next-gen tree is the closest public ancestor of Remastered.
- Sources: [Cross-Platform Mod Support (CD PROJEKT RED support)](https://support.cdprojektred.com/en/witcher-3/pc/gameplay/issue/3001/cross-platform-mod-support-how-to), [What the remaster means for modding (mod.io)](https://mod.io/g/the-witcher-3/r/what-does-the-remaster-mean-for-modding), [REDkit changelog (CD PROJEKT RED)](https://cdprojektred.atlassian.net/wiki/spaces/W3REDkit/pages/12058625/Changelog), [REDkit 5.0.1042178 (Steam news)](https://steampulse.org/update/6731766), [Witcher 3 Modding (Witcher wiki)](https://witcher-games.fandom.com/wiki/Witcher_3_Modding), [Modding The Witcher 3 with Vortex (Nexus)](https://github.com/Nexus-Mods/Vortex/wiki/MODDINGWIKI-Users-GameGuides-Modding-The-Witcher-3-with-Vortex), [Sinitar's TW3 guide](https://www.sinitargaming.com/tw3.html).
