# **Adding custom music and playing it in Far Cry 6**

# **First words**

Since Far Cry 5 Ubisoft added Wwise as a sound engine and doesn’t use an in-house engine. It means it is easier to add custom audio files, because Wwise can be used as a free program.

To download Wwise you just need to make an account, then you can select via their launcher what version of Wwise you want to install.

But \- you can’t just install the latest version, because every main version of Wwise is incompatible with others, so it means if a game uses version 2019, then you can’t use 2022 because it simply will not work.

So, for now we will add new music for Far Cry 6 and FC6 uses version 2019, exactly it is 2019.2.15.7667

# **Let’s start**

First, let’s describe some basic things about Wwise. As I stated, it can be used as a free program, but it’s limited to 200 audio files per project. It’s enough for our tutorial.

Wwise uses two file types \- BNK and WEM. The first one BNK is like config \- it contains defined behavior for the engine, events, states, etc. BNK is basically a file extension for soundbank. The second one WEM is just an audio file, but it can also contain small definitions like cues. If you want to know the exact description of these things, look at Wwise documentation, it is very detailed.

And what exactly will we make? We make a simple script which will play music immediately after the game is loaded, then second music will play once we fire from a weapon. It will play for 10 seconds and then it returns to the first one.

## **Some files from the tutorial**

Useful files for the tutorial, for example if you don’t want to make some part of the tutorial.

Here are two sample music files, they’re from Far Cry 4 from Meh Teh airport mission:

