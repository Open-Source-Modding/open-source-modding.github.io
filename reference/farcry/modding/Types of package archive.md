**Types of package archive**

There are several package archives types. The most known and also the
oldest one is a3 which is a zip archive.

ZIP archive is good as it's natively used in Windows, so no extra tools
are needed for its creation. ZIP is also good for fast extracting and
seeking for needed files inside the archive.

On the other hand, ZIP's compression isn't one of the best, so there was
added support for more the most known archives, like RAR and 7-Zip.

## RAR archive

It is a proprietary archive file format developed by Eugene Roshal. It's
not natively used in Windows, so extra software is needed.

Package which is using RAR as archive format has extension a4.

You can reach a much higher compression compared to the ZIP package. You
must also count with slower processing of the package. It means that
installing the package can take more time.

## 7-Zip archive

It\'s a free and open-source file archive developed by Igor Pavlov. It's
not natively used in Windows, so extra software is needed.

Package which is using 7-Zip as archive format has extension a5.

You can reach a higher compression compared to the ZIP package.

Please note that using 7-Zip isn't recommended for big packages (with
size over 50MB) and packages with a lot of files (more than 100) because
7-Zip archive isn't seekable, so it means all entries must be read till
the needed file is found. Using 7-Zip for big packages can cause loading
and installing the package for several minutes.

Please use 7-Zip for only small packages.

## Replace package

It's not an archive but a direct XML file. This type of package can be
used for only replace packages. It can't have any pictures or any other
files.

Package which is using this has extension a2.

In simple word it is info_replace.xml renamed to MyPackage.a2

It's useful in cases where your package is doing simple replace and you
want to make the whole archive for just one file. It's also faster if
you're making a new package and doing a lot of changes during testing.
