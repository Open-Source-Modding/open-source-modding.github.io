# WATCH DOGS 1

Algonquin Hood [MC2],  — 5/14/23, 11:01
also im trying to set up a second car color, i just copied the material from defalts wagon, in material editor inside user-defined options i set it to use "VehicleName_CarPaint_02" handle and set up the materialset accordingly, but it always ends up being a matte red color
 [MC2], 
Deleted User — 5/14/23, 11:01
did you made same slot name in xbg?
Algonquin Hood [MC2],  — 5/14/23, 11:02
if youre referring to the user-defined options then yeah it was something along the lines of "ambulance_carpaint_01" if i remember correctly, i told it to use "vehiclename_carpaint_02"
only lead i have is maybe the wrong material in itself is being used, since the main car paint is jlesage-m-2012022840268104.material.bin and the one i used for 2nd color is jlesage-m-2012022850500324.material.bin
Deleted User — 5/14/23, 11:05
for example muscle_retro_02
Image
Image
second pic from xbg

# Note
Read the actual file from WD1 for more context

Danbrother — 12/7/22, 13:33
I've found the needed jacket for the mash-up, but the color is wrong. How can I find the needed material used to change the color?
Image
So, I know the needed name for the model:  <field name="graphickitpartPart" type="BinHex">BA710830</field> 
But don't know the name to put in material line: <field name="selMaterialOverridesId" type="Enum">1132120660</field>
Where can I find all the material names used with that model?
The Silver — 12/7/22, 13:37
Where can I find all the material names used with that model?
⁠wd1_modding⁠ 
Danbrother — 12/7/22, 13:41
This?
Image
Or E04CA5D2?
The Silver — 12/7/22, 13:42
This?
Yep, take the binhex "Name" value from that list of materials of your part and convert it from Byte Order "Intel" to "unsigned long" with this tool for example (because in graphickit_parts it's type "binhex" but in graphickit_models it's type "enum") then you put it as the selMaterialOverridesId value. 
Attachment file type: unknown
BConv32.exe
57.55 KB
Danbrother — 12/7/22, 13:45
Right?
Image
The Silver — 12/7/22, 13:51
Right?
No should be like this (I've also updated my previous post): "Name" field binhex value -> Byte Order "Intel" / Data Type "unsigned long" -> enum value for "selMaterialOverridesId" 
Image
Danbrother — 12/7/22, 15:01
Thank you so much once more!
Image
Image
Already looks good imo. But if I manage to change shirt color for white and Aiden's hair for blond and something similar - it would be perfect 😉
Image
Image
Image
pepsilover117 — 12/7/22, 18:10
This reminded me of david martinez
ola ola [MIKU],  — 12/7/22, 18:56
is that james bond
Mikan — 12/7/22, 18:59
It is indeed James Bond
The Silver — 12/8/22, 01:29
@Danbrother Super cool! 😄 and if you want finer control beyond existing variations you can try editing the actual material.bin file of your chosen variant, materials controls all sorts of things like which texture it uses, how it should be colored (like the single parts too, so color of jacket and color of inner shirt) and so on
Danbrother — 12/8/22, 03:17
Would be cool! But how can I do it? Probably, first I beed to extract the library with all the material files. Then I need to find the files for the needed model part and there find the references for the needed textures. Right?
The Silver — 12/8/22, 03:25
For materials you already have the filename from the field 9301DFBD and then you just find it in graphics/_materials after unpacking windy_city. 
Editing that material.bin file however requires some extra effort, you can either hex edit it (pain in the ass) or edit it from ZModeler (don't know if it has export problems) or convert it to xml and edit that (don't know if it's always converted back properly). Sadly it's been quite a while since I messed with materials so it's better if someone else steps up with more accurate info, for now my best bet would be converting it to xml through the Disrupt Editor material tool.
@Vector4782 (Villain Arc) might help with that ^

***0kb file*** - something's wrong on export, most likely one or more parts of the mesh (in any lod) do not have the proper material, and are using "Default Material"

***Out of Memory*** - You'll get this if you try to import an already exported model that's broken, it could also not be broken but zmod decides it doesn't want to import it, in any case avoid importing a model you exported as xbg in zmod already as much as possible

***Model is completely deformed in the viewer*** - go into polygon editing mode and this will fix, don't know why it happens, just does

***Model that was supposed to have physics doesn't have any physics in game*** - Be sure when you import a model to check it's original path and find it's unknown files A through C, not all models will have them and not all models will have all of them but they need to be in your export path when you export

***Materials are not loading in zmod*** - This means that your import settings are off/you are missing the materials in windy_city_unpack. When you import an xbg zmod has two file paths you have to fill out, Root and Mod folder. Root folder is the directory of your windy_city_unpack, (if you don't have this, you have to unpack windy_city_unpack) so for instance, my root folder is *C:\Modding\bin\windy_city_unpack*. The mod folder is the folder in which windy_city_unpack resides it, so for me it would be *C:\Modding\bin*

***Model is completely black in game, but the mesh is still showing*** - This means something has gone wrong in your material, review or redo your material for the object to fix any problems with it.

***Model is not showing up in game properly*** - There's usually a few reasons for this to happen, for most models, on import you will need to hit "Merge Models." There are some exceptions but generally it's good practice to do a test import-export to the game to see if it will work in the first place before you start editing. Objects that are way bigger than the original mesh will also be pretty buggy.

***Model is not appearing in viewport*** - You first have to hit one of the LOD buttons in the top left for the mesh of that respective LOD to appear

***Only one LOD is showing as modded in game*** - Usually, most modders only modify the highest lod, so in game this would be high to ultra. But for all of the LOD's to be modded you have to also mod LOD1 as well as LOD0, you can just copy the LOD1 to LOD0, or make it so LOD1 never appears and LOD0 is instead constant.

***Custom model is not appearing in game/has no rigging in game*** - For custom models, you need to essentially do the same thing twice, so you need to import, copy rigging, and then attach to the retail model. After that, delete all of the retail model's parts in the attached model, so you are just left with your custom model and the retail's model respective skeleton. Then, import the retail model again, and repeat. If you only import the retail model once and rig the custom model once, it will either not show in game, or be unrigged.

***Zmod randomly crashes*** - Yeah. In zmod you pretty much want to avoid doing a fast series of actions, for instance, the fastest way to crash zmod is to hit the undo button too fast. Usually the lower your zmod fps, the more likely it will crash so unless you absolutely need it, it's good practice to hide a model's skeleton because skeletons tank the fps the most

***Materials not appearing in game/Model invisible in-game x2*** - Zmod has this really weird bug where sometimes it will just decide to cut off the material path, so if your model has proper materials in game, and you are absolutely sure it's not the mesh, check the materials tab and make sure it didn't cut "\graphics\materials" into "raphics\materials" It will be the material group name.


VP — 12/12/21, 17:49
find garment xbg 
then find material
= profit
maybe it uses some material override in the lib
slick would know
ping him
Rabbit [EQLZ],  — 12/12/21, 17:50
I'm so new to all of this and I didn't really wanna bother anyone because I felt bad xD like I know people are busy
VP — 12/12/21, 17:50
no it’s okay, he’s sleeping now
Rabbit [EQLZ],  — 12/12/21, 17:52
thank you tho <3 I'm gonna try looking through the files now because I had tried going back and I saw people were talking about zmodeler and... that was a nightmare for me
@the task of the Pesky Fly whenever you have time, would you be able to help me with recolors?
the task of the Pesky Fly — 12/12/21, 21:50
Pain
 [EQLZ], 
the task of the Pesky Fly — 12/12/21, 21:50
Zmodeler doesn't work with WDL
Rabbit [EQLZ],  — 12/12/21, 21:51
oh thank god so I don't have to use that shit
so what would I end up using for the recolors then?
the task of the Pesky Fly — 12/12/21, 22:13
Disrupt Editor which doesn't have a public link from what I know
And obv you need to find the material the clothing you want to edit of
Which alone is a bit painful
Rabbit [EQLZ],  — 12/12/21, 22:14
I've been looking through the filelist for legion and trying to go from there. I have like a rough idea what I was looking for
so would it be impossible to do any of this without the editor?
the task of the Pesky Fly — 12/12/21, 22:18
Correct
I mean I suppose I can link it later
Rabbit [EQLZ],  — 12/12/21, 22:19
tyt. I would greatly appreciate it if you could
qstlijku — 12/12/21, 22:37
Isn’t the link currently down?
and it’s like 300 GB?
Rabbit [EQLZ],  — 12/12/21, 22:38
300gb seems very excessive for my poor laptop
qstlijku — 12/12/21, 22:43
I'm asking if that's the one you were referring to, I have a feeling it is
Troplo — 12/12/21, 22:43
well its in the WDL channel so
qstlijku — 12/12/21, 22:46
I thought that’s a mega link which is down?
Troplo — 12/12/21, 22:49
oh I meant like its gotta be the one he's referring to because we're talking in the WDL channel, but yeah, the MEGA link is probably down
although you can likely find the original torrent magnet link somewhere, but it is 561GB or something
a laptop however probably won't be able to run it properly or at all even if you can sacrifice the disk space
Rabbit [EQLZ],  — 12/12/21, 22:52
well I didn't anticipate that tbh
the task of the Pesky Fly — 12/12/21, 22:54
Not that disrupt editor
Not the official one but fan-made which was made for WD1 originally
It has material converter which works for all three WD games
Rabbit [EQLZ],  — 12/12/21, 22:55
oh so there's hope for me
Troplo — 12/12/21, 23:00
oh
qstlijku — 12/12/21, 23:15
oh, what does it do exactly?
Troplo — 12/13/21, 00:20
Image
the task of the Pesky Fly — 12/13/21, 00:20
It can view the map in WD1, export layers to XML and import (again WD1 only) 
+ has audio (spk) conversion
And material editor and xbg2xml (wd1 only) (and reverse) 
Cobra — 12/13/21, 07:34
nd import (again WD1 only)
i'm pretty sure i used DE for wd2 layers
:happythonk:
Troplo — 12/13/21, 07:35
I think there was some experimental support for WD2 in DE 
never tested it, I think Jon or someone said it
Cobra — 12/13/21, 07:36
i modified it to make it work
back when the source code was available
the task of the Pesky Fly — 12/13/21, 07:37
im talking about the compiled version so
the one i have
not the source code
Cobra — 12/13/21, 07:39
is the one you have not compiled from source code?
lol
the task of the Pesky Fly — 12/13/21, 07:40
it is but i don't own the source code
i only have compiled version
not the source files

TheI3arracuda — 5/31/20, 09:31
silent hill
Deleted User — 5/31/20, 09:32
i'm love this stupid game
I edited only 2 materials and it already crashes sometimes
the task of the Pesky Fly — 5/31/20, 09:34
tools
i doubt its the game's fault but tools (not saying that they're bad though)
as they allow us to do this, but incomplete
iirc you can convert .xml from vanilla unedited material and it will be completely different
which most likely is the problem
Deleted User — 5/31/20, 09:37
hm yes
I converted vanilla material and converted it back to material
something at the end is different
Image
only few bytes are wrong, rest of it is same
If I right same story for zcancer
but for some reason these crashes non consistent which really annoying
みる97 — 5/31/20, 09:43
because the data is converted to float then back to binhex
it's the same
just that some floats aren't as precise
Deleted User — 5/31/20, 09:47
can we somehow fix it? or impossible?

or at least find out consistent solution for all edited materials
like, what exact values converts wrong
Crank Beige — 5/31/20, 10:26
I pointed that out a while ago
but not here obviously
disrupt material editor does not pack the same and using it to edit certain materials will result in crashes, wether on first load or on reload
just throw the whole thing away
Deleted User — 5/31/20, 10:34
zcancer never will help here
since when I found out that ZM can remove some setting on export, I do not trust this dumb thing at all
Crank Beige — 5/31/20, 10:35
you JUST found out about that
yeh it wont export the same
Deleted User — 5/31/20, 10:37
I think most safe option here is edit it with... hex maybe?
I need to edit only diffuse colors and path to the texture
Crank Beige — 5/31/20, 10:37
I tried doing that sometime ago
What you would want to do is this
1- get original material 
Keep it backed up somewhere then export it again, see what bytes change between the two files
Once you figure that out, assign new diffuses to that material, then compare disrupt exported material with edit diffuse material
Then port the diffuse into the original
Complicated and long process but that's how you'd do it

Para — 1/7/26, 12:35
@qstlijku since you're working with DE, could you fix the material converter outputting the wrong file length when converting back to .bin?
If you convert a material from bin to xml and then back to bin the length bytes will be wrong and it will cause random crashes
At that point if you try to convert to xml again it'll even throw an error 
Jordi Dihh — 1/7/26, 12:37
this game is not nice
(っ◔◡◔)っΞpicStreamMan [cтOS],  — 1/7/26, 12:38
hmm guess for now we can manually hex edit to fix 🤔
good to know tho... thank you

Olanov — 9/19/25, 03:04
hm, what textures in particular fall under this critique?
pretty much all textures are somehow vanilla, with incredibly small alterations (and more often relying on material bin changes rather than painting stuff), only the vehicle liveries, CPD detective badge and cop hat shield are fully scratch made

Do you have one for the STG 90? The SG552?
 [SCAV], 
rich off slaves — 4/24/25, 14:46
I didn't do those nah, i really don't care about them tbh
it's really easy thuogh
i'll tell you
use material converter to open the material bins
zzOLD.DONOTUSE [SCAV],  — 4/24/25, 14:47
Ahh okay. I'm all ears
rich off slaves — 4/24/25, 14:49
and replace the name with the original gun, so let's say the sg is an assault rifle right? i'm not sure which watchdog weapon it'd be but let's say its the OCP-11, you change the name to exactly the filename of the OCP-11 in the bin file.
Let's say you'd wanna do the m468, that's clearly the 416 right, so in the material bin it should be written instead of m468, the texture paths should be replaced with assault_hk416 
these are the m468 files slick sent
Image
i changed their names to the 416's
this is the m468 material bin, i also changed it to the 416, which is called assault_hk416
Image
rich off slaves — 4/24/25, 14:52
do you see the file names being mentioned in the xml photo right above?
it's straightforward, i don't have the names for the weapons but i'm sure someone can help with what their filenames are
zzOLD.DONOTUSE [SCAV],  — 4/24/25, 14:55
I'm on my way home right now actually. I'll take a look at the material.bin
For the sg552
rich off slaves — 4/24/25, 14:55
make sure to also change the names on the .xbt files, that's crucial or you'll have reflective black guns
they need to be named something that exists in your patch, the reason they were black and shiny is because Reborn 2 has custom addon weapons which work just fine, but since we don't have his depload or any of his weapon entries it will not work unless you replace it with an already existing gun in the game. 
zzOLD.DONOTUSE [SCAV],  — 4/24/25, 15:17
Alright
In my ocp 11 folder
It is named sg511_?.xbt
So rename it to ocp 11?
Is it ocp11_ or ocp underscore 11 underscore.xbt 
(っ◔◡◔)っΞpicStreamMan [cтOS],  — 4/24/25, 15:18
Thx @rich off slaves nice helping and info. Good stuff 🙏🏼
