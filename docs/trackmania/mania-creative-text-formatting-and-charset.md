# Mania-Creative: TrackMania Text Formatting and Character Sets

> **Source**: tutorials.mania-creative.com (Mania-Creative), a community TrackMania tutorial site live 2008-2013, recovered from the Wayback Machine captures at web.archive.org. Pages used and their capture dates: Formatting text 2010-06-28 and 2011-08-10; Colours 2010-06-25 and 2011-08-07; Special characters 2010-06-28 and 2011-06-14; Unicode chart index 2010-06-30 and 2011-05-06; the per-block Unicode chart pages 2011-04-26 to 2011-04-29; the TrackMania Forever (`tm_`) revisions of Colours 2012-04-11 and Special characters 2012-03-13; and TrackMania 2 Formatting text 2011-12-06. The tutorials are community work, authored by TStarGermany, with translations listed under Credits.

TrackMania runs every text entry through a Nadeo text interpreter. The same interpreter serves usernames, chat messages, track and challenge names, and Mediatracker text tracks. It understands three independent families of codes, and all three can be used together in a single string:

- Control codes, which start with `$` and change letter style, spacing or links.
- Colour codes, which are `$` followed by a three-digit RGB value.
- Special (Unicode) characters copied in from a text editor.

## Control codes

Control codes are typed directly into the text. The interpreter converts them the moment the text is submitted.

