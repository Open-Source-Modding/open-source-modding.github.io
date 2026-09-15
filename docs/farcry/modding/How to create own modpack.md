**How to create own modpack**

As you noticed, mods like Resistance or Libertad have their own single
file which contains all packages. This file is called modpack.

The modpack file is not any known file format like packages, but rather
it's custom Mod Installer format for fast loading of packages.

Unpacking of those files is covered in the tutorial "How to edit a
package from modpacks (Libertad, Resistance)".

Now we look at how to create those modpack files.

# Tools

Since this modpack file editing is not targeted for basic users, the
packing is supported only in CMD version of MI, so download the Mod
Installer CMD.

# Preparing the modpack

Now we must create a folder which will contain all packages which will
be added to the modpack.

Create new folder in the same location as ModifiedFilesFC folders:

As you can see, my modpack will be called "MyModPack".

Now put inside the new folder all packages which you want to add to the
modpack.

We also need to define modpack's display name which will appear when you
use drag drop adding to MI. So inside the folder create a file
"\_name.txt" and put a modpack's name inside the file. Please keep it as
short as possible and single line.

# Packing

Now we pack the folder to a bin file.

Run the Mod Installer CMD with parameter "-devstuff".

It will show this:

Now press a key with a number according to a game for which the modpack
will be created and it is targeted.

It will show another options:

We're interested in the last one (5) which is for creating modpack bin
files.

It will ask you about the folder / folders which to process, use keys Y
or N for Yes or No.

After process is done, it will show this:

Now your modpack bin file is in the base ModifiedFilesFC folder.

# Note

The name of modpack must be without spaces and must start with specific
text: "\_FC6\_" if it's for Far Cry 6, or "\_FCND\_" if it's for Far Cry
New Dawn, etc...
