# Mania-Creative — Environment Mods and Game Internals

> **Source**: `tutorials.mania-creative.com`, the tutorial arm of the private TrackMania site Mania-Creative, archived at `web.archive.org`. This page is built from three community tutorial captures: "Modifying Environments" by Tamonte with an addon by Firefly (capture 2011-06-13, English page 1; its second page survives only as the TMF-era French capture below), the TMF-era revision `tm_modifying_environment` page 2 (capture 2012-01-18), and "Modifying internals" by TStarGermany (capture 2011-01-23). The material is a community tutorial, not official Nadeo documentation.

---

## Scope and relationship to the other Trackmania pages

Environment mods and internals edits sit one level below the track and car material already on this site.

 - For where files live, the two content roots (`Gamedata\` versus the documents folder), the per-asset folder layout, the JPEG/TGA/DDS/BIK graphic overview, the WAV/OGG/MUX sound handling and the `.loc` locator system, see [TrackMania United Forever: file locations and asset reference](mania-creative-file-and-asset-reference.md). None of that is repeated here.
 - For the GBX container, NadeoPak archives, model extraction and the state of binary tooling, see [Trackmania / Maniaplanet GBX, Pak and model formats](trackmania-formats.md).
 - What is new here is the mod as an archive: its exact internal layout, the diffuse/normal/specular texture set and the letter suffix convention, the mood files that control sky, sunlight, clouds, cloud shadows and reflections, the presentation files, and the in-place edits of the game installation used for internals work.

The environment mod path is deliberately not a GBX path. Where the asset-reference page and the GBX page describe packed or binary containers, an environment mod is an ordinary `.zip` of raw image files that the game overlays onto its default content at load time.

## What an environment mod is

An environment modification (a "Mod") is a `.zip` archive containing modified texture graphics that determine the look of the environment being played.

 - When a mod is loaded, Trackmania replaces its default textures wherever the `.zip` contains a file of the same name. Any texture the `.zip` does not contain stays at the game default. A mod therefore only needs to ship the files it actually changes.
 - The mod is assigned per track, not globally. In the track-open dialog for the editor, hold `Ctrl` and click the track. A window appears listing the installed mods, and one is selected from that list.

In Trackmania Forever many DDS textures changed, most of them compressed with DXT1 either with or without an alpha channel, so format settings from earlier TrackMania releases do not carry over cleanly.

## Structure of the mod archive

The mod is always integrated as a `.zip`. Trackmania reads the files it needs from the archive and brings them into the gameplay.

 - The root of the archive holds one file, `Icon.dds`.
 - The `Image\` folder holds textures for the trackworks parts: road, grass, roof parts and so on.
 - The `Moods\` folder holds textures for the environment as a whole, such as day and night. Inside it, a `Day\` folder holds the daytime environment textures: sky, clouds and similar.

**Case sensitivity**: the directory filenames and folder names are treated case sensitively. Upper and lower case letters should remain exactly as they are in the original installation. A renamed folder with the wrong case will fail to be picked up.

## Working with temporary files

Preparation is a copy-and-edit cycle meant to give a clean overview of what is being changed.

1. Create a root folder for the mod, for example `c:\temp\`.
2. Go to the chosen environment's `Media\` folder and find the `Moods\` and `Texture\` folders.
3. Copy the `Moods\` folder into `c:\temp\`.
4. Go into the `Texture\` folder and copy its `Images\` folder into `c:\temp\`. Do not copy the `Texture\` folder itself; that does not work later on.

At this point all default textures of the chosen environment are under `c:\temp\`, ready to be edited individually. After editing, the unmodified files are deleted so the mod ships only the changed ones.

## Which texture is compressed with which DXT setting

Always save a modified texture with the same format settings as the original. Two ways to determine the original settings:

 - Try saving with different settings. The result whose file size matches the original will have the right settings.
 - Inspect the original texture's properties in an image viewer or graphic program: DXT 1a, 1c, 3 or 5, mipmaps yes or no, alpha channel present or not, and reuse those settings.

Leave files that are not being modified untouched and in place so the game's original look stays intact.

## The diffuse, normal and specular set

For some block parts there are three different textures. The trailing letter of the filename identifies the map, and the graphic quality level determines which of them the game uses.

| Suffix | Map | Role | Quality level use |
| --- | --- | --- | --- |
| `D` | Diffuse | How the texture looks with no special effect applied | PC0 and PC1 use the diffuse map only |
| `S` | Specular | Where the texture shines and in what colour | PC2 uses diffuse and specular |
| `N` | Normal | Adds depth (relief) to the diffuse map | PC3 uses diffuse, specular and normal |

A worked example from the Stadium environment is the set `StadiumRoadTurboD.dds`, `StadiumRoadTurboS.dds` and `StadiumRoadTurboN.dds`.

### The three maps

The diffuse map (`...D.dds`) is the base colour; save it as DDS with DXT1.

The specular map (`...S.dds`) says where the texture should shine and in what colour. Its alpha (transparency) channel sets how much it shines: 0% transparency means shiny, 100% transparency means matte. The image looks like the diffuse texture but much brighter.

A normal map (`...N.dds`) applies a depth or relief effect to a diffuse map. The usual tooling builds the relief around a surface colour of RGB `128/128/255`, then removes every red element from the RGB range of the picture, leaving only green and blue. A Nadeo normal map differs from a generic one only by that removal of red.

### Creating a diffuse map

The worked source texture is `StadiumInflatable2D.dds` at 512x512 pixels, the flat grey floor on top of the inflatables. A replacement should be at least 512x512 to avoid losing quality. Like most Trackmania textures it is a tiled image, repeating left and right, above and below, so the borders on all sides must line up with the opposite side.

### Creating a normal map

The source texture is `StadiumInflatable2N.dds` at 512x512 pixels.

With Adobe Photoshop:

1. Apply the `NormalMapFilter` from NVidia's Photoshop plugins to the diffuse map picture.
2. The only value to edit is `Scale`, which sets the height of the relief.
3. Click OK to generate the normal map.
4. Remove all red elements: set the background colour to black, select the whole picture in the `Red` channel only with the marquee tool, and press `DEL`. Check the `RGB` channel to confirm.
5. Smoothing the picture heavily is optional, so the surface does not become too crumbly.

With Corel Photopaint or another paint program:

1. Apply the `3D Relief` effect to the diffuse map picture, with surface colour RGB `128/128/255`, choosing depth and intensity to taste.
2. Apply the colour channel mixer to remove all red elements.
3. Smooth heavily if desired.

Save the result as DDS, DXT5, no alpha channel.

### Creating a specular map

The source texture is `StadiumInflatable2S.dds` at 512x512 pixels.

1. Apply brightness +80% and contrast +50% to the diffuse texture picture; this shows the shine colour.
2. Select background colour black, then apply 30% transparency (alpha) to the picture.
3. If the file cannot be saved directly as DDS, export it as PNG with transparency and convert to DDS afterwards, for example with `DDS Converter 2.1` or `nvDXT`.
4. Save as DDS with DXT5 compression and transparency.

The three maps work together. The inflatable surface uses all three to deliver a three-dimensional impression; a quality mod follows the full path rather than stopping at the diffuse map.

## Mood files (sky, sunlight, clouds, reflections)

The files in the `Moods\` folder matter because they can change a mod's appearance radically with little work.

### Sky

`SkyColor.tga` (256x256 pixels) changes the sky colour. Limitations:

 - The file represents one horizontal band of the sky, not the whole dome. The colour at the top and bottom edges of the image continues upwards and downwards respectively.
 - Stars or other objects cannot be added, because the file is stretched horizontally without limit.

### Sunlight

`LightSun.tga` (512x32 pixels). The left quarter of the file is the shadow colour; the right three quarters set both the sun and its light colour.

### Clouds, part 1

 - `CloudsMinColor.tga` (512x64 pixels): the shade colour of the clouds in the sky.
 - `CloudsMaxColor.tga` (512x32 pixels): the lit colour.

A simple approach is to reduce both files to 32x32 and paint `CloudsMaxColor.tga` with the colour wanted for the lit part, and `CloudsMinColor.tga` with the colour for the shaded part. Trackmania accepts textures smaller than the original without complaint. More complex painted graphics are possible if wanted.

### Clouds, part 2 (cloud shadows)

`StadiumFXClouds.dds` (512x512 pixels) projects the shadows of the clouds across the whole stadium and its blocks. It can be repurposed for other effects, such as huge logos, rainbow colours or creeping ground fog. A small object becomes a huge projection, so fine detail is impossible to keep. Save as DDS with DXT1, no alpha.

### Reflections (cubemaps)

`EnvCubic.dds` and `EnvCubicSpecA.dds` (768x128 pixels) drive the reflections on the cars and the inflatable structures.

 - The file is six square images side by side: the six faces of a cube that forms the environment.
 - Reading the original from left to right, the faces show the Stadium in front, the Stadium behind, the sky, the grass, the left view and the right view. If any of these surroundings change, the cubemap has to be updated to match.
 - The full 768x128 layout is demanding. A simpler route is to work on a 256x256 single 2D texture instead. Most programs cannot import this kind of DDS texture correctly, though Photoshop can. The trade-off is that the result is not quite as detailed as the full layout.
 - Save the texture as a 2D cube in DDS with DXT5 (interpolated alpha).

## Presentation files

Two further files affect how the mod appears in the game.

### `LoadScreen.dds`

`LoadScreen.dds` (1024x1024 pixels) is the screen shown while a track loads. The file is 1024x1024, but only the central 1024x768 area is visible; the top and bottom 128 pixels are not displayed.

### `Icon.dds`

`Icon.dds` (128x128 pixels) is the icon shown in the mod selection window in the editor. It is stored in the root of the mod `.zip`, outside the `Image\` and `Moods\` folders. Create a 128x128 image and save it as `Icon.dds` with DXT1, no alpha.

## Finishing and installing the mod

1. Delete every unmodified file from the working `Image\` and `Moods\` folders. Only the changed textures should remain.
2. Zip the contents of the working root (the files and folders inside it, not the root folder itself). Use standard compression, and match the archive structure described above.
3. Place the `.zip` in `...\My Documents\Trackmania\Skins\[Environment]\Mod\`.
4. Restart Trackmania and the mod is available.

## Testing a mod without restarting the game (Firefly's workflow)

A mod cannot be reloaded in place: the game caches mod data until another mod is selected and then the new one loaded. Firefly's workaround avoids restarting the whole game for every test.

1. Zip the mod as `test1.zip` and copy it into the environment's mod folder; make a second copy in the same folder named `test2.zip`.
2. Start the game and load the track with `test1.zip`.
3. Edit the mod, zip it as `test2.zip` and copy it in. Press `Alt+Tab` back to the game, load the map and select `test2`.
4. Edit again, zip it as `test1.zip` and copy it in, then load the map and select `test1`.
5. Repeat the alternation between `test1` and `test2` for each build.

The two alternating filenames force the game to treat each build as a different mod, so the cache is bypassed.

## Modifying internals (the game installation)

Trackmania uses the same data formats for its internal user interface. The default files in the game installation folders can therefore be edited in place using the same graphic, video and sound formats as custom content. The difference from a mod is scope:

 - These changes exist only on the local hard disk. They are not transferred to other players.
 - Always keep backup copies of the local textures and sounds being manipulated.
 - Usefully editing these files assumes familiarity with editing textures, converting video and editing sound.

### Internals example 1: `LoadScreen.dds`

`LoadScreen.dds` sits at `[GameInstallationFolder]\GameData\Stadium\Media\Texture\Image\`. It is 1024x1024, DXT1. The black areas above and below the picture in a 4:3 resolution are normally hidden during play. In Trackmania United there are multiple different `Loadscreen.dds` files, one per environment the player can be in, each in that environment's own `...\Media\Texture\Image\` folder. Replacing the Stadium file means copying a new `Loadscreen.dds` over the existing one.

### Internals example 2: `BgPlanete.bik`

`BgPlanete.bik` sits at `[GameInstallationFolder]\GameData\MenuForever\Media\Texture\Image\`. It is 720x480, 30 frames per second, greyscale, with no alpha plane. The clip appears as a shiny sculpture inside the large 3D planet model when Trackmania starts; it is greyscale so it does not interfere with the screen and globe colours. Any animation or video can be converted to BIK and used to replace it, even at another resolution or length, because Trackmania automatically scales the video to the required dimensions.

### Internals example 3: engine sounds

The Stadium car's default engine sounds are OGG files at `[GameInstallationFolder]\GameData\Vehicles\Media\Audio\Sound\WavData\`, namely `SportCarEngineLow.ogg`, `SportCarEngineMid.ogg` and `SportCarEngineFast.ogg`.

The procedure is the same for all three:

1. Open the file in Audacity.
2. Choose `Effect` then `Amplify`.
3. Enter `-3` as the amplification value and press OK; this lowers the volume.
4. Choose `File` then `Export` and save the file back into the default directory, replacing the original.

Any filter or effect can be applied instead, or the samples can be replaced with entirely custom sounds. More generally, many other files in the installation folders can be edited the same way by browsing for graphics, textures and sounds worth changing.

## Format-level notes

These are the claims in the source strong enough to be worth recording as facts about the game's layout and formats:

 - An environment mod is a plain `.zip`, not a GBX container; the game overlays the archive's raw image files onto the default environment content at load time and falls back to defaults for files the archive omits.
 - Mod archive folders (`Image\`, `Moods\`, `Moods\Day\`) are case sensitive, and the root presentation file is `Icon.dds`.
 - Texture filename suffixes encode the map: `D` diffuse, `S` specular, `N` normal. The graphic quality levels gate their use: PC0/PC1 diffuse only, PC2 diffuse plus specular, PC3 diffuse plus specular plus normal.
 - Specular shininess is carried in the alpha channel, with 0% transparency meaning shiny and 100% meaning matte.
 - A Nadeo normal map is a standard relief map with the red channel removed, built around surface colour RGB `128/128/255`.
 - In Trackmania Forever many DDS textures moved to DXT1, with or without alpha.
 - Mood file dimensions: `SkyColor.tga` 256x256, `LightSun.tga` 512x32 (left quarter shadow, right three quarters sunlight), `CloudsMinColor.tga` 512x64, `CloudsMaxColor.tga` 512x32, `StadiumFXClouds.dds` 512x512 (DXT1, no alpha).
 - `EnvCubic.dds` and `EnvCubicSpecA.dds` are cubemaps laid out as six faces side by side in a 768x128 strip (front, back, sky, grass, left, right); they can be authored as a 256x256 2D texture and saved as DXT5.
 - `LoadScreen.dds` is 1024x1024 with only the central 1024x768 visible; Trackmania United keeps one per environment.
 - `BgPlanete.bik` is 720x480, 30 fps, greyscale, no alpha, and the game auto-scales BIK video to the dimensions it needs.
 - Stadium engine sounds are OGG files (`SportCarEngineLow/Mid/Fast.ogg`) under `GameData\Vehicles\Media\Audio\Sound\WavData\`.
 - Mod content is cached under a filename; swapping the contents of a mod without changing its name is not detected until another mod is selected.

## Credits

Authors recorded in the captures: "Modifying Environments" by Tamonte, with an addon and the testing workflow by Firefly, translations by NicoG60, MindXperience and tramantana. "Modifying internals" by TStarGermany, translated by Firebird. Recovered from `web.archive.org` captures of `tutorials.mania-creative.com` dated 2011-06-13 (`modifying_environment`), 2012-01-18 (`tm_modifying_environment`) and 2011-01-23 (`modifying_internals`). The mod was verified against the sibling pages [file locations and asset reference](mania-creative-file-and-asset-reference.md) and [GBX, Pak and model formats](trackmania-formats.md) in this `/docs/trackmania/` section.
