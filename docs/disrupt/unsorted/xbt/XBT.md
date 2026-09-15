# Important

`http://svn.gib.me/public/dunia2/` - this has the tool to edit the XBT header

GR€ΛÐ [LSPD],  — 3/24/26, 17:29
Needs some contrasts adjustments, or maybe I'm not using the right DDS format.
But at least I'm getting there!!
Image
Image
Image
Image
kinda cool how vanilla it will look
qstlijku — 3/24/26, 17:36
Current progress
Image
 [LSPD], 
GR€ΛÐ [LSPD],  — 3/24/26, 18:16
Got it.
Looks compressed (because I had to push up the exposure and lower the contrasts) but almost vanilla. If anybody knows why it looks so bad without some heavy editing (see linked message) please let me know
Image
Image
Image
Image
the task of the Pesky Fly — 3/24/26, 18:21
Maybe wrong xbt header
If it's brighter outside the game

Manu — 12/14/24, 19:06
I didn't change the skin color, I just added tattoos on top of the original texture
but when you enter the game the skin turns white, I don't know the reason
Image
Deleted User — 12/14/24, 22:06
what tool are you using to cook image back to .xbt
⃟ [୧‧₊],  — 12/14/24, 22:59
Hello
Manu — 12/14/24, 23:00
@copy /B XBT.dummy + %1 %~n1.xbt
DDS2XBT.bat
1 KB
Attachment file type: unknown
XBT.dummy
44 bytes
Deleted User — 12/14/24, 23:02
ah, of course, someone (admin cough, cough) didn't delete this stupid tool from resources... 
use this
Attachment file type: unknown
hV_WD1ModdingKit.exe
173.50 KB
drag xbt to exe, you will get dds + its header, then edit dds and drag it back to exe
Yorpie — 12/14/24, 23:05
Oh damn this is gonna be helpful
Had the same as Manu, had to make textures very dark for em to show up normally ingame
So thanks for posting that
Deleted User — 12/14/24, 23:07
I had the very same issue when I just started modding years ago
⁠wd1_modding⁠



# Header might control brightness

F_Aria — 8/15/25, 04:02
is there a tutorial on how to edit the xbt header to change the address which lead to high texture , like change it to something else , I did use hex edit but didn't get result last time I remember

Deleted User — 12/14/24, 23:07
I had the very same issue when I just started modding years ago
⁠wd1_modding⁠
Manu — 12/14/24, 23:09
thank you!!
Deleted User — 12/14/24, 23:09
only time later I learnt that every xbt consists of 2 parts: xbt header and dds
xbt header tells the game how to utilize the texture, it is important to keep same header, otherwise stuff like this happens
I wanted cobra to completely update resources page, so old tools like these will be gone forever, but it didn't go as I planned...
Yorpie — 12/14/24, 23:12
Yeah, that tool is what popped up when looking for something to convert xtb with so having it get removed would defenitly prevent people from coming across this issue

Anyone experienced an oddity like this? proviblue_pd_epic_skin_mod.mp4
Para — 7/16/22, 00:48
Looks like you forgot to replace the _high texture
Olanov — 7/16/22, 00:51
are the ped _high textures used?
don't mean to hijack but they always had me curious
i never got the game to load them on refresh with the toolkit
FenderBender — 7/16/22, 00:52
I see.... I find it odd, since I didn't replace on any other car like the Police_02-04 and the SWAT one and on those it doesn't happen... I guess police_01 is built differently 
Thanks for the tip though, I look into it 🙂
The Silver — 7/16/22, 00:57
the _High textures are used when setting Ultra textures quality and you get up close. The not _High .xbt files have a reference to the filepath of their respective _High version in the xbt header, assuming they do have a corresponding _High version. 
Para — 7/16/22, 00:58
_high textures are basically the lowest mip level (highest resolution texture for viewing up close)
The "correct" way to replace textures is to export _high without mipmaps and halve the non-high's resolution & export it with mipmaps
The Silver — 7/16/22, 01:00
Yeah exactly, this is important FenderBender ^
(sometimes other games simply cap rendering the highest mipmap levels when lowering texture quality, WD instead has the highest mipmap as a separate file altogether, still wondering if stupid or genius...)

Lou Garou — 3/4/20, 14:44
I don't know how to edit the UI, so I used ShevNR's mod, that deletes the reticle of the profiler, then I added a similar one, that was in the 14min demo with reshade, and when you activate profiler it comes up, when you close it it goes away. And I tested it on 2 different res display and its always in the middle of the screen.
Deleted User — 3/4/20, 23:00
Mostly all icons located in ui folder
In windy_city, common and dlc_solo
Better to use dlc one
Some elements has own color that defined in feu
For example objective icon has yellow color and some mods with E3 UI has blue objective icon on minimap and green on screen
Because yellow color layered on blue
Deleted User — 3/4/20, 23:08
Also this true for other elements - profiler lines
They are same that used for subtitles
bg8x8.xbt if I right
And better for editing .xbt use Gibbed Tools Dunia `git@github.com:gibbed/Gibbed.Dunia.git` that The Silver `http://svn.gib.me/public/dunia2/` gave some time ago, because as I understand xbt header has some parameters inside, so it important to keep that header

Soma1one — 2/15/18, 07:54
Attachment file type: unknown
skybox_02b_d.xbt
4.00 MB
Attachment file type: unknown
skybox_03a_d_high.xbt
4.00 MB
it's the same texture for both files
no
nvm it's not the same
Nick — 2/15/18, 07:55
how can i convert them?
Soma1one — 2/15/18, 07:56
Gibbed Tools Dunia 2

# XBT has a header that controls which textures are loaded
