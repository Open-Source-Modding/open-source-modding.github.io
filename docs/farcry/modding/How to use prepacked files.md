**How to use prepacked files**

If your package contains more than 1,000 files, then packing can be very
slow, it's caused by slow selecting files from ZIP archive.

As a solution was created special prepacked files. It's basically a lot
of files packed to a DAT file. The DAT file is then added to the A3
archive.

Prepacked file has two files (dat and xml) with these name rules:

- \<your name\>.prepacked.dat

- \<your name\>.prepacked.xml

**Prepacked can't contain info_replace! It's just for completed files.**

Files are packed directly without processing.

# How to create prepacked file

**In new versions of Mod Installer you will not be able to create
prepacked, you'll need to use the CMD version of Mod Installer.**

Mod Installer has this functionality, you just need to keep these steps:

1. Create a directory with name: for example **myfiles_prepacked**

2. Then keep in this folder same folder structure as in unpacked game
 files or like in alternative package structure, so for example:
 **myfiles_prepacked\\soundbinary\\2066755731.wem**

3. Now we need Mod Installer, it's required to start it with a launch
 param:

ModInstallerCMD.exe \"d:\\modding\\myfiles_prepacked\"

4. Now it will create prepacked file from source directory with name:
 myfile.prepacked.dat and myfile.prepacked.xml

5. Now copy these two files to your package

6. If your package has Pairs, you need to add this:

    \<Pair\>

      \<Source\>myfiles.prepacked.dat\</Source\>

      \<Prepacked\>true\</Prepacked\>

    \</Pair\>

7. If your package uses alternative structure, then you don't need to
 do anything

8. That's all, now you have prepacked file

## Unpacking prepacked file

If you lost your source files of prepacked file, you can unpack existing
prepacked file. Again you need to run Mod Installer with special launch
param:

ModInstallerCMD.exe \"d:\\modding\\myfiles.prepacked.xml\"

Files will be unpacked to new directory located in the same folder as
the DAT file: **d:\\modding\\myfiles_prepacked**
