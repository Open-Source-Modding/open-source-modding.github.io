# Mania-Creative: Manialinks and Page Building

> **Source**: Mania-Creative (mania-creative.com), a private TrackMania site live 2010-2013, recovered from Wayback Machine captures of tutorials.mania-creative.com. This page draws on five captures: 2011-01-23 ("ManiaLink + ManiaCode Introduction" by Trick), 2011-05-18 ("Building a Manialink page", page 1, by Marcel), 2011-12-23 (the TMF-era revision of Trick's introduction), 2012-01-08 (page 4 of Marcel's build tutorial, "Interesting aspects in the end") and 2011-12-28 ("TrackMania 2 - General information" by TStarGermany). The text is a community tutorial, not official Nadeo documentation. The TMF-era revisions are preferred over the earlier captures; where the two differ or where only one revision covers a point, that is stated.

---

## What ManiaLinks and ManiaCodes are

A ManiaLink is a page viewed inside the game's in-game browser (the Explorer). It is written in XML, not HTML, and lives on ordinary web hosting: Nadeo does not host ManiaLinks, so an author needs their own file host and directory structure. A ManiaLink can link to other ManiaPages, and it can offer downloadable content such as avatars, car skins, 3D models, replays, tracks and movie files.

A ManiaCode is a special variety of ManiaLink that can interact directly with the game and make it perform actions, for example installing a skin, a modification or a track. The two share the same file format and the same registration system; the difference is the registered code type and whether the game is allowed to act without asking first.

## Getting started: XML pages and friendly codes

ManiaLink pages are XML documents. Instead of giving players a raw URL, the registration system assigns a friendly code name to each hosted XML file, and the player types that code into the in-game browser address box.

- Open the Explorer (top-left shows "Manialink:Home").
- Type the code in the address box, for example `tmtp:///:trick`.
- The browser looks up the URL assigned to that code and displays the XML, typically as one or more message boxes awaiting confirmation.

The player never sees the underlying URL while browsing, though it appears in the upper-right corner when the link is hovered.

## Code naming and the naming scheme

Each account creates a chain of codes on the Player page. The player's own login is the primary code; every other code is a prefixed variant of it:

- `Trick` is the primary ManiaLink for that account.
- `trick:p1`, `trick:p2` are further pages.
- `trick:p1c1s` denotes page 1, cell 1, screenshot.

At least one code must exist for other players to reach the ManiaLink at all, unless they type the full XML URL directly. A code cannot be renamed: it must be deleted and recreated with the new name. The directory structure and the XML files must already be in place on the host before a code is created or edited, otherwise saving returns an error.

## The Player page

Codes are registered on the TrackMania Forever Player page at `http://official.trackmania.com/tmf-playerpage/main.php`. Log in with a TrackMania United account, and use the **Views** box in the lower left, under the Manialinks and ManiaCodes headings, to assign URLs from one's own host to the general TrackMania Manialink database.

Workflow:

1. Enter the code name and the target XML URL in the top boxes.
2. Click **Save this Code**; a green confirmation appears.
3. The left column lists every code and the XML file it resolves to.

The old TrackMania United Player page was shut down when the Forever Addon arrived; the registered codes were carried over to the new Player page at the end of the beta, so older ManiaLinks remain reachable.

## Assigning a ManiaLink or a ManiaCode

| Member | Code type | Coppers cost | Behaviour |
|--------|-----------|--------------|-----------|
| ManiaLink | `ManiaLink` | zero (may be set) | Executes with no user intervention. A coppers cost can be set, but the player then receives no downloadable content. This is how pages link from one to the next. |
| ManiaCode | `ManiaCode` | non-zero | Does not execute until the player accepts the charge. Used to sell downloadable content. |

For a ManiaCode the coppers are split between the contributing players, with at least 10% going to Nadeo. Use **Remove** or **Modify** before clicking **Update this Code** to change an existing entry. Codes should be tested before they are made public. Syntax is case-sensitive, and a broken XML file can still be paid for by the player while the content fails to download, so the file must be valid before the ManiaCode is published.

A suggested starting point is `manialink_template.zip` by Bla, but that template predates the Forever Addon tags and does not include them.

## The Forever Addon element set

