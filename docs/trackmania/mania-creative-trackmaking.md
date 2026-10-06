# Mania-Creative: Trackmaking, Style, Editor and Block Mixing

> **Source**: Mania-Creative (mania-creative.com), a private TrackMania site live 2010-2013, recovered from Wayback Machine captures of tutorials.mania-creative.com. This page combines five tutorials: "Good trackmaking style" (capture 2011-08-06, page last updated 28 April 2011), "Track editor + workflow" by TStarGermany (capture 2011-08-07, updated 28 April 2011), "Blockmixing / hex editing" by NL_pwf and TStarGermany (capture 2011-03-05, updated 14 January 2011), "Challenge music" by TStarGermany (capture 2011-08-06, updated 28 April 2011) and "Marketing your tracks" by TStarGermany (capture 2011-03-19, updated 14 January 2011). The text is a community tutorial, not official Nadeo documentation. No TMF-era revision of these particular pages exists in the archive, so the captured wording stands; the track editor capture covers its first page only.

---

## What makes a good track: the authors' view

"Good trackmaking style" is not a step-by-step guide. It is a set of interviews with established authors, each introduced by the style and the tracks they are known for. Their answers form the craft rules below.

### Full speed and multiplayer tracks

Ganjarider (fast walls: *Dutch delight*, *Black Velvet*, *Sticky Tarmac*) builds full speed with plenty of margin, and builds for multiplayer first. The track must give the driver enough time to see where to go next. For an online track the target is a track a driven player can finish at least three times in five minutes. Building runs from start to finish, never backwards from the middle. In full speed the track should get longer and wider as it progresses. The finish is very important: never end on a dull straight road, but on a loop, a corkscrew or a jump. Reversed boosters should be avoided. Bottlenecks (a part that is tricky for most people on the first few runs) should be removed; the way to find them is to host the track online and spectate new drivers.

### All-round, story and technical tracks

Irondragons (all-round: *Ahead is a blur*, *Open your eyes*) tries to bring something new to the table every time, and scraps an idea if it needs awkward bends. Tamonte (story tracks: *Elvenpath*, *Venice challenge*) aims to add something never seen before, explores the game blocks to put them to new uses, and builds story tracks with heavy use of custom files; the atmosphere comes from music, track, mod, scenery, sounds and images together.

Swensen (3Don) (short technical: *Mini Series*) favours technical tracks and wants instant action, so a long PF (platform) start is bad. A technical track must be checked for shortcuts. Back boosters should be used with care, because people avoid them, and the author should not ruin a good track with lots of Mediatracker flashing where the eye goes while driving.

Smok3y (fast all-round: *F1 GranD Pr!x*, *MoOnL!gHt*) builds for easy, fun and challenging online driving, and likes precision jumps. A good track has a logical route, flow and smooth transitions. Its speed is neither too fast nor too slow, it is fun online, well decorated, has smooth tech and speed, good Mediatracker work and a good author description with a screenshot. Jumps must be well calculated and smooth, with soft landings. Reverse boosters should not be used; a brake is better. The track must suit both the fastest and the reasonably slowest driver, so the author asks how fast and how far a pro can jump, and how slow and how little a newbie can jump.

LoCoBuDDha (tournament technical: *Party Tech*, *Enlightment*, *Irish Tech*) builds tech and drift with new ideas, having started in fullspeed before the *Irish Tech* series. Patience is the keyword. The method is to create a basic path first, play and edit it many times, and decorate last. The style must be decided before building starts.

DerFeineHerr (DFH) (crazy technical: *Crossing my garden*, *Pandemonium*) wants every part to be superb, for racing and the eye, mixing tech, offroad, action, innovation and design, with no boring elements such as long straights or too-wide curves. The result is tight and calculated, dense with reuse, surface changes, drops and jumps. The track is edited until complete, with no emergency solution or compromise; if a part is not right, DFH often deletes the track and starts again. A good track is innovative with no rigid scheme, every part built with passion, well calculated and smooth, challenging and well designed, dense and tight with reuse or air crossing. For a tech track in TrackMania Nations Forever the whole track should be respawnable.

### The craft rules in summary

Do:

- Test drive repeatedly.
- Delete every part that is not really good, however long it took, and rebuild it.
- Spend more than a day on a track; the best ideas often come overnight.
- Get the maximum out of any idea.
- Build a tight atmosphere around the racing line.

Do not:

