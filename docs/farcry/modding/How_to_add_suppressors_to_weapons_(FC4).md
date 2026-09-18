## Pre-requisites

* You are familiar with creating .a3 packages 
* Know info\_replace and using addNode 
* Extracting files with and converting with FCBConverter. 
* Use HxD 
* Familiar with file extensions (.fat, .fcb, converted.xml etc) 
* Don’t give up\!

## Weapons:

Since FC3 and FC4 use very similar file structure/hashes and being a copypaste of 3, you can do almost everything that is possible to do in FC3. Here, I added the suppressor to the Buzzsaw\!

Let’s jump straight to it:

You need 3-4 files (if you’re lucky, only 2). That being the following: Archetype, Menu and Weapon itself. To easily identify the ark.fcbs you need, in FC4, convert the entitylibrary.fcb file. Located in **patch/generated/worlds/fcc\_main/**. Highly recommended you save this file somewhere you can easily find it\!

Archetypes are named like this: “**weapons:FC3/\<weapon\_name\>**” (some weapons may have two, where the second is the menu\!)

Menu are named “**weapons:FC3/\<weapon\_name\>.menu**”

For the usable weapons, you have “**weaponProperties:FC3/\<weapon\_name\>/NPC/Multi**” 

Archetype is the base template (skins, attachment positions on the weapon). From the previous game, it seems the designers wanted the weapons to be physical models in the menu, rather than boring 2D icons. Cool feature, let’s you see actual changes to paintjob/attachments in real time.

## Archetype file:

In the archetype file, you want to locate the following:

“**hidDescriptor**”

This section dictates the meshes/models/joints the weapon can attach its objects and player customizable attachments on. A catch: You can see there’s no BinHex value assigned to the fields. They are all in plain text. More on that later. 
Further down the file, you should see **skeleton\_reference\_list**

![][image1]

Notice the fields I have drawn a circle around. This is from the PKM. Surprisingly, it has a bone to attach a suppressor on it. ”**FX\_Fire**” could be the muzzle flash. Note the placement of the bone “**ATTACH\_SILENCER**”

If you have found the Buzzsaw, its Archetype file does not have this line. But, we’ll make it get one.

## WeaponProperties file:

In the weaponproperties file of the same weapon, scroll down to attachments. Most of them have a silencer entry. Unfortunately, machine guns don’t. As I mentioned earlier, we can use **addNode=”1”** in our info\_replace to tell MI to add in a silencer attachment. Be aware that not all weapons with silencers can work. I used the “silencersmg” from the Vector. Ubisoft somehow tied shooting sounds to the RPM/fire mode of the weapon. It’s been like that since FC2. 
![][image2]

## Editing the skeleton\_reference\_list.

MI looks after hashes to replace. This section doesn’t have one. How can we make it work? You need HxD.

In your editor of your choice, copy the entire “**hidDescriptor**” section only. Rename the file like this: 
![][image3] 
![][image4]

It will look like this in the editor. We copy-paste the PKM’s suppressor coordinates and place it after the bullet bone, usually the bottom of this section.

![][image5]

You might wonder how to find the coordinates for the suppressor to attach. Since the suppressor attaches itself at the end of the barrel and it is also where the muzzle FX (smoke and flames) spawn from, it makes sense to have coordinates be very close.

Let’s assume it’s X, Y, Z. For the PKM, the Y-value is about **0.06** from the FX\_FIRE, while Z is identical. FX\_FIRE will always be different from weapon to weapon. If you want 100% accuracy, subtract the Y-values. The PKM value in this case is **0.058783** difference on the Y-axis. Very close to the **0.06** I guessed on. 
![][image6]

## Time to HxD\!

We must convert the file to get the hashes to BinHex so the Mod Installer knows what to do.

First open FCBConverter and convert the file. Remember to select “converted file” when the Windows Explorer window pops up\!

You should now have a rml file

![][image7]

Open up in HxD. In HxD, ensure you only see the bytes in hexadecimal and that the text is not displayed.

![][image8]

If it looks like this, then it’s all good\!

In your .xml editor of choice, make an info\_replace and paste all the hash values in. You can wrap the content for readability. 
![][image9]

## You are done\!

Now, you pack the files as .a3. Patch the game and enjoy\!

## Why is this not necessary all the time?

Sometimes, the weapon archetype file lists suppressor bone. In that case, editing weaponProperties is enough. Remember to edit the menu file so it shows up in the menu\!

## I cannot take off the suppressor?

Doing this requires the shopfile to be modded.

## Suppressor stuck under the trigger?

The weapon model lacks the bone to attach it. A workaround: Make a new muzzle/silencer item and attach to FX\_FIRE. Then repeat the steps here to make new hidDescriptor.rml. You might need to make "ATTACH\_SILENCER" a parent to FX\_FIRE in the skeleton.