| Code | Effect | Notes |
|------|--------|-------|
| `$i` | italic | |
| `$s` | shadowed | |
| `$t` | uppercase | |
| `$w` | wide spacing | |
| `$n` | narrow spacing | |
| `$m` | normal setting | reset code |
| `$g` | default colour | reset code |
| `$z` | reset all | reset code |
| `$$` | writes a literal `$` | escape |
| `$000` to `$fff` | RGB colour | see [Colour codes](#colour-codes) |

A code applies to every following letter until a reset code is used, or until the same code is used again. In the source list the reset codes (`$m`, `$g`, `$z`) are marked in bold for exactly this reason.

### TrackMania Forever special codes

The Forever-era page added three codes that the older track page did not list:

| Code | Effect |
|------|--------|
| `$o` | bold with semi-wide spacing |
| `$h` | internal link to a ManiaLink |
| `$l` | external link to a URL |

The link syntax wraps the label: `$l[http://www.domain.com/]Name$l`. The same page warns that `$h` and `$l` are not accepted everywhere, and the sibling [Mediatracker Authoring](mania-creative-mediatracker.md) page records that manialinks are not allowed in a Mediatracker text track.

### Worked example

```text
"$nNarrow and $tuppercase plus $iitalic are nasty, but $znow everything is reset."
```

The shadowed letters, the uppercase run and the italic run all return to normal at `$z`. Without a reset the change would carry through the rest of the line.

### Removing control codes

When a large collection of track names or filenames has to be read plain, the codes can be stripped with a regular expression. The capture gave this syntax for a multi-rename tool:

```text
(\$[hilswnmogztHILSWNMGZOT]{1}|\$[\d\a-fA-F]{3})
```

The first alternative matches a single-letter control code in either case; the second matches a three-digit colour code. Applied across thousands of filenames it removes the codes in one pass.

## Colour codes

A colour code is `$` followed by three hexadecimal digits, one per channel in RGB order, for example `$F00` for red. `$g` restores the default colour. The value is case-insensitive in the source chart, which writes `$C00` and `$c00` alike.

### Colour chart

The published chart is deliberately reduced: each channel is sampled at six levels, Dark to Intense in steppings of 3 (`0, 3, 6, 9, C, F`) rather than all 256 values. To read a value, mark the text in the colour box on the original page.

| Digit | Intensity |
|-------|-----------|
| `0` | none (darkest) |
| `3` | low |
| `6` | mid-low |
| `9` | mid-high |
| `C` | high |
| `F` | full (brightest) |

The chart's most useful landmarks:

| Code | Colour |
|------|--------|
| `$000` | black |
| `$333` | dark grey |
| `$666` | mid grey |
| `$999` | light grey |
| `$CCC` | near white |
| `$FFF` | white |
| `$F00` | red |
| `$0F0` | green |
| `$00F` | blue |
| `$FF0` | yellow |
| `$0FF` | cyan |
| `$F0F` | magenta |

Any other channel combination is read the same way, for example `$F30` is full red, low green, no blue. The chart also prints a Black to White row (`$000`, `$333`, `$666`, `$999`, `$CCC`, `$FFF`) as a greyscale shortcut.

The colour interaction with the `$s` shadow code and the Mediatracker paint colour is covered in [Mediatracker Authoring](mania-creative-mediatracker.md).

## Special (Unicode) characters

Nadeo released the games across many markets, so the interpreter also accepts Unicode character sets beyond Latin. The chart index states that TrackMania can display around 13,000 signs. Support is not complete: the index warns that TrackMania does not support every character of every charset, and an unsupported code point renders as a `~square~` placeholder instead of the intended glyph.

The practical uses are cosmetic. Foreign alphabets offer many shapes that substitute for Latin letters, and letters from different alphabets can be mixed in one word. The source shows worked examples including a username that reads as a "giant spider chasing 3 people". The same freedom applies to chat messages and Mediatracker text.

### Multiline and vertical text

Pasting a multi-line string produces stacked text, because the interpreter preserves the line breaks. The source method: open a text editor, write the letters across several lines, then copy and paste the whole block into TrackMania. It also asks that this not be used for a username, because a vertical name looks poor and irritates other players on a server.

### Unicode chart coverage

The archive preserves one chart page per script block. Each page renders the supported code points in a grid. The block list is:

| Chart page | Heading(s) | Coverage |
|------------|------------|----------|
| `01_latin` | Basic Latin, Latin-1 Supplement, Latin Extended A, Latin Extended B | standard western alphabet; Latin Extended B is only partly supported |
| `02_cjk1` | CJK Compatibility forms, CJK Compatibility Ideographs, CJK Symbols and punctuation | CJK compatibility and punctuation |
| `03_cjk2*` | CJK Unified Ideographs Parts 1 to 6 | the main CJK ideograph blocks, described as huge |
| `04_cyrillic` | Cyrillic | Cyrillic |
| `05_devanagari` | Devanagari | Devanagari |
| `06_generalpunctuation` | General punctuation | punctuation signs |
| `07_greekcoptic` | Greek and Coptic | Greek and Coptic |
| `08_hanguljamo` | Hangul Jamo | Korean jamo |
| `09_hangulsyllables*` | Hangul Syllables Parts 1 to 4 | Korean syllable blocks, described as huge |
| `10_hebrew` | Hebrew | Hebrew |
| `11_hiragana_katakana` | Hiragana, Katakana | Japanese kana |
| `12_thai` | Thai | Thai |

### Viewing the charts on Windows XP

The chart pages were built for a desktop that had the relevant scripts installed. The index gave this procedure for Windows XP to view them correctly:

1. Open the Start menu and the System control panel.
2. Choose "Date, Time, Language, and Regional Options".
3. Open the "Languages" tab.
4. Activate "Install files for complex script and right-to-left languages".
5. Activate "Install files for East Asian languages".

## TrackMania 2 differences

The archived TrackMania 2 page is titled "Trackmania 2 - Formatting text" and was last updated 2011-09-16, with no translations credited. Its recovered half is page 2, headed "Special characters (Unicode)". Its substance matches the Forever page: the interpreter supports the same Unicode sets, the same warning about partial coverage and `~square~` placeholders, and the same chart list, which on the TM2 page points at the `tm_general_formattingtext_special_chart` paths. The multiline and vertical-text advice is repeated word for word.

No TrackMania 2 control-code differences can be reported, because that page's first half (the control codes themselves) was not recovered. The TMF control-code tables above are the newest captured wording on the subject.

## Notes on the source

- The `tm_` revisions are the TrackMania Forever rewrites. The `tm_` Colours page (2012-04-11) reprints the same colour chart as the plain page, differing only in whitespace. The `tm_` Special characters page (2012-03-13) is the Dutch translation of the same text (title "Trackmania - Speciale Karaktersoorten"), with the same facts. There is no factual disagreement between the `tm_` pages and the plain pages, so the plain English wording is used here for readability and the `tm_` wording is not quoted separately.
- The `tm_general_formattingtext` capture (2024-06-22) is unusable. By that date the domain had changed hands and the capture is an unrelated Chinese-language novel site, so it contains none of the tutorial. The control-code wording therefore rests on the 2010 and 2011 plain page captures.

## Credits

Recorded on the captures:

- Author: TStarGermany, for the Formatting text, Colours, Special characters, Unicode chart and TrackMania 2 pages.
- Formatting text translations: Adriweb, Fakko, oliverde8, Tamonte, Tuinhek.
- Colours translations: Adriweb, Fakko, Tamonte, xZise.
- Special characters translations: Adriweb, Fakko, Tamonte, trackmania.gen.tr, Jochem285.
- Unicode chart translations: Adriweb, Fakko, Tamonte.

Siblings in this folder: [Mania-Creative: File, Graphic, Sound and Locator Reference](mania-creative-file-and-asset-reference.md) and [Mediatracker Authoring](mania-creative-mediatracker.md).
