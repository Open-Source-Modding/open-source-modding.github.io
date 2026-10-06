# Mania-Creative — Recording and Producing Videos

> **Source**: tutorials.mania-creative.com (Mania-Creative, mania-creative.com), a private TrackMania site live 2010-2013, recovered from the Wayback Machine. The pages used are the "Video creation" capture of 2011-01-26 (the page itself dated "Last updated: January 14 2011") and the newer "Trackmania - Video creation" capture of 2011-12-06 (dated "Last updated: September 02 2011"). The recovered pages credit TStarGermany as author and -DexM- as translator. This is a community tutorial, not official Nadeo documentation.

Platforms such as TM-Tube hold hundreds of TrackMania videos, from basic gameplay captures to heavily produced films. This page records how the community tutorial of the time taught the recording and production of such a video, starting from a saved replay and ending with a compressed file ready to upload.

The tutorial assumes familiarity with the MediaTracker basics and its authoring workflow. The MediaTracker itself, its clips, tracks and keyframes, is covered on the sibling page [Mediatracker authoring](mania-creative-mediatracker.md).

The tool names, codecs, resolutions and bitrates below are exactly those the tutorial recorded in 2011. Video capture, encoders and upload services have all changed since, so read them as a historical recipe: the fact that a tool was recommended then is not a claim that it is a good choice now. No download links are repeated here.

## How a TrackMania video is made

The basis of every video shoot is a saved replay. The tutorial's general scheme is:

```text
saved replay(s)
  -> Replay MediaTracker (add effects, cameras, timeline work)
  -> save the combined replay ("big replay")
  -> in-game video shooting (produces uncompressed AVI)
  -> video editing (cuts, effects)
  -> compression / conversion (for TM-Tube or another host)
```

## Section 1: The sources

The basis of any TrackMania video shoot is a replay file. A replay contains the track data and the driving data, and it does not have to cover a complete start-to-finish run: short driving snippets can be saved and used as well.

A replay can be saved at any moment. During a race, pressing **S** saves the driving data recorded so far as a replay, regardless of where driving began or whether the race was finished. This is what makes a "super crash" video possible: a run that ends in a crash and never reaches the finish screen can still be saved, because the replay option is not limited to the end-of-race screen.

Replays for a project are placed in the game's replay folder:

```text
C:\...\My Documents\Trackmania\Tracks\Replays\Downloaded\
```

