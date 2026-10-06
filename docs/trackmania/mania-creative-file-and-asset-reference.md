# TrackMania United Forever: File Locations, Graphic, Sound and Locator Reference

> **Source**: Mania-Creative (mania-creative.com), a private TrackMania site live 2010-2013, recovered from the Wayback Machine captures of 2010-07-02, 2011-06-17, 2011-07-25, 2011-08-07 and 2011-08-10. The recovered pages credit The Doctor and TStarGermany as tutorial authors; no single site author is recorded.

This page is a practical reference for finding, replacing and referencing the media that TrackMania United Forever (also branded TrackMania Forever) loads at runtime. It covers the two content roots, the per-asset-type folders, the graphic and sound formats the game accepts, and the `.loc` locator files that let other players download media you did not ship with a track. Paths reflect the post-Forever layout.

## Default and custom content

TrackMania Forever reads media from two separate trees:

 - Default content lives under the game installation, in `Gamedata\`. It ships with the game, and editing it in place risks an update overwriting your changes.
 - Custom content lives under the player's documents folder, `...\My documents\Trackmania\`, inside the Windows user profile. This tree is nearly empty on a fresh install, and most subfolders have to be created by hand before the game will use them.
 - The TrackMania launcher shows the exact storage location in use. That is the reliable way to confirm both roots on a given install.
 - The game can also load content from hidden folders, so if a file appears to be ignored, search the whole drive for `*.gbx` (this surfaces tracks, replays and other GBX containers).

## Default content (game installation)

Root: `[Game installation folder]\Gamedata\`

```text
Gamedata\
  Skins\Any\Advertisement\          advertisement signs
  Skins\Avatars\                    player avatars
  Skins\Vehicles\American\          vehicle skins, one folder per environment
  Skins\Vehicles\BayCar\
  Skins\Vehicles\CoastCar\
  Skins\Vehicles\Rallye\
  Skins\Vehicles\SnowCar\
  Skins\Vehicles\SportCar\
  Skins\Vehicles\StadiumCar\
  Vehicles\Media\Texture\Image\     car textures
  Vehicles\Media\Audio\Sound\Wavdata\   car sounds
  American\Media\Moods\             environment moods and textures
  American\Media\Texture\
  Bay\Media\Moods\
  Bay\Media\Texture\
  CoastCar\Media\Moods\
  CoastCar\Media\Texture\
  Rallye\Media\Moods\
  Rallye\Media\Texture\
  SnowCar\Media\Moods\
  SnowCar\Media\Texture\
  SportCar\Media\Moods\
  SportCar\Media\Texture\
  StadiumCar\Media\Moods\
  StadiumCar\Media\Texture\
  Interface\Media\                  in-game GUI
  Menu\Media\Texture\Image\         menu textures
  MenuForever\Media\Texture\Image\  Forever menu textures
```

The environment folders (`American`, `Bay`, `CoastCar`, `Rallye`, `SnowCar`, `SportCar`, `StadiumCar`) each hold their own `Media\Moods\` and `Media\Texture\` pair.

## Custom content (user documents)

Root: `...\My documents\Trackmania\`

```text
Trackmania\
  Skins\Any\Advertisement\          advertisement signs
  Skins\Avatars\                    player avatars
  Skins\Vehicles\CarCommon\         3D car models as .ZIP, all environments
  Skins\Vehicles\American\          3D car models as .ZIP, one folder per environment
  Skins\Vehicles\BayCar\
  Skins\Vehicles\CoastCar\
  Skins\Vehicles\Rallye\
  Skins\Vehicles\SnowCar\
  Skins\Vehicles\SportCar\
  Skins\Vehicles\StadiumCar\
  Skins\[Environment]\Mod\          environment mods
  Mediatracker\Images\              MediaTracker images
  Mediatracker\Sounds\              MediaTracker sounds
  Challengemusics\                  custom track music
  Tracks\Challenges\                tracks
  Tracks\Replays\                   replays and ghosts
  Tracks\                           MediaTracker clip import and export