- Long straights.
- Too-wide curves.
- Airbreak, meaning poorly calculated jumps.
- Emergency solutions.
- Long PFs.

## The track editor and its workflow

The editor is opened from the main menu's **Editors** button. A new track should be started through **New Track** then **Advanced**; the simple editor is too limited. The environment is chosen next (Stadium is used for the exercises), then the daytime. To change the daytime, mood or environment mod of an existing track, hold **CTRL** before clicking the track's name in the file listing to open a popup menu.

### Moving around the editing area

Track blocks are inserted at the cursor's actual position, and the cursor box follows the mouse or the arrow keys. Moving the mouse to the screen edges pans the view, and the scroll wheel or page-up/page-down moves the cursor up and down. Freelook mode is entered by holding **left ALT**:

- ALT + left mouse button drags the view away from the centre.
- ALT + right mouse button rotates, raises and lowers the camera.
- ALT + mouse wheel zooms.

### Selecting, placing and connecting

The editing area is usually between 32 x 32 x 32 and 45 x 45 x 45 blocks (width, depth, height). Blocks are selected through the on-screen buttons or the number keys, which leaves the left hand free. **CTRL + left click** on an existing block selects that block at the same height, and a rotating preview of the selected block appears in the lower right.

A block is placed with the left mouse button or the **Space** key, using the example road part 2/1/1. The right mouse button rotates the selected block before it is placed. An **Underground** mode places a limited group of blocks (tunnels) below the ground surface; it is only available in TrackMania United environments. The excavator icon is deletion mode, which deletes clicked parts without moving the cursor, and the arrow icons are undo and redo.

To connect two parts, select a block, click the location, keep holding the left mouse button and drag to the other part. Regular road parts have coloured end pieces that show what they connect to, and many non-road combinations are possible. If two parts cannot connect, the editor will not let the click and drag through.

Ground blocks are found in subcategory 1. Some ground blocks can be piled up: create a large ground area of one type, go up one height step and place the same type again.

Not every block can be used at every location. Some are only usable in the air, some need a particular ground type and some replace an existing block; the form of the block tells the basis it needs. Two mud blocks, 7/4/4 and 7/4/5, connect to each other but require a mud hill to be piled up first. The huge advertisement sign block 8/8/7 can only replace a classic block (the sidewall of an inflatable).

### Start, finish and checkpoints

A Multilap Start/Finish requires at least one additional checkpoint. An additional red finish line behaves like the green multilap start-finish. Offline laps are configured in the **access objectives** menu; online Rounds behave the same as offline. In online Time Attack, each pass of the green start/finish (or the red finish) counts as a finished lap, the time restarts from 0:00 and as many laps as wanted may be driven.

One caution concerns mixing a multilap start with a separate real red finish. A driver can stop before the final red finish, return to the multilap start and drive into a new lap with a "flying start", gaining a better time. Drivers must pass every checkpoint placed between the start and the finish, and the checkpoints give feedback on the time per section.

## Block mixing and hex editing

Block mixing, also called hex editing, combines two or more parts that normally cannot be combined, and places blocks where they normally cannot be placed. The results are track works and scenery with looks or functions that the internal track editor does not provide. Basic mixing is easy to learn; sophisticated mixing needs a lot of experience and abstraction.

### The GBX track file

Driving tracks are stored in Nadeo's own `.GBX` format. Much of the file is encrypted or compressed, so a hex editor is needed to inspect or change it. A track's GBX file roughly contains:

- general track information (track name, author name, validation time, password),
- tile placement (all the road works),
- Mediatracker content,
- file dependencies (locations or URLs of signs, mods and music),
- a thumbnail screenshot in JPG format,
- and other data.

### Decimal and hexadecimal

The hexadecimal system counts 0-9 and A-F, so 16 ciphers before a register is added on the left, whereas decimal counts 0-9. Machine code is only 0 and 1, the two on/off electrical signals. A "0" or "1" is a bit; one hexadecimal cipher represents 2^4 bits; a full byte is 2^8 bits, which is two hexadecimal ciphers, giving 256 possible values. To edit bytes in the track file, the author must read and write these bytes. If a value would exceed 256, a second byte would be attached, but TrackMania does not use multi-byte values. The bytes on the left are mostly shown as ASCII characters on the right; not all bytes are displayable in ASCII, so empty gaps appear.

### How tiles are placed and stored

