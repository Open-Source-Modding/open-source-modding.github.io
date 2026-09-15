[Finding FOV bytes via HEX editing]{.underline}

[presented by]{.underline}: AOY.

[thanks to]{.underline}: WSGF.

Aspect Ratios[:]{dir="rtl"}

To calculate your aspect ratio decimal divide the resolution\'s width by
its height.

Note that ultra-widescreen is marketed as 21:9 but the aspect ratio
actually varies slightly between resolutions (these are listed
separately below).

  ------------------------------------------------------------------------
           Common Name             Decimal          Formatted Value
  ----------------------------- ------------- ----------------------------
               5:4                  1.25              00 00 A0 3F

               4:3                  1.33              AB AA AA 3F

               3:2                   1.5              00 00 C0 3F

              16:10                  1.6              CD CC CC 3F

              15:9                 1.66\~             55 55 D5 3F

              16:9                 1.77\~             39 8E E3 3F

             1.85:1                 1.85              CD CC EC 3F

             2.39:1                 2.39              C3 F5 18 40

             2.76:1                 2.76              D7 A3 30 40

              3x5:4                 3.75              00 00 70 40

              3x4:3                   4               00 00 80 40

             3x16:10                 4.8              9A 99 99 40

             3x15:9                   5               00 00 A0 40

             3x16:9                5.33\~             AB AA AA 40

        21:9 (2560x1080)           2.37\~             26 B4 17 40

        21:9 (3440x1440)           2.38\~             8E E3 18 40

        21:9 (3840x1600)             2.4              9A 99 19 40

              32:10                  3.2              CD CC 4C 40

              32:9                 3.55\~             39 8E 63 40
  ------------------------------------------------------------------------

Resolutions[:]{dir="rtl"}

  ---------------------------------------------------------------------
     Value           Hex\                 Formatted Value            
                    Value                                            
  ------------ ---------------- ------------------------------------ --
      200           0x0C8                       C800                 

      240           0x0F0                       F000                 

      320           0x140                       4001                 

      480           0x1E0                       E001                 

      525           0x20D                       0D02                 

      540           0x21C                       1C02                 

      576           0x240                       4002                 

      600           0x258                       5802                 

      640           0x280                       8002                 

      704           0x2C0                       C002                 

      720           0x2D0                       D002                 

      768           0x300                       0003                 

      800           0x320                       2003                 

      840           0x348                       4803                 

      848           0x350                       5003                 

      854           0x356                       5603                 

      900           0x384                       8403                 

      960           0x3C0                       C003                 

      992           0x3E0                       E003                 

      1000          0x3E8                       E803                 

      1024          0x400                       0004                 

      1050          0x41A                       1A04                 

      1080          0x438                       3804                 

      1152          0x480                       8004                 

      1200          0x4B0                       B004                 

      1280          0x500                       0005                 

      1360          0x550                       5005                 

      1366          0x556                       5605                 

      1368          0x558                       5805                 

      1440          0x5A0                       A005                 

      1536          0x600                       0006                 

      1600          0x640                       4006                 

      1680          0x690                       9006                 

      1768          0x6E8                       E806                 

      1776          0x6F0                       F006                 

      1800          0x708                       0807                 

      1920          0x780                       8007                 

      2048          0x800                       0008                 

      2160          0x870                       7008                 

      2400          0x960                       6009                 

      2560          0xA00                       000A                 

      2720          0xAA0                       A00A                 

      2880          0xB40                       400B                 

      3072          0xC00                       000C                 

      3200          0xC80                       800C                 

      3360          0xD20                       200D                 

      3440          0xD70                       700D                 

      3840          0xF00                       000F                 

      4080          0xFF0                       F00F                 

      4320          0x10E0                      E010                 

      4800          0x12C0                      C012                 

      5040          0x13B0                      B013                 

      5120          0x1400                      0014                 

      5760          0x1680                      8016                 
  ---------------------------------------------------------------------

Notes:

Sometimes the [field of view]{.underline} (FOV) for a game can be found
in hexadecimal form in one of the game\'s files. The format you will
most likely see is the IEEE 754 standard which is comprised of four
hexadecimal pairs.

For example a FOV of 60.0 would be the following in hexadecimal -

42 70 00 00

This will almost always be formatted to be read from right to left in
the game\'s files.

So 42 70 00 00 becomes the following in the file.

00 00 70 42

You can use a IEEE 754 calculator like this one to get your FOV value
into hexadecimal format. Simply paste your FOV in the decimal field and
click the Rounded or Not Rounded button (should not matter) and look at
the hexadecimal result under the single precision fields.

Think of the hexadecimal value in this format -

ST UV WX YZ

You will be using it in this format -

YZ WX UV ST

Remember you can calculate the field of view by using the FOV Calc tool
on this site along with any of several other methods for calculating the
field of view.

When re-calculating FOV based on aspect ratio, it can be done using
trigonometric tangent functions to convert the angle to radians divided
into half, then converted into a zoomable angle that can be divided or
multiplied by the zoom factor and converted back into degrees.

For example, the current display ratio is 4:3 and the conditional aspect
ratio is 16:9, the FOV degrees of 45.0 would become 57.822402 from
dividing the aspect ratio by the conditional aspect after converting the
angle into a tangent angle:

[Degrees:]{.underline}

Arctan(Tan(Fovy \* PI / 360) / (4/3) \* (16/9)) \* 360 / PI

[Degrees converted from radians:]{.underline}

Arctan(Tan(Fovyradians / 2) / (4/3) \* (16/9)) \* 360 / PI

[Radians:]{.underline}

2 x Arctan(Tan(Fovyradians / 2) / (4/3) \* (16/9))

[Radians converted from Degrees:]{.underline}

2 x Arctan(Tan(Fovy \* PI / 360) / (4/3) \* (16/9))
