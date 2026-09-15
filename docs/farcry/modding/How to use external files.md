**How to use external files**

If your mod has files which must be installed into the game directory
instead of DAT FAT, you can use an *external* function in Mod Installer.

During installation if the target file exists, then it's backuped and a
file from the package is installed. If you uninstall mods, the backup
file is reverted.

# How to make it?

The external function supports Pairs and alternative file structure.
Simply add your mod files into the package archive and edit info.xml.

First let's show the Pairs way.

# Defining external files via Pairs

Definitions for external files are the same as for DAT FAT files. The
only difference is the extra node *External* with boolean (true, false)
values.

Example below shows installing file IGE.WPF.Core64.dll into the game
directory. Inside the package there is info.xml and the dll file.

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfo\>

  \<Games\>

\<Game\>FC5\</Game\>

\</Games\>

  \<Name\>External file example\</Name\>

  \<Description\>This package installs a dll file into game
directory.\</Description\>

  \<Pairs\>

    \<Pair\>

      \<External\>true\</External\>

      \<Source\>IGE.WPF.Core64.dll\</Source\>

      \<Target\>bin\\IGE.WPF.Core64.dll\</Target\>

    \</Pair\>

  \</Pairs\>

\</PackageInfo\>

You can define as many files as you want.

# Defining external files via alternative file structure

External files can be used also in alternative file struct. You can just
add the *External* node to the folder manifest file.

Example:

We have one file IGE.WPF.Core64.dll which we need to install. Next file
will be info.xml and the *myfiles* directory manifest.

MyPackage.a3 / myfiles / bin / IGE.WPF.Core64.dll

MyPackage.a3 / myfiles / myfiles.xml

MyPackage.a3 / info.xml

myfiles.xml

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<Manifest\>

  \<External\>true\</External\>

\</Manifest\>

info.xml

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfo\>

  \<Games\>

\<Game\>FC5\</Game\>

\</Games\>

  \<Name\>External file example\</Name\>

  \<Description\>This package installs a dll file into game
directory.\</Description\>

\</PackageInfo\>

During installation the dll file will be installed into the bin
directory in the game install dir.