The workflow then runs through the game: **Editors / Edit a Replay**, open the replay folder, select a replay, press **Launch**, then **Edit** in the popup, which loads the replay into the Replay MediaTracker. MediaTracker clips are imported and exported through `...\Trackmania\Tracks\`; the folder layout is described in the [file and asset reference](mania-creative-file-and-asset-reference.md).

### The multiple-replay bug

Selecting several replays together and pressing Launch/Edit is normally meant to load every ghost into the Replay MediaTracker at once, which allows a video with hundreds or thousands of cars. At the time of the tutorial this no longer worked reliably. Instead, only one replay ghost appears in the timeline, and that one replay is described as defective: it causes TrackMania to ignore all the other selected replays.

The recorded workaround:

1. Attempt to load all the replays together.
2. Note which replay is responsible for the failure.
3. Return to the selection screen and load every replay except that one.
4. Use the MediaTracker's **Import Ghost** button to bring in the driving ghost from the defective replay afterwards.

Importing a driving ghost from a different replay works even when that ghost was recorded on an entirely different track. Played back in the combined replay, such a ghost car drives on what looks like an invisible track, a side effect the tutorial notes can be used deliberately for effects.

## Section 2: Adding effects to the replay

If no effects are wanted, the replay can be saved directly as **Videotrack_Bigreplay** (the save button is in the upper left corner) and the video shoot can proceed.

The replay MediaTracker exposes no clip listing, triggers or conditions, because all driving data is already recorded. Every effect track known from the normal MediaTracker can still be used. If the source track carried MediaTracker content in its "End game" section, that content appears in the track listing and can be kept or removed with the trashcan icon.

Because only one camera track can exist at a time, the default camera track is deleted first, then replaced with the project's own cameras. The tutorial's exercise builds three camera settings:

| Setting | Type | Block time | Target / anchor | Notes |
|---------|------|------------|-----------------|-------|
| 1st | Custom Camera | 0:00 to 00:05.00 | - | Camera placed in front of the start ramp; plays until the cars vanish behind the turbos. Copy the start keyframe values to the end keyframe |
| 2nd | Custom Camera | 05.00 to 07.00 | Target: Player1 | Two-second block after the first; remove the gap between the two camera blocks by dragging. Edit the XYZ values, set the target, then copy start values to the end keyframe |
| 3rd | Path Camera | 07.00 to 12.00 | Target + Anchor: Player2 | Added after the camera track and dragged left; keeps Player2 focused and circles around the car at the ramp. Add a keyframe at Time Value 9.00, then copy the middle keyframe values to the end keyframe |

Final polish: set Player1's ghost block End value to 12.00 so both ghost blocks share the same length, otherwise Player1 disappears too early. The whole replay is then previewed with the timeline's Play button.

A later subsection argues that camera work is easier in the replay MediaTracker than in the normal one, because the driving is already finished: there is no uncertainty about what a driver will do or where the car will go, so cameras can be placed exactly relative to the focused car. The tutorial recommends using that freedom for transitions such as moving from outside a car to inside it. The meaning of the **Start offset** value is deferred to the Common Effects GPS tutorial.

## Section 3: Timeline handling

The **Time** track exists only in the replay MediaTracker. It introduces a second timeline. The first is the familiar "superior timeline", where one second of timeline equals one second of playback and blocks and keyframes are arranged. The second timeline lives inside the replay that was loaded, and its relation to the superior timeline is controlled by the keyframe values of the time track.

The time track can be used to run time slower or faster, to rewind it, and to jump between points in the replay. The tutorial's three basic examples, each within a two-second block:

| Example | Start keyframe Time value | End keyframe Time value | Effect |
|---------|---------------------------|-------------------------|--------|
| Everything normal | 0 | 2 | Replay seconds 0 to 2 play back across the two-second block at normal speed |
| Fast forward | 0 | 4 | Replay seconds 0 to 4 play back in two seconds, so everything moves at 200% speed |
| Backwards in time | 2 | 0 | Replay seconds 2 to 0 play back, so everything runs backwards at 100% speed |

The **Tangent** value is comparable to the camera interpolation values: it influences how smooth or harsh a transition in time is.

### The "Matrix effect" exercise

The exercise adds a brief freeze to the replay. The time track is 0.6 seconds shorter than the two ghost tracks, and that 0.6 seconds is used for the effect so no complicated arithmetic is needed. In the time track:

1. Go to 09.00 and create an additional keyframe.
2. Go to 08.40 (0.6 seconds earlier) and create another keyframe.
3. Return to the keyframe at 09.00 and set its Time value to 8.4. Both keyframes now hold the same Time value, so the cars do not move at all during those 0.6 seconds while the superior-timeline camera keeps moving. This is the "Matrix effect".
4. At the end keyframe of the time track, set **Block End** to 12.00 and leave the Time value at 11.4, subtracting the 0.6 seconds of standstill from the superior timeline.

The result is saved as **Videotrack_Bigreplay** and previewed during playback.

When several time effects are wanted, the tutorial warns that the differences between the superior timeline and the replay's internal timeline require a lot of calculation. Multiple time blocks are easier to keep track of than many keyframes inside one block, and in some cases, such as switching from one time point to another, blocks are mandatory.

## Section 4: Shooting the video

With the combined replay saved, the in-game video shooting function is used. A dialogue sets the quality options. For the best possible result, all graphics options must be temporarily raised to their highest settings in the TrackMania launcher or config **before** the game is started; options such as PC3 shading and shadows must be enabled in the launcher beforehand.

The tutorial's stated recommendation for the shoot:

```text
shader:        PC3 or higher
resolution:    800*600
frame rate:    30 fps
anti-aliasing: 9x
motion blur:   off
```

This takes considerable shooting time, but produces the best material for further processing.

After confirming, a second dialogue selects the video codec that stores the video to disk. A lossless codec was recommended: full uncompressed RGB, or something like Microsoft-DV. Compressed codecs such as XVID were explicitly discouraged at this stage, because the whole video has to be compressed later anyway. The finished AVI is written to:

```text
C:\...\My documents\Trackmania\   (for example Video01.avi)
```

Workflow hint: shoot many short clips instead of one long one. The AVI output format has a hard size limit of about 4 GB, and several clips give more freedom during post-production when they are edited and rearranged into the final video.

Audio recording problems are covered by a separate forum discussion rather than in the tutorial text.

## Section 5: Video editing and compression

Additional effects and cuts are added in a video editing program. If none is available, the tutorial suggests creating as many effects as possible inside the MediaTracker itself, which can place pictures and music, create transitions and text effects; only the more elaborate effects need a dedicated editor.

Free editing and effects software named by the tutorial at the time:

- Microsoft Movie Maker (installed with Windows)
- Blender
- Zweistein
- Jashaka
- Wax

Commercial editors are noted as existing but expensive.

### Compression and conversion

An uncompressed AVI, for example one being readied for TM-Tube, must be converted or compressed. For inexperienced users the tutorial recommends **SUPER**, a free program that converts between many formats.

### Preparing a video for TM-Tube

The video should already be converted to the right format before uploading, so that TM-Tube does not re-compress it and degrade the quality. The tutorial's target settings:

```text
format:     .FLV/MP4 video
resolution: 1280*720 pixels (the resolution TM-Tube uses)
fps:        30 (the same frame rate used for the shoot)
bitrate:    1500-2500 (depending on the amount of action)
```

That completes the tutorial's basics of creating a TrackMania video.

## Credits

Recorded authors and translators in the captures:

- "Video creation" / "Trackmania - Video creation": author TStarGermany; translation by -DexM-.

Related pages: [Mediatracker authoring](mania-creative-mediatracker.md), [File and asset reference](mania-creative-file-and-asset-reference.md), [Trackmaking](mania-creative-trackmaking.md), [TrackMania formats](trackmania-formats.md).
