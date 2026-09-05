# WATCH_DOGS 1

Olanov — 7/22/22, 00:00
i was under the impression that if you add peds with new meshes and/or textures, material bins, they would need to be added to the depload or otherwise risk stability; but i've been testing some and i haven't crashed with all my time of testing

is this just a misconception on my part or is it very system dependent so i might be getting different results?
Olanov — 7/22/22, 01:31
and it's not unrelated for sure?
the crashing, it's not caused possibly by something other than a lack of a depload edit
just curious about all that, cos i had something that was causing me crashes before and i thought it was a depload problem, i had added all the depload entries for my new assets but the crash kept happening no matter how many tweaks i did, so i got rid of them and tracked it down to the model itself (i had defined materials incorrectly)

after that i didn't put the depload entries back and suffered no crashes, once i had fixed the model file up 
it kinda left me with a confused understanding of it 😄
i suppose if people report instability i can always quickly throw in the olds edits to the depload i did 😄 
i feel ya on materials though, always often related to it lol
overhauling cops with @FenderBender and @DukeOfFail
i've done cop ped assets, fender uv mapped cars and duke re-textured em 
well, i'm not suffering any crashes atm so all's good baby 🙂
just want to make sure that nobody else does, gotta make them quality mods 😄
yeah that is true
i haven't played through the game with the cop assets so far
just stress tested with multiple shootouts back to back
Olanov — 7/22/22, 01:38
seem to have worked swell
kinda sorta, no new agencies but new ped assets (re-used from game tho)
both
⁠wd1_modding⁠
Image
kinda both 😄
using base assets but did change some stuff about them in zmod
i added them all as stand-alone items tho
not re-using them straight up
except for the cars ofc, they are replaced
there's few occasions where they might use something shared with other peds
like the cop hats (default)
but i wanted to try not to fuck up anything with the other peds by introducing my tweaks
oh yeah, just below the copied link
they get caps, goggles
enforcer loses his sci-fi body armor 😄
plus minor re-textures
indeed
and i mean, it's very good from a gameplay standpoint 
but i'm trying for bit of authenticity with it
Olanov — 7/22/22, 01:45
oh that'd been awesome
tho i understand it is fall 😄
indeed, and slightly out of place for the colder temperature 😄
but it would look cool
indeed
it could swing a couple of ways as far as temperature
but i'm imagining it's a bit colder
<-- ඞ 2011 aiden connoisseur [SDP],  — 7/22/22, 02:27
Now that you guys said that
A light bulb has lightened up in my brain
But I'm at work🥲
Why do I always have genius ideas at work lol
<-- ඞ 2011 aiden connoisseur [SDP],  — 7/22/22, 04:09
I guess first thing I'll I'll is fix that 92fs
<-- ඞ 2011 aiden connoisseur [SDP],  — 7/22/22, 05:01
@Olanovbtw
the eroor in your model that made you crash
did it crash when the model was in render distance?
like if you spawned the car insta crash?
i have a problem like that where i have a gun if the model gets loaded bam crash
Olanov — 7/22/22, 05:02
nah, nothing like that, it was quite random, usually after reloading the game after a cop shootout
<-- ඞ 2011 aiden connoisseur [SDP],  — 7/22/22, 05:02
ah ok
Olanov — 7/22/22, 05:02
but it worked fine in normal gameplay
<-- ඞ 2011 aiden connoisseur [SDP],  — 7/22/22, 05:02
ill fix my materials
hopefully it the cause of the crashes for me
Olanov — 7/22/22, 05:02
definitely look over the materials, they are a source of a lot of headache
<-- ඞ 2011 aiden connoisseur [SDP],  — 7/22/22, 05:03
yeah
the paths
<-- ඞ 2011 aiden connoisseur [SDP],  — 7/22/22, 05:23
lmao my material used 1911 texture ahahah
The Silver — 7/22/22, 08:35
Woah congrats for the releases @Olanov and @FenderBender looks amazing! 😃

0_MidnightWolf — 4/12/26, 01:52
thank you!
GR€ΛÐ [LSPD],  — 4/12/26, 11:54
Excuse my ignorance but what can this be used for?
qstlijku
OP
 — 4/14/26, 20:00