With the Forever Addon the ManiaLink structure changed from the TrackMania United era. The narrow borders of the old system broke, but the format became far more flexible. The old `<line>` and `<cell>` elements are gone, replaced by `<quad>`, `<label>` and `<frame>`, which can be placed anywhere. All old TrackMania United ManiaLinks still work in TrackMania Forever.

The element reference below is the tutorial's own table of contents; the captured pages reproduce the titles but not the detail text, so the descriptions are the one-line summaries the TOC supplies.

| Element | Purpose |
|---------|---------|
| `<frame>` | Groups several elements so they can be moved and positioned together. |
| `<quad>` | Displays pictures and shapes. |
| `<format>` | Global definition of text and layout formats. |
| `<label>` | Displays text and buttons. |
| `<entry>` | A simple text entry field. |
| `<fileentry>` | An entry field for uploading a file. |
| `<timeout>` | Avoids caching of the page. |
| `<include>` | Imports another XML file. |
| `<music>` | Outputs background music. |
| `<audio>` | Outputs an audio file. |
| `<video>` | Outputs a video file. |

The tutorial's own pages on positioning and alignment, and on each of these elements, are not present in any archived capture. Only their titles survive, so the exact rules for the "two types of positioning", the third dimension, and the effects of `halign` and `valign` are not covered here.

## Frames, positioning and z-order

Elements take a `posn` attribute holding an X, Y and Z coordinate. The third value is the z-order: a larger value sits nearer the viewer.

- `<frame posn="X Y Z">` moves its whole contents as a unit; child coordinates are relative to the frame's own position, and negative values place a child to the lower-left.
- A `<quad>` with no `posn` inherits the frame and is commonly used as the window background.
- Stacking a title-bar quad below a label in z-order lets the quad's background sit behind the text.

Old-format ManiaLinks can be mixed with new ones: split the page into two parts, build the old part as a frame, and give it a z-value such as `pos="X Y -0.04"` or `posn="X Y 2.56"` so new elements can be placed in front of or behind it. Note that opening a partly or fully new ManiaLink in TrackMania United shows only the old part, or nothing, and offers no error message.

## Worked example: the "Test Quad" input window

The tutorial's extended example is an input window, published as the "Test Quad" ManiaLink. Values entered in the fields are submitted to a PHP script as URL parameters. This is the captured markup, with the URLs shortened in the original.

```xml
<?xml version="1.0" encoding="utf-8" ?>
<manialink>
  <frame posn="-60 -10 5">
    <label posn="25 -2 15" sizen="40 4" halign="center" style="TextTitle3" text="Input Window (Menu Mode)"/>
    <quad posn="25 -1 10" sizen="40 4" halign="center" style="Bgs1InRace" substyle="BgTitle3_4" manialink="http://example.com/manialink/quad/index.php?type=classic&amp;lang=en&amp;x=inputx&amp;y=inputy"/>
    <label posn="15 -6 5" halign="center" style="TextStaticSmall" text="$o$ff0Position:"/>
    <label posn="15 -8.5 5" halign="right" style="TextStaticSmall" text="$oX = "/>
    <label posn="15 -11 5" halign="right" style="TextStaticSmall" text="$oY = "/>
    <entry posn="15 -8.5 5" sizen="5 2" style="TextValueSmall" name="inputx" default="0"/>
    <entry posn="15 -11 5" sizen="5 2" style="TextValueSmall" name="inputy" default="0"/>
    <label posn="35 -6 5" halign="center" style="TextStaticSmall" text="$o$ff0Size:"/>
    <label posn="37 -8.5 5" halign="right" style="TextStaticSmall" text="$oWidth = "/>
    <label posn="37 -11 5" halign="right" style="TextStaticSmall" text="$oHeight = "/>
    <entry posn="37 -8.5 5" sizen="5 2" style="TextValueSmall" name="inputwidth" default="32"/>
    <entry posn="37 -11 5" sizen="5 2" style="TextValueSmall" name="inputheight" default="32"/>
    <label posn="25 -15 5" halign="center" style="TextStaticSmall" text="$o$ff0Alignment:"/>
    <label posn="25 -17.5 5" halign="right" style="TextStaticSmall" text="$ohalign = "/>
    <label posn="25 -20 5" halign="right" style="TextStaticSmall" text="$ovalign = "/>
    <entry posn="25 -17.5 5" sizen="10 2" style="TextValueSmall" name="inputhalign" default="left"/>
    <entry posn="25 -20 5" sizen="10 2" style="TextValueSmall" name="inputvalign" default="top"/>
    <quad posn=" 7 -25.5 5" sizen="6 3" halign="center" image="http://example.com/manialink/quad/en.png" manialink="http://example.com/manialink/quad/index.php?type=menu&amp;lang=en&amp;x=inputx&amp;y=inputy"/>
    <quad posn="43 -25.5 5" sizen="6 3" halign="center" image="http://example.com/manialink/quad/de.png" manialink="http://example.com/manialink/quad/index.php?type=menu&amp;lang=de&amp;x=inputx&amp;y=inputy"/>
    <label posn="25 -25 5" halign="center" style="CardButtonMedium" manialink="http://example.com/manialink/quad/index.php?type=menu&amp;lang=en&amp;x=inputx&amp;y=inputy" text="Apply Values"/>
    <quad sizen="50 30" style="Bgs1InRace" substyle="BgWindow2"/>
  </frame>
</manialink>
```

