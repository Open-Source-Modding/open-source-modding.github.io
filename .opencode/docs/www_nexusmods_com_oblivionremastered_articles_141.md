# Animating for Oblivion Remastered at Oblivion Remastered Nexus - Mods and community

> Source: https://www.nexusmods.com/oblivionremastered/articles/141
> Cached: 2026-08-18T19:27:56.413Z

---

> Prerequisites:

- ~ 200gbs of free space
- Blender [4.2 LTS](https://www.blender.org/download/releases/4-2/)﻿
- Unreal Engine 5.3.2 source code ([github](https://github.com/epicgames/unrealengine)﻿)
- Visual Studio [2022](https://visualstudio.microsoft.com/downloads/)﻿ (C++ workload)
- Oblivion Remastered [sample project](https://www.nexusmods.com/oblivionremastered/mods/880)
- [FModel](https://fmodel.app/)

BEWARE: Working with animations in Unreal Engine happens within Unreal, because of this we need to download and setup a working project.
This takes considerable hard drive space and 1-5 hours depending on your hardware.

> Configuring Visual Studio:

In visual studio make sure you have installed the **'Game development with C++'** workload
We'll need this to compile our Unreal Engine build later.

  ﻿

> Download [Unreal Engine 5.3.2 Source](https://github.com/EpicGames/UnrealEngine/releases/tag/5.3.2-release) from github

If the link won't open, note that you will need to join the Epic Games organization on github to download.
Here's an [article from Epic Games](https://dev.epicgames.com/documentation/en-us/unreal-engine/downloading-source-code-in-unreal-engine#accessingunrealenginesourcecodeongithub) on downloading source versions.

We need the source build to setup our project, release versions on Epic's launcher will not work for our purposes.

  ﻿﻿

UPDATE:

There's a new community maintained fork available [here](https://github.com/C0bra5/UnrealEngine)﻿
You will still need a github account linked to Epic Games to download.
This version should work the same or better than the [5.3.2 release](https://github.com/EpicGames/UnrealEngine/releases/tag/5.3.2-release)﻿ from Epic but I haven't tested.

> Compiling Unreal Engine

- Extract downloaded zip wherever you want
- Run Setup.bat
- Wait for it to finish
- Run GenerateProjectFiles.bat
- Wait for it to finish

  

> A file called 'UE5.sln' should have been created if successful
This is our Visual Studio 'solution' file, it's what we'll use to compile the engine within Visual studio.

  

> 
- Double click UE5.sln to open it, if it asks you which program to open with select 'Visual Studio 2022'
- Once Visual Studio opens, find the Solution Explorer (Usually it's in the top right corner)
- Expand 'Engine' folder
- Make sure the UE5 project file is highlighted (left click once)

  

> 
- Navigate to the Build tab and find 'Build UE5'
- Left click it to begin compiling our engine

**
WARNING:** This will take **30m-5hrs** depending on your hardware, make sure you don't have to use your PC for heavy tasks during the process.
You can expect general slowdowns during compilation.

  

> If your build succeeds:

- In explorer, navigate to:

```
﻿{PathToYourEngine}&#92;Engine&#92;Binaries&#92;Win64
```

- Run 'UnrealEditor.exe'

On first load, it will take a few minutes to compile all shaders.

Once you reach the project browser, you can close Unreal Engine. We only needed to run the engine at least once.

  ﻿

> Download the Oblivion Remastered [Sample Project](https://www.nexusmods.com/oblivionremastered/mods/880)﻿

- Extract wherever you want
- Right click OblivionRemastered.uproject
- Switch Unreal Engine Version
- Navigate to the engine that we compiled from source

Note: on Windows 11 you may need to select 'Show more options' to see this legacy context menu.

  

  

> Opening Our Sample Project

- Double click OblivionRemastered.uproject
- Wait for it to generate project files, this will take a few minutes

The project should start, note that it may need to compile shaders again first

If successful you should see something like this:

  

> Setting up Animation Workspace

Create the following directory in the content browser inside Unreal Engine:
﻿```
Content﻿&#92;Art&#92;Character&#92;Humanoid
```

  ﻿

- Click and drag the included "SK_HumanoidFull.fbx" into the created directory within the content browser

  

it should look like this upon import.
(Ignore missing smoothing group errors if you get them)
  ﻿

> Close Unreal Engine

In explorer, navigate to your uproject and open the humanoid directory
{PathToYourProject&#92;Content&#92;Art&#92;Humanoid

Copy paste the included resource 'SKEL_HumanoidSkeleton.uasset" into your project

  ﻿

> Open the OblivionRemastered.uproject again

Open SKEL_HumanoidSkeleton
Make sure these 'Virtual Bones' are present, they're the IK leg targets in the game. Without them your feet will be stuck in their A pose in game.

  

> 
- Right click our imported Skeletal Mesh and click 'Assign Skeleton'
- Select 'SKEL_HumanoidSkeleton'
- Make sure it is the file without the underscore between Humanoid and Skeleton!
- Keep all other settings default and hit Accept
- Delete the old skeleton that imported with our FBX

      ﻿

> IMPORTANT!
Rename SKEL_HumanoidSkeleton to something unique, I chose 'Mike_HumanoidSkeleton'

  

> Congrats, you're (finally) ready to begin animating!﻿

I highly suggest you review **[this document](https://www.nexusmods.com/oblivionremastered/articles/152)﻿** to learn about how to use the rig packaged with this mod to author animations

This guide assumes you know the basics of animating so I won't go over that, and we can skip to the step where
you have already authored an animation that is ready to be imported.

> Replacing animations

Use [FModel](https://fmodel.app/)﻿ to check the file path to the animations you want to replace.
Click and drag your animation FBX into the content browser in the correct destination
Folder structure and names **must be the same as the game's.**

Eg: I want to replace the 3rd person One Handed sprint, so my fbx should be called "A_Humanoid_OneHand_Sprint.fbx"
﻿I should also place the file into the correct directory. In my case it's Content&#92;Art&#92;Animation&#92;Humanoid&#92;ThirdPerson&#92;OneHanded&#92;Locomotion&#92;Normal

For import settings just assign the skeleton you created, you can keep all other settings the same

  
  
  ﻿﻿

> Configuring Packaging Settings

To package your mod, you will need to change a few settings in the project first.

In **Project Settings** you want to make sure you match the following settings:

- Use Pak File = True
- Use Io Store = True
- Generate Chunk = True
- Clear "Additional Asset Directories to Cook"
- Clear "List of maps to Include in a packaged build"
- Build Configuration = Shipping

﻿In **Editor Preferences** make sure:

- Allow ChunkID Assignments = True

> Packaging Mod Content

There's multiple ways to package content depending on your needs and how much modularity you wish to give the end user.
Although I cover two "methods" here, you can mix and match them as you see fit.

For example, say I wanted to create a mod that replaces the sprinting, idle and attack animations.
I want to package all the sprinting animations together while keeping the idle and attack animations separate. I also want to specifically make the idle optional.

I would structure my mod like this:
﻿Custom Skeleton - Assign Chunk ID to asset directly (Method 1)
﻿Sprinting animations - Asset Label to group animation sequences together (Method 2)
﻿Attack Animations - Asset label (Method 2)
﻿Idle Animation - Assign Chunk ID to asset directly (Method 1)

That would look something like this once renamed and structured in a mods folder:
﻿MyMod_Resources_P.pak (skeleton resource, tell the user that this is required!)
﻿MyMod_SprintingAnimations_P.pak (combined sprinting assets)
﻿MyMod_AttackAnimations_P.pak (attack anims)
﻿MyMod_IdleAnimation_P.pak (standalone idle replacer)

My end user could then pick and choose what to keep/remove from the provided pak files.

Method 1: Packaging multiple files in one .pak
___________________________________________________________________________

﻿This approach is convenient for being able to group multiple assets together, if you need multiple loose files use method 2 instead, or consider 
﻿creating multiple smaller labels.

- Create a new data asset
- For the class, select "Primary Asset Label"﻿
- Name your asset label something you'll remember (I chose 86_Sprintmod, but naming convention is arbitrary)

  

- Open your asset label
- Add all the assets you want to package in your mod (Pack your custom skeleton as well unless you're going to assign it in a separate chunk)
- Assign your asset label a chunk ID (this must be unique within your project, different pak files/assets can't share a chunk ID)
- Set Cook Rule to "Always Cook"﻿
- Give your label a **unique **Chunk ID in your project (I chose 86)
- Set "Priority" to 1

  

Method 2: Packaging individual assets in one .pak
___________________________________________________________________________

﻿This approach is good for resources or assets that you want to package separately. The downside is that it could quickly bloat your mod folder with ﻿multiple ﻿.pak files if not used conservatively.

- Right click the asset you want to package individually
- Under Asset Actions, click 'Assign to Chunk'
- Give it a **unique **chunk ID within your project (I chose 90)

  

When you are ready to ship your mod:

- Navigate to Platforms > Windows
- ﻿Ensure Binary Configuration = "Shipping" or "Use Project Setting (Shipping)"
- Click 'Package Project'
- Select any place to put your files
- Wait a few minutes for the project to package. This will take a long time the first time as it packages all the code and binaries for a standalone ﻿﻿windows game.

  ﻿

Upon success:

Open your the directory you selected. For me it was inside my uproject directory (it will be a folder called "Windows")
Navigate to {Selected directory} > Windows > OblivionRemastered > Content > Paks

Here you will find your pakchunk0 (your game's content goes here by default, so this will always exist)
You will also find the pak chunks you created using Method 1 & 2
For me, it created **pakchunk86 **(my sprint animation sequences) and **pakchunk90 **(my custom skeleton resource)
  

You can rename these paks to anything you want but it **MUST **have a _P suffix

Copy these pak files into your game's pak folder in {SteamDirectory} > OblivionRemastered > OblivionRemastered > Content > Paks
I decided to create a folder structure like this
```
{Paks}&#92;~mods&#92;AltSprint
```

  

﻿
Like this you can compress the ~mods folder content into a zip file, ready to upload. (Note that the ~mods folder is not necessary for loading pak files)
  

Congrats! Your mod is finally ready to be distributed﻿!