In the Stadium environment there are 32 x 32 x 32 cells for tiles. Every tile has a name and a coordinate (length, height and depth) from 0 to 31. For height, the grass ground itself is value 0, and everything above the ground starts at 1. Besides the tile name and XYZ position, the tile's rotation and variation are stored. Variations occur when the same part has different shapes, for example connected to the floor or to a neighbouring tile.

### Manual mixing (historical)

Nadeo compresses all tile information to keep the file small, which made single tiles hard to find by hand. The **ReCompressor** tool decompressed the file, saved a deflated version and listed every block with its hex address. A worked example moves a grass checkpoint from X:17/Y:1/Z:17 to X:16. The decompressed track is opened, the hex address 00001760 is found, and the StadiumGrassCheckPoint begins there; after that come multiple bytes for rotation, location and variation. The X byte is changed from hex "11" to "10" (17 to 16), the file is saved, and the checkpoint has moved exactly one cell onto the turbo part. ReCompressor had errors while processing files, so it should no longer be used for manual work; ChallengeEdit is used instead.

### Modern mixing with ChallengeEdit

The French TM-Hexa forum produced tools, among them **ChallengeEdit (Load)**, which decompresses a track, lists all tiles, performs the hex editing and saves the file again. ChallengeEdit can only edit tracks made with one of the local online profiles on the same harddisk, so an example track must first be loaded and saved once in the track editor, making the local player its author.

The worked example, **TMC-Blockmix-Example1**, places the start ramp, some small edgy road and the finish line, then selects the first available curve tile, 2/1/2, and places it two pieces above the ground at Y:3 so it does not interfere with the other road pieces or the finish top.

In ChallengeEdit:

1. Select **StadiumRoadMainGTCurve2**, the tile to lower.
2. Change its Z value from 3 to 1, lowering it by two cells, using the arrows or direct keyboard entry. ChallengeEdit uses XZY order rather than XYZ, which does not affect the result.
3. Press **Save block modification**.
4. Press **Save track**; the hex editing is done automatically.

### The healing process

After the mix, the editor shows an air hole in the ground, because a curve in the air was moved down to the ground. Click **Erase** and select the curve piece. Deleting it restores the floor, but everything looks scrambled. Press **UNDO** and TrackMania reinstalls the curve piece "with intelligence", connecting it properly to all surrounding tiles including the floor. One more road piece, 2/1/1, is then drawn to connect the curve to the other road pieces on the left.

Mixing moves a block out of the natural state it was placed in. Not only connections to neighbouring tiles are stored, but also the surface the tile was placed on. Pressing **Erase** then **UNDO** can bring TrackMania to a situation it cannot handle, in which case it shuts down; an earlier backup of the track is the fallback. Multiple backups should always be kept, because some mixing cannot be undone.

Sometimes a part should deliberately not be healed. In the archive's two-lane example, the left lane is normal and ends in a typical road edge piece. The right lane is mixed vertically down by one cell and not healed, so it has no road edge piece, and is then connected to the lane piece above. A car on the normal lane hits the road edge piece and starts to tip over at speed, while a car on the mixed, non-healed lane jumps down smoothly. A non-healed tile can be more productive for the driving.

### Tiles that should not be mixed

Mixing a turbo and a checkpoint makes TrackMania try to show both blocks at once. The two blocks have different texture surface graphics, so the surface flickers between the two depending on the camera position. Such mixes are ugly.

Two normal road checkpoints should not be combined. A start, a finish and a checkpoint should never be combined. Servers are protected against cheating, and driving through multiple checkpoints, starts or finishes at the same time is treated as cheating, which can get players banned from the server the track is played on.

### Advanced ChallengeEdit: primary blocks

Platforms built with **StadiumPlatformRoad** can be too thick. In the ChallengeEdit listing, a **-P** marks a primary block, the leading block of a group, and the blocks below it carry **-@**, meaning they depend on the primary block. Selecting the primary block and clicking the **Unlock** button at the bottom (after a confirmation popup) allows the primary block type to be changed from a dropdown list. Choosing **StadiumInflatableSupport** and pressing **Save block modifications** changes the primary block and all of its @ blocks to the new type. The result is a super thin platform, of the kind normally only found on top of the red inflatables; because the usual red bulges at the side are missing, road transition pieces can be used on the platform edges. Single @ blocks can also have their type changed, but only if a primary block of the desired type already exists in the listing. Block names usually identify the tile type, though the full overview takes practice. Some environments contain "hidden" blocks that are not in the internal editor but are available in this listing. Later ChallengeEdit versions were to add multi-block movement.

