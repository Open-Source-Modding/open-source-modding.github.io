**How to edit a package from modpacks (Libertad, Resistance)**

Packages in modpacks (main mods - Libertad, Resistance, ...) are
tweaked, it means they have special entries in info files and they are
checked by MD5 hash. It's due to a separate folder / file and also due
to updating. So it's not possible to edit them.

But of course it doesn't mean you can't do anything, there is a way.

This doc will cover FC6 Libertad mod, but it's of course the same for
all others.

# Automated process

You can use the built-in function in Mod Installer for extracting a
package from modpack. Simply find the desired package in the list, hold
Alt or Alt Gr key and click on the package in the list. Mod Installer
will ask you about extracting the package and if it already exists, it
will ask about overwriting.

# Manual way - copying a package

First you need to copy a package to the **ModifiedFilesFC6** folder.
Currently there are two things how modpacks are created - first is the
now deprecated separate ModifiedFiles folder and second is new single
mod BIN file. The new way is due to low skilled users who sometimes put
packages into the wrong location.

The second one is relatively easy - simply copy a package and paste it
to the **ModifiedFilesFC6** folder.

The first one needs a bit more work. First go to the FCModding website
and download Mod Installer CMD. Unzip it to the same folder as the main
Mod Installer. Now drag drop the **\_FC6Libertad.bin** on the
**ModInstallerCMD.exe**, it will unpack the file to the new folder
**\_FC6Libertad**. Now inside the folder there are the packages, so
select the one you need and copy it to the **ModifiedFilesFC6** folder.

![](media/image2.png){width="3.6979166666666665in"
height="4.604166666666667in"}

# 

## Edit the package to be used out of modpack

Now we need to edit info and remove the **ParentDir** node inside of it.
The **ParentDir** node is always in the info where Description and Name
nodes are.

![](media/image1.png){width="4.697916666666667in" height="0.90625in"}

Find it and remove the whole line.

Now the package is prepared for future editing.
