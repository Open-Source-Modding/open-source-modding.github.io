# Mediatracker Authoring (TrackMania United Forever)

> **Source**: Mania-Creative (mania-creative.com), a private TrackMania site live 2010-2013, recovered from the Wayback Machine captures dated August 2011. The recovered pages credit the individual tutorial authors: TStarGermany for the Mediatracker overview, camera basics, custom camera, path camera, race cameras, FX blur and FX colour; Nioko for camera effect shake, common effects (GPS) and text; Skeleton for 2D triangles. No single site author is recorded. Translations were credited to MindXperience, iNDEX, Adriweb, Eleven and Tamonte.

---

## What a MediaTracker clip is

The Mediatracker is the effects layer of a Trackmania track. It works like a video editor for a film with one actor, the driver's car. It is reached through the camera symbol in the track editor. With it you can play intro films before driving starts, overlay GPS guides and text, and drive colour, blur and camera effects.

A track's Mediatracker content is split into clips. Each clip:

- Is triggered by one or more trigger boxes (the green boxes placed in the 3D view).
- Contains one or more tracks (camera, text, blur, colour and so on) laid out on a timeline.
- Holds keyframes inside each track block, where the actual values are edited.
- Stops the previous clip from playing the moment it starts. This is the rule that makes a loopcam and every "back to normal" effect work.

Anything you change in a clip only becomes visible in-game after you record (or re-record) the Mediatracker ghost, because the editor needs ghost driving data to relate effects to the driving action.

## The three clip slots

| Slot | Also called | When it plays |
|------|-------------|---------------|
| Intro | - | Once, before the race starts, when the track is loaded in solo or on a server |
| In Game | - | From the moment driving action starts |
| End race | Replay, Outro | After the driver crosses the finish line, while the run is replayed |

Notes that change how you author:

- If a driver saves a replay (for the TMX Top10, or to shoot a video), the **End race** content is used, not the **In game** content.
- The End race slot works like In game but has a rule of its own: **always insert camera tracks**, otherwise many effect tracks will not show in the outro.
- On a server, the spectate **Follow mode** shows In game content, and **Replay mode** shows End race content, while the driver is still driving.
- To use custom music, create a locator for the music file and implement it in the track.

For any track's Mediatracker work, the driver's ghost must exist. Update the ghost every time the driving line changes (new road parts, replaced parts), or the editor will show false timing.

## Camera tracks overview

A camera placed in the Mediatracker is **enforced**: the driver gets no view other than the one you selected until another clip takes over. Only one camera block can be active at a time, but camera blocks of different kinds can follow each other.

The three camera kinds are colour-coded on the timeline:

| Camera type | Timeline colour | What it controls | Best used for |
|-------------|-----------------|------------------|---------------|
| Race | White | A preset camera angle only, no extra values | Quick default views for loopings and walls |
| Custom | Red | Position, orientation, target and anchor in full | Complex, precise camera work |
| Path | Aqua | A smoothed keyframe path (Hermite by default) plus a Weight value | Smooth flights, GPS systems, video shooting |

Choosing a camera: place one when it genuinely helps (loops, wallrides), not when it forces an unfamiliar view onto a fast section.

Two timing rules for enforced cameras:

- A camera change in the wrong place tends to cause a crash; a camera change at the wrong time tends to cost time, because the eye needs a moment to adapt.
- Trackmania itself takes roughly 2 to 3 hundredths of a second to switch cameras, so a change that arrives too late is as harmful as one placed badly.

To find good trigger spots, drive the section at race speed with no enforced camera and switch cameras by hand; then place enforced cameras where the hand-switching felt natural. Keyboard drivers can rebind keys to switch views themselves, gamepad drivers cannot, so consider who is driving.

## Camera movement basics: the loopcam

A loopcam keeps an internal/ego camera on for as long as the car is inside a looping. To build one:

