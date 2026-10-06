# Mania-Creative: Avatars, Custom Cars and Skins

> **Source**: Mania-Creative (mania-creative.com), a private TrackMania site live 2010-2013, recovered from Wayback Machine captures of tutorials.mania-creative.com. This page draws on: 2011-07-22 and 2012-04-28 ("Custom Avatars" by The Doctor), 2011-08-05 ("Custom Cars" by -DexM-), 2011-07-23 ("Painting your own skin" by RdV, translated by -DexM-), 2011-03-15 and 2012-05-08 ("Car shadows / neons" by Whizkid), 2011-01-23 ("Custom stickers for TM skin painter" by Skeleton), and 2011-07-25 ("Graphic formats", page 1, by The Doctor, translated by Lord M'zn). The text is a community tutorial, not official Nadeo documentation. The TMF-era revisions generally agree with the earlier captures; where only one revision covers a point, that is stated.

This page collects the player-appearance side of the site: the in-game avatar converter and manual avatar making, the custom car zip archive and its 3D models, 2D skin painting from the car prelight texture, custom stickers for the in-game skin painter, and the car shadow ("neon") texture. It is a companion to [File Locations, Graphic, Sound and Locator Reference](mania-creative-file-and-asset-reference.md), which covers the folder layout and the graphic formats in more detail.

---

## Avatars

An avatar is the image that represents a player in game. The game manual, quoted in the tutorial, says a player may import a personal image, obtainable from anywhere on the internet. The player is responsible for the choice: images that are shocking, offensive or protected by copyright must not be used.

### Account authentication and transfer

For other players to see an avatar, the account must be authenticated. Authentication requires buying TrackMania United and using its product key; older keys are no longer accepted. The tutorial records that, per Xymph, a United and a Nations account can both be authenticated with a Steam player key written into `Nadeo.ini`.

Avatars travel peer to peer. The game sends a packet containing the avatar to the other players connected to the same server. Peer-to-peer communication is enabled in the launcher under Configure, Advanced, Peer ToPeer. Avatars are small enough that a locator file is unnecessary, except possibly on a dial-up connection. In game, the Profile, Advanced menu has a "Show Avatars" button that must be lit to see other players' avatars.

A router between the computer and the internet needs port forwarding, which creates a tunnel to the local IP address and the ports the game uses. Port forwarding is also required to run a server other players can join. The default ports are 2350 (Server) and 3450 (Peer to Peer), and both are editable in the TrackMania launcher configuration.

### The in-game avatar converter

The simplest route uses the game's own import utility:

1. Open the Profile menu and left click the avatar in use (or the flag) to open a list of the avatars that ship with the game.
2. Click "Add Your Own Picture"; the "Import image file" utility starts.
3. Pick a file and click OPEN. JPG, DDS and TGA are accepted.

The utility converts the image to a DDS file written to:

```text
C:\Documents and Settings\(Your Name)\My Documents\TrackMania\Skins\Avatars\
```

The result must stay below 50 KB. Images below 128 pixels in either dimension are rescaled to 128x128; images above 256 pixels are rescaled to 256x256. An image need not be square, because the utility squares it.

The converter has a known fault: the import utility treats pure black, `#000000`, as a transparency mask, so black areas vanish. Changing `#000000` to `#010101` before import avoids this, but the tutorial prefers manual conversion.

### Manual avatar conversion and required size

Manual conversion means doing the work the utility does by hand, then using a specialised program for the best quality-to-filesize ratio:

- TrackMania always displays square avatars, so first select a square area of the source image and crop to it. The tutorial's example source is 640x480.
- Resize the crop to 128x128 or 256x256; both sizes are accepted.
- Save as `.BMP` for the intermediate step, then convert with a program that produces DDS.

### Avatar gaps

Only page 1 of the tutorial was captured. Parts 4 to 7, on page 2, were never archived: "File types, Resolution and Size", "Advanced transparency", "Advanced animated avatars" and "Advanced emoticons". Their details are not recorded here.

## Custom cars

### What a custom car is

A "Custom Car" is a `.zip` archive that can contain a 3D geometry model, several 2D skin textures and some sounds. When TrackMania finds custom data for a car element inside the file, it uses that instead of the Nadeo default. The driving physics are fixed in the engine and are not influenced by any of these files. A locator can transfer a custom car to other players.

### Using custom cars online

Peer-to-peer upload and download must be enabled (launcher, Configure, Advanced, Peertopeer) whether a car is transferred directly or through a locator, and the firewall or router must allow it. In Trackmania Forever, showing a locally used custom car to other players globally needs an upgraded account, that is, TrackMania United.

### How a car is assembled, and why only changed files go in the zip

The game builds a car on the fly from many pieces of data spread across several folders of the installation. A custom car zip replaces that process: the zip is chosen as "your car" in the player's profile menu. A fully loaded archive would be huge, so TrackMania first uses any files present in the zip and then gathers every other file it needs from its own data folders. Only files that differ from the default originals should be inserted.

### 3D car models

A 3D car model is complex geometry made in a 3D program and then converted to Nadeo's GBX container. Two files represent the model, one per detail level of the same vehicle, and the game picks one according to the user's graphics settings:

- `Mainbody.Solid.Gbx`
- `MainbodyHigh.Solid.Gbx`

Placement decides which environments use the model. To use the Stadium model in the Snow environment, insert the two GBX files and their textures, and place the zip at:

```text
..\My documents\TrackMania\Skins\Vehicles\SnowCar\
```

Without it, the game adds the Snow car model while assembling the car. To apply one model across all environments, use:

```text
..\My documents\TrackMania\Skins\Vehicles\Carcommon\
```

If the model is not a standard car model, the two GBX files must always be inside the zip.

Building and importing a 3D model is complicated, and the site never wrote its own tutorial for it. The custom cars page instead linked external guides: Trick's tutorial (English, ugghost.com), the Trackmania Carpark tutorial (French), the Deepsilver Forum tutorial (German), Taolung's tutorial (German) and a TM-Community guide (English).

### Custom car gaps

Only page 1 of the custom cars tutorial was captured. The remainder of Section 4 (extra skin textures, skins for custom 3D models) and Sections 5 to 7 (sounds, where to put the zip, sound and texture resources) were on page 2 and were never archived. The index also linked "3D Models", "/Neons" and "Car Sounds" pages; none of those three pages was ever captured by the Wayback Machine.

## Painting a custom 2D skin

A 2D car skin can be painted either with the game's own internal painter, which the tutorial does not cover and defers to the game manual, or with an image-editing program. The program method uses the car's prelight texture as its source.

The prelight texture is the graphic the game uses to add shadow and light zones to the car body. It is relatively large, 1024 or 2048 pixels on a side, and has the same structure as a normal skin, which makes it an ideal template. A copy of it lives at:

```text
[Game installation folder]\GameData\Painter\Layers\[Environment]\Prelight\Layer.dds
```

Work on a copy of this file, never on the original. The site also converted the StadiumCar prelight into two files that allow per-object selection, one compatible with Photoshop (`.psd`) and one with Photopaint (`.cpt`). Reading and editing DDS files is required; see the graphic format notes under [File Locations, Graphic, Sound and Locator Reference](mania-creative-file-and-asset-reference.md).

The painting workflow:

1. Open the `.psd` or `.cpt` template. The right side lists the vehicle's parts under evocative names.
2. Select the part to edit and hide the others. Colour it, apply brushes and add text. Attention to detail gives the best result, though a simple pattern (the tutorial used a carbon pattern) often suffices. Repeat for each object.
3. Optionally create an alpha channel (the "transparency mask" in Photoshop). In TrackMania this channel does not encode transparency: it encodes an object's luminance, how much light it reflects. Copy an object into the alpha channel to make it shiny, or leave it out to keep it matte.
4. Reinstate the original pre-shading by loading the original full prelight texture as a layer above all the others and setting that layer's Blend mode to Multiply.
5. Flatten the image and save.

With Photoshop the file can be saved directly as `.DDS`, using DXT5 to keep the MIP maps. If the program cannot write DDS, save `.TGA` or `.PNG` and convert with the DDS Converter utility. The finished skin is then included in the custom car zip.

The TMF-era revision of this tutorial is a broken capture, an unrelated page, so the earlier French original is the only usable source. The French text was translated by -DexM-, and the tutorial credits RdV as its author.

## Custom stickers for the in-game skin painter

The in-game painter offers only a limited choice of what can be added to a car. The alternative is to make a sticker in Photoshop. The tutorial's recorded author is Skeleton, its last update is January 14 2011, and it was captured on 2011-01-23.

Best source images are `.PNG` files with transparency, which removes half the work. The tutorial's example was found by searching for a character image with a `filetype:PNG` search term.

The workflow in the tutorial's order:

1. Prepare the canvas to stop colour bleeding. Use Image, Canvas Size, switch the drop-downs from pixels to percent, set both values to 110 and confirm. This leaves a margin so the sticker does not bleed colour from the canvas edge.
2. Make the image square. Use Image, Canvas Size in pixels, and set the lower of the two values to match the higher. The tutorial's example had a height of 559, so both width and height became 559.
3. Add a background. Create a new layer, fill it with a colour that best shows the image (the tutorial used black), and move it below the image layer.
4. Add an alpha channel. Hold Ctrl and left click the image layer's thumbnail to select the image, then use Channels, Create New Channel. The new alpha channel is black; with Ctrl held, press Backspace to fill the selection white.
5. Save this as `Sticker.tga`. The TGA format is required because it stores the alpha (transparency) information.
6. Make the icon. Go back to the layers, right click a layer and flatten, then use Image, Image Size, set both height and width to 64, and save as `Icon.dds` using `DXT1 no alpha`.
7. Place both files in a per-sticker folder. The target root is:

```text
C:\Users\[user name]\Documents\TrackMania\Painter\Stickers
```

Create the folders if they do not exist, then make a new folder for the sticker (the tutorial used "Bart") and put `Sticker.tga` and `Icon.dds` inside it.

8. Test it in game. Go to Editors, Paint a Car, choose a car and click Paint!, then click Apply stickers on the car icon and locate the sticker. The custom icon makes it easy to find. Apply it to the car.

## Car shadows and neons

The "neon" technique turns the Stadium car's shadow texture into coloured underglow, and the same method makes shadows for the other environment cars. It works only for the default Nadeo cars; it may work for a 3D model only if that model's author implemented an extra shadow file. A car shadow can only be used inside a car zip. Photoshop is used in the tutorial, but Paintshop Pro or GIMP work equally.

1. Get the source. The original Stadium shadow is at:

```text
[Game Installation folder]\GameData\Vehicles\Media\Texture\Image\
```

The file is `StadiumCarShadowProj.dds`. Copy it to a temporary work folder.

2. Open the DDS copy in the graphics program. Three notes apply: the texture is very large and has space on all sides around the car for graphics and text; the shadow is compressed along the car's Y-axis and is automatically stretched in game; and white is interpreted as 100 percent transparent, so the nearer the shadow colour is to white, the more transparent it appears in game.

3. Manipulate the colours to change the black shadow into neon colours. In Photoshop the quick route is the Gradient Tool, found under the Fill Bucket. Set the tool's Mode to "Colour", otherwise it fills the whole screen, and choose or create gradients, for example a red-orange gradient.

4. Insert text if wanted. Add a new layer with Layer, New Layer, then use the Text tool (the T button) and set colour and font. Text appears upside-down and mirrored in game, so correct it with Edit, Transform, Rotate 180, Flip Horizontal. For text with a transparent inside and a coloured border, make the text itself white and use Layer, Layer Styles, Stroke, then edit the border colour and size.

5. Finalise. If the program writes DDS, save directly; otherwise save `.bmp` and use the DDS Converter utility. Save the shadow file with `DXT1a` compression and leave all other values at default. Rename the saved file to `ProjShad.dds` and put it into the custom car zip. Re-choose the custom car zip in the in-game Profile section, and the shadow displays in game.

## File formats these workflows depend on

TrackMania uses four graphic formats: JPEG, TGA, DDS and BIK. Free tools include PaintDotNet (which supports DDS directly), GIMP (which needs a DDS plugin) and XnView (which converts between formats and has good DDS support). JPEG is lossy and has no transparency or alpha channel; in the game it is used only for images in MediaTracker content or on advertisement signs. The avatar converter and the shadow workflow produce DXT1a DDS files, while a painted skin is saved as DXT5 to keep its MIP maps. These formats, the DDS map types (diffuse, normal and specular) and the DDS save methods are covered in [File Locations, Graphic, Sound and Locator Reference](mania-creative-file-and-asset-reference.md).

## On-site avatar-making tools

Besides the downloaded game and its in-game converter, the site hosted its own avatar-making tools under `specials.mania-creative.com`: an `avatarmaker` and an `avatarmakerpremium`, each with an index page (`index_avatarmaker.php`, `index_avatarmakerpremium.php`) and a `generate.php` endpoint. The URL index records captures between 2010-06-30 and 2011-03-16, but the tool pages themselves were never captured, so their exact options are not recorded here. The tutorial text does not describe them either.

## Credits and source gaps

The recovered pages credit the following tutorial authors:

- The Doctor: "Custom Avatars" and "Graphic formats".
- -DexM-: "Custom Cars" and the translation of the skin-painting tutorial.
- RdV: "Painting your own skin".
- Whizkid: "Car shadows / neons".
- Skeleton: "Custom stickers for TM skin painter".

Source gaps, stated rather than filled:

- The avatars tutorial page 2 (Parts 4 to 7) was never archived.
- The custom cars tutorial page 2 (the rest of Section 4, and Sections 5 to 7) was never archived.
- The index-linked "3D Models", "/Neons" and "Car Sounds" pages were never captured by the Wayback Machine.
- The TMF-era revision of the skin-painting tutorial is a broken capture, an unrelated page, so only the earlier French original is usable.
- The graphic format detail pages for DDS (pages 2 to 4), TGA and BIK were not archived; only page 1 survives.
- The on-site `avatarmaker` and `avatarmakerpremium` pages were not archived.

Related pages: [File and asset reference](mania-creative-file-and-asset-reference.md), [Manialinks and page building](mania-creative-manialinks.md), [Mediatracker authoring](mania-creative-mediatracker.md), [TrackMania formats](trackmania-formats.md).
