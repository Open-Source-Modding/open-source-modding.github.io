# **Prerequisites:**

* Unpacked Far Cry 4 files 
* FCB Converter 
* Any xml editor of your choice 
* Understand info\_replace and using MI file insertion method (pair) 
* Good backup routines 
* Basic RGBA knowledge

## Why edit textures/materials?

You want a different color on an object or you just want to make meme mods (replace every texture with memes or other funny stuff). No, I am not going to tell you how to make textures from scratch. This is more of a color change guide on weapons.

## Where are all files located?

In Far Cry 4, all textures/material.bin files are located in an unpacked **FCC\_Main\\graphics\\\_materials** folder. Use FCBConverter to get them. In patch\_hd, they will be saved into **graphics\\\_materials.** This is important when making the info\_replace package.

## How to find the correct material?

In a weapon archetype file, scroll down to the skin list. Here’s the gold skin material entries for the AK47 as an example: 
![][image1]

And here’s the AMR skin 
![][image2] 
Find the **matMaterials** entry. Copy the file name into your unpacked folder. Why these entries and not the others?

## Do I need all the skin files?

No, but you must convert the bins to get them into human readable language. Skin slots with overrides have a **matMaterial** value assigned. Go into the graphics folder and copy the ones needed. **Backup the originals and the ones you are changing, once you're pleased with the results\!**

## Far Cry 4 Skin IDs:

* id="167196" value="BLACK" 
* id="167197" value="RED" 
* id="167198" value="GRANITE" 
* id="167199" value="PURPLE" 
* id="167200" value="DRY LANDS" 
* id="167201" value="GOLD" 
* id="167202" value="CLASSIC" 
* id="167203" value="VINTAGE" 
* id="167204" value="STALKER" 
* id="167205" value="GREEN" 
* id="167206" value="FIERCE" 
* id="167207" value="SILVER" 
* id="167208" value="PREDATOR" 
* id="167209" value="MODERN" 
* id="167210" value="BUSHMAN" 
* id="167211" value="WHITE" 
* id="167212" value="HUNTSMAN" 
* id="167213" value="PRIMAL" 
* id="167214" value="AQUA"

Signature weapons have unique skins, named after their in-game name (AMR's skin is named "**amr**", Buzzsaw "**buzzsaw"** and so on).

## **Material fields to manipulate:**

FC4 uses diffuse color, anisotropy, specularity, gloss level and masks to define colors/shininess and reflectiveness of a material. Basically a PBR workflow

The fields are vector values. Most of the fields will be in Vector2, where the numbers represent black/white. For the vector4 fields, it is RGBA, where A is the Alpha channel. In calculators, the alpha mask is sometimes known as transparency/opaqueness.

**Diffusecolor1 (Vector4**): Base layer/black mask. Multiplicative for final color output. Set to color **white**. 
**Diffusecolor2 (Vector4)**: Detail Layer/white mask. Adjusts color.

**Metallicity:** How metallic is the material? Set to 1,1 by default for certain metal parts. Does not exist in FC3.

**Specular mask:** Minor details such as dirt, smudges etc **(recommended for FC4, a value of 3,3 or 4,4 can work).** Used for areas not affected by glossiness. Controlled by **DiffuseTiling2(Vector2)**. If specmask texture is not defined, copy this into your material.bin as “String”: **\<field hash="1D775834" name="value" type="String"\>graphics\\\_singleplayer\\\_textures\\generic\\specmasks\\tilingmask\_metal.xbt\</field\>**

Alternative texture: 
**\<field hash="1D775834" name="value" type="String"\>graphics\\\_singleplayer\\\_textures\\generic\\specmasks\\metalbeams.xbt\</field\>**

If a weapon breaks, try changing the mask intensity from 1 to 0\.

**Specular reflectance:** 
How reflective the surface will be in sunlight. 
Parameter: **SpecularReflectance, Vector2**. 
Do not overdo it\! Tune as you see fit

![][image3] 
My eyes\!

**Glossiness (Vector2)**: Reflectiveness of the material. Adjust as needed. The higher white value, the more reflective it will be. On metallic colors, **1,1** works nicely, if you do colors like gold or any other type of precious metal.

**MaskScaleOffset:** Controls wear and tear on a gun. negative values remove the color, positive keeps it. **Vector4**, with a range of \-10 to 10\. Edit the **3rd** value. If it breaks, try looking at a vanilla setting on the material. For smaller metal pieces or non-metallic parts, do not change\! 
Examples below on the SA50

[Default MaskScaleOffsets SA50](https://cdn.discordapp.com/attachments/846426174200741898/1084227953233236129/image.png) 
[With a value of 0,0,10,0](https://cdn.discordapp.com/attachments/931695774604066916/1087416026100146218/image.png) 
[What happens when the 1st value is altered](https://cdn.discordapp.com/attachments/931695774604066916/1087414599369896046/image.png)

Look at the difference\!

The top entries define the default textures being used. Do not edit them.

![][image4]

If you want the best result/remove camo maps, you can set **DiffuseTexture2** to white. In that field, change the file path to the white.xbt. If the material lacks this entry, add it. 
![][image5]

RGBA calculator: 
[RGBA Color Picker](https://rgbacolorpicker.com/)

Remember to convert values to **Vector4 and BinHex\!**

## **Unknown fields:**

![][image6]

Set both to 1\.

# **FAQ**

## **Why can’t I simply copy over from a file directly?**

A model will have different settings for normals and detail maps (Far Cry 4 uses detail maps to make textures appear more detailed, despite low base resolution).

![][image7]

Buzzsaw, but a straight copy of the AK-47 metal properties. Oh no\!

## **Can base weapons/attachments be edited?**

Yes. Skin slots with material overrides use the **matMaterial** slot to change color. If the slot is not filled, then the game will use the base texture instead. Since you understand how to edit a skin’s material/color, you can make every weapon have a desired color available as skin or default paint. Just remember to assign the modified skin to NPC weapons, then they can have bling bling too\!

## **Which skins to pick?**

Personal preference. I'd pick any camo that covers most of the gun by itself. Touching base materials might ruin it. Through mask scale, you can improve the coverage of the color.

## **Are the settings explained somewhere?**

Yes\! In the folder materialdescriptors in common, every setting for materials are listed there.

## **Thanks:**

 Legendhavoc175 and Schrotflinte(12) for help and guidance. Arman III for the Mod Installer and the rest of the community behind the FC modding tools.