```

Replacing car skins means dropping the right files into the per-environment `Skins\Vehicles\<Environment>\` folder. Custom car models are distributed as `.ZIP` archives and go into `CarCommon\` when they apply to every environment, or into the matching environment folder otherwise.

## Cache, screenshots and troubleshooting

 - Cache: `...\ALL USERS\Trackmania\Cache\`. If the game hangs while connecting to a server, empty this folder.
 - Screenshots (F10) and recorded videos are written to `...\My documents\Trackmania\`.
 - If the game hangs entering the track editor or the MediaTracker, clear the `MediaTrackerGhosts` folder, a subfolder of the custom-content replays folder.
 - Most custom content can also be accompanied or represented by a locator, covered below.

## Graphic formats

TrackMania works with four graphic formats: JPEG, TGA, DDS and BIK. The original tutorial recommended three free tools that handle them: PaintDotNet (has `.dds` support), GIMP (needs the `.dds` plugin) and XnView (converts between formats, good DDS support).

JPEG is the lossy option. It is used for images inside MediaTracker content and for advertisement signs. Its characteristics:

 - Scalable compression, so file size can be traded against quality.
 - High compression produces visible blocking and artefacts around edges.
 - No transparency or alpha channel.

The companion sections of the source tutorial documented the remaining formats, including DDS in detail (diffuse, normal and specular maps, mip maps, cube maps) and three ways to save DDS files (an Adobe plugin, DDS Converter, and a command-line client). Those detail pages were not archived, so only their topics survive here:

| Format | Documented in the source as | Notes |
| --- | --- | --- |
| JPEG | full section, page 1 | lossy, small files, artefacts at high compression, no transparency |
| TGA | a dedicated section | detail page not archived |
| DDS | a dedicated section plus a detailed sub-section | diffuse / normal / specular maps, mip maps, cube maps; saved via Adobe plugin, DDS Converter or command-line client |
| BIK | a dedicated section | detail page not archived |

For the binary container formats the game uses (GBX and the Pak archives that wrap them), see the other Trackmania pages rather than repeating that material here.

## Sound formats

TrackMania accepts three audio formats: WAV, OGG and MUX. Two free tools cover editing and conversion: Foobar2000 (or Winamp) for conversion, and Audacity 1.3.4 or newer for editing and cutting.

WAV is the old "waveform audio" container with many subformats. TrackMania uses the plain `Microsoft/Windows PCM 16bit` subformat: lossless, CD quality, large files. It is only worth using for very short samples, such as MediaTracker sound effects or a car horn.

OGG (Ogg Vorbis) is a modern, free, lossy codec that compresses better than MP3. It is the best sound format to use in TrackMania.

MUX is not really a sound format. TrackMania takes an OGG file, rewrites its header data and saves the result as a `.mux`. The resulting file only plays inside TrackMania. The community explanation recorded at the time was that this was done to avoid legal conflict over copyrighted music distributed as public OGG files, though TrackMania reads ordinary OGG for custom track music as well.

### Converting with Foobar2000

Right-click a file in Foobar2000 and choose convert.

To WAV:

```text
output format: wav
parameters:    Microsoft/Windows 16-bit PCM
sample rate:   44100 Hz
all other processing options: off
```

To OGG:

```text
output format: ogg
click the "... " options button next to the dropdown to set quality
quality:       even 45 kbps is acceptable
```

For custom track music, also edit the OGG properties (the equivalent of MP3 ID3 tags): right-click the file, choose Properties, and set Artist name, Track title and Comments. Those three values appear at the top of the game screen while the custom music plays.

To MUX: only the Trackmania launcher can convert OGG to MUX. Any OGG properties you set are carried into the MUX file.

## Locators

A locator is a plain text file whose single line is the internet URL of a media file. It is a placeholder placed where the original media file would normally sit. Locators carry the `.loc` extension, for example `picture.jpg.loc`, `music.ogg.loc`.

Locators exist because media on your hard drive is not available to other players. The locator information, however, is transferred to them (or embedded in a track), so their game downloads the media automatically.

Important change in TrackMania Forever: the original media file must be placed together with the locator file in the same folder. If you keep only the `.loc`, the media may not appear in dialogues and lists.

### Which assets need a locator

 - Custom cars
 - Custom track music
 - Custom track graphics and sounds
 - Custom advertisement signs
 - Environmental mods

### Creating a locator

First prepare Windows, because a mis-saved locator silently becomes `name.loc.txt`. In any folder window, open Tools, then Folder Options, then View, and uncheck "Hide file extensions for known file types".

1. Upload the picture (or other media) to a webspace and note its URL, for example `http://www.yourdomain.com/picture.jpg`.
2. Open Notepad.
3. Type the URL as the file's only text.
4. Choose File, then Save as.
5. Change the file type to "All Types" (otherwise Notepad appends `.txt`).
6. Enter the filename as `picture.jpg.loc` and save.

### Hosting example

A worked example used the free host `www.fileden.com`, which allowed all file types and direct linking (hotlinking):

1. Create an account.
2. Upload `Star.jpg`.
3. Get the direct URL (right-click the file, Properties, URL), for example `http://www.fileden.com/files/2007/12/23/1658968/Star.jpg`.
4. In the same location as the original file, for example `...\Trackmania\Mediatracker\Images\`, create a text file, paste the URL and save it.
5. Rename the text file to `Star.jpg.loc`.
6. Verify by moving or renaming the original file, loading the track and waiting to see whether the download succeeds.
7. Save the track afterwards so it is associated with the `.loc` file.

The important property of a host is that it serves the file directly at a stable URL, not behind a download page or session check.

## Relationship to the other Trackmania pages

 - [Trackmania binary formats](trackmania-formats.md) and [Trackmania XeNTaX knowledge](xentax-trackmania-knowledge.md) already document the GBX container layout (magic `GBX`, per-type double extensions), the Blowfish-encrypted NadeoPak r18 archives, texture and audio extraction, and the state of GBX tooling. This page does not restate any of that.
 - What is new here is operational: the two content roots, the exact default and custom folder layout per asset type, the Cache and MediaTrackerGhosts troubleshooting steps, the JPEG/TGA/DDS/BIK graphic overview, WAV/OGG/MUX sound handling with Foobar2000 settings, and the `.loc` locator structure, placement rule and creation procedure.
 - Where you need the container internals for the files described here (for example decoding a track's GBX or unpacking a Pak), use the binary-format pages; this page tells you where the files live and what to put in them.
