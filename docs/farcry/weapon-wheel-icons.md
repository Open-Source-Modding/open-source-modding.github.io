---
title: Adding Weapon Wheel Icons
---

How to add custom icons to the Far Cry weapon wheel (and possibly other FEU files). Based on a tutorial by BIRDdude12.

## Overview

The weapon wheel icons live inside SWF files, which are stored as XML when decompiled. Adding a new icon requires:

1. Encoding a Base64 hash for your texture path
2. Adding the hash to the SWF XML
3. Creating a `DefineShape` block referencing the new texture
4. Registering a `FrameLabel` so the game can look up the icon
5. Recompiling the SWF back to FEU

## Prerequisites

- A C# command-line tool for Base64 hash encoding/decoding (included with the original tutorial)
- `fcbconverter` for SWF-to-FEU conversion
- A `.png` icon file (not `.xbt`)

## Step 1 — Encode the texture path

Run the command-line tool and select **Recompile**. Enter:

- Character ID (e.g. `850`)
- Width and height of the icon
- Texture path as a **`.png`** path (not `.xbt`)

The tool outputs a Base64-encoded value. Keep this for the next step.

## Step 2 — Add the hash to the SWF

Open the SWF XML. Look for tags like:

```xml
<UnknownTag id="0xF6">
    <data>3wAsATwAdWlccmVzb3VyY2VzXHRleHR1cmVzXDA2X2ljb25zXHdlYXBvbnNcdHhfMTg4Ny5wbmcAAAA=</data>
</UnknownTag>
```

Duplicate the entire `UnknownTag` block, then replace the `<data>` content in the duplicate with your new Base64 value:

```xml
<UnknownTag id="0xF6">
    <data>3wAsATwAdWlccmVzb3VyY2VzXHRleHR1cmVzXDA2X2ljb25zXHdlYXBvbnNcdHhfMTg4Ny5wbmcAAAA=</data>
</UnknownTag>
<UnknownTag id="0xF6">
    <data>UgMwAoQAdWlccmVzb3VyY2VzXHRleHR1cmVzXDA2X2ljb25zXHdlYXBvbnNcdHhfcmVjdXJ2ZWJvd21ha2VzaGlmdC5wbmcAAAA=</data>
</UnknownTag>
```

## Step 3 — Find the closest existing icon

Pick an existing icon as close as possible to yours (e.g. a recurve bow icon for a custom bow).

1. Decode its Base64 hash to find the character ID
2. Search the SWF XML for that character ID
3. Locate the surrounding `DefineShape` block

For example, the recurve bow icon uses character ID `22` and is wrapped in `DefineShape objectID="477"`.

## Step 4 — Duplicate the DefineShape

Copy the entire `DefineShape` block. Replace the inner `ClippedBitmap2 objectID` with your icon's character ID:

```xml
<DefineShape objectID="477">
    <bounds>
        <Rectangle left="-2680" right="2920" top="-660" bottom="660"/>
    </bounds>
    <styles>
        <StyleList>
            <fillStyles>
                <ClippedBitmap2 objectID="65535">
                    <matrix>
                        <Transform scaleX="20.00000000000000" scaleY="20.00000000000000" transX="0" transY="0"/>
                    </matrix>
                </ClippedBitmap2>
                <ClippedBitmap2 objectID="22">
                    <matrix>
                        <Transform scaleX="10.00000000000000" scaleY="10.00000000000000" transX="-2680" transY="-660"/>
                    </matrix>
                </ClippedBitmap2>
            </fillStyles>
            <lineStyles/>
        </StyleList>
    </styles>
    <shapes>
        <Shape>
            <edges>
                <ShapeSetup x="2920" y="660" fillStyle1="2"/>
                <LineTo x="-5600" y="0"/>
                <LineTo x="0" y="-1320"/>
                <LineTo x="5600" y="0"/>
                <LineTo x="0" y="1320"/>
                <ShapeSetup/>
            </edges>
        </Shape>
    </shapes>
</DefineShape>
```

In the duplicate, change the `ClippedBitmap2 objectID` to your icon ID (e.g. `850`) and set the `DefineShape objectID` to your icon ID minus 1 (e.g. `849`):

```xml
<DefineShape objectID="849">
    ...
    <ClippedBitmap2 objectID="850">
    ...
</DefineShape>
```

## Step 5 — Register the FrameLabel

Search the XML for the keyword of the icon you copied (e.g. "recurve"). You will find blocks like:

```xml
<FrameLabel label="BowRecurve">
    <flags/>
</FrameLabel>
<PlaceObject2 replace="0" depth="1" objectID="477">
    <transform>
        <Transform transX="0" transY="0"/>
    </transform>
</PlaceObject2>
<ShowFrame/>
```

Duplicate this block and change it to your new icon name:

```xml
<FrameLabel label="PrimitiveBow">
    <flags/>
</FrameLabel>
<PlaceObject2 replace="1" depth="1" objectID="849"/>
<ShowFrame/>
```

The `objectID` in `PlaceObject2` is the `DefineShape` objectID from Step 4 (your icon ID minus 1).

## Step 6 — Add to the store section

If the icon appears in the weapon wheel but not in the store, find the second occurrence of the keyword and paste your `FrameLabel` block there too.

## Step 7 — Recompile

1. Recompile the XML back into an SWF
2. Convert the SWF to FEU using `fcbconverter`
3. Verify the icon texture actually exists at the path you specified

For weapons, the icon ID is stored in the `itemdescriptor` file under the `icon` field (e.g. `PrimitiveBow`).

## References

- Original tutorial by BIRDdude12
- `fcbconverter` for SWF/FEU conversion