New version, should work for all 3409 compound cbatch files now
Attachment file type: unknown
ConvertBatchFile.exe
4.22 MB
qstlijku
OP
 — 5/6/26, 18:09
Forwarded
Usage: ConvertBatchFile.exe ./cbatch (converts in place)
Usage: ConvertBatchFile.exe ./cbatch ./cbatch_converted (put output in cbatch_converted folder)
Attachment file type: archive
ConvertBatchFile_Release.zip
11.47 MB
Made a release version so you shouldn't have to install any visual studio debug stuff
Also added support for hires cbatch
Oh yeah, round trip conversion (cbatch to XML and XML to cbatch) is also working now
0_MidnightWolf — 5/7/26, 04:03
awesome!
how much time did you spend on that
qstlijku
OP
 — 5/7/26, 21:15
Depends on what part, just this tool then maybe a few hours
but if you count the whole DisruptEditor tooling then at least 6 months
0_MidnightWolf — 5/8/26, 02:31
damn, much respect to you
0_MidnightWolf — 5/8/26, 02:33
I meant how much time did you take to figure out how cbatch files work, if that was difficult or not
It seems very difficult
qstlijku
OP
 — 5/8/26, 02:35
Well the existing DisruptEditor had a lot of it mapped out already, I just cleaned it up and made the converter tool similar to the existing ConvertXBG/ConvertMaterials
I did look at the Wii U version in IDA
as well as the WDL PDB
0_MidnightWolf — 5/8/26, 02:44
Oh so you didn't really start from scratch, good then
I wanted this converter so bad, thank you!
What's different in the Wii U version ?
qstlijku
OP
 — 5/8/26, 06:32
It's the only version that has partial debug symbols
0_MidnightWolf — 5/8/26, 06:32
oh so that helps

# WATCH_DOGS 2

Legi0n — Yesterday at 15:43
Lua file that prints everything that is streamed from game files (mostly helps with wems)

You need Cheat Engine to use it then follow this steps:
Open process -> Memory View -> Ctrl+L -> Paste script -> Execute 
--[[
    Watch Dogs 2 - Streamed File Debug Logger
    Disrupt_64.dll

    Hooks confirmados desde análisis del binario:
      BFFileOpen   RVA 0x4AD3040  → fileId (uint64* en RDX)

WD2_FileLogger-2.lua
6 KB
Jrjman05 [TF2C],  — Yesterday at 16:25
So like if the game is playing the music used in the intro, this script will tell me its filename?
 [TF2C], 
Legi0n — Yesterday at 16:40
Yeah for example you are playing an audio log or a song in the player the Memory View screen in CE will show the files used on live
Then you can copy paste the result in a filelist and you are set

# LEGION

□ΞnCrypTΞD□ — Yesterday at 09:00
Soon as I crack the havok crap it will have a tool. 
Junk — 18:33
Guys, does anyone know how spawn clan Kelley drones with their specific livery in game?
When I spawn drones in NOMAD Scripthook they all have albion livery
□ΞnCrypTΞD□ — 18:40
you would open say the material override script - and there vehicle spawn script find the clan kelley drones guid - but attach the material override you would also need to place the matieral override ids in there material override bank - for you to understand more extract entire legion dat/fat for common and london ignore sounds and sht like that find where the override ids are banked and get the id for drone
Junk — 18:42
Yeah, I found the IDs for the drones I needed and even spawned them. They’re even labeled as Kelly Clan drones, but they all have the Albion color scheme.
Junk — 18:44
For example, I’ve already managed to spawn a chase drone, and it’s labeled as "Kelly's drone," and has Kelly's drone guid, but it has the Albion livery
□ΞnCrypTΞD□ — 18:45
i mean you need to find the materialooverride id
Junk — 18:46
Are you saying I need to change not just the drone's ID, but also its livery ID?
□ΞnCrypTΞD□ — 18:48
correct
Junk — 18:50
Where can I find the game's material override guides? I mean, in which game folders?
You'll most likely have to unpack something from the .dat/.fat files there.
□ΞnCrypTΞD□ — 19:08
did i not give you instructions further up.
