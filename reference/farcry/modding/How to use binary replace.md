**How to use binary replace**

This is new type of the replace package. It can be used for replacing
binary values in files.

# Structure of info_binary.xml

This type of package also has new info.xml.

This is info_binary.xml of „Disable Low FOV" package:

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfoBinary\>

  \<Games\>

\<Game\>FC5\</Game\>

\</Games\>

  \<DefaultInclude\>false\</DefaultInclude\>

  \<Name\>Disable Low FOV\</Name\>

  \<Description\>Disables low FOV during actions like climbing, using the wingsuit, etc\...

  

  \[color=red\]NOTE: This package changes some data in the game DLL file. If you\'ll experience serious bugs or crashes, disable this package.\[/color\]

  

  \[color=red\]\[b\]NOTE: Tested only on game version 1.014\[/b\]\[/color\]\</Description\>

  \<Category\>2\</Category\>

  \<BinaryReplaces\>

    \<BinaryReplace\>

\<Files\>

\<File\>bin\\FC_m64.dll\</File\>

\<File MustExist=\"false\"\>bin_plus\\FC_m64.dll\</File\>

\</Files\>

      \<FindBytes\>C384D2741D\</FindBytes\>

      \<ReplaceBytes\>C384D27400\</ReplaceBytes\>

    \</BinaryReplace\>

  \</BinaryReplaces\>

\</PackageInfoBinary\>

The info has only one new node -- *BinaryReplaces*. Other is the same as
in info.xml.

## Note: In this type of package you can't use any other info.xml and you can't add new files to dat files.

# How does it work?

Before processing the *File* it is created its backup. The *File* is
then opened and processed. More below.

## Node BinaryReplaces

It can contain as many replaces as you want. It has child nodes named as
*BinaryReplace.*

## Nodes BinaryReplace

It contains *FindBytes* and *ReplaceBytes*.

## Node Files

This defines which files will be processed. Path is relative to the game
install directory. If special parameter *MustExist* is defined and is
set to **false** then it means that the file isn't required, so Mod
Installer will not show error if the file will not exist.

You can set as many files as you want.

## Node FindBytes

Bytes in hexadecimal value, this sequence of bytes will be searched in
the *File*. String can contain spaces between bytes like C3 84 D2 74 1D.

## Node ReplaceBytes

Bytes in hexadecimal value, this sequence of bytes will replace the
sequence defined in *FindBytes*. String can contain spaces between
bytes.

# Using options

You can also use options for each *BinaryReplace* node. See example:

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfoBinary\>

  \<Games\>

\<Game\>FC5\</Game\>

\</Games\>

  \<DefaultInclude\>false\</DefaultInclude\>

  \<Name\>Disable Low FOV\</Name\>

  \<Description\>Disables low FOV during actions like climbing, using the wingsuit, etc\...

  

  \[color=red\]NOTE: This package changes some data in the game DLL file. If you\'ll experience serious bugs or crashes, disable this package.\[/color\]

  

  \[color=red\]\[b\]NOTE: Tested only on game version 1.014\[/b\]\[/color\]\</Description\>

  \<Category\>2\</Category\>

\<Options\>

\<Option ID=\"Fov\" name=\"FOV\"\>

\<option name=\"Enabled\" value=\"1\" /\>

\<option name=\"Disabled\" value=\"0\" /\>

\</Option\>

\</Options\>

  \<BinaryReplaces\>

    \<BinaryReplace\>

\<Option ID=\"Fov\"\>1\</Option\>

\<Files\>

\<File\>bin\\FC_m64.dll\</File\>

\<File MustExist=\"false\"\>bin_plus\\FC_m64.dll\</File\>

\</Files\>

      \<FindBytes\>C384D2741D\</FindBytes\>

      \<ReplaceBytes\>C384D27400\</ReplaceBytes\>

    \</BinaryReplace\>

  \</BinaryReplaces\>

\</PackageInfoBinary\>

As you can see there is an additional *Option* node which defines for
what option the *BinaryReplace* will be processed.

# Example of more BinaryReplace

\<BinaryReplaces\>

    \<BinaryReplace\>

\<Files\>

\<File\>bin\\FC_m64.dll\</File\>

\<File MustExist=\"false\"\>bin_plus\\FC_m64.dll\</File\>

\</Files\>

        \<FindBytes\>7306F30F59DAEB04F30F5ECA\</FindBytes\>

        \<ReplaceBytes\>73060F1F4000EB040F1F4000\</ReplaceBytes\>

    \</BinaryReplace\>

    \<BinaryReplace\>

\<Files\>

\<File\>bin\\FC_m64.dll\</File\>

\<File MustExist=\"false\"\>bin_plus\\FC_m64.dll\</File\>

\</Files\>

        \<FindBytes\>40555356574156488DAC24B0FDFFFF\</FindBytes\>

        \<ReplaceBytes\>B001C356574156488DAC24B0FDFFFF\</ReplaceBytes\>

    \</BinaryReplace\>

\</BinaryReplaces\>