1. Open the loop-cam test track and drive it once (or record a Mediatracker ghost) so the Mediatracker content unlocks.
2. Go to the **In Game** section. The empty content initialises a new clip and asks for triggers.
3. Place a single trigger at the beginning of the looping, then deactivate the trigger box.
4. On the clip, add a **Camera** or **Camera Race** track. This inserts a 3 second block on the timeline; the 3 seconds are not important here.
5. On the first keyframe, set `Camera: Internal` and enable `Keep on playing`, so the block keeps playing even after its 3 seconds have elapsed (a driver can stay in the loop for any length of time).
6. Create a second clip with a single trigger at the end of the loop, and leave it empty.

How it works: reaching the loop entry starts clip 1, which keeps playing. Reaching the loop exit starts clip 2. Clip 2 carries no camera settings, so it does nothing except stop clip 1, and the view returns to normal. There is no "change back" instruction anywhere; the new clip simply stops the old one.

Do not switch back too early when a section after the loop is critical. Let the loop camera run until the car is fully clear of the danger, then switch back.

## Race cameras

Race cameras are the standard views normally reached with the Num keys. They need no special attributes or values; you only pick the angle.

| Race cam | Num key | Notes |
|----------|---------|-------|
| Behind cam | Num 1 | Standard driving camera. Known bug: causes stuttering when used with replay ghosts |
| Close cam | Num 2 | In TM Forever this is the Default camera |
| Internal / Ego cam | Num 3 | - |
| Orbital cam | Num 7 | Follows the driver. Outdated: removed in TM Forever, so old TM tracks fail to display it |

Speciality of race cams: if you change the race cam type at the first keyframe, the last keyframe automatically takes over that value. Path and Custom cams do not do this.

## Custom camera

The Custom camera gives full control of everything related to camera movement. The restored capture covers the Basics chapter (free, target, anchor, target plus anchor); the later chapters on camera position, orientation, driving cams, target position and interpolation were on pages that the archive did not recover.

To start, download and open the custom-cam test track (a short downhill ramp, a turbo and a long left curve), record a Mediatracker ghost, then enter the End race section.