### Special objects and tasks

Inflatables have the most codes and blocks, so they are the hardest to modify. The codes for a platform represent every single part of it: the first code is the first height of the platform on the ground, and the further into the codes, the higher the platform. The codes of each platform are separated from those of every other inflatable. After a platform has been modified so it no longer has its original shape, pressing **UNDO** in the editor will probably crash TrackMania.

Grass floating in the air can be made two ways. Method 1: put a block on the ground and move it up; the attached grass moves up too. Poles are best, but any part with a small grass strip works, and the smaller the part the more racing space is left. Deleting floating grass does not bring it back with undo. Dirt can be moved up without putting anything on it. Method 2: build a platform in the air and use ChallengeEdit to change it to **StadiumGrass**; the disadvantage is holes in the ground where the grass would normally be.

Invisible roads are the most difficult mix and need patience:

1. Create a swimming pool on the ground at the horizontal coordinates where the invisible ground is wanted.
2. Move up the water blocks with ChallengeEdit, and only the water, not the pool edges.
3. Under the water there is a floor. When the water is moved up, the floor is not visible, which creates the illusion of an invisible road.
4. Nothing can be mixed or built through, on or under the invisible road. It is also slightly higher than the regular road, so a platform jump (the black one) of exactly the same height is needed to get onto it.
5. Moving the water up leaves a large hole in the ground. Moving something such as a platform onto it covers the hole, but its coordinates must be calculated very exactly; a miss means redoing the whole platform. The water can be moved up only once, so to add or remove invisible road the water must first be moved back down and the pool expanded.

## Custom challenge music

Adding custom music is one of the easiest tasks. The music is converted to `.OGG`, with the best compression being mono channel and optimised for low bandwidth. The OGG properties (like MP3 ID3 tags) are edited so the artist name, track title and comment are stored; these three tags are displayed in TrackMania. The OGG may optionally be converted to `.MUX`. The file is uploaded to a web host, which must allow hotlinking or direct linking: a link ending in `.ogg` is right, a link that shows a web page is wrong. A locator is created and placed in the game's Challengemusic folder next to the music file. To apply it, the camera symbol at the bottom left of the track editor opens the Mediatracker menu, where **SELECT CUSTOM MUSIC** is chosen and the song picked from the listing. The "Add your own music" button is only an OGG-to-MUX converter. The track is then saved and published.

## Marketing a track

The archive also holds a long marketing guide. Its trackmaking-relevant points are short. A track must meet the author's own highest standard before it is published: if it does not work as intended, it is refined or broken down and rebuilt, and only a track the author is 100% convinced of should go public. Unfinished tracks are beta tested with a chosen group, not with clan comrades or online buddies who say "it is good" too easily, and not with casual drivers, but with experienced authors and drivers. Too many different ideas in one track destroy its theme.

Publishing too many tracks in a short time is worse than being known for rare gems, and a release is best delayed when another author publishes an outstanding track, unless a tournament forces a simultaneous release, in which case the track must be outstanding and different. The first destination is TM-Exchange, where a track has seven days to enter the Week Top10 and which acts as a distribution point for servers; the second is online servers, including servers in other countries; the third is forums. The guide's server arithmetic: a track played 5 times a day on a server visited by 20 people, on 10 servers, reaches 100 plays a day per server, 700 a week per server and 7000 a week across the ten. The guide also warns that TM-Exchange success is 90% about reputation, support and who you know, and advises building a network of co-authors, drivers, admins and clan people while avoiding those who devalue the work.

## Credits

Recorded authors and translators in the captures:

- "Good trackmaking style": interviews with Ganjarider, Irondragons, Tamonte, Swensen (3Don), Smok3y, LoCoBuDDha and DerFeineHerr (DFH); translations by Torx and TStarGermany.
- "Track editor + workflow": TStarGermany.
- "Blockmixing / hex editing": NL_pwf and TStarGermany; translations by Natzor and -DexM-.
- "Challenge music": TStarGermany.
- "Marketing your tracks": TStarGermany.

Related pages: [Manialinks and page building](mania-creative-manialinks.md), [Mediatracker authoring](mania-creative-mediatracker.md), [File and asset reference](mania-creative-file-and-asset-reference.md), [TrackMania formats](trackmania-formats.md).