Points the tutorial makes about this listing:

- `<frame>` groups everything; the final `<quad>` has no `posn`, so it inherits the frame and draws the window (`Bgs1InRace` with `substyle="BgWindow2"`).
- Labels use `style` names such as `TextTitle3`, `TextStaticSmall` and `TextValueSmall`. In `text`, `$o` sets bold and `$ff0` sets yellow.
- `<entry>` fields carry `name` and `default`. The name becomes the URL variable sent to the script, and `default` is the value shown before editing.
- `halign` and `valign` accept `left`, `center`, `right` and `top`, `center`, `bottom`.
- The button is a `<label>` using `CardButtonMedium`; its `manialink` attribute points at the same script, and the entered values are substituted into the URL from the matching `name` fields.
- Values are joined with `&`, which must be written `&amp;` inside XML. At the receiving end `...?type=menu&amp;lang=en&amp;x=inputx&amp;y=inputy` resolves to `...?type=menu&lang=en&x=0&y=0`.
- Flag quads use the `image` attribute (24-bit `.png` is supported) and a `manialink` attribute to switch language. `imagefocus` supplies the mouse-over image.

## Linking with the TMTP protocol

With the Forever Addon, a single click can open a ManiaLink. Use the `tmtp://` protocol in place of `http://`:

- Full URL: `tmtp://funtrackers.bplaced.net/manialink/quad/index.php`.
- Registered code: `tmtp:///:code`. The three slashes matter, otherwise some browsers append a trailing slash and the code becomes invalid in game. Example: `tmtp:///:Marcel`.

If TrackMania is running, the clicked link is handed to it (a browser confirmation may appear); if it is not running, the game launches and displays the content. Special characters are URL-encoded and break the link: a space becomes `%20`, so `tmtp:///:Test Quad` is transmitted as `tmtp:///:Test%20Quad` and fails.

## Multi-language ManiaLinks

The game can pick the player's own language automatically. All translations live in one ManiaLink plus a language file; the game selects the matching language and falls back to English when a key is missing. A missing key ultimately resolves to nothing, so English should be complete.

```xml
<?xml version="1.0" encoding="utf-8"?>
<manialink>
  <timeout>0</timeout>
  <include url="http://example.com/manialink/lang.xml" />
  <label posn="-20 0 1" textid="example" />
  <label posn="-20 -10 1" textid="nadeo" />
  <quad sizen="10 5" imageid="img" />
</manialink>
```

```xml
<dico>
  <language id="de">
    <example>Beispiel</example>
    <nadeo>Danke Nadeo für diese neuen ManiaLinks</nadeo>
    <img>http://example.com/manialink/de.png</img>
  </language>
  <language id="en">
    <example>Example</example>
    <nadeo>Thanks Nadeo for these new ManiaLinks</nadeo>
    <img>http://example.com/manialink/en.png</img>
  </language>
</dico>
```

The ManiaLink uses `textid` and `imageid` instead of literal `text` and `image`; each ID is a key in `lang.xml`. ID names must start with a letter, may then contain numbers and underscores, and should avoid other special characters. Save `lang.xml` as UTF-8 so special characters survive.