1. Place a trigger on the start block.
2. Insert a **Custom camera** track at 0:00.
3. Select the block and set `Block End` to `0:06.00` (about the ghost's start-to-finish time) and enable `Keep on Playing`.

The four basic behaviours are set with two values, `Target` and `Anchor`. Both refer to the exact geometrical centre point of the car, not the car as a shape.

| Target | Anchor | Behaviour | Good for | Drawback |
|--------|--------|-----------|----------|----------|
| None | None | Free camera: follows its keyframed path and nothing else | Flying around in an intro or GPS clip | The car may not be visible; a matter of luck |
| Ghost | None | Target camera: orientation always aims at the car | Keeping focus on a car | Roll is disabled, so side-to-side nodding (Ctrl plus right mouse) does not work |
| None | Ghost | Anchor camera: position is kept at a fixed offset from the car | Keeping the camera moving with the car | It does not look at anything in particular |
| Ghost | Ghost | Target plus Anchor: car stays centred and the camera keeps its distance | 1:1 driving action, almost like a driving cam | Neither value rotates with the car's heading |

A target or anchor does not turn the camera as the car changes direction, because it tracks a centre point that can rotate without being visually displayed. The recovered Basics chapter promises a solution in its chapter 4, which was not captured.

Workflow tips for the exercises:

- To make target placement easier, temporarily set `Target` to `None`, move the camera, then set `Target` back to the ghost.
- To hold a fixed relative position, copy the start keyframe values to the end keyframe. This guarantees the same offset from the anchor across the whole play.
- Free camera: place the start camera behind the start, copy values to the end keyframe, then move the end camera forward near the turbo and aim it toward the finish.

## Path camera

The Path camera is like a simpler Custom camera, with two differences that matter.

1. **AnchorRot handling.** With only `Anchor` and `AnchorRot` set and no target, the Path camera is broken (you may even need a Yaw of 90 degrees to make it look at the car). As soon as a `Target` is set, AnchorRot starts working, but with the usual Target drawback of disabled Roll. This was expected to be fixed in Trackmania Forever.
2. **Smoothing and Weight.** Interpolation cannot be changed on a Path cam (`Hermite` is set by default). Instead it offers a `Weight` value that feeds the Hermite curve calculation: the higher the weight, the more protruding the camera's curves.

The Path cam's real strength is smooth motion across a path: whenever you add a keyframe, Trackmania automatically computes a smooth movement through all keyframes, accounting for position and behaviour. This is the reverse of the Custom camera, which is less good at that.

- Use the Path cam for smooth flights, GPS systems and any content for video shooting. It handles easy tracking well; complex tracking is better with the Custom cam.
- Use the **Smooth camera speed** button in the timeline button line to recalculate the smoothest possible camera speed between all keyframes in a block.
- For video shooting, prefer the Path cam. Trackmania runs between roughly 40 and 100 fps, while a captured video holds only 25 to 30 fps. A quick camera move can fall between two snapshots and never be recorded, which shows as a jerk. A Path cam minimises this.

## Camera effect shake

The shake effect simulates an earthquake, a crash, or any rumble. It works in the intro, in game and in the outro. A shake track starts as a 3 second block.

Two values control it at each keyframe:

| Value | Meaning |
|-------|---------|
| Intensity | How much the camera shakes at that keyframe |
| Speed | How fast the shaking is |

Set both to 0 for no shake, both to 10 for maximum fast shake. A hard jump from calm to 10/10 in zero seconds is unrealistic; real objects have inertia, so build the shake up and tail it off.

Realistic shake recipe: open the cam-shake test track, record a ghost, enter the In Game section and insert a **Camera Effect Shake** track, triggered where the car hits the floor.

| Keyframe | Time | Values | Purpose |
|----------|------|--------|---------|
| Block start | 0:00.05 | - | Delay the effect until the nose actually touches down |
| Start keyframe | 0:00.05 | Intensity rises, Speed rises | Contact; inertia begins to be beaten |
| 2nd keyframe | 0:00.10 | Oscillations and Speed at their peak | The bounce and the climax of the shake |
| 3rd keyframe | 0:00.70 | Intensity reduced, Speed held at 5 | Speed starts to diminish from here, tailing to 0 |
| End keyframe | 0:01.15 | Both values 0 | Everything has come to a standstill |

Set `Block start`, the keyframe times and `Block End` to match the table. Two cautions from the source:

- The shake has a small unexplained lag in the **In Game** Mediatracker; adjust the block's timeline position if you see the effect arrive late.
- A shake does not show in the outro without a race camera track applied as well, the same phenomenon as many other effect tracks.

## Common effects: GPS and ghost replays

The most striking Mediatracker effects are combinations of single effects. A GPS is the standard example: a camera guide that shows the player how to drive a track.

A GPS is built from two clips:

1. First clip, a **text** effect telling the player how to start the GPS.
2. Second clip, a **camera** flight from the start to the end of the track.

To build the simple GPS from the recovered pages: open the GPS test track (one left turn, one right turn) and drive it once so content unlocks.

- **Clip 1 on the start ramp.** A text track displaying `BACKWARDS FOR GPS` for 3 seconds. The trigger sits on the start ramp. This plays after the 3-2-1 countdown and vanishes after 3 seconds, long enough to read and short enough not to disturb someone who just wants to drive.
- **Clip 2 just behind the start ramp.** It contains a text track saying `THIS IS MY GPS` and a camera flight through the track. It starts when the driver acts on the message and drives backwards.
- The camera flight is a **Path camera**, the best choice for a GPS: easier to handle than a Custom cam and it produces smoother motion. Set keyframe values with the mouse almost every time; typing them by hand is rarely needed.

The source also covers Part 4 (a GPS with a replay ghost, including how to create and import a replay ghost) and Part 5 (the start offset value), but those pages were not recovered.

## FX blur

Trackmania has two blur effects that add a "real life" quality to camera work. Both are more suited to preparing replays for video shooting than to in-game use.

Required settings: `Postprocess FXs` enabled in the config launcher, and shader model PC3 or higher. Without them, blur effects do not show while playing Mediatracker content. Onboard graphics chips often do not support these options properly.

An **FX Blur** track offers two choices:

| Effect | What it does | Key values |
|--------|--------------|------------|
| Motion Blur | Washes out all moving objects, like a real camera filming fast action, making the screen look more like a movie | - |
| Blur Depth | Controls how the lens and focus operate; best combined with other effects such as camera zooms | `Force focus` locks the sharp area at the `FocusZ` distance from the camera |

A large lens size renders far objects well but fails on small or nearby objects.

## FX colour

FX colour manipulates the entire colour layout of the screen. Required settings: `Postprocess FXs` enabled, shader model PC2 or higher. Trackmania automatically creates transitions between two keyframes when their values differ.

| Setting | Meaning | Slider behaviour |
|---------|---------|------------------|
| Intensity | How strongly the chosen colour effect is visible | Left none, right full |
| Hue | Base colour tone; moves all colours along the 360 degree colour wheel | Shown as a bar on the right |
| Saturation | Purity of colour; manipulated by mixing in grey | Left 100% grey, right 0% grey; default 50% |
| Brightness | How light or dark the colour is | Left 50% black, right 50% white; default in the middle |
| Contrast | How far two colours sit apart, from identical grey to black plus white | Left minimum, right maximum |
| Red, Green, Blue | Subtracts individual colour components | Default 100% each; left subtracts |
| Inverse | Photo-negative effect; all RGB values are subtracted from 255 | Left 0% inverted, right 100% inverted; middle gives flat grey (128,128,128) |

Examples: subtracting Blue gives a yellowish or golden atmosphere; subtracting Red and Green gives a dark blue "scary night" atmosphere; sliding all of R/G/B left gives a greyscale image (the same as sliding Saturation left). FX colour can produce shadowy areas, blinding sunlight and an old black-and-white film look.

**Near and Far.** By default, colour values affect everything on screen except other Mediatracker effects such as text and images. FX colour can apply different values to foreground and background, called `Near` and `Far`.

- A different Near/Far split requires PC3 shader or higher AND an FX Blur (Depth or Motion) effect inserted at the same time.
- `Distance` sets how far the Near area reaches from the camera.
- `BlendZ` sets how visible the Far settings are (left none, right fully visible).
- Intended uses include fog in the distance or a spotlight on the car.
- Drawback: the Far area wrongly excludes the sky and some textured background objects, while the Near area wrongly includes them, so realistic effects are hard. Use Near/Far only when you fully control what the camera sees, mostly intros and GPS systems.

## 2D triangles

Triangles draw simple vector graphics inside the Mediatracker, with no image loading.

- Creating a **2D Triangle** track enters editor mode (the `+` sign): click to set the three points that outline the triangle.
- In selector mode (the arrow sign), pick one point and edit its properties. To edit several points, click and hold the left mouse button and drag over the area.
- Each point has colour and opacity, and Trackmania interpolates automatically between points of a triangle.
- Join triangles into polygons by connecting a new triangle's end points to an existing triangle.
- Move points by selecting a shape and dragging the horizontal or vertical lines of the yellow cross, or type x/y values in the advanced (`.`) properties.
- Triangles use keyframes like any other track, so edit or copy all frames as desired.

Letterbox widescreen recipe (preferred over a stretched `I` for sharp edges, and over an image because images may not load):

1. Place two rectangles, each made from two triangles. When the pointer turns from green to yellow over the first triangle's corner, the new triangle will join it precisely.
2. Click the move button, then in the preview drag from top-left to bottom-right to select all corners of both rectangles.
3. Drag the colour changer until both rectangles are black.
4. Set the coordinates, top to bottom, left to right:

```
Point 1  x=1   y=1
Point 2  x=-1  y=1
Point 3  x=1   y=0.7
Point 4  x=-1  y=0.7
Point 5  x=1   y=-0.7
Point 6  x=-1  y=-0.7
Point 7  x=1   y=-1
Point 8  x=-1  y=-1
```

5. The 0.7 values set the bar thickness; adjust to taste.
6. Enable **Keep playing** so the widescreen lasts, then copy the start keyframe values to the end keyframe.
7. Export the clip and import it next time to reuse the effect.

Triangles can also draw simple pictures and animations, and unlike external graphics they are available immediately with no client loading. Save frequently: 2D triangles are known to be a little buggy.

## Text overlays

A **Text** track handles every text effect. A text track's block starts at 3 seconds by default. Below `Block End` the editable values are:

| Value | Meaning |
|-------|---------|
| TrackText | The text itself; colour codes and control codes are allowed, including multi-line, but manialinks are not. For long or complex text, prepare it in a text editor and paste it in |
| Colour | Main colour of the text; only text in Nadeo's default colour can be recoloured, text already affected by colour codes cannot. The colour is the same at all keyframes and cannot change per keyframe. With the `$s` shadow code, the shadow colour follows the picked colour |
| PosX, PosY | Position on screen; `0.00` is the middle, `1.00` is the screen edge; negatives and decimals allowed |
| Depth | Stacking order when two text lines cross; `0.00` up to `1.00` down |
| Rot | Rotation in degrees, for example `90` rotates 90 degrees left; negatives and decimals allowed |
| ScaleX, ScaleY | Text size; negative values mirror. Trackmania only resizes cleanly up to about 10x, after which letters turn edgy |
| Opacity | Transparency slider |

Important: change or copy the necessary values at every keyframe of a text track.

A text track is not only a text tool. Stretched `|` characters make background bars, screen borders, colour transitions and technical-looking lines. Two `|` scaled and positioned give a bar; an `o` can act as a screen border; two `|` give a colour transition.

Common problem and fixes:

- A line edge is blurred when a letter is stretched heavily. Adding a vertical-only stretched `|` at the border usually fixes it.
- When building a background bar plus text, the effect appears and disappears abruptly. Add keyframes near the ends (for example at second 1 and second 4) and move the opacity slider to the left at the start and end keyframes.
- If the text is invisible or flickers on top of the bar, both tracks share the same `Depth`. Lower the text's `Depth` (for example to `0.4`) at both the start and end keyframes so it sits above the bar.

Worked example (bar and text): record a ghost, enter the In Game section, trigger on the start ramp, add two text tracks of 5 seconds each. One track holds a `|` stretched horizontally and slightly vertically for the bar; the other holds the text, moved from left to right or right to left. Then apply the opacity and depth fixes above.

## Workflow checklist

1. Record or update the Mediatracker ghost after any change to how the track is driven.
2. Decide which slot the effect belongs in: Intro, In Game, or End race (replay and outro).
3. Create a clip and place its trigger box in the 3D view, then deactivate the trigger box.
4. Add the effect track (camera, shake, blur, colour, text, triangle) to the clip's timeline.
5. Set the block start and block end on the timeline; remember the default block length is 3 seconds.
6. Edit values at every keyframe; copy start values to end values when the value should not change.
7. Use `Keep on playing` when the effect must outlast a fixed time, such as a loopcam or a letterbox during an intro.
8. For outro work, always add a camera track so the effects are displayed.
9. Verify in game by replaying the track; then check the End race behaviour by finishing and watching the outro.
10. Before publishing for video, prefer the Path camera and check the shader and postprocessing requirements for blur and colour.