* [Meh Teh Airport.wav](https://fcmodding.com/files/FC6%20Custom%20Music%20Tutorial%20Files/Meh%20Teh%20Airport.wav) 
* [Meh Teh Airport War.wav](https://fcmodding.com/files/FC6%20Custom%20Music%20Tutorial%20Files/Meh%20Teh%20Airport%20War.wav)

Here is whole Wwise project:

* [Wwise project ZIP](https://fcmodding.com/files/FC6%20Custom%20Music%20Tutorial%20Files/Wwise%20Project.zip)

The script in lua:

* [mymusic.mymusic.lua](https://fcmodding.com/files/FC6%20Custom%20Music%20Tutorial%20Files/mymusic.mymusic.lua)

Whole finished and working package:

* [MyMusic.a3](https://fcmodding.com/files/FC6%20Custom%20Music%20Tutorial%20Files/MyMusic.a3)

Workspace source Domino file for Domino Visualizer:

* [mymusic.domino.xml](https://fcmodding.com/files/FC6%20Custom%20Music%20Tutorial%20Files/mymusic.domino.xml)

# **First part \- Wwise project**

## **Downloading and installing Wwise**

If you don’t want to create a new account and install it via launcher, look to fcmodding.com, here is zipped portable version 2019.2.15.7667.

If you go through the launcher, install the most basic Wwise without additional software, because we really don’t need it.

![][image1]

## **Creating new project**

After install, launch the program. It will open this window:

![][image2]

Click on New and type name of new project:

Don’t forget to uncheck all import assets, because we don’t need any.

![][image3]

Click on OK. It will open the new project in this window:

It can look a bit different because I added some views (via menu Views) \- we probably will need just Soundcaster, so add it too.

![][image4]

## **Dunia \<\> Wwise**

Now we prepared Wwise, so now a little info about how Dunia uses the Wwise.

In Dunia there are always two soundbanks (BNK) \- first one has defined all music logic \- playing, events, etc, and the second one for stopping the music.

Ubi named them in this format:

* **Play\_MUS\_MIS0020** 
* **Stop\_MUS\_MIS0020**

MIS0020 is the name of a mission in FC6.

As you can see, Ubi has some kind of order in naming.

So, this means that one mission can have defined one soundbank with many audio files and they are played via states. Ubi added 30 states. In the DuniaSoundData file they are named as **Set\_State\_ST\_MUS\_MIS\_Step\_01**.

This means that one soundbank can have 30 different audio files. Of course it can have more, but for some reason Ubi added just 30, maybe it’s enough. It’s possible to add more states, but more on that another time.

And how does this work?

Easy, in a script, first it is activated by the main soundbank, for example **Play\_MUS\_MIS0020**. But now nothing is played, because no state was activated. So, when a script activates **Set\_State\_ST\_MUS\_MIS\_Step\_01**, music will start playing. And if for example enter an area, the script activates **Set\_State\_ST\_MUS\_MIS\_Step\_02**, so another music will start playing. Easy, isn’t it?

Of course Wwise can define transitions between states, so music is changed for example by changing volume.

And after the mission is done, the script activates **Stop\_MUS\_MIS0020** and this will stop playing any music, so even if you then activate any state, nothing will be played, because no soundbank was activated.

## **Adding states to our project**

This part is important because names must be exact as they are in the game. We now add just two states, because we will use two audio files.

In the **project explorer** click on tab **Game Syncs**. Then in the **States** folder create a new **Work Unit** (right click on States \-\> New child \-\> Work Unit) and set name **State Groups.** Then inside add a new **State Group** and set name **ST\_MUS\_MIS\_Step**. Then inside add two new **States** and set names **ST\_MUS\_MIS\_Step\_01** and **ST\_MUS\_MIS\_Step\_02**.

Final result will look like this.

![][image5]

## **Adding soundbanks to our project**

**NOTE: On all pictures there are soundbank names without the “\_SB” on end, it’s because I forgot about it and didn’t want to recreate all pictures, so please, take care about it.**

Now we need to add two soundbanks \- for play and for stop.

In the **project explorer** click on the **SoundBanks** tab. Inside **SoundBanks** add a new **Work Unit** and set name **My Music**. Then inside add two **SoundBanks** and set names **Play\_MUS\_MyMusic\_SB** and **Stop\_MUS\_MyMusic\_SB**.

Results:

![][image6]

Now a small note \- Wwise by default generates files with names, but Dunia uses just names with IDs, so we need to set this in project settings. So click on Project \-\> Project settings… \-\> SoundBanks and here uncheck **Use SoundBank names.** This is important.

![][image7]

## **Adding events to our project**

Now we add events \- for playing and stopping.

In the **project explorer** click on the **Events** tab. Inside **Events** create a new **Work Unit** with the name **Events**. Then inside create two **Events** with names **Play\_MUS\_MyMusic** and **Stop\_MUS\_MyMusic**.

Now add one more event and name it as **MyMusic\_ResetCombat**.

![][image8]

## **Adding audio to our project**

And now the main part \- audio files. We will need two audio tracks in WAV format. You can download samples from links at the beginning or you can use your own.

In the **project explorer** click on the **Audio** tab.

In the **Master-Mixer Hierarchy,** add to the **Default Work Unit** a new **Audio Bus** and set name **GP\_MUS\_FC6\_OW\_Combat\_Randomizer.**

In **Interactive Music Hierarchy** create a new **Work Unit** with name **Music**. Inside create a new **Music Switch Container** with name **MyMusic\_MusicSwitchContainer**. Then inside create a new **Music Playlist Container** with name **MyMusic\_MusicPlaylistContainer\_1**. Then inside create **Music Segment** with name **MyMusic\_MusicSegment\_1** and finally inside add new **Music Track** with name **MyMusic\_Track\_1**.

Look if you have it correctly:

![][image9]

Now because we want two music tracks, we repeat steps from **Music Playlist Container**, and change 1 with 2, so it will look like this:

![][image10]

The basic setting is done. Now we must define params for each new item and add our audio files.

## **Adding audio files to our project**

Before adding audio files we must change conversion settings for the project. Dunia uses Vorbis.

Select **MyMusic\_MusicSwitchContainer** and go to tab Conversion. Here select Factory Conversion Settings \-\> Vorbis \-\> Vorbis Quality High.

![][image11]

Now let’s add audio files. Select our **MyMusic\_Track\_1** and via right click select **Import Audio Files…**

Select music in WAV format.

![][image12]

And click **Import**

Now repeat this but for **MyMusic\_Track\_2** and select the second music in WAV format.

![][image13]

If you got all things correctly, both items are now blue.

![][image14]

## **Setting up hierarchy in our project**

Now we must set up the whole config. We start at **MyMusic\_MusicSwitchContainer.**

In the menu select Layouts \-\> Interactive Music. Now we see what we need.

We add state group **ST\_MUS\_MIS\_Step** to the switch container: State Groups \-\> State Groups \-\> ST\_MUS\_MIS\_Step

![][image15]

![][image16]

Now we assign each state to the correct music track, so now basically define which state plays what music.

Drag drop both states to the place below and select for each music container, so:

* ST\_MUS\_MIS\_Step\_01 \- MyMusic\_MusicPlaylistContainer\_1 
* ST\_MUS\_MIS\_Step\_02 \- MyMusic\_MusicPlaylistContainer\_2

![][image17]

That’s all here. Move to the next item \- **MyMusic\_MusicPlaylistContainer\_1.** Here we define what track will be played.

![][image18]

Drag and drop **MyMusic\_MusicSegment\_1** to the window and set **Loop Count** to **Infinite** because we want to play the music indefinitely ;)

![][image19]

Now repeat this for second container

![][image20]

That’s all for containers. Move to the next item \- **MyMusic\_MusicSegment\_1**

Here we define the start and end position for the audio file. Move the red **Exit Cue** to the end of track:

![][image21]

Now click on the **Events** tab and drag drop **MyMusic\_ResetCombat** to the track and move it to begin. This means that when the audio will start playing, the engine will volume down battle music in the game.

![][image22]

Now we repeat this for **MyMusic\_MusicSegment\_2**

![][image23]

That’s all for segments.

Now for **MyMusic\_Track\_1** and **MyMusic\_Track\_2** we must set option Stream which makes the audio files to be outside soundbank, it’s because the whole big music file isn’t held in RAM but is streamed from file.

![][image24]

Now this is optional \- setting volume of tracks. Every single audio can have defined volume.

![][image25]

Now we set transitions between both tracks. Select **MyMusic\_MusicSwitchContainer** and then tab **Transitions**.

![][image26]

Click on **Add Transition**. As **Source** select our **MyMusic\_MusicPlaylistContainer\_1** and as **Destination** select **MyMusic\_MusicPlaylistContainer\_2**

![][image27]

Now select **Exit source at** to **Immediate** and check **Fade-out**

![][image28]

And click on **Edit…** 

In the window set **Time** to any value and close the window

![][image29]

Now we add a new transition and repeat this for the new second transition, don’t forget also the fade out time.

![][image30]

Now we set up events, so go to tab **Events** and select **Play\_MUS\_MyMusic**

Then right click in the window on right and select New Action \-\> Play. On the new item right click on the Target and select Browse and select our **MyMusic\_MusicSwitchContainer**

![][image31]

Now repeat this for **Stop\_MUS\_MyMusic** but as an action select **Stop** (**not Stop all\!**)

![][image32]

Done

![][image33]

Now set up the event **MyMusic\_ResetCombat,** as action select Bus Volume \-\> Reset Bus Volume

As target select our **GP\_MUS\_FC6\_OW\_Combat\_Randomizer** and don’t forget also set **Scope** to **Global**

![][image34]

Events are done. Now set up soundbanks.

In the menu select Layouts \-\> SoundBank and select tab SoundBanks.

In the **SoundBank Manager** check both our soundbanks **Play\_MUS\_MyMusic\_SB** and **Stop\_MUS\_MyMusic\_SB**

![][image35]

Now select **Play\_MUS\_MyMusic\_SB** and for now **SoundBank Editor** is empty

![][image36]

Click on tab **Events** and drag drop events **MyMusic\_ResetCombat** and **Play\_MUS\_MyMusic** to the **SoundBank Editor**. Then go to the Audio tab and drag all these items:

![][image37]

And drop them to the **SoundBank Editor**

![][image38]

Now select **Stop\_MUS\_MyMusic\_SB** and from Events tab drag drop **Stop\_MUS\_MyMusic**

![][image39]

That’s all from setting up config. Last thing is testing.

## **Testing our Wwise project**

If you done it all correctly, then it will work as expected. For testing we need a Soundcaster tool.

In the menu select Layouts \-\> Interactive Music.

In the Soundcaster window select \>\> and click new.

![][image40]

Then click on Show all on States window

![][image41]

Then from **Events** tab drag drop both our events to the Soundcaster window

![][image42]

Now select state **ST\_MUS\_MIS\_Step\_01** and click on the play button of **Play\_MUS\_MyMusic** event. You will hear our first music.

![][image43]

Now try to change the state to the second one **ST\_MUS\_MIS\_Step\_02.** You will hear that music will volume down and second music will start playing.

![][image44]

To stop playing click on the play button on the **Stop\_MUS\_MyMusic** event.

![][image45]

Testing is done.

## **Export our project to BNK files**

On SoundBanks tab right click on SoundBanks and select

![][image46]

Wwise will convert audio files and generate BNK files.

Output files will be in project folder in MyMusic\\GeneratedSoundBanks\\Windows

![][image47]

The txt files contains info about exported files and their IDs

![][image48]

Those IDs are actually some hashes of the names. More in Wwise docs.

We will need those files:

* 117581209.bnk \- our stop BNK 
* 691342655.bnk \- our play BNK 
* 474404542.wem \- our first music 
* 831668348.wem \- our second music

# **Second part \- making package**

You can use the template from the links at the beginning. Here we must make info\_replace.xml.

## **Adding info.xml**

We need to pack our files and add basic info about our package.

\<?xml version\="1.0" encoding\="utf-8" standalone\="yes"?\> 
\<PackageInfo\> 
 \<Games\> 
 \<Game\>FC6\</Game\> 
 \</Games\> 
 \<Name\>My Music\</Name\> 
 \<Description\> 
 \</Description\> 
 \<Pairs\> 
 \<Pair\> 
 \<Source\>fc6\_mymusic.fc6\_mymusic.lua\</Source\> 
 \<Target\>domino\\user\\fcmodding\\fc6\_mymusic.fc6\_mymusic.lua\</Target\> 
 \</Pair\> 
 \<Pair\> 
 \<Source\>117581209.bnk\</Source\> 
 \<Target\>soundbinary\\117581209.bnk\</Target\> 
 \</Pair\> 
 \<Pair\> 
 \<Source\>691342655.bnk\</Source\> 
 \<Target\>soundbinary\\691342655.bnk\</Target\> 
 \</Pair\> 
 \<Pair\> 
 \<Source\>474404542.wem\</Source\> 
 \<Target\>soundbinary\\474404542.wem\</Target\> 
 \</Pair\> 
 \<Pair\> 
 \<Source\>831668348.wem\</Source\> 
 \<Target\>soundbinary\\831668348.wem\</Target\> 
 \</Pair\> 
 \</Pairs\> 
\</PackageInfo\>

File **fc6\_mymusic.fc6\_mymusic.lua** is a script file for playing the music, we make it in the third part.

## **Adding info\_replace.xml**

Due to the script launching there are more entries, but I show here just the entries about music. You can just look here for learning but you can take the whole info\_replace from links at the beginning.

First, we need to binary replace an ID of master audio bus, because there isn’t its name in FC unpacked files and it’s impossible to get source from hash, so FC6 uses as missions music master audio bus with ID 2989583526\.

We look at what ID we have in our project \- open 1355168291.txt which is Init soundbank.

![][image49]

And its ID is 3803692087\.

Now we make replacer:

\<Replace RequiredFile\="soundbinary\\691342655.bnk"\> 
 \<Replace find\="3803692087" replace\="2989583526" type\="UInt32" /\> 
\</Replace\>

Next is adding info about Wwise files to Dunia sound config called soundinfo.bin

We need 691342655.txt which is our play soundbank.

Here is replacer, please look carefully:

![][image50]

Green is the ID of our play soundbank.

Yellow is the ID of our stop soundbank.

Notice that play soundbank must have defined its streamed audio files \- WEM files.

# **Third part \- script for playing our music**

This part has two ways \- first is via Domino Visualizer (not released yet) and second is writing the script.

I make a guide only for the first way because the second way is too hard to explain every step. In the links at the beginning is written script, so you can take a look.

## **Creating new project in Domino Visualizer**

Create new workspace with these settings, select Far Cry 6:

![][image51]

Now add input connector with name In:

![][image52]

Now add a new box **pawnweaponfiredlistener\_v2** and add also its output connector to output **OnFired.** Also add exec box in In connector to the new box

![][image53]

Now add a new box **soundmusictrigger** and add its output **Started**. Also add exec box from In connector with these params:

![][image54]

Notice the StartEvent and StopEvent which are our soundbank IDs.

Now add new box **postwwiseeventsetstate**, add exec box from the connector **Started** and set these params:

![][image55]

The ID 4013606668 is FC6 ID of soundbank with the state **ST\_MUS\_MIS\_Step\_01** 

So this box will start playing our first music.

Now add the next behavior \- after the player fires from a weapon, it will start our second music and after X seconds it will start again the first music.

We add **onceonly\_v3** box because the firing listener catches every fire, so we want to run it once.

![][image56]

Now add new two boxes \- **delay\_v5** and **postwwiseeventsetstate** and connect them like this:

![][image57]

This will play our second music \- notice the ID 2102059229 which FC6 ID for **ST\_MUS\_MIS\_Step\_02**

Now, on the **TimeElapsed** connector add two exec boxes \- to the **onceonly** box to reset it so it will allow next throughput and second exec box to the first created **postwwiseeventsetstate** to play our first music:

![][image58]

Now we need to register resources \- soundbanks to be loaded. It means add all four soundbanks IDs to **Resources** box:

![][image59]

Now save it and export it. Copy file mymusic.mymusic.lua to our package and also don’t forget add also depload entries, otherwise the game can crash or the script simply will not run.

# **Conclusion**

This is all from this tutorial. I hope you at least tried something to make. I also hope it increased your knowledge about the Dunia engine and also inspired you to make new features to the Far Cry 6 game.

Created by ArmanIII



























