Attributes known to accept an appended `id` form (this list came from the author's own tests and is described as incomplete):

| Element | Multi-language attributes |
|---------|---------------------------|
| `<quad>` | `image`, `imagefocus`, `url`, `manialink` |
| `<label>` | `text`, `url`, `manialink` |
| `<audio>` | `data` |
| `<video>` | `data` |

Language codes used by the game's language files:

| Code | Language | Code | Language | Code | Language |
|------|----------|------|----------|------|----------|
| cz | Czech | it | Italian | pt | Portuguese |
| de | German | jp | Japanese | ru | Russian |
| en | English | kr | Croatian | sk | Slovak |
| es | Espanol | nl | Dutch | zh | Chinese |
| fr | French | pl | Polish | hu | Hungarian |

## Passing parameters to a ManiaLink

A registered ManiaLink can take parameters the same way a PHP page does. Register the page without parameters, for example code `Page` pointing at `http://www.example.com/page.php`, then call it with `Page?id=15` and read the value server-side as `$_GET['id']`. Further variables are appended with `&amp;`, which is a normal `&` in TrackMania.

## United versus Nations

TrackMania United Forever is the paid game and offers the complete ManiaLink feature set. TrackMania Nations Forever has three limitations:

1. **No registered ManiaLinks.** Nations cannot register its own codes, nor reach registered codes. ManiaLinks can only be viewed by entering the complete XML URL, so a Nations-friendly page should make every link a full URL. Nations also has no Explorer menu entry; move the mouse to the top border and click the left icon to open it.
2. **No ManiaCodes.** Nations cannot use any. Direct URL input was already denied in United, and access through registered codes is unsupported, so Nations players cannot download content. The tutorial's phrasing is that they can only stare at the ManiaLinks.
3. **Green is the world.** ManiaLinks can borrow the game's menu textures through styles. Nations menus are almost entirely green, so its ManiaLink styles are green too, whereas United offers other colours.

Logging into Nations with a United account removes only the third limitation. The first two follow the account type rather than the game build. Element functionality is unaffected: Nations can still use entries and file entries.

## Known issues

- **Texture quality changes appearance.** Images on a ManiaLink render according to the launcher's texture setting. Only players set to texture quality "high" see ManiaLinks in full quality; lower settings disfigure the images.
- **`AddPlayerID` is manipulable.** The `AddPlayerID` behaviour on quads and labels can be bypassed by entering the URL manually, so anything security-critical must be secured another way.
- **Missing `folder` attribute crashes the game.** If the `folder` attribute is absent from a `<fileentry>` tag, opening the ManiaLink drops straight to the desktop. Write `folder=""` into the tag to avoid it.

## TrackMania 2

TrackMania 2 reuses much of the TrackMania United structure, and the site advised falling back to the TrackMania 1 tutorials when no TrackMania 2 equivalent exists. Manialinks remained part of the workflow: the general-information page lists a `[TM2] Graphics / Sound / Modding / Manialinks` subforum, and describes Planets, the TrackMania 2 in-game currency, as something players earn partly by selling their own content to others through Manialinks or Packs.

The linking protocol changed name. The TrackMania 2 captures show `maniaplanet:///:creative` links, the successor to the Forever Addon's `tmtp://` form. As with TMTP, the protocol is added to the Windows registry at install time, and registry cleaners can strip it out; the site offered a `.reg` file that could be edited to point at the install path and re-imported to restore the links. Beyond these references, no TrackMania 2 ManiaLink authoring tutorial appears in the archived captures.

## Credits

Recorded in the captures:

- **Trick**, author of "ManiaLink + ManiaCode Introduction".
- **Marcel** (FT»Marcel), author of "Building a Manialink page"; German version with special thanks to ECU kastun, who also translated nearly half of it and checked the translations.
- **JonTheKiller**, translator; English translation of the build tutorial also by FT»Dany, CyR4S and TStarGermany.
- **TStarGermany**, author of "TrackMania 2 - General information".
- **iNDEX**, translator of the TrackMania 2 page.
- **Konte**, credited for the multi-language attribute list.
- Beta testers, and the FT and ECU clans, for checking the build tutorial.

Related pages: [File and asset reference](mania-creative-file-and-asset-reference.md), [Mediatracker authoring](mania-creative-mediatracker.md), [TrackMania formats](trackmania-formats.md).
